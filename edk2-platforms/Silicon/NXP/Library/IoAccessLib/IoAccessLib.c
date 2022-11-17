/** IoAccessLib.c

  Provide MMIO APIs for BE modules.

  Copyright 2017-2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/IoAccessLib.h>
#include <Library/IoLib.h>

/**
  MmioRead16 for Big-Endian modules.

  @param  Address The MMIO register to read.

  @return The value read.

**/
STATIC
UINT16
EFIAPI
SwapMmioRead16 (
  IN  UINTN     Address
  )
{
  return SwapBytes16 (MmioRead16 (Address));
}

/**
  MmioRead32 for Big-Endian modules.

  @param  Address The MMIO register to read.

  @return The value read.

**/
STATIC
UINT32
EFIAPI
SwapMmioRead32 (
  IN  UINTN     Address
  )
{
  return SwapBytes32 (MmioRead32 (Address));
}

/**
  MmioRead64 for Big-Endian modules.

  @param  Address The MMIO register to read.

  @return The value read.

**/
STATIC
UINT64
EFIAPI
SwapMmioRead64 (
  IN  UINTN     Address
  )
{
  return SwapBytes64 (MmioRead64 (Address));
}

/**
  MmioWrite16 for Big-Endian modules.

  @param  Address The MMIO register to write.
  @param  Value   The value to write to the MMIO register.

**/
STATIC
UINT16
EFIAPI
SwapMmioWrite16 (
  IN  UINTN     Address,
  IN  UINT16    Value
  )
{
  return MmioWrite16 (Address, SwapBytes16 (Value));
}

/**
  MmioWrite32 for Big-Endian modules.

  @param  Address The MMIO register to write.
  @param  Value   The value to write to the MMIO register.

**/
STATIC
UINT32
EFIAPI
SwapMmioWrite32 (
  IN  UINTN     Address,
  IN  UINT32    Value
  )
{
  return MmioWrite32 (Address, SwapBytes32 (Value));
}

/**
  MmioWrite64 for Big-Endian modules.

  @param  Address The MMIO register to write.
  @param  Value   The value to write to the MMIO register.

**/
STATIC
UINT64
EFIAPI
SwapMmioWrite64 (
  IN  UINTN     Address,
  IN  UINT64    Value
  )
{
  return MmioWrite64 (Address, SwapBytes64 (Value));
}

/**
  MmioAndThenOr16 for Big-Endian modules.

  @param  Address The MMIO register to write.
  @param  AndData The value to AND with the read value from the MMIO register.
  @param  OrData  The value to OR with the result of the AND operation.

  @return The value written back to the MMIO register.

**/
STATIC
UINT16
EFIAPI
SwapMmioAndThenOr16 (
  IN  UINTN     Address,
  IN  UINT16    AndData,
  IN  UINT16    OrData
  )
{
  AndData = SwapBytes16 (AndData);
  OrData = SwapBytes16 (OrData);

  return MmioAndThenOr16 (Address, AndData, OrData);
}

/**
  MmioAndThenOr32 for Big-Endian modules.

  @param  Address The MMIO register to write.
  @param  AndData The value to AND with the read value from the MMIO register.
  @param  OrData  The value to OR with the result of the AND operation.

  @return The value written back to the MMIO register.

**/
STATIC
UINT32
EFIAPI
SwapMmioAndThenOr32 (
  IN  UINTN     Address,
  IN  UINT32    AndData,
  IN  UINT32    OrData
  )
{
  AndData = SwapBytes32 (AndData);
  OrData = SwapBytes32 (OrData);

  return MmioAndThenOr32 (Address, AndData, OrData);
}

/**
  MmioAndThenOr64 for Big-Endian modules.

  @param  Address The MMIO register to write.
  @param  AndData The value to AND with the read value from the MMIO register.
  @param  OrData  The value to OR with the result of the AND operation.

  @return The value written back to the MMIO register.

**/
STATIC
UINT64
EFIAPI
SwapMmioAndThenOr64 (
  IN  UINTN     Address,
  IN  UINT64    AndData,
  IN  UINT64    OrData
  )
{
  AndData = SwapBytes64 (AndData);
  OrData = SwapBytes64 (OrData);

  return MmioAndThenOr64 (Address, AndData, OrData);
}

/**
  MmioOr16 for Big-Endian modules.

  @param  Address The MMIO register to write.
  @param  OrData  The value to OR with the read value from the MMIO register.

  @return The value written back to the MMIO register.

**/
STATIC
UINT16
EFIAPI
SwapMmioOr16 (
  IN  UINTN     Address,
  IN  UINT16    OrData
  )
{
  return MmioOr16 (Address, SwapBytes16 (OrData));
}

/**
  MmioOr32 for Big-Endian modules.

  @param  Address The MMIO register to write.
  @param  OrData  The value to OR with the read value from the MMIO register.

  @return The value written back to the MMIO register.

**/
STATIC
UINT32
EFIAPI
SwapMmioOr32 (
  IN  UINTN     Address,
  IN  UINT32    OrData
  )
{
  return MmioOr32 (Address, SwapBytes32 (OrData));
}

/**
  MmioOr64 for Big-Endian modules.

  @param  Address The MMIO register to write.
  @param  OrData  The value to OR with the read value from the MMIO register.

  @return The value written back to the MMIO register.

**/
STATIC
UINT64
EFIAPI
SwapMmioOr64 (
  IN  UINTN     Address,
  IN  UINT64    OrData
  )
{
  return MmioOr64 (Address, SwapBytes64 (OrData));
}

/**
  MmioAnd16 for Big-Endian modules.

  @param  Address The MMIO register to write.
  @param  AndData The value to AND with the read value from the MMIO register.

  @return The value written back to the MMIO register.

**/
STATIC
UINT16
EFIAPI
SwapMmioAnd16 (
  IN  UINTN     Address,
  IN  UINT16    AndData
  )
{
  return MmioAnd16 (Address, SwapBytes16 (AndData));
}

/**
  MmioAnd32 for Big-Endian modules.

  @param  Address The MMIO register to write.
  @param  AndData The value to AND with the read value from the MMIO register.

  @return The value written back to the MMIO register.

**/
STATIC
UINT32
EFIAPI
SwapMmioAnd32 (
  IN  UINTN     Address,
  IN  UINT32    AndData
  )
{
  return MmioAnd32 (Address, SwapBytes32 (AndData));
}

/**
  MmioAnd64 for Big-Endian modules.

  @param  Address The MMIO register to write.
  @param  AndData The value to AND with the read value from the MMIO register.

  @return The value written back to the MMIO register.

**/
STATIC
UINT64
EFIAPI
SwapMmioAnd64 (
  IN  UINTN     Address,
  IN  UINT64    AndData
  )
{
  return MmioAnd64 (Address, SwapBytes64 (AndData));
}

STATIC MMIO_OPERATIONS SwappingFunctions = {
  SwapMmioRead16,
  SwapMmioWrite16,
  SwapMmioOr16,
  SwapMmioAnd16,
  SwapMmioAndThenOr16,
  SwapMmioRead32,
  SwapMmioWrite32,
  SwapMmioOr32,
  SwapMmioAnd32,
  SwapMmioAndThenOr32,
  SwapMmioRead64,
  SwapMmioWrite64,
  SwapMmioOr64,
  SwapMmioAnd64,
  SwapMmioAndThenOr64,
};

STATIC MMIO_OPERATIONS NonSwappingFunctions = {
  MmioRead16,
  MmioWrite16,
  MmioOr16,
  MmioAnd16,
  MmioAndThenOr16,
  MmioRead32,
  MmioWrite32,
  MmioOr32,
  MmioAnd32,
  MmioAndThenOr32,
  MmioRead64,
  MmioWrite64,
  MmioOr64,
  MmioAnd64,
  MmioAndThenOr64,
};

/**
  Function to return pointer to Mmio operations.

  @param  Swap  Flag to tell if Swap is needed or not
                on Mmio Operations.

  @return       Pointer to Mmio Operations.

**/
MMIO_OPERATIONS *
GetMmioOperations (BOOLEAN Swap) {
  if (Swap) {
    return &SwappingFunctions;
  } else {
    return &NonSwappingFunctions;
  }
}
