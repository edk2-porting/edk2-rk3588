/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "GpioLibrary.h"

//
// Chipset specific data
//
//SATA
extern GPIO_PAD_NATIVE_FUNCTION mPchHSataPortResetToGpioMap[PCH_H_AHCI_MAX_PORTS];
extern GPIO_PAD_NATIVE_FUNCTION mPchHSataDevSlpPinToGpioMap[PCH_H_AHCI_MAX_PORTS];

//
// SKX specific
//
extern GPIO_GROUP_INFO mPchGpioGroupInfo[V_PCH_GPIO_GROUP_MAX];

/**
  This procedure will set GPIO mode

  @param[in]  GpioPad             GPIO pad
  @param[out] PadModeValue        GPIO pad mode value

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
SetGpioPadMode (
  IN GPIO_PAD                GpioPad,
  IN GPIO_PAD_MODE           PadModeValue
  )
{
  GPIO_PAD_OWN         PadOwnVal;
  UINT32               PadCfgReg;
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;
  UINT32               PadNumber;
  UINT32               GroupIndex;
  UINT32               Dw0Reg;
  UINT32               Dw0RegMask;

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

DEBUG_CODE_BEGIN();
  if (!GpioIsCorrectPadForThisChipset (GpioPad)) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Incorrect GpioPad define used on this chipset (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }
DEBUG_CODE_END();

  GpioGetPadOwnership (GpioPad, &PadOwnVal);

  if (PadOwnVal != GpioPadOwnHost) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Accessing pad not owned by host (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
    return EFI_UNSUPPORTED;
  }

  if (GpioIsPadLocked (GroupIndex, PadNumber)) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Pad is locked (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
    return EFI_WRITE_PROTECTED;
  }

  //
  // Create Pad Configuration register offset
  //
  PadCfgReg = 0x8 * PadNumber + GpioGroupInfo[GroupIndex].PadCfgOffset;

  Dw0RegMask = ((((PadModeValue & GPIO_CONF_PAD_MODE_MASK) >> GPIO_CONF_PAD_MODE_BIT_POS) == GpioHardwareDefault) ? 0x0 : B_PCH_GPIO_PAD_MODE);
  Dw0Reg = (((PadModeValue & GPIO_CONF_PAD_MODE_MASK) >> (GPIO_CONF_PAD_MODE_BIT_POS + 1)) << N_PCH_GPIO_PAD_MODE);

  MmioAndThenOr32 (
    (UINTN)PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, PadCfgReg),
    ~(UINT32)Dw0RegMask,
    (UINT32)Dw0Reg
  );

  return EFI_SUCCESS;
}

/**
  This procedure will get GPIO mode

  @param[in]  GpioPad             GPIO pad
  @param[out] PadModeValue        GPIO pad mode value

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GetGpioPadMode (
  IN  GPIO_PAD                 GpioPad,
  OUT GPIO_PAD_MODE            *PadModeValue
  )
{
  GPIO_PAD_OWN         PadOwnVal;
  UINT32               PadCfgReg;
  GPIO_GROUP_INFO      *GpioGroupInfo;
  UINTN                GpioGroupInfoLength;
  UINT32               PadNumber;
  UINT32               GroupIndex;
  UINT32               Dw0Reg;

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioPad);
  PadNumber = GpioGetPadNumberFromGpioPad (GpioPad);

DEBUG_CODE_BEGIN();
  if (!GpioIsCorrectPadForThisChipset (GpioPad)) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Incorrect GpioPad define used on this chipset (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }
DEBUG_CODE_END();

  GpioGetPadOwnership (GpioPad, &PadOwnVal);

  if (PadOwnVal != GpioPadOwnHost) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Accessing pad not owned by host (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
    return EFI_UNSUPPORTED;
  }

  //
  // Create Pad Configuration register offset
  //
  PadCfgReg = 0x8 * PadNumber + GpioGroupInfo[GroupIndex].PadCfgOffset;

  Dw0Reg = MmioRead32 ((UINTN)PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, PadCfgReg));

  *PadModeValue = ((Dw0Reg & B_PCH_GPIO_PAD_MODE) >> (N_PCH_GPIO_PAD_MODE - (GPIO_CONF_PAD_MODE_BIT_POS + 1))) | (0x1 << GPIO_CONF_PAD_MODE_BIT_POS);

  return EFI_SUCCESS;
}


/**
  This procedure will retrieve address and length of GPIO info table

  @param[out]  GpioGroupInfoTableLength   Length of GPIO group table

  @retval Pointer to GPIO group table

**/
GPIO_GROUP_INFO*
GpioGetGroupInfoTable (
  OUT UINTN               *GpioGroupInfoTableLength
  )
{
  if (GetPchGeneration () == SklPch) {
      *GpioGroupInfoTableLength = sizeof (mPchGpioGroupInfo) / sizeof (GPIO_GROUP_INFO);
      return mPchGpioGroupInfo;
  } else {
    *GpioGroupInfoTableLength = 0;
    return NULL;
  }
}


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
DEBUG_CODE_BEGIN();
  PCH_SERIES PchSeries;

  PchSeries = GetPchSeries ();

  if ((PchSeries == PchH) && (GPIO_GET_CHIPSET_ID(GpioPad) == GPIO_SKL_H_CHIPSET_ID)) {
    return TRUE;
  } else if ((PchSeries == PchLp) && (GPIO_GET_CHIPSET_ID(GpioPad) == GPIO_SKL_LP_CHIPSET_ID)) {
    return TRUE;
  }

DEBUG_CODE_END();
  return FALSE;
}


/**
  This procedure will get number of pads for certain GPIO group

  @param[in] Group            GPIO group number

  @retval Value               Pad number for group
                              If illegal group number then return 0
**/
UINT32
GpioGetPadPerGroup (
  IN GPIO_GROUP      Group
  )
{
  GPIO_GROUP_INFO     *GpioGroupInfo;
  UINTN               GpioGroupInfoLength;
  UINT32              GroupIndex;
  //
  // Check if group argument exceeds GPIO GROUP INFO array
  //
  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);
  GroupIndex = GpioGetGroupIndexFromGroup (Group);

  if ((UINTN)GroupIndex >= GpioGroupInfoLength) {
    ASSERT(FALSE);
    return 0;
  } else {
    return GpioGroupInfo[GroupIndex].PadPerGroup;
  }
}

/**
  This procedure will get number of groups

  @param[in] none

  @retval Value               Group number
**/
UINT8
GpioGetNumberOfGroups (
  VOID
  )
{
    return V_PCH_H_GPIO_GROUP_MAX;
}
/**
  This procedure will get lowest group

  @param[in] none

  @retval Value               Lowest Group
**/
GPIO_GROUP
GpioGetLowestGroup (
  VOID
  )
{
    return (UINT32)GPIO_SKL_H_GROUP_GPP_A;
}
/**
  This procedure will get highest group

  @param[in] none

  @retval Value               Highest Group
**/
GPIO_GROUP
GpioGetHighestGroup (
  VOID
  )
{
    return (UINT32)GPIO_SKL_H_GROUP_GPD;
}

/**
  This procedure will get group number

  @param[in] GpioPad          Gpio Pad

  @retval Value               Group number
**/
GPIO_GROUP
GpioGetGroupFromGpioPad (
  IN GPIO_PAD         GpioPad
  )
{
  return GPIO_GET_GROUP_FROM_PAD (GpioPad);
}

/**
  This procedure will get group index (0 based)

  @param[in] GpioPad          Gpio Pad

  @retval Value               Group Index
**/
UINT32
GpioGetGroupIndexFromGpioPad (
  IN GPIO_PAD        GpioPad
  )
{
  return (UINT32)GPIO_GET_GROUP_INDEX_FROM_PAD (GpioPad);
}

/**
  This procedure will get group index (0 based) from group

  @param[in] GpioGroup        Gpio Group

  @retval Value               Group Index
**/
UINT32
GpioGetGroupIndexFromGroup (
  IN GPIO_GROUP        GpioGroup
  )
{
  return (UINT32)GPIO_GET_GROUP_INDEX (GpioGroup);
}

/**
  This procedure will get pad number (0 based) from Gpio Pad

  @param[in] GpioPad          Gpio Pad

  @retval Value               Pad Number
**/
UINT32
GpioGetPadNumberFromGpioPad (
  IN GPIO_PAD        GpioPad
  )
{
  return (UINT32)GPIO_GET_PAD_NUMBER (GpioPad);
}
/**
  This procedure will return GpioPad from Group and PadNumber

  @param[in] Group              GPIO group
  @param[in] PadNumber          GPIO PadNumber

  @retval GpioPad               GpioPad
**/
GPIO_PAD
GpioGetGpioPadFromGroupAndPadNumber (
  IN GPIO_GROUP      Group,
  IN UINT32          PadNumber
  )
{
    return GPIO_PAD_DEF(Group,PadNumber);
}



/**
  This function checks if GPIO pin for SATA reset port is in GPIO MODE

  @param[in]  SataPort            SATA port number

  @retval TRUE                    Pin is in GPIO mode
          FALSE                   Pin is in native mode
**/
BOOLEAN
GpioIsSataResetPortInGpioMode (
  IN  UINTN           SataPort
  )
{
  EFI_STATUS     Status;
  UINT32         GpioPin;
  GPIO_PAD_MODE  GpioMode;


    ASSERT (SataPort < PCH_H_AHCI_MAX_PORTS);
    GpioPin = mPchHSataPortResetToGpioMap[SataPort].Pad;

  Status =  GetGpioPadMode (GpioPin, &GpioMode);
  if ((EFI_ERROR (Status)) || (GpioMode != GpioPadModeGpio)) {
    return FALSE;
  } else {
    return TRUE;
  }
}


/**
  This function checks if GPIO pin is a SataDevSlp pin

  @param[in]  GpioPad             GPIO pad
  @param[in]  PadMode             GPIO pad mode

  @retval TRUE                    Pin is in GPIO mode
          FALSE                   Pin is in native mode
**/
BOOLEAN
GpioIsPadASataDevSlpPin (
  IN  GPIO_PAD        GpioPad,
  IN  GPIO_PAD_MODE   PadMode
  )
{
  UINT32                    SataDevSlpPinMax;
  UINT32                    SataDevSlpPinIndex;
  GPIO_PAD_OWN              PadOwnership;
  GPIO_PAD_NATIVE_FUNCTION  *SataDevSlpPinToGpioMap;

    SataDevSlpPinToGpioMap = mPchHSataDevSlpPinToGpioMap;
    SataDevSlpPinMax = sizeof(mPchHSataDevSlpPinToGpioMap)/sizeof(GPIO_PAD_NATIVE_FUNCTION);

  for (SataDevSlpPinIndex = 0; SataDevSlpPinIndex < SataDevSlpPinMax; SataDevSlpPinIndex++) {
    if ((GpioPad == SataDevSlpPinToGpioMap[SataDevSlpPinIndex].Pad) &&
      (PadMode == SataDevSlpPinToGpioMap[SataDevSlpPinIndex].Mode)) {
      GpioGetPadOwnership (SataDevSlpPinToGpioMap[SataDevSlpPinIndex].Pad , &PadOwnership);
      if (PadOwnership == GpioPadOwnHost) {
        return TRUE;
      } else {
        return FALSE;
      }
    }
  }
  return FALSE;
}

/**
  This function checks if SataDevSlp pin is in native mode

  @param[in]  SataPort            SATA port
  @param[out] DevSlpPad           DevSlpPad

  @retval TRUE                    DevSlp is in native mode
          FALSE                   DevSlp is not in native mode
**/
BOOLEAN
GpioIsSataDevSlpPinEnabled (
    IN  UINTN           SataPort,
    OUT GPIO_PAD        *DevSlpPad
  )
{
  GPIO_PAD_MODE  DevSlpPadMode;
  GPIO_PAD       DevSlpGpioPad;
  GPIO_PAD_MODE  GpioMode;
  EFI_STATUS     Status;

    ASSERT (SataPort < PCH_H_AHCI_MAX_PORTS);
    DevSlpGpioPad = mPchHSataDevSlpPinToGpioMap[SataPort].Pad;
    DevSlpPadMode = mPchHSataDevSlpPinToGpioMap[SataPort].Mode;

  Status = GetGpioPadMode (DevSlpGpioPad, &GpioMode);

  if (EFI_ERROR (Status) || (GpioMode != DevSlpPadMode)) {
    *DevSlpPad = 0x0;
    return FALSE;
  } else {
    *DevSlpPad = DevSlpGpioPad;
    return TRUE;
  }
}

