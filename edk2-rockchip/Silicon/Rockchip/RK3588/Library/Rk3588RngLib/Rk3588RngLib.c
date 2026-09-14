/** @file
 *
 *  RK3588 hardware random number generator (TRNG v1).
 *
 *  The RK3588 has a non-secure TRNG at 0xfe378000, described in Linux as
 *  "rockchip,trngv1". It is a different block from the RK356x RNG, which lives
 *  inside the crypto v2 unit and has an unrelated register layout.
 *
 *  Without this, RngLib falls back to BaseRngLibTimerLib, which derives numbers
 *  from the performance counter and reports itself as gEdkiiRngAlgorithmUnSafe.
 *
 *  Copyright (c) 2025, Corey Moyer <corey@buzztv.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Base.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Guid/Rng.h>
#include <Guid/RngAlgorithm.h>

#define TRNG_BASE  0xfe378000

#define TRNG_CTRL          (TRNG_BASE + 0x0000)
#define  TRNG_CTRL_NOP     0x0
#define  TRNG_CTRL_RAND    0x1
#define TRNG_STAT          (TRNG_BASE + 0x0004)
#define  TRNG_STAT_SEEDED      BIT9
#define  TRNG_STAT_GENERATING  BIT30
#define  TRNG_STAT_RESEEDING   BIT31
#define TRNG_MODE          (TRNG_BASE + 0x0008)
#define  TRNG_MODE_256_BIT  (0x1 << 3)
#define TRNG_IE            (TRNG_BASE + 0x0010)
#define TRNG_ISTAT         (TRNG_BASE + 0x0014)
#define  TRNG_ISTAT_RAND_RDY  BIT0
#define TRNG_RAND(n)       (TRNG_BASE + 0x0020 + (n) * 0x4)
#define TRNG_AUTO_RQSTS    (TRNG_BASE + 0x0060)
#define TRNG_VERSION       (TRNG_BASE + 0x00F0)

//
// Value the version register must read back. The block is clocked by
// SCMI_HCLK_SECURE_NS, so if the secure firmware has left that gate closed this
// will not match and we fall back to the performance counter.
//
#define TRNG_VERSION_CODE  0x46bc

//
// Reseed automatically after (1000 * 16) bytes have been generated.
//
#define TRNG_AUTO_RESEED_COUNT  1000

//
// One generate call produces 256 bits.
//
#define TRNG_BYTES_PER_ROUND  32

//
// A 256-bit generate takes 1024 clock cycles, so roughly 7 us at the ~150 MHz
// the block runs at. Poll well past that before giving up.
//
#define TRNG_POLL_PERIOD_US   10
#define TRNG_POLL_TIMEOUT_US  50000

#define TIMER_FALLBACK_DELAY_US  10

//
// Cached result of ProbeTrng(). Deliberately tolerant of a read-only data
// section: if the write is dropped the value stays TRNG_STATE_UNKNOWN and we
// simply probe again on the next call.
//
#define TRNG_STATE_UNKNOWN      0
#define TRNG_STATE_AVAILABLE    1
#define TRNG_STATE_UNAVAILABLE  2

STATIC UINT8  mTrngState = TRNG_STATE_UNKNOWN;

/**
  Check whether the TRNG block is present and responding, and if so put it in a
  known state.

  @retval TRUE   The TRNG is usable.
  @retval FALSE  The TRNG did not identify itself; use the fallback.

**/
STATIC
BOOLEAN
ProbeTrng (
  VOID
  )
{
  UINT32  Version;
  UINT32  Status;
  UINTN   Elapsed;

  if (mTrngState != TRNG_STATE_UNKNOWN) {
    return mTrngState == TRNG_STATE_AVAILABLE;
  }

  Version = MmioRead32 (TRNG_VERSION);
  if (Version != TRNG_VERSION_CODE) {
    DEBUG ((
      DEBUG_WARN,
      "Rk3588Rng: TRNG not available (version %08x, expected %08x), "
      "falling back to the performance counter.\n",
      Version,
      TRNG_VERSION_CODE
      ));
    mTrngState = TRNG_STATE_UNAVAILABLE;
    return FALSE;
  }

  //
  // The block seeds itself at power on. Wait for that to finish before asking
  // it for anything.
  //
  for (Elapsed = 0; Elapsed < TRNG_POLL_TIMEOUT_US; Elapsed += TRNG_POLL_PERIOD_US) {
    Status = MmioRead32 (TRNG_STAT);
    if (((Status & TRNG_STAT_SEEDED) != 0) &&
        ((Status & (TRNG_STAT_GENERATING | TRNG_STAT_RESEEDING)) == 0))
    {
      break;
    }

    MicroSecondDelay (TRNG_POLL_PERIOD_US);
  }

  if (Elapsed >= TRNG_POLL_TIMEOUT_US) {
    DEBUG ((DEBUG_WARN, "Rk3588Rng: timed out waiting for the TRNG to seed.\n"));
    mTrngState = TRNG_STATE_UNAVAILABLE;
    return FALSE;
  }

  //
  // We poll, so make sure nothing is going to raise an interrupt we have no
  // handler for, and clear whatever the power-on reseed left behind.
  //
  MmioWrite32 (TRNG_IE, 0);
  MmioWrite32 (TRNG_ISTAT, MmioRead32 (TRNG_ISTAT));

  MmioWrite32 (TRNG_AUTO_RQSTS, TRNG_AUTO_RESEED_COUNT);

  DEBUG ((DEBUG_INFO, "Rk3588Rng: TRNG v1 ready.\n"));

  mTrngState = TRNG_STATE_AVAILABLE;
  return TRUE;
}

/**
  Generate 256 bits and copy the first Size bytes of them out.

  @param[out] Buffer  Destination.
  @param[in]  Size    Number of bytes to write, at most TRNG_BYTES_PER_ROUND.

  @retval TRUE   Buffer was filled.
  @retval FALSE  The TRNG did not produce a result in time.

**/
STATIC
BOOLEAN
TrngGenerate (
  OUT UINT8  *Buffer,
  IN  UINTN  Size
  )
{
  UINT32  Istat;
  UINTN   Elapsed;
  UINTN   Index;
  UINTN   Count;
  UINT32  Value;
  BOOLEAN Result;

  ASSERT (Size <= TRNG_BYTES_PER_ROUND);

  Result = FALSE;

  MmioWrite32 (TRNG_ISTAT, MmioRead32 (TRNG_ISTAT));

  MmioWrite32 (TRNG_MODE, TRNG_MODE_256_BIT);
  MmioWrite32 (TRNG_CTRL, TRNG_CTRL_RAND);

  for (Elapsed = 0; Elapsed < TRNG_POLL_TIMEOUT_US; Elapsed += TRNG_POLL_PERIOD_US) {
    Istat = MmioRead32 (TRNG_ISTAT);
    if ((Istat & TRNG_ISTAT_RAND_RDY) != 0) {
      break;
    }

    MicroSecondDelay (TRNG_POLL_PERIOD_US);
  }

  if (Elapsed < TRNG_POLL_TIMEOUT_US) {
    for (Index = 0; Index < TRNG_BYTES_PER_ROUND / sizeof (UINT32) && Size > 0; Index++) {
      Value = MmioRead32 (TRNG_RAND (Index));
      Count = MIN (Size, sizeof (UINT32));
      CopyMem (Buffer, &Value, Count);
      Buffer += Count;
      Size   -= Count;
    }

    MmioWrite32 (TRNG_ISTAT, MmioRead32 (TRNG_ISTAT));
    Result = TRUE;
  } else {
    DEBUG ((DEBUG_WARN, "Rk3588Rng: timed out waiting for random data.\n"));
  }

  //
  // Put the block back to idle whether or not we got anything.
  //
  MmioWrite32 (TRNG_CTRL, TRNG_CTRL_NOP);

  return Result;
}

/**
  Fill a buffer from the performance counter, one byte at a time, pausing long
  enough between reads for the counter to move.

  This is what BaseRngLibTimerLib does, and it is not a source of real entropy.
  It exists so that a board where the TRNG is unreachable behaves exactly as it
  did before this library was introduced rather than losing RNG support.

  @param[out] Buffer  Destination.
  @param[in]  Size    Number of bytes to write.

**/
STATIC
VOID
TimerFallbackGenerate (
  OUT UINT8  *Buffer,
  IN  UINTN  Size
  )
{
  UINT64  CounterHz;
  UINT32  DelayInMicroSeconds;
  UINTN   Index;

  CounterHz = GetPerformanceCounterProperties (NULL, NULL);
  if (CounterHz == 0) {
    DelayInMicroSeconds = TIMER_FALLBACK_DELAY_US;
  } else {
    //
    // One and a half counter periods, so the counter is guaranteed to have
    // advanced between two reads.
    //
    DelayInMicroSeconds = (UINT32)MAX (DivU64x64Remainder (1500000, CounterHz, NULL), 1);
  }

  for (Index = 0; Index < Size; Index++) {
    Buffer[Index] = (UINT8)(GetPerformanceCounter () & 0xFF);
    MicroSecondDelay (DelayInMicroSeconds);
  }
}

/**
  Fill a buffer with random bytes from the best source available.

  @param[out] Buffer  Destination.
  @param[in]  Size    Number of bytes to write.

  @retval TRUE   Buffer was filled.
  @retval FALSE  Buffer was not filled.

**/
STATIC
BOOLEAN
RngGetBytes (
  OUT UINT8  *Buffer,
  IN  UINTN  Size
  )
{
  UINTN  Count;

  if (!ProbeTrng ()) {
    TimerFallbackGenerate (Buffer, Size);
    return TRUE;
  }

  while (Size > 0) {
    Count = MIN (Size, TRNG_BYTES_PER_ROUND);
    if (!TrngGenerate (Buffer, Count)) {
      return FALSE;
    }

    Buffer += Count;
    Size   -= Count;
  }

  return TRUE;
}

/**
  Generates a 16-bit random number.

  @param[out] Rand     Buffer pointer to store the 16-bit random value.

  @retval TRUE         Random number generated successfully.
  @retval FALSE        Failed to generate the random number.

**/
BOOLEAN
EFIAPI
GetRandomNumber16 (
  OUT UINT16  *Rand
  )
{
  ASSERT (Rand != NULL);

  if (Rand == NULL) {
    return FALSE;
  }

  return RngGetBytes ((UINT8 *)Rand, sizeof (UINT16));
}

/**
  Generates a 32-bit random number.

  @param[out] Rand     Buffer pointer to store the 32-bit random value.

  @retval TRUE         Random number generated successfully.
  @retval FALSE        Failed to generate the random number.

**/
BOOLEAN
EFIAPI
GetRandomNumber32 (
  OUT UINT32  *Rand
  )
{
  ASSERT (Rand != NULL);

  if (Rand == NULL) {
    return FALSE;
  }

  return RngGetBytes ((UINT8 *)Rand, sizeof (UINT32));
}

/**
  Generates a 64-bit random number.

  @param[out] Rand     Buffer pointer to store the 64-bit random value.

  @retval TRUE         Random number generated successfully.
  @retval FALSE        Failed to generate the random number.

**/
BOOLEAN
EFIAPI
GetRandomNumber64 (
  OUT UINT64  *Rand
  )
{
  ASSERT (Rand != NULL);

  if (Rand == NULL) {
    return FALSE;
  }

  return RngGetBytes ((UINT8 *)Rand, sizeof (UINT64));
}

/**
  Generates a 128-bit random number.

  @param[out] Rand     Buffer pointer to store the 128-bit random value.

  @retval TRUE         Random number generated successfully.
  @retval FALSE        Failed to generate the random number.

**/
BOOLEAN
EFIAPI
GetRandomNumber128 (
  OUT UINT64  *Rand
  )
{
  ASSERT (Rand != NULL);

  if (Rand == NULL) {
    return FALSE;
  }

  return RngGetBytes ((UINT8 *)Rand, sizeof (UINT64) * 2);
}

/**
  Get a GUID identifying the RNG algorithm implementation.

  @param[out] RngGuid  The GUID.

  @retval EFI_SUCCESS             Success.
  @retval EFI_INVALID_PARAMETER   RngGuid is NULL.

**/
EFI_STATUS
EFIAPI
GetRngGuid (
  OUT GUID  *RngGuid
  )
{
  if (RngGuid == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Report what we can actually deliver. With the TRNG up this is raw hardware
  // entropy; without it we are no better than BaseRngLibTimerLib and must say
  // so, or callers will trust numbers that do not deserve it.
  //
  if (ProbeTrng ()) {
    CopyGuid (RngGuid, &gEfiRngAlgorithmRaw);
  } else {
    CopyGuid (RngGuid, &gEdkiiRngAlgorithmUnSafe);
  }

  return EFI_SUCCESS;
}
