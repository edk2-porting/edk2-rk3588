/********************************************************************************
Copyright (C) 2021 Rockchip Electronics Co., Ltd

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#ifndef __RK_I2C_DXE_H__
#define __RK_I2C_DXE_H__

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

#define I2C_BAUD_RATE_REG    0x0c
#define I2C_BAUD_RATE_PARAM(M,N)  ((((M) << 3) | ((N) & 0x7)) & 0x7f)
#define I2C_BAUD_RATE_RAW(C,M,N)  ((C)/((10*(M+1))<<(N+1)))
#define I2C_M_FROM_BAUD(baud)     (((baud) >> 3) & 0xf)
#define I2C_N_FROM_BAUD(baud)     ((baud) & 0x7)

#define I2C_SOFT_RESET    0x1c
#define I2C_TRANSFER_TIMEOUT 10000
#define I2C_READY_TIMEOUT 100

#define I2C_UNKNOWN        0x0
#define I2C_SLOW           0x1
#define I2C_FAST           0x2
#define I2C_FASTEST        0x3

#define DIV_ROUND_UP(n, d)  (((n) + (d) - 1) / (d))

/* rk i2c fifo max transfer bytes */
#define RK_I2C_FIFO_SIZE	32

/* rk i2c device register size */
#define RK_I2C_REGISTER_SIZE	3

/* i2c timerout */
#define I2C_TIMEOUT_US		100000  // 100000us = 100ms
#define I2C_RETRY_COUNT		3

#define I2C_ADAP_SEL_BIT(nr)	((nr) + 11)
#define I2C_ADAP_SEL_MASK(nr)	((nr) + 27)

#define RK_CEIL(x, y) \
	({ unsigned long __x = (x), __y = (y); (__x + __y - 1) / __y; })

/* Control register */
#define I2C_CON			0x000
#define I2C_CON_EN		(1 << 0)
#define I2C_CON_MOD(mod)	((mod) << 1)
#define I2C_MODE_TX		0x00
#define I2C_MODE_TRX		0x01
#define I2C_MODE_RX		0x02
#define I2C_MODE_RRX		0x03
#define I2C_CON_MASK		(3 << 1)

#define I2C_CON_START		(1 << 3)
#define I2C_CON_STOP		(1 << 4)
#define I2C_CON_LASTACK		(1 << 5)
#define I2C_CON_ACTACK		(1 << 6)

#define I2C_CON_SDA_CFG(cfg)	((cfg) << 8)
#define I2C_CON_STA_CFG(cfg)	((cfg) << 12)
#define I2C_CON_STO_CFG(cfg)	((cfg) << 14

#define I2C_CON_VERSION	        (0xff << 16)
#define I2C_CON_VERSION_SHIFT	16

/* Clock dividor register */
#define I2C_CLKDIV		0x004
#define I2C_CLK_DIV_HIGH_SHIFT	16
#define I2C_CLKDIV_VAL(divl, divh)	(((divl) & 0xffff) | (((divh) << 16) & 0xffff0000))

/* the slave address accessed  for master rx mode */
#define I2C_MRXADDR		0x008
#define I2C_MRXADDR_SET(vld, addr)	(((vld) << 24) | (addr))

/* the slave register address accessed  for master rx mode */
#define I2C_MRXRADDR		0x00c
#define I2C_MRXRADDR_SET(vld, raddr)	(((vld) << 24) | (raddr))

/* master tx count */
#define I2C_MTXCNT		0x010

/* master rx count */
#define I2C_MRXCNT		0x014

/* interrupt enable register */
#define I2C_IEN			0x018
#define I2C_BTFIEN		(1 << 0)
#define I2C_BRFIEN		(1 << 1)
#define I2C_MBTFIEN		(1 << 2)
#define I2C_MBRFIEN		(1 << 3)
#define I2C_STARTIEN		(1 << 4)
#define I2C_STOPIEN		(1 << 5)
#define I2C_NAKRCVIEN		(1 << 6)

/* interrupt pending register */
#define I2C_IPD                 0x01c
#define I2C_BTFIPD              (1 << 0)
#define I2C_BRFIPD              (1 << 1)
#define I2C_MBTFIPD             (1 << 2)
#define I2C_MBRFIPD             (1 << 3)
#define I2C_STARTIPD            (1 << 4)
#define I2C_STOPIPD             (1 << 5)
#define I2C_NAKRCVIPD           (1 << 6)
#define I2C_IPD_ALL_CLEAN       0x7f

/* finished count */
#define I2C_FCNT                0x020

/* I2C tx data register */
#define I2C_TXDATA_BASE         0X100

/* I2C rx data register */
#define I2C_RXDATA_BASE         0x200


#define I2C_GUID \
  { \
  0xadc1901b, 0xb83c, 0x4831, { 0x8f, 0x59, 0x70, 0x89, 0x8f, 0x26, 0x57, 0x1e } \
  }

#define I2C_MASTER_SIGNATURE          SIGNATURE_32 ('I', '2', 'C', 'M')

typedef struct {
  UINT32 Signature;
  EFI_HANDLE Controller;
  EFI_LOCK Lock;
  UINTN TclkFrequency;
  UINTN BaseAddress;
  INTN Bus;
  UINTN Config;
  EFI_I2C_MASTER_PROTOCOL I2cMaster;
  EFI_I2C_ENUMERATE_PROTOCOL I2cEnumerate;
  EFI_I2C_BUS_CONFIGURATION_MANAGEMENT_PROTOCOL I2cBusConf;
} I2C_MASTER_CONTEXT;

#define I2C_SC_FROM_MASTER(a) CR (a, I2C_MASTER_CONTEXT, I2cMaster, I2C_MASTER_SIGNATURE)
#define I2C_SC_FROM_ENUMERATE(a) CR (a, I2C_MASTER_CONTEXT, I2cEnumerate, I2C_MASTER_SIGNATURE)
#define I2C_SC_FROM_BUSCONF(a) CR (a, I2C_MASTER_CONTEXT, I2cBusConf, I2C_MASTER_SIGNATURE)

typedef struct {
  VENDOR_DEVICE_PATH Guid;
  UINTN Instance;
  EFI_DEVICE_PATH_PROTOCOL End;
} I2C_DEVICE_PATH;

typedef struct {
  UINT32 MinLowNs;
  UINT32 MinHighNs;
  UINT32 MaxRiseNs;
  UINT32 MaxFallNs;
} I2C_SPEC_VALUES;

typedef enum {
  RkI2cVersion0 = 0,
  RkI2cVersion1,
  RkI2cVersion5 = 5,
} I2C_VERSION;

STATIC
UINT32
I2cRead(
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINTN Off
  );

STATIC
EFI_STATUS
I2cWrite (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINTN Off,
  IN UINT32 Val
  );

STATIC
CONST I2C_SPEC_VALUES *
I2cGetSpec (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINT32 Speed);

EFI_STATUS
EFIAPI
I2cInitialise (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  );

STATIC
VOID
I2cCalBaudRate (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINT32 Target,
  IN UINT32 ClkRate
  );

STATIC
EFI_STATUS
I2cAdapterBaudRate (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN CONST UINT32 Target,
  IN CONST UINT32 ClkRate
  );

EFI_STATUS
EFIAPI
I2cReset (
  IN CONST EFI_I2C_MASTER_PROTOCOL *This
  );

STATIC
VOID
I2cDisable (
  IN I2C_MASTER_CONTEXT *I2cMasterContext
  );

STATIC
EFI_STATUS
I2cStartEnable (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINT32 Con,
  IN UINTN Timeout
  );

STATIC
EFI_STATUS
I2cStop (
  IN I2C_MASTER_CONTEXT *I2cMasterContext
  );

STATIC
EFI_STATUS
I2cReadOperation (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINTN SlaveAddress,
  IN OUT UINT8 *buf,
  IN UINTN len,
  IN OUT UINTN *read,
  IN UINTN last,
  IN UINTN delay
  );

STATIC
EFI_STATUS
I2cWriteOperation (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINTN SlaveAddress,
  IN OUT CONST UINT8 *buf,
  IN UINTN len,
  IN OUT UINTN *sent,
  IN UINTN timeout
  );

STATIC
EFI_STATUS
EFIAPI
I2cStartRequest (
  IN CONST EFI_I2C_MASTER_PROTOCOL *This,
  IN UINTN                         SlaveAddress,
  IN EFI_I2C_REQUEST_PACKET        *RequestPacket,
  IN EFI_EVENT                     Event      OPTIONAL,
  OUT EFI_STATUS                   *I2cStatus OPTIONAL
  );

STATIC
EFI_STATUS
EFIAPI
I2cEnumerate (
  IN CONST EFI_I2C_ENUMERATE_PROTOCOL *This,
  IN OUT CONST EFI_I2C_DEVICE         **Device
  );

STATIC
EFI_STATUS
EFIAPI
I2cEnableConf (
  IN CONST EFI_I2C_BUS_CONFIGURATION_MANAGEMENT_PROTOCOL *This,
  IN UINTN                                               I2cBusConfiguration,
  IN EFI_EVENT                                           Event      OPTIONAL,
  IN EFI_STATUS                                          *I2cStatus OPTIONAL
  );

STATIC
I2C_VERSION
I2cGetVersion(
  IN I2C_MASTER_CONTEXT *I2cMasterContext
  );

STATIC
VOID
I2cShowRegs (
  IN I2C_MASTER_CONTEXT *I2cMasterContex
  );

#endif // __RK_I2C_DXE_H__
