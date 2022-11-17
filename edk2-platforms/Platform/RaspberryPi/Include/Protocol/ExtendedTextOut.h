/** @file
 *
 *  Copyright (c) 2018, Andrei Warkentin <andrey.warkentin@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __EXTENDED_TEXT_OUT_PROTOCOL_H__
#define __EXTENDED_TEXT_OUT_PROTOCOL_H__

#include <Protocol/SimpleTextOut.h>
#include <Protocol/GraphicsOutput.h>

#define EXTENDED_TEXT_OUTPUT_PROTOCOL_GUID \
  { \
    0x387477ff, 0xffc7, 0xffd2, {0x8e, 0x39, 0x0, 0xff, 0xc9, 0x69, 0x72, 0x3b } \
  }

typedef struct _EXTENDED_TEXT_OUTPUT_PROTOCOL EXTENDED_TEXT_OUTPUT_PROTOCOL;

struct _EXTENDED_TEXT_OUTPUT_PROTOCOL {
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *TextOut;
  EFI_GRAPHICS_OUTPUT_PROTOCOL    *GraphicsOutput;
  BOOLEAN                         AutoWrap;
};

extern EFI_GUID gExtendedTextOutputProtocolGuid;

#endif /* __EXTENDED_TEXT_OUT_PROTOCOL_H__ */
