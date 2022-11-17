/********************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#ifndef __MV_I2C_DXE_H__
#define __MV_I2C_DXE_H__

#include <Uefi.h>

#define I2C_BASE_ADDRESS    0xf0511000

#define I2C_SLAVE_ADDR    0x00
#define I2C_EXT_SLAVE_ADDR  0x10
#define I2C_DATA    0x04

#define I2C_CONTROL    0x08
#define I2C_CONTROL_ACK  (1 << 2)
#define I2C_CONTROL_IFLG  (1 << 3)
#define I2C_CONTROL_STOP  (1 << 4)
#define I2C_CONTROL_START  (1 << 5)
#define I2C_CONTROL_I2CEN  (1 << 6)
#define I2C_CONTROL_INTEN  (1 << 7)

#define I2C_STATUS      0x0c
#define I2C_STATUS_START    0x08
#define I2C_STATUS_RPTD_START    0x10
#define I2C_STATUS_ADDR_W_ACK    0x18
#define I2C_STATUS_DATA_WR_ACK    0x28
#define I2C_STATUS_ADDR_R_ACK    0x40
#define I2C_STATUS_DATA_RD_ACK    0x50
#define I2C_STATUS_DATA_RD_NOACK  0x58

#define I2C_BAUD_RATE    0x0c
#define I2C_BAUD_RATE_PARAM(M,N)  ((((M) << 3) | ((N) & 0x7)) & 0x7f)
#define I2C_BAUD_RATE_RAW(C,M,N)  ((C)/((10*(M+1))<<(N+1)))
#define I2C_M_FROM_BAUD(baud)     (((baud) >> 3) & 0xf)
#define I2C_N_FROM_BAUD(baud)     ((baud) & 0x7)

#define I2C_SOFT_RESET    0x1c
#define I2C_TRANSFER_TIMEOUT 10000
#define I2C_OPERATION_TIMEOUT 100

#define I2C_UNKNOWN        0x0
#define I2C_SLOW           0x1
#define I2C_FAST           0x2
#define I2C_FASTEST        0x3

#define I2C_GUID \
  { \
  0xadc1901b, 0xb83c, 0x4831, { 0x8f, 0x59, 0x70, 0x89, 0x8f, 0x26, 0x57, 0x1e } \
  }

#define I2C_MASTER_SIGNATURE          SIGNATURE_32 ('I', '2', 'C', 'M')

typedef struct {
  UINT32      Signature;
  EFI_HANDLE  Controller;
  EFI_LOCK    Lock;
  UINTN       TclkFrequency;
  UINTN       BaseAddress;
  INTN        Bus;
  EFI_I2C_MASTER_PROTOCOL I2cMaster;
  EFI_I2C_ENUMERATE_PROTOCOL I2cEnumerate;
  EFI_I2C_BUS_CONFIGURATION_MANAGEMENT_PROTOCOL I2cBusConf;
} I2C_MASTER_CONTEXT;

#define I2C_SC_FROM_MASTER(a) CR (a, I2C_MASTER_CONTEXT, I2cMaster, I2C_MASTER_SIGNATURE)
#define I2C_SC_FROM_ENUMERATE(a) CR (a, I2C_MASTER_CONTEXT, I2cEnumerate, I2C_MASTER_SIGNATURE)
#define I2C_SC_FROM_BUSCONF(a) CR (a, I2C_MASTER_CONTEXT, I2cBusConf, I2C_MASTER_SIGNATURE)

typedef struct {
  UINT32  raw;
  UINTN    param;
  UINTN    m;
  UINTN    n;
} MV_I2C_BAUD_RATE;

typedef struct {
  VENDOR_DEVICE_PATH            Guid;
  UINTN                         Instance;
  EFI_DEVICE_PATH_PROTOCOL      End;
} MV_I2C_DEVICE_PATH;

STATIC
UINT32
I2C_READ(
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINTN off
  );

STATIC
EFI_STATUS
I2C_WRITE (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINTN off,
  IN UINT32 val
  );

EFI_STATUS
EFIAPI
MvI2cInitialise (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  );

STATIC
VOID
MvI2cControlClear (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINT32 mask
  );

STATIC
VOID
MvI2cControlSet (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINT32 mask
  );

STATIC
VOID
MvI2cClearIflg (
 IN I2C_MASTER_CONTEXT *I2cMasterContext
 );
STATIC
UINTN
MvI2cPollCtrl (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINTN timeout,
  IN UINT32 mask
  );

STATIC
EFI_STATUS
MvI2cLockedStart (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN INT32 mask,
  IN UINT8 slave,
  IN UINTN timeout
  );

STATIC
VOID
MvI2cCalBaudRate (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN CONST UINT32 target,
  IN OUT MV_I2C_BAUD_RATE *rate,
  UINT32 clk
  );

EFI_STATUS
EFIAPI
MvI2cReset (
  IN CONST EFI_I2C_MASTER_PROTOCOL *This
  );

STATIC
EFI_STATUS
MvI2cRepeatedStart (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINT8 slave,
  IN UINTN timeout
  );

STATIC
EFI_STATUS
MvI2cStart (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINT8 slave,
  IN UINTN timeout
  );

STATIC
EFI_STATUS
MvI2cStop (
  IN I2C_MASTER_CONTEXT *I2cMasterContext
  );

STATIC
EFI_STATUS
MvI2cRead (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN OUT UINT8 *buf,
  IN UINTN len,
  IN OUT UINTN *read,
  IN UINTN last,
  IN UINTN delay
  );

STATIC
EFI_STATUS
MvI2cWrite (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN OUT CONST UINT8 *buf,
  IN UINTN len,
  IN OUT UINTN *sent,
  IN UINTN timeout
  );

STATIC
EFI_STATUS
EFIAPI
MvI2cStartRequest (
  IN CONST EFI_I2C_MASTER_PROTOCOL *This,
  IN UINTN                         SlaveAddress,
  IN EFI_I2C_REQUEST_PACKET        *RequestPacket,
  IN EFI_EVENT                     Event      OPTIONAL,
  OUT EFI_STATUS                   *I2cStatus OPTIONAL
  );

STATIC
EFI_STATUS
EFIAPI
MvI2cEnumerate (
  IN CONST EFI_I2C_ENUMERATE_PROTOCOL *This,
  IN OUT CONST EFI_I2C_DEVICE         **Device
  );

STATIC
EFI_STATUS
EFIAPI
MvI2cEnableConf (
  IN CONST EFI_I2C_BUS_CONFIGURATION_MANAGEMENT_PROTOCOL *This,
  IN UINTN                                               I2cBusConfiguration,
  IN EFI_EVENT                                           Event      OPTIONAL,
  IN EFI_STATUS                                          *I2cStatus OPTIONAL
  );

#endif // __MV_I2C_DXE_H__
