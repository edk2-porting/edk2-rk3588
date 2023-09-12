/** @file
 *
 *  Platform Status LED controller
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/RockchipPlatformLib.h>

#define REPEAT_INFINITE         MAX_UINT32

#define LONG_PULSE_PERIOD_MS    840
#define SHORT_PULSE_PERIOD_MS   140

#define MILLIS_TO_MICROS(x)     (x * 1000)

// Timer for async execution
EFI_EVENT mTimerEvent;

// Parameters
UINT32 mAsyncLongPulseCount;
UINT32 mAsyncShortPulseCount;
UINT32 mAsyncRepeatCount;
UINT32 mAsyncPatternDelayMs;
UINT32 mAsyncContinueWithIdlePattern;

// State variables
UINT32 mAsyncRepeatIndex;
UINT32 mAsyncShortPulseIndex;
UINT32 mAsyncLongPulseIndex;

BOOLEAN mLedEnabled;

STATIC
VOID
SetLed (
  IN BOOLEAN Enable
  )
{
  PlatformSetStatusLed (Enable);
  mLedEnabled = Enable;
}

STATIC
VOID
InitLed (
  VOID
  )
{
  PlatformInitLeds ();
}

STATIC
VOID
BlinkLedAsync (
  IN UINT32   LongPulseCount,
  IN UINT32   ShortPulseCount,
  IN UINT32   RepeatCount,
  IN UINT32   PatternDelayMs,
  IN BOOLEAN  ContinueWithIdlePattern
  )
{
  gBS->SetTimer (mTimerEvent, TimerCancel, 0);

  mAsyncLongPulseCount = LongPulseCount;
  mAsyncShortPulseCount = ShortPulseCount;
  mAsyncRepeatCount = RepeatCount;
  mAsyncPatternDelayMs = PatternDelayMs;
  mAsyncContinueWithIdlePattern = ContinueWithIdlePattern;
  mAsyncRepeatIndex = 0;
  mAsyncShortPulseIndex = 0;
  mAsyncLongPulseIndex = 0;

  SetLed (FALSE);

  gBS->SetTimer (mTimerEvent, TimerRelative, 0);
}

STATIC
VOID
BlinkLedIdleAsync (
  VOID
  )
{
  BlinkLedAsync (0, 1, REPEAT_INFINITE, 2000, FALSE);
}

STATIC
VOID
TimerHandler (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  //
  // Process long pulses
  //
  if (mAsyncLongPulseIndex < mAsyncLongPulseCount) {
    if (mLedEnabled) {
      mAsyncLongPulseIndex++;
    }
    SetLed (!mLedEnabled);

    gBS->SetTimer (mTimerEvent, TimerRelative,
          EFI_TIMER_PERIOD_MILLISECONDS (LONG_PULSE_PERIOD_MS));
    return;
  }

  //
  // Process short pulses
  //
  if (mAsyncShortPulseIndex < mAsyncShortPulseCount) {
    if (mLedEnabled) {
      mAsyncShortPulseIndex++;
    }
    SetLed (!mLedEnabled);

    gBS->SetTimer (mTimerEvent, TimerRelative,
          EFI_TIMER_PERIOD_MILLISECONDS (SHORT_PULSE_PERIOD_MS));
    return;
  }

  //
  // Completion happens at mAsyncRepeatCount + 1, because
  // we schedule this routine one more time just to wait
  // for mAsyncPatternDelayMs and thus introduce a pause
  // before the next pattern.
  //
  if (mAsyncRepeatIndex > mAsyncRepeatCount) {
    if (mAsyncContinueWithIdlePattern) {
      BlinkLedIdleAsync ();
    }
    return;
  }

  //
  // Do we need to repeat the pattern? Reset the pulse indexes.
  //
  if (mAsyncRepeatIndex < mAsyncRepeatCount) {
    mAsyncShortPulseIndex = 0;
    mAsyncLongPulseIndex = 0;
  }

  //
  // Keep repeating until completion above.
  //
  if (mAsyncRepeatCount != REPEAT_INFINITE) {
     mAsyncRepeatIndex++;
  }

  gBS->SetTimer (mTimerEvent, TimerRelative,
                 EFI_TIMER_PERIOD_MILLISECONDS (mAsyncPatternDelayMs));
}

STATIC
VOID
BlinkLedSync (
  IN UINT32   LongPulseCount,
  IN UINT32   ShortPulseCount,
  IN UINT32   RepeatCount,
  IN UINT32   PatternDelayMs,
  IN BOOLEAN  ContinueWithIdlePattern
  )
{
  UINT32 RepeatIndex = 0;
  UINT32 PulseIndex = 0;

  gBS->SetTimer (mTimerEvent, TimerCancel, 0);
  SetLed (FALSE);

  while (RepeatIndex <= RepeatCount) {
    for (PulseIndex = 0; PulseIndex < LongPulseCount; PulseIndex++) {
      SetLed (TRUE);
      gBS->Stall (MILLIS_TO_MICROS(LONG_PULSE_PERIOD_MS));
      SetLed (FALSE);
      gBS->Stall (MILLIS_TO_MICROS(LONG_PULSE_PERIOD_MS));
    }

    for (PulseIndex = 0; PulseIndex < ShortPulseCount; PulseIndex++) {
      SetLed (TRUE);
      gBS->Stall (MILLIS_TO_MICROS(SHORT_PULSE_PERIOD_MS));
      SetLed (FALSE);
      gBS->Stall (MILLIS_TO_MICROS(SHORT_PULSE_PERIOD_MS));
    }

    gBS->Stall (MILLIS_TO_MICROS(PatternDelayMs));

    if (RepeatCount != REPEAT_INFINITE) {
      RepeatIndex++;
    }
  }

  if (ContinueWithIdlePattern) {
    BlinkLedIdleAsync ();
  }
}

STATIC
VOID
EFIAPI
NotifyPlatformBmAfterConsole (
  IN EFI_EVENT   Event,
  IN VOID        *Context
  )
{
  //
  // All platform drivers have been dispatched and configured
  // at this point. Start the idle pattern to indicate that the
  // firmware is alive and (hopefully) well.
  //
  BlinkLedIdleAsync ();
}

STATIC
VOID
EFIAPI
NotifyExitBootServices (
  IN EFI_EVENT   Event,
  IN VOID        *Context
  )
{
  //
  // Turn off the LED in case the timer didn't have a chance to.
  //
  SetLed (FALSE);
}

EFI_STATUS
EFIAPI
StatusLedDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS Status;
  EFI_EVENT Event = NULL;

  InitLed ();

  Status = gBS->CreateEvent (
                  EVT_TIMER | EVT_NOTIFY_SIGNAL,  // Type
                  TPL_NOTIFY,                     // NotifyTpl
                  TimerHandler,                   // NotifyFunction
                  NULL,                           // NotifyContext
                  &mTimerEvent                    // Event
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Initial firmware loading pattern
  BlinkLedAsync (0, 1, REPEAT_INFINITE, 0, FALSE);

  Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,                          // Type
                    TPL_NOTIFY,                                 // NotifyTpl
                    NotifyPlatformBmAfterConsole,               // NotifyFunction
                    NULL,                                       // NotifyContext
                    &gRockchipEventPlatformBmAfterConsoleGuid,  // EventGroup
                    &Event                                      // Event
                    );

  Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,              // Type
                    TPL_NOTIFY,                     // NotifyTpl
                    NotifyExitBootServices,         // NotifyFunction
                    NULL,                           // NotifyContext
                    &gEfiEventExitBootServicesGuid, // EventGroup
                    &Event                          // Event
                    );

  return EFI_SUCCESS;
}
