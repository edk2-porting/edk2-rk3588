/** @file
 *
 *  Copyright (c) 2019, ARM Limited. All rights reserved.
 *  Copyright (c) 2017 - 2020, Andrei Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) 2016, Linaro Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __RASPBERRY_PI_FIRMWARE_PROTOCOL_H__
#define __RASPBERRY_PI_FIRMWARE_PROTOCOL_H__

#define RASPBERRY_PI_FIRMWARE_PROTOL_GUID \
  { 0x0ACA9535, 0x7AD0, 0x4286, { 0xB0, 0x2E, 0x87, 0xFA, 0x7E, 0x2A, 0x57, 0x11 } }

typedef
EFI_STATUS
(EFIAPI *SET_POWER_STATE) (
  IN  UINT32    DeviceId,
  IN  BOOLEAN   PowerState,
  IN  BOOLEAN   Wait
  );

typedef
EFI_STATUS
(EFIAPI *GET_MAC_ADDRESS) (
  OUT UINT8     MacAddress[6]
  );

typedef
EFI_STATUS
(EFIAPI *GET_COMMAND_LINE) (
  IN  UINTN     BufferSize,
  OUT CHAR8     CommandLine[]
  );

typedef
EFI_STATUS
(EFIAPI *GET_CLOCK_STATE) (
  IN  UINT32    ClockId,
  OUT UINT32    *ClockState
  );

typedef
EFI_STATUS
(EFIAPI *SET_CLOCK_STATE) (
  IN  UINT32 ClockId,
  IN  UINT32 ClockState
  );

typedef
EFI_STATUS
(EFIAPI *GET_CLOCK_RATE) (
  IN  UINT32    ClockId,
  OUT UINT32    *ClockRate
  );

typedef
EFI_STATUS
(EFIAPI *SET_CLOCK_RATE) (
  IN  UINT32    ClockId,
  OUT UINT32    ClockRate,
  IN  BOOLEAN   SkipTurbo
  );

typedef
EFI_STATUS
(EFIAPI *GET_FB) (
  IN  UINT32 Width,
  IN  UINT32 Height,
  IN  UINT32 Depth,
  OUT EFI_PHYSICAL_ADDRESS *FbBase,
  OUT UINTN *FbSize,
  OUT UINTN *Pitch
  );

typedef
EFI_STATUS
(EFIAPI *GET_FB_SIZE) (
  OUT   UINT32 *Width,
  OUT   UINT32 *Height
  );

typedef
EFI_STATUS
(EFIAPI *FREE_FB) (
  VOID
  );

typedef
VOID
(EFIAPI *SET_LED) (
  BOOLEAN On
  );

typedef
EFI_STATUS
(EFIAPI *GET_SERIAL) (
  UINT64 *Serial
  );

typedef
EFI_STATUS
(EFIAPI *GET_MODEL) (
  UINT32 *Model
  );

typedef
EFI_STATUS
(EFIAPI *GET_MODEL_REVISION) (
  UINT32 *Revision
  );

typedef
CHAR8*
(EFIAPI *GET_MODEL_NAME) (
  INTN ModelId
  );

typedef
EFI_STATUS
(EFIAPI *GET_MODEL_FAMILY) (
  UINT32 *ModelFamily
  );

typedef
EFI_STATUS
(EFIAPI *GET_FIRMWARE_REVISION) (
  UINT32 *Revision
  );

typedef
EFI_STATUS
(EFIAPI *GET_MODEL_INSTALLED_MB) (
  UINT32 *InstalledMB
  );

typedef
CHAR8*
(EFIAPI *GET_MANUFACTURER_NAME) (
  INTN ManufacturerId
  );

typedef
CHAR8*
(EFIAPI *GET_CPU_NAME) (
  INTN CpuId
  );

typedef
EFI_STATUS
(EFIAPI *GET_ARM_MEM) (
  UINT32 *Base,
  UINT32 *Size
  );

typedef
EFI_STATUS
(EFIAPI *NOTIFY_XHCI_RESET) (
  UINTN BusNumber,
  UINTN DeviceNumber,
  UINTN FunctionNumber
  );

typedef
EFI_STATUS
(EFIAPI *GPIO_SET_CFG) (
  UINTN Gpio,
  UINTN Direction,
  UINTN State
  );

typedef struct {
  SET_POWER_STATE        SetPowerState;
  GET_MAC_ADDRESS        GetMacAddress;
  GET_COMMAND_LINE       GetCommandLine;
  GET_CLOCK_RATE         GetClockRate;
  GET_CLOCK_RATE         GetMaxClockRate;
  GET_CLOCK_RATE         GetMinClockRate;
  SET_CLOCK_RATE         SetClockRate;
  GET_FB                 GetFB;
  FREE_FB                FreeFB;
  GET_FB_SIZE            GetFBSize;
  SET_LED                SetLed;
  GET_SERIAL             GetSerial;
  GET_MODEL              GetModel;
  GET_MODEL_REVISION     GetModelRevision;
  GET_MODEL_NAME         GetModelName;
  GET_MODEL_FAMILY       GetModelFamily;
  GET_FIRMWARE_REVISION  GetFirmwareRevision;
  GET_MANUFACTURER_NAME  GetManufacturerName;
  GET_CPU_NAME           GetCpuName;
  GET_ARM_MEM            GetArmMem;
  GET_MODEL_INSTALLED_MB GetModelInstalledMB;
  NOTIFY_XHCI_RESET      NotifyXhciReset;
  GET_CLOCK_STATE        GetClockState;
  SET_CLOCK_STATE        SetClockState;
  GPIO_SET_CFG           SetGpioConfig;
} RASPBERRY_PI_FIRMWARE_PROTOCOL;

extern EFI_GUID gRaspberryPiFirmwareProtocolGuid;

#endif /* __RASPBERRY_PI_FIRMWARE_PROTOCOL_H__ */
