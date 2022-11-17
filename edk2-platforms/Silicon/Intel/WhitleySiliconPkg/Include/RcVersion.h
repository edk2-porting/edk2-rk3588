/** @file
  RC Version header file.

  @copyright
  Copyright 2018 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _RC_VERSION_H_
#define _RC_VERSION_H_

///
/// RC version number structure.
///
typedef struct {
  UINT8   Major;
  UINT8   Minor;
  UINT8   Revision;
  UINT16  BuildNumber;
} RC_VERSION;

#endif // _RC_VERSION_H_
