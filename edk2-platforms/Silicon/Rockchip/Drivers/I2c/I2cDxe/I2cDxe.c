/********************************************************************************
Copyright (C) 2021 Rockchip Electronics Co., Ltd

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#include <Protocol/I2cMaster.h>
#include <Protocol/I2cEnumerate.h>
#include <Protocol/I2cBusConfigurationManagement.h>
#include <Protocol/DevicePath.h>
#include <Protocol/I2c.h>

#include <Library/TimerLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/RockchipPlatformLib.h>

#include "I2cDxe.h"

#define RK_I2C_DUMPREG 1

STATIC CONST I2C_SPEC_VALUES StandardModeSpec = {
  .MinLowNs = 4700,
  .MinHighNs = 4000,
  .MaxRiseNs = 1000,
  .MaxFallNs = 300,
};

STATIC CONST I2C_SPEC_VALUES FastModeSpec = {
  .MinLowNs = 1300,
  .MinHighNs = 600,
  .MaxRiseNs = 300,
  .MaxFallNs = 300,
};

STATIC CONST I2C_SPEC_VALUES FastModeplusSpec = {
  .MinLowNs = 500,
  .MinHighNs = 260,
  .MaxRiseNs = 120,
  .MaxFallNs = 120,
};

STATIC I2C_DEVICE_PATH I2cDevicePathProtocol = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(OFFSET_OF (I2C_DEVICE_PATH, End)),
        (UINT8)(OFFSET_OF (I2C_DEVICE_PATH, End) >> 8),
      },
    },
    EFI_CALLER_ID_GUID
  },
  0,  // Instance
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      sizeof(EFI_DEVICE_PATH_PROTOCOL),
      0
    }
  }
};

STATIC
CONST I2C_SPEC_VALUES *
I2cGetSpec (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINT32 Speed
  )
{
  if (Speed == 1000)
    return &FastModeplusSpec;
  else if (Speed == 400)
    return &FastModeSpec;
  else
    return &StandardModeSpec;
}

STATIC
UINT32
I2cRead (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINTN Off
  )
{
  ASSERT (I2cMasterContext != NULL);
  return MmioRead32 (I2cMasterContext->BaseAddress + Off);
}

STATIC
EFI_STATUS
I2cWrite (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINTN Off,
  IN UINT32 Value
  )
{
  ASSERT (I2cMasterContext != NULL);
  return MmioWrite32 (I2cMasterContext->BaseAddress + Off, Value);
}

STATIC
VOID
I2cShowRegs (
  IN I2C_MASTER_CONTEXT *I2cMasterContex
  )
{
#if RK_I2C_DUMPREG
  UINTN i;

  DEBUG((DEBUG_INFO | DEBUG_LOAD, "i2c_con: 0x%08x\n", I2cRead(I2cMasterContex, I2C_CON)));
  DEBUG((DEBUG_INFO | DEBUG_LOAD, "i2c_clkdiv: 0x%08x\n", I2cRead(I2cMasterContex, I2C_CLKDIV)));
  DEBUG((DEBUG_INFO | DEBUG_LOAD, "i2c_mrxaddr: 0x%08x\n", I2cRead(I2cMasterContex, I2C_MRXADDR)));
  DEBUG((DEBUG_INFO | DEBUG_LOAD, "i2c_mrxraddR: 0x%08x\n", I2cRead(I2cMasterContex, I2C_MRXRADDR)));
  DEBUG((DEBUG_INFO | DEBUG_LOAD, "i2c_mtxcnt: 0x%08x\n", I2cRead(I2cMasterContex, I2C_MTXCNT)));
  DEBUG((DEBUG_INFO | DEBUG_LOAD, "i2c_mrxcnt: 0x%08x\n", I2cRead(I2cMasterContex, I2C_MRXCNT)));
  DEBUG((DEBUG_INFO | DEBUG_LOAD, "i2c_ien: 0x%08x\n", I2cRead(I2cMasterContex, I2C_IEN)));
  DEBUG((DEBUG_INFO | DEBUG_LOAD, "i2c_ipd: 0x%08x\n", I2cRead(I2cMasterContex, I2C_IPD)));
  DEBUG((DEBUG_INFO | DEBUG_LOAD, "i2c_fcnt: 0x%08x\n", I2cRead(I2cMasterContex, I2C_FCNT)));
  for (i = 0; i < 8; i++)
    DEBUG((DEBUG_INFO | DEBUG_LOAD, "i2c_txdata%d: 0x%08x\n", i, I2cRead(I2cMasterContex, I2C_TXDATA_BASE + 4 * i)));
  for (i = 0; i < 8; i++)
    DEBUG((DEBUG_INFO | DEBUG_LOAD, "i2c_rxdata%d: 0x%08x\n", i, I2cRead(I2cMasterContex,I2C_RXDATA_BASE + 4 * i)));
#endif
}

STATIC
I2C_VERSION
I2cGetVersion(
  IN I2C_MASTER_CONTEXT *I2cMasterContext
  )
{
  I2C_VERSION Version;

  Version = I2cRead(I2cMasterContext, I2C_CON) & I2C_CON_VERSION;
  return Version >>= I2C_CON_VERSION_SHIFT;
}

EFI_STATUS
EFIAPI
I2cInitialiseController (
  IN EFI_HANDLE  ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable,
  IN EFI_PHYSICAL_ADDRESS BaseAddress,
  IN UINT32 BusId
  )
{
  EFI_STATUS Status;
  I2C_MASTER_CONTEXT *I2cMasterContext;
  I2C_DEVICE_PATH *DevicePath;

  DEBUG ((EFI_D_VERBOSE, "I2cInitialiseController\n"));
  DevicePath = AllocateCopyPool (sizeof(I2cDevicePathProtocol),
                                 &I2cDevicePathProtocol);
  if (DevicePath == NULL) {
    DEBUG((DEBUG_ERROR, "I2cDxe: I2C device path allocation failed\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  DevicePath->Instance = BusId;

  /* if attachment succeeds, this gets freed at ExitBootServices */
  I2cMasterContext = AllocateZeroPool (sizeof (I2C_MASTER_CONTEXT));
  if (I2cMasterContext == NULL) {
    DEBUG((DEBUG_ERROR, "I2cDxe: I2C master context allocation failed\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  I2cMasterContext->Signature = I2C_MASTER_SIGNATURE;
  I2cMasterContext->I2cMaster.Reset = I2cReset;
  I2cMasterContext->I2cMaster.StartRequest = I2cStartRequest;
  I2cMasterContext->I2cEnumerate.Enumerate = I2cEnumerate;
  I2cMasterContext->I2cBusConf.EnableI2cBusConfiguration = I2cEnableConf;
  I2cMasterContext->TclkFrequency = PcdGet32 (PcdI2cClockFrequency);
  I2cMasterContext->BaseAddress = BaseAddress;
  I2cMasterContext->Bus = BusId;
  EfiInitializeLock(&I2cMasterContext->Lock, TPL_NOTIFY);

  if (I2cGetVersion(I2cMasterContext) >= RkI2cVersion1) {
    I2cAdapterBaudRate(I2cMasterContext,
                       PcdGet32 (PcdI2cBaudRate),
		       I2cMasterContext->TclkFrequency
		       );
  } else {
    I2cCalBaudRate(I2cMasterContext,
		   PcdGet32 (PcdI2cBaudRate),
		   I2cMasterContext->TclkFrequency
		   );
  }

  Status = gBS->InstallMultipleProtocolInterfaces(
    &I2cMasterContext->Controller,
    &gEfiI2cMasterProtocolGuid,
    &I2cMasterContext->I2cMaster,
    &gEfiI2cEnumerateProtocolGuid,
    &I2cMasterContext->I2cEnumerate,
    &gEfiI2cBusConfigurationManagementProtocolGuid,
    &I2cMasterContext->I2cBusConf,
    &gEfiDevicePathProtocolGuid,
    (EFI_DEVICE_PATH_PROTOCOL *) DevicePath,
    NULL);

  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "I2cDxe: Installing protocol interfaces failed!\n"));
    goto fail;
  }

  DEBUG ((DEBUG_INFO, "Succesfully installed controller %d at 0x%llx\n", BusId, I2cMasterContext->BaseAddress));
  return EFI_SUCCESS;

fail:
  FreePool(I2cMasterContext);
  return Status;
}

STATIC
VOID
EFIAPI
OnEndOfDxe (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  I2C_DEVICE_PATH        *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathPointer;
  EFI_HANDLE                DeviceHandle;
  EFI_STATUS                Status;

  gBS->CloseEvent (Event);

  DevicePath = AllocateCopyPool (sizeof (I2cDevicePathProtocol),
                 &I2cDevicePathProtocol);
  if (DevicePath == NULL) {
    DEBUG ((DEBUG_ERROR, "I2cDxe: I2C device path allocation failed\n"));
    return;
  }

  do {
    DevicePathPointer = (EFI_DEVICE_PATH_PROTOCOL *)DevicePath;
    Status = gBS->LocateDevicePath (&gEfiI2cMasterProtocolGuid,
                                    &DevicePathPointer,
                                    &DeviceHandle);
    if (EFI_ERROR (Status)) {
      break;
    }

    Status = gBS->ConnectController (DeviceHandle, NULL, NULL, TRUE);
    DEBUG ((DEBUG_INFO,
      "%a: ConnectController () returned %r\n",
      __FUNCTION__,
      Status));

    DevicePath->Instance++;
  } while (TRUE);

  gBS->FreePool (DevicePath);
}

EFI_STATUS
EFIAPI
I2cInitialise (
  IN EFI_HANDLE  ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_PHYSICAL_ADDRESS BaseAddress;
  EFI_EVENT EndOfDxeEvent;
  UINT8 *DeviceBusPcd;
  EFI_STATUS Status;
  UINTN Index;

  DEBUG ((EFI_D_VERBOSE, "I2cInitialise count: %d\n", PcdGet32 (PcdI2cBusCount)));

   DeviceBusPcd = PcdGetPtr (PcdI2cSlaveBuses);
  /* Initialize enabled chips */
  for (Index = 0; Index < PcdGet32 (PcdI2cBusCount); Index++) {
    BaseAddress = I2cGetBase(DeviceBusPcd[Index]);

    if (!BaseAddress)
        continue;
      Status = I2cInitialiseController(
        ImageHandle,
        SystemTable,
        BaseAddress,
        DeviceBusPcd[Index]
        );
    if (EFI_ERROR(Status))
      return Status;
  }

  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  OnEndOfDxe,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

STATIC
VOID
I2cCalBaudRate (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINT32 Target,
  IN UINT32 ClkRate
  )
{
  int Div, Divl, Divh;

  Div = DIV_ROUND_UP(ClkRate, Target * 8) - 2;
  Divh = 0;
  Divl = 0;
  if (Div >= 0) {
    Divl = Div / 2;
    if (Div % 2 == 0)
      Divh = Div / 2;
    else
      Divh = DIV_ROUND_UP(Div, 2);
  }

  I2cWrite(I2cMasterContext, I2C_CLKDIV,  I2C_CLKDIV_VAL(Divl, Divh));
}

STATIC
EFI_STATUS
I2cAdapterBaudRate (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINT32 Target,
  IN UINT32 ClkRate
  )
{
  CONST I2C_SPEC_VALUES *Spec;
  UINT32 MinTotalDiv, MinLowDiv, MinHighDiv, MinHoldDiv;
  UINT32 LowDiv, HighDiv, ExtraDiv, ExtraLowDiv;
  UINT32 MinLowNs, MinHighNs;
  UINT32 StartSetup = 0;
  UINT32 Speed;

  if (Target <= 100000 && Target >= 1000) {
    StartSetup = 1;
    Speed = 100;
  } else if (Target <= 400000 && Target >= 100000) {
    Speed = 400;
  } else if (Target <= 1000000 && Target > 400000) {
    Speed = 1000;
  } else {
    DEBUG ((EFI_D_ERROR, "invalid i2c speed : %d\n", Target));
    return EFI_INVALID_PARAMETER;
  }

  Spec = I2cGetSpec(I2cMasterContext, Speed);
  ClkRate = DIV_ROUND_UP(ClkRate, 1000);
  Speed = DIV_ROUND_UP(Target, 1000);

  MinTotalDiv = DIV_ROUND_UP(ClkRate, Speed * 8);

  MinHighNs = Spec->MaxRiseNs + Spec->MinHighNs;
  MinHighDiv = DIV_ROUND_UP(ClkRate * MinHighNs, 8 * 1000000);

  MinLowNs = Spec->MaxFallNs + Spec->MinLowNs;
  MinLowDiv = DIV_ROUND_UP(ClkRate * MinLowNs, 8 * 1000000);

  MinHighDiv = (MinHighDiv < 1) ? 2 : MinHighDiv;
  MinLowDiv = (MinLowDiv < 1) ? 2 : MinLowDiv;

  MinHoldDiv = MinHighDiv + MinLowDiv;

  if (MinHoldDiv >= MinTotalDiv) {
    HighDiv = MinHighDiv;
    LowDiv = MinLowDiv;
  } else {
    ExtraDiv = MinTotalDiv - MinHoldDiv;
    ExtraLowDiv = DIV_ROUND_UP(MinLowDiv * ExtraDiv,
                              MinHoldDiv);

    LowDiv = MinLowDiv + ExtraLowDiv;
    HighDiv = MinHighDiv + (ExtraDiv - ExtraLowDiv);
  }

  HighDiv--;
  LowDiv--;

  if (HighDiv > 0xffff || LowDiv > 0xffff)
    return EFI_UNSUPPORTED;

  /* 1 for data hold/setup time is enough */
  I2cMasterContext->Config = I2C_CON_SDA_CFG(1) | I2C_CON_STA_CFG(StartSetup);
  I2cWrite(I2cMasterContext, I2C_CLKDIV, (HighDiv << I2C_CLK_DIV_HIGH_SHIFT) | LowDiv);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
I2cReset (
  IN CONST EFI_I2C_MASTER_PROTOCOL *This
  )
{
  DEBUG((EFI_D_VERBOSE, "I2cReset\n"));
  return EFI_SUCCESS;
}

STATIC
VOID
I2cDisable (
  IN I2C_MASTER_CONTEXT *I2cMasterContext
  )
{
  DEBUG ((EFI_D_VERBOSE, "I2c I2cDisable.\n"));

  EfiAcquireLock (&I2cMasterContext->Lock);
  I2cWrite(I2cMasterContext, I2C_IEN, 0);
  I2cWrite(I2cMasterContext, I2C_IPD, I2C_IPD_ALL_CLEAN);
  I2cWrite(I2cMasterContext, I2C_CON, 0);
  EfiReleaseLock (&I2cMasterContext->Lock);
}

/*
 * enable and start at same time, Timeout is given in us.
 */
STATIC
EFI_STATUS
I2cStartEnable (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINT32 Con,
  IN UINTN Timeout
  )
{
  DEBUG ((EFI_D_VERBOSE, "I2cStartEnable\n"));

  I2cWrite(I2cMasterContext, I2C_IPD, I2C_IPD_ALL_CLEAN);
  I2cWrite(I2cMasterContext, I2C_IEN, I2C_STARTIEN);
  I2cWrite(I2cMasterContext, I2C_CON, I2C_CON_EN | I2C_CON_START | Con |I2cMasterContext->Config);

  while (Timeout--) {
    if (I2cRead(I2cMasterContext, I2C_IPD) & I2C_STARTIPD) {
        I2cWrite(I2cMasterContext, I2C_IPD, I2C_STARTIPD);
        break;
    }
    MicroSecondDelay(1);
  }
  if (Timeout <= 0) {
    DEBUG ((EFI_D_ERROR, "I2C Send Start Bit Timeout\n"));
    I2cShowRegs(I2cMasterContext);
    EfiReleaseLock (&I2cMasterContext->Lock);
    return EFI_TIMEOUT;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
I2cStop (
  IN I2C_MASTER_CONTEXT *I2cMasterContext
  )
{
  int TimeOut = I2C_READY_TIMEOUT;

  DEBUG ((EFI_D_VERBOSE, "I2c Send Stop bit.\n"));

  EfiAcquireLock (&I2cMasterContext->Lock);

  I2cWrite(I2cMasterContext, I2C_IPD, I2C_IPD_ALL_CLEAN);
  I2cWrite(I2cMasterContext, I2C_CON, I2C_CON_EN | I2C_CON_STOP |I2cMasterContext->Config);
  I2cWrite(I2cMasterContext, I2C_IEN, I2C_CON_STOP);

  while (TimeOut--) {
    if (I2cRead(I2cMasterContext, I2C_IPD) & I2C_STOPIPD) {
      I2cWrite(I2cMasterContext, I2C_IPD, I2C_STOPIPD);
      break;
    }
    MicroSecondDelay(1);
  }

  if (TimeOut <= 0) {
    DEBUG ((EFI_D_ERROR, "I2C Send Stop Bit Timeout\n"));
    I2cShowRegs(I2cMasterContext);
    EfiReleaseLock (&I2cMasterContext->Lock);
    return EFI_TIMEOUT;
  }

  EfiReleaseLock (&I2cMasterContext->Lock);

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
I2cReadOperation (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINTN SlaveAddress,
  IN OUT UINT8 *Buf,
  IN UINTN Length,
  IN OUT UINTN *Read,
  IN UINTN Snd,
  IN UINTN Timeout
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  int TimeOut = I2C_TIMEOUT_US;
  UINT8 *PBuf = Buf;
  UINT32 BytesRemainLen = Length;
  UINT32 BytesTranferedLen = 0;
  UINT32 WordsTranferedLen = 0;
  UINT32 Con = 0;
  UINT32 RxData;
  UINT32 i, j;
  UINTN SndChunk = 0;

  DEBUG ((EFI_D_VERBOSE, "I2cRead: base_addr = 0x%x buf = %p, Length = %d\n",
  	         I2cMasterContext->BaseAddress, Buf, Length));

  EfiAcquireLock (&I2cMasterContext->Lock);
  /* If the second message for TRX read, resetting internal state. */
  if (Snd) {
    I2cWrite(I2cMasterContext, I2C_CON, 0);
  }

 I2cWrite(I2cMasterContext, I2C_MRXADDR, I2C_MRXADDR_SET(1, SlaveAddress << 1 | 1));

 (*Read) = 0;
  while (BytesRemainLen) {
  if (BytesRemainLen > RK_I2C_FIFO_SIZE) {
    Con = I2C_CON_EN;
    BytesTranferedLen = 32;
  } else {
    /*
      * The hw can read up to 32 bytes at a time. If we need
      * more than one chunk, send an ACK after the last byte.
      */
    Con = I2C_CON_EN | I2C_CON_LASTACK;
    BytesTranferedLen = BytesRemainLen;
  }
  WordsTranferedLen = DIV_ROUND_UP(BytesTranferedLen, 4);

  /*
    * make sure we are in plain RX mode if we read a second chunk;
    * and first rx read need to send start bit.
    */
  if (SndChunk) {
    Con |= I2C_CON_MOD(I2C_MODE_RX);
    I2cWrite(I2cMasterContext, I2C_CON, Con | I2cMasterContext->Config);
  } else {
    Con |= I2C_CON_MOD(I2C_MODE_TRX);
    Status = I2cStartEnable(I2cMasterContext, Con, I2C_TIMEOUT_US);
    if (EFI_ERROR(Status))
      goto out;
    }

    I2cWrite(I2cMasterContext, I2C_IEN, I2C_MBRFIEN | I2C_NAKRCVIEN); 
    I2cWrite(I2cMasterContext, I2C_MRXCNT, BytesTranferedLen);

    while (TimeOut--) {
      if (I2cRead(I2cMasterContext, I2C_IPD) & I2C_NAKRCVIPD) {
        I2cWrite(I2cMasterContext, I2C_IPD, I2C_NAKRCVIPD);
        Status = EFI_NO_RESPONSE;
        goto out;
      }

      if (I2cRead(I2cMasterContext, I2C_IPD) & I2C_MBRFIPD) {
        I2cWrite(I2cMasterContext, I2C_IPD, I2C_MBRFIPD);
        break;
      }
      MicroSecondDelay(1);
    }

    if (TimeOut <= 0) {
      DEBUG ((EFI_D_ERROR, "I2C Read Data Timeout\n"));
      I2cShowRegs(I2cMasterContext);
      Status = EFI_TIMEOUT;
      goto out;
    }

    for (i = 0; i < WordsTranferedLen; i++) {
      RxData = I2cRead(I2cMasterContext, I2C_RXDATA_BASE + i * 4);
      DEBUG ((EFI_D_VERBOSE, "I2c Read RXDATA[%d] = 0x%x\n", i, RxData));
      for (j = 0; j < 4; j++) {
        if ((i * 4 + j) == BytesTranferedLen) {
        break;
      }
      *PBuf++ = (RxData >> (j * 8)) & 0xff;
    }
  }
  BytesRemainLen -= BytesTranferedLen;
  SndChunk = 1;
  (*Read) = BytesTranferedLen;
  }

  Status = EFI_SUCCESS;
  (*Read) = Length;

out:
  EfiReleaseLock (&I2cMasterContext->Lock);
  return (Status);
}

STATIC
EFI_STATUS
I2cWriteOperation (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINTN SlaveAddress,
  IN OUT CONST UINT8 *Buf,
  IN UINTN Length,
  IN OUT UINTN *Sent,
  IN UINTN Timeout
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  int TimeOut = I2C_TIMEOUT_US;
  CONST UINT8 *PBuf = Buf;
  UINT32 BytesRemainLen = Length + 1;
  UINT32 BytesTranferedLen = 0;
  UINT32 WordsTranferedLen = 0;
  UINT32 TxData;
  UINT32 i, j;
  UINTN Next = 0;

  DEBUG ((EFI_D_VERBOSE, "I2cWrite: base_addr = 0x%x buf = %p, Length = %d\n",
                 I2cMasterContext->BaseAddress, Buf, Length));

  EfiAcquireLock (&I2cMasterContext->Lock);
  (*Sent) = 0;

  while (BytesRemainLen) {
    if (BytesRemainLen > RK_I2C_FIFO_SIZE) {
      BytesTranferedLen = 32;
    } else {
      BytesTranferedLen = BytesRemainLen;
    }
    WordsTranferedLen = DIV_ROUND_UP(BytesTranferedLen, 4);

    for (i = 0; i < WordsTranferedLen; i++) {
      TxData = 0;
      for (j = 0; j < 4; j++) {
        if ((i * 4 + j) == BytesTranferedLen) {
          break;
        }

        if (i == 0 && j == 0 && PBuf == Buf) {
          TxData |= (SlaveAddress << 1);
        } else {
          TxData |= (*PBuf++)<<(j * 8);
        }
      }
      I2cWrite(I2cMasterContext, I2C_TXDATA_BASE + i * 4, TxData);
      DEBUG ((EFI_D_VERBOSE, "I2c Write TXDATA[%d] = 0x%x\n", i, TxData));
    }

    /* If the write is the first, need to send start bit */
    if (!Next) {
      Status = I2cStartEnable(I2cMasterContext, I2C_CON_EN |
                 I2C_CON_MOD(I2C_MODE_TX), I2C_TIMEOUT_US);
      if (EFI_ERROR(Status)) {
        goto out;
      }
       Next = 1;
    } else {
      I2cWrite(I2cMasterContext, I2C_CON, I2C_CON_EN |
               I2C_CON_MOD(I2C_MODE_TX) | I2cMasterContext->Config);
    }

    I2cWrite(I2cMasterContext, I2C_IEN, I2C_MBTFIEN | I2C_NAKRCVIEN);
    I2cWrite(I2cMasterContext, I2C_MTXCNT, BytesTranferedLen);

    while (TimeOut--) {
      if (I2cRead(I2cMasterContext, I2C_IPD) & I2C_NAKRCVIPD) {
        I2cWrite(I2cMasterContext, I2C_IPD, I2C_NAKRCVIPD);
        Status = EFI_NO_RESPONSE;
        goto out;
      }
      if (I2cRead(I2cMasterContext, I2C_IPD) & I2C_MBTFIPD) {
        I2cWrite(I2cMasterContext, I2C_IPD, I2C_MBTFIPD);
        break;
      }
      MicroSecondDelay(1);
    }

    if (TimeOut <= 0) {
      DEBUG ((EFI_D_ERROR, "I2C Write Data Timeout\n"));
      I2cShowRegs(I2cMasterContext);
      Status = EFI_TIMEOUT;
      goto out;
    }

    BytesRemainLen -= BytesTranferedLen;
    (*Sent) = BytesTranferedLen;
  }

  (*Sent) = Length;
  Status = EFI_SUCCESS;
out:
  EfiReleaseLock (&I2cMasterContext->Lock);
  return (Status);
}


/*
 * I2cStartRequest should be called only by I2cHost.
 * I2C device drivers ought to use EFI_I2C_IO_PROTOCOL instead.
 */
STATIC
EFI_STATUS
I2cStartRequest (
  IN CONST EFI_I2C_MASTER_PROTOCOL *This,
  IN UINTN                         SlaveAddress,
  IN EFI_I2C_REQUEST_PACKET        *RequestPacket,
  IN EFI_EVENT                     Event      OPTIONAL,
  OUT EFI_STATUS                   *I2cStatus OPTIONAL
  )
{
  UINTN Count = RequestPacket->OperationCount;
  UINTN ReadMode;
  UINTN Transmitted;
  I2C_MASTER_CONTEXT *I2cMasterContext = I2C_SC_FROM_MASTER(This);
  EFI_I2C_OPERATION *Operation;
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN i;

  ASSERT (RequestPacket != NULL);
  ASSERT (I2cMasterContext != NULL);

  DEBUG ((EFI_D_VERBOSE, "I2cStartRequest.\n"));

  if (Count > 2 || ((Count == 2) && (RequestPacket->Operation->Flags & I2C_FLAG_READ))) {
    DEBUG ((EFI_D_VERBOSE, "Not support more messages now, split them\n"));
    return EFI_INVALID_PARAMETER;
  }

  for (i = 0; i < Count; i++) {
    Operation = &RequestPacket->Operation[i];
    ReadMode = Operation->Flags & I2C_FLAG_READ;

    if (Operation->Flags & I2C_FLAG_READ) {
      /* If snd is true, it is TRX mode. */
      Status = I2cReadOperation ( I2cMasterContext,
                                  SlaveAddress,
                                  Operation->Buffer,
                                  Operation->LengthInBytes,
                                  &Transmitted,
                                  i == 1,
                                  I2C_TRANSFER_TIMEOUT);
                                  Operation->LengthInBytes = Transmitted;
    } else {
      Status = I2cWriteOperation ( I2cMasterContext,
                                   SlaveAddress,
                                   Operation->Buffer,
                                   Operation->LengthInBytes,
                                   &Transmitted,
                                   I2C_TRANSFER_TIMEOUT);
      Operation->LengthInBytes = Transmitted;
    }

    /* I2C transaction was aborted, so stop further transactions */
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_VERBOSE, "I2cStartRequest: Failed at Count = %d\n", Count));
      break;
    }
  }

  I2cStop (I2cMasterContext);
  I2cDisable (I2cMasterContext);

  if (I2cStatus != NULL)
    *I2cStatus = EFI_SUCCESS;
  if (Event != NULL)
    gBS->SignalEvent(Event);
  return EFI_SUCCESS;
}

STATIC CONST EFI_GUID DevGuid = I2C_GUID;

STATIC
EFI_STATUS
I2cAllocDevice (
  IN UINT8 SlaveAddress,
  IN UINT8 Bus,
  IN OUT CONST EFI_I2C_DEVICE **Device
  )
{
  EFI_STATUS Status;
  EFI_I2C_DEVICE *Dev;
  UINT32 *TmpSlaveArray;
  EFI_GUID *TmpGuidP;

  Status = gBS->AllocatePool ( EfiBootServicesData,
             sizeof(EFI_I2C_DEVICE),
             (VOID **) &Dev );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "I2cDxe: I2C device memory allocation failed\n"));
    return Status;
  }
  *Device = Dev;
  Dev->DeviceIndex = SlaveAddress;
  Dev->DeviceIndex = I2C_DEVICE_INDEX(Bus, SlaveAddress);
  Dev->SlaveAddressCount = 1;
  Dev->I2cBusConfiguration = 0;
  Status = gBS->AllocatePool ( EfiBootServicesData,
             sizeof(UINT32),
             (VOID **) &TmpSlaveArray);
  if (EFI_ERROR(Status)) {
    goto fail1;
  }
  TmpSlaveArray[0] = SlaveAddress;
  Dev->SlaveAddressArray = TmpSlaveArray;

  Status = gBS->AllocatePool ( EfiBootServicesData,
             sizeof(EFI_GUID),
             (VOID **) &TmpGuidP);
  if (EFI_ERROR(Status)) {
    goto fail2;
  }
  *TmpGuidP = DevGuid;
  Dev->DeviceGuid = TmpGuidP;

  return EFI_SUCCESS;

fail2:
  FreePool(TmpSlaveArray);
fail1:
  FreePool(Dev);

  return Status;
}

/*
 * It is called by I2cBus to enumerate devices on I2C bus. In this case,
 * enumeration is based on PCD configuration - all Slave addresses specified
 * in PCD get their corresponding EFI_I2C_DEVICE structures here.
 *
 * After enumeration succeeds, Supported() function of drivers that installed
 * DriverBinding protocol is called.
 */
STATIC
EFI_STATUS
EFIAPI
I2cEnumerate (
  IN CONST EFI_I2C_ENUMERATE_PROTOCOL *This,
  IN OUT CONST EFI_I2C_DEVICE         **Device
  )
{
  UINT8 *DevicesPcd;
  UINT8 *DeviceBusPcd;
  UINTN Index, NextIndex, DevCount;
  UINT8 NextDeviceAddress;
  I2C_MASTER_CONTEXT *I2cMasterContext = I2C_SC_FROM_ENUMERATE(This);

  DevCount = PcdGetSize (PcdI2cSlaveAddresses);
  DevicesPcd = PcdGetPtr (PcdI2cSlaveAddresses);
  DeviceBusPcd = PcdGetPtr (PcdI2cSlaveBuses);
  if (*Device == NULL) {
    for (Index = 0; Index < DevCount ; Index++) {
      if (DeviceBusPcd[Index] != I2cMasterContext->Bus)
        continue;
      if (Index < DevCount)
        I2cAllocDevice (DevicesPcd[Index], I2cMasterContext->Bus, Device);

      return EFI_SUCCESS;
    }
  } else {
    /* Device is not NULL, so something was already allocated */
    for (Index = 0; Index < DevCount; Index++) {
      if (DeviceBusPcd[Index] != I2cMasterContext->Bus)
        continue;
      if (DevicesPcd[Index] == I2C_DEVICE_ADDRESS((*Device)->DeviceIndex)) {
        for (NextIndex = Index + 1; NextIndex < DevCount; NextIndex++) {
          if (DeviceBusPcd[NextIndex] != I2cMasterContext->Bus)
            continue;
          NextDeviceAddress = DevicesPcd[NextIndex];
          if (NextIndex < DevCount)
            I2cAllocDevice(NextDeviceAddress, I2cMasterContext->Bus, Device);

          return EFI_SUCCESS;
        }
      }
    }
    *Device = NULL;
    return EFI_SUCCESS;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
I2cEnableConf (
  IN CONST EFI_I2C_BUS_CONFIGURATION_MANAGEMENT_PROTOCOL *This,
  IN UINTN                                               I2cBusConfiguration,
  IN EFI_EVENT                                           Event      OPTIONAL,
  IN EFI_STATUS                                          *I2cStatus OPTIONAL
  )
{
  /* do nothing */
  if (I2cStatus != NULL)
    I2cStatus = EFI_SUCCESS;
  if (Event != NULL)
    gBS->SignalEvent(Event);

  return EFI_SUCCESS;
}
