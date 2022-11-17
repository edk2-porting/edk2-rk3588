/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "UsraAccessLib.h"

#define MAX_IO_PORT_ADDRESS   0xFFFF

//
// Lookup table for increment values based on transfer widths
//
UINT8 mInStride[] = {
  1, // UsraWidth8
  2, // UsraWidth16
  4, // UsraWidth32
  8, // UsraWidth64
  0, // UsraWidthFifo8
  0, // UsraWidthFifo16
  0, // UsraWidthFifo32
  0, // UsraWidthFifo64
  1, // UsraWidthFill8
  2, // UsraWidthFill16
  4, // UsraWidthFill32
  8  // UsraWidthFill64
};

//
// Lookup table for increment values based on transfer widths
//
UINT8 mOutStride[] = {
  1, // UsraWidth8
  2, // UsraWidth16
  4, // UsraWidth32
  8, // UsraWidth64
  1, // UsraWidthFifo8
  2, // UsraWidthFifo16
  4, // UsraWidthFifo32
  8, // UsraWidthFifo64
  0, // UsraWidthFill8
  0, // UsraWidthFill16
  0, // UsraWidthFill32
  0  // UsraWidthFill64
};


/**
  This API gets the Pcie address from the given USRA Address.

  @param[in] Global               Global pointer
  @param[in] Virtual              Virtual address
  @param[in] Address              A pointer of the address of the USRA Address Structure
  @param[out] AlignedAddress      A pointer of aligned address converted from USRA address

  @retval NONE
**/
VOID
GetPcieAccessAddress (
  IN VOID                     *Global,
  IN BOOLEAN                  Virtual,
  IN USRA_ADDRESS             *Address,
  OUT UINTN                   *AlignedAddress
  )
{
  INTN                        MmCfgBase;

  MmCfgBase = GetPcieSegMmcfgBaseAddress(Address);
  // TODO: add Error Check for NULL later
  *AlignedAddress = MmCfgBase + (UINTN)(Address->Attribute.RawData32[0] & 0x0fffffff);
}

/**
  This API performs 8-bit, 16-bit, 32-bit or 64-bit Pcie silicon register read operations.
  It transfers data from a register into a naturally aligned data buffer.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be read out
  @param[in] Buffer               A pointer of buffer for the value read from the register

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
PcieRegisterRead (
  IN USRA_ADDRESS             *Address,
  IN VOID                     *Buffer
  )
{
  UINTN                       AlignedAddress;

  GetPcieAccessAddress (NULL, 0, Address, &AlignedAddress);
  UsraRegAlignedRead((UINT32)Address->Attribute.AccessWidth, AlignedAddress, Buffer);

  return RETURN_SUCCESS;
}

/**
  Check parameters to PcieBlkRegisterRead() function request.

  The I/O operations are carried out exactly as requested. The caller is responsible
  for satisfying any alignment and I/O width restrictions that a PI System on a
  platform might require. For example on some platforms, width requests of
  UsraWidth64 do not work. Misaligned buffers, on the other hand, will
  be handled by the driver.

  @param[in] MmioOperation  TRUE for an MMIO operation, FALSE for I/O Port operation.
  @param[in] Width          Signifies the width of the I/O or Memory operation.
  @param[in] Address        The base address of the I/O operation.
  @param[in] Count          The number of I/O operations to perform. The number of
                            bytes moved is Width size * Count, starting at Address.
  @param[in] Buffer         For read operations, the destination buffer to store the results.
                            For write operations, the source buffer from which to write data.

  @retval EFI_SUCCESS            The parameters for this request pass the checks.
  @retval EFI_INVALID_PARAMETER  Buffer is NULL.
  @retval EFI_INVALID_PARAMETER  Width is invalid for this PI system.
  @retval EFI_UNSUPPORTED        The Buffer is not aligned for the given Width.
  @retval EFI_UNSUPPORTED        The address range specified by Address, Width,
                                 and Count is not valid for this PI system.

**/
STATIC
RETURN_STATUS
CpuIoCheckParameter (
  IN BOOLEAN                    MmioOperation,
  IN USRA_ACCESS_WIDTH          Width,
  IN UINT64                     Address,
  IN UINTN                      Count,
  IN VOID                       *Buffer
  )
{
  UINT64  MaxCount;
  UINT64  Limit;

  //
  // Check to see if Buffer is NULL
  //
  if (Buffer == NULL) {
    return RETURN_INVALID_PARAMETER;
  }

  //
  // Check to see if Width is in the valid range
  //
  if ((UINT32)Width >= UsraWidthMaximum) {
    return RETURN_INVALID_PARAMETER;
  }

  //
  // For FIFO type, the target address won't increase during the access,
  // so treat Count as 1
  //
  if (Width >= UsraWidthFifo8 && Width <= UsraWidthFifo64) {
    Count = 1;
  }

  //
  // Check to see if Width is in the valid range for I/O Port operations
  //
  Width = (USRA_ACCESS_WIDTH) (Width & 0x03);
  if (!MmioOperation && (Width == UsraWidth64)) {
    return RETURN_INVALID_PARAMETER;
  }

  //
  // Check to see if Address is aligned
  //
  if ((Address & (UINT64)(mInStride[Width] - 1)) != 0) {
    return RETURN_UNSUPPORTED;
  }

  //
  // Check to see if any address associated with this transfer exceeds the maximum
  // allowed address.  The maximum address implied by the parameters passed in is
  // Address + Size * Count.  If the following condition is met, then the transfer
  // is not supported.
  //
  //    Address + Size * Count > (MmioOperation ? MAX_ADDRESS : MAX_IO_PORT_ADDRESS) + 1
  //
  // Since MAX_ADDRESS can be the maximum integer value supported by the CPU and Count
  // can also be the maximum integer value supported by the CPU, this range
  // check must be adjusted to avoid all oveflow conditions.
  //
  // The following form of the range check is equivalent but assumes that
  // MAX_ADDRESS and MAX_IO_PORT_ADDRESS are of the form (2^n - 1).
  //
  Limit = (MmioOperation ? MAX_ADDRESS : MAX_IO_PORT_ADDRESS);
  if (Count == 0) {
    if (Address > Limit) {
      return RETURN_UNSUPPORTED;
    }
  } else {
    MaxCount = RShiftU64 (Limit, Width);
    if (MaxCount < (Count - 1)) {
      return RETURN_UNSUPPORTED;
    }
    if (Address > LShiftU64 (MaxCount - Count + 1, Width)) {
      return RETURN_UNSUPPORTED;
    }
  }

  //
  // Check to see if Buffer is aligned
  // (IA-32 allows UINT64 and INT64 data types to be 32-bit aligned.)
  //
  if (((UINTN)Buffer & ((MIN (sizeof (UINTN), mInStride[Width])  - 1))) != 0) {
    return RETURN_UNSUPPORTED;
  }

  return RETURN_SUCCESS;
}

/**
  This API performs 8-bit, 16-bit, 32-bit or 64-bit Pcie block silicon register read operations.
  It transfers data from a register into a naturally aligned data buffer.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be read out
  @param[in] Buffer               A pointer of buffer for the value read from the register

  @retval RETURN_SUCCESS          The function completed successfully.
  @retval Others                  Some error occurs when executing CpuIoCheckParameter function.
**/
RETURN_STATUS
PcieBlkRegisterRead (
  IN USRA_ADDRESS             *Address,
  IN VOID                     *Buffer
  )
{
  UINT8                       InStride;
  UINT8                       OutStride;
  RETURN_STATUS               Status;
  UINTN                       AlignedAddress;
  UINT32                      ReadCount = Address->PcieBlk.Count;
  UINT8                       *UINT8Buffer;

  GetPcieAccessAddress (NULL, 0, Address, &AlignedAddress);
  Status = CpuIoCheckParameter (TRUE, Address->Attribute.AccessWidth, AlignedAddress, ReadCount, Buffer);
  if (RETURN_ERROR (Status)) {
    return Status;
  }

  InStride = mInStride[Address->Attribute.AccessWidth];
  OutStride = mOutStride[Address->Attribute.AccessWidth];
  for (UINT8Buffer = Buffer; ReadCount > 0; AlignedAddress += InStride, UINT8Buffer += OutStride, ReadCount--) {
    UsraRegAlignedRead((USRA_ACCESS_WIDTH) (Address->Attribute.AccessWidth & 0x03), AlignedAddress, (VOID *)UINT8Buffer);
  }

  return RETURN_SUCCESS;
}

/**
  This API performs 8-bit, 16-bit, 32-bit or 64-bit Pcie silicon register write operations.
  It transfers data from a naturally aligned data buffer into a register.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be written
  @param[in] Buffer               A pointer of buffer for the value write to the register

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
PcieRegisterWrite (
  IN USRA_ADDRESS             *Address,
  OUT VOID                    *Buffer
  )
{
  UINTN                       AlignedAddress;

  GetPcieAccessAddress(NULL, 0, Address, &AlignedAddress);
  UsraRegAlignedWrite((UINT32)Address->Attribute.AccessWidth, AlignedAddress, Buffer);

  if (FeaturePcdGet (PcdUsraSupportS3))
  {
    if(Address->Attribute.S3Enable)
    {
      S3BootScriptSaveMemWrite ((S3_BOOT_SCRIPT_LIB_WIDTH)Address->Attribute.AccessWidth, (UINT64)AlignedAddress, 1, Buffer);
    }
  }

  return RETURN_SUCCESS;
}

/**
  This API performs 8-bit, 16-bit, 32-bit or 64-bit Pcie block silicon register write operations.
  It transfers data from a naturally aligned data buffer into a register.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be written
  @param[in] Buffer               A pointer of buffer for the value write to the register

  @retval RETURN_SUCCESS          The function completed successfully.
  @retval Others                  Some error occurs when executing CpuIoCheckParameter function.
**/
RETURN_STATUS
PcieBlkRegisterWrite (
  IN USRA_ADDRESS             *Address,
  OUT VOID                    *Buffer
  )
{
  UINT8                       InStride;
  UINT8                       OutStride;
  RETURN_STATUS               Status;
  UINTN                       AlignedAddress;
  UINT32                      WriteCount = Address->PcieBlk.Count;
  UINT8                       *UINT8Buffer;

  GetPcieAccessAddress (NULL, 0, Address, &AlignedAddress);
  Status = CpuIoCheckParameter (TRUE, Address->Attribute.AccessWidth, AlignedAddress, WriteCount, Buffer);
  if (RETURN_ERROR (Status)) {
    return Status;
  }

  InStride = mInStride[Address->Attribute.AccessWidth];
  OutStride = mOutStride[Address->Attribute.AccessWidth];
  for (UINT8Buffer = Buffer; WriteCount > 0; AlignedAddress += InStride, UINT8Buffer += OutStride, WriteCount--) {
    UsraRegAlignedWrite((USRA_ACCESS_WIDTH) (Address->Attribute.AccessWidth & 0x03), AlignedAddress, (VOID *)UINT8Buffer);

    if (FeaturePcdGet (PcdUsraSupportS3)) {
      if(Address->Attribute.S3Enable) {
        S3BootScriptSaveMemWrite ((S3_BOOT_SCRIPT_LIB_WIDTH)(Address->Attribute.AccessWidth & 0x03), (UINT64)AlignedAddress, 1, (VOID *)UINT8Buffer);
      }
    }
  }

  return RETURN_SUCCESS;
}

/**
  This API performs 8-bit, 16-bit, 32-bit or 64-bit Pcie silicon register AND then OR operations. It read data from a
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
  )
{
  UINT64                      Data;
  UINT8                       WidthTable[] = {1,2,4,8};

  PcieRegisterRead(Address, &Data);
  DataAndOr (&Data, AndBuffer, OrBuffer, WidthTable[(UINT8)Address->Attribute.AccessWidth]);
  PcieRegisterWrite(Address, &Data);

  return RETURN_SUCCESS;
}

