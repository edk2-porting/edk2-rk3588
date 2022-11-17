/**
 * @file VideoModes.c
 * @brief Pre-calculated video timings sent to the DisplayLink device when a video mode is selected
 *
 * Copyright (c) 2018-2019, DisplayLink (UK) Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 *
**/

#include "UsbDisplayLink.h"


// Supported video modes - must be in order of pixel count (i.e. hres x vres)

STATIC CONST struct VideoMode ModeData[] = {
  {
     // 640 x 480 @ 60Hz
    .Reserved2 = 2,
    .PixelClock = 2517,
    .HActive = 640,
    .HBlanking = 160,
    .HSyncOffset = 16,
    .HSyncWidth = 96,
    .VActive = 480,
    .VBlanking = 45,
    .VSyncOffset = 10,
    .VSyncWidth = 2,
    .Flags = 0x00000300,
    .Accumulate = 1,
    .Reserved3 = 0,
    .Reserved4 = 0,
    .Reserved5 = 0x00000000,
    .Vic = 0,
    .ActiveFormat = 0,
  },
  {
    // 800 x 600 @ 60Hz
    .Reserved2 = 2,
    .PixelClock = 4000,
    .HActive = 800,
    .HBlanking = 256,
    .HSyncOffset = 40,
    .HSyncWidth = 128,
    .VActive = 600,
    .VBlanking = 28,
    .VSyncOffset = 1,
    .VSyncWidth = 3,
    .Flags = 0x00000000,
    .Accumulate = 1,
    .Reserved3 = 0,
    .Reserved4 = 0,
    .Reserved5 = 0x00000000,
    .Vic = 0,
    .ActiveFormat = 0,
  },
  {
    // 1024x768 @ 60Hz
    .Reserved1 = 0,
    .Reserved2 = 2,
    .PixelClock = 6500,
    .HActive = 1024,
    .HBlanking = 320,
    .HSyncOffset = 24,
    .HSyncWidth = 136,
    .VActive = 768,
    .VBlanking = 38,
    .VSyncOffset = 3,
    .VSyncWidth = 6,
    .Flags = 0x00000300,
    .Accumulate = 1,
    .Reserved3 = 0,
    .Reserved4 = 0,
    .Reserved5 = 0x00000000,
    .Vic = 0,
    .ActiveFormat = 0,
  },
  {
    // 1360x768 @ 60Hz
    .Reserved1 = 0,
    .Reserved2 = 2,
    .PixelClock = 8550,
    .HActive = 1360,
    .HBlanking = 432,
    .HSyncOffset = 64,
    .HSyncWidth = 112,
    .VActive = 768,
    .VBlanking = 27,
    .VSyncOffset = 3,
    .VSyncWidth = 6,
    .Flags = 0x00000000,
    .Accumulate = 1,
    .Reserved3 = 0,
    .Reserved4 = 0,
    .Reserved5 = 0x00000000,
    .Vic = 0,
    .ActiveFormat = 0,
  },
  {
    // 1280x960 @ 60Hz
    .Reserved1 = 0,
    .Reserved2 = 2,
    .PixelClock = 10800,
    .HActive = 1280,
    .HBlanking = 520,
    .HSyncOffset = 96,
    .HSyncWidth = 112,
    .VActive = 960,
    .VBlanking = 40,
    .VSyncOffset = 1,
    .VSyncWidth = 3,
    .Flags = 0x00000000,
    .Accumulate = 1,
    .Reserved3 = 0,
    .Reserved4 = 0,
    .Reserved5 = 0x00000000,
    .Vic = 0,
    .ActiveFormat = 0,
  },
  {
    // 1280x1024 @ 60Hz
    .Reserved1 = 0,
    .Reserved2 = 2,
    .PixelClock = 10800,
    .HActive = 1280,
    .HBlanking = 408,
    .HSyncOffset = 48,
    .HSyncWidth = 112,
    .VActive = 1024,
    .VBlanking = 42,
    .VSyncOffset = 1,
    .VSyncWidth = 3,
    .Flags = 0x00000000,
    .Accumulate = 1,
    .Reserved3 = 0,
    .Reserved4 = 0,
    .Reserved5 = 0x00000000,
    .Vic = 0,
    .ActiveFormat = 0,
  },
  {
    // 1600x900 @ 60Hz
    .Reserved2 = 2,
    .PixelClock = 11825,
    .HActive = 1600,
    .HBlanking = 512,
    .HSyncOffset = 88,
    .HSyncWidth = 168,
    .VActive = 900,
    .VBlanking = 34,
    .VSyncOffset = 3,
    .VSyncWidth = 5,
    .Flags = 0x00000500,
    .Accumulate = 1,
    .Reserved3 = 0,
    .Reserved4 = 0,
    .Reserved5 = 0x00000000,
    .Vic = 0,
    .ActiveFormat = 0,
  },
  {
    // 1400x1050 @ 60Hz
    .Reserved1 = 0,
    .Reserved2 = 2,
    .PixelClock = 12175,
    .HActive = 1400,
    .HBlanking = 464,
    .HSyncOffset = 88,
    .HSyncWidth = 144,
    .VActive = 1050,
    .VBlanking = 39,
    .VSyncOffset = 3,
    .VSyncWidth = 4,
    .Flags = 0x00000100,
    .Accumulate = 1,
    .Reserved3 = 0,
    .Reserved4 = 0,
    .Reserved5 = 0x00000000,
    .Vic = 0,
    .ActiveFormat = 0,
  },
  {
    // 1600x1200 @ 60Hz
    .Reserved1 = 0,
    .Reserved2 = 2,
    .PixelClock = 16200,
    .HActive = 1600,
    .HBlanking = 560,
    .HSyncOffset = 64,
    .HSyncWidth = 192,
    .VActive = 1200,
    .VBlanking = 50,
    .VSyncOffset = 1,
    .VSyncWidth = 3,
    .Flags = 0x00000000,
    .Accumulate = 1,
    .Reserved3 = 0,
    .Reserved4 = 0,
    .Reserved5 = 0x00000000,
    .Vic = 0,
    .ActiveFormat = 0,
  },
  {
    // 1920 x 1080
    .Reserved2 = 2,
    .PixelClock = 14850,
    .HActive = 1920,
    .HBlanking = 280,
    .HSyncOffset = 88,
    .HSyncWidth = 44,
    .VActive = 1080,
    .VBlanking = 45,
    .VSyncOffset = 4,
    .VSyncWidth = 5,
    .Flags = 0x00000000,
    .Accumulate = 1,
    .Reserved3 = 0,
    .Reserved4 = 0,
    .Reserved5 = 0x00000000,
    .Vic = 0,
    .ActiveFormat = 0,
  }
};

STATIC CONST UINT32 NumSupportedVideoModes = (sizeof (ModeData) / sizeof (struct VideoMode));

/**
Find the number of pre-calculated video modes that we support.

@retval Number of modes.

**/
UINT32 DlVideoModeGetNumSupportedVideoModes ()
{
  return NumSupportedVideoModes;
}

/**
Get one of the pre-calculated video modes

@param  index      The video mode that we want.

@retval NULL       The index was out of range.

**/
CONST struct VideoMode *DlVideoModeGetSupportedVideoMode (
    UINT32 Index
    )
{
  if (Index >= NumSupportedVideoModes) {
    return NULL;
  }
  return &ModeData[Index];
}
