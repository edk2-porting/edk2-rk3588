/** @file
  CPU REGBAR ACCESS library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/CpuRegbarAccessLib.h>

/**
  Definition for REGBAR address
  The REGBAR address is used for the CPU IP's SB register access
**/
#define CPU_REGBAR_ADDRESS(Pid, Offset)    (PcdGet32(PcdRegBarBaseAddress) | ((UINT8)(Pid) << 16) | (UINT16)(Offset))

/**
  Read REGBAR register.
  It returns REGBAR register and size in 8bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid   CPU SB Device Port ID
  @param[in]  Offset           Register offset of this Port ID

  @retval     UINT64           REGBAR register value.
**/
UINT64
CpuRegbarRead64 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset
  )
{
  UINT8    Pid;

  Pid = CpuSbDevicePid;
  if (Pid != INVALID_PID)
    return ((UINT64) MmioRead32 (CPU_REGBAR_ADDRESS (Pid, Offset)) + LShiftU64 ((UINT64) MmioRead32 (CPU_REGBAR_ADDRESS (Pid, Offset+4)), 32));
  else
    return INVALID_DATA_64;
}


/**
  Read REGBAR register.
  It returns REGBAR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid   CPU SB Device Port ID
  @param[in]  Offset              Register offset of this Port ID

  @retval     UINT32           REGBAR register value.
**/
UINT32
CpuRegbarRead32 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset
  )
{
  UINT8    Pid;

  Pid = CpuSbDevicePid;
  if (Pid != INVALID_PID)
    return MmioRead32 (CPU_REGBAR_ADDRESS (Pid, Offset));
  else
    return INVALID_DATA_32;
}

/**
  Read REGBAR register.
  It returns REGBAR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid   CPU SB Device Port ID
  @param[in]  Offset           Register offset of this Port ID

  @retval     UINT16           REGBAR register value.
**/
UINT16
CpuRegbarRead16 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset
  )
{
  UINT16   DwOffset;
  UINT32   Data32;
  UINT16   Data16;

  Data16 = 0;
  Data32 = 0;
  DwOffset = 0;

  if (CpuSbDevicePid == INVALID_PID) {
    return INVALID_DATA_16;
  }
  switch (Offset & 0x0003) {
    case 0:
      DwOffset = Offset;
      break;
    case 2:
      DwOffset = Offset - 0x2;
      break;
  }
  Data32 = MmioRead32 (CPU_REGBAR_ADDRESS (CpuSbDevicePid, DwOffset));
  switch (Offset & 0x0003) {
    case 0:
      Data16 = (UINT16) Data32;
      break;
    case 2:
      Data16 = (UINT16) (Data32 >> 16);
      break;
  }
  return Data16;
}

/**
  Read REGBAR register.
  It returns REGBAR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid   CPU SB Device Port ID
  @param[in]  Offset           Register offset of this Port ID

  @retval     UINT8            REGBAR regsiter value
**/
UINT8
CpuRegbarRead8 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset
  )
{
  UINT16   DwOffset;
  UINT32   Data32;
  UINT8    Data8;

  DwOffset = 0;
  Data32 = 0;
  Data8 = 0;

  if (CpuSbDevicePid == INVALID_PID)
    return INVALID_DATA_8;
  switch (Offset & 0x0003) {
    case 0:
      DwOffset = Offset;
      break;
    case 1:
      DwOffset = Offset - 0x1;
      break;
    case 2:
      DwOffset = Offset - 0x2;
      break;
    case 3:
      DwOffset = Offset - 0x3;
      break;
  }
  Data32 = MmioRead32 (CPU_REGBAR_ADDRESS (CpuSbDevicePid, DwOffset));
  switch (Offset & 0x0003) {
    case 0:
      Data8 = (UINT8) Data32;
      break;
    case 1:
      Data8 = (UINT8) (Data32 >> 8);
      break;
    case 2:
      Data8 = (UINT8) (Data32 >> 16);
      break;
    case 3:
      Data8 = (UINT8) (Data32 >> 24);
      break;
  }
  return Data8;
}


/**
  Write REGBAR register.
  It programs REGBAR register and size in 8bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid   CPU SB Device Port ID
  @param[in]  Offset           Register offset of Port ID.
  @param[in]  Data             Input Data. Must be the same size as Size parameter.

  @retval     UINT64           Value written to register
**/
UINT64
CpuRegbarWrite64 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset,
  IN  UINT64                            Data
  )
{
  UINT8       Pid;

  Pid = CpuSbDevicePid;
  if (Pid != INVALID_PID) {
    MmioWrite32 (CPU_REGBAR_ADDRESS (Pid, Offset) + 4, (UINT32) RShiftU64 (Data, 32));
    MmioWrite32 (CPU_REGBAR_ADDRESS (Pid, Offset), (UINT32) Data);
    return Data;
  }
  else
    return INVALID_DATA_64;
}


/**
  Write REGBAR register.
  It programs REGBAR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid   CPU SB Device Port ID
  @param[in]  Offset           Register offset of Port ID.
  @param[in]  Data             Input Data. Must be the same size as Size parameter.

  @retval     UINT32           Value written to register
**/
UINT32
CpuRegbarWrite32 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset,
  IN  UINT32                            Data
  )
{
  UINT8    Pid;

  Pid = CpuSbDevicePid;
  if (Pid != INVALID_PID)
    return MmioWrite32 (CPU_REGBAR_ADDRESS (Pid, Offset), Data);
  else
    return INVALID_DATA_32;
}

/**
  Write REGBAR register.
  It programs REGBAR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid   CPU SB Device Port ID
  @param[in]  Offset           Register offset of Port ID.
  @param[in]  Data             Input Data. Must be the same size as Size parameter.

  @retval     UINT16           Value written to register
**/
UINT16
CpuRegbarWrite16 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset,
  IN  UINT16                            Data
  )
{
  UINT8    Pid;

  Pid = CpuSbDevicePid;
  if (Pid != INVALID_PID)
    return MmioWrite16 (CPU_REGBAR_ADDRESS (Pid, Offset), Data);
  else
    return INVALID_DATA_16;
}

/**
  Write REGBAR register.
  It programs REGBAR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid   CPU SB Device Port ID
  @param[in]  Offset           Register offset of Port ID.
  @param[in]  Data             Input Data. Must be the same size as Size parameter.

  @retval     UINT8            Value written to register
**/
UINT8
CpuRegbarWrite8 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset,
  IN  UINT8                             Data
  )
{
  UINT8    Pid;

  Pid = CpuSbDevicePid;
  if (Pid != INVALID_PID)
    return MmioWrite8 (CPU_REGBAR_ADDRESS (Pid, Offset), Data);
  else
    return INVALID_DATA_8;
}

/**
  Write REGBAR register.
  It programs REGBAR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid   CPU SB Device Port ID
  @param[in]  Offset           Register offset of Port ID.
  @param[in]  OrData           OR Data. Must be the same size as Size parameter.

  @retval     UINT32           Value written to register

**/
UINT32
CpuRegbarOr32 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset,
  IN  UINT32                            OrData
  )
{
  return CpuRegbarWrite32 (CpuSbDevicePid, Offset, CpuRegbarRead32(CpuSbDevicePid, Offset) | OrData);
}

/**
  Write REGBAR register.
  It programs REGBAR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid   CPU SB Device Port ID
  @param[in]  Offset           Register offset of Port ID.
  @param[in]  OrData           OR Data. Must be the same size as Size parameter.

  @retval     UINT16           Value written to register

**/
UINT16
CpuRegbarOr16 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset,
  IN  UINT16                            OrData
  )
{
  return CpuRegbarWrite16 (CpuSbDevicePid, Offset, CpuRegbarRead16(CpuSbDevicePid, Offset) | OrData);
}

/**
  Write REGBAR register.
  It programs REGBAR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid   CPU SB Device Port ID
  @param[in]  Offset           Register offset of Port ID.
  @param[in]  OrData           OR Data. Must be the same size as Size parameter.

  @retval     UINT8            Value written to register

**/
UINT8
CpuRegbarOr8(
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset,
  IN  UINT8                             OrData
  )
{
  return CpuRegbarWrite8 (CpuSbDevicePid, Offset, CpuRegbarRead8(CpuSbDevicePid, Offset) | OrData);
}

/**
  Performs a bitwise AND of a 32-bit data.
  It programs REGBAR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevice      CPU SB Device
  @param[in]  Offset           Register offset of Port ID.
  @param[in]  AndData          And Data. Must be the same size as Size parameter.

  @retval     UINT32           Value written to register

**/
UINT32
CpuRegbarAnd32 (
  IN  CPU_SB_DEVICE_PID                     CpuSbDevicePid,
  IN  UINT16                            Offset,
  IN  UINT32                            AndData
  )
{
  return CpuRegbarWrite32 (CpuSbDevicePid, Offset, CpuRegbarRead32 (CpuSbDevicePid, Offset) & AndData);
}

/**
  Performs a bitwise AND of a 16-bit data.
  It programs REGBAR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevice      CPU SB Device
  @param[in]  Offset           Register offset of Port ID.
  @param[in]  AndData          And Data. Must be the same size as Size parameter.

  @retval     UINT16           Value written to register

**/
UINT16
CpuRegbarAnd16 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset,
  IN  UINT16                            AndData
  )
{
  return CpuRegbarWrite16 (CpuSbDevicePid, Offset, CpuRegbarRead16 (CpuSbDevicePid, Offset) & AndData);
}

/**
Performs a bitwise AND of a 8-bit data.
It programs REGBAR register and size in 1byte.
The Offset should not exceed 0xFFFF and must be aligned with size.

@param[in]  CpuSbDevice      CPU SB Device
@param[in]  Offset           Register offset of Port ID.
@param[in]  AndData          And Data. Must be the same size as Size parameter.

@retval     UINT8           Value written to register

**/
UINT8
CpuRegbarAnd8 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset,
  IN  UINT8                             AndData
  )
{
  return CpuRegbarWrite8 (CpuSbDevicePid, Offset, CpuRegbarRead8 (CpuSbDevicePid, Offset) & AndData);
}

/**
  Write REGBAR register.
  It programs REGBAR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid   CPU SB Device Port ID
  @param[in]  Offset           Register offset of Port ID.
  @param[in]  AndData          AND Data. Must be the same size as Size parameter.
  @param[in]  OrData           OR Data. Must be the same size as Size parameter.

  @retval     UINT32           Value written to register

**/
UINT32
CpuRegbarAndThenOr32 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset,
  IN  UINT32                            AndData,
  IN  UINT32                            OrData
  )
{
  return CpuRegbarWrite32 (CpuSbDevicePid, Offset, (CpuRegbarRead32 (CpuSbDevicePid, Offset) & AndData) | OrData);
}

/**
  Write REGBAR register.
  It programs REGBAR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid   CPU SB Device Port ID
  @param[in]  Offset           Register offset of Port ID.
  @param[in]  AndData          AND Data. Must be the same size as Size parameter.
  @param[in]  OrData           OR Data. Must be the same size as Size parameter.

  @retval     UINT16           Value written to register

**/
UINT16
CpuRegbarAndThenOr16 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset,
  IN  UINT16                            AndData,
  IN  UINT16                            OrData
  )
{
  return CpuRegbarWrite16 (CpuSbDevicePid, Offset, (CpuRegbarRead16 (CpuSbDevicePid, Offset) & AndData) | OrData);
}

/**
  Write REGBAR register.
  It programs REGBAR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid   CPU SB Device Port ID
  @param[in]  Offset           Register offset of Port ID.
  @param[in]  AndData          AND Data. Must be the same size as Size parameter.
  @param[in]  OrData           OR Data. Must be the same size as Size parameter.

  @retval     UINT8            Value written to register

**/
UINT8
CpuRegbarAndThenOr8 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset,
  IN  UINT8                             AndData,
  IN  UINT8                             OrData
  )
{
  return CpuRegbarWrite8 (CpuSbDevicePid, Offset, (CpuRegbarRead8 (CpuSbDevicePid, Offset) & AndData) | OrData);
}
