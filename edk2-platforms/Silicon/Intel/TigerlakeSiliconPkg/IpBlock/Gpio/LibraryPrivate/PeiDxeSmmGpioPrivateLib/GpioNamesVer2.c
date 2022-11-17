/** @file
  This file contains GPIO name library implementation specific to Ver2

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Base.h>
#include <Library/BaseLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/PchInfoLib.h>
#include <Library/GpioPrivateLib.h>
#include <Pins/GpioPinsVer2Lp.h>

STATIC CONST CHAR8*  mGpioGppbNames[] = {
  "GSPI0_CLK_LOOPBK",
  "GSPI1_CLK_LOOPBK"
};

STATIC CONST CHAR8*  mGpioGppaNames[] = {
  "SPI0_CLK_LOOPBK",
  "ESPI_CLK_LOOPBK"
};

STATIC CONST CHAR8*  mPchLpGpioGpdNames[] = {
  "INPUT3VSEL",
  "SLP_LANB",
  "SLP_SUSB",
  "SLP_WAKEB",
  "SLP_DRAM_RESETB"
};

STATIC CONST CHAR8*  mPchLpGpioGppdNames[] = {
  "GSPI2_CLK_LOOPBK"
};

STATIC CONST CHAR8*  mGpioGppfNames[] = {
  "GPPF_CLK_LOOPBK"
};

STATIC CONST CHAR8*  mGpioGppeNames[] = {
  "GPPE_CLK_LOOPBK"
};


STATIC CONST GPIO_GROUP_NAME_INFO  mPchLpGroupDescriptors[] = {
  GPIO_GROUP_NAME("GPP_B", GPIO_VER2_LP_GSPI0_CLK_LOOPBK, mGpioGppbNames),
  GPIO_GROUP_NAME_BASIC(""),
  GPIO_GROUP_NAME("GPP_A", GPIO_VER2_LP_ESPI_CLK_LOOPBK, mGpioGppaNames),
  GPIO_GROUP_NAME_BASIC("GPP_R"),
  GPIO_GROUP_NAME_BASIC(""),
  GPIO_GROUP_NAME("GPD", GPIO_VER2_LP_INPUT3VSEL, mPchLpGpioGpdNames),
  GPIO_GROUP_NAME_BASIC("GPP_S"),
  GPIO_GROUP_NAME_BASIC("GPP_H"),
  GPIO_GROUP_NAME("GPP_D", GPIO_VER2_LP_GSPI2_CLK_LOOPBK, mPchLpGpioGppdNames),
  GPIO_GROUP_NAME_BASIC(""),
  GPIO_GROUP_NAME_BASIC(""),
  GPIO_GROUP_NAME_BASIC("GPP_C"),
  GPIO_GROUP_NAME("GPP_F", GPIO_VER2_LP_GPPF_CLK_LOOPBK, mGpioGppfNames),
  GPIO_GROUP_NAME_BASIC(""),
  GPIO_GROUP_NAME("GPP_E", GPIO_VER2_LP_GPPE_CLK_LOOPBK, mGpioGppeNames),
  GPIO_GROUP_NAME_BASIC(""),
  GPIO_GROUP_NAME_BASIC(""),
  GPIO_GROUP_NAME_BASIC("")
};

/**
  Returns GPIO_GROUP_NAME_INFO corresponding to the given GpioPad

  @param[in] GroupIndex  Group index

  @retval GPIO_GROUP_NAME_INFO*  Pointer to the GPIO_GROUP_NAME_INFO
  @reval  NULL                   If no group descriptor was found
**/
CONST
GPIO_GROUP_NAME_INFO*
GpioGetGroupNameInfo (
  IN UINT32  GroupIndex
  )
{
  if (GroupIndex < ARRAY_SIZE (mPchLpGroupDescriptors)) {
    return &mPchLpGroupDescriptors[GroupIndex];
  }

  ASSERT (FALSE);
  return NULL;
}

