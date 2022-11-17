/** @file
  This file contains GPIO routines for RC usage

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>
#include <Library/GpioPrivateLib.h>
#include <Library/GpioNameBufferLib.h>
#include <Register/PchPcrRegs.h>
#include <Register/GpioRegs.h>

/**
  This procedure is used to check if GpioPad is valid for certain chipset

  @param[in]  GpioPad             GPIO pad

  @retval TRUE                    This pin is valid on this chipset
          FALSE                   Incorrect pin
**/
BOOLEAN
GpioIsCorrectPadForThisChipset (
  IN  GPIO_PAD        GpioPad
  )
{
  return ((GPIO_GET_CHIPSET_ID (GpioPad) == GpioGetThisChipsetId ()) &&
         (GpioGetGroupIndexFromGpioPad (GpioPad) < GpioGetNumberOfGroups ()));
}

/**
  This procedure is used by PchSmiDispatcher and will return information
  needed to register GPI SMI.

  @param[in]  Index                   GPI SMI number
  @param[out] GpioPin                 GPIO pin
  @param[out] GpiSmiBitOffset         GPI SMI bit position within GpiSmi Registers
  @param[out] GpiHostSwOwnRegAddress  Address of HOSTSW_OWN register
  @param[out] GpiSmiStsRegAddress     Address of GPI SMI status register

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetPadAndSmiRegs (
  IN UINT32            Index,
  OUT GPIO_PAD         *GpioPin,
  OUT UINT8            *GpiSmiBitOffset,
  OUT UINT32           *GpiHostSwOwnRegAddress,
  OUT UINT32           *GpiSmiStsRegAddress
  )
{
  UINT32                 GroupIndex;
  UINT32                 PadNumber;
  CONST GPIO_GROUP_INFO  *GpioGroupInfo;
  GPIO_GROUP             GpioGroup;
  UINT32                 GpioGroupInfoLength;
  UINT32                 SmiStsRegOffset;
  UINT32                 HostSwOwnRegOffset;
  GPIO_PAD_OWN           PadOwnVal;

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

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

  GpioGroup = GpioGetGroupFromGroupIndex (GroupIndex);
  *GpioPin = GpioGetGpioPadFromGroupAndPadNumber (GpioGroup, PadNumber);

  DEBUG_CODE_BEGIN ();
  //
  // Check if selected GPIO Pad is not owned by CSME/ISH/IE
  //
  GpioGetPadOwnership (*GpioPin, &PadOwnVal);
  if (PadOwnVal != GpioPadOwnHost) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: %a not owned by host!\n", GpioName (*GpioPin)));
    return EFI_INVALID_PARAMETER;
  }
  DEBUG_CODE_END ();

  *GpiSmiBitOffset = (UINT8)(PadNumber % 32);

  HostSwOwnRegOffset = GpioGroupInfo[GroupIndex].HostOwnOffset + (PadNumber / 32) * 0x4;
  *GpiHostSwOwnRegAddress = PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, HostSwOwnRegOffset);

  SmiStsRegOffset = GpioGroupInfo[GroupIndex].SmiStsOffset + (PadNumber / 32) * 0x4;
  *GpiSmiStsRegAddress = PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, SmiStsRegOffset);

  return EFI_SUCCESS;
}

/**
  This procedure will check if GpioPad is owned by host.

  @param[in] GpioPad       GPIO pad

  @retval TRUE             GPIO pad is owned by host
  @retval FALSE            GPIO pad is not owned by host and should not be used with GPIO lib API
**/
BOOLEAN
GpioIsPadHostOwned (
  IN GPIO_PAD             GpioPad
  )
{
  GPIO_PAD_OWN         PadOwnVal;

  //
  // Check if selected GPIO Pad is not owned by CSME/ISH
  // If GPIO is not owned by Host all access to PadCfg will be dropped
  //
  GpioGetPadOwnership (GpioPad, &PadOwnVal);
  if (PadOwnVal != GpioPadOwnHost) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: %a is not owned by host!\n", GpioName (GpioPad)));
    return FALSE;
  }

  return TRUE;
}

/**
  This procedure will check if GpioPad argument is valid.
  Function will check below conditions:
   - GpioPad represents a pad for current PCH
   - GpioPad belongs to valid GpioGroup
   - GPIO PadNumber is not greater than number of pads for this group

  @param[in] GpioPad       GPIO pad

  @retval TRUE             GPIO pad is valid and can be used with GPIO lib API
  @retval FALSE            GPIO pad is invalid and cannot be used with GPIO lib API
**/
BOOLEAN
GpioIsPadValid (
  IN GPIO_PAD             GpioPad
  )
{
  CONST GPIO_GROUP_INFO  *GpioGroupInfo;
  UINT32                 GpioGroupInfoLength;
  UINT32                 PadNumber;
  UINT32                 GroupIndex;

  if (!GpioIsCorrectPadForThisChipset (GpioPad)) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Incorrect GpioPad (0x%08x) used on this chipset!\n", GpioPad));
    goto Error;
  }

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  //
  // Check if legal pin number
  //
  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);
  if (PadNumber >= GpioGroupInfo[GroupIndex].PadPerGroup) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Pin number (%d) exceeds range of %a group (max: %d)\n",
                                                                                   PadNumber,
                                                                                   GpioGetGroupName (GroupIndex),
                                                                                   GpioGroupInfo[GroupIndex].PadPerGroup));
    goto Error;
  }

  return TRUE;
Error:
  ASSERT (FALSE);
  return FALSE;
}

/**
  This procedure will read GPIO Pad Configuration register

  @param[in] GpioPad          GPIO pad
  @param[in] DwReg            Choose PADCFG register: 0:DW0, 1:DW1

  @retval PadCfgRegValue      PADCFG_DWx value
**/
UINT32
GpioReadPadCfgReg (
  IN GPIO_PAD             GpioPad,
  IN UINT8                DwReg
  )
{
  UINT32                 PadCfgReg;
  CONST GPIO_GROUP_INFO  *GpioGroupInfo;
  UINT32                 GpioGroupInfoLength;
  UINT32                 GroupIndex;
  UINT32                 PadNumber;

  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  //
  // Create Pad Configuration register offset
  //
  PadCfgReg = GpioGroupInfo[GroupIndex].PadCfgOffset + S_GPIO_PCR_PADCFG * PadNumber + 0x4 * DwReg;

  return MmioRead32 (PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, PadCfgReg));
}

/**
  This procedure will write or read GPIO Pad Configuration register

  @param[in] GpioPad              GPIO pad
  @param[in] DwReg                Choose PADCFG register: 0:DW0, 1:DW1
  @param[in] PadCfgAndMask        Mask to be AND'ed with PADCFG reg value
  @param[in] PadCfgOrMask         Mask to be OR'ed with PADCFG reg value

  @retval none
**/
VOID
GpioWritePadCfgReg (
  IN GPIO_PAD             GpioPad,
  IN UINT8                DwReg,
  IN UINT32               PadCfgAndMask,
  IN UINT32               PadCfgOrMask
  )
{
  UINT32                 PadCfgReg;
  CONST GPIO_GROUP_INFO  *GpioGroupInfo;
  UINT32                 GpioGroupInfoLength;
  UINT32                 GroupIndex;
  UINT32                 PadNumber;
  UINT32                 PadCfgLock;
  UINT32                 PadCfgLockTx;

  PadCfgLock = 0;
  PadCfgLockTx = 0;

  //
  // Check if Pad Configuration (except output state) is to be changed.
  // If AND and OR masks will indicate that configuration fields (other than output control)
  // are to be modified it means that there is a need to perform an unlock (if set)
  //
  if ((~PadCfgAndMask | PadCfgOrMask) & (UINT32)~B_GPIO_PCR_TX_STATE) {
    GpioGetPadCfgLock (GpioPad, &PadCfgLock);
    if (PadCfgLock) {
      GpioUnlockPadCfg (GpioPad);
    }
  }

  //
  // Check if Pad Output state is to be changed
  // If AND and OR masks will indicate that output control
  // is to be modified it means that there is a need to perform an unlock (if set)
  //
  if ((~PadCfgAndMask | PadCfgOrMask) & B_GPIO_PCR_TX_STATE) {
    GpioGetPadCfgLockTx (GpioPad, &PadCfgLockTx);
    if (PadCfgLockTx) {
      GpioUnlockPadCfgTx (GpioPad);
    }
  }

  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  //
  // Create Pad Configuration register offset
  //
  PadCfgReg = GpioGroupInfo[GroupIndex].PadCfgOffset + S_GPIO_PCR_PADCFG * PadNumber + 0x4 * DwReg;

  MmioAndThenOr32 (
    PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, PadCfgReg),
    PadCfgAndMask,
    PadCfgOrMask
    );

  if (PadCfgLock) {
    GpioLockPadCfg (GpioPad);
  }
  if (PadCfgLockTx) {
    GpioLockPadCfgTx (GpioPad);
  }
}

/**
  This procedure will set GPIO mode

  @param[in]  GpioPad             GPIO pad
  @param[in]  PadModeValue        GPIO pad mode value

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioSetPadMode (
  IN GPIO_PAD                GpioPad,
  IN GPIO_PAD_MODE           PadModeValue
  )
{
  UINT32               PadCfgOrMask;

  PadCfgOrMask = 0;

  if (!GpioIsPadValid (GpioPad)) {
    return EFI_INVALID_PARAMETER;
  }

  if (!GpioIsPadHostOwned (GpioPad)) {
    return EFI_UNSUPPORTED;
  }

  if (PadModeValue != (GPIO_PAD_MODE)GpioHardwareDefault) {

    PadCfgOrMask = (((PadModeValue & B_GPIO_PAD_MODE_MASK) >> (N_GPIO_PAD_MODE_BIT_POS + 1)) << N_GPIO_PCR_PAD_MODE);

    GpioWritePadCfgReg (
      GpioPad,
      0,
      (UINT32)~B_GPIO_PCR_PAD_MODE,
      PadCfgOrMask
      );
  }

  return EFI_SUCCESS;
}

/**
  This procedure will get GPIO mode

  @param[in]  GpioPad             GPIO pad
  @param[out] PadModeValue        GPIO pad mode value

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetPadMode (
  IN  GPIO_PAD                 GpioPad,
  OUT GPIO_PAD_MODE            *PadModeValue
  )
{
  UINT32        PadCfgRegValue;

  if (!GpioIsPadValid (GpioPad)) {
    return EFI_INVALID_PARAMETER;
  }

  if (!GpioIsPadHostOwned (GpioPad)) {
    return EFI_UNSUPPORTED;
  }

  PadCfgRegValue = GpioReadPadCfgReg (GpioPad, 0);

  *PadModeValue = (GPIO_PAD_MODE)(((PadCfgRegValue & B_GPIO_PCR_PAD_MODE) >> (N_GPIO_PCR_PAD_MODE - (N_GPIO_PAD_MODE_BIT_POS + 1))) | (0x1 << N_GPIO_PAD_MODE_BIT_POS));

  return EFI_SUCCESS;
}

/**
  Generates GPIO name from GpioPad
  This function returns pointer to the static buffer

  @param[in] GpioPad  GpioPad

  @retval CHAR8*  Pointer to the gpio name string
**/
CHAR8*
GpioName (
  IN GPIO_PAD  GpioPad
  )
{
  return GpioGetPadName (GpioPad, GpioGetStaticNameBuffer (), GPIO_NAME_LENGTH_MAX);
}
