/** @file

  Copyright (c) 2025, Mario Bălănică <mariobalanica02@gmail.com>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BaseMemoryLib.h>

#include "LcdGraphicsOutputDxe.h"

EFI_STATUS
EFIAPI
LcdGraphicsBlt (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL       *This,
  IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL  *BltBuffer  OPTIONAL,
  IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION  BltOperation,
  IN UINTN                              SourceX,
  IN UINTN                              SourceY,
  IN UINTN                              DestinationX,
  IN UINTN                              DestinationY,
  IN UINTN                              Width,
  IN UINTN                              Height,
  IN UINTN                              Delta       OPTIONAL
  )
{
  UINT32  *FrameBuffer;
  UINT32  HorizontalResolution;
  UINT32  VerticalResolution;
  UINTN   WidthInBytes;
  UINT32  *SourceBuffer;
  UINT32  *DestinationBuffer;
  UINTN   Y;

  FrameBuffer          = (UINT32 *)This->Mode->FrameBufferBase;
  HorizontalResolution = This->Mode->Info->HorizontalResolution;
  VerticalResolution   = This->Mode->Info->VerticalResolution;
  WidthInBytes         = Width * RK_BYTES_PER_PIXEL;

  if ((Width == 0) || (Height == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((BltBuffer == NULL) &&
      ((BltOperation == EfiBltVideoFill) ||
       (BltOperation == EfiBltBufferToVideo) ||
       (BltOperation == EfiBltVideoToBltBuffer)))
  {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  //
  // If we are reading data out of the video buffer,
  // check that the source area is within the display limits.
  //
  if ((BltOperation == EfiBltVideoToBltBuffer) ||
      (BltOperation == EfiBltVideoToVideo))
  {
    if ((SourceY + Height > VerticalResolution) ||
        (SourceX + Width > HorizontalResolution))
    {
      return EFI_INVALID_PARAMETER;
    }
  }

  //
  // If we are writing data into the video buffer,
  // check that the destination area is within the display limits.
  //
  if ((BltOperation == EfiBltVideoFill) ||
      (BltOperation == EfiBltBufferToVideo) ||
      (BltOperation == EfiBltVideoToVideo))
  {
    if ((DestinationY + Height > VerticalResolution) ||
        (DestinationX + Width > HorizontalResolution))
    {
      return EFI_INVALID_PARAMETER;
    }
  }

  //
  // Perform the Block Transfer Operation
  //
  switch (BltOperation) {
    case EfiBltVideoFill:
      SourceBuffer = (UINT32 *)BltBuffer;

      for (Y = 0; Y < Height; Y++) {
        DestinationBuffer = FrameBuffer +
                            (DestinationY + Y) * HorizontalResolution +
                            DestinationX;

        SetMem32 (DestinationBuffer, WidthInBytes, *SourceBuffer);
      }

      break;

    case EfiBltVideoToBltBuffer:
      if (Delta == 0) {
        Delta = WidthInBytes;
      }

      for (Y = 0; Y < Height; Y++) {
        SourceBuffer = FrameBuffer +
                       (SourceY + Y) * HorizontalResolution +
                       SourceX;

        DestinationBuffer = (UINT32 *)((UINTN)BltBuffer +
                                       (DestinationY + Y) * Delta +
                                       DestinationX * RK_BYTES_PER_PIXEL);

        CopyMem (DestinationBuffer, SourceBuffer, WidthInBytes);
      }

      break;

    case EfiBltBufferToVideo:
      if (Delta == 0) {
        Delta = WidthInBytes;
      }

      for (Y = 0; Y < Height; Y++) {
        SourceBuffer = (UINT32 *)((UINTN)BltBuffer +
                                  (SourceY + Y) * Delta +
                                  SourceX * RK_BYTES_PER_PIXEL);

        DestinationBuffer = FrameBuffer +
                            (DestinationY + Y) * HorizontalResolution +
                            DestinationX;

        CopyMem (DestinationBuffer, SourceBuffer, WidthInBytes);
      }

      break;

    case EfiBltVideoToVideo:
      if (SourceY < DestinationY) {
        for (Y = Height; Y-- > 0;) {
          SourceBuffer = FrameBuffer +
                         (SourceY + Y) * HorizontalResolution +
                         SourceX;

          DestinationBuffer = FrameBuffer +
                              (DestinationY + Y) * HorizontalResolution +
                              DestinationX;

          CopyMem (DestinationBuffer, SourceBuffer, WidthInBytes);
        }
      } else {
        for (Y = 0; Y < Height; Y++) {
          SourceBuffer = FrameBuffer +
                         (SourceY + Y) * HorizontalResolution +
                         SourceX;

          DestinationBuffer = FrameBuffer +
                              (DestinationY + Y) * HorizontalResolution +
                              DestinationX;

          CopyMem (DestinationBuffer, SourceBuffer, WidthInBytes);
        }
      }

      break;

    default:
      return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}
