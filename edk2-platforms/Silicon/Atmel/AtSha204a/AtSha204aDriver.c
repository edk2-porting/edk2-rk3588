/** @file
  Device driver for the Atmel ATSHA204A random number generator.

  Copyright (c) 2018, Linaro Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "AtSha204aDriver.h"

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>

#define MAX_RETRIES                 5

// Don't bother calculating the CRC for the immutable RANDOM opcode packet
#define OPCODE_COMMAND_PACKET_CRC   0xcd24

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
AtSha240aGetInfo (
  IN      EFI_RNG_PROTOCOL    *This,
  IN  OUT UINTN               *AlgorithmListSize,
  OUT     EFI_RNG_ALGORITHM   *AlgorithmList
)
{
  UINTN Size;

  //
  // We only implement the raw algorithm
  //
  Size = sizeof (EFI_GUID);

  if (*AlgorithmListSize < Size) {
    *AlgorithmListSize = Size;
    return EFI_BUFFER_TOO_SMALL;
  }

  CopyGuid (AlgorithmList, &gEfiRngAlgorithmRaw);
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
AtSha240aGetRNG (
  IN EFI_RNG_PROTOCOL   *This,
  IN EFI_RNG_ALGORITHM  *Algorithm OPTIONAL,
  IN UINTN              ValueLength,
  OUT UINT8             *Value
)
{
  EFI_STATUS                  Status;
  ATSHA204A_DEV               *AtSha204a;
  ATSHA204A_I2C_RNG_COMMAND   Command;
  ATSHA204A_I2C_RNG_RESULT    Result;
  I2C_RNG_REQUEST             Request;
  I2C_RNG_REQUEST             Response;
  UINTN                       Retries;

  if (Algorithm != NULL && !CompareGuid (Algorithm, &gEfiRngAlgorithmRaw)) {
    return EFI_UNSUPPORTED;
  }

  AtSha204a = ATSHA204A_DEV_FROM_THIS (This);

  Request.OperationCount  = 1;
  Request.Operation.Flags = 0;

  Command.Command   = ATSHA204A_COMMAND;
  Command.Count     = sizeof (Command) - 1;
  Command.Opcode    = ATSHA204A_OPCODE_RANDOM;
  Command.Param1    = 0;
  Command.Param2    = 0;
  Command.Crc       = OPCODE_COMMAND_PACKET_CRC;

  Response.OperationCount           = 1;
  Response.Operation.Flags          = I2C_FLAG_READ;
  Response.Operation.LengthInBytes  = sizeof (Result);
  Response.Operation.Buffer         = (VOID *)&Result;

  Retries = 0;
  while (ValueLength > 0) {
    //
    // The AtSha204a will go back to sleep right in the middle of a transaction
    // if it does not complete in ~1.3 seconds. So send the wake sequence for
    // each iteration, which consists of a dummy write to slave address 0x0.
    //
    Request.Operation.LengthInBytes = 0;
    Status = AtSha204a->I2cIo->QueueRequest (AtSha204a->I2cIo, 1, NULL,
                                 (VOID *)&Request, NULL);
    DEBUG ((DEBUG_INFO, "%a: wake AtSha204a: I2cIo->QueueRequest() - %r\n",
      __FUNCTION__, Status));

    gBS->Stall (2500); // wait 2.5 ms for wake to complete

    Request.Operation.LengthInBytes = sizeof (Command);
    Request.Operation.Buffer = (VOID *)&Command;
    Status = AtSha204a->I2cIo->QueueRequest (AtSha204a->I2cIo, 0, NULL,
                                 (VOID *)&Request, NULL);
    if (EFI_ERROR (Status)) {
      if (++Retries <= MAX_RETRIES) {
        continue;
      }
      DEBUG ((DEBUG_ERROR, "%a: I2C request transfer failed, Status == %r\n",
        __FUNCTION__, Status));
      return EFI_DEVICE_ERROR;
    }

    gBS->Stall (50 * 1000); // 50 ms max execution time for RANDOM opcode

    Status = AtSha204a->I2cIo->QueueRequest (AtSha204a->I2cIo, 0, NULL,
                                 (VOID *)&Response, NULL);
    if (EFI_ERROR (Status)) {
      if (++Retries <= MAX_RETRIES) {
        continue;
      }
      DEBUG ((DEBUG_ERROR, "%a: I2C response transfer failed, Status == %r\n",
        __FUNCTION__, Status));
      return EFI_DEVICE_ERROR;
    }

    if (Result.Count < sizeof (Result)) {
      //
      // Incomplete packet received, most likely due to an error. Retry.
      //
      if (++Retries <= MAX_RETRIES) {
        continue;
      }
      DEBUG ((DEBUG_WARN, "%a: incomplete packet received\n", __FUNCTION__));
      return EFI_DEVICE_ERROR;
    }

    gBS->CopyMem (Value, Result.Result, MIN (ValueLength,
                                             ATSHA204A_OUTPUT_SIZE));
    if (ValueLength < ATSHA204A_OUTPUT_SIZE) {
      break;
    }

    Value += ATSHA204A_OUTPUT_SIZE;
    ValueLength -= ATSHA204A_OUTPUT_SIZE;
    Retries = 0;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
AtSha204aInit (
  IN      EFI_HANDLE        DriverBindingHandle,
  IN      EFI_HANDLE        ControllerHandle
  )
{
  EFI_STATUS                Status;
  ATSHA204A_DEV             *AtSha204a;

  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (ATSHA204A_DEV),
                  (VOID **) &AtSha204a);
  if (EFI_ERROR (Status)) {
    return EFI_OUT_OF_RESOURCES;
  }

  AtSha204a->Signature    = ATSHA204A_DEV_SIGNATURE;
  AtSha204a->Rng.GetInfo  = AtSha240aGetInfo;
  AtSha204a->Rng.GetRNG   = AtSha240aGetRNG;

  //
  // Open I2C I/O Protocol
  //
  Status = gBS->OpenProtocol (ControllerHandle,
                              &gEfiI2cIoProtocolGuid,
                              (VOID **)&AtSha204a->I2cIo,
                              DriverBindingHandle,
                              ControllerHandle,
                              EFI_OPEN_PROTOCOL_BY_DRIVER);
  if (EFI_ERROR (Status)) {
    goto ErrorFreeDev;
  }

  Status = gBS->InstallProtocolInterface (&ControllerHandle,
                                          &gEfiRngProtocolGuid,
                                          EFI_NATIVE_INTERFACE,
                                          &AtSha204a->Rng);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "Failed to install RNG protocol interface (Status == %r)\n",
    Status));
    goto ErrorCloseProtocol;
  }

  return EFI_SUCCESS;

ErrorCloseProtocol:
  gBS->CloseProtocol (ControllerHandle, &gEfiI2cIoProtocolGuid,
         DriverBindingHandle, ControllerHandle);

ErrorFreeDev:
  gBS->FreePool (AtSha204a);

  return Status;
}

EFI_STATUS
AtSha204aRelease (
  IN  EFI_HANDLE        DriverBindingHandle,
  IN  EFI_HANDLE        ControllerHandle
  )
{
  EFI_RNG_PROTOCOL    *Rng;
  ATSHA204A_DEV       *AtSha204a;
  EFI_STATUS          Status;

  Status = gBS->HandleProtocol (ControllerHandle,
                                &gEfiRngProtocolGuid,
                                (VOID **)&Rng);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  AtSha204a = ATSHA204A_DEV_FROM_THIS (Rng);

  Status = gBS->UninstallProtocolInterface (ControllerHandle,
                                            &gEfiRngProtocolGuid,
                                            Rng);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->CloseProtocol (ControllerHandle,
                               &gEfiI2cIoProtocolGuid,
                               DriverBindingHandle,
                               ControllerHandle);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  gBS->FreePool (AtSha204a);

  return EFI_SUCCESS;
}
