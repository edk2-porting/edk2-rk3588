/** @file
  Header file for BiosLockLib.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _BIOSLOCK_LIB_H_
#define _BIOSLOCK_LIB_H_

/**
  Enable BIOS lock. This will set the LE (Lock Enable) and EISS (Enable In SMM.STS).
  When this is set, attempts to write the WPD (Write Protect Disable) bit in PCH
  will cause a SMI which will allow the BIOS to verify that the write is from a valid source.

  Bios should always enable LockDownConfig.BiosLock policy to set Bios Lock bit in FRC.
  If capsule udpate is enabled, it's expected to not do BiosLock by setting BiosLock policy disable
  so it can udpate BIOS region.
  After flash update, it should utilize this lib to do BiosLock for security.
**/
VOID
BiosLockEnable (
  VOID
  );

#endif // _BIOSLOCK_LIB_H_
