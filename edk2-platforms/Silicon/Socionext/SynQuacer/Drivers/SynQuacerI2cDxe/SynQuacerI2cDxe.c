/** @file

  Copyright (c) 2017, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SynQuacerI2cDxe.h"

//
// We cannot use Stall () or timer events at runtime, so we need to busy-wait
// for the controller to signal the completion interrupts. This value was
// arbitrarily chosen, and does not appear to produce any premature timeouts
// nor does it result in noticeable stalls in case of bus errors.
//
#define WAIT_FOR_INTERRUPT_TIMEOUT    50000

/**
  Set the frequency for the I2C clock line.

  This routine must be called at or below TPL_NOTIFY.

  The software and controller do a best case effort of using the specified
  frequency for the I2C bus.  If the frequency does not match exactly then
  the I2C master protocol selects the next lower frequency to avoid
  exceeding the operating conditions for any of the I2C devices on the bus.
  For example if 400 KHz was specified and the controller's divide network
  only supports 402 KHz or 398 KHz then the I2C master protocol selects 398
  KHz.  If there are not lower frequencies available, then return
  EFI_UNSUPPORTED.

  @param[in] This           Pointer to an EFI_I2C_MASTER_PROTOCOL structure
  @param[in] BusClockHertz  Pointer to the requested I2C bus clock frequency
                            in Hertz.  Upon return this value contains the
                            actual frequency in use by the I2C controller.

  @retval EFI_SUCCESS           The bus frequency was set successfully.
  @retval EFI_ALREADY_STARTED   The controller is busy with another transaction.
  @retval EFI_INVALID_PARAMETER BusClockHertz is NULL
  @retval EFI_UNSUPPORTED       The controller does not support this frequency.

**/
STATIC
EFI_STATUS
EFIAPI
SynQuacerI2cSetBusFrequency (
  IN CONST EFI_I2C_MASTER_PROTOCOL   *This,
  IN OUT UINTN                       *BusClockHertz
  )
{
  SYNQUACER_I2C_MASTER        *I2c;
  UINT8                       Ccr, Csr;

  I2c = SYNQUACER_I2C_FROM_THIS (This);

  if (BusClockHertz == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (*BusClockHertz >= F_I2C_SPEED_FM) {
    if (REFCLK_RATE <= F_I2C_CLK_RATE_18M) {
      Ccr = F_I2C_CCR_CS_FAST_MAX_18M (REFCLK_RATE);
      Csr = F_I2C_CSR_CS_FAST_MAX_18M (REFCLK_RATE);
    } else {
      Ccr = F_I2C_CCR_CS_FAST_MIN_18M (REFCLK_RATE);
      Csr = F_I2C_CSR_CS_FAST_MIN_18M (REFCLK_RATE);
    }

    // Set Clock and enable, Set fast mode
    MmioWrite8 (I2c->MmioBase + F_I2C_REG_CCR,
      Ccr | F_I2C_CCR_FM | F_I2C_CCR_EN);
    MmioWrite8 (I2c->MmioBase + F_I2C_REG_CSR, Csr);

    *BusClockHertz = F_I2C_SPEED_FM;

  } else if (*BusClockHertz >= F_I2C_SPEED_SM) {
    if (REFCLK_RATE <= F_I2C_CLK_RATE_18M) {
      Ccr = F_I2C_CCR_CS_STANDARD_MAX_18M (REFCLK_RATE);
      Csr = F_I2C_CSR_CS_STANDARD_MAX_18M (REFCLK_RATE);
    } else {
      Ccr = F_I2C_CCR_CS_STANDARD_MIN_18M (REFCLK_RATE);
      Csr = F_I2C_CSR_CS_STANDARD_MIN_18M (REFCLK_RATE);
    }

    // Set Clock and enable, Set standard mode
    MmioWrite8 (I2c->MmioBase + F_I2C_REG_CCR, Ccr | F_I2C_CCR_EN);
    MmioWrite8 (I2c->MmioBase + F_I2C_REG_CSR, Csr);

    *BusClockHertz = F_I2C_SPEED_SM;
  } else {
    return EFI_UNSUPPORTED;
  }

  MemoryFence ();

  return EFI_SUCCESS;
}

/**
  Reset the I2C controller and configure it for use

  This routine must be called at or below TPL_NOTIFY.

  The I2C controller is reset.  The caller must call SetBusFrequench() after
  calling Reset().

  @param[in]     This       Pointer to an EFI_I2C_MASTER_PROTOCOL structure.

  @retval EFI_SUCCESS         The reset completed successfully.
  @retval EFI_ALREADY_STARTED The controller is busy with another transaction.
  @retval EFI_DEVICE_ERROR    The reset operation failed.

**/
STATIC
EFI_STATUS
EFIAPI
SynQuacerI2cReset (
  IN CONST EFI_I2C_MASTER_PROTOCOL *This
  )
{
  SYNQUACER_I2C_MASTER        *I2c;

  I2c = SYNQUACER_I2C_FROM_THIS (This);

  // Disable the clock
  MmioWrite8 (I2c->MmioBase + F_I2C_REG_CCR, 0);
  MmioWrite8 (I2c->MmioBase + F_I2C_REG_CSR, 0);

  MemoryFence ();

  // Set own Address
  MmioWrite8 (I2c->MmioBase + F_I2C_REG_ADR, 0);

  // Set PCLK frequency
  MmioWrite8 (I2c->MmioBase + F_I2C_REG_FSR, F_I2C_BUS_CLK_FR (REFCLK_RATE));

  // clear IRQ (INT=0, BER=0), Interrupt Disable
  MmioWrite8 (I2c->MmioBase + F_I2C_REG_BCR, 0);
  MmioWrite8 (I2c->MmioBase + F_I2C_REG_BC2R, 0);

  MemoryFence ();

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
SynQuacerI2cMasterStart (
  IN  SYNQUACER_I2C_MASTER        *I2c,
  IN  UINTN                       SlaveAddress,
  IN  EFI_I2C_OPERATION           *Op
  )
{
  UINT8                       Bsr;
  UINT8                       Bcr;

  if (Op->Flags & I2C_FLAG_READ) {
    MmioWrite8 (I2c->MmioBase + F_I2C_REG_DAR, (SlaveAddress << 1) | 1);
  } else {
    MmioWrite8 (I2c->MmioBase + F_I2C_REG_DAR, SlaveAddress << 1);
  }

  DEBUG ((DEBUG_INFO, "%a: slave:0x%02x\n", __FUNCTION__,
    SlaveAddress));

  Bsr = MmioRead8 (I2c->MmioBase + F_I2C_REG_BSR);
  Bcr = MmioRead8 (I2c->MmioBase + F_I2C_REG_BCR);

  if ((Bsr & F_I2C_BSR_BB) && !(Bcr & F_I2C_BCR_MSS)) {
    DEBUG ((DEBUG_INFO, "%a: bus is busy\n", __FUNCTION__));
    return EFI_ALREADY_STARTED;
  }

  if (Bsr & F_I2C_BSR_BB) { // Bus is busy
    DEBUG ((DEBUG_INFO, "%a: Continuous Start\n", __FUNCTION__));
    MmioWrite8 (I2c->MmioBase + F_I2C_REG_BCR, Bcr | F_I2C_BCR_SCC);
  } else {
    if (Bcr & F_I2C_BCR_MSS) {
      DEBUG ((DEBUG_WARN,
        "%a: is not in master mode\n", __FUNCTION__));
      return EFI_DEVICE_ERROR;
    }
    DEBUG ((DEBUG_INFO, "%a: Start Condition\n", __FUNCTION__));
    MmioWrite8 (I2c->MmioBase + F_I2C_REG_BCR,
                Bcr | F_I2C_BCR_MSS | F_I2C_BCR_INTE | F_I2C_BCR_BEIE);
  }
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
WaitForInterrupt (
  IN  SYNQUACER_I2C_MASTER        *I2c
  )
{
  UINT8       Bsr;
  UINTN       Timeout = WAIT_FOR_INTERRUPT_TIMEOUT;

  do {
    MemoryFence ();

    Bsr = MmioRead8 (I2c->MmioBase + F_I2C_REG_BCR);
    if (Bsr & F_I2C_BCR_INT) {
      return EFI_SUCCESS;
    }
  } while (Timeout--);

  return EFI_DEVICE_ERROR;
}

/**
  Start an I2C transaction on the host controller.

  This routine must be called at or below TPL_NOTIFY.  For synchronous
  requests this routine must be called at or below TPL_CALLBACK.

  This function initiates an I2C transaction on the controller.  To
  enable proper error handling by the I2C protocol stack, the I2C
  master protocol does not support queuing but instead only manages
  one I2C transaction at a time.  This API requires that the I2C bus
  is in the correct configuration for the I2C transaction.

  The transaction is performed by sending a start-bit and selecting the
  I2C device with the specified I2C slave address and then performing
  the specified I2C operations.  When multiple operations are requested
  they are separated with a repeated start bit and the slave address.
  The transaction is terminated with a stop bit.

  When Event is NULL, StartRequest operates synchronously and returns
  the I2C completion status as its return value.

  When Event is not NULL, StartRequest synchronously returns EFI_SUCCESS
  indicating that the I2C transaction was started asynchronously.  The
  transaction status value is returned in the buffer pointed to by
  I2cStatus upon the completion of the I2C transaction when I2cStatus
  is not NULL.  After the transaction status is returned the Event is
  signaled.

  Note: The typical consumer of this API is the I2C host protocol.
  Extreme care must be taken by other consumers of this API to prevent
  confusing the third party I2C drivers due to a state change at the
  I2C device which the third party I2C drivers did not initiate.  I2C
  platform specific code may use this API within these guidelines.

  @param[in] This           Pointer to an EFI_I2C_MASTER_PROTOCOL structure.
  @param[in] SlaveAddress   Address of the device on the I2C bus.  Set the
                            I2C_ADDRESSING_10_BIT when using 10-bit addresses,
                            clear this bit for 7-bit addressing.  Bits 0-6
                            are used for 7-bit I2C slave addresses and bits
                            0-9 are used for 10-bit I2C slave addresses.
  @param[in] RequestPacket  Pointer to an EFI_I2C_REQUEST_PACKET
                            structure describing the I2C transaction.
  @param[in] Event          Event to signal for asynchronous transactions,
                            NULL for synchronous transactions
  @param[out] I2cStatus     Optional buffer to receive the I2C transaction
                            completion status

  @retval EFI_SUCCESS           The asynchronous transaction was successfully
                                started when Event is not NULL.
  @retval EFI_SUCCESS           The transaction completed successfully when
                                Event is NULL.
  @retval EFI_ALREADY_STARTED   The controller is busy with another transaction.
  @retval EFI_BAD_BUFFER_SIZE   The RequestPacket->LengthInBytes value is too
                                large.
  @retval EFI_DEVICE_ERROR      There was an I2C error (NACK) during the
                                transaction.
  @retval EFI_INVALID_PARAMETER RequestPacket is NULL
  @retval EFI_NOT_FOUND         Reserved bit set in the SlaveAddress parameter
  @retval EFI_NO_RESPONSE       The I2C device is not responding to the slave
                                address.  EFI_DEVICE_ERROR will be returned if
                                the controller cannot distinguish when the NACK
                                occurred.
  @retval EFI_OUT_OF_RESOURCES  Insufficient memory for I2C transaction
  @retval EFI_UNSUPPORTED       The controller does not support the requested
                                transaction.

**/
STATIC
EFI_STATUS
EFIAPI
SynQuacerI2cStartRequest (
  IN  CONST EFI_I2C_MASTER_PROTOCOL   *This,
  IN  UINTN                           SlaveAddress,
  IN  EFI_I2C_REQUEST_PACKET          *RequestPacket,
  IN  EFI_EVENT                       Event      OPTIONAL,
  OUT EFI_STATUS                      *I2cStatus OPTIONAL
  )
{
  SYNQUACER_I2C_MASTER        *I2c;
  UINTN                       Idx;
  EFI_I2C_OPERATION           *Op;
  UINTN                       BufIdx;
  EFI_STATUS                  Status;
  EFI_TPL                     Tpl;
  BOOLEAN                     AtRuntime;
  UINT8                       Bsr;
  UINT8                       Bcr;

  I2c = SYNQUACER_I2C_FROM_THIS (This);

  //
  // We can only do synchronous operations at runtime
  //
  AtRuntime = EfiAtRuntime ();
  if (AtRuntime && Event != NULL) {
    return EFI_UNSUPPORTED;
  }

  if (!AtRuntime) {
    Tpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);
  }

  for (Idx = 0, Op = RequestPacket->Operation, Status = EFI_SUCCESS;
       Idx < RequestPacket->OperationCount && !EFI_ERROR (Status);
       Idx++, Op++) {

    Status = SynQuacerI2cMasterStart (I2c, SlaveAddress, Op);
    if (EFI_ERROR (Status)) {
      break;
    }

    Status = WaitForInterrupt (I2c);
    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_WARN, "%a: Timeout waiting for interrupt - %r\n",
          __FUNCTION__, Status));
      break;
    }

    if (MmioRead8 (I2c->MmioBase + F_I2C_REG_BSR) & F_I2C_BSR_LRB) {
      DEBUG ((DEBUG_WARN, "%a: No ack received\n", __FUNCTION__));
      Status = EFI_DEVICE_ERROR;
      break;
    }

    BufIdx = 0;
    do {
      Bsr = MmioRead8 (I2c->MmioBase + F_I2C_REG_BSR);
      Bcr = MmioRead8 (I2c->MmioBase + F_I2C_REG_BCR);

      if (Bcr & F_I2C_BCR_BER) {
        DEBUG ((DEBUG_WARN, "%a: Bus error detected\n", __FUNCTION__));
        Status = EFI_DEVICE_ERROR;
        break;
      }

      if ((Bsr & F_I2C_BSR_AL) || !(Bcr & F_I2C_BCR_MSS)) {
        DEBUG ((DEBUG_WARN, "%a: Arbitration lost\n", __FUNCTION__));
        Status = EFI_DEVICE_ERROR;
        break;
      }

      if (Op->Flags & I2C_FLAG_READ) {
        if (BufIdx == Op->LengthInBytes - 1) {
          MmioWrite8 (I2c->MmioBase + F_I2C_REG_BCR,
            F_I2C_BCR_MSS | F_I2C_BCR_INTE | F_I2C_BCR_BEIE);
        } else {
          MmioWrite8 (I2c->MmioBase + F_I2C_REG_BCR,
            F_I2C_BCR_MSS | F_I2C_BCR_INTE | F_I2C_BCR_BEIE | F_I2C_BCR_ACK);
        }

        Status = WaitForInterrupt (I2c);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_WARN,
            "%a: Timeout waiting for interrupt - %r\n", __FUNCTION__, Status));
          break;
        }

        if (!(MmioRead8 (I2c->MmioBase + F_I2C_REG_BSR) & F_I2C_BSR_FBT)) {
          Op->Buffer [BufIdx++] = MmioRead8 (I2c->MmioBase + F_I2C_REG_DAR);
        }
      } else {
        MmioWrite8 (I2c->MmioBase + F_I2C_REG_DAR, Op->Buffer [BufIdx++]);
        MmioWrite8 (I2c->MmioBase + F_I2C_REG_BCR,
          F_I2C_BCR_MSS | F_I2C_BCR_INTE | F_I2C_BCR_BEIE);

        Status = WaitForInterrupt (I2c);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_WARN,
            "%a: Timeout waiting for interrupt - %r\n", __FUNCTION__, Status));
          break;
        }

        if (MmioRead8 (I2c->MmioBase + F_I2C_REG_BSR) & F_I2C_BSR_LRB) {
          DEBUG ((DEBUG_WARN, "%a: No ack received\n", __FUNCTION__));
          Status = EFI_DEVICE_ERROR;
          break;
        }
      }
    } while (BufIdx < Op->LengthInBytes);
  }

  // Force bus state to idle, terminating any ongoing transfer
  MmioWrite8 (I2c->MmioBase + F_I2C_REG_BCR, 0);

  if (!AtRuntime) {
    gBS->RestoreTPL (Tpl);
  }

  if (Event) {
    *I2cStatus = Status;
    gBS->SignalEvent (Event);
  }
  return Status;
}

STATIC CONST EFI_I2C_CONTROLLER_CAPABILITIES mI2cControllerCapabilities = {
  sizeof (EFI_I2C_CONTROLLER_CAPABILITIES),   // StructureSizeInBytes
  MAX_UINT32,                                 // MaximumReceiveBytes
  MAX_UINT32,                                 // MaximumTransmitBytes
  MAX_UINT32,                                 // MaximumTotalBytes
};

STATIC
VOID
EFIAPI
SynQuacerI2cVirtualNotifyEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  SYNQUACER_I2C_MASTER    *I2c = Context;

  EfiConvertPointer (0x0, (VOID **)&I2c->I2cMaster.SetBusFrequency);
  EfiConvertPointer (0x0, (VOID **)&I2c->I2cMaster.Reset);
  EfiConvertPointer (0x0, (VOID **)&I2c->I2cMaster.StartRequest);
  EfiConvertPointer (0x0, (VOID **)&I2c->I2cMaster.I2cControllerCapabilities);
  EfiConvertPointer (0x0, (VOID **)&I2c->MmioBase);
}

EFI_STATUS
SynQuacerI2cInit (
  IN      EFI_HANDLE        DriverBindingHandle,
  IN      EFI_HANDLE        ControllerHandle
  )
{
  EFI_STATUS                        Status;
  NON_DISCOVERABLE_DEVICE           *Dev;
  SYNQUACER_I2C_MASTER              *I2c;
  BOOLEAN                           Runtime;

  Status = gBS->OpenProtocol (ControllerHandle,
                              &gEdkiiNonDiscoverableDeviceProtocolGuid,
                              (VOID **)&Dev, DriverBindingHandle,
                              ControllerHandle, EFI_OPEN_PROTOCOL_BY_DRIVER);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Runtime = CompareGuid (Dev->Type,
                         &gSynQuacerNonDiscoverableRuntimeI2cMasterGuid);

  // Allocate Resources
  if (Runtime) {
    I2c = AllocateRuntimeZeroPool (sizeof (SYNQUACER_I2C_MASTER));
  } else {
    I2c = AllocateZeroPool (sizeof (SYNQUACER_I2C_MASTER));
  }
  if (I2c == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto CloseProtocol;
  }

  I2c->Signature                            = SYNQUACER_I2C_SIGNATURE;
  I2c->I2cMaster.SetBusFrequency            = SynQuacerI2cSetBusFrequency;
  I2c->I2cMaster.Reset                      = SynQuacerI2cReset;
  I2c->I2cMaster.StartRequest               = SynQuacerI2cStartRequest;
  I2c->I2cMaster.I2cControllerCapabilities  = &mI2cControllerCapabilities;
  I2c->MmioBase                             = Dev->Resources[0].AddrRangeMin;
  I2c->Dev                                  = Dev;

  if (Runtime) {
    I2c->Runtime = TRUE;

    // Declare the controller as EFI_MEMORY_RUNTIME
    Status = gDS->AddMemorySpace (
                    EfiGcdMemoryTypeMemoryMappedIo,
                    Dev->Resources[0].AddrRangeMin,
                    Dev->Resources[0].AddrLen,
                    EFI_MEMORY_UC | EFI_MEMORY_RUNTIME);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "%a: failed to add memory space - %r\n",
        __FUNCTION__, Status));
    }

    Status = gDS->SetMemorySpaceAttributes (
                    Dev->Resources[0].AddrRangeMin,
                    Dev->Resources[0].AddrLen,
                    EFI_MEMORY_UC | EFI_MEMORY_RUNTIME);
    if (EFI_ERROR (Status)) {
      goto FreeDevice;
    }

    //
    // Register for the virtual address change event
    //
    Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_NOTIFY,
                    SynQuacerI2cVirtualNotifyEvent, I2c,
                    &gEfiEventVirtualAddressChangeGuid,
                    &I2c->VirtualAddressChangeEvent);
    if (EFI_ERROR (Status)) {
      goto FreeDevice;
    }
  }

  CopyGuid (&I2c->DevicePath.Vendor.Guid, &gEfiCallerIdGuid);
  I2c->DevicePath.MmioBase = I2c->MmioBase;
  SetDevicePathNodeLength (&I2c->DevicePath.Vendor,
    sizeof (I2c->DevicePath) - sizeof (I2c->DevicePath.End));
  SetDevicePathEndNode (&I2c->DevicePath.End);

  Status = gBS->InstallMultipleProtocolInterfaces (&ControllerHandle,
                  &gEfiI2cMasterProtocolGuid, &I2c->I2cMaster,
                  &gEfiDevicePathProtocolGuid, &I2c->DevicePath,
                  NULL);
  if (EFI_ERROR (Status)) {
    goto CloseEvent;
  }
  return EFI_SUCCESS;

CloseEvent:
  if (Runtime) {
    gBS->CloseEvent (I2c->VirtualAddressChangeEvent);
  }

FreeDevice:
  FreePool (I2c);

CloseProtocol:
  gBS->CloseProtocol (ControllerHandle,
                      &gEdkiiNonDiscoverableDeviceProtocolGuid,
                      DriverBindingHandle,
                      ControllerHandle);
  return Status;
}

EFI_STATUS
SynQuacerI2cRelease (
  IN      EFI_HANDLE        DriverBindingHandle,
  IN      EFI_HANDLE        ControllerHandle
  )
{
  EFI_I2C_MASTER_PROTOCOL       *I2cMaster;
  SYNQUACER_I2C_MASTER          *I2c;
  EFI_STATUS                    Status;

  Status = gBS->HandleProtocol (ControllerHandle,
                                &gEfiI2cMasterProtocolGuid,
                                (VOID **)&I2cMaster);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  I2c = SYNQUACER_I2C_FROM_THIS (I2cMaster);

  Status = gBS->UninstallMultipleProtocolInterfaces (ControllerHandle,
                  &gEfiI2cMasterProtocolGuid, I2cMaster,
                  &gEfiDevicePathProtocolGuid, &I2c->DevicePath,
                  NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (I2c->Runtime) {
    gBS->CloseEvent (I2c->VirtualAddressChangeEvent);
  }

  Status = gBS->CloseProtocol (ControllerHandle,
                               &gEdkiiNonDiscoverableDeviceProtocolGuid,
                               DriverBindingHandle,
                               ControllerHandle);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  gBS->FreePool (I2c);

  return EFI_SUCCESS;
}
