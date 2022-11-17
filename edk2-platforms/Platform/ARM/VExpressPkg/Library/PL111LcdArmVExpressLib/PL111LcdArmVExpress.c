/** @file

  Copyright (c) 2011 - 2020, Arm Limited. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>

#include <Library/ArmPlatformSysConfigLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/LcdPlatformLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/Cpu.h>
#include <Protocol/EdidDiscovered.h>
#include <Protocol/EdidActive.h>

#include <ArmPlatform.h>

typedef struct {
  UINT32                     Mode;
  LCD_BPP                    Bpp;
  UINT32                     OscFreq;

  SCAN_TIMINGS               Horizontal;
  SCAN_TIMINGS               Vertical;
} DISPLAY_MODE;

/** The display modes supported by the platform.
**/
STATIC DISPLAY_MODE mDisplayModes[] = {
  { // Mode 0 : VGA : 640 x 480 x 24 bpp
    VGA, LcdBitsPerPixel_24,
    VGA_OSC_FREQUENCY,
    {VGA_H_RES_PIXELS, VGA_H_SYNC, VGA_H_BACK_PORCH, VGA_H_FRONT_PORCH},
    {VGA_V_RES_PIXELS, VGA_V_SYNC, VGA_V_BACK_PORCH, VGA_V_FRONT_PORCH}
  },
  { // Mode 1 : SVGA : 800 x 600 x 24 bpp
    SVGA, LcdBitsPerPixel_24,
    SVGA_OSC_FREQUENCY,
    {SVGA_H_RES_PIXELS, SVGA_H_SYNC, SVGA_H_BACK_PORCH, SVGA_H_FRONT_PORCH},
    {SVGA_V_RES_PIXELS, SVGA_V_SYNC, SVGA_V_BACK_PORCH, SVGA_V_FRONT_PORCH}
  },
  { // Mode 2 : XGA : 1024 x 768 x 24 bpp
    XGA, LcdBitsPerPixel_24,
    XGA_OSC_FREQUENCY,
    {XGA_H_RES_PIXELS, XGA_H_SYNC, XGA_H_BACK_PORCH, XGA_H_FRONT_PORCH},
    {XGA_V_RES_PIXELS, XGA_V_SYNC, XGA_V_BACK_PORCH, XGA_V_FRONT_PORCH}
  },
  { // Mode 3 : SXGA : 1280 x 1024 x 24 bpp
    SXGA, LcdBitsPerPixel_24,
    (SXGA_OSC_FREQUENCY/2),
    {SXGA_H_RES_PIXELS, SXGA_H_SYNC, SXGA_H_BACK_PORCH, SXGA_H_FRONT_PORCH},
    {SXGA_V_RES_PIXELS, SXGA_V_SYNC, SXGA_V_BACK_PORCH, SXGA_V_FRONT_PORCH}
  },
  { // Mode 4 : UXGA : 1600 x 1200 x 24 bpp
    UXGA, LcdBitsPerPixel_24,
    (UXGA_OSC_FREQUENCY/2),
    {UXGA_H_RES_PIXELS, UXGA_H_SYNC, UXGA_H_BACK_PORCH, UXGA_H_FRONT_PORCH},
    {UXGA_V_RES_PIXELS, UXGA_V_SYNC, UXGA_V_BACK_PORCH, UXGA_V_FRONT_PORCH}
  },
  { // Mode 5 : HD : 1920 x 1080 x 24 bpp
    HD, LcdBitsPerPixel_24,
    (HD_OSC_FREQUENCY/2),
    {HD_H_RES_PIXELS, HD_H_SYNC, HD_H_BACK_PORCH, HD_H_FRONT_PORCH},
    {HD_V_RES_PIXELS, HD_V_SYNC, HD_V_BACK_PORCH, HD_V_FRONT_PORCH}
  },
  { // Mode 6 : VGA : 640 x 480 x 16 bpp (565 Mode)
    VGA, LcdBitsPerPixel_16_565,
    VGA_OSC_FREQUENCY,
    {VGA_H_RES_PIXELS, VGA_H_SYNC, VGA_H_BACK_PORCH, VGA_H_FRONT_PORCH},
    {VGA_V_RES_PIXELS, VGA_V_SYNC, VGA_V_BACK_PORCH, VGA_V_FRONT_PORCH}
  },
  { // Mode 7 : SVGA : 800 x 600 x 16 bpp (565 Mode)
    SVGA, LcdBitsPerPixel_16_565,
    SVGA_OSC_FREQUENCY,
    {SVGA_H_RES_PIXELS, SVGA_H_SYNC, SVGA_H_BACK_PORCH, SVGA_H_FRONT_PORCH},
    {SVGA_V_RES_PIXELS, SVGA_V_SYNC, SVGA_V_BACK_PORCH, SVGA_V_FRONT_PORCH}
  },
  { // Mode 8 : XGA : 1024 x 768 x 16 bpp (565 Mode)
    XGA, LcdBitsPerPixel_16_565,
    XGA_OSC_FREQUENCY,
    {XGA_H_RES_PIXELS, XGA_H_SYNC, XGA_H_BACK_PORCH, XGA_H_FRONT_PORCH},
    {XGA_V_RES_PIXELS, XGA_V_SYNC, XGA_V_BACK_PORCH, XGA_V_FRONT_PORCH}
  },
  { // Mode 9 : VGA : 640 x 480 x 15 bpp
    VGA, LcdBitsPerPixel_16_555,
    VGA_OSC_FREQUENCY,
    {VGA_H_RES_PIXELS, VGA_H_SYNC, VGA_H_BACK_PORCH, VGA_H_FRONT_PORCH},
    {VGA_V_RES_PIXELS, VGA_V_SYNC, VGA_V_BACK_PORCH, VGA_V_FRONT_PORCH}
  },
  { // Mode 10 : SVGA : 800 x 600 x 15 bpp
    SVGA, LcdBitsPerPixel_16_555,
    SVGA_OSC_FREQUENCY,
    {SVGA_H_RES_PIXELS, SVGA_H_SYNC, SVGA_H_BACK_PORCH, SVGA_H_FRONT_PORCH},
    {SVGA_V_RES_PIXELS, SVGA_V_SYNC, SVGA_V_BACK_PORCH, SVGA_V_FRONT_PORCH}
  },
  { // Mode 11 : XGA : 1024 x 768 x 15 bpp
    XGA, LcdBitsPerPixel_16_555,
    XGA_OSC_FREQUENCY,
    {XGA_H_RES_PIXELS, XGA_H_SYNC, XGA_H_BACK_PORCH, XGA_H_FRONT_PORCH},
    {XGA_V_RES_PIXELS, XGA_V_SYNC, XGA_V_BACK_PORCH, XGA_V_FRONT_PORCH}
  },
  { // Mode 12 : XGA : 1024 x 768 x 15 bpp - All the timing info is derived from Linux Kernel Driver Settings
    XGA, LcdBitsPerPixel_16_555,
    63500000,
    {XGA_H_RES_PIXELS, XGA_H_SYNC, XGA_H_BACK_PORCH, XGA_H_FRONT_PORCH},
    {XGA_V_RES_PIXELS, XGA_V_SYNC, XGA_V_BACK_PORCH, XGA_V_FRONT_PORCH}
  },
  { // Mode 13 : VGA : 640 x 480 x 12 bpp (444 Mode)
    VGA, LcdBitsPerPixel_12_444,
    VGA_OSC_FREQUENCY,
    {VGA_H_RES_PIXELS, VGA_H_SYNC, VGA_H_BACK_PORCH, VGA_H_FRONT_PORCH},
    {VGA_V_RES_PIXELS, VGA_V_SYNC, VGA_V_BACK_PORCH, VGA_V_FRONT_PORCH}
  },
  { // Mode 14 : SVGA : 800 x 600 x 12 bpp (444 Mode)
    SVGA, LcdBitsPerPixel_12_444,
    SVGA_OSC_FREQUENCY,
    {SVGA_H_RES_PIXELS, SVGA_H_SYNC, SVGA_H_BACK_PORCH, SVGA_H_FRONT_PORCH},
    {SVGA_V_RES_PIXELS, SVGA_V_SYNC, SVGA_V_BACK_PORCH, SVGA_V_FRONT_PORCH}
  },
  { // Mode 15 : XGA : 1024 x 768 x 12 bpp (444 Mode)
    XGA, LcdBitsPerPixel_12_444,
    XGA_OSC_FREQUENCY,
    {XGA_H_RES_PIXELS, XGA_H_SYNC, XGA_H_BACK_PORCH, XGA_H_FRONT_PORCH},
    {XGA_V_RES_PIXELS, XGA_V_SYNC, XGA_V_BACK_PORCH, XGA_V_FRONT_PORCH}
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

/** PL111 Platform specific initialization function.

  @param[in] Handle              Handle to the LCD device instance.

  @retval EFI_SUCCESS            Plaform library initialized successfully.
  @retval EFI_UNSUPPORTED        PcdGopPixelFormat must be
                                 PixelRedGreenBlueReserved8BitPerColor OR
                                 PixelBlueGreenRedReserved8BitPerColor
                                 any other format is not supported
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

  // Set the FPGA multiplexer to select the video output from the motherboard
  // or the daughterboard
  Status = ArmPlatformSysConfigSet (SYS_CFG_MUXFPGA, PL111_CLCD_SITE);
  if (!EFI_ERROR (Status)) {
    // Install the EDID Protocols
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &Handle,
                    &gEfiEdidDiscoveredProtocolGuid,
                    &mEdidDiscovered,
                    &gEfiEdidActiveProtocolGuid,
                    &mEdidActive,
                    NULL
                    );
  }

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
  EFI_CPU_ARCH_PROTOCOL   *Cpu;

  ASSERT (VramBaseAddress != NULL);
  ASSERT (VramSize != NULL);

  // Is it on the motherboard or on the daughterboard?
  switch (PL111_CLCD_SITE) {

  case ARM_VE_MOTHERBOARD_SITE:
    *VramBaseAddress = (EFI_PHYSICAL_ADDRESS)PL111_CLCD_VRAM_MOTHERBOARD_BASE;
    *VramSize = LCD_VRAM_SIZE;
    Status = EFI_SUCCESS;
    break;

  case ARM_VE_DAUGHTERBOARD_1_SITE:
    *VramBaseAddress = (EFI_PHYSICAL_ADDRESS)LCD_VRAM_CORE_TILE_BASE;
    *VramSize = LCD_VRAM_SIZE;

    // Allocate the VRAM from the DRAM so that nobody else uses it.
    Status = gBS->AllocatePages (
                    AllocateAddress,
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
    }
    break;

  default:
    // Unsupported site
    Status = EFI_UNSUPPORTED;
    break;
  }

  return Status;
}

/** Return total number of modes supported.

  Note: Valid mode numbers are 0 to MaxMode - 1
  See Section 12.9 of the UEFI Specification 2.7

  @retval UINT32             Mode Number.
**/
UINT32
LcdPlatformGetMaxMode (VOID)
{
  // The following line would correctly report the total number
  // of graphics modes supported by the PL111CLCD.
  // return (sizeof(mResolutions) / sizeof(CLCD_RESOLUTION)) - 1;

  // However, on some platforms it is desirable to ignore some graphics modes.
  // This could be because the specific implementation of PL111 has
  // certain limitations.

  // Set the maximum mode allowed
  return (FixedPcdGet32 (PcdPL111LcdMaxMode));
}

/** Set the requested display mode.

  @param[in] ModeNumber            Mode Number.

  @retval  EFI_SUCCESS             Mode set successfully.
  @retval  EFI_INVALID_PARAMETER   Requested mode not found.
  @retval  EFI_UNSUPPORTED         PLL111 configuration not supported.
  @retval  !(EFI_SUCCESS)          Other errors.
**/
EFI_STATUS
LcdPlatformSetMode (
  IN UINT32                         ModeNumber
  )
{
  EFI_STATUS            Status;
  UINT32                OscillatorId;
  SYS_CONFIG_FUNCTION   Function;
  UINT32                SysId;

  if (ModeNumber >= LcdPlatformGetMaxMode ()) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  switch (PL111_CLCD_SITE) {
  case ARM_VE_MOTHERBOARD_SITE:
    Function = SYS_CFG_OSC;
    OscillatorId = PL111_CLCD_MOTHERBOARD_VIDEO_MODE_OSC_ID;
    break;
  case ARM_VE_DAUGHTERBOARD_1_SITE:
    Function = SYS_CFG_OSC_SITE1;
    OscillatorId = FixedPcdGet32 (PcdPL111LcdVideoModeOscId);
    break;
  default:
    return EFI_UNSUPPORTED;
  }

  // Set the video mode oscillator
  Status = ArmPlatformSysConfigSetDevice (
             Function,
             OscillatorId,
             mDisplayModes[ModeNumber].OscFreq
             );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  // The FVP foundation model does not have an LCD.
  // On the FVP models, the build variant is encoded in bits [15:12].
  // Note: The DVI Mode is not modelled by RTSM or FVP models.
  SysId = MmioRead32 (ARM_VE_SYS_ID_REG);
  if (SysId != ARM_RTSM_SYS_ID) {
    // Ignore build variant and revision.
    SysId &= ~(ARM_FVP_SYS_ID_VARIANT_MASK | ARM_FVP_SYS_ID_REV_MASK);
    if (SysId != ARM_FVP_BASE_BOARD_SYS_ID) {
      // Set the DVI into the new mode
      Status = ArmPlatformSysConfigSet (
                 SYS_CFG_DVIMODE,
                 mDisplayModes[ModeNumber].Mode
                 );
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
        return Status;
      }
    }
  }

  // Set the multiplexer
  Status = ArmPlatformSysConfigSet (SYS_CFG_MUXFPGA, PL111_CLCD_SITE);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/** Return information for the requested mode number.

  @param[in]  ModeNumber         Mode Number.
  @param[out] Info               Pointer for returned mode information
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
  ASSERT (Bpp != NULL);

  if (ModeNumber >= LcdPlatformGetMaxMode ()) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  *Bpp = mDisplayModes[ModeNumber].Bpp;

  return EFI_SUCCESS;
}
