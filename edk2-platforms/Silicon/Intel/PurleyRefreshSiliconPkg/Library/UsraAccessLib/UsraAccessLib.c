/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "UsraAccessLib.h"

GET_ALLIGNED_ACCESS_ADDRESS mAccessAddrPtr[] =
{
  &GetPcieAccessAddress,     // AddrTypePCIE
  &GetPcieAccessAddress,     // AddrTypePCIEBLK
  &GetCsrAccessAddress,      // AddrTypeCSR
};

REGISTER_READ mRegisterReadPtr[] =
{
  &PcieRegisterRead,        // AddrTypePCIE
  &PcieBlkRegisterRead,     // AddrTypePCIEBLK
  &CsrRegisterRead,         // AddrTypeCSR
};

REGISTER_WRITE mRegisterWritePtr[] =
{
  &PcieRegisterWrite,       // AddrTypePCIE
  &PcieBlkRegisterWrite,    // AddrTypePCIEBLK
  &CsrRegisterWrite,        // AddrTypeCSR
};

REGISTER_MODIFY mRegisterModifyPtr[] =
{
  &PcieRegisterModify,      // AddrTypePCIE
  &PcieRegisterModify,      // AddrTypePCIEBLK
  &CsrRegisterModify,       // AddrTypeCSR
};

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
  )
{
  switch (AccessWidth)
  {
  case  UsraWidth8:
    *((UINT8*)Buffer) = MmioRead8 (AlignedAddress);
    break;
  case  UsraWidth16:
    *((UINT16*)Buffer) = MmioRead16 (AlignedAddress);
    break;
  case  UsraWidth32:
    *((UINT32*)Buffer) = MmioRead32 (AlignedAddress);
    break;
  default:
    *((UINT64*)Buffer) = MmioRead64 (AlignedAddress);
    break;
  }

  return RETURN_SUCCESS;
};

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
  )
{
  switch (AccessWidth)
  {
  case  UsraWidth8:
    MmioWrite8 (AlignedAddress,*((UINT8*)Buffer));
    break;
  case  UsraWidth16:
    MmioWrite16 (AlignedAddress,*((UINT16*)Buffer));
    break;
  case  UsraWidth32:
    MmioWrite32 (AlignedAddress,*((UINT32*)Buffer));
    break;
  default:
    MmioWrite64 (AlignedAddress, *((UINT64*)Buffer));
    break;
  }
  return RETURN_SUCCESS;
}

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
)
{
  union{
    UINT64      QW;
    UINT8       Byte[8];
  } Buffer;
  UINT8                       AndData[8], OrData[8], i;

  Buffer.QW = *Data;
  for(i=0;i<NumOfByte;i++)
  {
    if (AndBuffer == NULL)
      ((UINT8*)AndData)[i] = 0xff;
    else
      AndData[i] = ((UINT8*)AndBuffer)[i];
    if (OrBuffer == NULL)
      ((UINT8*)OrData)[i] = 0;
    else
      OrData[i] = ((UINT8*)OrBuffer)[i];
    Buffer.Byte[i] = (Buffer.Byte[i] & AndData[i]) | OrData[i];
  }

  *Data = Buffer.QW;
}

//////////////////////////////////////////////////////////////////////////
//
// USRA Hardware Access Library
//
//////////////////////////////////////////////////////////////////////////

/**
  This API gets the flat address from the given USRA Address.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be read out

  @retval                         The flat address
**/
INTN
EFIAPI
GetRegisterAddress (
  IN USRA_ADDRESS            *Address
  )
{
  UINTN                       AlignedAddress;

  mAccessAddrPtr[Address->Attribute.AddrType] (NULL, 0, Address, &AlignedAddress);

  return AlignedAddress;
};

/**
  This API performs 8-bit, 16-bit, 32-bit or 64-bit silicon register read operations.
  It transfers data from a register into a naturally aligned data buffer.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be read out
  @param[in] Buffer               A pointer of buffer for the value read from the register

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
EFIAPI
RegisterRead (
  IN USRA_ADDRESS             *Address,
  IN VOID                     *Buffer
  )
{
  return mRegisterReadPtr[Address->Attribute.AddrType] (Address, Buffer);
};

/**
  This API performs 8-bit, 16-bit, 32-bit or 64-bit silicon register write operations.
  It transfers data from a naturally aligned data buffer into a silicon register.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be written
  @param[in] Buffer               A pointer of buffer for the value write to the register

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
EFIAPI
RegisterWrite (
  IN USRA_ADDRESS             *Address,
  OUT VOID                    *Buffer
  )
{
  return mRegisterWritePtr[Address->Attribute.AddrType] (Address, Buffer);
};

/**
  This API performs 8-bit, 16-bit, 32-bit or 64-bit silicon register AND then OR operations. It read data from a
  register, And it with the AndBuffer, then Or it with the OrBuffer, and write the result back to the register

  @param[in] Address              A pointer of the address of the silicon register to be written
  @param[in] AndBuffer            A pointer of buffer for the value used for AND operation
                                  A NULL pointer means no AND operation. RegisterModify() equivalents to RegisterOr()
  @param[in] OrBuffer             A pointer of buffer for the value used for OR operation
                                  A NULL pointer means no OR operation. RegisterModify() equivalents to RegisterAnd()

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
EFIAPI
RegisterModify (
  IN USRA_ADDRESS             *Address,
  IN VOID                     *AndBuffer,
  IN VOID                     *OrBuffer
  )
{
  return mRegisterModifyPtr[Address->Attribute.AddrType] (Address, AndBuffer, OrBuffer);
};
