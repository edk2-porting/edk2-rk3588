/** @file
  Header file for HD Audio configuration.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _HDA_H_
#define _HDA_H_

typedef enum {
  HdaVc0 = 0,
  HdaVc1 = 1
} HDAUDIO_VC_TYPE;

typedef enum {
  HdaDmicDisabled = 0,
  HdaDmic2chArray = 1,
  HdaDmic4chArray = 2,
  HdaDmic1chArray = 3
} HDAUDIO_DMIC_TYPE;

typedef enum {
  HdaLinkFreq6MHz  = 0,
  HdaLinkFreq12MHz = 1,
  HdaLinkFreq24MHz = 2,
  HdaLinkFreq48MHz = 3,
  HdaLinkFreq96MHz = 4,
  HdaLinkFreqInvalid
} HDAUDIO_LINK_FREQUENCY;

typedef enum  {
  HdaIDispMode2T  = 0,
  HdaIDispMode1T  = 1,
  HdaIDispMode4T  = 2,
  HdaIDispMode8T  = 3,
  HdaIDispMode16T = 4,
  HdaIDispTModeInvalid
} HDAUDIO_IDISP_TMODE;

typedef enum {
  HdaLink      = 0,
  HdaIDispLink = 1,
  HdaDmic      = 2,
  HdaSsp       = 3,
  HdaSndw      = 4,
  HdaLinkUnsupported
} HDAUDIO_LINK_TYPE;

typedef enum {
  HdaDmicClockSelectBoth = 0,
  HdaDmicClockSelectClkA = 1,
  HdaDmicClockSelectClkB = 2,
  HdaDmicClockSelectInvalid
} HDAUDIO_DMIC_CLOCK_SELECT;

#endif
