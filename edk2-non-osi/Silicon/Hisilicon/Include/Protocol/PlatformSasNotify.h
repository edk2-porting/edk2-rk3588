/** @file
*
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#ifndef _PLATFORM_SAS_NOTIFY_H_
#define _PLATFORM_SAS_NOTIFY_H_

typedef struct _PLATFORM_SAS_NOTIFY PLATFORM_SAS_NOTIFY;

struct _PLATFORM_SAS_NOTIFY {
  EFI_EVENT               WaitDiskEvent;
};

extern EFI_GUID gPlatformSasNotifyProtocolGuid;

#endif
