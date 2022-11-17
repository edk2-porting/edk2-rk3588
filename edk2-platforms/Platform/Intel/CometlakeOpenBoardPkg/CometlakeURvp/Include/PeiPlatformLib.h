/** @file

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_PLATFORM_LIB_H_
#define _PEI_PLATFORM_LIB_H_



#define PEI_DEVICE_DISABLED 0
#define PEI_DEVICE_ENABLED  1

typedef struct {
  UINT8   Register;
  UINT32  Value;
} PCH_GPIO_DEV;

//
// GPIO Initialization Data Structure
//
typedef struct{
  PCH_GPIO_DEV Use_Sel;
  PCH_GPIO_DEV Use_Sel2;
  PCH_GPIO_DEV Use_Sel3;
  PCH_GPIO_DEV Io_Sel;
  PCH_GPIO_DEV Io_Sel2;
  PCH_GPIO_DEV Io_Sel3;
  PCH_GPIO_DEV Lvl;
  PCH_GPIO_DEV Lvl2;
  PCH_GPIO_DEV Lvl3;
  PCH_GPIO_DEV Inv;
  PCH_GPIO_DEV Blink;
  PCH_GPIO_DEV Rst_Sel;
  PCH_GPIO_DEV Rst_Sel2;
} GPIO_INIT_STRUCT;

#endif

