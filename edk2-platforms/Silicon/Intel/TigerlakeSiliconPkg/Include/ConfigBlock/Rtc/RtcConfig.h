/** @file
  RTC policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _RTC_CONFIG_H_
#define _RTC_CONFIG_H_

#define RTC_CONFIG_REVISION 1
extern EFI_GUID gRtcConfigGuid;

#pragma pack (push,1)

/**
  The RTC_CONFIG block describes the expected configuration of RTC configuration.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  /**
    When set, prevents RTC TS (BUC.TS) from being changed.
    This BILD bit has different function compared to LPC/eSPI, SPI.
    0: Disabled; <b>1: Enabled</b>
  **/
  UINT32  BiosInterfaceLock       :  1;
  /**
    When set, bytes 38h-3Fh in the upper 128bytes bank of RTC RAM are locked
    and cannot be accessed.
    Writes will be droipped and reads will not return any guaranteed data.
    0: Disabled; <b>1: Enabled</b>
  **/
  UINT32  MemoryLock              :  1;
  UINT32  RsvdBits0               : 30;
} RTC_CONFIG;

#pragma pack (pop)

#endif // _RTC_CONFIG_H_
