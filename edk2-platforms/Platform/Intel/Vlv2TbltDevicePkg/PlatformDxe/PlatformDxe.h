/*++

  Copyright (c) 2004  - 2019, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

  PlatformDxe.h

Abstract:

  Header file for Platform Initialization Driver.



++*/

#ifndef _PLATFORM_DRIVER_H
#define _PLATFORM_DRIVER_H

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/HobLib.h>
#include <Library/EfiRegTableLib.h>
#include <Library/Tpm2CommandLib.h>
#include <Library/Tpm2DeviceLib.h>
#include <Library/BaseCryptLib.h>
#include <Library/BiosIdLib.h>
#include <Protocol/GlobalNvsArea.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/CpuIo2.h>
#include <Guid/GlobalVariable.h>
#include <Guid/BoardFeatures.h>
#include <Protocol/PciIo.h>
#include <Protocol/Speaker.h>
#include <IndustryStandard/Pci22.h>
#include <Guid/SetupVariable.h>
#include <Guid/PlatformInfo.h>
#include "Configuration.h"
#include "PchAccess.h"
#include "VlvAccess.h"
#include "BoardIdDecode.h"
#include "PlatformBaseAddresses.h"
#include "SetupMode.h"
#include "PlatformBootMode.h"
#include "CpuType.h"

#define PCAT_RTC_ADDRESS_REGISTER   0x74
#define PCAT_RTC_DATA_REGISTER      0x75

#define RTC_ADDRESS_SECOND_ALARM    0x01
#define RTC_ADDRESS_MINUTE_ALARM    0x03
#define RTC_ADDRESS_HOUR_ALARM      0x05

#define RTC_ADDRESS_REGISTER_A      0x0A
#define RTC_ADDRESS_REGISTER_B      0x0B
#define RTC_ADDRESS_REGISTER_C      0x0C
#define RTC_ADDRESS_REGISTER_D      0x0D

#define B_RTC_ALARM_INT_ENABLE      0x20
#define B_RTC_ALARM_INT_STATUS      0x20

#define B_RTC_DATE_ALARM_MASK       0x3F

//
// Platform driver GUID
//
#define PLATFORM_NORMAL_MODE          0x01
#define PLATFORM_SAFE_MODE            0x02
#define PLATFORM_RECOVERY_MODE        0x04
#define PLATFORM_MANUFACTURING_MODE   0x08
#define PLATFORM_BACK_TO_BIOS_MODE    0x10

#define EFI_OEM_SPECIFIC      0x8000
#define EFI_CU_PLATFORM_DXE_INIT                     (EFI_OEM_SPECIFIC | 0x00000011)
#define EFI_CU_PLATFORM_DXE_STEP1                    (EFI_OEM_SPECIFIC | 0x00000012)
#define EFI_CU_PLATFORM_DXE_STEP2                    (EFI_OEM_SPECIFIC | 0x00000013)
#define EFI_CU_PLATFORM_DXE_STEP3                    (EFI_OEM_SPECIFIC | 0x00000014)
#define EFI_CU_PLATFORM_DXE_STEP4                    (EFI_OEM_SPECIFIC | 0x00000015)
#define EFI_CU_PLATFORM_DXE_INIT_DONE                (EFI_OEM_SPECIFIC | 0x00000016)

#pragma pack(1)

typedef UINT64 EFI_BOARD_FEATURES;

typedef struct {
  CHAR8 AaNumber[7];
  UINT8 BoardId;
  EFI_BOARD_FEATURES Features;
  UINT16 SubsystemDeviceId;
  UINT16 AudioSubsystemDeviceId;
  UINT64 AcpiOemTableId;
} BOARD_ID_DECODE;

#pragma pack()

//
// Prototypes
//
VOID
EFIAPI
ReadyToBootFunction (
  EFI_EVENT  Event,
  VOID       *Context
  );

VOID
EFIAPI
IdeDataFilter (
  IN EFI_EVENT    Event,
  IN VOID*        Context
  );

VOID
SwapEntries (
  IN  CHAR8 *Data
  );

VOID
AsciiToUnicode (
  IN    CHAR8     *AsciiString,
  IN    CHAR16    *UnicodeString
  );

VOID
EFIAPI
PciBusEvent (
  IN EFI_EVENT    Event,
  IN VOID*        Context
  );

UINT8
ReadCmosBank1Byte (
  IN  EFI_CPU_IO2_PROTOCOL            *CpuIo,
  IN  UINT8                           Index
  );

VOID
WriteCmosBank1Byte (
  IN  EFI_CPU_IO2_PROTOCOL            *CpuIo,
  IN  UINT8                           Index,
  IN  UINT8                           Data
  );

VOID
InitializeBoardId (
  );

EFI_STATUS
EFIAPI
ProgramToneFrequency (
  IN  EFI_SPEAKER_IF_PROTOCOL           *This,
  IN  UINT16                            Frequency
  );

EFI_STATUS
EFIAPI
GenerateBeepTone (
  IN  EFI_SPEAKER_IF_PROTOCOL           *This,
  IN  UINTN                             NumberOfBeeps,
  IN  UINTN                             BeepDuration,
  IN  UINTN                             TimeInterval
  );

EFI_STATUS
InitializeObservableProtocol();

EFI_STATUS
PciBusDriverHook();

VOID
EFIAPI
AdjustDefaultRtcTimeCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  );

//
// Global externs
//
extern SYSTEM_CONFIGURATION mSystemConfiguration;

extern EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *mPciRootBridgeIo;

extern UINT8 mBoardIdIndex;
extern BOOLEAN mFoundAANum;
extern EFI_BOARD_FEATURES mBoardFeatures;
extern UINT16 mSubsystemDeviceId;
extern UINT16 mSubsystemAudioDeviceId;
extern BOARD_ID_DECODE mBoardIdDecodeTable[];
extern UINTN mBoardIdDecodeTableSize;

extern UINT8 mSmbusRsvdAddresses[];
extern UINT8 mNumberSmbusAddress;
extern BOOLEAN mMfgMode;
extern UINT32 mPlatformBootMode;
extern CHAR8 BoardAaNumber[];

#endif
