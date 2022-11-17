/** @file
  Header file for CPU REGBAR ACCESS library.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _CPU_REGBAR_ACCESS_LIB_H_
#define _CPU_REGBAR_ACCESS_LIB_H_

#define INVALID_DATA_64  0xFFFFFFFFFFFFFFFF
#define INVALID_DATA_32  0xFFFFFFFF
#define INVALID_DATA_16  0xFFFF
#define INVALID_DATA_8   0xFF
#define INVALID_PID      0xFF

typedef UINT8    CPU_SB_DEVICE_PID;

/**
  Read REGBAR register.
  It returns REGBAR register and size in 8bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  Offset              Register offset of this Port ID

  @retval     UINT64           REGBAR register value.
**/
UINT64
CpuRegbarRead64 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset
  );


/**
  Read REGBAR register.
  It returns REGBAR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  Offset              Register offset of this Port ID

  @retval     UINT32           REGBAR register value.
**/
UINT32
CpuRegbarRead32 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset
  );

/**
  Read REGBAR register.
  It returns REGBAR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  Offset              Register offset of this Port ID

  @retval     UINT16           REGBAR register value.
**/
UINT16
CpuRegbarRead16 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset
  );

/**
  Read REGBAR register.
  It returns REGBAR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  Offset              Register offset of this Port ID

  @retval     UINT8            REGBAR regsiter value
**/
UINT8
CpuRegbarRead8 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset
  );

/**
  Write REGBAR register.
  It programs REGBAR register and size in 8bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  Offset              Register offset of Port ID.
  @param[in]  Data             Input Data. Must be the same size as Size parameter.

  @retval     UINT64           Value written to register
**/
UINT64
CpuRegbarWrite64 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset,
  IN  UINT64                            Data
  );


/**
  Write REGBAR register.
  It programs REGBAR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  Offset           Register offset of Port ID.
  @param[in]  Data             Input Data. Must be the same size as Size parameter.

  @retval     UINT32           Value written to register
**/
UINT32
CpuRegbarWrite32 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset,
  IN  UINT32                            Data
  );

/**
  Write REGBAR register.
  It programs REGBAR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  Offset           Register offset of Port ID.
  @param[in]  Data             Input Data. Must be the same size as Size parameter.

  @retval     UINT16           Value written to register
**/
UINT16
CpuRegbarWrite16 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset,
  IN  UINT16                            Data
  );

/**
  Write REGBAR register.
  It programs REGBAR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  Offset           Register offset of Port ID.
  @param[in]  Data             Input Data. Must be the same size as Size parameter.

  @retval     UINT8            Value written to register
**/
UINT8
CpuRegbarWrite8 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset,
  IN  UINT8                             Data
  );

/**
  Write REGBAR register.
  It programs REGBAR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  Offset           Register offset of Port ID.
  @param[in]  OrData           OR Data. Must be the same size as Size parameter.

  @retval     UINT32           Value written to register

**/
UINT32
CpuRegbarOr32 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset,
  IN  UINT32                            OrData
  );

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
  );

/**
  Write REGBAR register.
  It programs REGBAR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
  @param[in]  Offset           Register offset of Port ID.
  @param[in]  OrData           OR Data. Must be the same size as Size parameter.

  @retval     UINT8            Value written to register

**/
UINT8
CpuRegbarOr8 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset,
  IN  UINT8                             OrData
  );

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
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset,
  IN  UINT32                            AndData
  );

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
  );

/**
  Performs a bitwise AND of a 8-bit data.
  It programs REGBAR register and size in 1byte.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevice      CPU SB Device
  @param[in]  Offset           Register offset of Port ID.
  @param[in]  AndData          And Data. Must be the same size as Size parameter.

  @retval     UINT8            Value written to register

**/
UINT8
CpuRegbarAnd8 (
  IN  CPU_SB_DEVICE_PID                 CpuSbDevicePid,
  IN  UINT16                            Offset,
  IN  UINT8                             AndData
  );
/**
  Write REGBAR register.
  It programs REGBAR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
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
  );

/**
  Write REGBAR register.
  It programs REGBAR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
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
  );

/**
  Write REGBAR register.
  It programs REGBAR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  CpuSbDevicePid      CPU SB Device Port ID
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
  );

#endif
