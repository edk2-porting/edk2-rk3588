/** @file
  PCH SMBUS library implementation built upon I/O library.

Copyright (c) 2017-2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Library/SmbusLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/DebugLib.h>
#include <PchAccess.h>
#include <Library/MmPciLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED UINT16           mSmbusIoBase = 0;

/**
  Gets Io port base address of Smbus Host Controller.

  @retval The Io port base address of Smbus host controller.

**/
UINT16
InternalGetSmbusIoPortBaseAddress (
  VOID
  )
{
  UINTN     SmbusPciBase;
  UINT16    IoPortBaseAddress;

  if (mSmbusIoBase != 0) {
    return mSmbusIoBase;
  }

  SmbusPciBase = MmPciBase (
                   DEFAULT_PCI_BUS_NUMBER_PCH,
                   PCI_DEVICE_NUMBER_PCH_SMBUS,
                   PCI_FUNCTION_NUMBER_PCH_SMBUS
                   );
  IoPortBaseAddress = (UINT16) MmioRead32 (SmbusPciBase + R_PCH_SMBUS_BASE);

  //
  // Make sure that the IO port base address has been properly set.
  //
  if ((IoPortBaseAddress == 0) || (IoPortBaseAddress == 0xFFFF)) {
    ASSERT (FALSE);
    return 0;
  }

  IoPortBaseAddress &= B_PCH_SMBUS_BASE_BAR;
  mSmbusIoBase = IoPortBaseAddress;

  return IoPortBaseAddress;
}


/**
  Acquires the ownership of SMBUS.

  This internal function reads the host state register.
  If the SMBUS is not available, RETURN_TIMEOUT is returned;
  Otherwise, it performs some basic initializations and returns
  RETURN_SUCCESS.

  @param[in]  IoPortBaseAddress The Io port base address of Smbus Host controller.

  @retval     RETURN_SUCCESS    The SMBUS command was executed successfully.
  @retval     RETURN_TIMEOUT    A timeout occurred while executing the SMBUS command.

**/
RETURN_STATUS
InternalSmBusAcquire (
  IN UINT16                   IoPortBaseAddress
  )
{
  UINT8   HostStatus;

  HostStatus = IoRead8 (IoPortBaseAddress + R_PCH_SMBUS_HSTS);
  if ((HostStatus & B_PCH_SMBUS_IUS) != 0) {
    return RETURN_TIMEOUT;
  } else if ((HostStatus & B_PCH_SMBUS_HBSY) != 0) {
    //
    // Clear host status register and exit.
    //
    IoWrite8 (IoPortBaseAddress + R_PCH_SMBUS_HSTS, B_PCH_SMBUS_HSTS_ALL);
    return RETURN_TIMEOUT;
  }
  //
  // Clear out any odd status information (Will Not Clear In Use).
  //
  IoWrite8 (IoPortBaseAddress + R_PCH_SMBUS_HSTS, HostStatus);

  return RETURN_SUCCESS;
}

/**
  Starts the SMBUS transaction and waits until the end.

  This internal function start the SMBUS transaction and waits until the transaction
  of SMBUS is over by polling the INTR bit of Host status register.
  If the SMBUS is not available, RETURN_TIMEOUT is returned;
  Otherwise, it performs some basic initializations and returns
  RETURN_SUCCESS.

  @param[in]  IoPortBaseAddress   The Io port base address of Smbus Host controller.
  @param[in]  HostControl         The Host control command to start SMBUS transaction.

  @retval     RETURN_SUCCESS      The SMBUS command was executed successfully.
  @retval     RETURN_CRC_ERROR    The checksum is not correct (PEC is incorrect).
  @retval     RETURN_DEVICE_ERROR The request was not completed because a failure reflected
                                  in the Host Status Register bit.  Device errors are
                                  a result of a transaction collision, illegal command field,
                                  unclaimed cycle (host initiated), or bus errors (collisions).

**/
RETURN_STATUS
InternalSmBusStart (
  IN  UINT16                  IoPortBaseAddress,
  IN  UINT8                   HostControl
  )
{
  UINT8   HostStatus;
  UINT8   AuxiliaryStatus;

  //
  // Set Host Control Register (Initiate Operation, Interrupt disabled).
  //
  IoWrite8 (IoPortBaseAddress + R_PCH_SMBUS_HCTL, (UINT8)(HostControl + B_PCH_SMBUS_START));

  do {
    //
    // Poll INTR bit of Host Status Register.
    //
    HostStatus = IoRead8 (IoPortBaseAddress + R_PCH_SMBUS_HSTS);
  } while ((HostStatus & (B_PCH_SMBUS_INTR | B_PCH_SMBUS_ERROR | B_PCH_SMBUS_BYTE_DONE_STS)) == 0);

  if ((HostStatus & B_PCH_SMBUS_ERROR) == 0) {
    return RETURN_SUCCESS;
  }
  //
  // Clear error bits of Host Status Register.
  //
  IoWrite8 (IoPortBaseAddress + R_PCH_SMBUS_HSTS, B_PCH_SMBUS_ERROR);
  //
  // Read Auxiliary Status Register to judge CRC error.
  //
  AuxiliaryStatus = IoRead8 (IoPortBaseAddress + R_PCH_SMBUS_AUXS);
  if ((AuxiliaryStatus & B_PCH_SMBUS_CRCE) != 0) {
    return RETURN_CRC_ERROR;
  }

  return RETURN_DEVICE_ERROR;
}

/**
  Executes an SMBUS quick, byte or word command.

  This internal function executes an SMBUS quick, byte or word commond.
  If Status is not NULL, then the status of the executed command is returned in Status.

  @param[in]  HostControl     The value of Host Control Register to set.
  @param[in]  SmBusAddress    Address that encodes the SMBUS Slave Address,
                              SMBUS Command, SMBUS Data Length, and PEC.
  @param[in]  Value           The byte/word write to the SMBUS.
  @param[out] Status          Return status for the executed command.
                              This is an optional parameter and may be NULL.

  @retval The byte/word read from the SMBUS.

**/
UINT16
InternalSmBusNonBlock (
  IN  UINT8                     HostControl,
  IN  UINTN                     SmBusAddress,
  IN  UINT16                    Value,
  OUT RETURN_STATUS             *Status
  )
{
  RETURN_STATUS                 ReturnStatus;
  UINT16                        IoPortBaseAddress;
  UINT8                         AuxiliaryControl;

  IoPortBaseAddress = InternalGetSmbusIoPortBaseAddress ();

  //
  // Try to acquire the ownership of SMBUS.
  //
  ReturnStatus = InternalSmBusAcquire (IoPortBaseAddress);
  if (RETURN_ERROR (ReturnStatus)) {
    goto Done;
  }
  //
  // Set the appropriate Host Control Register and auxiliary Control Register.
  //
  AuxiliaryControl = 0;
  if (SMBUS_LIB_PEC (SmBusAddress)) {
    AuxiliaryControl |= B_PCH_SMBUS_AAC;
    HostControl      |= B_PCH_SMBUS_PEC_EN;
  }
  //
  // We do not need Data Register for SendByte Command
  //
  if ((HostControl == V_PCH_SMBUS_SMB_CMD_BYTE) && ((SmBusAddress & BIT0) == B_PCH_SMBUS_WRITE)) {
    //
    // Set Host Command Register.
    //
    IoWrite8 (IoPortBaseAddress + R_PCH_SMBUS_HCMD, (UINT8)Value);
  } else {
    //
    // Set Host Command Register.
    //
    IoWrite8 (IoPortBaseAddress + R_PCH_SMBUS_HCMD, (UINT8) SMBUS_LIB_COMMAND (SmBusAddress));
    //
    // Write value to Host Data 0 and Host Data 1 Registers.
    //
    IoWrite8 (IoPortBaseAddress + R_PCH_SMBUS_HD0, (UINT8) Value);
    IoWrite8 (IoPortBaseAddress + R_PCH_SMBUS_HD1, (UINT8) (Value >> 8));
  }
  //
  // Set Auxiliary Control Regiester.
  //
  IoWrite8 (IoPortBaseAddress + R_PCH_SMBUS_AUXC, AuxiliaryControl);
  //
  // Set SMBUS slave address for the device to send/receive from.
  //
  IoWrite8 (IoPortBaseAddress + R_PCH_SMBUS_TSA, (UINT8) SmBusAddress);
  //
  // Start the SMBUS transaction and wait for the end.
  //
  ReturnStatus = InternalSmBusStart (IoPortBaseAddress, HostControl);
  //
  // Read value from Host Data 0 and Host Data 1 Registers.
  //
  Value = (UINT16)(IoRead8 (IoPortBaseAddress + R_PCH_SMBUS_HD1) << 8);
  Value = (UINT16)(Value | IoRead8 (IoPortBaseAddress + R_PCH_SMBUS_HD0));
  //
  // Clear Host Status Register and Auxiliary Status Register.
  //
  IoWrite8 (IoPortBaseAddress + R_PCH_SMBUS_HSTS, B_PCH_SMBUS_HSTS_ALL);
  IoWrite8 (IoPortBaseAddress + R_PCH_SMBUS_AUXS, B_PCH_SMBUS_CRCE);

Done:
  if (Status != NULL) {
    *Status = ReturnStatus;
  }

  return Value;
}

/**
  Executes an SMBUS quick read command.

  Executes an SMBUS quick read command on the SMBUS device specified by SmBusAddress.
  Only the SMBUS slave address field of SmBusAddress is required.
  If Status is not NULL, then the status of the executed command is returned in Status.
  If PEC is set in SmBusAddress, then ASSERT().
  If Command in SmBusAddress is not zero, then ASSERT().
  If Length in SmBusAddress is not zero, then ASSERT().
  If any reserved bits of SmBusAddress are set, then ASSERT().

  @param[in]  SmBusAddress  Address that encodes the SMBUS Slave Address,
                            SMBUS Command, SMBUS Data Length, and PEC.
  @param[out] Status        Return status for the executed command.
                            This is an optional parameter and may be NULL.

**/
VOID
EFIAPI
SmBusQuickRead (
  IN  UINTN                     SmBusAddress,
  OUT RETURN_STATUS             *Status       OPTIONAL
  )
{
  ASSERT (!SMBUS_LIB_PEC (SmBusAddress));
  ASSERT (SMBUS_LIB_COMMAND (SmBusAddress)  == 0);
  ASSERT (SMBUS_LIB_LENGTH (SmBusAddress)   == 0);
  ASSERT (SMBUS_LIB_RESERVED (SmBusAddress) == 0);

  if (SMBUS_LIB_PEC (SmBusAddress)           ||
    (SMBUS_LIB_COMMAND (SmBusAddress)  != 0) ||
    (SMBUS_LIB_LENGTH (SmBusAddress)   != 0) ||
    (SMBUS_LIB_RESERVED (SmBusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return;
  }

  InternalSmBusNonBlock (
    V_PCH_SMBUS_SMB_CMD_QUICK,
    SmBusAddress | B_PCH_SMBUS_READ,
    0,
    Status
    );
}

/**
  Executes an SMBUS quick write command.

  Executes an SMBUS quick write command on the SMBUS device specified by SmBusAddress.
  Only the SMBUS slave address field of SmBusAddress is required.
  If Status is not NULL, then the status of the executed command is returned in Status.
  If PEC is set in SmBusAddress, then ASSERT().
  If Command in SmBusAddress is not zero, then ASSERT().
  If Length in SmBusAddress is not zero, then ASSERT().
  If any reserved bits of SmBusAddress are set, then ASSERT().

  @param[in]  SmBusAddress  Address that encodes the SMBUS Slave Address,
                            SMBUS Command, SMBUS Data Length, and PEC.
  @param[out] Status        Return status for the executed command.
                            This is an optional parameter and may be NULL.

**/
VOID
EFIAPI
SmBusQuickWrite (
  IN  UINTN                     SmBusAddress,
  OUT RETURN_STATUS             *Status       OPTIONAL
  )
{
  ASSERT (!SMBUS_LIB_PEC (SmBusAddress));
  ASSERT (SMBUS_LIB_COMMAND (SmBusAddress)  == 0);
  ASSERT (SMBUS_LIB_LENGTH (SmBusAddress)   == 0);
  ASSERT (SMBUS_LIB_RESERVED (SmBusAddress) == 0);

  if (SMBUS_LIB_PEC (SmBusAddress)           ||
    (SMBUS_LIB_COMMAND (SmBusAddress)  != 0) ||
    (SMBUS_LIB_LENGTH (SmBusAddress)   != 0) ||
    (SMBUS_LIB_RESERVED (SmBusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return;
  }

  InternalSmBusNonBlock (
    V_PCH_SMBUS_SMB_CMD_QUICK,
    SmBusAddress | B_PCH_SMBUS_WRITE,
    0,
    Status
    );
}

/**
  Executes an SMBUS receive byte command.

  Executes an SMBUS receive byte command on the SMBUS device specified by SmBusAddress.
  Only the SMBUS slave address field of SmBusAddress is required.
  The byte received from the SMBUS is returned.
  If Status is not NULL, then the status of the executed command is returned in Status.
  If Command in SmBusAddress is not zero, then ASSERT().
  If Length in SmBusAddress is not zero, then ASSERT().
  If any reserved bits of SmBusAddress are set, then ASSERT().

  @param[in]  SmBusAddress  Address that encodes the SMBUS Slave Address,
                            SMBUS Command, SMBUS Data Length, and PEC.
  @param[out] Status        Return status for the executed command.
                            This is an optional parameter and may be NULL.

  @retval The byte received from the SMBUS.

**/
UINT8
EFIAPI
SmBusReceiveByte (
  IN  UINTN          SmBusAddress,
  OUT RETURN_STATUS  *Status        OPTIONAL
  )
{
  ASSERT (SMBUS_LIB_COMMAND (SmBusAddress)  == 0);
  ASSERT (SMBUS_LIB_LENGTH (SmBusAddress)   == 0);
  ASSERT (SMBUS_LIB_RESERVED (SmBusAddress) == 0);

  if ((SMBUS_LIB_COMMAND (SmBusAddress)  != 0) ||
      (SMBUS_LIB_LENGTH (SmBusAddress)   != 0) ||
      (SMBUS_LIB_RESERVED (SmBusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return 0;
  }

  return (UINT8) InternalSmBusNonBlock (
                   V_PCH_SMBUS_SMB_CMD_BYTE,
                   SmBusAddress | B_PCH_SMBUS_READ,
                   0,
                   Status
                   );
}

/**
  Executes an SMBUS send byte command.

  Executes an SMBUS send byte command on the SMBUS device specified by SmBusAddress.
  The byte specified by Value is sent.
  Only the SMBUS slave address field of SmBusAddress is required.  Value is returned.
  If Status is not NULL, then the status of the executed command is returned in Status.
  If Command in SmBusAddress is not zero, then ASSERT().
  If Length in SmBusAddress is not zero, then ASSERT().
  If any reserved bits of SmBusAddress are set, then ASSERT().

  @param[in]  SmBusAddress  Address that encodes the SMBUS Slave Address,
                            SMBUS Command, SMBUS Data Length, and PEC.
  @param[in]  Value         The 8-bit value to send.
  @param[out] Status        Return status for the executed command.
                            This is an optional parameter and may be NULL.

  @retval The parameter of Value.

**/
UINT8
EFIAPI
SmBusSendByte (
  IN  UINTN          SmBusAddress,
  IN  UINT8          Value,
  OUT RETURN_STATUS  *Status        OPTIONAL
  )
{
  ASSERT (SMBUS_LIB_COMMAND (SmBusAddress)  == 0);
  ASSERT (SMBUS_LIB_LENGTH (SmBusAddress)   == 0);
  ASSERT (SMBUS_LIB_RESERVED (SmBusAddress) == 0);

  if ((SMBUS_LIB_COMMAND (SmBusAddress)  != 0) ||
      (SMBUS_LIB_LENGTH (SmBusAddress)   != 0) ||
      (SMBUS_LIB_RESERVED (SmBusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return 0;
  }

  return (UINT8) InternalSmBusNonBlock (
                   V_PCH_SMBUS_SMB_CMD_BYTE,
                   SmBusAddress | B_PCH_SMBUS_WRITE,
                   Value,
                   Status
                   );
}

/**
  Executes an SMBUS read data byte command.

  Executes an SMBUS read data byte command on the SMBUS device specified by SmBusAddress.
  Only the SMBUS slave address and SMBUS command fields of SmBusAddress are required.
  The 8-bit value read from the SMBUS is returned.
  If Status is not NULL, then the status of the executed command is returned in Status.
  If Length in SmBusAddress is not zero, then ASSERT().
  If any reserved bits of SmBusAddress are set, then ASSERT().

  @param[in]  SmBusAddress    Address that encodes the SMBUS Slave Address,
                              SMBUS Command, SMBUS Data Length, and PEC.
  @param[out] Status          Return status for the executed command.
                              This is an optional parameter and may be NULL.

  @retval The byte read from the SMBUS.

**/
UINT8
EFIAPI
SmBusReadDataByte (
  IN  UINTN          SmBusAddress,
  OUT RETURN_STATUS  *Status        OPTIONAL
  )
{
  ASSERT (SMBUS_LIB_LENGTH (SmBusAddress)   == 0);
  ASSERT (SMBUS_LIB_RESERVED (SmBusAddress) == 0);

  if ((SMBUS_LIB_LENGTH (SmBusAddress)   != 0) ||
      (SMBUS_LIB_RESERVED (SmBusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return 0;
  }

  return (UINT8) InternalSmBusNonBlock (
                   V_PCH_SMBUS_SMB_CMD_BYTE_DATA,
                   SmBusAddress | B_PCH_SMBUS_READ,
                   0,
                   Status
                   );
}

/**
  Executes an SMBUS write data byte command.

  Executes an SMBUS write data byte command on the SMBUS device specified by SmBusAddress.
  The 8-bit value specified by Value is written.
  Only the SMBUS slave address and SMBUS command fields of SmBusAddress are required.
  Value is returned.
  If Status is not NULL, then the status of the executed command is returned in Status.
  If Length in SmBusAddress is not zero, then ASSERT().
  If any reserved bits of SmBusAddress are set, then ASSERT().

  @param[in]  SmBusAddress  Address that encodes the SMBUS Slave Address,
                            SMBUS Command, SMBUS Data Length, and PEC.
  @param[in]  Value         The 8-bit value to write.
  @param[out] Status        Return status for the executed command.
                            This is an optional parameter and may be NULL.

  @retval The parameter of Value.

**/
UINT8
EFIAPI
SmBusWriteDataByte (
  IN  UINTN          SmBusAddress,
  IN  UINT8          Value,
  OUT RETURN_STATUS  *Status        OPTIONAL
  )
{
  ASSERT (SMBUS_LIB_LENGTH (SmBusAddress)   == 0);
  ASSERT (SMBUS_LIB_RESERVED (SmBusAddress) == 0);

  if ((SMBUS_LIB_LENGTH (SmBusAddress)   != 0) ||
      (SMBUS_LIB_RESERVED (SmBusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return 0;
  }

  return (UINT8) InternalSmBusNonBlock (
                   V_PCH_SMBUS_SMB_CMD_BYTE_DATA,
                   SmBusAddress | B_PCH_SMBUS_WRITE,
                   Value,
                   Status
                   );
}

/**
  Executes an SMBUS read data word command.

  Executes an SMBUS read data word command on the SMBUS device specified by SmBusAddress.
  Only the SMBUS slave address and SMBUS command fields of SmBusAddress are required.
  The 16-bit value read from the SMBUS is returned.
  If Status is not NULL, then the status of the executed command is returned in Status.
  If Length in SmBusAddress is not zero, then ASSERT().
  If any reserved bits of SmBusAddress are set, then ASSERT().

  @param[in]  SmBusAddress  Address that encodes the SMBUS Slave Address,
                            SMBUS Command, SMBUS Data Length, and PEC.
  @param[out] Status        Return status for the executed command.
                            This is an optional parameter and may be NULL.

  @retval The byte read from the SMBUS.

**/
UINT16
EFIAPI
SmBusReadDataWord (
  IN  UINTN          SmBusAddress,
  OUT RETURN_STATUS  *Status        OPTIONAL
  )
{
  ASSERT (SMBUS_LIB_LENGTH (SmBusAddress)   == 0);
  ASSERT (SMBUS_LIB_RESERVED (SmBusAddress) == 0);

  if ((SMBUS_LIB_LENGTH (SmBusAddress)   != 0) ||
      (SMBUS_LIB_RESERVED (SmBusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return 0;
  }

  return InternalSmBusNonBlock (
           V_PCH_SMBUS_SMB_CMD_WORD_DATA,
           SmBusAddress | B_PCH_SMBUS_READ,
           0,
           Status
           );
}

/**
  Executes an SMBUS write data word command.

  Executes an SMBUS write data word command on the SMBUS device specified by SmBusAddress.
  The 16-bit value specified by Value is written.
  Only the SMBUS slave address and SMBUS command fields of SmBusAddress are required.
  Value is returned.
  If Status is not NULL, then the status of the executed command is returned in Status.
  If Length in SmBusAddress is not zero, then ASSERT().
  If any reserved bits of SmBusAddress are set, then ASSERT().

  @param[in]  SmBusAddress  Address that encodes the SMBUS Slave Address,
                            SMBUS Command, SMBUS Data Length, and PEC.
  @param[in]  Value         The 16-bit value to write.
  @param[out] Status        Return status for the executed command.
                            This is an optional parameter and may be NULL.

  @retval The parameter of Value.

**/
UINT16
EFIAPI
SmBusWriteDataWord (
  IN  UINTN          SmBusAddress,
  IN  UINT16         Value,
  OUT RETURN_STATUS  *Status        OPTIONAL
  )
{
  ASSERT (SMBUS_LIB_LENGTH (SmBusAddress)   == 0);
  ASSERT (SMBUS_LIB_RESERVED (SmBusAddress) == 0);

  if ((SMBUS_LIB_LENGTH (SmBusAddress)   != 0) ||
      (SMBUS_LIB_RESERVED (SmBusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return 0;
  }

  return InternalSmBusNonBlock (
           V_PCH_SMBUS_SMB_CMD_WORD_DATA,
           SmBusAddress | B_PCH_SMBUS_WRITE,
           Value,
           Status
           );
}

/**
  Executes an SMBUS process call command.

  Executes an SMBUS process call command on the SMBUS device specified by SmBusAddress.
  The 16-bit value specified by Value is written.
  Only the SMBUS slave address and SMBUS command fields of SmBusAddress are required.
  The 16-bit value returned by the process call command is returned.
  If Status is not NULL, then the status of the executed command is returned in Status.
  If Length in SmBusAddress is not zero, then ASSERT().
  If any reserved bits of SmBusAddress are set, then ASSERT().

  @param[in]  SmBusAddress  Address that encodes the SMBUS Slave Address,
                            SMBUS Command, SMBUS Data Length, and PEC.
  @param[in]  Value         The 16-bit value to write.
  @param[out] Status        Return status for the executed command.
                            This is an optional parameter and may be NULL.

  @retval The 16-bit value returned by the process call command.

**/
UINT16
EFIAPI
SmBusProcessCall (
  IN  UINTN          SmBusAddress,
  IN  UINT16         Value,
  OUT RETURN_STATUS  *Status        OPTIONAL
  )
{
  ASSERT (SMBUS_LIB_LENGTH (SmBusAddress)   == 0);
  ASSERT (SMBUS_LIB_RESERVED (SmBusAddress) == 0);

  if ((SMBUS_LIB_LENGTH (SmBusAddress)   != 0) ||
      (SMBUS_LIB_RESERVED (SmBusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return 0;
  }

  return InternalSmBusNonBlock (
           V_PCH_SMBUS_SMB_CMD_PROCESS_CALL,
           SmBusAddress | B_PCH_SMBUS_WRITE,
           Value,
           Status
           );
}

/**
  Executes an SMBUS block command.

  Executes an SMBUS block read, block write and block write-block read command
  on the SMBUS device specified by SmBusAddress.
  Bytes are read from the SMBUS and stored in Buffer.
  The number of bytes read is returned, and will never return a value larger than 32-bytes.
  If Status is not NULL, then the status of the executed command is returned in Status.
  It is the caller's responsibility to make sure Buffer is large enough for the total number of bytes read.
  SMBUS supports a maximum transfer size of 32 bytes, so Buffer does not need to be any larger than 32 bytes.

  @param[in]  HostControl     The value of Host Control Register to set.
  @param[in]  SmBusAddress    Address that encodes the SMBUS Slave Address,
                              SMBUS Command, SMBUS Data Length, and PEC.
  @param[in]  WriteBuffer     Pointer to the buffer of bytes to write to the SMBUS.
  @param[out] ReadBuffer      Pointer to the buffer of bytes to read from the SMBUS.
  @param[out] Status          Return status for the executed command.
                              This is an optional parameter and may be NULL.

  @retval The number of bytes read from the SMBUS.

**/
UINTN
InternalSmBusBlock (
  IN  UINT8                     HostControl,
  IN  UINTN                     SmBusAddress,
  IN  UINT8                     *WriteBuffer,
  OUT UINT8                     *ReadBuffer,
  OUT RETURN_STATUS             *Status
  )
{
  RETURN_STATUS                 ReturnStatus;
  UINTN                         Index;
  UINTN                         BytesCount;
  UINT16                        IoPortBaseAddress;
  UINT8                         AuxiliaryControl;

  IoPortBaseAddress = InternalGetSmbusIoPortBaseAddress ();

  BytesCount = SMBUS_LIB_LENGTH (SmBusAddress);

  //
  // Try to acquire the ownership of SMBUS.
  //
  ReturnStatus = InternalSmBusAcquire (IoPortBaseAddress);
  if (RETURN_ERROR (ReturnStatus)) {
    goto Done;
  }
  //
  // Set the appropriate Host Control Register and auxiliary Control Register.
  //
  AuxiliaryControl = B_PCH_SMBUS_E32B;
  if (SMBUS_LIB_PEC (SmBusAddress)) {
    AuxiliaryControl |= B_PCH_SMBUS_AAC;
    HostControl      |= B_PCH_SMBUS_PEC_EN;
  }
  //
  // Set Host Command Register.
  //
  IoWrite8 (IoPortBaseAddress + R_PCH_SMBUS_HCMD, (UINT8) SMBUS_LIB_COMMAND (SmBusAddress));
  //
  // Set Auxiliary Control Regiester.
  //
  IoWrite8 (IoPortBaseAddress + R_PCH_SMBUS_AUXC, AuxiliaryControl);
  //
  // Clear byte pointer of 32-byte buffer.
  //
  IoRead8 (IoPortBaseAddress + R_PCH_SMBUS_HCTL);

  if (WriteBuffer != NULL) {
    //
    // Write the number of block to Host Block Data Byte Register.
    //
    IoWrite8 (IoPortBaseAddress + R_PCH_SMBUS_HD0, (UINT8) BytesCount);
    //
    // Write data block to Host Block Data Register.
    //
    for (Index = 0; Index < BytesCount; Index++) {
      IoWrite8 (IoPortBaseAddress + R_PCH_SMBUS_HBD, WriteBuffer[Index]);
    }
  }
  //
  // Set SMBUS slave address for the device to send/receive from.
  //
  IoWrite8 (IoPortBaseAddress + R_PCH_SMBUS_TSA, (UINT8) SmBusAddress);
  //
  // Start the SMBUS transaction and wait for the end.
  //
  ReturnStatus = InternalSmBusStart (IoPortBaseAddress, HostControl);
  if (RETURN_ERROR (ReturnStatus)) {
    goto Done;
  }

  if (ReadBuffer != NULL) {
    //
    // Read the number of block from host block data byte register.
    //
    BytesCount = IoRead8 (IoPortBaseAddress + R_PCH_SMBUS_HD0);
    //
    // Write data block from Host Block Data Register.
    //
    for (Index = 0; Index < BytesCount; Index++) {
      ReadBuffer[Index] = IoRead8 (IoPortBaseAddress + R_PCH_SMBUS_HBD);
    }
  }

Done:
  //
  // Clear Host Status Register and Auxiliary Status Register.
  //
  IoWrite8 (IoPortBaseAddress + R_PCH_SMBUS_HSTS, B_PCH_SMBUS_HSTS_ALL);
  IoWrite8 (IoPortBaseAddress + R_PCH_SMBUS_AUXS, B_PCH_SMBUS_CRCE);

  if (Status != NULL) {
    *Status = ReturnStatus;
  }

  return BytesCount;
}

/**
  Executes an SMBUS read block command.

  Executes an SMBUS read block command on the SMBUS device specified by SmBusAddress.
  Only the SMBUS slave address and SMBUS command fields of SmBusAddress are required.
  Bytes are read from the SMBUS and stored in Buffer.
  The number of bytes read is returned, and will never return a value larger than 32-bytes.
  If Status is not NULL, then the status of the executed command is returned in Status.
  It is the caller's responsibility to make sure Buffer is large enough for the total number of bytes read.
  SMBUS supports a maximum transfer size of 32 bytes, so Buffer does not need to be any larger than 32 bytes.
  If Length in SmBusAddress is not zero, then ASSERT().
  If Buffer is NULL, then ASSERT().
  If any reserved bits of SmBusAddress are set, then ASSERT().

  @param[in]  SmBusAddress  Address that encodes the SMBUS Slave Address,
                            SMBUS Command, SMBUS Data Length, and PEC.
  @param[out] Buffer        Pointer to the buffer to store the bytes read from the SMBUS.
  @param[out] Status        Return status for the executed command.
                            This is an optional parameter and may be NULL.

  @retval The number of bytes read.

**/
UINTN
EFIAPI
SmBusReadBlock (
  IN  UINTN          SmBusAddress,
  OUT VOID           *Buffer,
  OUT RETURN_STATUS  *Status        OPTIONAL
  )
{
  ASSERT (Buffer != NULL);
  ASSERT (SMBUS_LIB_LENGTH (SmBusAddress)   == 0);
  ASSERT (SMBUS_LIB_RESERVED (SmBusAddress) == 0);

  if ((Buffer == NULL)                         ||
      (SMBUS_LIB_LENGTH (SmBusAddress)   != 0) ||
      (SMBUS_LIB_RESERVED (SmBusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return 0;
  }

  return InternalSmBusBlock (
           V_PCH_SMBUS_SMB_CMD_BLOCK,
           SmBusAddress | B_PCH_SMBUS_READ,
           NULL,
           Buffer,
           Status
           );
}

/**
  Executes an SMBUS write block command.

  Executes an SMBUS write block command on the SMBUS device specified by SmBusAddress.
  The SMBUS slave address, SMBUS command, and SMBUS length fields of SmBusAddress are required.
  Bytes are written to the SMBUS from Buffer.
  The number of bytes written is returned, and will never return a value larger than 32-bytes.
  If Status is not NULL, then the status of the executed command is returned in Status.
  If Length in SmBusAddress is zero or greater than 32, then ASSERT().
  If Buffer is NULL, then ASSERT().
  If any reserved bits of SmBusAddress are set, then ASSERT().

  @param[in]  SmBusAddress  Address that encodes the SMBUS Slave Address,
                            SMBUS Command, SMBUS Data Length, and PEC.
  @param[out] Buffer        Pointer to the buffer to store the bytes read from the SMBUS.
  @param[out] Status        Return status for the executed command.
                            This is an optional parameter and may be NULL.

  @retval The number of bytes written.

**/
UINTN
EFIAPI
SmBusWriteBlock (
  IN  UINTN          SmBusAddress,
  OUT VOID           *Buffer,
  OUT RETURN_STATUS  *Status        OPTIONAL
  )
{
  ASSERT (Buffer != NULL);
  ASSERT (SMBUS_LIB_LENGTH (SmBusAddress)   >= 1);
  ASSERT (SMBUS_LIB_LENGTH (SmBusAddress)   <= 32);
  ASSERT (SMBUS_LIB_RESERVED (SmBusAddress) == 0);

  if ((Buffer == NULL)                         ||
      (SMBUS_LIB_LENGTH (SmBusAddress)   == 0) ||
      (SMBUS_LIB_LENGTH (SmBusAddress)   > 32) ||
      (SMBUS_LIB_RESERVED (SmBusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return 0;
  }

  return InternalSmBusBlock (
           V_PCH_SMBUS_SMB_CMD_BLOCK,
           SmBusAddress | B_PCH_SMBUS_WRITE,
           Buffer,
           NULL,
           Status
           );
}

/**
  Executes an SMBUS block process call command.

  Executes an SMBUS block process call command on the SMBUS device specified by SmBusAddress.
  The SMBUS slave address, SMBUS command, and SMBUS length fields of SmBusAddress are required.
  Bytes are written to the SMBUS from WriteBuffer.  Bytes are then read from the SMBUS into ReadBuffer.
  If Status is not NULL, then the status of the executed command is returned in Status.
  It is the caller's responsibility to make sure ReadBuffer is large enough for the total number of bytes read.
  SMBUS supports a maximum transfer size of 32 bytes, so Buffer does not need to be any larger than 32 bytes.
  If Length in SmBusAddress is zero or greater than 32, then ASSERT().
  If WriteBuffer is NULL, then ASSERT().
  If ReadBuffer is NULL, then ASSERT().
  If any reserved bits of SmBusAddress are set, then ASSERT().

  @param[in]  SmBusAddress  Address that encodes the SMBUS Slave Address,
                            SMBUS Command, SMBUS Data Length, and PEC.
  @param[in]  WriteBuffer   Pointer to the buffer of bytes to write to the SMBUS.
  @param[out] ReadBuffer    Pointer to the buffer of bytes to read from the SMBUS.
  @param[out] Status        Return status for the executed command.
                            This is an optional parameter and may be NULL.

  @retval The number of bytes written.

**/
UINTN
EFIAPI
SmBusBlockProcessCall (
  IN  UINTN          SmBusAddress,
  IN  VOID           *WriteBuffer,
  OUT VOID           *ReadBuffer,
  OUT RETURN_STATUS  *Status        OPTIONAL
  )
{
  ASSERT (WriteBuffer != NULL);
  ASSERT (ReadBuffer  != NULL);
  ASSERT (SMBUS_LIB_LENGTH (SmBusAddress)   >= 1);
  ASSERT (SMBUS_LIB_LENGTH (SmBusAddress)   <= 32);
  ASSERT (SMBUS_LIB_RESERVED (SmBusAddress) == 0);

  if ((WriteBuffer == NULL)                    ||
      (ReadBuffer  == NULL)                    ||
      (SMBUS_LIB_LENGTH (SmBusAddress)   == 0) ||
      (SMBUS_LIB_LENGTH (SmBusAddress)   > 32) ||
      (SMBUS_LIB_RESERVED (SmBusAddress) != 0))
  {
    if (Status != NULL) {
      *Status = RETURN_INVALID_PARAMETER;
    }
    return 0;
  }

  return InternalSmBusBlock (
           V_PCH_SMBUS_SMB_CMD_BLOCK_PROCESS,
           SmBusAddress | B_PCH_SMBUS_WRITE,
           WriteBuffer,
           ReadBuffer,
           Status
           );
}

/**
  The library constructuor.

  The function does the necessary initialization work for this library instance.

  @retval     EFI_SUCCESS       The function always return EFI_SUCCESS for now.
**/
RETURN_STATUS
EFIAPI
BaseSmbusLibConstructor (
  VOID
  )
{
  UINTN     SmbusPciBase;
  UINT16    IoPortBaseAddress;

  //
  // Init mSmbusIoBase variable.
  //
  SmbusPciBase = MmPciBase (
                   DEFAULT_PCI_BUS_NUMBER_PCH,
                   PCI_DEVICE_NUMBER_PCH_SMBUS,
                   PCI_FUNCTION_NUMBER_PCH_SMBUS
                   );
  IoPortBaseAddress = (UINT16) MmioRead32 (SmbusPciBase + R_PCH_SMBUS_BASE);

  if ((IoPortBaseAddress != 0) && (IoPortBaseAddress != 0xFFFF)) {
    mSmbusIoBase = IoPortBaseAddress & B_PCH_SMBUS_BASE_BAR;
  }

  return RETURN_SUCCESS;
}
