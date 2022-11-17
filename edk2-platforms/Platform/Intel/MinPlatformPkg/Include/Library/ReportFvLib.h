/** @file

  Report Firmware Volume (FV) library

  This library installs pre-memory and post-memory firmware volumes.

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _REPORT_FV_LIB_H_
#define _REPORT_FV_LIB_H_

#include <PiPei.h>
#include <Uefi.h>

VOID
ReportPreMemFv (
  VOID
  );

VOID
ReportPostMemFv (
  VOID
  );

#endif