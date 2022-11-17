/** @file
*
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef _HISI_PLATFORM_SAS_PROTOCOL_H_
#define _HISI_PLATFORM_SAS_PROTOCOL_H_

typedef struct _HISI_PLATFORM_SAS_PROTOCOL HISI_PLATFORM_SAS_PROTOCOL;

struct _HISI_PLATFORM_SAS_PROTOCOL {
  UINT32        ControllerId;
  BOOLEAN       Enable;
  UINT64        BaseAddr;
  UINT64        ResetAddr;
};

extern EFI_GUID gHisiPlatformSasProtocolGuid;

#endif
