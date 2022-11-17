/** @file
  Device driver for the Atmel ATSHA204A random number generator.

  Copyright (c) 2018, Linaro Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _ATSHA204A_I2C_HWRNG_DRIVER_H_
#define _ATSHA204A_I2C_HWRNG_DRIVER_H_

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include <Protocol/Rng.h>
#include <Protocol/I2cIo.h>

#define ATSHA204A_OUTPUT_SIZE     32

#define ATSHA204A_DEV_SIGNATURE   SIGNATURE_32('a','t','s','h')

typedef struct {
  UINT32                        Signature;
  EFI_I2C_IO_PROTOCOL           *I2cIo;
  EFI_RNG_PROTOCOL              Rng;
} ATSHA204A_DEV;

#define ATSHA204A_DEV_FROM_THIS(a) \
  CR(a, ATSHA204A_DEV, Rng, ATSHA204A_DEV_SIGNATURE)

#pragma pack(1)
typedef struct {
  UINT8                           Command;
  UINT8                           Count;
  UINT8                           Opcode;
  UINT8                           Param1;
  UINT16                          Param2;
  UINT16                          Crc;
} ATSHA204A_I2C_RNG_COMMAND;

typedef struct {
  UINT8                           Count;
  UINT8                           Result[ATSHA204A_OUTPUT_SIZE];
  UINT16                          Crc;
} ATSHA204A_I2C_RNG_RESULT;
#pragma pack()

typedef struct {
  UINTN                           OperationCount;
  EFI_I2C_OPERATION               Operation;
} I2C_RNG_REQUEST;

#define ATSHA204A_COMMAND         0x3

#define ATSHA204A_OPCODE_RANDOM   0x1b

extern EFI_COMPONENT_NAME2_PROTOCOL gAtSha204aDriverComponentName2;

EFI_STATUS
AtSha204aInit (
  IN  EFI_HANDLE        DriverBindingHandle,
  IN  EFI_HANDLE        ControllerHandle
  );

EFI_STATUS
AtSha204aRelease (
  IN  EFI_HANDLE        DriverBindingHandle,
  IN  EFI_HANDLE        ControllerHandle
  );

#endif // _ATSHA204A_I2C_HWRNG_DRIVER_H_
