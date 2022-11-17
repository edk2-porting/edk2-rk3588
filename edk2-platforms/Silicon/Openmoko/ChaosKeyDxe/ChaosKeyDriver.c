/** @file
  Device driver for the ChaosKey hardware random number generator.

  Copyright (c) 2016 - 2017, Linaro Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "ChaosKeyDriver.h"

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>

STATIC
BOOLEAN
IsBulkInEndpoint (
  IN  EFI_USB_ENDPOINT_DESCRIPTOR     *Endpoint
  )
{
  if ((Endpoint->Attributes & USB_ENDPOINT_TYPE_MASK) == USB_ENDPOINT_BULK) {
    if (Endpoint->EndpointAddress & USB_ENDPOINT_DIR_IN) {
      return TRUE;
    }
  }
  return FALSE;
}


STATIC
EFI_STATUS
FindEndpoint (
  IN  CHAOSKEY_DEV *ChaosKey
  )
{
  EFI_USB_IO_PROTOCOL             *UsbIo;
  EFI_STATUS                      Status;
  UINTN                           Index;
  EFI_USB_INTERFACE_DESCRIPTOR    InterfaceDescriptor;

  UsbIo = ChaosKey->UsbIo;

  //
  // Get interface & endpoint descriptor
  //
  Status = UsbIo->UsbGetInterfaceDescriptor (UsbIo, &InterfaceDescriptor);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // The ChaosKey provides two endpoints:
  // - The first one is the 'cooked' one, to be used as random data input
  // - The second one is the raw bitstream from the generator, higher
  //   throughput, but lower randomness.
  // So locate the first bulk IN endpoint and save it for later use.
  //
  for (Index = 0; Index < InterfaceDescriptor.NumEndpoints; Index++) {
    EFI_USB_ENDPOINT_DESCRIPTOR  Endpoint;

    Status = UsbIo->UsbGetEndpointDescriptor (UsbIo, Index, &Endpoint);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "UsbGetEndPointDescriptor(%d) failed!\n", Index));
      return Status;
    }

    if (IsBulkInEndpoint(&Endpoint)) {
      ChaosKey->EndpointAddress = Endpoint.EndpointAddress;
      ChaosKey->EndpointSize = Endpoint.MaxPacketSize;
      return EFI_SUCCESS;
    }
  }

  DEBUG ((DEBUG_ERROR, "Failed to locate suitable BULK IN USB endpoint!\n"));
  return EFI_DEVICE_ERROR;
}


/**
  Returns information about the random number generation implementation.

  @param[in]     This               A pointer to the EFI_RNG_PROTOCOL instance.
  @param[in,out] AlgorithmListSize  On input, the size in bytes of AlgorithmList
                                    On output with a return code of EFI_SUCCESS,
                                    the size in bytes of the data returned in
                                    AlgorithmList. On output with a return
                                    code of EFI_BUFFER_TOO_SMALL, the size of
                                    AlgorithmList required to obtain the list.
  @param[out] AlgorithmList         A caller-allocated memory buffer filled by
                                    the driver with one EFI_RNG_ALGORITHM
                                    element for each supported RNG algorithm.
                                    The list must not change across multiple
                                    calls to the same driver. The first
                                    algorithm in the list is the default
                                    algorithm for the driver.

  @retval EFI_SUCCESS               The RNG algorithm list was returned
                                    successfully.
  @retval EFI_UNSUPPORTED           The services is not supported by this driver
  @retval EFI_DEVICE_ERROR          The list of algorithms could not be
                                    retrieved due to a hardware or firmware
                                    error.
  @retval EFI_INVALID_PARAMETER     One or more of the parameters are incorrect.
  @retval EFI_BUFFER_TOO_SMALL      The buffer RNGAlgorithmList is too small to
                                    hold the result.

**/
STATIC
EFI_STATUS
EFIAPI
GetInfo (
  IN      EFI_RNG_PROTOCOL    *This,
  IN  OUT UINTN               *AlgorithmListSize,
  OUT     EFI_RNG_ALGORITHM   *AlgorithmList
)
{
  UINTN Size;

  //
  // We only implement the raw algorithm
  //
  Size = sizeof gEfiRngAlgorithmRaw;

  if (*AlgorithmListSize < Size) {
    *AlgorithmListSize = Size;
    return EFI_BUFFER_TOO_SMALL;
  }

  gBS->CopyMem (AlgorithmList, &gEfiRngAlgorithmRaw, Size);
  *AlgorithmListSize = Size;

  return EFI_SUCCESS;
}


/**
  Produces and returns an RNG value using either the default or specified RNG
  algorithm.

  @param[in]  This                A pointer to the EFI_RNG_PROTOCOL instance.
  @param[in]  Algorithm           A pointer to the EFI_RNG_ALGORITHM that
                                  identifies the RNG algorithm to use. May be
                                  NULL in which case the function will use its
                                  default RNG algorithm.
  @param[in]  ValueLength         The length in bytes of the memory buffer
                                  pointed to by RNGValue. The driver shall
                                  return exactly this numbers of bytes.
  @param[out] Value               A caller-allocated memory buffer filled by the
                                  driver with the resulting RNG value.

  @retval EFI_SUCCESS             The RNG value was returned successfully.
  @retval EFI_UNSUPPORTED         The algorithm specified by RNGAlgorithm is not
                                  supported by this driver.
  @retval EFI_DEVICE_ERROR        An RNG value could not be retrieved due to a
                                  hardware or firmware error.
  @retval EFI_NOT_READY           There is not enough random data available to
                                  satisfy the length requested by
                                  RNGValueLength.
  @retval EFI_INVALID_PARAMETER   RNGValue is NULL or RNGValueLength is zero.

**/
STATIC
EFI_STATUS
EFIAPI
GetRNG (
  IN EFI_RNG_PROTOCOL   *This,
  IN EFI_RNG_ALGORITHM  *Algorithm OPTIONAL,
  IN UINTN              ValueLength,
  OUT UINT8             *Value
)
{
  EFI_STATUS        Status;
  CHAOSKEY_DEV      *ChaosKey;
  UINT8             Buffer[CHAOSKEY_MAX_EP_SIZE];
  UINT8             *OutPointer;
  UINTN             OutSize;
  UINT32            Result;

  if (Algorithm != NULL && !CompareGuid (Algorithm, &gEfiRngAlgorithmRaw)) {
    return EFI_UNSUPPORTED;
  }

  ChaosKey = CHAOSKEY_DEV_FROM_THIS (This);

  while (ValueLength > 0) {
    //
    // If more data is requested than the endpoint can deliver in a single
    // transfer, put it straight into the caller's buffer.
    //
    if (ValueLength >= ChaosKey->EndpointSize) {
      OutPointer = Value;
    } else {
      OutPointer = Buffer;
    }
    OutSize = ChaosKey->EndpointSize;

    Status = ChaosKey->UsbIo->UsbBulkTransfer (ChaosKey->UsbIo,
                                               ChaosKey->EndpointAddress,
                                               OutPointer,
                                               &OutSize,
                                               CHAOSKEY_TIMEOUT,
                                               &Result);

    if (Status == EFI_TIMEOUT) {
      DEBUG ((DEBUG_ERROR, "Bulk transfer timed out, USB status == %d\n",
        Result));
      return EFI_NOT_READY;
    } else if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR,
        "Bulk transfer failed, Status == %r, USB status == %d\n",
        Status, Result));
      return EFI_DEVICE_ERROR;
    }

    OutSize = MIN (OutSize, ValueLength);

    if (Value != Buffer) {
      gBS->CopyMem (Value, Buffer, OutSize);
    }
    Value += OutSize;
    ValueLength -= OutSize;
  }
  return EFI_SUCCESS;
}


EFI_STATUS
ChaosKeyInit (
  IN      EFI_HANDLE        DriverBindingHandle,
  IN      EFI_HANDLE        ControllerHandle
  )
{
  EFI_STATUS                Status;
  CHAOSKEY_DEV              *ChaosKey;

  Status  = gBS->AllocatePool (EfiBootServicesData,
                               sizeof (CHAOSKEY_DEV),
                               (VOID **) &ChaosKey);
  if (EFI_ERROR (Status)) {
    return EFI_OUT_OF_RESOURCES;
  }

  ChaosKey->Signature         = CHAOSKEY_DEV_SIGNATURE;
  ChaosKey->Rng.GetInfo       = GetInfo;
  ChaosKey->Rng.GetRNG        = GetRNG;

  //
  // Open USB I/O Protocol
  //
  Status = gBS->OpenProtocol (ControllerHandle,
                              &gEfiUsbIoProtocolGuid,
                              (VOID **)&ChaosKey->UsbIo,
                              DriverBindingHandle,
                              ControllerHandle,
                              EFI_OPEN_PROTOCOL_BY_DRIVER);
  if (EFI_ERROR (Status)) {
    goto ErrorFreeDev;
  }

  Status = FindEndpoint (ChaosKey);
  if (EFI_ERROR (Status)) {
    goto ErrorCloseProtocol;
  }

  //
  // The following can only occur if the Chaoskey is suddenly reissued
  // as a high speed or super speed device under the same VID/PID.
  //
  ASSERT (ChaosKey->EndpointSize <= CHAOSKEY_MAX_EP_SIZE);

  Status = gBS->InstallProtocolInterface (&ControllerHandle,
                                          &gEfiRngProtocolGuid,
                                          EFI_NATIVE_INTERFACE,
                                          &ChaosKey->Rng);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "Failed to install RNG protocol interface (Status == %r)\n",
    Status));
    goto ErrorCloseProtocol;
  }

  return EFI_SUCCESS;

ErrorCloseProtocol:
  gBS->CloseProtocol (ControllerHandle, &gEfiUsbIoProtocolGuid,
         DriverBindingHandle, ControllerHandle);

ErrorFreeDev:
  gBS->FreePool (ChaosKey);

  return Status;
}

EFI_STATUS
ChaosKeyRelease (
  IN  EFI_HANDLE        DriverBindingHandle,
  IN  EFI_HANDLE        ControllerHandle
  )
{
  EFI_RNG_PROTOCOL    *Rng;
  CHAOSKEY_DEV        *ChaosKey;
  EFI_STATUS          Status;

  Status = gBS->HandleProtocol (ControllerHandle,
                                &gEfiRngProtocolGuid,
                                (VOID **)&Rng);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ChaosKey = CHAOSKEY_DEV_FROM_THIS (Rng);

  Status = gBS->UninstallProtocolInterface (ControllerHandle,
                                            &gEfiRngProtocolGuid,
                                            Rng);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->CloseProtocol (ControllerHandle,
                               &gEfiUsbIoProtocolGuid,
                               DriverBindingHandle,
                               ControllerHandle);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  gBS->FreePool (ChaosKey);

  return EFI_SUCCESS;
}
