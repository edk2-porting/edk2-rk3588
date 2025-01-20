/** @file
*
*  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Library/DebugLib.h>
#include <Library/RK806.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/I2c.h>
#include <Protocol/ArmScmi.h>
#include <Protocol/ArmScmiClockProtocol.h>
#include <Protocol/Rk860xRegulator.h>
#include <VarStoreData.h>

#include "RK3588DxeFormSetGuid.h"
#include "CpuPerformance.h"

#define SCMI_CLK_CPUL    0
#define SCMI_CLK_CPUB01  2
#define SCMI_CLK_CPUB23  3

#define FREQ_1_MHZ  1000000

typedef struct {
  UINT64    Hz;
  UINT32    Microvolts;
} OPERATING_PERFORMANCE_POINT;

STATIC CONST OPERATING_PERFORMANCE_POINT  mCPULOppTable[] = {
  { 408000000,  675000 },
  { 600000000,  675000 },
  { 816000000,  675000 },
  { 1008000000, 675000 },
  { 1200000000, 712500 },
  { 1416000000, 762500 },
  { 1608000000, 850000 },
  { 1800000000, 950000 }
};

STATIC CONST OPERATING_PERFORMANCE_POINT  mCPUBOppTable[] = {
  { 408000000,  675000  },
  { 600000000,  675000  },
  { 816000000,  675000  },
  { 1008000000, 675000  },
  { 1200000000, 675000  },
  { 1416000000, 725000  },
  { 1608000000, 762500  },
  { 1800000000, 850000  },
  { 2016000000, 925000  },
  { 2208000000, 987500  },
  { 2256000000, 1000000 },
  { 2304000000, 1000000 },
  { 2352000000, 1000000 },
  { 2400000000, 1000000 }
};

typedef struct {
  UINT32                               ClockId;
  CONST OPERATING_PERFORMANCE_POINT    *Opp;
  UINT32                               OppCount;
} SCMI_OPP_TABLE;

STATIC CONST SCMI_OPP_TABLE  mScmiOppTable[] = {
  { SCMI_CLK_CPUL,   mCPULOppTable, ARRAY_SIZE (mCPULOppTable) },
  { SCMI_CLK_CPUB01, mCPUBOppTable, ARRAY_SIZE (mCPUBOppTable) },
  { SCMI_CLK_CPUB23, mCPUBOppTable, ARRAY_SIZE (mCPUBOppTable) }
};

STATIC
EFI_STATUS
EFIAPI
ScmiSetClockRate (
  IN UINT32  ClockId,
  IN UINT64  Hz
  )
{
  EFI_STATUS           Status;
  SCMI_CLOCK_PROTOCOL  *ClockProtocol;
  EFI_GUID             ClockProtocolGuid = ARM_SCMI_CLOCK_PROTOCOL_GUID;
  UINT64               ClockRate;
  BOOLEAN              Enabled;
  CHAR8                ClockName[SCMI_MAX_STR_LEN];

  Status = gBS->LocateProtocol (
                  &ClockProtocolGuid,
                  NULL,
                  (VOID **)&ClockProtocol
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = ClockProtocol->GetClockAttributes (
                            ClockProtocol,
                            ClockId,
                            &Enabled,
                            ClockName
                            );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = ClockProtocol->RateGet (ClockProtocol, ClockId, &ClockRate);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  DEBUG ((DEBUG_INFO, "SCMI: %a: Current rate is %uHz\n", ClockName, ClockRate));

  ClockRate = Hz;

  DEBUG ((DEBUG_INFO, "SCMI: %a: New rate is %uHz\n", ClockName, ClockRate));

  Status = ClockProtocol->RateSet (
                            ClockProtocol,
                            ClockId,
                            ClockRate
                            );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = ClockProtocol->RateGet (ClockProtocol, ClockId, &ClockRate);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  DEBUG ((DEBUG_INFO, "SCMI: %a: Current rate is %uHz\n", ClockName, ClockRate));

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
ScmiGetClockRate (
  IN  UINT32  ClockId,
  OUT UINT64  *Hz
  )
{
  EFI_STATUS           Status;
  SCMI_CLOCK_PROTOCOL  *ClockProtocol;
  EFI_GUID             ClockProtocolGuid = ARM_SCMI_CLOCK_PROTOCOL_GUID;

  Status = gBS->LocateProtocol (
                  &ClockProtocolGuid,
                  NULL,
                  (VOID **)&ClockProtocol
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return ClockProtocol->RateGet (ClockProtocol, ClockId, Hz);
}

STATIC
EFI_STATUS
EFIAPI
GetOppVoltage (
  IN  CONST OPERATING_PERFORMANCE_POINT  *OppTable,
  IN  UINT32                             OppCount,
  IN  UINT64                             Hz,
  OUT UINT32                             *Microvolts
  )
{
  for (UINTN Index = 0; Index < OppCount; Index++) {
    if (Hz <= OppTable[Index].Hz) {
      *Microvolts = OppTable[Index].Microvolts;
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

STATIC
VOID
EFIAPI
SetRk860xRegulatorByTag (
  IN  UINT32  Tag,
  OUT UINT32  Microvolts
  )
{
  EFI_STATUS                 Status;
  EFI_HANDLE                 *HandleBuffer;
  RK860X_REGULATOR_PROTOCOL  *Rk860xRegulator;
  UINTN                      NumRegulators;
  UINT32                     Index;
  UINT32                     Voltage;
  BOOLEAN                    FoundReg;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gRk860xRegulatorProtocolGuid,
                  NULL,
                  &NumRegulators,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Couldn't locate gRk860xRegulatorProtocolGuid. Status=%r\n", Status));
    return;
  }

  FoundReg = FALSE;
  for (Index = 0; Index < NumRegulators; Index++) {
    Status = gBS->OpenProtocol (
                    HandleBuffer[Index],
                    &gRk860xRegulatorProtocolGuid,
                    (VOID **)&Rk860xRegulator,
                    gImageHandle,
                    NULL,
                    EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
                    );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to open protocol for reg %d. Status=%r\n", Index));
      return;
    }

    if (Rk860xRegulator->Tag != Tag) {
      goto CloseProtocol;
    }

    FoundReg = TRUE;
    DEBUG ((
      DEBUG_INFO,
      "Found 0x%x on I2C bus %d for tag %u\n",
      I2C_DEVICE_ADDRESS (Rk860xRegulator->Identifier),
      I2C_DEVICE_BUS (Rk860xRegulator->Identifier),
      Tag
      ));

    Status = Rk860xRegulator->GetVoltage (Rk860xRegulator, &Voltage, FALSE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to get voltage. Status=%r\n", Status));
      goto CloseProtocol;
    }

    DEBUG ((DEBUG_INFO, "Current voltage: %d uV\n", Voltage));

    Voltage = Microvolts;

    DEBUG ((DEBUG_INFO, "Setting voltage to: %d uV\n", Voltage));
    Status = Rk860xRegulator->SetVoltage (Rk860xRegulator, Voltage, FALSE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to set voltage. Status=%r\n", Status));
      goto CloseProtocol;
    }

    Status = Rk860xRegulator->GetVoltage (Rk860xRegulator, &Voltage, FALSE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to get voltage. Status=%r\n", Status));
      goto CloseProtocol;
    }

    DEBUG ((DEBUG_INFO, "Current voltage: %d uV\n", Voltage));

CloseProtocol:
    gBS->CloseProtocol (
           HandleBuffer[Index],
           &gRk860xRegulatorProtocolGuid,
           gImageHandle,
           NULL
           );

    if (FoundReg) {
      break;
    }
  }
}

STATIC
VOID
EFIAPI
SetCpuVoltage (
  IN  UINT32  ClockId,
  OUT UINT32  Microvolts
  )
{
  if (Microvolts > CLUSTER_MICROVOLTS_MAX) {
    Microvolts = CLUSTER_MICROVOLTS_MAX;
  }

  if (Microvolts < CLUSTER_MICROVOLTS_MIN) {
    Microvolts = CLUSTER_MICROVOLTS_MIN;
  }

  if (ClockId == SCMI_CLK_CPUL) {
    SetCPULittleVoltage (Microvolts);
  } else {
    SetRk860xRegulatorByTag (ClockId, Microvolts);
  }
}

VOID
EFIAPI
ApplyCpuClockVariables (
  VOID
  )
{
  EFI_STATUS      Status;
  UINT32          Index;
  UINT64          ClockRate;
  SCMI_OPP_TABLE  ScmiOppTable;
  UINT32          CPUClusterClockPreset[] = {
    PcdGet32 (PcdCPULClusterClockPreset),
    PcdGet32 (PcdCPUB01ClusterClockPreset),
    PcdGet32 (PcdCPUB23ClusterClockPreset)
  };
  UINT32          CPUClusterClockCustom[] = {
    PcdGet32 (PcdCPULClusterClockCustom),
    PcdGet32 (PcdCPUB01ClusterClockCustom),
    PcdGet32 (PcdCPUB23ClusterClockCustom)
  };

  for (Index = 0; Index < ARRAY_SIZE (CPUClusterClockPreset); Index++) {
    ScmiOppTable = mScmiOppTable[Index];

    switch (CPUClusterClockPreset[Index]) {
      case CPU_PERF_CLUSTER_CLOCK_PRESET_MIN:
        ClockRate = ScmiOppTable.Opp[0].Hz;
        break;
      case CPU_PERF_CLUSTER_CLOCK_PRESET_MAX:
        ClockRate = ScmiOppTable.Opp[ScmiOppTable.OppCount - 1].Hz;
        break;
      case CPU_PERF_CLUSTER_CLOCK_PRESET_CUSTOM:
        ClockRate = CPUClusterClockCustom[Index] * FREQ_1_MHZ;
        break;
      default:
        continue;
    }

    Status = ScmiSetClockRate (ScmiOppTable.ClockId, ClockRate);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "%a: ScmiSetClockRate failed. Status=%r\n", __FUNCTION__, Status));
    }
  }
}

VOID
EFIAPI
ApplyCpuVoltageVariables (
  VOID
  )
{
  EFI_STATUS      Status;
  UINT64          ClockRate;
  UINT32          Index;
  SCMI_OPP_TABLE  ScmiOppTable;
  UINT32          Microvolts;
  UINT32          CPUClusterVoltageMode[] = {
    PcdGet32 (PcdCPULClusterVoltageMode),
    PcdGet32 (PcdCPUB01ClusterVoltageMode),
    PcdGet32 (PcdCPUB23ClusterVoltageMode)
  };
  UINT32          CPUClusterVoltageCustom[] = {
    PcdGet32 (PcdCPULClusterVoltageCustom),
    PcdGet32 (PcdCPUB01ClusterVoltageCustom),
    PcdGet32 (PcdCPUB23ClusterVoltageCustom)
  };

  for (Index = 0; Index < ARRAY_SIZE (CPUClusterVoltageMode); Index++) {
    ScmiOppTable = mScmiOppTable[Index];

    switch (CPUClusterVoltageMode[Index]) {
      case CPU_PERF_CLUSTER_VOLTAGE_MODE_AUTO:
        Status = ScmiGetClockRate (ScmiOppTable.ClockId, &ClockRate);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "%a: ScmiGetClockRate failed. Status=%r\n", __FUNCTION__, Status));
          continue;
        }

        Status = GetOppVoltage (
                   ScmiOppTable.Opp,
                   ScmiOppTable.OppCount,
                   ClockRate,
                   &Microvolts
                   );
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "%a: GetOppVoltage failed. Status=%r\n", __FUNCTION__, Status));
          continue;
        }

        break;
      case CPU_PERF_CLUSTER_VOLTAGE_MODE_CUSTOM:
        Microvolts = CPUClusterVoltageCustom[Index];
        break;
      default:
        continue;
    }

    SetCpuVoltage (ScmiOppTable.ClockId, Microvolts);
  }
}

VOID
EFIAPI
SetupCpuPerfVariables (
  VOID
  )
{
  UINTN       Size;
  UINT32      Var32;
  EFI_STATUS  Status;

  Size   = sizeof (UINT32);
  Status = gRT->GetVariable (
                  L"CpuPerf_CPULClusterClockPreset",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdCPULClusterClockPreset, PcdGet32 (PcdCPULClusterClockPresetDefault));
    ASSERT_EFI_ERROR (Status);
  }

  Size   = sizeof (UINT32);
  Status = gRT->GetVariable (
                  L"CpuPerf_CPULClusterClockCustom",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdCPULClusterClockCustom, PcdGet32 (PcdCPULClusterClockCustom));
    ASSERT_EFI_ERROR (Status);
  }

  Size   = sizeof (UINT32);
  Status = gRT->GetVariable (
                  L"CpuPerf_CPUB01ClusterClockPreset",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdCPUB01ClusterClockPreset, PcdGet32 (PcdCPUB01ClusterClockPresetDefault));
    ASSERT_EFI_ERROR (Status);
  }

  Size   = sizeof (UINT32);
  Status = gRT->GetVariable (
                  L"CpuPerf_CPUB01ClusterClockCustom",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdCPUB01ClusterClockCustom, PcdGet32 (PcdCPUB01ClusterClockCustom));
    ASSERT_EFI_ERROR (Status);
  }

  Size   = sizeof (UINT32);
  Status = gRT->GetVariable (
                  L"CpuPerf_CPUB23ClusterClockPreset",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdCPUB23ClusterClockPreset, PcdGet32 (PcdCPUB23ClusterClockPresetDefault));
    ASSERT_EFI_ERROR (Status);
  }

  Size   = sizeof (UINT32);
  Status = gRT->GetVariable (
                  L"CpuPerf_CPUB23ClusterClockCustom",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdCPUB23ClusterClockCustom, PcdGet32 (PcdCPUB23ClusterClockCustom));
    ASSERT_EFI_ERROR (Status);
  }

  Size   = sizeof (UINT32);
  Status = gRT->GetVariable (
                  L"CpuPerf_CPULClusterVoltageMode",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdCPULClusterVoltageMode, PcdGet32 (PcdCPULClusterVoltageMode));
    ASSERT_EFI_ERROR (Status);
  }

  Size   = sizeof (UINT32);
  Status = gRT->GetVariable (
                  L"CpuPerf_CPULClusterVoltageCustom",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdCPULClusterVoltageCustom, PcdGet32 (PcdCPULClusterVoltageCustom));
    ASSERT_EFI_ERROR (Status);
  }

  Size   = sizeof (UINT32);
  Status = gRT->GetVariable (
                  L"CpuPerf_CPUB01ClusterVoltageMode",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdCPUB01ClusterVoltageMode, PcdGet32 (PcdCPUB01ClusterVoltageMode));
    ASSERT_EFI_ERROR (Status);
  }

  Size   = sizeof (UINT32);
  Status = gRT->GetVariable (
                  L"CpuPerf_CPUB01ClusterVoltageCustom",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdCPUB01ClusterVoltageCustom, PcdGet32 (PcdCPUB01ClusterVoltageCustom));
    ASSERT_EFI_ERROR (Status);
  }

  Size   = sizeof (UINT32);
  Status = gRT->GetVariable (
                  L"CpuPerf_CPUB23ClusterVoltageMode",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdCPUB23ClusterVoltageMode, PcdGet32 (PcdCPUB23ClusterVoltageMode));
    ASSERT_EFI_ERROR (Status);
  }

  Size   = sizeof (UINT32);
  Status = gRT->GetVariable (
                  L"CpuPerf_CPUB23ClusterVoltageCustom",
                  &gRK3588DxeFormSetGuid,
                  NULL,
                  &Size,
                  &Var32
                  );
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdCPUB23ClusterVoltageCustom, PcdGet32 (PcdCPUB23ClusterVoltageCustom));
    ASSERT_EFI_ERROR (Status);
  }
}
