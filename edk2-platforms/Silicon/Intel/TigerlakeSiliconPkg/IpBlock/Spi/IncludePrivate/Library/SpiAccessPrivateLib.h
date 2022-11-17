/** @file
  SPI library header for abstraction of SPI HW registers accesses

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SPI_ACCESS_PRIVATE_LIB_H_
#define _SPI_ACCESS_PRIVATE_LIB_H_

/**
  Disable EISS (Enable InSMM.STS)
**/
VOID
SpiDisableEiss (
  VOID
  );

/**
  Configure BiosLockEnable bit and BiosInterfaceLock bit according to policy setting.

  @param[in] BiosLockEnable     Policy for BiosLockEnable bit programming
  @param[in] BiosInterfaceLock  Policy for BiosInterfaceLock bit programming

**/
VOID
SpiBiosLockEnableAndBiosInterfaceLockWithS3BootScript (
  IN BOOLEAN  BiosLockEnable,
  IN BOOLEAN  BiosInterfaceLock
  );

/**
  Clears BIOS Write Protect Disable bit
**/
VOID
SpiClearBiosWriteProtectDisable (
  VOID
  );

#endif // _SPI_ACCESS_PRIVATE_LIB_H_
