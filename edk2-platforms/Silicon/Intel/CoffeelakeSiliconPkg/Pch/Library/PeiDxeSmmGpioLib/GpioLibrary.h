/** @file
  Header file for GPIO Lib implementation.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _GPIO_LIBRARY_H_
#define _GPIO_LIBRARY_H_

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>
#include <Private/Library/GpioPrivateLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SataLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchSbiAccessLib.h>
#include <Private/Library/PmcPrivateLib.h>
#include <Private/Library/GpioHelpersLib.h>
#include <Register/PchRegsGpio.h>

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
#define GPIO_GET_DW_NUM(PadNumber)            ((PadNumber) / 32u)

//
// Number of PADCFG_DW registers
//
#define GPIO_PADCFG_DW_REG_NUMBER  4

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
**/
VOID
GpioPadCfgRegValueFromGpioConfig (
  IN  GPIO_PAD           GpioPad,
  IN  CONST GPIO_CONFIG  *GpioConfig,
  OUT UINT32             *PadCfgDwReg,
  OUT UINT32             *PadCfgDwRegMask
  );

/**
  Generates GPIO group name from GroupIndex

  @param[in] GroupIndex  Gpio GroupIndex

  @retval CHAR8*  Pointer to the GPIO group name
**/
CONST
CHAR8*
GpioGetGroupName (
  IN UINT32  GroupIndex
  );

#endif // _GPIO_LIBRARY_H_
