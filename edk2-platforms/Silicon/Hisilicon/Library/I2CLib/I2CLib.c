/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/



#include <PiDxe.h>
#include <Library/ArmLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/I2CLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Library/PlatformSysCtrlLib.h>

#include "I2CLibInternal.h"
#include "I2CHw.h"

#define I2C_100KB_SPEED 0x1
#define I2C_400KB_SPEED 0x2

VOID
I2C_Delay (
  UINT32 Count
  )
{
  MicroSecondDelay (Count);
  return;
}


EFI_STATUS
EFIAPI
I2C_Disable (
  UINT32 Socket,
  UINT8  Port
  )
{
  UINT32                  TimeCnt = I2C_READ_TIMEOUT;
  I2C0_STATUS_U           I2cStatusReg;
  I2C0_ENABLE_U           I2cEnableReg;
  I2C0_ENABLE_STATUS_U    I2cEnableStatusReg;

  UINTN Base = GetI2cBase (Socket, Port);

  I2C_REG_READ ((Base + I2C_STATUS_OFFSET), I2cStatusReg.Val32);

  while (I2cStatusReg.bits.activity) {
    I2C_Delay (10000);

    TimeCnt--;
    I2C_REG_READ (Base + I2C_STATUS_OFFSET, I2cStatusReg.Val32);
    if (TimeCnt == 0) {
      return EFI_DEVICE_ERROR;
    }
  }

  I2C_REG_READ (Base + I2C_ENABLE_OFFSET, I2cEnableReg.Val32);
  I2cEnableReg.bits.enable = 0;
  I2C_REG_WRITE (Base + I2C_ENABLE_OFFSET, I2cEnableReg.Val32);

  I2C_REG_READ (Base + I2C_ENABLE_OFFSET, I2cEnableStatusReg.Val32);
  if (I2cEnableStatusReg.bits.ic_en == 0) {
    return EFI_SUCCESS;
  } else {
    return EFI_DEVICE_ERROR;
  }
}


EFI_STATUS
EFIAPI
I2C_Enable (
  UINT32 Socket,
  UINT8  Port
  )
{
  I2C0_ENABLE_U           I2cEnableReg;
  I2C0_ENABLE_STATUS_U    I2cEnableStatusReg;
  UINT32                  TimeCnt = I2C_READ_TIMEOUT;

  UINTN Base = GetI2cBase (Socket, Port);

  I2C_REG_READ (Base + I2C_ENABLE_OFFSET, I2cEnableReg.Val32);
  I2cEnableReg.bits.enable = 1;
  I2C_REG_WRITE (Base + I2C_ENABLE_OFFSET, I2cEnableReg.Val32);

  do {
    // This is a empirical value for I2C delay. MemoryFence is no need here.
    I2C_Delay (10000);

    TimeCnt--;
    I2C_REG_READ (Base + I2C_ENABLE_STATUS_OFFSET, I2cEnableStatusReg.Val32);
    if (TimeCnt == 0) {
      return EFI_DEVICE_ERROR;
    }
  } while (I2cEnableStatusReg.bits.ic_en == 0);

  return EFI_SUCCESS;
}

VOID
I2C_SetTarget (
  UINT32 Socket,
  UINT8  Port,
  UINT32 I2cDeviceAddr
  )
{
  I2C0_TAR_U    I2cTargetReg;

  UINTN Base = GetI2cBase (Socket, Port);

  I2C_REG_READ (Base + I2C_TAR_OFFSET, I2cTargetReg.Val32);
  I2cTargetReg.bits.ic_tar = I2cDeviceAddr;
  I2C_REG_WRITE (Base + I2C_TAR_OFFSET, I2cTargetReg.Val32);

  return;
}


EFI_STATUS
EFIAPI
I2CInit (
  UINT32     Socket,
  UINT32     Port,
  SPEED_MODE SpeedMode
  )
{
  I2C0_CON_U              I2cControlReg;
  I2C0_SS_SCL_HCNT_U      I2cStandardSpeedSclHighCount;
  I2C0_SS_SCL_LCNT_U      I2cStandardSpeedSclLowCount;
  I2C0_RX_TL_U            I2cRxFifoReg;
  I2C0_TX_TL_U            I2cTxFifoReg;
  I2C0_INTR_MASK_U        I2cIntrMask;
  EFI_STATUS              Status;

  UINTN Base = GetI2cBase (Socket, Port);

  if ((Socket >= MAX_SOCKET) ||
     (Port >= I2C_PORT_MAX) ||
     (SpeedMode >= SPEED_MODE_MAX)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = I2C_Disable (Socket,Port);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  I2C_REG_READ (Base + I2C_CON_OFFSET, I2cControlReg.Val32);
  I2cControlReg.bits.master = 1;
  if(SpeedMode == Normal) {
    I2cControlReg.bits.Speed = I2C_100KB_SPEED;
  } else {
    I2cControlReg.bits.Speed = I2C_400KB_SPEED;
  }
  I2cControlReg.bits.restart_en = 1;
  I2cControlReg.bits.slave_disable = 1;
  I2C_REG_WRITE (Base + I2C_CON_OFFSET, I2cControlReg.Val32);

  if (SpeedMode == Normal) {
    I2C_REG_READ (Base + I2C_SS_SCL_HCNT_OFFSET, I2cStandardSpeedSclHighCount.Val32);
    I2cStandardSpeedSclHighCount.bits.ic_ss_scl_hcnt = I2C_SS_SCLHCNT;
    I2C_REG_WRITE (Base + I2C_SS_SCL_HCNT_OFFSET, I2cStandardSpeedSclHighCount.Val32);
    I2C_REG_READ (Base + I2C_SS_SCL_LCNT_OFFSET, I2cStandardSpeedSclLowCount.Val32);
    I2cStandardSpeedSclLowCount.bits.ic_ss_scl_lcnt = I2C_SS_SCLLCNT;
    I2C_REG_WRITE (Base + I2C_SS_SCL_LCNT_OFFSET, I2cStandardSpeedSclLowCount.Val32);
  } else {
    I2C_REG_READ (Base + I2C_FS_SCL_HCNT_OFFSET, I2cStandardSpeedSclHighCount.Val32);
    I2cStandardSpeedSclHighCount.bits.ic_ss_scl_hcnt = I2C_SS_SCLHCNT;
    I2C_REG_WRITE (Base + I2C_FS_SCL_HCNT_OFFSET, I2cStandardSpeedSclHighCount.Val32);
    I2C_REG_READ (Base + I2C_FS_SCL_LCNT_OFFSET, I2cStandardSpeedSclLowCount.Val32);
    I2cStandardSpeedSclLowCount.bits.ic_ss_scl_lcnt = I2C_SS_SCLLCNT;
    I2C_REG_WRITE (Base + I2C_FS_SCL_LCNT_OFFSET, I2cStandardSpeedSclLowCount.Val32);
  }

  I2C_REG_READ (Base + I2C_RX_TL_OFFSET, I2cRxFifoReg.Val32);
  I2cRxFifoReg.bits.rx_tl = I2C_TXRX_THRESHOLD;
  I2C_REG_WRITE (Base + I2C_RX_TL_OFFSET, I2cRxFifoReg.Val32);
  I2C_REG_READ (Base + I2C_TX_TL_OFFSET, I2cTxFifoReg.Val32);
  I2cTxFifoReg.bits.tx_tl = I2C_TXRX_THRESHOLD;
  I2C_REG_WRITE (Base + I2C_TX_TL_OFFSET, I2cTxFifoReg.Val32);

  I2C_REG_READ (Base + I2C_INTR_MASK_OFFSET, I2cIntrMask.Val32);
  I2cIntrMask.Val32 = 0x0;
  I2C_REG_WRITE (Base + I2C_INTR_MASK_OFFSET, I2cIntrMask.Val32);

  Status = I2C_Enable (Socket, Port);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return I2cLibRuntimeSetup (Socket, Port);
}

EFI_STATUS
EFIAPI
I2CSdaConfig (
  UINT32 Socket,
  UINT32 Port
  )
{
  UINTN Base = GetI2cBase (Socket, Port);

  if ((Socket >= MAX_SOCKET) || (Port >= I2C_PORT_MAX)) {
    return EFI_INVALID_PARAMETER;
  }

  I2C_REG_WRITE (Base + I2C_SDA_HOLD, 0x14);

  return EFI_SUCCESS;
}



UINT32
I2C_GetTxStatus (
  UINT32 Socket,
  UINT8  Port
  )
{
  I2C0_TXFLR_U Fifo;
  UINTN        Base = GetI2cBase (Socket, Port);

  I2C_REG_READ (Base + I2C_TXFLR_OFFSET, Fifo.Val32);
  return Fifo.bits.txflr;
}

UINT32
I2C_GetRxStatus (
  UINT32 Socket,
  UINT8 Port
  )
{
  I2C0_RXFLR_U Fifo;
  UINTN Base = GetI2cBase (Socket, Port);

  I2C_REG_READ (Base + I2C_RXFLR_OFFSET, Fifo.Val32);
  return Fifo.bits.rxflr;
}

EFI_STATUS
EFIAPI
CheckI2CTimeOut (
  UINT32      Socket,
  UINT8       Port,
  I2CTransfer Transfer
  )
{
  UINT32 Times = 0;
  UINT32 Fifo;

  if (Transfer == I2CTx) {
    Fifo = I2C_GetTxStatus (Socket, Port);
    while (Fifo != 0) {
      if (Port == I2C_EXTENDER_PORT_HNS) {
        // This is a empirical value for I2C delay. MemoryFence is no need here.
        I2C_Delay (1000);
      } else {
        // This is a empirical value for I2C delay. MemoryFence is no need here.
        I2C_Delay (2);
      }
      if (++Times > I2C_READ_TIMEOUT) {
        (VOID)I2C_Disable (Socket, Port);
        return EFI_TIMEOUT;
      }
      Fifo = I2C_GetTxStatus (Socket, Port);
    }
  } else {
    Fifo = I2C_GetRxStatus (Socket, Port);
    while (Fifo == 0) {
      if (Port == I2C_EXTENDER_PORT_HNS) {
        // This is a empirical value for I2C delay. MemoryFence is no need here.
        I2C_Delay (1000);
      } else {
        // This is a empirical value for I2C delay. MemoryFence is no need here.
        I2C_Delay (2);
      }
      if (++Times > I2C_READ_TIMEOUT) {
        (VOID)I2C_Disable (Socket, Port);
        return EFI_TIMEOUT;
      }
      Fifo = I2C_GetRxStatus (Socket, Port);
    }
  }

  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
WriteBeforeRead (
  I2C_DEVICE *I2cInfo,
  UINT32     Length,
  UINT8      *pBuf
  )
{
  UINT32 Fifo;
  UINT32 Count;
  UINT32 Times = 0;

  UINTN  Base = GetI2cBase (I2cInfo->Socket, I2cInfo->Port);

  I2C_SetTarget (I2cInfo->Socket, I2cInfo->Port, I2cInfo->SlaveDeviceAddress);

  if (CheckI2CTimeOut (I2cInfo->Socket, I2cInfo->Port, I2CTx) == EFI_TIMEOUT) {
    return EFI_TIMEOUT;
  }

  Fifo = 0;
  for (Count = 0; Count < Length; Count++) {
    Times = 0;
    while (Fifo > I2C_TXRX_THRESHOLD) {
      I2C_Delay (2);
      if (++Times > I2C_READ_TIMEOUT) {
        return EFI_TIMEOUT;
      }
      Fifo = I2C_GetTxStatus (I2cInfo->Socket, I2cInfo->Port);
    }

    I2C_REG_WRITE (Base + I2C_DATA_CMD_OFFSET, *pBuf++);
    Fifo = I2C_GetTxStatus (I2cInfo->Socket, I2cInfo->Port);
  }

  if (CheckI2CTimeOut (I2cInfo->Socket, I2cInfo->Port, I2CTx) == EFI_TIMEOUT) {
    return EFI_TIMEOUT;
  }

  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
I2CWrite(
  I2C_DEVICE *I2cInfo,
  UINT16     InfoOffset,
  UINT32     Length,
  UINT8 *pBuf
  )
{
  UINT32 Fifo;
  UINT32 Times = 0;
  UINT32  Idx;
  UINTN  Base;

  if (I2cInfo->Port >= I2C_PORT_MAX) {
    return EFI_INVALID_PARAMETER;
  }

  Base = GetI2cBase (I2cInfo->Socket, I2cInfo->Port);

  (VOID)I2C_Enable(I2cInfo->Socket, I2cInfo->Port);

  I2C_SetTarget(I2cInfo->Socket, I2cInfo->Port, I2cInfo->SlaveDeviceAddress);

  if (CheckI2CTimeOut (I2cInfo->Socket, I2cInfo->Port, I2CTx) == EFI_TIMEOUT) {
    return EFI_TIMEOUT;
  }

  if (I2cInfo->DeviceType) {
    I2C_REG_WRITE (Base + I2C_DATA_CMD_OFFSET, (InfoOffset >> 8) & 0xff);
    I2C_REG_WRITE (Base + I2C_DATA_CMD_OFFSET, InfoOffset & 0xff);
  } else {
    I2C_REG_WRITE (Base + I2C_DATA_CMD_OFFSET, InfoOffset & 0xff);
  }

  if (CheckI2CTimeOut (I2cInfo->Socket, I2cInfo->Port, I2CTx) == EFI_TIMEOUT) {
    return EFI_TIMEOUT;
  }

  for (Idx = 0; Idx < Length; Idx++) {
    Times = 0;
    Fifo = I2C_GetTxStatus (I2cInfo->Socket, I2cInfo->Port);
    while (Fifo > I2C_TXRX_THRESHOLD) {
      // This is a empirical value for I2C delay. MemoryFence is no need here.
      I2C_Delay (1000);
      if (++Times > I2C_READ_TIMEOUT) {
        (VOID)I2C_Disable (I2cInfo->Socket, I2cInfo->Port);
        return EFI_TIMEOUT;
      }
      Fifo = I2C_GetTxStatus (I2cInfo->Socket, I2cInfo->Port);
    }

    if (Idx < Length - 1) {
      I2C_REG_WRITE (Base + I2C_DATA_CMD_OFFSET, (*pBuf++));
    } else {
      //Send command stop bit for the last transfer
      I2C_REG_WRITE (Base + I2C_DATA_CMD_OFFSET, (*pBuf++) | I2C_CMD_STOP_BIT);
    }
  }

  if (CheckI2CTimeOut (I2cInfo->Socket, I2cInfo->Port, I2CTx) == EFI_TIMEOUT) {
    return EFI_TIMEOUT;
  }
  (VOID)I2C_Disable (I2cInfo->Socket, I2cInfo->Port);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
I2CRead(
  I2C_DEVICE *I2cInfo,
  UINT16     InfoOffset,
  UINT32     RxLen,
  UINT8 *pBuf
  )
{
  UINT8       I2CWAddr[2];
  EFI_STATUS  Status;
  UINT32      Idx = 0;
  UINTN       Base;

  if (I2cInfo->Port >= I2C_PORT_MAX) {
    return EFI_INVALID_PARAMETER;
  }

  (VOID)I2C_Enable(I2cInfo->Socket, I2cInfo->Port);
  Base = GetI2cBase (I2cInfo->Socket, I2cInfo->Port);
  if (I2cInfo->DeviceType) {
    I2CWAddr[0] = (InfoOffset >> 8) & 0xff;
    I2CWAddr[1] = (InfoOffset & 0xff);
    Status = WriteBeforeRead (I2cInfo, 2,I2CWAddr);
    if (EFI_ERROR (Status)) {
      (VOID)I2C_Disable (I2cInfo->Socket, I2cInfo->Port);
      return EFI_ABORTED;
    }
  } else {
    I2CWAddr[0] = (InfoOffset & 0xff);
    Status = WriteBeforeRead (I2cInfo, 1, I2CWAddr);
    if (EFI_ERROR (Status)) {
      (VOID)I2C_Disable (I2cInfo->Socket, I2cInfo->Port);
      return EFI_ABORTED;
    }
  }

  I2C_SetTarget (I2cInfo->Socket, I2cInfo->Port, I2cInfo->SlaveDeviceAddress);

  if (CheckI2CTimeOut (I2cInfo->Socket, I2cInfo->Port, I2CTx) == EFI_TIMEOUT) {
    return EFI_TIMEOUT;
  }

  while (RxLen > 0) {
    if (RxLen > 1) {
      I2C_REG_WRITE (Base + I2C_DATA_CMD_OFFSET, I2C_READ_SIGNAL);
    } else {
      //Send command stop bit for the last transfer
      I2C_REG_WRITE (Base + I2C_DATA_CMD_OFFSET, I2C_READ_SIGNAL | I2C_CMD_STOP_BIT);
    }

    if (CheckI2CTimeOut (I2cInfo->Socket, I2cInfo->Port, I2CRx) == EFI_TIMEOUT) {
      return EFI_TIMEOUT;
    }

    I2C_REG_READ (Base + I2C_DATA_CMD_OFFSET, pBuf[Idx++]);

    RxLen --;
  }
  (VOID)I2C_Disable (I2cInfo->Socket, I2cInfo->Port);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
I2CReadMultiByte (
  I2C_DEVICE *I2cInfo,
  UINT32     InfoOffset,
  UINT32     RxLen,
  UINT8      *pBuf
  )
{
  UINT32      Count;
  UINT16      TotalLen = 0;
  UINT8       I2CWAddr[4];
  EFI_STATUS  Status;
  UINT32      BytesLeft;
  UINT32      Idx = 0;
  UINTN       Base;

  if (I2cInfo->Port >= I2C_PORT_MAX) {
    return EFI_INVALID_PARAMETER;
  }

  (VOID)I2C_Enable (I2cInfo->Socket, I2cInfo->Port);
  Base = GetI2cBase (I2cInfo->Socket, I2cInfo->Port);
  if (I2cInfo->DeviceType == DEVICE_TYPE_E2PROM) {
    I2CWAddr[0] = (InfoOffset >> 8) & 0xff;
    I2CWAddr[1] = (InfoOffset & 0xff);
    Status = WriteBeforeRead (I2cInfo, 2,I2CWAddr);
    if (EFI_ERROR (Status)) {
      (VOID)I2C_Disable (I2cInfo->Socket, I2cInfo->Port);
      return EFI_ABORTED;
    }
  } else if (I2cInfo->DeviceType == DEVICE_TYPE_CPLD_3BYTE_OPERANDS) {
    I2CWAddr[0] = (InfoOffset >> 16) & 0xff;
    I2CWAddr[1] = (InfoOffset >> 8) & 0xff;
    I2CWAddr[2] = (InfoOffset & 0xff);
    Status = WriteBeforeRead (I2cInfo, 3, I2CWAddr);
    if (EFI_ERROR (Status)) {
      (VOID)I2C_Disable (I2cInfo->Socket, I2cInfo->Port);
      return EFI_ABORTED;
    }
  } else if (I2cInfo->DeviceType == DEVICE_TYPE_CPLD_4BYTE_OPERANDS) {
    I2CWAddr[0] = (InfoOffset >> 24) & 0xff;
    I2CWAddr[1] = (InfoOffset >> 16) & 0xff;
    I2CWAddr[2] = (InfoOffset >> 8) & 0xff;
    I2CWAddr[3] = (InfoOffset & 0xff);
    Status = WriteBeforeRead (I2cInfo, 4,I2CWAddr);
    if (EFI_ERROR (Status)) {
      (VOID)I2C_Disable (I2cInfo->Socket, I2cInfo->Port);
      return EFI_ABORTED;
    }
  } else {
    I2CWAddr[0] = (InfoOffset & 0xff);
    Status = WriteBeforeRead (I2cInfo, 1,I2CWAddr);
    if (EFI_ERROR (Status)) {
      (VOID)I2C_Disable (I2cInfo->Socket, I2cInfo->Port);
      return EFI_ABORTED;
    }
  }

  I2C_SetTarget(I2cInfo->Socket, I2cInfo->Port, I2cInfo->SlaveDeviceAddress);
  TotalLen = RxLen;
  BytesLeft = TotalLen;

  for (Count = 0; Count < BytesLeft; Count++) {
    I2C_REG_WRITE (Base + I2C_DATA_CMD_OFFSET, I2C_READ_SIGNAL);
  }

  for (Count = 0; Count < BytesLeft; Count++) {
    if (CheckI2CTimeOut (I2cInfo->Socket, I2cInfo->Port, I2CRx) == EFI_TIMEOUT) {
      return EFI_TIMEOUT;
    }

    I2C_REG_READ (Base + I2C_DATA_CMD_OFFSET, pBuf[Idx++]);
  }
  (VOID)I2C_Disable (I2cInfo->Socket, I2cInfo->Port);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
I2CWriteMultiByte(
  I2C_DEVICE *I2cInfo,
  UINT32     InfoOffset,
  UINT32     Length,
  UINT8      *pBuf
  )
{
  UINT32  Idx;
  UINTN  Base;

  if (I2cInfo->Port >= I2C_PORT_MAX) {
    return EFI_INVALID_PARAMETER;
  }

  Base = GetI2cBase (I2cInfo->Socket, I2cInfo->Port);

  (VOID)I2C_Enable(I2cInfo->Socket, I2cInfo->Port);

  I2C_SetTarget(I2cInfo->Socket, I2cInfo->Port, I2cInfo->SlaveDeviceAddress);

  if (CheckI2CTimeOut (I2cInfo->Socket, I2cInfo->Port, I2CTx) == EFI_TIMEOUT) {
    return EFI_TIMEOUT;
  }

  if (I2cInfo->DeviceType == DEVICE_TYPE_CPLD_3BYTE_OPERANDS) {
    I2C_REG_WRITE (Base + I2C_DATA_CMD_OFFSET, (InfoOffset >> 16) & 0xff);
    I2C_REG_WRITE (Base + I2C_DATA_CMD_OFFSET, (InfoOffset >> 8) & 0xff);
    I2C_REG_WRITE (Base + I2C_DATA_CMD_OFFSET, InfoOffset & 0xff);
  } else if (I2cInfo->DeviceType == DEVICE_TYPE_CPLD_4BYTE_OPERANDS) {
    I2C_REG_WRITE (Base + I2C_DATA_CMD_OFFSET, (InfoOffset >> 24) & 0xff);
    I2C_REG_WRITE (Base + I2C_DATA_CMD_OFFSET, (InfoOffset >> 16) & 0xff);
    I2C_REG_WRITE (Base + I2C_DATA_CMD_OFFSET, (InfoOffset >> 8) & 0xff);
    I2C_REG_WRITE (Base + I2C_DATA_CMD_OFFSET, InfoOffset & 0xff);
  } else {
  }

  for (Idx = 0; Idx < Length; Idx++) {
    I2C_REG_WRITE (Base + I2C_DATA_CMD_OFFSET, *pBuf++);
  }

  if (CheckI2CTimeOut (I2cInfo->Socket, I2cInfo->Port, I2CTx) == EFI_TIMEOUT) {
    return EFI_TIMEOUT;
  }
  (VOID)I2C_Disable (I2cInfo->Socket, I2cInfo->Port);

  return EFI_SUCCESS;
}

