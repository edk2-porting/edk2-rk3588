/** @file
  Pch common library for PCH INIT PEI/DXE/SMM modules

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <PchPolicyCommon.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PchSbiAccessLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SataLib.h>
#include <Library/S3BootScriptLib.h>
#include <Register/PchRegsPcie.h>

extern CONST PCH_PCIE_CONTROLLER_INFO mPchPcieControllerInfo[];
extern CONST UINT32 mPchPcieControllerInfoSize;

#define PORT_PLS_TIMEOUT 100    ///< 100 * 10 us = 1ms timeout for USB3 PortSC PLS polling

/**
  This function returns PID according to PCIe controller index

  @param[in] ControllerIndex     PCIe controller index

  @retval PCH_SBI_PID    Returns PID for SBI Access
**/
PCH_SBI_PID
PchGetPcieControllerSbiPid (
  IN  UINT32  ControllerIndex
  )
{
  ASSERT (ControllerIndex < mPchPcieControllerInfoSize);
  return mPchPcieControllerInfo[ControllerIndex].Pid;
}

/**
  This function returns PID according to Root Port Number

  @param[in] RpIndex     Root Port Index (0-based)

  @retval PCH_SBI_PID    Returns PID for SBI Access
**/
PCH_SBI_PID
GetRpSbiPid (
  IN  UINTN  RpIndex
  )
{
  return PchGetPcieControllerSbiPid ((UINT32) (RpIndex / PCH_PCIE_CONTROLLER_PORTS));
}

/**
  Calculate root port device number based on physical port index.

  @param[in]  RpIndex              Root port index (0-based).

  @retval     Root port device number.
**/
UINT32
PchGetPcieRpDevice (
  IN  UINT32   RpIndex
  )
{
  UINTN ControllerIndex;
  ControllerIndex = RpIndex / PCH_PCIE_CONTROLLER_PORTS;
  ASSERT (ControllerIndex < mPchPcieControllerInfoSize);
  return mPchPcieControllerInfo[ControllerIndex].DevNum;
}

/**
  This function reads Pci Config register via SBI Access

  @param[in]  RpIndex             Root Port Index (0-based)
  @param[in]  Offset              Offset of Config register
  @param[out] *Data32             Value of Config register

  @retval EFI_SUCCESS             SBI Read successful.
**/
EFI_STATUS
PchSbiRpPciRead32 (
  IN    UINT32  RpIndex,
  IN    UINT32  Offset,
  OUT   UINT32  *Data32
  )
{
  EFI_STATUS    Status;
  UINT32        RpDevice;
  UINT8         Response;
  UINT16        Fid;

  RpDevice = PchGetPcieRpDevice (RpIndex);
  Fid = (UINT16) ((RpDevice << 3) | (RpIndex % 4 ));
  Status = PchSbiExecutionEx (
             GetRpSbiPid (RpIndex),
             Offset,
             PciConfigRead,
             FALSE,
             0xF,
             0,
             Fid,
             Data32,
             &Response
             );
  if (Status != EFI_SUCCESS) {
    DEBUG((DEBUG_ERROR,"Sideband Read Failed of RpIndex %d Offset 0x%x. Device = %d Fid = 0x%x\n",RpIndex, Offset, RpDevice, Fid));
    ASSERT (FALSE);
  }
  return Status;
}

/**
  This function And then Or Pci Config register via SBI Access

  @param[in]  RpIndex             Root Port Index (0-based)
  @param[in]  Offset              Offset of Config register
  @param[in]  Data32And           Value of Config register to be And-ed
  @param[in]  Data32AOr           Value of Config register to be Or-ed

  @retval EFI_SUCCESS             SBI Read and Write successful.
**/
EFI_STATUS
PchSbiRpPciAndThenOr32 (
  IN  UINT32  RpIndex,
  IN  UINT32  Offset,
  IN  UINT32  Data32And,
  IN  UINT32  Data32Or
  )
{
  EFI_STATUS  Status;
  UINT32      RpDevice;
  UINT32      Data32;
  UINT8       Response;
  UINT16      Fid;

  RpDevice = PchGetPcieRpDevice (RpIndex);
  Status = PchSbiRpPciRead32 (RpIndex, Offset, &Data32);
  if (Status == EFI_SUCCESS) {
    Data32 &= Data32And;
    Data32 |= Data32Or;
    Fid = (UINT16) ((RpDevice << 3) | (RpIndex % 4 ));
    Status = PchSbiExecutionEx (
               GetRpSbiPid (RpIndex),
               Offset,
               PciConfigWrite,
               FALSE,
               0xF,
               0,
               Fid,
               &Data32,
               &Response
               );
    if (Status != EFI_SUCCESS) {
      DEBUG((DEBUG_ERROR,"Sideband Write Failed of RpIndex %d Offset 0x%x. Device = %d Fid = 0x%x\n",RpIndex, Offset, RpDevice, Fid));
      ASSERT (FALSE);
    }
  } else {
    ASSERT (FALSE);
  }
  return Status;
}

/**
  Print registers value

  @param[in] PrintMmioBase       Mmio base address
  @param[in] PrintSize           Number of registers
  @param[in] OffsetFromBase      Offset from mmio base address

  @retval None
**/
VOID
PrintRegisters (
  IN  UINTN        PrintMmioBase,
  IN  UINT32       PrintSize,
  IN  UINT32       OffsetFromBase
  )
{
  UINT32  Offset;
  DEBUG ((DEBUG_VERBOSE, "       00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F"));
  for (Offset = 0; Offset < PrintSize; Offset++) {
    if ((Offset % 16) == 0) {
      DEBUG ((DEBUG_VERBOSE, "\n %04X: ", (Offset + OffsetFromBase) & 0xFFF0));
    }
    DEBUG ((DEBUG_VERBOSE, "%02X ", MmioRead8 (PrintMmioBase + Offset)));
  }
  DEBUG ((DEBUG_VERBOSE, "\n"));
}

/**
  Print registers value

  @param[in] PrintPciSegmentBase Pci segment base address
  @param[in] PrintSize           Number of registers
  @param[in] OffsetFromBase      Offset from mmio base address

  @retval None
**/
VOID
PrintPciRegisters (
  IN  UINT64       PrintPciSegmentBase,
  IN  UINT32       PrintSize,
  IN  UINT32       OffsetFromBase
  )
{
  UINT32  Offset;
  DEBUG ((DEBUG_VERBOSE, "       00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F"));
  for (Offset = 0; Offset < PrintSize; Offset++) {
    if ((Offset % 16) == 0) {
      DEBUG ((DEBUG_VERBOSE, "\n %04X: ", (Offset + OffsetFromBase) & 0xFFF0));
    }
    DEBUG ((DEBUG_VERBOSE, "%02X ", PciSegmentRead8 (PrintPciSegmentBase + Offset)));
  }
  DEBUG ((DEBUG_VERBOSE, "\n"));
}
