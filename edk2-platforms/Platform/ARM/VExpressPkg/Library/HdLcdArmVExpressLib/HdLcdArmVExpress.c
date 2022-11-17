/** @file

  Copyright (c) 2012 - 2020, Arm Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>

#include <Library/ArmPlatformSysConfigLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/LcdPlatformLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/Cpu.h>
#include <Protocol/EdidDiscovered.h>
#include <Protocol/EdidActive.h>

#include <ArmPlatform.h>

typedef struct {
  UINT32                     Mode;
  UINT32                     OscFreq;

  // These are used by HDLCD
  SCAN_TIMINGS               Horizontal;
  SCAN_TIMINGS               Vertical;
} DISPLAY_MODE;

/** The display modes supported by the platform.
**/
STATIC DISPLAY_MODE mDisplayModes[] = {
  { // Mode 0 : VGA : 640 x 480 x 24 bpp
    VGA,
    VGA_OSC_FREQUENCY,
    {VGA_H_RES_PIXELS, VGA_H_SYNC, VGA_H_BACK_PORCH, VGA_H_FRONT_PORCH},
    {VGA_V_RES_PIXELS, VGA_V_SYNC, VGA_V_BACK_PORCH, VGA_V_FRONT_PORCH}
  },
  { // Mode 1 : SVGA : 800 x 600 x 24 bpp
    SVGA,
    SVGA_OSC_FREQUENCY,
    {SVGA_H_RES_PIXELS, SVGA_H_SYNC, SVGA_H_BACK_PORCH, SVGA_H_FRONT_PORCH},
    {SVGA_V_RES_PIXELS, SVGA_V_SYNC, SVGA_V_BACK_PORCH, SVGA_V_FRONT_PORCH}
  },
  { // Mode 2 : XGA : 1024 x 768 x 24 bpp
    XGA,
    XGA_OSC_FREQUENCY,
    {XGA_H_RES_PIXELS, XGA_H_SYNC, XGA_H_BACK_PORCH, XGA_H_FRONT_PORCH},
    {XGA_V_RES_PIXELS, XGA_V_SYNC, XGA_V_BACK_PORCH, XGA_V_FRONT_PORCH}
  },
  { // Mode 3 : SXGA : 1280 x 1024 x 24 bpp
    SXGA,
    (SXGA_OSC_FREQUENCY/2),
    {SXGA_H_RES_PIXELS, SXGA_H_SYNC, SXGA_H_BACK_PORCH, SXGA_H_FRONT_PORCH},
    {SXGA_V_RES_PIXELS, SXGA_V_SYNC, SXGA_V_BACK_PORCH, SXGA_V_FRONT_PORCH}
  },
  { // Mode 4 : UXGA : 1600 x 1200 x 24 bpp
    UXGA,
    (UXGA_OSC_FREQUENCY/2),
    {UXGA_H_RES_PIXELS, UXGA_H_SYNC, UXGA_H_BACK_PORCH, UXGA_H_FRONT_PORCH},
    {UXGA_V_RES_PIXELS, UXGA_V_SYNC, UXGA_V_BACK_PORCH, UXGA_V_FRONT_PORCH}
  },
  { // Mode 5 : HD : 1920 x 1080 x 24 bpp
    HD,
    (HD_OSC_FREQUENCY/2),
    {HD_H_RES_PIXELS, HD_H_SYNC, HD_H_BACK_PORCH, HD_H_FRONT_PORCH},
    {HD_V_RES_PIXELS, HD_V_SYNC, HD_V_BACK_PORCH, HD_V_FRONT_PORCH}
  }
};

EFI_EDID_DISCOVERED_PROTOCOL  mEdidDiscovered = {
  0,
  NULL
};

EFI_EDID_ACTIVE_PROTOCOL      mEdidActive = {
  0,
  NULL
};

/** HDLCD platform specific initialization function.

  @param[in] Handle              Handle to the LCD device instance.

  @retval EFI_SUCCESS            Plaform library initialized successfully.
  @retval EFI_UNSUPPORTED        PcdGopPixelFormat must be
                                 PixelRedGreenBlueReserved8BitPerColor OR
                                 PixelBlueGreenRedReserved8BitPerColor
                                 any other format is not supported.
  @retval !(EFI_SUCCESS)         Other errors.
**/
EFI_STATUS
LcdPlatformInitializeDisplay (
  IN EFI_HANDLE   Handle
  )
{
  EFI_STATUS  Status;
  EFI_GRAPHICS_PIXEL_FORMAT PixelFormat;

  // PixelBitMask and PixelBltOnly pixel formats are not supported
  PixelFormat = FixedPcdGet32 (PcdGopPixelFormat);
  if (PixelFormat != PixelRedGreenBlueReserved8BitPerColor &&
      PixelFormat != PixelBlueGreenRedReserved8BitPerColor) {

    ASSERT (PixelFormat == PixelRedGreenBlueReserved8BitPerColor ||
      PixelFormat == PixelBlueGreenRedReserved8BitPerColor);
    return EFI_UNSUPPORTED;
  }

  // Set the FPGA multiplexer to select the video output from the
  // motherboard or the daughterboard
  Status = ArmPlatformSysConfigSet (
             SYS_CFG_MUXFPGA,
             ARM_VE_DAUGHTERBOARD_1_SITE
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Install the EDID Protocols
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gEfiEdidDiscoveredProtocolGuid,
                  &mEdidDiscovered,
                  &gEfiEdidActiveProtocolGuid,
                  &mEdidActive,
                  NULL
                  );

  return Status;
}

/** Allocate VRAM memory in DRAM for the framebuffer
  (unless it is reserved already).

  The allocated address can be used to set the framebuffer.

  @param[out] VramBaseAddress     A pointer to the framebuffer address.
  @param[out] VramSize            A pointer to the size of the framebuffer
                                  in bytes

  @retval EFI_SUCCESS             Framebuffer memory allocated successfully.
  @retval !(EFI_SUCCESS)          Other errors.
**/
EFI_STATUS
LcdPlatformGetVram (
  OUT EFI_PHYSICAL_ADDRESS*  VramBaseAddress,
  OUT UINTN*                 VramSize
  )
{
  EFI_STATUS              Status;
  EFI_ALLOCATE_TYPE       AllocationType;
  EFI_CPU_ARCH_PROTOCOL   *Cpu;

  ASSERT (VramBaseAddress != NULL);
  ASSERT (VramSize != NULL);

  // Set the vram size
  *VramSize = LCD_VRAM_SIZE;

  *VramBaseAddress = (EFI_PHYSICAL_ADDRESS)LCD_VRAM_CORE_TILE_BASE;

  // Allocate the VRAM from the DRAM so that nobody else uses it.
  if (*VramBaseAddress == 0) {
    AllocationType = AllocateAnyPages;
  } else {
    AllocationType = AllocateAddress;
  }
  Status = gBS->AllocatePages (
                  AllocationType,
                  EfiReservedMemoryType,
                  EFI_SIZE_TO_PAGES (((UINTN)LCD_VRAM_SIZE)),
                  VramBaseAddress
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  // Ensure the Cpu architectural protocol is already installed
  Status = gBS->LocateProtocol (
                  &gEfiCpuArchProtocolGuid,
                  NULL,
                  (VOID **)&Cpu
                  );
  if (!EFI_ERROR (Status)) {
    // The VRAM is inside the DRAM, which is cacheable.
    // Mark the VRAM as write-combining (uncached) and non-executable.
    Status = Cpu->SetMemoryAttributes (
                    Cpu,
                    *VramBaseAddress,
                    *VramSize,
                    EFI_MEMORY_WC | EFI_MEMORY_XP
                    );
  }
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    gBS->FreePages (*VramBaseAddress, EFI_SIZE_TO_PAGES (*VramSize));
    return Status;
  }

  return EFI_SUCCESS;
}

/** Return total number of modes supported.

  Note: Valid mode numbers are 0 to MaxMode - 1
  See Section 12.9 of the UEFI Specification 2.7

  @retval UINT32             Mode Number.
**/
UINT32
LcdPlatformGetMaxMode (VOID)
{
  // The following line will report correctly the total number of graphics modes
  // that could be supported by the graphics driver
  return (sizeof (mDisplayModes) / sizeof (DISPLAY_MODE));
}

/** Set the requested display mode.

  @param[in] ModeNumber             Mode Number.

  @retval EFI_SUCCESS               Mode set successfully.
  @retval EFI_INVALID_PARAMETER     Requested mode not found.
  @retval !(EFI_SUCCESS)            Other errors.
**/
EFI_STATUS
LcdPlatformSetMode (
  IN UINT32                         ModeNumber
  )
{
  EFI_STATUS            Status;

  if (ModeNumber >= LcdPlatformGetMaxMode ()) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  // Set the video mode oscillator
  Status = ArmPlatformSysConfigSetDevice (
             SYS_CFG_OSC_SITE1,
             FixedPcdGet32 (PcdHdLcdVideoModeOscId),
             mDisplayModes[ModeNumber].OscFreq
             );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  // Set the DVI into the new mode
  Status = ArmPlatformSysConfigSet (
             SYS_CFG_DVIMODE,
             mDisplayModes[ModeNumber].Mode
             );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  // Set the multiplexer
  Status = ArmPlatformSysConfigSet (
             SYS_CFG_MUXFPGA,
             ARM_VE_DAUGHTERBOARD_1_SITE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
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
  ASSERT (Info != NULL);

  if (ModeNumber >= LcdPlatformGetMaxMode ()) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

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
  IN  UINT32                  ModeNumber,
  OUT SCAN_TIMINGS         ** Horizontal,
  OUT SCAN_TIMINGS         ** Vertical
  )
{
  // One of the pointers is NULL
  ASSERT (Horizontal != NULL);
  ASSERT (Vertical != NULL);

  if (ModeNumber >= LcdPlatformGetMaxMode ()) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  *Horizontal = &mDisplayModes[ModeNumber].Horizontal;
  *Vertical   = &mDisplayModes[ModeNumber].Vertical;

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
  IN  UINT32                              ModeNumber,
  OUT LCD_BPP  *                          Bpp
  )
{
  if (ModeNumber >= LcdPlatformGetMaxMode ()) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  *Bpp = LcdBitsPerPixel_24;

  return EFI_SUCCESS;
}
