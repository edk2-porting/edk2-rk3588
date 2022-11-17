/** @file
  I2c Lib to control I2c controller.

  Copyright 2017, 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Revision Reference:
  - PI Version 1.7

  @par Glossary:
  - Ibfd - I2c Bus Frequency Divider
**/
#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/I2cLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>

#include "I2cLibInternal.h"

/**
  I2C divisor and Ibfd register values when glitch filter is enabled

  In case of duplicate SCL Divisor value, the Ibfd value with high MUL value
  has been selected. A higher MUL value results in a lower sampling rate of
  the I2C signals. This gives the I2C module greater immunity against glitches
  in the I2C signals.

  These values can be referred from NXP QorIQ Layerscape series SOC Reference
  Manual in which this I2C ip has been used. e.g. LX2160ARM, LS1043ARM
**/
STATIC CONST I2C_CLOCK_DIVISOR_PAIR mI2cClockDivisorGlitchEnabled[] = {
  { 34, 0x0 }, { 36, 0x1 }, { 38, 0x2 }, { 40, 0x3 },
  { 42, 0x4 }, { 44, 0x8 }, { 48, 0x9 }, { 52, 0xA },
  { 54, 0x7 }, { 56, 0xB }, { 60, 0xC }, { 64, 0x10 },
  { 68, 0x40 }, { 72, 0x41 }, { 76, 0x42 }, { 80, 0x43 },
  { 84, 0x44 }, { 88, 0x48 }, { 96, 0x49 }, { 104, 0x4A },
  { 108, 0x47 }, { 112, 0x4B }, { 120, 0x4C }, { 128, 0x50 },
  { 136, 0x80 }, { 144, 0x81 }, { 152, 0x82 }, { 160, 0x83 },
  { 168, 0x84 }, { 176, 0x88 }, { 192, 0x89 }, { 208, 0x8A },
  { 216, 0x87 }, { 224, 0x8B }, { 240, 0x8C }, { 256, 0x90 },
  { 288, 0x91 }, { 320, 0x92 }, { 336, 0x8F }, { 352, 0x93 },
  { 384, 0x98 }, { 416, 0x95 }, { 448, 0x99 }, { 480, 0x96 },
  { 512, 0x9A }, { 576, 0x9B }, { 640, 0xA0 }, { 704, 0x9D },
  { 768, 0xA1 }, { 832, 0x9E }, { 896, 0xA2 }, { 960, 0x67 },
  { 1024, 0xA3 }, { 1152, 0xA4 }, { 1280, 0xA8 }, { 1536, 0xA9 },
  { 1792, 0xAA }, { 1920, 0xA7 }, { 2048, 0xAB }, { 2304, 0xAC },
  { 2560, 0xB0 }, { 3072, 0xB1 }, { 3584, 0xB2 }, { 3840, 0xAF },
  { 4096, 0xB3 }, { 4608, 0xB4 }, { 5120, 0xB8 }, { 6144, 0xB9 },
  { 7168, 0xBA }, { 7680, 0xB7 }, { 8192, 0xBB }, { 9216, 0xBC },
  { 10240, 0xBD }, { 12288, 0xBE }, { 15360, 0xBF }
};

/**
  I2C divisor and Ibfd register values when glitch filter is disabled

  In case of duplicate SCL Divisor value, the Ibfd value with high MUL value
  has been selected. A higher MUL value results in a lower sampling rate of
  the I2C signals. This gives the I2C module greater immunity against glitches
  in the I2C signals.

  These values can be referred from NXP QorIQ Layerscape series SOC Reference
  Manual in which this I2C ip has been used. e.g. LX2160ARM, LS1043ARM
**/
STATIC CONST I2C_CLOCK_DIVISOR_PAIR mI2cClockDivisorGlitchDisabled[] = {
  { 20, 0x0 },{ 22, 0x1 },{ 24, 0x2 },{ 26, 0x3 },
  { 28, 0x8 },{ 30, 0x5 },{ 32, 0x9 },{ 34, 0x6 },
  { 36, 0x0A },{ 40, 0x40 },{ 44, 0x41 },{ 48, 0x42 },
  { 52, 0x43 },{ 56, 0x48 },{ 60, 0x45 },{ 64, 0x49 },
  { 68, 0x46 },{ 72, 0x4A },{ 80, 0x80 },{ 88, 0x81 },
  { 96, 0x82 },{ 104, 0x83 },{ 112, 0x88 },{ 120, 0x85 },
  { 128, 0x89 },{ 136, 0x86 },{ 144, 0x8A },{ 160, 0x8B },
  { 176, 0x8C },{ 192, 0x90 },{ 208, 0x56 },{ 224, 0x91 },
  { 240, 0x1F },{ 256, 0x92 },{ 272, 0x8F },{ 288, 0x93 },
  { 320, 0x98 },{ 352, 0x95 },{ 384, 0x99 },{ 416, 0x96 },
  { 448, 0x9A },{ 480, 0x5F },{ 512, 0x9B },{ 576, 0x9C },
  { 640, 0xA0 },{ 768, 0xA1 },{ 896, 0xA2 },{ 960, 0x9F },
  { 1024, 0xA3 },{ 1152, 0xA4 },{ 1280, 0xA8 },{ 1536, 0xA9 },
  { 1792, 0xAA },{ 1920, 0xA7 },{ 2048, 0xAB },{ 2304, 0xAC },
  { 2560, 0xAD },{ 3072, 0xB1 },{ 3584, 0xB2 },{ 3840, 0xAF },
  { 4096, 0xB3 },{ 4608, 0xB4 },{ 5120, 0xB8 },{ 6144, 0xB9 },
  { 7168, 0xBA },{ 7680, 0xB7 },{ 8192, 0xBB },{ 9216, 0xBC },
  { 10240, 0xBD },{ 12288, 0xBE },{ 15360, 0xBF }
};

/**
  A-009203 :   I2C may not work reliably with the default setting

  Description : The clocking circuitry of I2C module may not work reliably due
                to the slow rise time of SCL signal.
  Workaround :  Enable the receiver digital filter by setting IBDBG[GLFLT_EN]
                to 1. refer https://patchwork.ozlabs.org/patch/453575/
**/
STATIC
VOID
I2cErratumA009203 (
  IN UINTN  Base
  )
{
  I2C_REGS *Regs;

  Regs = (I2C_REGS *)Base;

  MmioOr8 ((UINTN)&Regs->Ibdbg, I2C_IBDBG_GLFLT_EN);
}

/**
  software reset of the entire I2C module.
  The module is reset and disabled.
  Status register fields (IBSR) are cleared.

  @param[in] Base       Base Address of I2c controller's registers

  @return  EFI_SUCCESS  successfuly reset the I2c module
**/
EFI_STATUS
I2cReset (
  IN UINTN  Base
  )
{
  I2C_REGS *Regs;

  Regs = (I2C_REGS *)Base;

  MmioOr8 ((UINTN)&Regs->Ibcr, I2C_IBCR_MDIS);
  MmioOr8 ((UINTN)&Regs->Ibsr, (I2C_IBSR_IBAL | I2C_IBSR_IBIF));
  MmioAnd8 ((UINTN)&Regs->Ibcr, ~(I2C_IBCR_IBIE | I2C_IBCR_DMAEN));
  MmioAnd8 ((UINTN)&Regs->Ibic, (UINT8)(~I2C_IBIC_BIIE));

  return EFI_SUCCESS;
}

/**
  Configure I2c bus to operate at a given speed

  @param[in] Base         Base Address of I2c controller's registers
  @param[in] I2cBusClock  Input clock to I2c controller
  @param[in] Speed        speed to be configured for I2c bus

  @return  EFI_SUCCESS  successfuly setup the i2c bus
**/
EFI_STATUS
I2cInitialize (
  IN UINTN   Base,
  IN UINT64  I2cBusClock,
  IN UINT64  Speed
  )
{
  I2C_REGS                       *Regs;
  UINT16                         ClockDivisor;
  UINT8                          Ibfd; // I2c Bus Frequency Divider Register
  CONST I2C_CLOCK_DIVISOR_PAIR   *ClockDivisorPair;
  UINT32                         ClockDivisorPairSize;
  UINT32                         Index;

  Regs = (I2C_REGS *)Base;
  if (FeaturePcdGet (PcdI2cErratumA009203)) {
    // Apply Erratum A-009203 before writing Ibfd register
    I2cErratumA009203 (Base);
  }

  Ibfd = 0;
  ClockDivisor = (I2cBusClock + Speed - 1) / Speed;

  if (MmioRead8 ((UINTN)&Regs->Ibdbg) & I2C_IBDBG_GLFLT_EN) {
    ClockDivisorPair = mI2cClockDivisorGlitchEnabled;
    ClockDivisorPairSize = ARRAY_SIZE (mI2cClockDivisorGlitchEnabled);
  } else {
    ClockDivisorPair = mI2cClockDivisorGlitchDisabled;
    ClockDivisorPairSize = ARRAY_SIZE (mI2cClockDivisorGlitchDisabled);
  }

  if (ClockDivisor > ClockDivisorPair[ClockDivisorPairSize - 1].Divisor) {
    Ibfd = ClockDivisorPair[ClockDivisorPairSize - 1].Ibfd;
  } else {
    for (Index = 0; Index < ClockDivisorPairSize; Index++) {
      if (ClockDivisorPair[Index].Divisor >= ClockDivisor) {
        Ibfd = ClockDivisorPair[Index].Ibfd;
        break;
      }
    }
  }

  MmioWrite8 ((UINTN)&Regs->Ibfd, Ibfd);

  I2cReset (Base);

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
I2cBusTestBusBusy (
  IN  I2C_REGS  *Regs,
  IN  BOOLEAN   TestBusy
  )
{
  UINT32  Index;
  UINT8   Reg;

  for (Index = 0; Index < I2C_NUM_RETRIES; Index++) {
    Reg = MmioRead8 ((UINTN)&Regs->Ibsr);

    if (Reg & I2C_IBSR_IBAL) {
      MmioWrite8 ((UINTN)&Regs->Ibsr, Reg);
      return EFI_NOT_READY;
    }

    if (TestBusy && (Reg & I2C_IBSR_IBB)) {
      break;
    }

    if (!TestBusy && !(Reg & I2C_IBSR_IBB)) {
      break;
    }

    MicroSecondDelay (1);
  }

  if (Index == I2C_NUM_RETRIES) {
    return EFI_TIMEOUT;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
I2cTransferComplete (
  IN  I2C_REGS  *Regs,
  IN  BOOLEAN   TestRxAck
)
{
  UINT32     Index;
  UINT8      Reg;

  for (Index = 0; Index < I2C_NUM_RETRIES; Index++) {
    Reg = MmioRead8 ((UINTN)&Regs->Ibsr);

    if (Reg & I2C_IBSR_IBIF) {
      // Write 1 to clear the IBIF field
      MmioWrite8 ((UINTN)&Regs->Ibsr, Reg);
      break;
    }

    MicroSecondDelay (1);
  }

  if (Index == I2C_NUM_RETRIES) {
    return EFI_TIMEOUT;
  }

  if (TestRxAck && (Reg & I2C_IBSR_RXAK)) {
    return EFI_NO_RESPONSE;
  }

  if (Reg & I2C_IBSR_TCF) {
    return EFI_SUCCESS;
  }

  return EFI_DEVICE_ERROR;
}

STATIC
EFI_STATUS
I2cRead (
  IN  I2C_REGS           *Regs,
  IN  UINT32             SlaveAddress,
  IN  EFI_I2C_OPERATION  *Operation,
  IN  BOOLEAN            IsLastOperation
)
{
  EFI_STATUS Status;
  UINTN      Index;

  // Write Slave Address
  MmioWrite8 ((UINTN)&Regs->Ibdr, (SlaveAddress << BIT0) | BIT0);
  Status = I2cTransferComplete (Regs, I2C_BUS_TEST_RX_ACK);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  // select Receive mode.
  MmioAnd8 ((UINTN)&Regs->Ibcr, ~I2C_IBCR_TXRX);
  if (Operation->LengthInBytes > 1) {
    // Set No ACK = 0
    MmioAnd8 ((UINTN)&Regs->Ibcr, ~I2C_IBCR_NOACK);
  }

  // Perform a dummy read to initiate the receive operation.
  MmioRead8 ((UINTN)&Regs->Ibdr);

  for (Index = 0; Index < Operation->LengthInBytes; Index++) {
    Status = I2cTransferComplete (Regs, I2C_BUS_NO_TEST_RX_ACK);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    if (Index == (Operation->LengthInBytes - 2)) {
      // Set No ACK = 1
      MmioOr8 ((UINTN)&Regs->Ibcr, I2C_IBCR_NOACK);
    } else if (Index == (Operation->LengthInBytes - 1)) {
      if (!IsLastOperation) {
        // select Transmit mode (for repeat start)
        MmioOr8 ((UINTN)&Regs->Ibcr, I2C_IBCR_TXRX);
      } else {
        // Generate Stop Signal
        MmioAnd8 ((UINTN)&Regs->Ibcr, ~(I2C_IBCR_MSSL | I2C_IBCR_TXRX));
        Status = I2cBusTestBusBusy (Regs, I2C_BUS_TEST_IDLE);
        if (EFI_ERROR (Status)) {
          return Status;
        }
      }
    }
    Operation->Buffer[Index] = MmioRead8 ((UINTN)&Regs->Ibdr);
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
I2cWrite (
  IN  I2C_REGS           *Regs,
  IN  UINT32             SlaveAddress,
  IN  EFI_I2C_OPERATION  *Operation
)
{
  EFI_STATUS Status;
  UINTN      Index;

  // Write Slave Address
  MmioWrite8 ((UINTN)&Regs->Ibdr, (SlaveAddress << BIT0) & (UINT8)(~BIT0));
  Status = I2cTransferComplete (Regs, I2C_BUS_TEST_RX_ACK);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Write Data
  for (Index = 0; Index < Operation->LengthInBytes; Index++) {
    MmioWrite8 ((UINTN)&Regs->Ibdr, Operation->Buffer[Index]);
    Status = I2cTransferComplete (Regs, I2C_BUS_TEST_RX_ACK);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
I2cStop (
  IN  I2C_REGS  *Regs
  )
{
  EFI_STATUS Status;
  UINT8      Reg;

  Status = EFI_SUCCESS;
  Reg = MmioRead8 ((UINTN)&Regs->Ibsr);
  if (Reg & I2C_IBSR_IBB) {
    // Generate Stop Signal
    MmioAnd8 ((UINTN)&Regs->Ibcr, ~(I2C_IBCR_MSSL | I2C_IBCR_TXRX));
    Status = I2cBusTestBusBusy (Regs, I2C_BUS_TEST_IDLE);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  // Disable I2c Controller
  MmioOr8 ((UINTN)&Regs->Ibcr, I2C_IBCR_MDIS);

  return Status;
}

STATIC
EFI_STATUS
I2cStart (
  IN  I2C_REGS  *Regs
  )
{
  EFI_STATUS Status;

  MmioOr8 ((UINTN)&Regs->Ibsr, (I2C_IBSR_IBAL | I2C_IBSR_IBIF));
  MmioAnd8 ((UINTN)&Regs->Ibcr, (UINT8)(~I2C_IBCR_MDIS));

  //Wait controller to be stable
  MicroSecondDelay (1);

  // Generate Start Signal
  MmioOr8 ((UINTN)&Regs->Ibcr, I2C_IBCR_MSSL);
  Status = I2cBusTestBusBusy (Regs, I2C_BUS_TEST_BUSY);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Select Transmit Mode. set No ACK = 1
  MmioOr8 ((UINTN)&Regs->Ibcr, (I2C_IBCR_TXRX | I2C_IBCR_NOACK));

  return Status;
}

/**
  Transfer data to/from I2c slave device

  @param[in] Base           Base Address of I2c controller's registers
  @param[in] SlaveAddress   Slave Address from which data is to be read
  @param[in] RequestPacket  Pointer to an EFI_I2C_REQUEST_PACKET structure
                            describing the I2C transaction

  @return  EFI_SUCCESS       successfuly transfer the data
  @return  EFI_DEVICE_ERROR  There was an error while transferring data through
                             I2c bus
  @return  EFI_NO_RESPONSE   There was no Ack from i2c device
  @return  EFI_TIMEOUT       I2c Bus is busy
  @return  EFI_NOT_READY     I2c Bus Arbitration lost
**/
EFI_STATUS
I2cBusXfer (
  IN UINTN                  Base,
  IN UINT32                 SlaveAddress,
  IN EFI_I2C_REQUEST_PACKET *RequestPacket
  )
{
  UINTN              Index;
  I2C_REGS           *Regs;
  EFI_I2C_OPERATION  *Operation;
  EFI_STATUS         Status;
  BOOLEAN            IsLastOperation;

  Regs = (I2C_REGS *)Base;
  IsLastOperation = FALSE;

  Status = I2cBusTestBusBusy (Regs, I2C_BUS_TEST_IDLE);
  if (EFI_ERROR (Status)) {
    goto ErrorExit;
  }

  Status = I2cStart (Regs);
  if (EFI_ERROR (Status)) {
    goto ErrorExit;
  }

  for (Index = 0, Operation = RequestPacket->Operation;
       Index < RequestPacket->OperationCount;
       Index++, Operation++) {
    if (Index == (RequestPacket->OperationCount - 1)) {
      IsLastOperation = TRUE;
    }
    // Send repeat start after first transmit/recieve
    if (Index) {
      MmioOr8 ((UINTN)&Regs->Ibcr, I2C_IBCR_RSTA);
      Status = I2cBusTestBusBusy (Regs, I2C_BUS_TEST_BUSY);
      if (EFI_ERROR (Status)) {
        goto ErrorExit;
      }
    }
    // Read/write data
    if (Operation->Flags & I2C_FLAG_READ) {
      Status = I2cRead (Regs, SlaveAddress, Operation, IsLastOperation);
    } else {
      Status = I2cWrite (Regs, SlaveAddress, Operation);
    }
    if (EFI_ERROR (Status)) {
      goto ErrorExit;
    }
  }

ErrorExit:

  I2cStop (Regs);

  return Status;
}

/**
  Read a register from I2c slave device. This API is wrapper around I2cBusXfer

  @param[in]  Base                   Base Address of I2c controller's registers
  @param[in]  SlaveAddress           Slave Address from which register value is
                                     to be read
  @param[in]  RegAddress             Register Address in Slave's memory map
  @param[in]  RegAddressWidthInBytes Number of bytes in RegAddress to send to
                                     I2c Slave for simple reads without any
                                     register, make this value = 0
                                     (RegAddress is don't care in that case)
  @param[out] RegValue               Value to be read from I2c slave's regiser
  @param[in]  RegValueNumBytes       Number of bytes to read from I2c slave
                                     register

  @return  EFI_SUCCESS       successfuly read the registers
  @return  EFI_DEVICE_ERROR  There was an error while transferring data through
                             I2c bus
  @return  EFI_NO_RESPONSE   There was no Ack from i2c device
  @return  EFI_TIMEOUT       I2c Bus is busy
  @return  EFI_NOT_READY     I2c Bus Arbitration lost
**/
EFI_STATUS
I2cBusReadReg (
  IN  UINTN   Base,
  IN  UINT32  SlaveAddress,
  IN  UINT64  RegAddress,
  IN  UINT8   RegAddressWidthInBytes,
  OUT UINT8   *RegValue,
  IN  UINT32  RegValueNumBytes
  )
{
  EFI_I2C_OPERATION       *Operations;
  I2C_REG_REQUEST         RequestPacket;
  UINTN                   OperationCount;
  UINT8                   Address[sizeof (RegAddress)];
  UINT8                   *AddressPtr;
  EFI_STATUS              Status;

  ZeroMem (&RequestPacket, sizeof (RequestPacket));
  OperationCount = 0;
  Operations = RequestPacket.Operation;
  AddressPtr = Address;

  if (RegAddressWidthInBytes > ARRAY_SIZE (Address)) {
    return EFI_INVALID_PARAMETER;
  }

  if (RegAddressWidthInBytes != 0) {
    Operations[OperationCount].LengthInBytes = RegAddressWidthInBytes;
    Operations[OperationCount].Buffer = AddressPtr;
    while (RegAddressWidthInBytes--) {
      *AddressPtr++ = RegAddress >> (8 * RegAddressWidthInBytes);
    }
    OperationCount++;
  }

  Operations[OperationCount].LengthInBytes = RegValueNumBytes;
  Operations[OperationCount].Buffer = RegValue;
  Operations[OperationCount].Flags = I2C_FLAG_READ;
  OperationCount++;

  RequestPacket.OperationCount = OperationCount;

  Status = I2cBusXfer (
             Base, SlaveAddress,
             (EFI_I2C_REQUEST_PACKET *)&RequestPacket
             );

  return Status;
}
