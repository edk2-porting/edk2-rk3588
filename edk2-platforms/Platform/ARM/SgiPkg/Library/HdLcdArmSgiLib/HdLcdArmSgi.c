/** @file
*
*  Copyright (c) 2018 - 2020, Arm Limited. All rights reserved.<BR>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/LcdPlatformLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/Cpu.h>

typedef struct {
  UINT32                      OscFreq;
  SCAN_TIMINGS                Horizontal;
  SCAN_TIMINGS                Vertical;
} DISPLAY_MODE;

STATIC DISPLAY_MODE mDisplayModes[] = {
  {
    // SVGA : 800 x 600 x 24 bpp.
    SVGA_OSC_FREQUENCY,
    {SVGA_H_RES_PIXELS, SVGA_H_SYNC, SVGA_H_BACK_PORCH, SVGA_H_FRONT_PORCH},
    {SVGA_V_RES_PIXELS, SVGA_V_SYNC, SVGA_V_BACK_PORCH, SVGA_V_FRONT_PORCH}
  }
};

STATIC CONST UINT32 mMaxMode = ARRAY_SIZE (mDisplayModes);

/** HDLCD platform specific initialization function.

  @param[in] Handle              Handle to the LCD device instance.

  @retval EFI_SUCCESS            Plaform library initialized successfully.
  @retval EFI_UNSUPPORTED        PcdGopPixelFormat must be
                                 PixelRedGreenBlueReserved8BitPerColor OR
                                 PixelBlueGreenRedReserved8BitPerColor
                                 any other format is not supported.
  @retval other                  Other errors.
**/
EFI_STATUS
LcdPlatformInitializeDisplay (
  IN  EFI_HANDLE  Handle
  )
{
  EFI_GRAPHICS_PIXEL_FORMAT PixelFormat;

  // PixelBitMask and PixelBltOnly pixel formats are not supported.
  PixelFormat = FixedPcdGet32 (PcdGopPixelFormat);
  if (PixelFormat != PixelRedGreenBlueReserved8BitPerColor &&
      PixelFormat != PixelBlueGreenRedReserved8BitPerColor) {

    ASSERT (PixelFormat == PixelRedGreenBlueReserved8BitPerColor ||
      PixelFormat == PixelBlueGreenRedReserved8BitPerColor);
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/** Allocate VRAM memory in DRAM for the framebuffer

  The allocated address can be used to set the framebuffer.

  @param[out] VramBaseAddress     A pointer to the framebuffer address.
  @param[out] VramSize            A pointer to the size of the framebuffer
                                  in bytes

  @retval EFI_SUCCESS             Framebuffer memory allocated successfully.
  @retval other                   Other errors.
**/
EFI_STATUS
LcdPlatformGetVram (
  OUT EFI_PHYSICAL_ADDRESS  *VramBaseAddress,
  OUT UINTN                 *VramSize
  )
{
  EFI_STATUS             Status;
  EFI_CPU_ARCH_PROTOCOL  *Cpu;

  ASSERT (VramBaseAddress != NULL);
  ASSERT (VramSize != NULL);

  // Set the VRAM size.
  *VramSize = (UINTN)FixedPcdGet32 (PcdArmLcdDdrFrameBufferSize);

  // Check if base address is already reserved for the framebuffer.
  *VramBaseAddress =
     (EFI_PHYSICAL_ADDRESS)FixedPcdGet64 (PcdArmLcdDdrFrameBufferBase);

  if (*VramBaseAddress == 0) {
    // If not already reserved, attempt to allocate the VRAM from the DRAM.
    Status = gBS->AllocatePages (
                    AllocateAnyPages,
                    EfiReservedMemoryType,
                    EFI_SIZE_TO_PAGES (*VramSize),
                    VramBaseAddress);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "HdLcdArmSgi: Failed to allocate memory for"
        "frame buffer.\n"));
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
  }

  // Ensure the Cpu architectural protocol is already installed
  Status = gBS->LocateProtocol (
                  &gEfiCpuArchProtocolGuid,
                  NULL,
                  (VOID **)&Cpu);
  ASSERT_EFI_ERROR (Status);

  // The VRAM is inside the DRAM, which is cacheable.
  // Mark the VRAM as write-combining (uncached) and non-executable.
  Status = Cpu->SetMemoryAttributes (
                  Cpu,
                  *VramBaseAddress,
                  *VramSize,
                  EFI_MEMORY_WC | EFI_MEMORY_XP);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    gBS->FreePages (*VramBaseAddress, EFI_SIZE_TO_PAGES (*VramSize));
  }

  return Status;
}

/** Return total number of modes supported.

  @retval UINT32             Mode Number.
**/
UINT32
LcdPlatformGetMaxMode (VOID)
{
  return  mMaxMode;
}

/** Set the requested display mode.

  @param[in] ModeNumber            Mode Number.

  @retval EFI_SUCCESS              Mode set successfully.
  @retval EFI_INVALID_PARAMETER    Requested mode not found.
**/
EFI_STATUS
LcdPlatformSetMode (
  IN  UINT32  ModeNumber
  )
{
  if (ModeNumber >= mMaxMode) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

/** Return information for the requested mode number.

  @param[in]  ModeNumber          Mode Number.

  @param[out] Info                Pointer for returned mode information
                                  (on success).

  @retval EFI_SUCCESS             Mode information for the requested mode
                                  returned successfully.
  @retval EFI_INVALID_PARAMETER   Requested mode not found.
**/
EFI_STATUS
LcdPlatformQueryMode (
  IN  UINT32                                ModeNumber,
  OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  *Info
  )
{
  if (ModeNumber >= mMaxMode) {
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (Info != NULL);

  Info->Version = 0;
  Info->HorizontalResolution = mDisplayModes[ModeNumber].Horizontal.Resolution;
  Info->VerticalResolution = mDisplayModes[ModeNumber].Vertical.Resolution;
  Info->PixelsPerScanLine = mDisplayModes[ModeNumber].Horizontal.Resolution;
  Info->PixelFormat = FixedPcdGet32 (PcdGopPixelFormat);

  return EFI_SUCCESS;
}

/** Return display timing information for the requested mode number.

  @param[in]  ModeNumber          Mode Number.

  @param[out] Horizontal          Pointer to horizontal timing parameters.
                                  (Resolution, Sync, Back porch, Front porch)
  @param[out] Vertical            Pointer to vertical timing parameters.
                                  (Resolution, Sync, Back porch, Front porch)

  @retval EFI_SUCCESS             Display timing information for the requested
                                  mode returned successfully.
  @retval EFI_INVALID_PARAMETER   Requested mode not found.
**/
EFI_STATUS
LcdPlatformGetTimings (
  IN  UINT32        ModeNumber,
  OUT SCAN_TIMINGS  **Horizontal,
  OUT SCAN_TIMINGS  **Vertical
  )
{
  if (ModeNumber >= mMaxMode) {
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (Horizontal != NULL);
  ASSERT (Vertical != NULL);

  *Horizontal = &mDisplayModes[ModeNumber].Horizontal;
  *Vertical   = &mDisplayModes[ModeNumber].Vertical;

  // Pretend that clock probing and get timings are successful for SGI FVP
  return EFI_SUCCESS;
}

/** Return bits per pixel information for a mode number.

  @param[in]  ModeNumber          Mode Number.

  @param[out] Bpp                 Pointer to bits per pixel information.

  @retval EFI_SUCCESS             Bits per pixel information for the requested
                                  mode returned successfully.
  @retval EFI_INVALID_PARAMETER   Requested mode not found.
**/
EFI_STATUS
LcdPlatformGetBpp (
  IN  UINT32   ModeNumber,
  OUT LCD_BPP  *Bpp
  )
{
  if (ModeNumber >= mMaxMode) {
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (Bpp != NULL);

  *Bpp = LcdBitsPerPixel_24;

  return EFI_SUCCESS;
}
