/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __USRA_ACCESS_LIB_H__
#define __USRA_ACCESS_LIB_H__

#include <Base.h>
#include <Library/UsraAccessApi.h>
#include <Library/CsrToPcieAddress.h>
#include <Library/PcieAddress.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/S3BootScriptLib.h>

//
// Get Aligned Access Address
//
typedef
  VOID
  (EFIAPI *GET_ALLIGNED_ACCESS_ADDRESS) (VOID*, BOOLEAN, USRA_ADDRESS*, UINTN*);

//
// Register Read
//
typedef
  RETURN_STATUS
  (EFIAPI *REGISTER_READ) (USRA_ADDRESS *, VOID *);

//
// Register Write
//
typedef
  RETURN_STATUS
  (EFIAPI *REGISTER_WRITE) (USRA_ADDRESS *, VOID *);

//
// Register Write
//
typedef
  RETURN_STATUS
  (EFIAPI *REGISTER_MODIFY) (USRA_ADDRESS *, VOID *, VOID *);

/**
  This API get the Pcie address from the given USRA Address.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be read out

  @retval NONE
**/
VOID
GetPcieAccessAddress (
  IN VOID                     *Global,
  IN BOOLEAN                  Virtual,
  IN USRA_ADDRESS             *Address,
  OUT UINTN                   *AlignedAddress
  );

/**
  This API get the CSR address from the given USRA Address.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be read out
  @param[in] Buffer               A pointer of buffer for the value read from the register

  @retval NONE
**/
VOID
GetCsrAccessAddress (
  IN VOID                     *Global,
  IN BOOLEAN                  Virtual,
  IN USRA_ADDRESS             *Address,
  OUT UINTN                   *AlignedAddress
  );

/**
  This API Perform 8-bit, 16-bit, 32-bit or 64-bit Pcie silicon register read operations.
  It transfers data from a register into a naturally aligned data buffer.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be read out
  @param[in] Buffer               A pointer of buffer for the value read from the register

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
PcieRegisterRead (
  IN USRA_ADDRESS             *Address,
  IN VOID                     *Buffer
  );

/**
  This API Perform 8-bit, 16-bit, 32-bit or 64-bit Pcie block silicon register read operations.
  It transfers data from a register into a naturally aligned data buffer.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be read out
  @param[in] Buffer               A pointer of buffer for the value read from the register

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
PcieBlkRegisterRead (
  IN USRA_ADDRESS             *Address,
  IN VOID                     *Buffer
  );

/**
  This API Perform 8-bit, 16-bit, 32-bit or 64-bit CSR silicon register read operations.
  It transfers data from a register into a naturally aligned data buffer.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be read out
  @param[in] Buffer               A pointer of buffer for the value read from the register

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
CsrRegisterRead (
  IN USRA_ADDRESS             *Address,
  IN VOID                     *Buffer
  );

/**
  This API Perform 8-bit, 16-bit, 32-bit or 64-bit Pcie silicon register write operations.
  It transfers data from a naturally aligned data buffer into a register.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be written
  @param[in] Buffer               A pointer of buffer for the value write to the register

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
PcieRegisterWrite (
  IN USRA_ADDRESS             *Address,
  OUT VOID                    *Buffer
  );

/**
  This API Perform 8-bit, 16-bit, 32-bit or 64-bit Pcie block silicon register write operations.
  It transfers data from a naturally aligned data buffer into a register.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be written
  @param[in] Buffer               A pointer of buffer for the value write to the register

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
PcieBlkRegisterWrite (
  IN USRA_ADDRESS             *Address,
  OUT VOID                    *Buffer
  );

/**
  This API Perform 8-bit, 16-bit, 32-bit or 64-bit CSR silicon register write operations.
  It transfers data from a naturally aligned data buffer into a register.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be written
  @param[in] Buffer               A pointer of buffer for the value write to the register

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
CsrRegisterWrite (
  IN USRA_ADDRESS             *Address,
  OUT VOID                    *Buffer
  );

/**
  This API Perform 8-bit, 16-bit, 32-bit or 64-bit Pcie silicon register AND then OR operations. It read data from a
  register, And it with the AndBuffer, then Or it with the OrBuffer, and write the result back to the register

  @param[in] Address              A pointer of the address of the silicon register to be modified
  @param[in] AndBuffer            A pointer of buffer for the value used for AND operation
                                  A NULL pointer means no AND operation. RegisterModify() equivalents to RegisterOr()
  @param[in] OrBuffer             A pointer of buffer for the value used for OR operation
                                  A NULL pointer means no OR operation. RegisterModify() equivalents to RegisterAnd()

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
PcieRegisterModify (
  IN USRA_ADDRESS             *Address,
  IN VOID                     *AndBuffer,
  IN VOID                     *OrBuffer
  );

/**
  This API Perform 8-bit, 16-bit, 32-bit or 64-bit CSR silicon register AND then OR operations. It read data from a
  register, And it with the AndBuffer, then Or it with the OrBuffer, and write the result back to the register

  @param[in] Address              A pointer of the address of the silicon register to be modified
  @param[in] AndBuffer            A pointer of buffer for the value used for AND operation
                                  A NULL pointer means no AND operation. RegisterModify() equivalents to RegisterOr()
  @param[in] OrBuffer             A pointer of buffer for the value used for OR operation
                                  A NULL pointer means no OR operation. RegisterModify() equivalents to RegisterAnd()

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
CsrRegisterModify (
  IN USRA_ADDRESS             *Address,
  IN VOID                     *AndBuffer,
  IN VOID                     *OrBuffer
  );

/**
  Perform MMIO read

  @param[in] AccessWidth          Access Width
  @param[in] AlignedAddress       An address to be read out
  @param[in] Buffer               A pointer of buffer contains the data to be read out

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
UsraRegAlignedRead (
  IN UINT32                   AccessWidth,
  IN UINTN                    AlignedAddress,
  OUT VOID                    *Buffer
  );

/**
  Perform AND then OR operations for a input data

  @param[in out] Data             A pointer of the address of the register to be modified
  @param[in] AndBuffer            A pointer of buffer for the value used for AND operation
                                  A NULL pointer means no AND operation. RegisterModify() equivalents to RegisterOr()
  @param[in] OrBuffer             A pointer of buffer for the value used for OR operation
                                  A NULL pointer means no OR operation. RegisterModify() equivalents to RegisterAnd()
  @param[in] NumOfByte            NumOfByte Count of byte data to be performed

  @retval NONE
**/
VOID
DataAndOr (
  IN  UINT64                  *Data,
  IN  VOID                    *AndBuffer,
  IN  VOID                    *OrBuffer,
  IN  UINT8                   NumOfByte
);

/**
  Perform MMIO write

  @param[in] AccessWidth          Access Width
  @param[in] AlignedAddress       An address to be written
  @param[in] Buffer               A pointer of buffer contains the data to be written

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
UsraRegAlignedWrite (
  IN UINT32                   AccessWidth,
  IN UINTN                    AlignedAddress,
  OUT VOID                    *Buffer
  );
#endif
