/** @file
  Virtural Keyboard Layout Engine

  Copyright (c) 2012 - 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "VirtualKeyboard.h"

//
// 1.FullKeyboardBody
// Follow below design
//   (1).\KeyboardLayout\CapitalLetterKeyboard.bmp
//   (2).\KeyboardLayout\DigitKeyboard.bmp
// Based on keyboard's (startX, startY)
//
// Page0Font
// +---+---+---+---+---+---+---+---+---+---+---+
// | q | w | e | r | t | y | u | i | o | p |<X|| Line 0
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// | | a | s | d | f | g | h | j | k | l | F2| | Line 1
// +-+---+---+---+---+---+---+---+---+---+---+-+
// |Caps | z | x | c | v | b | n | m |aU |Enter| Line 2
// +-----+---+---+---+---+---+---+---+---+---+-+
// | Esc |12#|       Space       |aL |aD |aR | | Line 3
// +-----+---+-------------------+---+---+---+-+
//
// Page1Font
// +---+---+---+---+---+---+---+---+---+---+---+
// | Q | W | E | R | T | Y | U | I | O | P |<X|| Line 0
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// | | A | S | D | F | G | H | J | K | L | F2| | Line 1
// +-+---+---+---+---+---+---+---+---+---+---+-+
// |Caps | Z | X | C | V | B | N | M |aU |Enter| Line 2
// +-----+---+---+---+---+---+---+---+---+---+-+
// | Esc |12#|       Space       |aL |aD |aR | | Line 3
// +-----+---+-------------------+---+---+---+-+
//
// Page2Font
// +---+---+---+---+---+---+---+---+---+---+---+
// | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 0 |<X|| Line 0
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// | |F1 |F2 |F3 |F4 |F5 |F6 |F7 |F8 |F9 |F10| | Line 1
// +-+---+---+---+---+---+---+---+---+---+---+-+
// |Shift| . | ; | ' | , | . | / |F11|F12|Enter| Line 2
// +-----+---+---+---+---+---+---+---+---+---+-+
// | Esc |12#|       Space       | \ | - | = | | Line 3
// +-----+---+-------------------+---+---+---+-+
//
// Page3Font
// +---+---+---+---+---+---+---+---+---+---+---+
// | ! | @ | # | $ | % | ^ | & | * | ( | ) |<X|| Line 0
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// | |F1 |F2 |F3 |F4 |F5 |F6 |F7 |F8 |F9 |F10| | Line 1
// +-+---+---+---+---+---+---+---+---+---+---+-+
// |Shift| ~ | : | " | < | > | ? |F11|F12|Enter| Line 2
// +-----+---+---+---+---+---+---+---+---+---+-+
// | Esc |12#|       Space       | | | _ | + | | Line 3
// +-----+---+-------------------+---+---+---+-+
//
// 2.Screen Corner
// Follow below design
//   (1).\KeyboardLayout\SimpleIcon.bmp # Screen Corner A/B
//   (2).\KeyboardLayout\FullIcon.bmp   # Screen Corner C/D
// +-+------------------------------------+-+
// |A|                                    |C|
// +-+                                    +-+
// |                                        |
// |                                        |
// |                                        |
// |                                        |
// |                                        |
// |                                        |
// +-+                                    +-+
// |B|                                    |D|
// +-+------------------------------------+-+
//
VK_STRUCT mFullKeyboardBody[] = {
  // StartX  StartY  EndX    EndY     Page0Font            Page1Font            Page2Font            Page3Font
  // Line 0
  {  0x0000, 0x0000, 0x0032, 0x0032, {L'q',                L'Q',                L'1',                L'!'            }},
  {  0x0032, 0x0000, 0x0064, 0x0032, {L'w',                L'W',                L'2',                L'@'            }},
  {  0x0064, 0x0000, 0x0096, 0x0032, {L'e',                L'E',                L'3',                L'#'            }},
  {  0x0096, 0x0000, 0x00C8, 0x0032, {L'r',                L'R',                L'4',                L'$'            }},
  {  0x00C8, 0x0000, 0x00FA, 0x0032, {L't',                L'T',                L'5',                L'%'            }},
  {  0x00FA, 0x0000, 0x012C, 0x0032, {L'y',                L'Y',                L'6',                L'^'            }},
  {  0x012C, 0x0000, 0x015E, 0x0032, {L'u',                L'U',                L'7',                L'&'            }},
  {  0x015E, 0x0000, 0x0190, 0x0032, {L'i',                L'I',                L'8',                L'*'            }},
  {  0x0190, 0x0000, 0x01C2, 0x0032, {L'o',                L'O',                L'9',                L'('            }},
  {  0x01C2, 0x0000, 0x01F4, 0x0032, {L'p',                L'P',                L'0',                L')'            }},
  {  0x01F4, 0x0000, 0x0226, 0x0032, {VkKeyBackspace,      VkKeyBackspace,      VkKeyBackspace,      VkKeyBackspace  }},
  // Line 1
  {  0x0000, 0x0032, 0x0019, 0x0064, {VkKeyNull,           VkKeyNull,           VkKeyNull,           VkKeyNull       }},
  {  0x0019, 0x0032, 0x004B, 0x0064, {L'a',                L'A',                VkKeyF1,             VkKeyF1         }},
  {  0x004B, 0x0032, 0x007D, 0x0064, {L's',                L'S',                VkKeyF2,             VkKeyF2         }},
  {  0x007D, 0x0032, 0x00AF, 0x0064, {L'd',                L'D',                VkKeyF3,             VkKeyF3         }},
  {  0x00AF, 0x0032, 0x00E1, 0x0064, {L'f',                L'F',                VkKeyF4,             VkKeyF4         }},
  {  0x00E1, 0x0032, 0x0113, 0x0064, {L'g',                L'G',                VkKeyF5,             VkKeyF5         }},
  {  0x0113, 0x0032, 0x0145, 0x0064, {L'h',                L'H',                VkKeyF6,             VkKeyF6         }},
  {  0x0145, 0x0032, 0x0177, 0x0064, {L'j',                L'J',                VkKeyF7,             VkKeyF7         }},
  {  0x0177, 0x0032, 0x01A9, 0x0064, {L'k',                L'K',                VkKeyF8,             VkKeyF8         }},
  {  0x01A9, 0x0032, 0x01DB, 0x0064, {L'l',                L'L',                VkKeyF9,             VkKeyF9         }},
  {  0x01DB, 0x0032, 0x020D, 0x0064, {VkKeyF2,             VkKeyF2,             VkKeyF10,            VkKeyF10        }},
  // Line 2
  {  0x0000, 0x0064, 0x004B, 0x0096, {VkKeyCapslock,       VkKeyCapslock,       VkKeyShift,          VkKeyShift      }},
  {  0x004B, 0x0064, 0x007D, 0x0096, {L'z',                L'Z',                L'`',                L'~'            }},
  {  0x007D, 0x0064, 0x00AF, 0x0096, {L'x',                L'X',                L';',                L':'            }},
  {  0x00AF, 0x0064, 0x00E1, 0x0096, {L'c',                L'C',                L'\'',               L'"'            }},
  {  0x00E1, 0x0064, 0x0113, 0x0096, {L'v',                L'V',                L',',                L'<'            }},
  {  0x0113, 0x0064, 0x0145, 0x0096, {L'b',                L'B',                L'.',                L'>'            }},
  {  0x0145, 0x0064, 0x0177, 0x0096, {L'n',                L'N',                L'/',                L'?'            }},
  {  0x0177, 0x0064, 0x01A9, 0x0096, {L'm',                L'M',                VkKeyF11,            VkKeyF11        }},
  {  0x01A9, 0x0064, 0x01DB, 0x0096, {VkKeyUp,             VkKeyUp,             VkKeyF12,            VkKeyF12        }},
  {  0x01DB, 0x0064, 0x0226, 0x0096, {VkKeyEnter,          VkKeyEnter,          VkKeyEnter,          VkKeyEnter      }},
  // Line 3
  {  0x0000, 0x0096, 0x004B, 0x00C8, {VkKeyEsc,            VkKeyEsc,            VkKeyEsc,            VkKeyEsc        }},
  {  0x004B, 0x0096, 0x007D, 0x00C8, {VkKeyTwoPage,        VkKeyTwoPage,        VkKeyTwoPage,        VkKeyTwoPage    }},
  {  0x007D, 0x0096, 0x0177, 0x00C8, {L' ',                L' ',                L' ',                L' '            }},
  {  0x0177, 0x0096, 0x01A9, 0x00C8, {VkKeyLeft,           VkKeyLeft,           L'\\',               L'|'            }},
  {  0x01A9, 0x0096, 0x01DB, 0x00C8, {VkKeyDown,           VkKeyDown,           L'-',                L'_'            }},
  {  0x01DB, 0x0096, 0x020D, 0x00C8, {VkKeyRight,          VkKeyRight,          L'=',                L'+'            }},
  // Screen Corner A
  {  0x0000, 0x0000, 0x001E, 0x001E, {VkKeyTypeMaximum,    VkKeyTypeMaximum,    VkKeyTypeMaximum,    VkKeyTypeMaximum}},
  // Screen Corner B
  {  0x0000, 0x023A, 0x001E, 0x0258, {VkKeyTypeMaximum,    VkKeyTypeMaximum,    VkKeyTypeMaximum,    VkKeyTypeMaximum}},
  // Screen Corner C
  {  0x02E4, 0x0000, 0x0320, 0x001E, {VkKeyTypeMaximum,    VkKeyTypeMaximum,    VkKeyTypeMaximum,    VkKeyTypeMaximum}},
  // Screen Corner D
  {  0x02E4, 0x023A, 0x0320, 0x0258, {VkKeyTypeMaximum,    VkKeyTypeMaximum,    VkKeyTypeMaximum,    VkKeyTypeMaximum}}
};

//
// 1.SimpleKeyboardBody
// Follow below design
//   (1).\KeyboardLayout\SimpleKeyboard.bmp
// Based on keyboard's (startX, startY)
// +-----+-----+
// | Esc |Enter| Line 0
// +-----+-----+
// | Up  | Down| Line 1
// +-----+-----+
//
// 2.Screen Corner
// Follow below design
//   (1).\KeyboardLayout\SimpleIcon.bmp # Screen Corner A/B
//   (2).\KeyboardLayout\FullIcon.bmp   # Screen Corner C/D
// +-+------------------------------------+-+
// |A|                                    |C|
// +-+                                    +-+
// |                                        |
// |                                        |
// |                                        |
// |                                        |
// |                                        |
// |                                        |
// +-+                                    +-+
// |B|                                    |D|
// +-+------------------------------------+-+
//
VK_STRUCT mSimpleKeyboardBody[] = {
  // StartX  StartY  EndX    EndY     Page0Font            Page1Font            Page2Font            Page3Font
  // Line 0
  {  0x0000, 0x0000, 0x0032, 0x0032, {VkKeyEsc,            VkKeyEsc,            VkKeyEsc,            VkKeyEsc        }},
  {  0x0032, 0x0000, 0x0064, 0x0032, {VkKeyEnter,          VkKeyEnter,          VkKeyEnter,          VkKeyEnter      }},
  // Line 1
  {  0x0000, 0x0032, 0x0032, 0x0064, {VkKeyUp,             VkKeyUp,             VkKeyUp,             VkKeyUp         }},
  {  0x0032, 0x0032, 0x0064, 0x0064, {VkKeyDown,           VkKeyDown,           VkKeyDown,           VkKeyDown       }},
  // Screen Corner A
  {  0x0000, 0x0000, 0x001E, 0x001E, {VkKeyTypeMaximum,    VkKeyTypeMaximum,    VkKeyTypeMaximum,    VkKeyTypeMaximum}},
  // Screen Corner B
  {  0x0000, 0x023A, 0x001E, 0x0258, {VkKeyTypeMaximum,    VkKeyTypeMaximum,    VkKeyTypeMaximum,    VkKeyTypeMaximum}},
  // Screen Corner C
  {  0x02E4, 0x0000, 0x0320, 0x001E, {VkKeyTypeMaximum,    VkKeyTypeMaximum,    VkKeyTypeMaximum,    VkKeyTypeMaximum}},
  // Screen Corner D
  {  0x02E4, 0x023A, 0x0320, 0x0258, {VkKeyTypeMaximum,    VkKeyTypeMaximum,    VkKeyTypeMaximum,    VkKeyTypeMaximum}}
};

/**
  Modify the color of key if Shift or CapsLock is pressed.

  @param[in]       VkContext   Address of an VK_CONTEXT structure.
  @param[in, out]  BltBuffer   Address of a blt buffer.

  @retval EFI_SUCCESS          Success for the function.

**/
EFI_STATUS
ModifyShiftKeyColor (
  IN     VK_CONTEXT                    *VkContext,
  IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL **BltBuffer
  )
{
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *TempBltBuffer;
  UINTN                         BltSize;
  BOOLEAN                       IsPressed;

  TempBltBuffer = *BltBuffer;
  BltSize       = VkContext->VkBodyBltHeight * VkContext->VkBodyBltWidth;
  IsPressed     = VkContext->PageNumber <= VkPage1 ?
                  VkContext->IsCapsLockFlag :
                  VkContext->IsShiftKeyFlag;

  while (BltSize-- != 0) {
    //
    // Color gradient issue
    //
    if (((TempBltBuffer->Red - TempBltBuffer->Green) > 0x20) &&
        ((TempBltBuffer->Red - TempBltBuffer->Blue)  > 0x20)) {
      if (IsPressed) {
        TempBltBuffer->Red   = 0;
        TempBltBuffer->Green = 255;
        TempBltBuffer->Blue  = 255;
      } else {
        TempBltBuffer->Red   = 255;
        TempBltBuffer->Green = 255;
        TempBltBuffer->Blue  = 255;
      }
    }
    TempBltBuffer++;
  }

  return EFI_SUCCESS;
}

/**
  Make the keyboard transparent.

  @param[in]      VkContext      Address of an VK_CONTEXT structure.
  @param[in]      IsTransparent  TRUE for set keyboard transparent.
  @param[in]      BltIn          Address of keyboard blt buffer.
  @param[in, out] BltOut         Address of output blt buffer.
                                 NULL will allocate it.

  @retval EFI_SUCCESS            Success for the function.
  @retval EFI_OUT_OF_RESOURCES   Allocate memory failed.

**/
EFI_STATUS
MakeKeyboardTransparent (
  IN     VK_CONTEXT                    *VkContext,
  IN     BOOLEAN                       IsTransparent,
  IN     EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltIn,
  IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL **BltOut
  )
{
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Compound;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Keyboard;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Background;
  UINTN                         BltSize;

  if (*BltOut == NULL) {
    *BltOut = AllocateZeroPool (VkContext->VkBodyBltSize);
    if (*BltOut == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
  }
  Compound   = *BltOut;
  Keyboard   = BltIn;
  Background = VkContext->VkBodyBackgroundBltBuffer;
  BltSize    = VkContext->VkBodyBltHeight * VkContext->VkBodyBltWidth;
  while (BltSize-- != 0) {
    if (IsTransparent) {
      Compound->Red   = (Keyboard->Red * TRANSPARENCY_WEIGHT) / 100 + (Background->Red * (100 - TRANSPARENCY_WEIGHT)) / 100;
      Compound->Green = (Keyboard->Green * TRANSPARENCY_WEIGHT) / 100 + (Background->Green * (100 - TRANSPARENCY_WEIGHT)) / 100;
      Compound->Blue  = (Keyboard->Blue * TRANSPARENCY_WEIGHT) / 100 + (Background->Blue * (100 - TRANSPARENCY_WEIGHT)) / 100;
    } else {
      *Compound = *Keyboard;
    }
    Compound++;
    Keyboard++;
    Background++;
  }

  return EFI_SUCCESS;
}

/**
  Save the background blt buffer.

  @param[in] VkContext           Address of an VK_CONTEXT structure.
  @param[in] BltSize             Size of blt.

  @retval EFI_SUCCESS            Success for the function.
  @retval EFI_OUT_OF_RESOURCES   Allocate memory failed.

**/
EFI_STATUS
EFIAPI
SaveVkBodyBackgroundBltBuffer (
  IN VK_CONTEXT *VkContext,
  IN UINTN      BltSize
  )
{
  EFI_STATUS                    Status;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *CurrentBltBuffer;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *CurrentBltBufferSave;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *OldBltBuffer;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *OldBltBufferSave;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Compound;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Background;
  UINTN                         Size;

  CurrentBltBufferSave = NULL;
  OldBltBufferSave     = NULL;

  //
  // Save original blt buffer first.
  //
  OldBltBuffer = AllocateZeroPool (VkContext->VkBodyBltSize);
  if (OldBltBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  if (VkContext->VkBodyBackgroundBltBuffer != NULL) {
    CopyMem (OldBltBuffer, VkContext->VkBodyBackgroundBltBuffer, VkContext->VkBodyBltSize);
  }

  if (VkContext->VkBodyBackgroundBltBuffer == NULL) {
    VkContext->VkBodyBltSize = BltSize;
    VkContext->VkBodyBackgroundBltBuffer = AllocateZeroPool (VkContext->VkBodyBltSize);
    ASSERT (VkContext->VkBodyBackgroundBltBuffer != NULL);
    if (VkContext->VkBodyBackgroundBltBuffer == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
  } else if (BltSize > VkContext->VkBodyBltSize) {
    VkContext->VkBodyBltSize = BltSize;
    FreePool (VkContext->VkBodyBackgroundBltBuffer);
    VkContext->VkBodyBackgroundBltBuffer = NULL;
    VkContext->VkBodyBackgroundBltBuffer = AllocateZeroPool (VkContext->VkBodyBltSize);
    ASSERT (VkContext->VkBodyBackgroundBltBuffer != NULL);
    if (VkContext->VkBodyBackgroundBltBuffer == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
  } else {
    ZeroMem (VkContext->VkBodyBackgroundBltBuffer, VkContext->VkBodyBltSize);
  }

  CurrentBltBuffer = NULL;
  if (VkContext->IsBackgroundChanged == TRUE) {
    //
    // Background changed, merge current visioning blt buffer with old background blt buffer.
    //
    Compound   = VkContext->VkBodyCompoundBltBuffer;
    Background = VkContext->VkBodyBackgroundBltBuffer;
    CurrentBltBuffer = AllocateZeroPool (VkContext->VkBodyBltSize);
    ASSERT (CurrentBltBuffer != NULL);
    if (CurrentBltBuffer == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    Status = VkContext->GraphicsOutput->Blt (
                                          VkContext->GraphicsOutput,
                                          CurrentBltBuffer,
                                          EfiBltVideoToBltBuffer,
                                          VkContext->VkBodyBltStartX,
                                          VkContext->VkBodyBltStartY,
                                          0,
                                          0,
                                          VkContext->VkBodyBltWidth,
                                          VkContext->VkBodyBltHeight,
                                          VkContext->VkBodyBltWidth * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
                                          );
    CurrentBltBufferSave = CurrentBltBuffer;
    OldBltBufferSave = OldBltBuffer;
    Size = VkContext->VkBodyBltHeight * VkContext->VkBodyBltWidth;
    while (Size-- != 0) {
      if ((Compound->Red   != CurrentBltBuffer->Red) ||
          (Compound->Green != CurrentBltBuffer->Green) ||
          (Compound->Blue  != CurrentBltBuffer->Blue)) {
        *Background = *CurrentBltBuffer;
      } else {
        *Background = *OldBltBuffer;
      }
      Compound++;
      Background++;
      CurrentBltBuffer++;
      OldBltBuffer++;
    }
  } else {
    //
    // Background NOT changed, save it to VkBodyBackgroundBltBuffer directly.
    //
    Status = VkContext->GraphicsOutput->Blt (
                                          VkContext->GraphicsOutput,
                                          VkContext->VkBodyBackgroundBltBuffer,
                                          EfiBltVideoToBltBuffer,
                                          VkContext->VkBodyBltStartX,
                                          VkContext->VkBodyBltStartY,
                                          0,
                                          0,
                                          VkContext->VkBodyBltWidth,
                                          VkContext->VkBodyBltHeight,
                                          VkContext->VkBodyBltWidth * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
                                          );
  }

  if (CurrentBltBufferSave != NULL) {
    FreePool (CurrentBltBufferSave);
  }
  if (OldBltBufferSave != NULL) {
    FreePool (OldBltBufferSave);
  }

  return Status;
}

/**
  Restore the background blt buffer.

  @param[in] VkContext           Address of an VK_CONTEXT structure.

  @retval EFI_SUCCESS            Success for the function.
  @retval EFI_UNSUPPORTED        Input blt buffer is NULL.
  @retval Others                 An unexpected error occurred.

**/
EFI_STATUS
EFIAPI
RestoreVkBodyBackgroundBltBuffer (
  IN VK_CONTEXT *VkContext
  )
{
  EFI_STATUS Status;

  if (VkContext->VkBodyBackgroundBltBuffer == NULL) {
    return EFI_UNSUPPORTED;
  }

  Status = VkContext->GraphicsOutput->Blt (
                                        VkContext->GraphicsOutput,
                                        VkContext->VkBodyBackgroundBltBuffer,
                                        EfiBltBufferToVideo,
                                        0,
                                        0,
                                        VkContext->VkBodyBltStartX,
                                        VkContext->VkBodyBltStartY,
                                        VkContext->VkBodyBltWidth,
                                        VkContext->VkBodyBltHeight,
                                        VkContext->VkBodyBltWidth * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
                                        );

  FreePool (VkContext->VkBodyBackgroundBltBuffer);
  VkContext->VkBodyBackgroundBltBuffer = NULL;

  return Status;
}

/**
  Set the position of character.

  @param[in] VkContext      Address of an VK_CONTEXT structure.
  @param[in] DestX          X position.
  @param[in] DestY          Y position.

  @retval EFI_SUCCESS       Success for the function.
  @retval Others            An unexpected error occurred.

**/
EFI_STATUS
SetCharacterPosition (
  IN VK_CONTEXT *VkContext,
  IN UINT32     DestX,
  IN UINT32     DestY
  )
{
  UINTN     Index;
  VK_STRUCT *KeyArryPtr;
  UINT32    KeyArrySize;

  switch (VkContext->TargetKeyboardDisplay) {
  case VkDisplayAttributeSimpleTop:
  case VkDisplayAttributeSimpleBottom:
    KeyArryPtr  = mSimpleKeyboardBody;
    KeyArrySize = DIM (mSimpleKeyboardBody);
    break;

  case VkDisplayAttributeFullBottom:
  case VkDisplayAttributeFullTop:
    KeyArryPtr = mFullKeyboardBody;
    KeyArrySize = DIM (mFullKeyboardBody);
    break;

  case VkDisplayAttributeNone:
    KeyArryPtr = mFullKeyboardBody;
    KeyArrySize = DIM (mFullKeyboardBody);
    break;

  default:
    return EFI_UNSUPPORTED;
  }

  for (Index = 0; Index < KeyArrySize; Index++) {
    VkContext->KeyboardBodyPtr[Index] = KeyArryPtr[Index];
  }
  VkContext->NumOfKeysInfo = KeyArrySize;

  for (Index = 0; Index < (VkContext->NumOfKeysInfo - 4); Index++) {
    VkContext->KeyboardBodyPtr[Index].DisStartX += (UINT16)DestX;
    VkContext->KeyboardBodyPtr[Index].DisStartY += (UINT16)DestY;
    VkContext->KeyboardBodyPtr[Index].DisEndX   += (UINT16)DestX;
    VkContext->KeyboardBodyPtr[Index].DisEndY   += (UINT16)DestY;
  }

  return EFI_SUCCESS;
}

/**
  Save the icon background blt buffer.

  @param[in] VkContext      Address of an VK_CONTEXT structure.
  @param[in] IconType       The icon type.

  @retval EFI_SUCCESS            Success for the function.
  @retval EFI_OUT_OF_RESOURCES   Allocate memory failed.
  @retval Others                 An unexpected error occurred.

**/
EFI_STATUS
EFIAPI
SaveVkIconBackgroundBltBuffer (
  IN VK_CONTEXT           *VkContext,
  IN VK_DISPLAY_ATTRIBUTE IconType
  )
{
  EFI_STATUS                    Status;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *GraphicBlt;
  UINTN                         BltSize;
  UINTN                         Height;
  UINTN                         Width;
  INTN                          StartX;
  INTN                          StartY;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *TempIconBackBuffer;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *IconBackBuffer;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Compound;
  UINTN                         Size;
  BOOLEAN                       SaveCursor;

  Status             = EFI_SUCCESS;
  StartX             = 0;
  StartY             = 0;
  TempIconBackBuffer = NULL;
  IconBackBuffer     = NULL;
  Compound           = NULL;

  if ((IconType == VkDisplayAttributeFullTop) ||
      (IconType == VkDisplayAttributeSimpleTop)) {
    return EFI_SUCCESS;
  }

  SaveCursor = gST->ConOut->Mode->CursorVisible;
  gST->ConOut->EnableCursor (gST->ConOut, FALSE);

  if (IconType == VkDisplayAttributeFullBottom) {
    GraphicBlt = VkContext->FullIcon->Bitmap;
    BltSize    = VkContext->FullIcon->Height * VkContext->FullIcon->Width * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL);
    Height     = VkContext->FullIcon->Height;
    Width      = VkContext->FullIcon->Width;
    StartX     = VkContext->FullIconBackStartX;
    StartY     = VkContext->FullIconBackStartY;
  } else if (IconType == VkDisplayAttributeSimpleBottom) {
    GraphicBlt = VkContext->SmallIcon->Bitmap;
    BltSize    = VkContext->SmallIcon->Height * VkContext->SmallIcon->Width * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL);
    Height     = VkContext->SmallIcon->Height;
    Width      = VkContext->SmallIcon->Width;
    StartX     = VkContext->SimIconBackStartX;
    StartY     = VkContext->SimIconBackStartY;
  } else {
    gST->ConOut->EnableCursor (gST->ConOut, SaveCursor);
    return EFI_UNSUPPORTED;
  }

  IconBackBuffer = AllocateZeroPool (BltSize);
  if (IconBackBuffer == NULL) {
    gST->ConOut->EnableCursor (gST->ConOut, SaveCursor);
    return EFI_OUT_OF_RESOURCES;
  }
  TempIconBackBuffer = IconBackBuffer;
  Status = VkContext->GraphicsOutput->Blt (
                                        VkContext->GraphicsOutput,
                                        IconBackBuffer,
                                        EfiBltVideoToBltBuffer,
                                        StartX,
                                        StartY,
                                        0,
                                        0,
                                        Width,
                                        Height,
                                        Width * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
                                        );

  if (IconType == VkDisplayAttributeFullBottom) {
    //
    // Store full icon background framebuffer
    //
    VkContext->FullIconBackHeight = Height;
    VkContext->FullIconBackWidth  = Width;
    if (VkContext->FullIconUpdatedFlag == FALSE) {
      //
      // No icon draw, save the buffer directly.
      //
      if (VkContext->FullIconBackBuffer == NULL) {
        VkContext->FullIconBackBuffer = AllocateZeroPool (BltSize);
        VkContext->FullIconBackSize = BltSize;
      }
      CopyMem (VkContext->FullIconBackBuffer, IconBackBuffer, BltSize);
      VkContext->FullIconUpdatedFlag = TRUE;
    } else {
      if (CompareMem (VkContext->FullIconBackBuffer, IconBackBuffer, BltSize) != 0) {
        Compound = VkContext->FullIconBackBuffer;
        Size = Height * Width;
        while (Size-- != 0) {
          if ((GraphicBlt->Red   != IconBackBuffer->Red) ||
              (GraphicBlt->Green != IconBackBuffer->Green) ||
              (GraphicBlt->Blue  != IconBackBuffer->Blue)) {
            *Compound = *IconBackBuffer;
          }
          Compound++;
          GraphicBlt++;
          IconBackBuffer++;
        }
      }
    }
  } else if (IconType == VkDisplayAttributeSimpleBottom) {
    //
    // Store simple icon background framebuffer
    //
    VkContext->SimIconBackHeight = Height;
    VkContext->SimIconBackWidth  = Width;
    if (VkContext->SimIconUpdatedFlag == FALSE) {
      //
      // No icon draw, save the buffer directly.
      //
      if (VkContext->SimIconBackBuffer == NULL) {
        VkContext->SimIconBackBuffer = AllocateZeroPool (BltSize);
        VkContext->SimIconBackSize = BltSize;
      }
      CopyMem (VkContext->SimIconBackBuffer, IconBackBuffer, BltSize);
      VkContext->SimIconUpdatedFlag = TRUE;
    } else {
      if (CompareMem (VkContext->SimIconBackBuffer, IconBackBuffer, BltSize) != 0) {
        Compound = VkContext->SimIconBackBuffer;
        Size = Height * Width;
        while (Size-- != 0) {
          if ((GraphicBlt->Red   != IconBackBuffer->Red) ||
              (GraphicBlt->Green != IconBackBuffer->Green) ||
              (GraphicBlt->Blue  != IconBackBuffer->Blue)) {
            *Compound = *IconBackBuffer;
          }
          Compound++;
          GraphicBlt++;
          IconBackBuffer++;
        }
      }
    }
  }

  if (TempIconBackBuffer != NULL)  FreePool (TempIconBackBuffer);

  gST->ConOut->EnableCursor (gST->ConOut, SaveCursor);
  return Status;
}

/**
  Use to draw the keyboard icon.

  @param[in] VkContext  Pointer to virtual keyboard's context
  @param[in] VkImage    Image of keyboard to display on the screen.
  @param[in] Attribute  Attribute of keyboard to display on the screen.

  @retval EFI_SUCCESS           ConsoleControl has been flipped to graphics and keyboard icon displayed.
  @retval EFI_UNSUPPORTED       KeyboardFile not found
  @retval EFI_INVALID_PARAMETER Attribute is unknown.

**/
EFI_STATUS
EFIAPI
DrawVkIcon (
  IN VK_CONTEXT                    *VkContext,
  IN EFI_IMAGE_INPUT               *VkImage,
  IN VK_DISPLAY_ATTRIBUTE          Attribute
  )
{
  EFI_STATUS                    Status;
  UINT32                        SizeOfX;
  UINT32                        SizeOfY;
  INTN                          DestX;
  INTN                          DestY;
  UINTN                         CoordinateX;
  UINTN                         CoordinateY;
  EFI_GRAPHICS_OUTPUT_PROTOCOL  *GraphicsOutput;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Blt;
  UINTN                         BltSize;
  UINTN                         Height;
  UINTN                         Width;

  Status         = EFI_SUCCESS;
  GraphicsOutput = VkContext->GraphicsOutput;
  SizeOfX        = GraphicsOutput->Mode->Info->HorizontalResolution;
  SizeOfY        = GraphicsOutput->Mode->Info->VerticalResolution;
  CoordinateX    = 0;
  CoordinateY    = 0;
  Height         = VkImage->Height;
  Width          = VkImage->Width;
  Blt            = VkImage->Bitmap;
  BltSize        = sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL) * (UINT32)(Width * Height);

  //
  // Calculate the display position according to Attribute.
  //
  switch (Attribute) {
  case VkDisplayAttributeSimpleTop:
    DestX = CoordinateX;
    DestY = CoordinateY;
    break;

  case VkDisplayAttributeFullTop:
    DestX = (SizeOfX - Width - CoordinateX);
    DestY = CoordinateY;;
    break;

  case VkDisplayAttributeFullBottom:
    DestX                         = (SizeOfX - Width - CoordinateX);
    DestY                         = (SizeOfY - Height - CoordinateY);
    VkContext->FullIconBackStartX = DestX;
    VkContext->FullIconBackStartY = DestY;
    break;

  case VkDisplayAttributeSimpleBottom:
    DestX = CoordinateX;
    DestY = (SizeOfY - Height - CoordinateY);

    //
    // Save to check icon/screen cleared
    //
    if (VkContext->IconBltBuffer == NULL) {
      VkContext->IconBltSize   = BltSize;
      VkContext->IconBltWidth  = Width;
      VkContext->IconBltHeight = Height;
      VkContext->IconBltBuffer = AllocateZeroPool (BltSize);
    }
    CopyMem (VkContext->IconBltBuffer, Blt, VkContext->IconBltSize);

    VkContext->SimIconBackStartX = DestX;
    VkContext->SimIconBackStartY = DestY;
    break;

  case VkDisplayAttributeNone:
    return EFI_SUCCESS;

  default:
    return EFI_INVALID_PARAMETER;
  }

  if ((DestX >= 0) && (DestY >= 0)) {
    //
    // Store icon background framebuffer
    //
    SaveVkIconBackgroundBltBuffer (VkContext, Attribute);
    Status = GraphicsOutput->Blt (
                               GraphicsOutput,
                               Blt,
                               EfiBltBufferToVideo,
                               0,
                               0,
                               (UINTN) DestX,
                               (UINTN) DestY,
                               Width,
                               Height,
                               Width * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
                               );
  }

  return Status;
}

/**
  Use to draw the keyboard.

  @param[in] VkContext  Pointer to virtual keyboard's context
  @param[in] VkImage    Image of keyboard to display on the screen.
  @param[in] Attribute  Attribute of keyboard to display on the screen.

  @retval EFI_SUCCESS           ConsoleControl has been flipped to graphics and keyboard displayed.
  @retval EFI_UNSUPPORTED       KeyboardFile not found
  @retval EFI_INVALID_PARAMETER Attribute is unknown.

**/
EFI_STATUS
EFIAPI
DrawVkBody (
  IN VK_CONTEXT                    *VkContext,
  IN EFI_IMAGE_INPUT               *VkImage,
  IN VK_DISPLAY_ATTRIBUTE          Attribute
  )
{
  EFI_STATUS                    Status;
  UINT32                        SizeOfX;
  UINT32                        SizeOfY;
  INTN                          DestX;
  INTN                          DestY;
  UINTN                         BltSize;
  UINTN                         Height;
  UINTN                         Width;
  UINTN                         CoordinateY;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltIn;
  EFI_GRAPHICS_OUTPUT_PROTOCOL  *GraphicsOutput;

  Status         = EFI_SUCCESS;
  GraphicsOutput = VkContext->GraphicsOutput;
  SizeOfX        = GraphicsOutput->Mode->Info->HorizontalResolution;
  SizeOfY        = GraphicsOutput->Mode->Info->VerticalResolution;
  CoordinateY    = 0;
  Height         = VkImage->Height;
  Width          = VkImage->Width;
  BltSize        = sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL) * (UINT32)(Width * Height);
  BltIn          = AllocateCopyPool (BltSize, VkImage->Bitmap);

  if (BltIn == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Calculate the display position according to Attribute.
  //
  switch (Attribute) {
  case VkDisplayAttributeSimpleTop:
    DestX = ((SizeOfX / 2) - Width) / 4;
    DestY = CoordinateY;
    VkContext->CurrentKeyboardDisplay = VkDisplayAttributeSimpleTop;
    break;

  case VkDisplayAttributeSimpleBottom:
    DestX = ((SizeOfX / 2) - Width) / 4;
    DestY = (SizeOfY - Height - CoordinateY);
    VkContext->CurrentKeyboardDisplay = VkDisplayAttributeSimpleBottom;
    break;

  case VkDisplayAttributeFullTop:
    DestX = (SizeOfX - Width) / 2;
    DestY = CoordinateY;
    VkContext->CurrentKeyboardDisplay = VkDisplayAttributeFullTop;
    break;

  case VkDisplayAttributeFullBottom:
    DestX = (SizeOfX - Width) / 2;
    DestY = (SizeOfY - Height - CoordinateY);
    VkContext->CurrentKeyboardDisplay = VkDisplayAttributeFullBottom;
    break;

  case VkDisplayAttributeNone:
    VkContext->CurrentKeyboardDisplay = VkDisplayAttributeNone;
    goto DVKBODY_Exit;

  default:
    VkContext->CurrentKeyboardDisplay = VkDisplayAttributeNone;
    Status = EFI_INVALID_PARAMETER;
    goto DVKBODY_Exit;
  }
  if ((DestX >= 0) && (DestY >= 0)) {
    SetCharacterPosition (VkContext, (UINT32)DestX, (UINT32)DestY);

    //
    // Store current framebuffer
    //
    VkContext->VkBodyBltStartX = DestX;
    VkContext->VkBodyBltStartY = DestY;
    VkContext->VkBodyBltHeight = Height;
    VkContext->VkBodyBltWidth  = Width;
    SaveVkBodyBackgroundBltBuffer (VkContext, BltSize);

    //
    // Free compound buffer first.
    //
    if (VkContext->VkBodyCompoundBltBuffer != NULL) {
      FreePool (VkContext->VkBodyCompoundBltBuffer);
    }
    VkContext->VkBodyCompoundBltBuffer = NULL;
    ModifyShiftKeyColor (VkContext, &BltIn);
    MakeKeyboardTransparent (VkContext, TRUE, BltIn, &(VkContext->VkBodyCompoundBltBuffer));

    //
    // Draw keyboard body
    //
    Status = GraphicsOutput->Blt (
                               GraphicsOutput,
                               VkContext->VkBodyCompoundBltBuffer,
                               EfiBltBufferToVideo,
                               0,
                               0,
                               (UINTN) DestX,
                               (UINTN) DestY,
                               Width,
                               Height,
                               Width * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
                               );
  }


DVKBODY_Exit:
  if (BltIn != NULL) {
    FreePool (BltIn);
  }

  return Status;
}

/**
  Clear the keyboard body

  @param  VkContext             Code context.

  @retval EFI_SUCCESS           Clear rectangle is done.

**/
EFI_STATUS
HideVkBody (
  IN VK_CONTEXT *VkContext
  )
{
  RestoreVkBodyBackgroundBltBuffer (VkContext);
  VkContext->CurrentKeyboardDisplay = VkDisplayAttributeNone;

  return EFI_SUCCESS;
}


/**
  Clear the keyboard icon

  @param  VkContext             Code context.

  @retval EFI_SUCCESS           Clear rectangle is done.

**/
EFI_STATUS
HideVkIcon (
  IN VK_CONTEXT *VkContext
  )
{
  EFI_STATUS                    Status;

  if ((VkContext->FullIconBackBuffer == NULL) || (VkContext->SimIconBackBuffer == NULL)) {
    return EFI_UNSUPPORTED;
  }

  if ((VkContext->FullIconUpdatedFlag == FALSE) || (VkContext->SimIconUpdatedFlag == FALSE)) {
    return EFI_UNSUPPORTED;
  }
  Status = VkContext->GraphicsOutput->Blt (
                                        VkContext->GraphicsOutput,
                                        VkContext->FullIconBackBuffer,
                                        EfiBltBufferToVideo,
                                        0,
                                        0,
                                        VkContext->FullIconBackStartX,
                                        VkContext->FullIconBackStartY,
                                        VkContext->FullIconBackWidth,
                                        VkContext->FullIconBackHeight,
                                        VkContext->FullIconBackWidth * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
                                        );

  ZeroMem (VkContext->FullIconBackBuffer, VkContext->FullIconBackSize);
  VkContext->FullIconUpdatedFlag = FALSE;


  Status = VkContext->GraphicsOutput->Blt (
                                        VkContext->GraphicsOutput,
                                        VkContext->SimIconBackBuffer,
                                        EfiBltBufferToVideo,
                                        0,
                                        0,
                                        VkContext->SimIconBackStartX,
                                        VkContext->SimIconBackStartY,
                                        VkContext->SimIconBackWidth,
                                        VkContext->SimIconBackHeight,
                                        VkContext->SimIconBackWidth * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
                                        );
  if (!EFI_ERROR (Status)) {
    if (VkContext->ScreenCheckBuffer == NULL) {
      VkContext->ScreenCheckBufferSize = VkContext->SimIconBackHeight *
                                         VkContext->SimIconBackWidth *
                                         sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL);
      VkContext->ScreenCheckBuffer = AllocateZeroPool (VkContext->ScreenCheckBufferSize);
    }

    CopyMem (
      VkContext->ScreenCheckBuffer,
      VkContext->SimIconBackBuffer,
      (VkContext->SimIconBackHeight * VkContext->SimIconBackWidth * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL))
      );
  }
  ZeroMem (VkContext->SimIconBackBuffer, VkContext->SimIconBackSize);
  VkContext->SimIconUpdatedFlag = FALSE;

  return EFI_SUCCESS;
}

/**
  Draw key board on the display

  @param[in] VkContext          Graphic Protocol for draw the alphabet.

  @retval EFI_SUCCESS           Draw keyboard was done.
  @retval EFI_UNSUPPORTED       Did not get key mapping table.

**/
EFI_STATUS
DrawKeyboardLayout (
  IN VK_CONTEXT *VkContext
  )
{
  EFI_STATUS Status;
  if (!VkContext->IsIconShowed) {
    Status = DrawVkIcon (VkContext, VkContext->SmallIcon, VkDisplayAttributeSimpleTop);
    ASSERT_EFI_ERROR (Status);

    Status = DrawVkIcon (VkContext, VkContext->SmallIcon, VkDisplayAttributeSimpleBottom);
    ASSERT_EFI_ERROR (Status);

    Status = DrawVkIcon (VkContext, VkContext->FullIcon,  VkDisplayAttributeFullTop);
    ASSERT_EFI_ERROR (Status);

    Status = DrawVkIcon (VkContext, VkContext->FullIcon,  VkDisplayAttributeFullBottom);
    ASSERT_EFI_ERROR (Status);
    VkContext->IsIconShowed = TRUE;
  }

  if (VkContext->TargetKeyboardDisplay != VkContext->CurrentKeyboardDisplay) {
    switch (VkContext->TargetKeyboardDisplay) {
    case VkDisplayAttributeSimpleTop:
    case VkDisplayAttributeSimpleBottom:
      DrawVkBody (VkContext, VkContext->SimKeyBody, VkContext->TargetKeyboardDisplay);
      break;

    case VkDisplayAttributeFullTop:
    case VkDisplayAttributeFullBottom:
      if (VkContext->PageNumber <= VkPage1) {
        DrawVkBody (VkContext, VkContext->CapLeKeyBody, VkContext->TargetKeyboardDisplay);
      } else {
        DrawVkBody (VkContext, VkContext->DigKeyBody, VkContext->TargetKeyboardDisplay);
      }

    case VkDisplayAttributeNone:
      break;

    default:
      break;
    }
  }

  return EFI_SUCCESS;
}

/**
  Set the keyboard layout.

  @param[in] VkContext          Graphic Protocol for draw the alphabet.
  @param[in] Index              The layout selected.

  @retval EFI_SUCCESS           Draw keyboard was done.
  @retval Others                An unexpected error occurred.

**/
EFI_STATUS
KeyboardLayoutHandler (
  IN VK_CONTEXT *VkContext,
  IN UINT32     Index
  )
{
  EFI_STATUS Status;

  Status = EFI_SUCCESS;

  if (Index == (VkContext->NumOfKeysInfo - 4)) {
    //
    // Touch the LeftTop icon
    //
    if (VkContext->CurrentKeyboardDisplay == VkDisplayAttributeSimpleTop) {
      VkContext->TargetKeyboardDisplay = VkDisplayAttributeNone;
    } else {
      VkContext->TargetKeyboardDisplay = VkDisplayAttributeSimpleTop;
    }
  } else if (Index == (VkContext->NumOfKeysInfo - 3)) {
    //
    // Touch the LeftBottom icon
    //
    if (VkContext->CurrentKeyboardDisplay == VkDisplayAttributeSimpleBottom) {
      VkContext->TargetKeyboardDisplay = VkDisplayAttributeNone;
    } else {
      VkContext->TargetKeyboardDisplay = VkDisplayAttributeSimpleBottom;
    }
  } else if (Index == (VkContext->NumOfKeysInfo - 2)) {
    //
    // Touch the RightTop icon
    //
    if (VkContext->CurrentKeyboardDisplay == VkDisplayAttributeFullTop) {
      VkContext->TargetKeyboardDisplay = VkDisplayAttributeNone;
    } else {
      VkContext->TargetKeyboardDisplay = VkDisplayAttributeFullTop;
    }
  } else {
    //
    // Touch the RightBottom icon
    //
    if (VkContext->CurrentKeyboardDisplay == VkDisplayAttributeFullBottom) {
      VkContext->TargetKeyboardDisplay = VkDisplayAttributeNone;
    } else {
      VkContext->TargetKeyboardDisplay = VkDisplayAttributeFullBottom;
    }
  }

  if (VkContext->TargetKeyboardDisplay == VkDisplayAttributeNone) {
    //
    // Just hide the current keyboard
    //
    HideVkBody (VkContext);
    VkContext->KeyTouchedTimeOut = VK_REPEAT_TIMEOUT;

  } else {
    //
    // If current keyboard status is NOT none,
    // hide current keyboard first and then draw the target keyboard
    //
    if (VkContext->CurrentKeyboardDisplay != VkDisplayAttributeNone) {
      HideVkBody (VkContext);
    }
    Status = DrawKeyboardLayout (VkContext);
  }

  return Status;
}

/**
  This routine is used to check if icon has been cleared.

  @param[in] VkContext  Pointer to virtual keyboard's context

  @retval EFI_SUCCESS   Function completed.

**/
EFI_STATUS
CheckIconCleared (
  IN VK_CONTEXT *VkContext
  )
{
  EFI_STATUS                    Status;
  UINT32                        VerticalResolution;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer;

  Status = EFI_SUCCESS;
  BltBuffer = NULL;
  VkContext->IconReDrawCheck++;
  if (VkContext->IconReDrawCheck <= 10) {
    //
    // Check it every 10 * 100ms.
    //
    return Status;
  }

  //
  // Check if right-bottomed region is black, if yes, clean screen happened, need to re-draw keyboard.
  //
  VerticalResolution    = VkContext->GraphicsOutput->Mode->Info->VerticalResolution;
  BltBuffer             = AllocateZeroPool (VkContext->IconBltSize);
  if (BltBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Status = VkContext->GraphicsOutput->Blt (
                                        VkContext->GraphicsOutput,
                                        BltBuffer,
                                        EfiBltVideoToBltBuffer,
                                        0,
                                        (VerticalResolution - VkContext->IconBltHeight),
                                        0,
                                        0,
                                        VkContext->IconBltWidth,
                                        VkContext->IconBltHeight,
                                        VkContext->IconBltWidth * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
                                        );
  if (EFI_ERROR (Status)) {
    FreePool (BltBuffer);
    return Status;
  }
  VkContext->IsIconShowed = TRUE;
  if (VkContext->IconBltBuffer == NULL) {
    //
    // No icon has been drawn.
    //
    VkContext->IsIconShowed = FALSE;
  } else {
    if (CompareMem (BltBuffer, VkContext->IconBltBuffer, VkContext->IconBltSize) != 0) {
      //
      // Icon has been overridden, need to re-draw.
      //
      VkContext->IsIconShowed = FALSE;
    }
  }

  if (VkContext->IsIconShowed == FALSE) {
    if (VkContext->ScreenCheckBuffer != NULL) {
      if (CompareMem (BltBuffer, VkContext->ScreenCheckBuffer, VkContext->IconBltSize) == 0) {
        //
        // Icon has been overridden, force to re-draw the keyboard.
        //
        Status = DrawKeyboardLayout (VkContext);
      } else {
        //
        // Save blt buffer of icon position and use it to check if icon is overridden.
        //
        CopyMem (VkContext->ScreenCheckBuffer, BltBuffer, VkContext->IconBltSize);
      }
    } else {
      //
      // Draw the keyboard.
      //
      Status = DrawKeyboardLayout (VkContext);
    }
  }

  if (BltBuffer != NULL) {
    FreePool (BltBuffer);
  }

  VkContext->IconReDrawCheck = 0;

  return Status;
}

/**
  ConvertCoordinate - Convert the touch panel's coordinate to display's coordinate.

  @param[in]  VkContext             Virtual Keyboard context.
  @param[in]  Point                 The coordinate reported from touch panel.
  @param[out] TouchX                The coordinate X converted to display panel.
  @param[out] TouchY                The coordinate Y converted to display panel..

  @retval EFI_SUCCESS               Convert success.

**/
EFI_STATUS
ConvertCoordinate (
  IN  VK_CONTEXT                 *VkContext,
  IN  EFI_ABSOLUTE_POINTER_STATE Point,
  OUT UINT32                     *TouchX,
  OUT UINT32                     *TouchY
  )
{
  UINT64 AbsoluteMaxX;
  UINT64 AbsoluteMaxY;
  UINT32 HorizontalResolution;
  UINT32 VerticalResolution;

  AbsoluteMaxX         = VkContext->AbsolutePointer->Mode->AbsoluteMaxX;
  AbsoluteMaxY         = VkContext->AbsolutePointer->Mode->AbsoluteMaxY;
  HorizontalResolution = VkContext->GraphicsOutput->Mode->Info->HorizontalResolution;
  VerticalResolution   = VkContext->GraphicsOutput->Mode->Info->VerticalResolution;
  *TouchX              = (UINT32) MultU64x32 (Point.CurrentX, HorizontalResolution) / (UINT32) AbsoluteMaxX;
  *TouchY              = (UINT32) MultU64x32 (Point.CurrentY, VerticalResolution) / (UINT32) AbsoluteMaxY;

  return EFI_SUCCESS;
}

/**
  This routine is used to check if screen has been cleared.

  @param[in] VkContext  Pointer to virtual keyboard's context

  @retval EFI_SUCCESS   Function completed.

**/
EFI_STATUS
CheckScreenCleared (
  IN VK_CONTEXT *VkContext
  )
{
  EFI_STATUS                    Status;
  UINT32                        HorizontalResolution;
  UINT32                        VerticalResolution;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBufferIndex;
  UINTN                         BltSize;
  BOOLEAN                       IsScreenCleared;

  //
  // Check left-bottom side.
  // If IconBltBuffer is null, checking is meaningless.
  //
  if (VkContext->IconBltBuffer == NULL) {
    return EFI_SUCCESS;
  }

  IsScreenCleared = FALSE;
  Status          = EFI_SUCCESS;
  if ((gST->ConOut->Mode->CursorColumn == 0) && (gST->ConOut->Mode->CursorRow == 0)) {
    //
    // System may call gST->ConOut->ClearScreen
    //
    HorizontalResolution  = VkContext->GraphicsOutput->Mode->Info->HorizontalResolution;
    VerticalResolution    = VkContext->GraphicsOutput->Mode->Info->VerticalResolution;
    BltBuffer             = AllocateZeroPool (VkContext->IconBltSize);
    if (BltBuffer == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    BltBufferIndex = BltBuffer;
    Status = VkContext->GraphicsOutput->Blt (
                                          VkContext->GraphicsOutput,
                                          BltBuffer,
                                          EfiBltVideoToBltBuffer,
                                          (HorizontalResolution - VkContext->IconBltWidth),
                                          (VerticalResolution - VkContext->IconBltHeight),
                                          0,
                                          0,
                                          VkContext->IconBltWidth,
                                          VkContext->IconBltHeight,
                                          VkContext->IconBltWidth * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
                                          );
    if (EFI_ERROR (Status)) {
      FreePool (BltBuffer);
      return Status;
    }
    BltSize = VkContext->IconBltHeight * VkContext->IconBltWidth;
    IsScreenCleared = TRUE;
    while (BltSize-- != 0) {
      if ((BltBufferIndex->Red != 0) || (BltBufferIndex->Green != 0) || (BltBufferIndex->Blue != 0)) {
        IsScreenCleared = FALSE;
        break;
      }
      BltBufferIndex++;
    }
    FreePool (BltBuffer);
  }

  if (IsScreenCleared) {
    VkContext->IsIconShowed   = FALSE;
    VkContext->CurrentKeyboardDisplay = VkDisplayAttributeNone;

    if (VkContext->VkBodyBackgroundBltBuffer != NULL) {
      FreePool (VkContext->VkBodyBackgroundBltBuffer);
      VkContext->VkBodyBackgroundBltBuffer = NULL;
    }

    if (VkContext->VkBodyCompoundBltBuffer != NULL) {
      FreePool (VkContext->VkBodyCompoundBltBuffer);
      VkContext->VkBodyCompoundBltBuffer = NULL;
    }

    if (VkContext->IconBltBuffer != NULL) {
      FreePool (VkContext->IconBltBuffer);
      VkContext->IconBltBuffer = NULL;
    }

  }

  return Status;
}

/**
  This routine is used to check if background beneath virtual keyboard has been cleared.

  @param[in] VkContext  Pointer to virtual keyboard's context

  @retval EFI_SUCCESS   Function completed.

**/
EFI_STATUS
CheckBackgroundChanged (
  IN VK_CONTEXT *VkContext
  )
{
  EFI_STATUS                    Status;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer;

  Status                         = EFI_SUCCESS;
  VkContext->IsBackgroundChanged = FALSE;

  if ((VkContext->CurrentKeyboardDisplay == VkDisplayAttributeNone) || (VkContext->VkBodyCompoundBltBuffer == NULL)) {
    return EFI_SUCCESS;
  }

  BltBuffer = AllocateZeroPool (VkContext->VkBodyBltSize);
  if (BltBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Status = VkContext->GraphicsOutput->Blt (
                                        VkContext->GraphicsOutput,
                                        BltBuffer,
                                        EfiBltVideoToBltBuffer,
                                        VkContext->VkBodyBltStartX,
                                        VkContext->VkBodyBltStartY,
                                        0,
                                        0,
                                        VkContext->VkBodyBltWidth,
                                        VkContext->VkBodyBltHeight,
                                        VkContext->VkBodyBltWidth * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
                                        );
  if (EFI_ERROR (Status)) {
    FreePool (BltBuffer);
    return Status;
  }
  if (CompareMem (BltBuffer, VkContext->VkBodyCompoundBltBuffer, VkContext->VkBodyBltSize) != 0) {
    VkContext->IsBackgroundChanged = TRUE;
    VkContext->CurrentKeyboardDisplay = VkDisplayAttributeNone;
    DrawKeyboardLayout (VkContext);
  }

  if (BltBuffer != NULL) {
    FreePool (BltBuffer);
  }

  return Status;
}

/**
  Get unicode by VkContext->PageNumber and VkContext->KeyboardBodyPtr.

  @param[in]  VkContext            Address of an VK_CONTEXT structure.
  @param[in]  KeyItem              Key Item.
  @param[out] FontPtr              Follow VkContext->PageNumber to translate
                                   font unicode.

  @retval EFI_SUCCESS              Finish translating FontPtr.
  @retval EFI_INVALID_PARAMETER    VkContext or FontPtr is NULL.

**/
EFI_STATUS
VkGetMappingFont (
  IN  VK_CONTEXT *VkContext,
  IN  VK_STRUCT  KeyItem,
  OUT UINT32     *FontPtr
  )
{
  if ((VkContext == NULL) || (FontPtr == NULL) || (VkContext->PageNumber>=VkPageMaximum)) {
    return EFI_INVALID_PARAMETER;
  }
  *FontPtr = (UINT32) KeyItem.PageFont[VkContext->PageNumber];

  return EFI_SUCCESS;
}
