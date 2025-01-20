/** @file
 *
 *  RK3588 OTP Library.
 *
 *  Copyright (c) 2022, Jared McNeill <jmcneill@invisible.ca>
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/OtpLib.h>

#define OTP_BASE  0xfecc0000

#define RK3588_OTPC_AUTO_CTRL  (OTP_BASE + 0x0004)
#define  RK3588_ADDR_SHIFT     16
#define  RK3588_BURST_SHIFT    8
#define RK3588_OTPC_AUTO_EN    (OTP_BASE + 0x0008)
#define  RK3588_AUTO_EN        BIT0
#define RK3588_OTPC_DOUT0      (OTP_BASE + 0x0020)
#define RK3588_OTPC_INT_ST     (OTP_BASE + 0x0084)
#define  RK3588_RD_DONE        BIT1

#define RK3588_BLOCK_SIZE        4
#define RK3588_NO_SECURE_OFFSET  0x300

VOID
OtpRead (
  IN UINT16  Offset,
  IN UINT16  Length,
  OUT UINT8  *Data
  )
{
  UINT32  Addr;
  UINT16  AddrOffset;
  UINT16  BytesRemaining;
  UINT32  Value;
  UINTN   Retry;

  Addr           = (Offset / RK3588_BLOCK_SIZE) + RK3588_NO_SECURE_OFFSET;
  AddrOffset     = Offset % RK3588_BLOCK_SIZE;
  BytesRemaining = Length;
  Retry          = 100000;

  while (BytesRemaining > 0) {
    MmioWrite32 (RK3588_OTPC_AUTO_CTRL, Addr << RK3588_ADDR_SHIFT | 1 << RK3588_BURST_SHIFT);
    MmioWrite32 (RK3588_OTPC_AUTO_EN, RK3588_AUTO_EN);
    while ((MmioRead32 (RK3588_OTPC_INT_ST) & RK3588_RD_DONE) == 0) {
      MicroSecondDelay (1);
      if (--Retry == 0) {
        DEBUG ((DEBUG_WARN, "OTP read timeout!\n"));
        break;
      }
    }

    MmioWrite32 (RK3588_OTPC_INT_ST, RK3588_RD_DONE);

    Value = MmioRead32 (RK3588_OTPC_DOUT0);

    while (AddrOffset < RK3588_BLOCK_SIZE && BytesRemaining > 0) {
      *Data++ = (Value >> (8 * AddrOffset)) & 0xFF;
      AddrOffset++;
      BytesRemaining--;
    }

    AddrOffset = 0;
    Addr++;
  }
}

VOID
OtpReadCpuCode (
  OUT UINT16  *CpuCode
  )
{
  OtpRead (0x02, 0x2, (UINT8 *)CpuCode);
}

VOID
OtpReadId (
  OUT UINT8  Id[16]
  )
{
  OtpRead (0x07, 0x10, Id);
}

VOID
OtpReadCpuVersion (
  OUT UINT8  *Version
  )
{
  OtpRead (0x1c, 0x1, Version);
}
