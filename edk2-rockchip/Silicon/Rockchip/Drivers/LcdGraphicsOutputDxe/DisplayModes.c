/** @file

  Copyright (c) 2025, Mario Bălănică <mariobalanica02@gmail.com>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "LcdGraphicsOutputDxe.h"

//
// Common display modes
// Keep in sync with VarStoreData.h definitions.
// New entries are added at the bottom.
//
STATIC CONST DISPLAY_MODE  mDisplayModes[] = {
  [DISPLAY_MODE_640_480_60] = /* CEA */      {
    .Vic         = 1,
    .OscFreq     = 25175,
    .HActive     = 640,
    .HFrontPorch = 16,
    .HSync       = 96,
    .HBackPorch  = 48,
    .HSyncActive = 0,
    .VActive     = 480,
    .VFrontPorch = 10,
    .VSync       = 2,
    .VBackPorch  = 33,
    .VSyncActive = 0,
  },
  [DISPLAY_MODE_800_600_60] = /* DMT */      {
    .Vic         = 0,
    .OscFreq     = 40000,
    .HActive     = 800,
    .HFrontPorch = 40,
    .HSync       = 128,
    .HBackPorch  = 88,
    .HSyncActive = 1,
    .VActive     = 600,
    .VFrontPorch = 1,
    .VSync       = 4,
    .VBackPorch  = 23,
    .VSyncActive = 1,
  },
  [DISPLAY_MODE_1024_768_60] = /* DMT */     {
    .Vic         = 0,
    .OscFreq     = 65000,
    .HActive     = 1024,
    .HFrontPorch = 24,
    .HSync       = 136,
    .HBackPorch  = 160,
    .HSyncActive = 0,
    .VActive     = 768,
    .VFrontPorch = 3,
    .VSync       = 6,
    .VBackPorch  = 29,
    .VSyncActive = 0,
  },
  [DISPLAY_MODE_1152_864_60] = /* CVT */     {
    .Vic         = 0,
    .OscFreq     = 81750,
    .HActive     = 1152,
    .HFrontPorch = 64,
    .HSync       = 120,
    .HBackPorch  = 184,
    .HSyncActive = 0,
    .VActive     = 864,
    .VFrontPorch = 3,
    .VSync       = 4,
    .VBackPorch  = 26,
    .VSyncActive = 1,
  },
  [DISPLAY_MODE_1280_720_60] = /* CEA */     {
    .Vic         = 4,
    .OscFreq     = 74250,
    .HActive     = 1280,
    .HFrontPorch = 110,
    .HSync       = 40,
    .HBackPorch  = 220,
    .HSyncActive = 1,
    .VActive     = 720,
    .VFrontPorch = 5,
    .VSync       = 5,
    .VBackPorch  = 20,
    .VSyncActive = 1,
  },
  [DISPLAY_MODE_1280_768_60] = /* DMT */     {
    .Vic         = 0,
    .OscFreq     = 79500,
    .HActive     = 1280,
    .HFrontPorch = 64,
    .HSync       = 128,
    .HBackPorch  = 192,
    .HSyncActive = 0,
    .VActive     = 768,
    .VFrontPorch = 3,
    .VSync       = 7,
    .VBackPorch  = 20,
    .VSyncActive = 1,
  },
  [DISPLAY_MODE_1280_800_60] = /* DMT */     {
    .Vic         = 0,
    .OscFreq     = 83500,
    .HActive     = 1280,
    .HFrontPorch = 72,
    .HSync       = 128,
    .HBackPorch  = 200,
    .HSyncActive = 0,
    .VActive     = 800,
    .VFrontPorch = 3,
    .VSync       = 6,
    .VBackPorch  = 22,
    .VSyncActive = 1,
  },
  [DISPLAY_MODE_1280_960_60] = /* DMT */     {
    .Vic         = 0,
    .OscFreq     = 108000,
    .HActive     = 1280,
    .HFrontPorch = 96,
    .HSync       = 112,
    .HBackPorch  = 312,
    .HSyncActive = 1,
    .VActive     = 960,
    .VFrontPorch = 1,
    .VSync       = 3,
    .VBackPorch  = 36,
    .VSyncActive = 1,
  },
  [DISPLAY_MODE_1280_1024_60] = /* DMT */    {
    .Vic         = 0,
    .OscFreq     = 108000,
    .HActive     = 1280,
    .HFrontPorch = 48,
    .HSync       = 112,
    .HBackPorch  = 248,
    .HSyncActive = 1,
    .VActive     = 1024,
    .VFrontPorch = 1,
    .VSync       = 3,
    .VBackPorch  = 38,
    .VSyncActive = 1,
  },
  [DISPLAY_MODE_1360_768_60] = /* DMT */     {
    .Vic         = 0,
    .OscFreq     = 85500,
    .HActive     = 1360,
    .HFrontPorch = 64,
    .HSync       = 112,
    .HBackPorch  = 256,
    .HSyncActive = 1,
    .VActive     = 768,
    .VFrontPorch = 3,
    .VSync       = 6,
    .VBackPorch  = 18,
    .VSyncActive = 1,
  },
  [DISPLAY_MODE_1400_1050_60] = /* DMT */    {
    .Vic         = 0,
    .OscFreq     = 121750,
    .HActive     = 1400,
    .HFrontPorch = 88,
    .HSync       = 144,
    .HBackPorch  = 232,
    .HSyncActive = 0,
    .VActive     = 1050,
    .VFrontPorch = 3,
    .VSync       = 4,
    .VBackPorch  = 32,
    .VSyncActive = 1,
  },
  [DISPLAY_MODE_1440_900_60] = /* DMT */     {
    .Vic         = 0,
    .OscFreq     = 106500,
    .HActive     = 1440,
    .HFrontPorch = 80,
    .HSync       = 152,
    .HBackPorch  = 232,
    .HSyncActive = 0,
    .VActive     = 900,
    .VFrontPorch = 3,
    .VSync       = 6,
    .VBackPorch  = 25,
    .VSyncActive = 1,
  },
  [DISPLAY_MODE_1600_900_60] = /* DMT */     {
    .Vic         = 0,
    .OscFreq     = 108000,
    .HActive     = 1600,
    .HFrontPorch = 24,
    .HSync       = 80,
    .HBackPorch  = 96,
    .HSyncActive = 1,
    .VActive     = 900,
    .VFrontPorch = 1,
    .VSync       = 3,
    .VBackPorch  = 96,
    .VSyncActive = 1,
  },
  [DISPLAY_MODE_1600_1200_60] = /* DMT */    {
    .Vic         = 0,
    .OscFreq     = 162000,
    .HActive     = 1600,
    .HFrontPorch = 64,
    .HSync       = 192,
    .HBackPorch  = 304,
    .HSyncActive = 1,
    .VActive     = 1200,
    .VFrontPorch = 1,
    .VSync       = 3,
    .VBackPorch  = 46,
    .VSyncActive = 1,
  },
  [DISPLAY_MODE_1680_1050_60] = /* DMT */    {
    .Vic         = 0,
    .OscFreq     = 146250,
    .HActive     = 1680,
    .HFrontPorch = 104,
    .HSync       = 176,
    .HBackPorch  = 280,
    .HSyncActive = 0,
    .VActive     = 1050,
    .VFrontPorch = 3,
    .VSync       = 6,
    .VBackPorch  = 30,
    .VSyncActive = 1,
  },
  [DISPLAY_MODE_1920_1080_60] = /* CEA */    {
    .Vic         = 16,
    .OscFreq     = 148500,
    .HActive     = 1920,
    .HFrontPorch = 88,
    .HSync       = 44,
    .HBackPorch  = 148,
    .HSyncActive = 1,
    .VActive     = 1080,
    .VFrontPorch = 4,
    .VSync       = 5,
    .VBackPorch  = 36,
    .VSyncActive = 1,
  },
  [DISPLAY_MODE_1920_1200_60] = /* CVT-RB */ {
    .Vic         = 0,
    .OscFreq     = 154000,
    .HActive     = 1920,
    .HFrontPorch = 48,
    .HSync       = 32,
    .HBackPorch  = 80,
    .HSyncActive = 1,
    .VActive     = 1200,
    .VFrontPorch = 3,
    .VSync       = 6,
    .VBackPorch  = 26,
    .VSyncActive = 0,
  },
  [DISPLAY_MODE_2048_1152_60] = /* DMT */    {
    .Vic         = 0,
    .OscFreq     = 162000,
    .HActive     = 2048,
    .HFrontPorch = 26,
    .HSync       = 80,
    .HBackPorch  = 96,
    .HSyncActive = 1,
    .VActive     = 1152,
    .VFrontPorch = 1,
    .VSync       = 3,
    .VBackPorch  = 44,
    .VSyncActive = 1,
  },
  [DISPLAY_MODE_2560_1080_60] = /* CEA */    {
    .Vic         = 90,
    .OscFreq     = 198000,
    .HActive     = 2560,
    .HFrontPorch = 248,
    .HSync       = 44,
    .HBackPorch  = 148,
    .HSyncActive = 1,
    .VActive     = 1080,
    .VFrontPorch = 4,
    .VSync       = 5,
    .VBackPorch  = 11,
    .VSyncActive = 1,
  },
  [DISPLAY_MODE_2560_1440_60] = /* CVT-RB */ {
    .Vic         = 0,
    .OscFreq     = 241500,
    .HActive     = 2560,
    .HFrontPorch = 48,
    .HSync       = 32,
    .HBackPorch  = 80,
    .HSyncActive = 1,
    .VActive     = 1440,
    .VFrontPorch = 3,
    .VSync       = 5,
    .VBackPorch  = 33,
    .VSyncActive = 0,
  },
  [DISPLAY_MODE_2560_1600_60] = /* CVT-RB */ {
    .Vic         = 0,
    .OscFreq     = 268500,
    .HActive     = 2560,
    .HFrontPorch = 48,
    .HSync       = 32,
    .HBackPorch  = 80,
    .HSyncActive = 1,
    .VActive     = 1600,
    .VFrontPorch = 3,
    .VSync       = 6,
    .VBackPorch  = 37,
    .VSyncActive = 0,
  },
  [DISPLAY_MODE_3440_1440_60] = /* CVT-RB */ {
    .Vic         = 0,
    .OscFreq     = 319750,
    .HActive     = 3440,
    .HFrontPorch = 48,
    .HSync       = 32,
    .HBackPorch  = 80,
    .HSyncActive = 1,
    .VActive     = 1440,
    .VFrontPorch = 3,
    .VSync       = 10,
    .VBackPorch  = 28,
    .VSyncActive = 0,
  },
  [DISPLAY_MODE_3840_2160_30] = /* CEA */    {
    .Vic         = 95,
    .OscFreq     = 297000,
    .HActive     = 3840,
    .HFrontPorch = 176,
    .HSync       = 88,
    .HBackPorch  = 296,
    .HSyncActive = 1,
    .VActive     = 2160,
    .VFrontPorch = 8,
    .VSync       = 10,
    .VBackPorch  = 72,
    .VSyncActive = 1,
  },
  [DISPLAY_MODE_3840_2160_60] = /* CEA */    {
    .Vic         = 97,
    .OscFreq     = 594000,
    .HActive     = 3840,
    .HFrontPorch = 176,
    .HSync       = 88,
    .HBackPorch  = 296,
    .HSyncActive = 1,
    .VActive     = 2160,
    .VFrontPorch = 8,
    .VSync       = 10,
    .VBackPorch  = 72,
    .VSyncActive = 1,
  },
  [DISPLAY_MODE_4096_2160_30] = /* CEA */    {
    .Vic         = 100,
    .OscFreq     = 297000,
    .HActive     = 4096,
    .HFrontPorch = 88,
    .HSync       = 88,
    .HBackPorch  = 128,
    .HSyncActive = 1,
    .VActive     = 2160,
    .VFrontPorch = 8,
    .VSync       = 10,
    .VBackPorch  = 72,
    .VSyncActive = 1,
  },
  [DISPLAY_MODE_4096_2160_60] = /* CEA */    {
    .Vic         = 102,
    .OscFreq     = 594000,
    .HActive     = 4096,
    .HFrontPorch = 88,
    .HSync       = 88,
    .HBackPorch  = 128,
    .HSyncActive = 1,
    .VActive     = 2160,
    .VFrontPorch = 8,
    .VSync       = 10,
    .VBackPorch  = 72,
    .VSyncActive = 1,
  },
};

STATIC CONST UINT32  mDisplayModesCount = ARRAY_SIZE (mDisplayModes);

UINT32
GetPredefinedDisplayModesCount (
  VOID
  )
{
  return mDisplayModesCount;
}

CONST DISPLAY_MODE *
GetPredefinedDisplayMode (
  IN UINT32  Index
  )
{
  if (Index >= mDisplayModesCount) {
    ASSERT (FALSE);
    return NULL;
  }

  return &mDisplayModes[Index];
}

CONST DISPLAY_MODE *
GetPredefinedDisplayModeByVic (
  IN UINT8  Vic
  )
{
  UINT32              Index;
  CONST DISPLAY_MODE  *PredefinedMode;

  for (Index = 0; Index < mDisplayModesCount; Index++) {
    PredefinedMode = &mDisplayModes[Index];

    if (PredefinedMode->Vic == Vic) {
      return PredefinedMode;
    }
  }

  return NULL;
}

CONST DISPLAY_MODE *
GetPredefinedDisplayModeByResolution (
  IN UINT32  HorizontalResolution,
  IN UINT32  VerticalResolution,
  IN UINT32  RefreshRate
  )
{
  UINT32              Index;
  CONST DISPLAY_MODE  *PredefinedMode;

  for (Index = 0; Index < mDisplayModesCount; Index++) {
    PredefinedMode = &mDisplayModes[Index];

    if ((PredefinedMode->HActive == HorizontalResolution) &&
        (PredefinedMode->VActive == VerticalResolution) &&
        (DisplayModeVRefresh (PredefinedMode) == RefreshRate))
    {
      return PredefinedMode;
    }
  }

  return NULL;
}

CONST DISPLAY_MODE *
MatchPredefinedDisplayMode (
  IN CONST DISPLAY_MODE  *DisplayMode,
  IN UINT32              ClockTolerance
  )
{
  UINT32              Index;
  CONST DISPLAY_MODE  *PredefinedMode;

  for (Index = 0; Index < mDisplayModesCount; Index++) {
    PredefinedMode = &mDisplayModes[Index];

    if ((DisplayMode->HActive != PredefinedMode->HActive) ||
        (DisplayMode->HFrontPorch != PredefinedMode->HFrontPorch) ||
        (DisplayMode->HSync != PredefinedMode->HSync) ||
        (DisplayMode->HBackPorch != PredefinedMode->HBackPorch) ||
        (DisplayMode->HSyncActive != PredefinedMode->HSyncActive) ||
        (DisplayMode->VActive != PredefinedMode->VActive) ||
        (DisplayMode->VFrontPorch != PredefinedMode->VFrontPorch) ||
        (DisplayMode->VSync != PredefinedMode->VSync) ||
        (DisplayMode->VBackPorch != PredefinedMode->VBackPorch) ||
        (DisplayMode->VSyncActive != PredefinedMode->VSyncActive) ||
        (DisplayMode->DenActive != PredefinedMode->DenActive) ||
        (DisplayMode->ClkActive != PredefinedMode->ClkActive))
    {
      continue;
    }

    if (ABS ((INT32)(DisplayMode->OscFreq - PredefinedMode->OscFreq)) > ClockTolerance) {
      continue;
    }

    return PredefinedMode;
  }

  return NULL;
}
