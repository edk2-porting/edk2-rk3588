/** @file
  PCH PCR library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PchPcrLib.h>
#include <Register/PchRegsPcr.h>

#ifndef MDEPKG_NDEBUG
/**
  Checks if the offset is valid for a given memory access width

  @param[in]  Offset  Offset of a register
  @param[in]  Size    Size of memory access in bytes

  @retval FALSE  Offset is not valid for a given memory access
  @retval TRUE   Offset is valid
**/
STATIC
BOOLEAN
PchIsPcrOffsetValid (
  IN UINT32  Offset,
  IN UINTN   Size
  )
{
  if (((Offset & (Size - 1)) != 0) || (Offset > 0xFFFF)) {
    DEBUG ((DEBUG_ERROR, "PCR offset error. Invalid Offset: %x Size: %x", Offset, Size));
    return FALSE;
  } else {
    return TRUE;
  }
}
#endif

/**
  Read PCR register.
  It returns PCR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of this Port ID

  @retval UINT32       PCR register value.
**/
UINT32
PchPcrRead32 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT32                            Offset
  )
{
#ifndef MDEPKG_NDEBUG
  ASSERT (PchIsPcrOffsetValid (Offset, 4));
#endif
  return MmioRead32 (PCH_PCR_ADDRESS (Pid, Offset));
}

/**
  Read PCR register.
  It returns PCR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of this Port ID

  @retval UINT16       PCR register value.
**/
UINT16
PchPcrRead16 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT32                            Offset
  )
{
#ifndef MDEPKG_NDEBUG
  ASSERT (PchIsPcrOffsetValid (Offset, 2));
#endif
  return MmioRead16 (PCH_PCR_ADDRESS (Pid, Offset));
}

/**
  Read PCR register.
  It returns PCR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of this Port ID

  @retval UINT8        PCR register value
**/
UINT8
PchPcrRead8 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT32                            Offset
  )
{
  return MmioRead8 (PCH_PCR_ADDRESS (Pid, Offset));
}

/**
  Write PCR register.
  It programs PCR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  Data     Input Data. Must be the same size as Size parameter.

  @retval UINT32       Value written to register
**/
UINT32
PchPcrWrite32 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT32                            Offset,
  IN  UINT32                            Data
  )
{
#ifndef MDEPKG_NDEBUG
  ASSERT (PchIsPcrOffsetValid (Offset, 4));
#endif
  MmioWrite32 (PCH_PCR_ADDRESS (Pid, Offset), Data);

  return Data;
}

/**
  Write PCR register.
  It programs PCR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  Data     Input Data. Must be the same size as Size parameter.

  @retval  UINT16      Value written to register
**/
UINT16
PchPcrWrite16 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT32                            Offset,
  IN  UINT16                            Data
  )
{
#ifndef MDEPKG_NDEBUG
  ASSERT (PchIsPcrOffsetValid (Offset, 2));
#endif
  MmioWrite16 (PCH_PCR_ADDRESS (Pid, Offset), Data);

  return Data;
}

/**
  Write PCR register.
  It programs PCR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  Data     Input Data. Must be the same size as Size parameter.

  @retval  UINT8       Value written to register
**/
UINT8
PchPcrWrite8 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT32                            Offset,
  IN  UINT8                             Data
  )
{
  MmioWrite8 (PCH_PCR_ADDRESS (Pid, Offset), Data);

  return Data;
}

/**
  Write PCR register.
  It programs PCR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  AndData  AND Data. Must be the same size as Size parameter.
  @param[in]  OrData   OR Data. Must be the same size as Size parameter.

  @retval  UINT32      Value written to register

**/
UINT32
PchPcrAndThenOr32 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT32                            Offset,
  IN  UINT32                            AndData,
  IN  UINT32                            OrData
  )
{
  return PchPcrWrite32 (Pid, Offset, (PchPcrRead32 (Pid, Offset) & AndData) | OrData);
}

/**
  Write PCR register and read back.
  The read back ensures the PCR cycle is completed before next operation.
  It programs PCR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  AndData  AND Data. Must be the same size as Size parameter.
  @param[in]  OrData   OR Data. Must be the same size as Size parameter.

  @retval  UINT32      Value read back from the register
**/
UINT32
PchPcrAndThenOr32WithReadback (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT32                            Offset,
  IN  UINT32                            AndData,
  IN  UINT32                            OrData
  )
{
  PchPcrWrite32 (Pid, Offset, (PchPcrRead32 (Pid, Offset) & AndData) | OrData);
  return PchPcrRead32 (Pid, Offset);
}

/**
  Write PCR register.
  It programs PCR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  AndData  AND Data. Must be the same size as Size parameter.
  @param[in]  OrData   OR Data. Must be the same size as Size parameter.

  @retval UINT16       Value written to register

**/
UINT16
PchPcrAndThenOr16 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT32                            Offset,
  IN  UINT16                            AndData,
  IN  UINT16                            OrData
  )
{
  return PchPcrWrite16 (Pid, Offset, (PchPcrRead16 (Pid, Offset) & AndData) | OrData);
}

/**
  Write PCR register.
  It programs PCR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  AndData  AND Data. Must be the same size as Size parameter.
  @param[in]  OrData   OR Data. Must be the same size as Size parameter.

  @retval  UINT8       Value written to register

**/
UINT8
PchPcrAndThenOr8 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT32                            Offset,
  IN  UINT8                             AndData,
  IN  UINT8                             OrData
  )
{
  return PchPcrWrite8 (Pid, Offset, (PchPcrRead8 (Pid, Offset) & AndData) | OrData);
}

