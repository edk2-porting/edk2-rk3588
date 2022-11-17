/** @file
 *
 *  Copyright (c) 2006 - 2016, Intel Corporation. All rights reserved.
 *  Copyright (c) 2018, Andrei Warkentin <andrey.warkentin@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

/*
 * Loosely based on CrScreenShotDxe (https://github.com/LongSoft/CrScreenshotDxe).
 *
 * Copyright (c) 2016, Nikolaj Schlej, All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "DisplayDxe.h"
#include <Protocol/SimpleFileSystem.h>
#include <Library/PrintLib.h>
#include <Library/BmpSupportLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

/*
 * ShowStatus defs.
 */
#define STATUS_SQUARE_SIDE 5
#define STATUS_YELLOW 0xff, 0xff, 0x00
#define STATUS_GREEN  0x00, 0xff, 0x00
#define STATUS_BLUE   0x00, 0x00, 0xff
#define STATUS_RED    0xff, 0x00, 0x00

EFI_STATUS
ShowStatus (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput,
  IN UINT8 Red,
  IN UINT8 Green,
  IN UINT8 Blue
  )
{
  UINTN Index;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL Square[STATUS_SQUARE_SIDE * STATUS_SQUARE_SIDE];
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL Backup[STATUS_SQUARE_SIDE * STATUS_SQUARE_SIDE];

  for (Index = 0; Index < STATUS_SQUARE_SIDE * STATUS_SQUARE_SIDE; Index++) {
    Square[Index].Blue = Blue;
    Square[Index].Green = Green;
    Square[Index].Red = Red;
    Square[Index].Reserved = 0x00;
  }

  // Backup current image.
  GraphicsOutput->Blt (GraphicsOutput, Backup,
                    EfiBltVideoToBltBuffer, 0, 0, 0, 0,
                    STATUS_SQUARE_SIDE, STATUS_SQUARE_SIDE, 0);

  // Draw the status square.
  GraphicsOutput->Blt (GraphicsOutput, Square,
                    EfiBltBufferToVideo, 0, 0, 0, 0,
                    STATUS_SQUARE_SIDE, STATUS_SQUARE_SIDE, 0);

  // Wait 500ms.
  gBS->Stall (500 * 1000);

  // Restore the backup.
  GraphicsOutput->Blt (GraphicsOutput, Backup,
                    EfiBltBufferToVideo, 0, 0, 0, 0,
                    STATUS_SQUARE_SIDE, STATUS_SQUARE_SIDE, 0);
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
FindWritableFs (
  OUT EFI_FILE_PROTOCOL **WritableFs
  )
{
  EFI_FILE_PROTOCOL *Fs = NULL;
  EFI_HANDLE *HandleBuffer = NULL;
  UINTN      HandleCount;
  UINTN      Index;

  EFI_STATUS Status = gBS->LocateHandleBuffer (ByProtocol,
                             &gEfiSimpleFileSystemProtocolGuid,
                             NULL, &HandleCount, &HandleBuffer);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFs = NULL;
    EFI_FILE_PROTOCOL *File = NULL;

    Status = gBS->HandleProtocol (HandleBuffer[Index],
                    &gEfiSimpleFileSystemProtocolGuid,
                    (VOID**)&SimpleFs);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      /*
       * Not supposed to happen.
       */
      continue;
    }

    Status = SimpleFs->OpenVolume (SimpleFs, &Fs);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a OpenVolume[%u] returned %r\n", __FUNCTION__, Index, Status));
      continue;
    }

    Status = Fs->Open (Fs, &File, L"--------.---",
                   EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ |
                   EFI_FILE_MODE_WRITE, 0);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a Open[%u] returned %r\n", __FUNCTION__, Index, Status));
      continue;
    }

    /*
     * Okay, we have a writable filesystem!
     */
    Fs->Delete (File);
    *WritableFs = Fs;
    Status = EFI_SUCCESS;
    break;
  }

  if (HandleBuffer) {
    FreePool (HandleBuffer);
  }

  return Status;
}

STATIC
VOID
TakeScreenshot (
  VOID
  )
{
  VOID *BmpImage = NULL;
  EFI_FILE_PROTOCOL *Fs = NULL;
  EFI_FILE_PROTOCOL *File = NULL;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput = &gDisplayProto;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Image = NULL;
  EFI_STATUS Status;
  CHAR16 FileName[8 + 1 + 3 + 1];
  UINT32 ScreenWidth;
  UINT32 ScreenHeight;
  UINTN ImageSize;
  UINTN BmpSize;
  UINTN Index;
  EFI_TIME Time;

  Status = FindWritableFs (&Fs);
  if (EFI_ERROR (Status)) {
    ShowStatus (GraphicsOutput, STATUS_YELLOW);
  }

  ScreenWidth = GraphicsOutput->Mode->Info->HorizontalResolution;
  ScreenHeight = GraphicsOutput->Mode->Info->VerticalResolution;
  ImageSize = ScreenWidth * ScreenHeight;

  Status = gRT->GetTime (&Time, NULL);
  if (!EFI_ERROR (Status)) {
    UnicodeSPrint (FileName, sizeof (FileName), L"%02d%02d%02d%02d.bmp",
      Time.Day, Time.Hour, Time.Minute, Time.Second);
  } else {
    UnicodeSPrint (FileName, sizeof (FileName), L"scrnshot.bmp");
  }

  Image = AllocatePool (ImageSize * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
  if (Image == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    ShowStatus (GraphicsOutput, STATUS_RED);
    goto Done;
  }

  Status = GraphicsOutput->Blt (GraphicsOutput, Image,
                             EfiBltVideoToBltBuffer, 0, 0, 0, 0,
                             ScreenWidth, ScreenHeight, 0);
  if (EFI_ERROR (Status)) {
    ShowStatus (GraphicsOutput, STATUS_RED);
    goto Done;
  }

  for (Index = 0; Index < ImageSize; Index++) {
    if (Image[Index].Red != 0x00 ||
        Image[Index].Green != 0x00 ||
        Image[Index].Blue != 0x00) {
      break;
    }
  }

  if (Index == ImageSize) {
    ShowStatus (GraphicsOutput, STATUS_BLUE);
    goto Done;
  }

  Status = TranslateGopBltToBmp (Image, ScreenHeight, ScreenWidth,
             &BmpImage, (UINT32*)&BmpSize);
  if (EFI_ERROR (Status)) {
    ShowStatus (GraphicsOutput, STATUS_RED);
    goto Done;
  }

  Status = Fs->Open (Fs, &File, FileName, EFI_FILE_MODE_CREATE |
                 EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);
  if (EFI_ERROR (Status)) {
    ShowStatus (GraphicsOutput, STATUS_RED);
    goto Done;
  }

  Status = File->Write (File, &BmpSize, BmpImage);
  File->Close (File);
  if (EFI_ERROR (Status)) {
    ShowStatus (GraphicsOutput, STATUS_RED);
    goto Done;
  }

  ShowStatus (GraphicsOutput, STATUS_GREEN);

Done:
  if (BmpImage != NULL) {
    FreePool (BmpImage);
  }

  if (Image != NULL) {
    FreePool (Image);
  }
}

STATIC
EFI_STATUS
EFIAPI
ScreenshotKeyHandler (
  IN EFI_KEY_DATA *KeyData
  )
{
  TakeScreenshot ();
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
ProcessScreenshotHandler (
  IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleTextInEx
  )
{
  EFI_STATUS Status;
  EFI_HANDLE Handle;
  EFI_KEY_DATA ScreenshotKey;

  /*
   * LCtrl+LAlt+F12
   */
  ScreenshotKey.Key.ScanCode = SCAN_F12;
  ScreenshotKey.Key.UnicodeChar = 0;
  ScreenshotKey.KeyState.KeyShiftState = EFI_SHIFT_STATE_VALID |
    EFI_LEFT_CONTROL_PRESSED | EFI_LEFT_ALT_PRESSED;
  ScreenshotKey.KeyState.KeyToggleState = 0;

  Status = SimpleTextInEx->RegisterKeyNotify (
                             SimpleTextInEx,
                             &ScreenshotKey,
                             ScreenshotKeyHandler,
                             &Handle
                           );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: couldn't register key notification: %r\n", __FUNCTION__, Status));
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC
VOID
ProcessScreenshotHandlers (
  VOID
  )
{
  UINTN Index;
  EFI_STATUS Status;
  UINTN HandleCount;
  EFI_HANDLE *HandleBuffer;
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleTextInEx;

  Status = gBS->LocateHandleBuffer (ByProtocol, &gEfiSimpleTextInputExProtocolGuid,
                  NULL, &HandleCount, &HandleBuffer);
  if (EFI_ERROR (Status)) {
    return;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (HandleBuffer[Index],
                    &gEfiSimpleTextInputExProtocolGuid,
                    (VOID**)&SimpleTextInEx);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      /*
       * Not supposed to happen.
       */
      continue;
    }

    Status = ProcessScreenshotHandler (SimpleTextInEx);
    if (EFI_ERROR (Status)) {
      continue;
    }
  }
}

STATIC
VOID
EFIAPI
OnTextInExInstall (
  IN EFI_EVENT Event,
  IN VOID *Context
  )
{
  ProcessScreenshotHandlers ();
}

VOID
RegisterScreenshotHandlers (
  VOID
)
{
  EFI_STATUS Status;
  EFI_EVENT TextInExInstallEvent;
  VOID *TextInExInstallRegistration;

  ProcessScreenshotHandlers ();

  Status = gBS->CreateEvent (EVT_NOTIFY_SIGNAL, TPL_CALLBACK,
                  OnTextInExInstall, NULL,
                  &TextInExInstallEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: couldn't create protocol install event: %r\n", __FUNCTION__, Status));
    return;
  }

  Status = gBS->RegisterProtocolNotify (&gEfiSimpleTextInputExProtocolGuid,
                  TextInExInstallEvent,
                  &TextInExInstallRegistration);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: couldn't register protocol install notify: %r\n", __FUNCTION__, Status));
    return;
  }
}
