/** @file
  PCH Smbus Executive Code (common PEI/DXE/SMM code)

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <IndustryStandard/SmBus.h>
#include <IndustryStandard/Pci30.h>
#include <PchAccess.h>
#include <Library/MmPciLib.h>
#include <Library/PchSmbusCommonLib.h>

/**
  Get SMBUS IO Base address


  @retval UINT32                  The SMBUS IO Base Address
**/
UINT32
SmbusGetIoBase (
  VOID
  )
{
  UINT32  SmbusIoBase;

  SmbusIoBase = MmioRead32 (
                  MmPciBase (
                    DEFAULT_PCI_BUS_NUMBER_PCH,
                    PCI_DEVICE_NUMBER_PCH_SMBUS,
                    PCI_FUNCTION_NUMBER_PCH_SMBUS)
                    + R_PCH_SMBUS_BASE) & B_PCH_SMBUS_BASE_BAR;

  ASSERT (SmbusIoBase != B_PCH_SMBUS_BASE_BAR && SmbusIoBase != 0);

  return SmbusIoBase;
}

/**
  This function provides a standard way to read PCH Smbus IO registers.

  @param[in] Offset               Register offset from Smbus base IO address.

  @retval UINT8                   Returns data read from IO.
**/
UINT8
EFIAPI
SmbusIoRead (
  IN      UINT8           Offset
  )
{
  return IoRead8 (SmbusGetIoBase () + Offset);
}

/**
  This function provides a standard way to write PCH Smbus IO registers.

  @param[in] Offset               Register offset from Smbus base IO address.
  @param[in] Data                 Data to write to register.

**/
VOID
EFIAPI
SmbusIoWrite (
  IN      UINT8           Offset,
  IN      UINT8           Data
  )
{
  ///
  /// Write New Value
  ///
  IoWrite8 (SmbusGetIoBase () + Offset, Data);
  return;
}

/**
  This function provides a standard way to check if an SMBus transaction has
  completed.

  @param[in] StsReg               Not used for input. On return, contains the
                                  value of the SMBus status register.

  @retval TRUE                    Transaction is complete
  @retval FALSE                   Otherwise.
**/
BOOLEAN
EFIAPI
IoDone (
  IN      UINT8           *StsReg
  )
{
  ///
  /// Wait for IO to complete
  ///
  UINTN StallIndex;
  UINTN StallTries;

  StallTries = STALL_TIME / STALL_PERIOD;

  for (StallIndex = 0; StallIndex < StallTries; StallIndex++) {
    *StsReg = SmbusIoRead (R_PCH_SMBUS_HSTS);
    if (*StsReg & (B_PCH_SMBUS_INTR | B_PCH_SMBUS_BYTE_DONE_STS | B_PCH_SMBUS_DERR | B_PCH_SMBUS_BERR)) {
      return TRUE;
    } else {
      MicroSecondDelay (STALL_PERIOD);
    }
  }

  return FALSE;
}

/**
  Check if it's ok to use the bus.


  @retval EFI_SUCCESS             SmBus is acquired and it's safe to send commands.
  @retval EFI_TIMEOUT             SmBus is busy, it's not safe to send commands.
**/
EFI_STATUS
AcquireBus (
  VOID
  )
{
  UINT8 StsReg;

  StsReg  = 0;
  StsReg  = SmbusIoRead (R_PCH_SMBUS_HSTS);
  if (StsReg & B_PCH_SMBUS_IUS) {
    return EFI_TIMEOUT;
  } else if (StsReg & B_PCH_SMBUS_HBSY) {
    ///
    /// Clear Status Register and exit
    ///
    SmbusIoWrite (R_PCH_SMBUS_HSTS, B_PCH_SMBUS_HSTS_ALL);
    return EFI_TIMEOUT;
  } else {
    ///
    /// Clear out any odd status information (Will Not Clear In Use)
    ///
    SmbusIoWrite (R_PCH_SMBUS_HSTS, StsReg);
    return EFI_SUCCESS;
  }
}

/**
  This function provides a standard way to execute Smbus protocols
  as defined in the SMBus Specification. The data can either be of
  the Length byte, word, or a block of data. The resulting transaction will be
  either the SMBus Slave Device accepts this transaction or this function
  returns with an error

  @param[in] SlaveAddress         Smbus Slave device the command is directed at
  @param[in] Command              Slave Device dependent
  @param[in] Operation            Which SMBus protocol will be used
  @param[in] PecCheck             Defines if Packet Error Code Checking is to be used
  @param[in, out] Length          How many bytes to read. Must be 0 <= Length <= 32 depending on Operation
                                  It will contain the actual number of bytes read/written.
  @param[in, out] Buffer          Contain the data read/written.

  @retval EFI_SUCCESS             The operation completed successfully.
  @exception EFI_UNSUPPORTED      The operation is unsupported.

  @retval EFI_INVALID_PARAMETER   Length or Buffer is NULL for any operation besides
                                  quick read or quick write.
  @retval EFI_TIMEOUT             The transaction did not complete within an internally
                                  specified timeout period, or the controller is not
                                  available for use.
  @retval EFI_DEVICE_ERROR        There was an Smbus error (NACK) during the operation.
                                  This could indicate the slave device is not present
                                  or is in a hung condition.
**/
EFI_STATUS
SmbusExec (
  IN      EFI_SMBUS_DEVICE_ADDRESS  SlaveAddress,
  IN      EFI_SMBUS_DEVICE_COMMAND  Command,
  IN      EFI_SMBUS_OPERATION       Operation,
  IN      BOOLEAN                   PecCheck,
  IN OUT  UINTN                     *Length,
  IN OUT  VOID                      *Buffer
  )
{
  EFI_STATUS  Status;
  UINT8       AuxcReg;
  UINT8       AuxStsReg;
  UINT8       SmbusOperation;
  UINT8       StsReg;
  UINT8       SlvAddrReg;
  UINT8       HostCmdReg;
  UINT8       BlockCount;
  BOOLEAN     BufferTooSmall;
  UINTN       Index;
  UINTN       BusIndex;
  UINT8       *CallBuffer;
  UINT8       SmbusHctl;
  UINT32      Timeout;

  CallBuffer  = Buffer;
  BlockCount  = 0;

  ///
  /// For any operations besides quick read & write, the pointers to
  /// Length and Buffer must not be NULL.
  ///
  if ((Operation != EfiSmbusQuickRead) && (Operation != EfiSmbusQuickWrite)) {
    if ((Length == NULL) || (Buffer == NULL)) {
      return EFI_INVALID_PARAMETER;
    }
  }
  ///
  /// See if its ok to use the bus based upon INUSE_STS bit.
  ///
  Status = AcquireBus ();
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ///
  /// This is the main operation loop.  If the operation results in a Smbus
  /// collision with another master on the bus, it attempts the requested
  /// transaction again at least BUS_TRIES attempts.
  ///
  for (BusIndex = 0; BusIndex < BUS_TRIES; BusIndex++) {
    ///
    /// Operation Specifics (pre-execution)
    ///
    Status          = EFI_SUCCESS;
    SmbusOperation  = V_PCH_SMBUS_SMB_CMD_QUICK;
    SlvAddrReg      = (UINT8) ((SlaveAddress.SmbusDeviceAddress << 1) | 1);
    HostCmdReg      = (UINT8) Command;
    AuxcReg         = 0;

    switch (Operation) {

      case EfiSmbusQuickWrite:
        SlvAddrReg--;

        ///
        /// The "break;" command is not present here to allow code execution
        /// do drop into the next case, which contains common code to this case.
        ///
      case EfiSmbusQuickRead:
        if (PecCheck == TRUE) {
          Status = EFI_UNSUPPORTED;
        }
        break;

      case EfiSmbusSendByte:
        HostCmdReg = CallBuffer[0];
        SlvAddrReg--;

        ///
        /// The "break;" command is not present here to allow code execution
        /// do drop into the next case, which contains common code to this case.
        ///
      case EfiSmbusReceiveByte:
        SmbusOperation = V_PCH_SMBUS_SMB_CMD_BYTE;
        if (*Length < 1) {
          Status = EFI_BUFFER_TOO_SMALL;
        }

        *Length = 1;
        break;

      case EfiSmbusWriteByte:
        SmbusIoWrite (R_PCH_SMBUS_HD0, CallBuffer[0]);
        SlvAddrReg--;
        *Length = 1;

        ///
        /// The "break;" command is not present here to allow code execution
        /// do drop into the next case, which contains common code to this case.
        ///
      case EfiSmbusReadByte:
        if (*Length < 1) {
          Status = EFI_BUFFER_TOO_SMALL;
        } else if (*Length == 1) {
          SmbusOperation = V_PCH_SMBUS_SMB_CMD_BYTE_DATA;
        } else if (*Length <= 256) {
          if (PecCheck == TRUE) {
            ///
            /// The I2C Read command with either PEC_EN or AAC bit set
            /// produces undefined results.
            ///
            Status = EFI_UNSUPPORTED;
          }

          SmbusOperation = V_PCH_SMBUS_SMB_CMD_IIC_READ;
        } else {
          Status = EFI_INVALID_PARAMETER;
        }

        break;

      case EfiSmbusReadWord:
        SmbusOperation = V_PCH_SMBUS_SMB_CMD_WORD_DATA;
        if (*Length < 2) {
          Status = EFI_BUFFER_TOO_SMALL;
        }

        *Length = 2;
        break;

      case EfiSmbusWriteWord:
        SmbusOperation = V_PCH_SMBUS_SMB_CMD_WORD_DATA;
        SlvAddrReg--;
        SmbusIoWrite (R_PCH_SMBUS_HD1, CallBuffer[1]);
        SmbusIoWrite (R_PCH_SMBUS_HD0, CallBuffer[0]);
        if (*Length < 2) {
          Status = EFI_BUFFER_TOO_SMALL;
        }

        *Length = 2;
        break;

      case EfiSmbusWriteBlock:
        SmbusIoWrite (R_PCH_SMBUS_HD0, *(UINT8 *) Length);
        SlvAddrReg--;
        BlockCount = (UINT8) (*Length);

        ///
        /// The "break;" command is not present here to allow code execution
        /// do drop into the next case, which contains common code to this case.
        ///
      case EfiSmbusReadBlock:
        SmbusOperation = V_PCH_SMBUS_SMB_CMD_BLOCK;
        if ((*Length < 1) || (*Length > 32)) {
          Status = EFI_INVALID_PARAMETER;
          break;
        }

        AuxcReg |= B_PCH_SMBUS_E32B;
        break;

      case EfiSmbusProcessCall:
        SmbusOperation = V_PCH_SMBUS_SMB_CMD_PROCESS_CALL;
        SmbusIoWrite (R_PCH_SMBUS_HD1, CallBuffer[1]);
        SmbusIoWrite (R_PCH_SMBUS_HD0, CallBuffer[0]);
        if (*Length < 2) {
          Status = EFI_BUFFER_TOO_SMALL;
        }

        *Length = 2;
        break;

      case EfiSmbusBWBRProcessCall:
        ///
        /// The write byte count cannot be zero or more than
        /// 32 bytes.
        ///
        if ((*Length < 1) || (*Length > 32)) {
          Status = EFI_INVALID_PARAMETER;
          break;
        }

        SmbusIoWrite (R_PCH_SMBUS_HD0, *(UINT8 *) Length);
        BlockCount      = (UINT8) (*Length);
        SmbusOperation  = V_PCH_SMBUS_SMB_CMD_BLOCK_PROCESS;

        AuxcReg |= B_PCH_SMBUS_E32B;
        break;

      default:
        Status = EFI_INVALID_PARAMETER;
        break;
    }

    if (EFI_ERROR (Status)) {
      break;
    }

    if (PecCheck == TRUE) {
      AuxcReg |= B_PCH_SMBUS_AAC;
    }
    ///
    /// Set Auxiliary Control register
    ///
    SmbusIoWrite (R_PCH_SMBUS_AUXC, AuxcReg);

    ///
    /// Reset the pointer of the internal buffer
    ///
    SmbusIoRead (R_PCH_SMBUS_HCTL);

    ///
    /// Now that the 32 byte buffer is turned on, we can write th block data
    /// into it
    ///
    if ((Operation == EfiSmbusWriteBlock) || (Operation == EfiSmbusBWBRProcessCall)) {
      for (Index = 0; Index < BlockCount; Index++) {
        ///
        /// Write next byte
        ///
        SmbusIoWrite (R_PCH_SMBUS_HBD, CallBuffer[Index]);
      }
    }
    ///
    /// Set SMBus slave address for the device to send/receive from
    ///
    SmbusIoWrite (R_PCH_SMBUS_TSA, SlvAddrReg);

    ///
    /// For I2C read, send DATA1 register for the offset (address)
    /// within the serial memory chips
    ///
    if ((Operation == EfiSmbusReadByte) && (*Length > 1)) {
      SmbusIoWrite (R_PCH_SMBUS_HD1, HostCmdReg);
    } else {
      ///
      /// Set Command register
      ///
      SmbusIoWrite (R_PCH_SMBUS_HCMD, HostCmdReg);
    }
    ///
    /// Set Control Register (Initiate Operation, Interrupt disabled)
    ///
    SmbusIoWrite (R_PCH_SMBUS_HCTL, (UINT8) (SmbusOperation + B_PCH_SMBUS_START));

    ///
    /// Wait for IO to complete
    ///
    if (!IoDone (&StsReg)) {
      Status = EFI_TIMEOUT;
      break;
    } else if (StsReg & B_PCH_SMBUS_DERR) {
      AuxStsReg = SmbusIoRead (R_PCH_SMBUS_AUXS);
      if (AuxStsReg & B_PCH_SMBUS_CRCE) {
        Status = EFI_CRC_ERROR;
      } else {
        Status = EFI_DEVICE_ERROR;
      }
      break;
    } else if (StsReg & B_PCH_SMBUS_BERR) {
      ///
      /// Clear the Bus Error for another try
      ///
      Status = EFI_DEVICE_ERROR;
      SmbusIoWrite (R_PCH_SMBUS_HSTS, B_PCH_SMBUS_BERR);
      ///
      /// Clear Status Registers
      ///
      SmbusIoWrite (R_PCH_SMBUS_HSTS, B_PCH_SMBUS_HSTS_ALL);
      SmbusIoWrite (R_PCH_SMBUS_AUXS, B_PCH_SMBUS_CRCE);
      ///
      /// If bus collision happens, stall some time, then try again
      /// Here we choose 10 milliseconds to avoid MTCP transfer.
      ///
      MicroSecondDelay (STALL_PERIOD);
      continue;
    }
    ///
    /// successfull completion
    /// Operation Specifics (post-execution)
    ///
    switch (Operation) {

      case EfiSmbusReadWord:
        ///
        /// The "break;" command is not present here to allow code execution
        /// do drop into the next case, which contains common code to this case.
        ///
      case EfiSmbusProcessCall:
        CallBuffer[1] = SmbusIoRead (R_PCH_SMBUS_HD1);
        CallBuffer[0] = SmbusIoRead (R_PCH_SMBUS_HD0);
        break;

      case EfiSmbusReadByte:
        if (*Length > 1) {
          for (Index = 0; Index < *Length; Index++) {
            ///
            /// Read the byte
            ///
            CallBuffer[Index] = SmbusIoRead (R_PCH_SMBUS_HBD);
            ///
            /// After receiving byte n-1 (1-base) of the message, the
            /// software will then set the LAST BYTE bit. The software
            /// will then clear the BYTE_DONE_STS bit.
            ///
            if (Index == ((*Length - 1) - 1)) {
              SmbusHctl = SmbusIoRead (R_PCH_SMBUS_HCTL) | (UINT8) B_PCH_SMBUS_LAST_BYTE;
              SmbusIoWrite (R_PCH_SMBUS_HCTL, SmbusHctl);
            } else if (Index == (*Length - 1)) {
              ///
              /// Clear the LAST BYTE bit after receiving byte n (1-base) of the message
              ///
              SmbusHctl = SmbusIoRead (R_PCH_SMBUS_HCTL) & (UINT8) ~B_PCH_SMBUS_LAST_BYTE;
              SmbusIoWrite (R_PCH_SMBUS_HCTL, SmbusHctl);
            }
            ///
            /// Clear the BYTE_DONE_STS bit
            ///
            SmbusIoWrite (R_PCH_SMBUS_HSTS, B_PCH_SMBUS_BYTE_DONE_STS);
            ///
            /// Check BYTE_DONE_STS bit to know if it has completed transmission
            /// of a byte. No need to check it for the last byte.
            ///
            if (Index < (*Length - 1)) {
              ///
              /// If somehow board operates at 10Khz, it will take 0.9 ms (9/10Khz) for another byte.
              /// Add 10 us delay for a loop of 100 that the total timeout is 1 ms to take care of
              /// the slowest case.
              ///
              for (Timeout = 0; Timeout < 100; Timeout++) {
                if ((SmbusIoRead (R_PCH_SMBUS_HSTS) & (UINT8) B_PCH_SMBUS_BYTE_DONE_STS) != 0) {
                  break;
                }
                ///
                /// Delay 10 us
                ///
                MicroSecondDelay (STALL_PERIOD);
              }

              if (Timeout >= 100) {
                Status = EFI_TIMEOUT;
                break;
              }
            }
          }
          break;
        }

      case EfiSmbusReceiveByte:
        CallBuffer[0] = SmbusIoRead (R_PCH_SMBUS_HD0);
        break;

      case EfiSmbusWriteBlock:
        SmbusIoWrite (R_PCH_SMBUS_HSTS, B_PCH_SMBUS_BYTE_DONE_STS);
        break;

      case EfiSmbusReadBlock:
        BufferTooSmall = FALSE;
        ///
        /// Find out how many bytes will be in the block
        ///
        BlockCount = SmbusIoRead (R_PCH_SMBUS_HD0);
        if (*Length < BlockCount) {
          BufferTooSmall = TRUE;
        } else {
          for (Index = 0; Index < BlockCount; Index++) {
            ///
            /// Read the byte
            ///
            CallBuffer[Index] = SmbusIoRead (R_PCH_SMBUS_HBD);
          }
        }

        *Length = BlockCount;
        if (BufferTooSmall) {
          Status = EFI_BUFFER_TOO_SMALL;
        }
        break;

      case EfiSmbusBWBRProcessCall:
        ///
        /// Find out how many bytes will be in the block
        ///
        BlockCount = SmbusIoRead (R_PCH_SMBUS_HD0);
        ///
        /// The read byte count cannot be zero.
        ///
        if (BlockCount < 1) {
          Status = EFI_BUFFER_TOO_SMALL;
          break;
        }
        ///
        /// The combined data payload (the write byte count + the read byte count)
        /// must not exceed 32 bytes
        ///
        if (((UINT8) (*Length) + BlockCount) > 32) {
          Status = EFI_DEVICE_ERROR;
          break;
        }

        for (Index = 0; Index < BlockCount; Index++) {
          ///
          /// Read the byte
          ///
          CallBuffer[Index] = SmbusIoRead (R_PCH_SMBUS_HBD);
        }

        *Length = BlockCount;
        break;

      default:
        break;
    };

    if ((StsReg & B_PCH_SMBUS_BERR) && (Status != EFI_BUFFER_TOO_SMALL)) {
      ///
      /// Clear the Bus Error for another try
      ///
      Status = EFI_DEVICE_ERROR;
      SmbusIoWrite (R_PCH_SMBUS_HSTS, B_PCH_SMBUS_BERR);
      ///
      /// If bus collision happens, stall some time, then try again
      /// Here we choose 10 milliseconds to avoid MTCP transfer.
      ///
      MicroSecondDelay (STALL_PERIOD);
      continue;
    } else {
      break;
    }
  }
  ///
  /// Clear Status Registers and exit
  ///
  SmbusIoWrite (R_PCH_SMBUS_HSTS, B_PCH_SMBUS_HSTS_ALL);
  SmbusIoWrite (R_PCH_SMBUS_AUXS, B_PCH_SMBUS_CRCE);
  SmbusIoWrite (R_PCH_SMBUS_AUXC, 0);
  return Status;
}

/**
  This function initializes the Smbus Registers.

**/
VOID
InitializeSmbusRegisters (
  VOID
  )
{
  UINTN SmbusRegBase;

  SmbusRegBase = MmPciBase (
                   DEFAULT_PCI_BUS_NUMBER_PCH,
                   PCI_DEVICE_NUMBER_PCH_SMBUS,
                   PCI_FUNCTION_NUMBER_PCH_SMBUS
                   );
  ///
  /// Enable the Smbus I/O Enable
  ///
  MmioOr8 (SmbusRegBase + PCI_COMMAND_OFFSET, (UINT8) EFI_PCI_COMMAND_IO_SPACE);

  ///
  /// Enable the Smbus host controller
  ///
  MmioAndThenOr8 (
    SmbusRegBase + R_PCH_SMBUS_HOSTC,
    (UINT8) (~(B_PCH_SMBUS_HOSTC_SMI_EN | B_PCH_SMBUS_HOSTC_I2C_EN)),
    B_PCH_SMBUS_HOSTC_HST_EN
    );

  SmbusIoWrite (R_PCH_SMBUS_HSTS, B_PCH_SMBUS_HSTS_ALL);
}