/** @file
  Device driver for the OpteeRng hardware random number generator.

  Copyright (c) 2018, Linaro Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/OpteeLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/Rng.h>

#define PTA_COMMAND_GET_ENTROPY  0x0
#define OPTEE_RNG_POOL_SIZE      (4 * 1024)

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
  EFI_STATUS                 Status;
  OPTEE_OPEN_SESSION_ARG     OpenSessionArg;
  OPTEE_INVOKE_FUNCTION_ARG  InvokeFunctionArg;
  UINT8                      *OutPointer;
  UINTN                      OutSize;
  UINTN                      WaitMiliSeconds;

  if ((Value == NULL) || (ValueLength == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  if (ValueLength > OPTEE_RNG_POOL_SIZE) {
    return EFI_UNSUPPORTED;
  }

  if (Algorithm != NULL && !CompareGuid (Algorithm, &gEfiRngAlgorithmRaw)) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&OpenSessionArg, sizeof (OPTEE_OPEN_SESSION_ARG));
  CopyMem (&OpenSessionArg.Uuid, &gOpteeRngTaGuid, sizeof (EFI_GUID));

  Status = OpteeOpenSession (&OpenSessionArg);
  if ((Status != EFI_SUCCESS) || (OpenSessionArg.Return != OPTEE_SUCCESS)) {
    DEBUG ((DEBUG_ERROR, "OP-TEE Open Session failed with return: %08x and"
      "return origin: %d\n", OpenSessionArg.Return,
      OpenSessionArg.ReturnOrigin));
    return EFI_DEVICE_ERROR;
  }

  OutPointer = Value;

  while (ValueLength > 0) {
    ZeroMem (&InvokeFunctionArg, sizeof (OPTEE_INVOKE_FUNCTION_ARG));

    InvokeFunctionArg.Function = PTA_COMMAND_GET_ENTROPY;
    InvokeFunctionArg.Session = OpenSessionArg.Session;

    InvokeFunctionArg.Params[0].Attribute =
      OPTEE_MESSAGE_ATTRIBUTE_TYPE_MEMORY_INOUT;
    InvokeFunctionArg.Params[0].Union.Memory.BufferAddress =
      (UINTN) OutPointer;
    InvokeFunctionArg.Params[0].Union.Memory.Size = ValueLength;

    Status = OpteeInvokeFunction (&InvokeFunctionArg);
    if ((Status != EFI_SUCCESS) ||
        (InvokeFunctionArg.Return != OPTEE_SUCCESS)) {
      DEBUG ((DEBUG_ERROR, "OP-TEE Invoke Function failed with return: %x and"
        "return origin: %d\n", InvokeFunctionArg.Return,
        InvokeFunctionArg.ReturnOrigin));

      OpteeCloseSession (OpenSessionArg.Session);

      return EFI_DEVICE_ERROR;
    }

    OutSize = MIN (InvokeFunctionArg.Params[0].Union.Memory.Size, ValueLength);

    OutPointer += OutSize;
    ValueLength -= OutSize;

    //
    // OP-TEE RNG Trusted application takes approximately 256ms for every 32
    // bytes of full entropy output.
    //
    if (ValueLength > 0) {
      WaitMiliSeconds = ((ValueLength + 32) * 256) / 32;
      MicroSecondDelay (WaitMiliSeconds * 1000);
    }
  }

  OpteeCloseSession (OpenSessionArg.Session);

  return EFI_SUCCESS;
}

//
// OP-TEE based Random Number Generator (RNG) protocol
//
EFI_RNG_PROTOCOL mOpteeRng = {
  GetInfo,
  GetRNG
};

/**
  The user Entry Point for the OP-TEE Random Number Generator (RNG) driver.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval EFI_NOT_FOUND     Not able to find OP-TEE based RNG.
  @retval Other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
OpteeRngEntry (
  IN EFI_HANDLE          ImageHandle,
  IN EFI_SYSTEM_TABLE    *SystemTable
  )
{
  EFI_STATUS                 Status;
  EFI_HANDLE                 Handle;
  OPTEE_OPEN_SESSION_ARG     OpenSessionArg;

  if (!IsOpteePresent()) {
    return EFI_NOT_FOUND;
  }

  //
  //  Initialize OP-TEE
  //
  Status = OpteeInit ();
  if (Status != EFI_SUCCESS) {
    return EFI_NOT_FOUND;
  }

  ZeroMem (&OpenSessionArg, sizeof (OPTEE_OPEN_SESSION_ARG));
  CopyMem (&OpenSessionArg.Uuid, &gOpteeRngTaGuid, sizeof (EFI_GUID));

  //
  //  Try to open session with RNG Trusted Application to check if its present
  //
  Status = OpteeOpenSession (&OpenSessionArg);
  if ((Status != EFI_SUCCESS) || (OpenSessionArg.Return != OPTEE_SUCCESS)) {
    return EFI_NOT_FOUND;
  } else {
    OpteeCloseSession (OpenSessionArg.Session);
  }

  //
  // Install UEFI RNG (Random Number Generator) Protocol
  //
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (&Handle,
                                          &gEfiRngProtocolGuid,
                                          EFI_NATIVE_INTERFACE,
                                          &mOpteeRng);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "Failed to install OP-TEE RNG protocol interface (Status == %r)\n",
    Status));
    return Status;
  }

  DEBUG ((DEBUG_INIT | DEBUG_INFO, "*** Installed OpteeRng driver! ***\n"));

  return EFI_SUCCESS;
}
