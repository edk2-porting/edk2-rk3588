/** @file
  Header file for UserAuthentication formset.

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _USER_AUTHENTICATION_DXE_FORMSET_H_
#define _USER_AUTHENTICATION_DXE_FORMSET_H_

//
// Vendor GUID of the formset
//
#define USER_AUTHENTICATION_FORMSET_GUID \
  { 0x760e3022, 0xf149, 0x4560, {0x9c, 0x6f, 0x33, 0xaa, 0x7d, 0x48, 0x75, 0xfa} }

#define ADMIN_PASSWORD_KEY_ID       0x2001

#define MAX_PASSWORD_LEN  32
#define MIN_PASSWORD_LEN  0

#endif
