/** @file
  Header file for GPIO Lib implementation.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _GPIO_LIBRARY_H_
#define _GPIO_LIBRARY_H_

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <PchAccess.h>
#include <GpioPinsSklLp.h>
#include <GpioPinsSklH.h>
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>
#include <Library/MmPciLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchSbiAccessLib.h>

typedef struct {
  GPIO_PAD       Pad;
  GPIO_PAD_MODE  Mode;
} GPIO_PAD_NATIVE_FUNCTION;


// BIT15-0  - pad number
// BIT31-16 - group info
//   BIT23- 16 - group index
//   BIT31- 24 - chipset ID
#define PAD_INFO_MASK          0x0000FFFF
#define GROUP_INFO_POSITION    16
#define GROUP_INFO_MASK        0xFFFF0000
#define GROUP_INDEX_MASK       0x00FF0000
#define UNIQUE_ID_MASK         0xFF000000
#define UNIQUE_ID_POSITION     24

#define GPIO_PAD_DEF(Group,Pad)               (UINT32)(((Group) << 16) + (Pad))
#define GPIO_GROUP_DEF(Index,ChipsetId)       ((Index) | ((ChipsetId) << 8))
#define GPIO_GET_GROUP_INDEX(Group)           ((Group) & 0xFF)
#define GPIO_GET_GROUP_FROM_PAD(Pad)          ((Pad) >> 16)
#define GPIO_GET_GROUP_INDEX_FROM_PAD(Pad)    GPIO_GET_GROUP_INDEX (((Pad) >> 16))
#define GPIO_GET_PAD_NUMBER(Pad)              ((Pad) & 0xFFFF)
#define GPIO_GET_CHIPSET_ID(Pad)              ((Pad) >> 24)

#define GPIO_GET_PAD_POSITION(PadNumber)      ((PadNumber) % 32)
#define GPIO_GET_DW_NUM(PadNumber)            ((PadNumber) / 32)

//
// Unique ID used in GpioPad defines
//
#define GPIO_SKL_H_CHIPSET_ID       0x1
#define GPIO_SKL_LP_CHIPSET_ID      0x2

//
// Below defines are based on GPIO_CONFIG structure fields
//
#define B_GPIO_PAD_MODE_MASK                            0xF
#define N_GPIO_PAD_MODE_BIT_POS                         0
#define B_GPIO_HOSTSW_OWN_MASK                          0x3
#define N_GPIO_HOSTSW_OWN_BIT_POS                       0
#define B_GPIO_DIRECTION_MASK                           0x1F
#define B_GPIO_DIRECTION_DIR_MASK                       0x7
#define N_GPIO_DIRECTION_DIR_BIT_POS                    0
#define B_GPIO_DIRECTION_INV_MASK                       0x18
#define N_GPIO_DIRECTION_INV_BIT_POS                    3
#define B_GPIO_OUTPUT_MASK                              0x3
#define N_GPIO_OUTPUT_BIT_POS                           0
#define N_GPIO_INT_CONFIG_INT_SOURCE_BIT_POS            0
#define N_GPIO_INT_CONFIG_INT_TYPE_BIT_POS              5
#define B_GPIO_RESET_CONFIG_RESET_MASK                  0x3F
#define N_GPIO_RESET_CONFIG_OLD_RESET_TYPE              BIT1
#define B_GPIO_RESET_CONFIG_OLD_RESET_MASK              0xF
#define N_GPIO_RESET_CONFIG_RESET_BIT_POS               0
#define B_GPIO_RESET_CONFIG_GPD_RESET_MASK              (BIT5 | BIT4)
#define B_GPIO_RESET_CONFIG_GPP_RESET_MASK              (BIT3 | BIT2)
#define N_GPIO_ELECTRICAL_CONFIG_TERMINATION_BIT_POS    0
#define N_GPIO_ELECTRICAL_CONFIG_1V8_TOLERANCE_BIT_POS  5
#define N_GPIO_OTHER_CONFIG_RXRAW_BIT_POS               0

//
// Structure for storing information about registers offset, community,
// maximal pad number for available groups
//
typedef struct {
  PCH_SBI_PID  Community;
  UINT32       PadOwnOffset;
  UINT32       HostOwnOffset;
  UINT32       GpiIsOffset;
  UINT32       GpiIeOffset;
  UINT32       GpiGpeStsOffset;
  UINT32       GpiGpeEnOffset;
  UINT32       SmiStsOffset;
  UINT32       SmiEnOffset;
  UINT32       NmiStsOffset;
  UINT32       NmiEnOffset;
  UINT32       PadCfgLockOffset;
  UINT32       PadCfgLockTxOffset;
  UINT32       PadCfgOffset;
  UINT32       PadPerGroup;
} GPIO_GROUP_INFO;

//
// If in GPIO_GROUP_INFO structure certain register doesn't exist
// it will have value equal to NO_REGISTER_FOR_PROPERTY
//
#define NO_REGISTER_FOR_PROPERTY (~0u)

//
// PADCFG register is split into multiple DW registers
// PAD_CFG_SIZE holds number of bytes used by all those registers for one pad
//
#define PAD_CFG_SIZE 0x08

/**
  This procedure is used to check if GpioPad is valid for certain chipset

  @param[in]  GpioPad             GPIO pad

  @retval TRUE                    This pin is valid on this chipset
          FALSE                   Incorrect pin
**/
BOOLEAN
GpioIsCorrectPadForThisChipset (
  IN  GPIO_PAD        GpioPad
  );


/**
  This procedure will retrieve address and length of GPIO info table

  @param[out]  GpioGroupInfoTableLength   Length of GPIO group table

  @retval Pointer to GPIO group table

**/
GPIO_GROUP_INFO*
GpioGetGroupInfoTable (
  OUT UINTN               *GpioGroupInfoTableLength
  );

/**
  This procedure will check if GpioPad is owned by host.

  @param[in] GpioPad       GPIO pad

  @retval TRUE             GPIO pad is owned by host
  @retval FALSE            GPIO pad is not owned by host and should not be used with GPIO lib API
**/
BOOLEAN
GpioIsPadHostOwned (
  IN GPIO_PAD             GpioPad
  );

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
  );

/**
  This procedure will write or read GPIO Pad Configuration register

  @param[in] GpioPad              GPIO pad
  @param[in] DwReg                Choose PADCFG register: 0:DW0, 1:DW1
  @param[out] PadCfgRegValue      Read data

  @retval none
**/
VOID
GpioReadPadCfgReg (
  IN GPIO_PAD             GpioPad,
  IN UINT8                DwReg,
  OUT UINT32              *PadCfgRegValue
  );

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
  );

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
  );

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
  );

/**
  This function checks if GPIO pin is a GSPI chip select pin

  @param[in]  GpioPad             GPIO pad
  @param[in]  PadMode             GPIO pad mode

  @retval TRUE                    Pin is in GPIO mode
          FALSE                   Pin is in native mode
**/
BOOLEAN
GpioIsGpioPadAGSpiCsbPin (
  IN  GPIO_PAD        GpioPad,
  IN  GPIO_PAD_MODE   PadMode
  );

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
  );

/**
  This internal procedure will calculate GPIO_RESET_CONFIG value  (new type)
  based on provided PadRstCfg for a specific GPIO Pad.

  @param[in]  GpioPad               GPIO Pad
  @param[in]  PadRstCfg             GPIO PadRstCfg value

  @retval GpioResetConfig           GPIO Reset configuration (new type)
**/
GPIO_RESET_CONFIG
GpioResetConfigFromPadRstCfg (
  IN  GPIO_PAD           GpioPad,
  IN  UINT32             PadRstCfg
  );

/**
  This internal procedure will calculate PadRstCfg register value based
  on provided GPIO Reset configuration for a certain pad.

  @param[in]  GpioPad                   GPIO Pad
  @param[in]  GpioResetConfig           GPIO Reset configuration
  @param[out] PadRstCfg                 GPIO PadRstCfg value

  @retval EFI_SUCCESS                   The function completed successfully
  @retval EFI_INVALID_PARAMETER         Invalid configuration
**/
EFI_STATUS
GpioPadRstCfgFromResetConfig (
  IN  GPIO_PAD           GpioPad,
  IN  GPIO_RESET_CONFIG  GpioResetConfig,
  OUT UINT32             *PadRstCfg
  );

/**
  This procedure will calculate PADCFG register value based on GpioConfig data

  @param[in]  GpioPad                   GPIO Pad
  @param[in]  GpioConfig                GPIO Configuration data
  @param[out] PadCfgDwReg               PADCFG DWx register value
  @param[out] PadCfgDwRegMask           Mask with PADCFG DWx register bits to be modified

  @retval Status
**/
EFI_STATUS
GpioPadCfgRegValueFromGpioConfig (
  IN  GPIO_PAD           GpioPad,
  IN  CONST GPIO_CONFIG  *GpioConfig,
  OUT UINT32             *PadCfgDwReg,
  OUT UINT32             *PadCfgDwRegMask
  );
#endif // _GPIO_LIBRARY_H_
