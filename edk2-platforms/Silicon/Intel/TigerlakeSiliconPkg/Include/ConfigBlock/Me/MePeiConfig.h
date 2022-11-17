/** @file
  ME config block for PEI phase

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _ME_PEI_CONFIG_H_
#define _ME_PEI_CONFIG_H_

#define ME_PEI_PREMEM_CONFIG_REVISION 2
extern EFI_GUID gMePeiPreMemConfigGuid;

#ifndef PLATFORM_POR
#define PLATFORM_POR  0
#endif
#ifndef FORCE_ENABLE
#define FORCE_ENABLE  1
#endif
#ifndef FORCE_DISABLE
#define FORCE_DISABLE 2
#endif

#pragma pack (push,1)

/**
  ME Pei Pre-Memory Configuration Structure.

  <b>Revision 1:</b>
  - Initial version.
  <b>Revision 2:</b>
  - Add SkipCpuReplacementCheck Option.
  <b>Revision 3:</b>
  - Deprecate SendDidMsg.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                 ///< Config Block Header
  UINT32 HeciTimeouts                     : 1;  ///< 0: Disable; <b>1: Enable</b> - HECI Send/Receive Timeouts.
  /**
    <b>(Test)</b>
    <b>0: Disabled</b>
       1: ME DID init stat 0 - Success
       2: ME DID init stat 1 - No Memory in Channels
       3: ME DID init stat 2 - Memory Init Error
  **/
  UINT32 DidInitStat                      : 2;
  /**
    <b>(Test)</b>
    <b>0: Set to 0 to enable polling for CPU replacement</b>
       1: Set to 1 will disable polling for CPU replacement
  **/
  UINT32 DisableCpuReplacedPolling        : 1;
  UINT32 SendDidMsg                       : 1;  ///< <b>(Deprecated)</b> 0: Disable; <b>1: Enable</b> - Enable/Disable to send DID message.
  /**
    <b>(Test)</b>
    <b>0: ME BIOS will check each messages before sending</b>
       1: ME BIOS always sends messages without checking
  **/
  UINT32 DisableMessageCheck              : 1;
  /**
    <b>(Test)</b>
    The SkipMbpHob policy determines whether ME BIOS Payload data will be requested during boot
    in a MBP message. If set to 1, BIOS will send the MBP message with SkipMbp flag
    set causing CSME to respond with MKHI header only and no MBP data
    <b>0: ME BIOS will keep MBP and create HOB for MBP data</b>
       1: ME BIOS will skip MBP data
  **/
  UINT32 SkipMbpHob                       : 1;
  UINT32 HeciCommunication2               : 1;  ///< <b>(Test)</b> <b>0: Disable</b>; 1: Enable - Enable/Disable HECI2.
  UINT32 KtDeviceEnable                   : 1;  ///< <b>(Test)</b> 0: Disable; <b>1: Enable</b> - Enable/Disable Kt Device.
  UINT32 SkipCpuReplacementCheck          : 1;  ///< <b>(Test)</b> <b>0: Disable</b>; 1: Enable - Enable/Disable to skip CPU replacement check.
  UINT32 RsvdBits                         : 22; ///< Reserved for future use & Config block alignment
  UINT32 Heci1BarAddress;                       ///< HECI1 BAR address.
  UINT32 Heci2BarAddress;                       ///< HECI2 BAR address.
  UINT32 Heci3BarAddress;                       ///< HECI3 BAR address.
} ME_PEI_PREMEM_CONFIG;
#pragma pack (pop)


#define ME_PEI_CONFIG_REVISION 3
extern EFI_GUID gMePeiConfigGuid;

#pragma pack (push,1)

/**
  ME Pei Post-Memory Configuration Structure.

  <b>Revision 1:</b>
  - Initial version.
  <b>Revision 2</b>:
  - Deprecated Heci3Enabled.
  <b>Revision 3</b>
  - Added EnforceEDebugMode.
**/

typedef struct {
  CONFIG_BLOCK_HEADER   Header;                 ///< Config Block Header

  UINT32 EndOfPostMessage                 : 2;  ///< 0: Disabled; 1: Send in PEI; <b>2: Send in DXE</b> - Send EOP at specific phase.
  UINT32 Heci3Enabled                     : 1;  ///< @deprecated
  UINT32 DisableD0I3SettingForHeci        : 1;  ///< <b>(Test)</b> <b>0: Disable</b>; 1: Enable - Enable/Disable D0i3 for HECI.
  /**
    Enable/Disable Me Unconfig On Rtc Clear. If enabled, BIOS will send MeUnconfigOnRtcClearDisable Msg with parameter 0.
    It will cause ME to unconfig if RTC is cleared.
    -    0: Disable
    - <b>1: Enable</b>
    -    2: Cmos is clear, status unkonwn
    -    3: Reserved
  **/
  UINT32 MeUnconfigOnRtcClear             : 2;
  UINT32 MctpBroadcastCycle               : 1;   ///< <b>(Test)</b> <b>0: Disable</b>; 1: Enable - Program registers for MCTP Cycle.
  UINT32 EnforceEDebugMode                : 1;   ///< <b>0: Disable</b>; 1: Enable - Enforces ME to enter Enhanced Debug Mode
  UINT32 RsvdBits                         : 24;  ///< Reserved for future use & Config block alignment
} ME_PEI_CONFIG;

#pragma pack (pop)

#endif // _ME_PEI_CONFIG_H_
