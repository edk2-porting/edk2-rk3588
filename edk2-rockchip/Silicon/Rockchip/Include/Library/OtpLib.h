/** @file
 *
 *  Copyright (c) 2022, Jared McNeill <jmcneill@invisible.ca>
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef OTPLIB_H__
#define OTPLIB_H__

VOID
OtpRead (
  IN UINT16  Offset,
  IN UINT16  Length,
  OUT UINT8  *Data
  );

VOID
OtpReadCpuCode (
  OUT UINT16  *CpuCode
  );

VOID
OtpReadId (
  OUT UINT8  Id[16]
  );

VOID
OtpReadCpuVersion (
  OUT UINT8  *Version
  );

#endif /* OTPLIB_H__ */
