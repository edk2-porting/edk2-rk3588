/** @file

  This driver produces an EFI_RNG_PROTOCOL instance for the Armada 70x0 TRNG

  Copyright (C) 2017, Linaro Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/Rng.h>

#define TRNG_OUTPUT_REG                         mTrngBaseAddress
#define TRNG_OUTPUT_SIZE                        0x10

#define TRNG_STATUS_REG                         (mTrngBaseAddress + 0x10)
#define TRNG_STATUS_READY                       BIT0

#define TRNG_INTACK_REG                         (mTrngBaseAddress + 0x10)
#define TRNG_INTACK_READY                       BIT0

#define TRNG_CONTROL_REG                        (mTrngBaseAddress + 0x14)
#define TRNG_CONTROL_REG_ENABLE                 BIT10

#define TRNG_CONFIG_REG                         (mTrngBaseAddress + 0x18)
#define __MIN_REFILL_SHIFT                      0
#define __MAX_REFILL_SHIFT                      16
#define TRNG_CONFIG_MIN_REFILL_CYCLES           (0x05 << __MIN_REFILL_SHIFT)
#define TRNG_CONFIG_MAX_REFILL_CYCLES           (0x22 << __MAX_REFILL_SHIFT)

#define TRNG_FRODETUNE_REG                      (mTrngBaseAddress + 0x24)
#define TRNG_FRODETUNE_MASK                     0x0

#define TRNG_FROENABLE_REG                      (mTrngBaseAddress + 0x20)
#define TRNG_FROENABLE_MASK                     0xffffff

#define TRNG_MAX_RETRIES                        20

STATIC EFI_PHYSICAL_ADDRESS                     mTrngBaseAddress;

/**
  Returns information about the random number generation implementation.

  @param[in]     This                 A pointer to the EFI_RNG_PROTOCOL
                                      instance.
  @param[in,out] RNGAlgorithmListSize On input, the size in bytes of
                                      RNGAlgorithmList.
                                      On output with a return code of
                                      EFI_SUCCESS, the size in bytes of the
                                      data returned in RNGAlgorithmList. On
                                      output with a return code of
                                      EFI_BUFFER_TOO_SMALL, the size of
                                      RNGAlgorithmList required to obtain the
                                      list.
  @param[out] RNGAlgorithmList        A caller-allocated memory buffer filled
                                      by the driver with one EFI_RNG_ALGORITHM
                                      element for each supported RNG algorithm.
                                      The list must not change across multiple
                                      calls to the same driver. The first
                                      algorithm in the list is the default
                                      algorithm for the driver.

  @retval EFI_SUCCESS                 The RNG algorithm list was returned
                                      successfully.
  @retval EFI_UNSUPPORTED             The services is not supported by this
                                      driver.
  @retval EFI_DEVICE_ERROR            The list of algorithms could not be
                                      retrieved due to a hardware or firmware
                                      error.
  @retval EFI_INVALID_PARAMETER       One or more of the parameters are
                                      incorrect.
  @retval EFI_BUFFER_TOO_SMALL        The buffer RNGAlgorithmList is too small
                                      to hold the result.

**/
STATIC
EFI_STATUS
EFIAPI
Armada7k8kRngGetInfo (
  IN      EFI_RNG_PROTOCOL        *This,
  IN OUT  UINTN                   *RNGAlgorithmListSize,
  OUT     EFI_RNG_ALGORITHM       *RNGAlgorithmList
  )
{
  if (This == NULL || RNGAlgorithmListSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (*RNGAlgorithmListSize < sizeof (EFI_RNG_ALGORITHM)) {
    *RNGAlgorithmListSize = sizeof (EFI_RNG_ALGORITHM);
    return EFI_BUFFER_TOO_SMALL;
  }

  if (RNGAlgorithmList == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *RNGAlgorithmListSize = sizeof (EFI_RNG_ALGORITHM);
  CopyGuid (RNGAlgorithmList, &gEfiRngAlgorithmRaw);

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
GetTrngData (
  IN    UINTN   Length,
  OUT   UINT8   *Bits
  )
{
  UINTN   Tries;
  UINT32  Buf[TRNG_OUTPUT_SIZE / sizeof (UINT32)];
  UINTN   Index;

  for (Tries = 0; Tries < TRNG_MAX_RETRIES; Tries++) {
    if (MmioRead32 (TRNG_STATUS_REG) & TRNG_STATUS_READY) {
      for (Index = 0; Index < ARRAY_SIZE (Buf); Index++) {
        Buf[Index] = MmioRead32 (TRNG_OUTPUT_REG + Index * sizeof (UINT32));
      }
      CopyMem (Bits, Buf, Length);
      MmioWrite32 (TRNG_INTACK_REG, TRNG_INTACK_READY);

      return EFI_SUCCESS;
    }
    // Wait for more TRNG data to arrive
    gBS->Stall (10);
  }
  return EFI_DEVICE_ERROR;
}

/**
  Produces and returns an RNG value using either the default or specified RNG
  algorithm.

  @param[in]  This                    A pointer to the EFI_RNG_PROTOCOL
                                      instance.
  @param[in]  RNGAlgorithm            A pointer to the EFI_RNG_ALGORITHM that
                                      identifies the RNG algorithm to use. May
                                      be NULL in which case the function will
                                      use its default RNG algorithm.
  @param[in]  RNGValueLength          The length in bytes of the memory buffer
                                      pointed to by RNGValue. The driver shall
                                      return exactly this numbers of bytes.
  @param[out] RNGValue                A caller-allocated memory buffer filled
                                      by the driver with the resulting RNG
                                      value.

  @retval EFI_SUCCESS                 The RNG value was returned successfully.
  @retval EFI_UNSUPPORTED             The algorithm specified by RNGAlgorithm
                                      is not supported by this driver.
  @retval EFI_DEVICE_ERROR            An RNG value could not be retrieved due
                                      to a hardware or firmware error.
  @retval EFI_NOT_READY               There is not enough random data available
                                      to satisfy the length requested by
                                      RNGValueLength.
  @retval EFI_INVALID_PARAMETER       RNGValue is NULL or RNGValueLength is
                                      zero.

**/
STATIC
EFI_STATUS
EFIAPI
Armada7k8kRngGetRNG (
  IN EFI_RNG_PROTOCOL            *This,
  IN EFI_RNG_ALGORITHM           *RNGAlgorithm, OPTIONAL
  IN UINTN                       RNGValueLength,
  OUT UINT8                      *RNGValue
  )
{
  UINTN         Length;
  EFI_STATUS    Status;

  if (This == NULL || RNGValueLength == 0 || RNGValue == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // We only support the raw algorithm, so reject requests for anything else
  //
  if (RNGAlgorithm != NULL &&
      !CompareGuid (RNGAlgorithm, &gEfiRngAlgorithmRaw)) {
    return EFI_UNSUPPORTED;
  }

  do {
    Length = MIN (RNGValueLength, TRNG_OUTPUT_SIZE);
    Status = GetTrngData (Length, RNGValue);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    RNGValue += Length;
    RNGValueLength -= Length;
  } while (RNGValueLength > 0);

  return EFI_SUCCESS;
}

STATIC EFI_RNG_PROTOCOL mArmada7k8kRngProtocol = {
  Armada7k8kRngGetInfo,
  Armada7k8kRngGetRNG
};

//
// Entry point of this driver.
//
EFI_STATUS
EFIAPI
Armada7k8kRngDxeEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  mTrngBaseAddress = PcdGet64 (PcdEip76TrngBaseAddress);

  //
  // Disable the TRNG before updating its configuration
  //
  MmioAnd32 (TRNG_CONTROL_REG, ~TRNG_CONTROL_REG_ENABLE);

  //
  // Configure the internal conditioning parameters of the TRNG
  //
  MmioWrite32 (TRNG_CONFIG_REG, TRNG_CONFIG_MIN_REFILL_CYCLES |
                                TRNG_CONFIG_MAX_REFILL_CYCLES);

  //
  // Configure the FROs
  //
  MmioWrite32 (TRNG_FRODETUNE_REG, TRNG_FRODETUNE_MASK);
  MmioWrite32 (TRNG_FROENABLE_REG, TRNG_FROENABLE_MASK);

  //
  // Enable the TRNG
  //
  MmioOr32 (TRNG_CONTROL_REG, TRNG_CONTROL_REG_ENABLE);

  return SystemTable->BootServices->InstallMultipleProtocolInterfaces (
                                      &ImageHandle,
                                      &gEfiRngProtocolGuid,
                                      &mArmada7k8kRngProtocol,
                                      NULL
                                      );
}
