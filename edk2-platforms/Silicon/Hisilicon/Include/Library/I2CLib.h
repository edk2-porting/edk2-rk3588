/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/


#ifndef _I2C_LIB_H_
#define _I2C_LIB_H_

//I2C0 or I2C1
typedef enum {
  DEVICE_TYPE_SPD = 0,
  DEVICE_TYPE_E2PROM,
  DEVICE_TYPE_CPLD_3BYTE_OPERANDS,
  DEVICE_TYPE_CPLD_4BYTE_OPERANDS
}I2C_DEVICE_TYPE;


typedef enum {
  Normal = 0,
  Fast,
  SPEED_MODE_MAX
}SPEED_MODE;


#define    I2C_PORT_MAX            10



typedef struct {
    UINT32           Socket;
    UINT32           Port;
    I2C_DEVICE_TYPE  DeviceType;
    UINT32           SlaveDeviceAddress;
}I2C_DEVICE;


UINTN
EFIAPI
I2CInit(UINT32 Socket, UINT32 Port, SPEED_MODE SpeedMode);

EFI_STATUS
EFIAPI
I2CWrite(I2C_DEVICE *I2cInfo, UINT16 InfoOffset, UINT32 ulLength, UINT8 *pBuf);

EFI_STATUS
EFIAPI
I2CRead(I2C_DEVICE *I2cInfo, UINT16 InfoOffset,UINT32 ulRxLen,UINT8 *pBuf);

EFI_STATUS
EFIAPI
I2CWriteMultiByte(I2C_DEVICE *I2cInfo, UINT32 InfoOffset, UINT32 ulLength, UINT8 *pBuf);

EFI_STATUS
EFIAPI
I2CReadMultiByte(I2C_DEVICE *I2cInfo, UINT32 InfoOffset,UINT32 ulRxLen,UINT8 *pBuf);

EFI_STATUS
EFIAPI
I2CSdaConfig(UINT32 Socket, UINT32 Port);


#endif
