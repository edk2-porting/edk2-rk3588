/** @file
  This file contains GPIO routines for RC usage

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <PchAccess.h>
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>
#include <Library/GpioPrivateLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Pch/Library/PeiDxeSmmGpioLib/GpioLibrary.h>
/**
  This procedure will get value of selected gpio register

  @param[in]  Group               GPIO group number
  @param[in]  Offset              GPIO register offset
  @param[out] RegVal              Value of gpio register

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetReg (
  IN  GPIO_GROUP              Group,
  IN  UINT32                  Offset,
  OUT UINT32                  *RegVal
  )
{
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;
  UINTN                GroupIndex;

  GroupIndex = GpioGetGroupIndexFromGroup (Group);
  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);
  //
  // Check if group argument exceeds GPIO GROUP INFO array
  //
  if ((UINTN) GroupIndex >= GpioGroupInfoLength) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  *RegVal = MmioRead32 (PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, Offset));

  return EFI_SUCCESS;
}

/**
  This procedure will set value of selected gpio register

  @param[in] Group               GPIO group number
  @param[in] Offset              GPIO register offset
  @param[in] RegVal              Value of gpio register

  @retval EFI_SUCCESS            The function completed successfully
  @retval EFI_INVALID_PARAMETER  Invalid group or pad number
**/
EFI_STATUS
GpioSetReg (
  IN GPIO_GROUP              Group,
  IN UINT32                  Offset,
  IN UINT32                  RegVal
  )
{
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;
  UINTN                GroupIndex;

  GroupIndex = GpioGetGroupIndexFromGroup (Group);
  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);
  //
  // Check if group argument exceeds GPIO GROUP INFO array
  //
  if ((UINTN) GroupIndex >= GpioGroupInfoLength) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  MmioWrite32 (PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, Offset), RegVal);

  return EFI_SUCCESS;
}

/**
  This procedure is used by PchSmiDispatcher and will return information
  needed to register GPI SMI. Relation between Index and GpioPad number is:
  Index = GpioGroup + 24 * GpioPad

  @param[in]  Index               GPI SMI number
  @param[out] GpioPin             GPIO pin
  @param[out] GpiSmiBitOffset     GPI SMI bit position within GpiSmi Registers
  @param[out] GpiSmiEnRegAddress  Address of GPI SMI Enable register
  @param[out] GpiSmiStsRegAddress Address of GPI SMI status register

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetPadAndSmiRegs (
  IN UINT32            Index,
  OUT GPIO_PAD         *GpioPin,
  OUT UINT8            *GpiSmiBitOffset,
  OUT UINT32           *GpiSmiEnRegAddress,
  OUT UINT32           *GpiSmiStsRegAddress
  )
{
  UINT32               GroupIndex;
  UINT32               PadNumber;
  GPIO_GROUP_INFO      *GpioGroupInfo;
  GPIO_GROUP           GpioGroupOffset;
  UINTN                GpioGroupInfoLength;
  UINT32               SmiRegOffset;

  GPIO_PAD_OWN       PadOwnVal;
  UINT32             HostOwnVal;


  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  GpioGroupOffset = GpioGetLowestGroup ();

  PadNumber = 0;
  GroupIndex = 0;
  for (GroupIndex = 0; GroupIndex < GpioGroupInfoLength; GroupIndex++) {
    PadNumber = Index;
    if (PadNumber < GpioGroupInfo[GroupIndex].PadPerGroup) {
      //
      // Found group and pad number
      //
      break;
    }
    Index = Index - GpioGroupInfo[GroupIndex].PadPerGroup;
  }

  //
  // Check if legal pad number
  //
  if (PadNumber >= GpioGroupInfo[GroupIndex].PadPerGroup){
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if selected group has GPI SMI Enable and Status registers
  //
  if (GpioGroupInfo[GroupIndex].SmiEnOffset == NO_REGISTER_FOR_PROPERTY) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG_CODE_BEGIN ();
  //
  // Check if selected GPIO Pad is not owned by CSME/ISH/IE
  //
  GpioGetPadOwnership (GpioGetGpioPadFromGroupAndPadNumber (GroupIndex + GpioGroupOffset, PadNumber), &PadOwnVal);
  if (PadOwnVal != GpioPadOwnHost) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Accessing pad not owned by host (Group=%d, Pad=%d)!\n",GroupIndex, PadNumber));
    return EFI_INVALID_PARAMETER;
  }
  //
  // Check if Host Software Pad Ownership is set to ACPI Mode
  //
  GpioGetHostSwOwnershipForPad (GpioGetGpioPadFromGroupAndPadNumber (GroupIndex + GpioGroupOffset, PadNumber), &HostOwnVal);
  if (HostOwnVal != V_PCH_PCR_GPIO_HOSTSW_OWN_ACPI) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  DEBUG_CODE_END ();

  *GpioPin = GpioGetGpioPadFromGroupAndPadNumber (GroupIndex + GpioGroupOffset, PadNumber);
  *GpiSmiBitOffset = (UINT8)(PadNumber % 32);

  SmiRegOffset = GpioGroupInfo[GroupIndex].SmiEnOffset + (PadNumber / 32) * 0x4;
  *GpiSmiEnRegAddress = PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, SmiRegOffset);

  SmiRegOffset = GpioGroupInfo[GroupIndex].SmiStsOffset + (PadNumber / 32) * 0x4;
  *GpiSmiStsRegAddress = PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, SmiRegOffset);

  return EFI_SUCCESS;
}

/**
  This procedure will clear GPIO_UNLOCK_SMI_STS

  @param[in]  None

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioClearUnlockSmiSts (
  VOID
  )
{
  UINT16  AcpiBaseAddr;

  PchAcpiBaseGet (&AcpiBaseAddr);

  //
  // GPIO_UNLOCK_SMI_STS is cleared by writing 1 to it. Other bits in
  // R_PCH_SMI_STS registers are either RO or RW/1C so writing 0 to them
  // will not change their state.
  //
  IoWrite32 (AcpiBaseAddr + R_PCH_SMI_STS, B_PCH_SMI_STS_GPIO_UNLOCK);

  return EFI_SUCCESS;
}


/**
  This procedure will set GPIO Driver IRQ number

  @param[in]  Irq                 Irq number

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid IRQ number
**/
EFI_STATUS
GpioSetIrq (
  IN  UINT8          Irq
  )
{
  UINT32   Data32And;
  UINT32   Data32Or;

  //
  // Check if Irq is 14 or 15
  //
  if ((Irq < 14) || (Irq > 15)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Data32And = (UINT32) ~(B_PCH_PCR_GPIO_MISCCFG_IRQ_ROUTE);
  Data32Or  = (UINT32) (Irq - 14) << N_PCH_PCR_GPIO_MISCCFG_IRQ_ROUTE;

  //
  // Program MISCCFG register for Community 0
  //
  MmioAndThenOr32 (
    PCH_PCR_ADDRESS (PID_GPIOCOM0, R_PCH_PCR_GPIO_MISCCFG),
    Data32And,
    Data32Or
    );

  //
  // Program MISCCFG register for Community 1
  //
  MmioAndThenOr32 (
    PCH_PCR_ADDRESS (PID_GPIOCOM1, R_PCH_PCR_GPIO_MISCCFG),
    Data32And,
    Data32Or
    );

  //
  // Program MISCCFG register for Community 2
  //
  MmioAndThenOr32 (
    PCH_PCR_ADDRESS (PID_GPIOCOM2, R_PCH_PCR_GPIO_MISCCFG),
    Data32And,
    Data32Or
    );

  //
  // Program MISCCFG register for Community 3
  //
  MmioAndThenOr32 (
    PCH_PCR_ADDRESS (PID_GPIOCOM3, R_PCH_PCR_GPIO_MISCCFG),
    Data32And,
    Data32Or
    );

  return EFI_SUCCESS;
}

/**
  This procedure will perform special handling of GPP_A_12 on PCH-LP.

  @param[in]  None

  @retval None
**/
VOID
GpioA12SpecialHandling (
  VOID
  )
{
  GPIO_PAD_OWN         PadOwnVal;

  //
  // SKL PCH BWG 16.6. PCH-LP GPP_A_12 Special Handling
  //
  if (GetPchSeries () == PchLp) {
    GpioGetPadOwnership (GPIO_SKL_LP_GPP_A12, &PadOwnVal);

    //
    // If the pad is host-own, BIOS has to always lock this pad after being initialized
    //
    if (PadOwnVal == GpioPadOwnHost) {
      //
      // Set PadCfgLock for GPP_A_12
      //
      GpioLockPadCfg (GPIO_SKL_LP_GPP_A12);
    }
  }
}
