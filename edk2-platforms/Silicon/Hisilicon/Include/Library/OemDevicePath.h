/** @file
*
*  Copyright (c) 2015 - 2017, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015 - 2017, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef _OEM_DEVICE_PATH_H_
#define _OEM_DEVICE_PATH_H_
#include <Protocol/DevicePath.h>

typedef enum
{
    C_NIC  = 1,
    C_SATA = 2,
    C_SAS  = 3,
    C_USB  = 4,
} CONTROLLER_TYPE;

typedef struct{
  VENDOR_DEVICE_PATH Vender;
  UINT8 ControllerType;
  UINT8 Socket;
  UINT8 Port;
} EXT_VENDOR_DEVICE_PATH;

typedef struct{
  UINT16  BootIndex;
  UINT16  Port;
} SATADES;

typedef struct{
  UINT16  BootIndex;
  UINT16  ParentPortNumber;
  UINT16  InterfaceNumber;
} USBDES;

typedef struct{
  UINT16  BootIndex;
  UINT16  Port;
} PXEDES;

#endif

