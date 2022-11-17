/** @file
  PCH private PMC Library for all PCH generations.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PmcLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Library/TimerLib.h>
#include <Private/Library/PmcPrivateLib.h>
#include <Private/Library/PchPsfPrivateLib.h>
#include <PchReservedResources.h>
#include <Register/PchRegs.h>
#include <Register/PchRegsPmc.h>
#include <Register/PchRegsItss.h>
#include <IndustryStandard/Pci30.h>

#include "PmcPrivateLibInternal.h"

/**
  Send PMC IPC1 Normal Read/Write command

  @param[in]  Command           Command to be issued to PMC IPC 1 interface
  @param[in]  SubCmdId          SUB_CMD_ID for provided Command
  @param[in]  CmdSize           Total size in byte to be sent via PMC IPC 1 interface
  @param[in]  WriteBufPtr       Pointer to Structure of 4 DWORDs to be issued to PMC IPC 1 interface
  @param[out] ReadBufPtr        Pointer to Structure of 4 DWORDs to be filled by PMC IPC 1 interface

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_INVALID_PARAMETER   Invalid command size
  @retval EFI_DEVICE_ERROR        IPC command failed with an error
  @retval EFI_TIMEOUT             IPC command did not complete after 1s
**/
EFI_STATUS
PmcSendCommand (
  IN  UINT8                    Command,
  IN  UINT8                    SubCmdId,
  IN  UINT8                    CmdSize,
  IN  PMC_IPC_COMMAND_BUFFER   *WriteBufPtr,
  OUT PMC_IPC_COMMAND_BUFFER   *ReadBufPtr
  )
{
  EFI_STATUS             Status;
  UINT32                 PchPwrmBase;
  UINT32                 IpcSts;
  UINTN                  Timeout;

  DEBUG ((DEBUG_INFO, "PmcSendCommand(): IPC_COMMAND=0x%02X, IPC_SUB_CMD = 0x%02X, IPC_SIZE=0x%02X \n", Command, SubCmdId, CmdSize));
  DEBUG ((DEBUG_INFO, "WBUF0=0x%08X, WBUF1=0x%08X, WBUF2=0x%08X, WBUF3=0x%08X \n", WriteBufPtr->Buf0, WriteBufPtr->Buf1, WriteBufPtr->Buf2, WriteBufPtr->Buf3));

  if (CmdSize > 16) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Program the Write Buffer 0 with the Data that needs to be written to PMC
  //
  PchPwrmBase = PmcGetPwrmBase ();
  MmioWrite32 ((PchPwrmBase + R_PMC_PWRM_IPC_WBUF0), WriteBufPtr->Buf0);
  MmioWrite32 ((PchPwrmBase + R_PMC_PWRM_IPC_WBUF1), WriteBufPtr->Buf1);
  MmioWrite32 ((PchPwrmBase + R_PMC_PWRM_IPC_WBUF2), WriteBufPtr->Buf2);
  MmioWrite32 ((PchPwrmBase + R_PMC_PWRM_IPC_WBUF3), WriteBufPtr->Buf3);
  //
  // Program the command register with command and size
  //
  MmioWrite32 (
    PchPwrmBase + R_PMC_PWRM_IPC_CMD,
    (UINT32) ((CmdSize << N_PMC_PWRM_IPC_CMD_SIZE) |
      (SubCmdId << N_PMC_PWRM_IPC_CMD_CMD_ID) |
      (Command << N_PMC_PWRM_IPC_CMD_COMMAND))
    );

  //
  // Read the IPC_STS register to get BUSY or Error status
  //
  Timeout = 0;
  Status = EFI_SUCCESS;
  while (TRUE) {
    IpcSts = MmioRead32 (PchPwrmBase + R_PMC_PWRM_IPC_STS);
    if ((IpcSts & B_PMC_PWRM_IPC_STS_BUSY) == 0) {
      break;
    }

    if (Timeout > (1000 * 100)) {
      Status = EFI_TIMEOUT;
      break;
    }
    MicroSecondDelay (10);
    Timeout++;
  }

  if ((IpcSts & B_PMC_PWRM_IPC_STS_ERROR) != 0) {
    Status = EFI_DEVICE_ERROR;
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PmcSendCommand() Error: IPC_STS=0x%08X\n", IpcSts));
    return Status;
  }

  if (ReadBufPtr != NULL) {
    //
    // Fill the  ReadBuffer contents with the Data that needs to be read from PMC
    //
    ReadBufPtr->Buf0 = MmioRead32(PchPwrmBase + R_PMC_PWRM_IPC_RBUF0);
    ReadBufPtr->Buf1 = MmioRead32(PchPwrmBase + R_PMC_PWRM_IPC_RBUF1);
    ReadBufPtr->Buf2 = MmioRead32(PchPwrmBase + R_PMC_PWRM_IPC_RBUF2);
    ReadBufPtr->Buf3 = MmioRead32(PchPwrmBase + R_PMC_PWRM_IPC_RBUF3);

    DEBUG ((DEBUG_INFO, "RBUF0=0x%08X, RBUF1=0x%08X, RBUF2=0x%08X, RBUF3=0x%08X \n", ReadBufPtr->Buf0, ReadBufPtr->Buf1, ReadBufPtr->Buf2, ReadBufPtr->Buf3));
  }

  return Status;
}

/**
  This function checks if SCI interrupt is enabled

  @retval SCI Enable state
**/
BOOLEAN
PmcIsSciEnabled (
  VOID
  )
{
  return ((IoRead8 (PmcGetAcpiBase () + R_ACPI_IO_PM1_CNT) & B_ACPI_IO_PM1_CNT_SCI_EN) != 0);
}

/**
  This function triggers Software GPE
**/
VOID
PmcTriggerSwGpe (
  VOID
  )
{
  IoOr32 (PmcGetAcpiBase () + R_ACPI_IO_GPE_CNTL, B_ACPI_IO_GPE_CNTL_SWGPE_CTRL);
}

/**
  Set PCH ACPI base address.
  The Address should not be 0 and should be 256 bytes aligned. It is IO space, so must not exceed 0xFFFF.
  Only address matching PcdAcpiBaseAddress is the acceptable value for ACPI IO Base

  @param[in] Address                    Address for ACPI base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PmcSetAcpiBase (
  IN  UINT16                            Address
  )
{

  if (Address != PcdGet16 (PcdAcpiBaseAddress)) {
    DEBUG ((DEBUG_ERROR, "PmcSetAcpiBase Error. Invalid Address: %x.\n", Address));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  PsfSetPmcAbase (Address);
  return EFI_SUCCESS;
}

/**
  Set PCH PWRM base address.
  Only 0xFE000000 (PCH_PWRM_BASE_ADDRESS) is the acceptable value for PWRMBASE

  @param[in] Address                    Address for PWRM base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PmcSetPwrmBase (
  IN  UINT32                            Address
  )
{
  UINT64                                PmcBase;

  if (Address != PCH_PWRM_BASE_ADDRESS) {
    DEBUG ((DEBUG_ERROR, "PmcSetPwrmBase Error. Invalid Address: %x.\n", Address));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  PmcBase      = PCI_SEGMENT_LIB_ADDRESS (
                   DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                   DEFAULT_PCI_BUS_NUMBER_PCH,
                   PCI_DEVICE_NUMBER_PCH_PMC,
                   PCI_FUNCTION_NUMBER_PCH_PMC,
                   0
                   );
  if (PciSegmentRead16 (PmcBase) == 0xFFFF) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  //
  // Disable PWRMBASE in PMC Device first before changing PWRM base address.
  //
  PciSegmentAnd16 (
    PmcBase + PCI_COMMAND_OFFSET,
    (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE
    );

  //
  // Program PWRMBASE in PMC Device
  //
  PciSegmentAndThenOr32 (
    PmcBase + R_PMC_CFG_BASE,
    (UINT32) (~B_PMC_CFG_PWRM_BASE_MASK),
    Address
    );

  //
  // Enable PWRMBASE in PMC Device
  //
  PciSegmentOr16 (
    PmcBase + PCI_COMMAND_OFFSET,
    EFI_PCI_COMMAND_MEMORY_SPACE
    );
  return EFI_SUCCESS;
}

/**
  This function checks if function disable (static and non-static power gating)
  configuration is locked

  @retval lock state
**/
BOOLEAN
PmcIsFunctionDisableConfigLocked (
  VOID
  )
{
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_ST_PG_FDIS_PMC_1) & B_PMC_PWRM_ST_PG_FDIS_PMC_1_ST_FDIS_LK) != 0);
}

/**
  Check if MODPHY SUS PG is supported

  @retval  Status of MODPHY SUS PG support
**/
BOOLEAN
PmcIsModPhySusPgSupported (
  VOID
  )
{
  if (IsPchLp ()) {
    //
    // MPHY SUS PG supported on PCH-LP only:
    //
    return TRUE;
  }
  return FALSE;
}

/**
  This function checks if ISH is function disabled
  by static power gating

  @retval ISH device state
**/
BOOLEAN
PmcIsIshFunctionDisabled (
  VOID
  )
{
  //
  // Get PG info from PWRMBASE + ST_PG_FDIS_PMC_1
  //
  return ((MmioRead32 ((UINTN) (PmcGetPwrmBase () + R_PMC_PWRM_ST_PG_FDIS_PMC_1)) & B_PMC_PWRM_ST_PG_FDIS_PMC_1_ISH_FDIS_PMC) != 0);
}

/**
  This function checks if ISH device is supported (not disabled by fuse)

  @retval ISH support state
**/
BOOLEAN
PmcIsIshSupported (
  VOID
  )
{
  //
  // Get fuse info from PWRMBASE + FUSE_DIS_RD_2
  //
  return ((MmioRead32 ((UINTN) (PmcGetPwrmBase () + R_PMC_PWRM_FUSE_DIS_RD_2)) & B_PMC_PWRM_FUSE_DIS_RD_2_ISH_FUSE_SS_DIS) == 0);
}

/**
  This function disables ISH device by static power gating.
  For static power gating to take place Global Reset, G3 or DeepSx transition must happen.
**/
VOID
PmcStaticDisableIsh (
  VOID
  )
{
  //
  // Set PWRMBASE + ST_PG_FDIS_PMC_1[5] = 1b to statically disable ISH Controller
  //
  MmioOr32 (PmcGetPwrmBase () + R_PMC_PWRM_ST_PG_FDIS_PMC_1, B_PMC_PWRM_ST_PG_FDIS_PMC_1_ISH_FDIS_PMC);
}

/**
  This function enables ISH device by disabling static power gating.
  Static power gating disabling takes place after Global Reset, G3 or DeepSx transition.
**/
VOID
PmcEnableIsh (
  VOID
  )
{
  //
  // Set PWRMBASE + ST_PG_FDIS_PMC_1[5] = 0b to enable ISH controller
  //
  MmioAnd32 (PmcGetPwrmBase () + R_PMC_PWRM_ST_PG_FDIS_PMC_1, (UINT32) (~B_PMC_PWRM_ST_PG_FDIS_PMC_1_ISH_FDIS_PMC));
}

/**
  This function checks if GbE is function disabled
  by static power gating

  @retval GbE device state
**/
BOOLEAN
PmcIsGbeFunctionDisabled (
  VOID
  )
{
  //
  // Get PG info from PWRMBASE + ST_PG_FDIS_PMC_1
  //
  return ((MmioRead32 ((UINTN) (PmcGetPwrmBase () + R_PMC_PWRM_ST_PG_FDIS_PMC_1)) & B_PMC_PWRM_ST_PG_FDIS_PMC_1_GBE_FDIS_PMC) != 0);
}

/**
  This function disables GbE device by static power gating and enables ModPHY SPD gating (PCH-LP only).
  For static power gating to take place Global Reset, G3 or DeepSx transition must happen.
**/
VOID
PmcStaticDisableGbe (
  VOID
  )
{
  UINT32 PchPwrmBase;
  PchPwrmBase = PmcGetPwrmBase ();
  //
  // Set PWRMBASE + ST_PG_FDIS_PMC_1[0] = 1b to statically disable GbE Controller
  //
  MmioOr32 (PchPwrmBase + R_PMC_PWRM_ST_PG_FDIS_PMC_1, B_PMC_PWRM_ST_PG_FDIS_PMC_1_GBE_FDIS_PMC);

  if (PmcIsModPhySusPgSupported ()) {
    //
    // Set MSPDRTREQ:
    // PWRMBASE + R_PWRM_MODPHY_PM_CFG5[13] = 1 to enable ASL code trigger request for ModPHY SPD gating.
    //
    PmcGbeModPhyPowerGating ();
  }
}

/**
  This function enables GbE device by disabling static power gating.
  Static power gating disabling takes place after Global Reset, G3 or DeepSx transition.
**/
VOID
PmcEnableGbe (
  VOID
  )
{
  //
  // Set PWRMBASE + ST_PG_FDIS_PMC_1[0] = 0b to enable GbE controller
  //
  MmioAnd32 (PmcGetPwrmBase () + R_PMC_PWRM_ST_PG_FDIS_PMC_1, (UINT32) ~B_PMC_PWRM_ST_PG_FDIS_PMC_1_GBE_FDIS_PMC);
}

/**
  This function checks if GbE device is supported (not disabled by fuse)

  @retval GbE support state
**/
BOOLEAN
PmcIsGbeSupported (
  VOID
  )
{
  //
  // Get fuse info from PWRMBASE + FUSE_SS_DIS_RD_2
  //
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_FUSE_DIS_RD_2) & B_PMC_PWRM_FUSE_DIS_RD_2_GBE_FUSE_SS_DIS) == 0);
}

/**
  This function disables (non-static power gating) HDA device
**/
VOID
PmcDisableHda (
  VOID
  )
{
  MmioOr32 (PmcGetPwrmBase () + R_PMC_PWRM_NST_PG_FDIS_1, B_PMC_PWRM_NST_PG_FDIS_1_ADSP_FDIS_PMC);
}

/**
  This function checks if Cnvi device is supported (not disabled by fuse)

  @retval Cnvi support state
**/
BOOLEAN
PmcIsCnviSupported (
  VOID
  )
{
  //
  // Get fuse info from PWRMBASE + FUSE_SS_DIS_RD_2
  //
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_FUSE_DIS_RD_2) & B_PMC_PWRM_FUSE_DIS_RD_2_CNVI_FUSE_SS_DIS) == 0);
}

/**
  This function checks if CNVi is function disabled
  by static power gating

  @retval GbE device state
**/
BOOLEAN
PmcIsCnviFunctionDisabled (
  VOID
  )
{
  //
  // Get PG info from PWRMBASE + ST_PG_FDIS_PMC_1
  //
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_ST_PG_FDIS_PMC_1) & B_PMC_PWRM_ST_PG_FDIS_PMC_1_CNVI_FDIS_PMC) != 0);
}

/**
  This function enables CNVi device by disabling static power gating.
  Static power gating disabling takes place after Global Reset, G3 or DeepSx transition.
**/
VOID
PmcEnableCnvi (
  VOID
  )
{
  //
  // Set PWRMBASE + ST_PG_FDIS_PMC_1 to enable CNVi controller
  //
  MmioAnd32 (PmcGetPwrmBase () + R_PMC_PWRM_ST_PG_FDIS_PMC_1, (UINT32) ~B_PMC_PWRM_ST_PG_FDIS_PMC_1_CNVI_FDIS_PMC);
}

/**
  This function disables CNVi device by static power gating.
  For static power gating to take place Global Reset, G3 or DeepSx transition must happen.
**/
VOID
PmcStaticDisableCnvi (
  VOID
  )
{
  MmioOr32 (PmcGetPwrmBase () + R_PMC_PWRM_ST_PG_FDIS_PMC_1, B_PMC_PWRM_ST_PG_FDIS_PMC_1_CNVI_FDIS_PMC);
}

/**
  This function disables (non-static power gating) PCIe Root Port and enables ModPHY SPD gating (PCH-LP only).

  @param[in] RpIndex        PCIe Root Port Index (0 based)
**/
VOID
PmcDisablePcieRootPort (
  IN UINT32  RpIndex
  )
{
  UINT32   NstPgFdis1Mask;
  UINT32   PchPwrmBase;

  PchPwrmBase = PmcGetPwrmBase ();
  //
  // Set PchPwrmBase + NST_PG_FDIS_1 to function disable PCIE port in PMC
  //
  if (IsPchH () && RpIndex >= 20) {
    NstPgFdis1Mask = B_PCH_H_PMC_PWRM_NST_PG_FDIS_1_PCIE_F0_FDIS_PMC << (RpIndex % 20);
  } else {
    NstPgFdis1Mask = B_PMC_PWRM_NST_PG_FDIS_1_PCIE_A0_FDIS_PMC << RpIndex;
  }
  MmioOr32 (PchPwrmBase + R_PMC_PWRM_NST_PG_FDIS_1, NstPgFdis1Mask);

  if (PmcIsModPhySusPgSupported ()) {
    //
    // Set MSPDRTREQ:
    // PWRMBASE + R_PWRM_MODPHY_PM_CFG5[26:0] = 1 to enable ASL code trigger request for ModPHY SPD gating.
    //
    if (RpIndex <= 11) {
      MmioOr32 (PchPwrmBase + R_PMC_PWRM_MODPHY_PM_CFG5, B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_A0 << RpIndex);
    } else {
      MmioOr32 (PchPwrmBase + R_PMC_PWRM_MODPHY_PM_CFG5, B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_D0 << (RpIndex - 12));
    }
  }
}

/**
  This function disables (non-static power gating) xHCI and enables ModPHY SPD gating (PCH-LP only).
**/
VOID
PmcDisableXhci (
  VOID
  )
{
  UINT32   PchPwrmBase;
  PchPwrmBase = PmcGetPwrmBase ();

  //
  // Set PWRMBASE + NST_PG_FDIS_1 [0] = 1b
  //
  MmioOr32 (PchPwrmBase + R_PMC_PWRM_NST_PG_FDIS_1, B_PMC_PWRM_NST_PG_FDIS_1_XHCI_FDIS_PMC);

  if (PmcIsModPhySusPgSupported ()) {
    //
    // Set MSPDRTREQ:
    // PchPwrmBase + R_PWRM_MODPHY_PM_CFG5[14] = 1 to enable ASL code trigger request for ModPHY SPD gating.
    //
    MmioOr32 (PchPwrmBase + R_PMC_PWRM_MODPHY_PM_CFG5, B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_XHCI);
  }
}

/**
  This function disables (non-static power gating) XDCI and enables ModPHY SPD gating (PCH-LP only).
**/
VOID
PmcDisableXdci (
  VOID
  )
{
  UINT32 PchPwrmBase;
  PchPwrmBase = PmcGetPwrmBase ();

  //
  // Set PWRMBASE + NST_PG_FDIS_1 [26] = 1b to disable XDCI Controller in PMC
  //
  MmioOr32 (PchPwrmBase + R_PMC_PWRM_NST_PG_FDIS_1, B_PMC_PWRM_NST_PG_FDIS_1_XDCI_FDIS_PMC);

  if (PmcIsModPhySusPgSupported ()) {
    //
    // Set MSPDRTREQ:
    // PWRMBASE + R_PWRM_MODPHY_PM_CFG5[15] = 1 to enable ASL code trigger request for ModPHY SPD gating.
    //
    MmioOr32 (PchPwrmBase + R_PMC_PWRM_MODPHY_PM_CFG5, B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_XDCI);
  }
}

/**
  This function checks if XDCI device is supported (not disabled by fuse)

  @retval XDCI support state
**/
BOOLEAN
PmcIsXdciSupported (
  VOID
  )
{
  //
  // Get fuse info from PWRMBASE + FUSE_SS_DIS_RD_2
  //
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_FUSE_DIS_RD_2) & B_PMC_PWRM_FUSE_DIS_RD_2_OTG_FUSE_SS_DIS) == 0);
}

/**
  This function locks HOST SW power gating control
**/
VOID
PmcLockHostSwPgCtrl (
  VOID
  )
{
  MmioOr32 (PmcGetPwrmBase () + R_PMC_PWRM_HSWPGCR1, B_PMC_PWRM_SW_PG_CTRL_LOCK);
}

/**
  This function checks if HOST SW Power Gating Control is locked

  @retval lock state
**/
BOOLEAN
PmcIsHostSwPgCtrlLocked (
  VOID
  )
{
  //
  // Get lock info from PWRMBASE + HSWPGCR1
  //
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_HSWPGCR1) & B_PMC_PWRM_SW_PG_CTRL_LOCK) != 0);
}

/**
  This function checks if LAN wake from DeepSx is enabled

  @retval Lan Wake state
**/
BOOLEAN
PmcIsLanDeepSxWakeEnabled (
  VOID
  )
{
  //
  // Get wake info from PWRMBASE + DSX_CFG
  //
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_DSX_CFG) & (UINT32) B_PMC_PWRM_DSX_CFG_LAN_WAKE_EN) != 0);
}

/**
  Disables USB2 Core PHY PowerGating during power on for Chipsetinit table update
**/
VOID
PmcUsb2CorePhyPowerGatingDisable (
  VOID
  )
{
  MmioAnd32 (PmcGetPwrmBase () + R_PMC_PWRM_CFG, (UINT32) ~B_PMC_PWRM_CFG_ALLOW_USB2_CORE_PG);
}


/**
  This function reads CPU Early Power-on Configuration (EPOC)

  @retval CPU EPOC value
**/
UINT32
PmcGetCpuEpoc (
  VOID
  )
{
  return MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_CPU_EPOC);
}

/**
  This function sets CPU Early Power-on Configuration (EPOC)

  @param[in] CpuEpocValue      CPU EPOC value
**/
VOID
PmcSetCpuEpoc (
  IN UINT32     CpuEpocValue
  )
{
  MmioWrite32 (PmcGetPwrmBase () + R_PMC_PWRM_CPU_EPOC, CpuEpocValue);
}

/**
  This function sets DRAM_RESET# Control Pin value

  @param[in] DramResetVal      0: Pin output is low
                               1: Pin output is tri-stated
**/
VOID
PmcSetDramResetCtlState (
  IN UINT32     DramResetVal
  )
{
  ASSERT (DramResetVal < 2);

  MmioAndThenOr32 (
    PmcGetPwrmBase () + R_PMC_PWRM_CFG2,
    (UINT32)~B_PMC_PWRM_CFG2_DRAM_RESET_CTL,
    DramResetVal << N_PMC_PWRM_CFG2_DRAM_RESET_CTL
    );
}

/**
  This function enables triggering Global Reset of both
  the Host and the ME partitions after CF9h write of 6h or Eh
**/
VOID
PmcEnableCf9GlobalReset (
  VOID
  )
{
  MmioOr32 (PmcGetPwrmBase () + R_PMC_PWRM_ETR3, (UINT32) (B_PMC_PWRM_ETR3_CF9GR));
}

/**
  This function disables triggering Global Reset of both
  the Host and the ME partitions after CF9h write of 6h or Eh.
**/
VOID
PmcDisableCf9GlobalReset (
  VOID
  )
{
  MmioAnd32 (PmcGetPwrmBase () + R_PMC_PWRM_ETR3, (UINT32) ~B_PMC_PWRM_ETR3_CF9GR);
}

/**
  This function disables triggering Global Reset of both
  the Host and the ME partitions after CF9h write of 6h or Eh.
  Global Reset configuration is locked after programming
**/
VOID
PmcDisableCf9GlobalResetWithLock (
  VOID
  )
{
  MmioAndThenOr32 (
    PmcGetPwrmBase () + R_PMC_PWRM_ETR3,
    (UINT32) ~B_PMC_PWRM_ETR3_CF9GR,
    (UINT32) B_PMC_PWRM_ETR3_CF9LOCK
    );
}

/**
  This function disables CF9 reset without Resume Well reset.
  Cf9 0x6/0xE reset will also reset resume well logic.
**/
VOID
PmcDisableCf9ResetWithoutResumeWell (
  VOID
  )
{

  MmioAnd32 (PmcGetPwrmBase () + R_PMC_PWRM_ETR3, (UINT32) ~B_PMC_PWRM_ETR3_CWORWRE);
}

/**
  This function clears RTC Power Failure status (RTC_PWR_FLR)
**/
VOID
PmcClearRtcPowerFailureStatus (
  VOID
  )
{
  //
  // Set B_PMC_PWRM_GEN_PMCON_B_RTC_PWR_STS to 0 to clear it.
  //
  MmioAnd8 ((UINTN) (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_B), (UINT8) (~B_PMC_PWRM_GEN_PMCON_B_RTC_PWR_STS));
}

/**
  This function enables PCI Express* PME events
**/
VOID
PmcEnablePciExpressPmeEvents (
  VOID
  )
{
  MmioOr32 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_B, B_PMC_PWRM_GEN_PMCON_B_BIOS_PCI_EXP_EN);
}

/**
  This function sets eSPI SMI Lock
**/
VOID
PmcLockEspiSmi (
  VOID
  )
{
  MmioAndThenOr8 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A + 1,
    (UINT8) ~((B_PMC_PWRM_GEN_PMCON_A_PWR_FLR | B_PMC_PWRM_GEN_PMCON_A_HOST_RST_STS) >> 8),
    B_PMC_PWRM_GEN_PMCON_A_ESPI_SMI_LOCK >> 8
    );
}

/**
  This function checks if eSPI SMI Lock is set

  @retval eSPI SMI Lock state
**/
BOOLEAN
PmcIsEspiSmiLockSet (
  VOID
  )
{
  return ((MmioRead32 ((UINTN) (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A)) & B_PMC_PWRM_GEN_PMCON_A_ESPI_SMI_LOCK) != 0);
}

/**
  This function sets SW SMI Rate.

  @param[in] SwSmiRate        Refer to PMC_SWSMI_RATE for possible values
**/
VOID
PmcSetSwSmiRate (
  IN PMC_SWSMI_RATE          SwSmiRate
  )
{
  UINT32        PchPwrmBase;
  STATIC UINT8  SwSmiRateRegVal[4] = {
    V_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL_1_5MS,
    V_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL_16MS,
    V_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL_32MS,
    V_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL_64MS
  };

  ASSERT (SwSmiRate <= PmcSwSmiRate64ms);

  PchPwrmBase = PmcGetPwrmBase ();

  //
  // SWSMI_RATE_SEL BIT (PWRMBASE offset 1020h[7:6]) bits are in RTC well
  //
  MmioAndThenOr8 (
    PchPwrmBase + R_PMC_PWRM_GEN_PMCON_A,
    (UINT8)~B_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL,
    SwSmiRateRegVal[SwSmiRate]
    );
}

/**
  This function sets Periodic SMI Rate.

  @param[in] PeriodicSmiRate        Refer to PMC_PERIODIC_SMI_RATE for possible values
**/
VOID
PmcSetPeriodicSmiRate (
  IN PMC_PERIODIC_SMI_RATE    PeriodicSmiRate
  )
{
  UINT32        PchPwrmBase;
  STATIC UINT8  PeriodicSmiRateRegVal[4] = {
    V_PMC_PWRM_GEN_PMCON_A_PER_SMI_8S,
    V_PMC_PWRM_GEN_PMCON_A_PER_SMI_16S,
    V_PMC_PWRM_GEN_PMCON_A_PER_SMI_32S,
    V_PMC_PWRM_GEN_PMCON_A_PER_SMI_64S
  };

  ASSERT (PeriodicSmiRate <= PmcPeriodicSmiRate64s);

  PchPwrmBase = PmcGetPwrmBase ();

  MmioAndThenOr8 (
    PchPwrmBase + R_PMC_PWRM_GEN_PMCON_A,
    (UINT8)~B_PMC_PWRM_GEN_PMCON_A_PER_SMI_SEL,
    PeriodicSmiRateRegVal[PeriodicSmiRate]
    );
}

/**
  This function reads Power Button Level

  @retval State of PWRBTN# signal (0: Low, 1: High)
**/
UINT8
PmcGetPwrBtnLevel (
  VOID
  )
{
  if (MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_B) & B_PMC_PWRM_GEN_PMCON_B_PWRBTN_LVL) {
    return 1;
  } else {
    return 0;
  }
}

/**
  This function gets Group to GPE0 configuration

  @param[out] GpeDw0Value       GPIO Group to GPE_DW0 assignment
  @param[out] GpeDw1Value       GPIO Group to GPE_DW1 assignment
  @param[out] GpeDw2Value       GPIO Group to GPE_DW2 assignment
**/
VOID
PmcGetGpioGpe (
  OUT UINT32    *GpeDw0Value,
  OUT UINT32    *GpeDw1Value,
  OUT UINT32    *GpeDw2Value
  )
{
  UINT32 Data32;

  Data32 = MmioRead32 ((UINTN) (PmcGetPwrmBase () + R_PMC_PWRM_GPIO_CFG));

  *GpeDw0Value = ((Data32 & B_PMC_PWRM_GPIO_CFG_GPE0_DW0) >> N_PMC_PWRM_GPIO_CFG_GPE0_DW0);
  *GpeDw1Value = ((Data32 & B_PMC_PWRM_GPIO_CFG_GPE0_DW1) >> N_PMC_PWRM_GPIO_CFG_GPE0_DW1);
  *GpeDw2Value = ((Data32 & B_PMC_PWRM_GPIO_CFG_GPE0_DW2) >> N_PMC_PWRM_GPIO_CFG_GPE0_DW2);
}

/**
  This function sets Group to GPE0 configuration

  @param[out] GpeDw0Value       GPIO Group to GPE_DW0 assignment
  @param[out] GpeDw1Value       GPIO Group to GPE_DW1 assignment
  @param[out] GpeDw2Value       GPIO Group to GPE_DW2 assignment
**/
VOID
PmcSetGpioGpe (
  IN UINT32    GpeDw0Value,
  IN UINT32    GpeDw1Value,
  IN UINT32    GpeDw2Value
  )
{
  UINT32               Data32Or;
  UINT32               Data32And;

  //
  // Program GPIO_CFG register
  //
  Data32And = (UINT32) ~(B_PMC_PWRM_GPIO_CFG_GPE0_DW2 | B_PMC_PWRM_GPIO_CFG_GPE0_DW1 | B_PMC_PWRM_GPIO_CFG_GPE0_DW0);
  Data32Or  = (UINT32) ((GpeDw2Value << N_PMC_PWRM_GPIO_CFG_GPE0_DW2) |
                        (GpeDw1Value << N_PMC_PWRM_GPIO_CFG_GPE0_DW1) |
                        (GpeDw0Value << N_PMC_PWRM_GPIO_CFG_GPE0_DW0));

  MmioAndThenOr32 (
    (PmcGetPwrmBase () + R_PMC_PWRM_GPIO_CFG),
    Data32And,
    Data32Or
    );
}

/**
  Disable SLP_S0# assertion when system is in debug mode
**/
VOID
PmcDisableSlpS0AssertionInDebugMode (
  VOID
  )
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));

  Status = PmcSendCommand (V_PMC_PWRM_IPC_CMD_COMMAND_SLP_CTRL, 0, 4, &Wbuf, NULL);
  ASSERT_EFI_ERROR (Status);
}

/**
  Enable SLP_S0# assertion even when system is in debug mode
**/
VOID
PmcEnableSlpS0AssertionInDebugMode (
  VOID
  )
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));

  Wbuf.Buf0 = 1;
  Status = PmcSendCommand (V_PMC_PWRM_IPC_CMD_COMMAND_SLP_CTRL, 0, 4, &Wbuf, NULL);
  ASSERT_EFI_ERROR (Status);
}

/**
  This function gets NMI regsiter.

  @retval  NMI register setting
**/
UINT32
PmcGetNmiControl (
  VOID
  )
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;
  PMC_IPC_COMMAND_BUFFER    Rbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  ZeroMem (&Rbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  //
  // WBUF0 = 2 for NMI delivery control and status register (entire register PCR[ITSS] 0x3330)
  //
  Wbuf.Buf0 = V_PMC_PWRM_IPC_CMD_WBUF0_PROXY_NMI;
  Status = PmcSendCommand (
             V_PMC_PWRM_IPC_CMD_COMMAND_PROXY,
             V_PMC_PWRM_IPC_CMD_CMD_ID_PROXY_READ,
             4,
             &Wbuf,
             &Rbuf
             );
  ASSERT_EFI_ERROR (Status);
  return Rbuf.Buf0;
}

/**
  This function sets the NMI register

  @param[in]  NmiRegister    The whole NMI register
**/
VOID
PmcSetNmiControl (
  UINT32   NmiRegister
  )
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  //
  // WBUF0 = 2 for NMI delivery control and status register (entire register PCR[ITSS] 0x3330)
  //
  Wbuf.Buf0 = V_PMC_PWRM_IPC_CMD_WBUF0_PROXY_NMI;
  Wbuf.Buf1 = NmiRegister;
  Status = PmcSendCommand (
             V_PMC_PWRM_IPC_CMD_COMMAND_PROXY,
             V_PMC_PWRM_IPC_CMD_CMD_ID_PROXY_WRITE,
             8,
             &Wbuf,
             NULL
             );
  ASSERT_EFI_ERROR (Status);
}

/**
  This function enables GBE ModPHY SPD gating.
**/
VOID
PmcGbeModPhyPowerGating (
  VOID
  )
{
  if (PmcIsModPhySusPgSupported ()) {
    //
    // Set B_PCH_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_GBE ModPHY SPD RT Request
    //
    MmioOr32 (PmcGetPwrmBase () + R_PMC_PWRM_MODPHY_PM_CFG5, B_PMC_PWRM_MODPHY_PM_CFG5_MSPDRTREQ_GBE);
  }
}
