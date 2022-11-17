/** @file
  Header file for HD Audio configuration.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_HDA_H_
#define _PCH_HDA_H_


enum PCH_HDAUDIO_DMIC_TYPE {
  PchHdaDmicDisabled = 0,
  PchHdaDmic2chArray = 1,
  PchHdaDmic4chArray = 2,
  PchHdaDmic1chArray = 3
};

typedef enum {
  PchHdaLinkFreq6MHz  = 0,
  PchHdaLinkFreq12MHz = 1,
  PchHdaLinkFreq24MHz = 2,
  PchHdaLinkFreq48MHz = 3,
  PchHdaLinkFreq96MHz = 4,
  PchHdaLinkFreqInvalid
} PCH_HDAUDIO_LINK_FREQUENCY;

typedef enum  {
  PchHdaIDispMode2T  = 0,
  PchHdaIDispMode1T  = 1,
  PchHdaIDispMode4T  = 2,
  PchHdaIDispMode8T  = 3,
  PchHdaIDispMode16T = 4,
  PchHdaIDispTModeInvalid
} PCH_HDAUDIO_IDISP_TMODE;

#endif // _PCH_HDA_H_
