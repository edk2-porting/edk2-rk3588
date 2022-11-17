/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/I2cAccessLib.h>

EFI_STATUS
I2cWriteRead (
  IN UINTN  MmioBase,
  IN UINT8  SlaveAddress,
  IN UINT8  WriteLength,
  IN UINT8  *WriteBuffer,
  IN UINT8  ReadLength,
  IN UINT8  *ReadBuffer,
  IN UINT64  TimeBudget
  //TODO: add Speed parameter
  )
{
  UINT8 ReadsNeeded = ReadLength;
  UINT64 CutOffTime;

  if ((WriteLength == 0 && ReadLength == 0) ||
      (WriteLength != 0 && WriteBuffer == NULL) ||
      (ReadLength != 0 && ReadBuffer == NULL) ) {
    DEBUG ((DEBUG_ERROR, "I2cWR Invalid Parameters\n"));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Sanity checks to verify the I2C controller is alive
  // Conveniently, ICON register's values of 0 or FFFFFFFF indicate
  // I2c controller is out-of-order: either disabled, in D3 or in reset.
  //
  if (MmioRead32(MmioBase+R_IC_CON) == 0xFFFFFFFF || MmioRead32(MmioBase+R_IC_CON) == 0x0) {
    DEBUG ((DEBUG_ERROR, "I2cWR Device Error\n"));
    return EFI_DEVICE_ERROR;
  }

  MmioWrite32(MmioBase+R_IC_ENABLE, 0x0);
  MmioRead32(MmioBase+0x40);
  MmioRead32(MmioBase+R_IC_CLR_TX_ABRT);
  MmioWrite32(MmioBase+R_IC_SDA_HOLD, 0x001C001C);
  //
  // Set I2C Bus Speed at 400 kHz for GPIO Expander
  //
  MmioWrite32(MmioBase + R_IC_FS_SCL_HCNT, 128);
  MmioWrite32(MmioBase + R_IC_FS_SCL_LCNT, 160);
  MmioWrite32(MmioBase + R_IC_TAR, SlaveAddress);
  MmioWrite32(MmioBase + R_IC_CON, B_IC_MASTER_MODE | V_IC_SPEED_FAST | B_IC_RESTART_EN | B_IC_SLAVE_DISABLE );
  MmioWrite32(MmioBase+R_IC_ENABLE, 0x1);
  CutOffTime = AsmReadTsc() + TimeBudget;

  while ( (MmioRead32(MmioBase+R_IC_ENABLE_STATUS) & 1)==0 ) {
    if (AsmReadTsc() > CutOffTime) {
      DEBUG ((DEBUG_ERROR, "I2cWR timeout\n"));
      return EFI_TIMEOUT;
    }
  }

  while(1) {
    if(MmioRead32(MmioBase+R_IC_INTR_STAT) & B_IC_INTR_TX_ABRT) {
      DEBUG ((DEBUG_ERROR, "I2cWR Transfer aborted, reason = 0x%08x\n",MmioRead32(MmioBase+R_IC_TX_ABRT_SOURCE)));
      MmioRead32(MmioBase+R_IC_CLR_TX_ABRT);
      MmioAnd32(MmioBase+R_IC_ENABLE, 0xFFFFFFFE);
      while ( (MmioRead32(MmioBase+R_IC_ENABLE_STATUS) & 1)==1 ) {}
      return EFI_DEVICE_ERROR;
    }
    if (MmioRead32(MmioBase+R_IC_STATUS) & B_IC_STATUS_TFNF) {
      if (WriteLength > 1) {
        MmioWrite32(MmioBase+R_IC_DATA_CMD, *WriteBuffer);
        WriteBuffer++;
        WriteLength--;
      } else if (WriteLength==1 && ReadLength != 0) {
        MmioWrite32(MmioBase+R_IC_DATA_CMD, *WriteBuffer);
        WriteBuffer++;
        WriteLength--;
      } else if (WriteLength==1 && ReadLength == 0) {
        MmioWrite32(MmioBase+R_IC_DATA_CMD, *WriteBuffer | B_IC_CMD_STOP);
        WriteBuffer++;
        WriteLength--;
      } else if (ReadLength > 1) {
        MmioWrite32(MmioBase+R_IC_DATA_CMD, B_IC_CMD_READ);
        ReadLength--;
      } else if (ReadLength == 1) {
        MmioWrite32(MmioBase+R_IC_DATA_CMD, B_IC_CMD_READ|B_IC_CMD_STOP);
        ReadLength--;
      }
    }

    if (ReadsNeeded) {
      if (MmioRead32(MmioBase+R_IC_STATUS) & B_IC_STATUS_RFNE) {
        *ReadBuffer = (UINT8)MmioRead32(MmioBase+R_IC_DATA_CMD);
        ReadBuffer++;
        ReadsNeeded--;
      }
    }
    if (WriteLength==0 && ReadsNeeded==0 && !(MmioRead32(MmioBase+R_IC_STATUS)&B_IC_STATUS_ACTIVITY)) {
      MmioAnd32(MmioBase+R_IC_ENABLE, 0xFFFFFFFE);
      while ( (MmioRead32(MmioBase+R_IC_ENABLE_STATUS) & 1)==1 ) {}
      DEBUG ((DEBUG_INFO, "I2cWR success\n"));
      return EFI_SUCCESS;
    }
    if (AsmReadTsc() > CutOffTime) {
      MmioAnd32(MmioBase+R_IC_ENABLE, 0xFFFFFFFE);
      while ( (MmioRead32(MmioBase+R_IC_ENABLE_STATUS) & 1)==1 ) {}
      DEBUG ((DEBUG_ERROR, "I2cWR wrong ENST value\n"));
      return EFI_TIMEOUT;
    }

  }
}

