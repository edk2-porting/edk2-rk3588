/** @file
*
*  Copyright (c) 2017, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2017, Linaro Limited. All rights reserved.
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

#ifndef _IPMI_CMD_LIB_H_
#define _IPMI_CMD_LIB_H_

#define BOOT_OPTION_BOOT_FLAG_VALID         1
#define BOOT_OPTION_BOOT_FLAG_INVALID       0

typedef enum {
  EfiReserved,
  EfiBiosFrb2,
  EfiBiosPost,
  EfiOsLoad,
  EfiSmsOs,
  EfiOem,
  EfiFrbReserved1,
  EfiFrbReserved2
} EFI_WDT_USER_TYPE;

typedef enum {
  NoOverride = 0x0,
  ForcePxe,
  ForceDefaultHardDisk,
  ForceDefaultHardDiskSafeMode,
  ForceDefaultDiagnosticPartition,
  ForceDefaultCD,
  ForceSetupUtility,
  ForceRemoteRemovableMedia,
  ForceRemoteCD,
  ForcePrimaryRemoteMedia,
  ForceRemoteHardDisk = 0xB,
  ForcePrimaryRemovableMedia = 0xF
} BOOT_DEVICE_SELECTOR;

//
// Get System Boot Option data structure
//
typedef struct {
  UINT8 ParameterVersion           :4;
  UINT8 Reserved1                  :4;
  UINT8 ParameterSelector          :7;
  UINT8 ParameterValid             :1;
  //
  // Boot Flags Data 1
  //
  UINT8 Reserved2                  :5;
  UINT8 BiosBootType               :1;
  UINT8 Persistent                 :1;
  UINT8 BootFlagsValid             :1;
  //
  // Boot Flags Data 2
  //
  UINT8 LockResetBtn               :1;
  UINT8 ScreenBlank                :1;
  UINT8 BootDeviceSelector         :4;
  UINT8 LockKeyboard               :1;
  UINT8 ClearCmos                  :1;
  //
  // Boot Flags Data 3
  //
  UINT8 ConsoleRedirectionControl  :2;
  UINT8 LockSleepBtn               :1;
  UINT8 UserPasswordByPass         :1;
  UINT8 Reserved3                  :1;
  UINT8 FirmwareVerbosity          :2;
  UINT8 LockPowerBtn               :1;
  //
  // Boot Flags Data 4
  //
  UINT8 MuxControlOverride         :3;
  UINT8 ShareModeOverride          :1;
  UINT8 Reserved4                  :4;
  //
  // Boot Flags Data 5
  //
  UINT8 DeviceInstanceSelector     :5;
  UINT8 Reserved5                  :3;
} IPMI_GET_BOOT_OPTION;

EFI_STATUS
EFIAPI
IpmiCmdSetSysBootOptions (
  OUT IPMI_GET_BOOT_OPTION  *BootOption
  );

EFI_STATUS
EFIAPI
IpmiCmdGetSysBootOptions (
  IN IPMI_GET_BOOT_OPTION   *BootOption
  );

EFI_STATUS
IpmiCmdStopWatchdogTimer (
  IN EFI_WDT_USER_TYPE  UserType
  );

#endif
