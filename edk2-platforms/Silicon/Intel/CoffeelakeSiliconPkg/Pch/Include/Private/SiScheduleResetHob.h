/** @file
  This file contains definitions of Si Schedule Reset HOB.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SI_SCHEDULE_RESET_HOB_H_
#define _SI_SCHEDULE_RESET_HOB_H_

#include <PchResetPlatformSpecific.h>

/**
  This structure is used to provide information about PCH Resets
**/
typedef struct {
  EFI_RESET_TYPE             ResetType;
  PCH_RESET_DATA             ResetData;
} SI_SCHEDULE_RESET_HOB;

extern EFI_GUID gSiScheduleResetHobGuid;

#endif // _SI_SCHEDULE_RESET_HOB_H_

