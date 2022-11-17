/** @file

  The file contains Arm Mali DP platform specific implementation.

  Copyright (c) 2017 - 2020, Arm Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>
#include <ArmPlatform.h>
#include <Library/DebugLib.h>
#include <Library/LcdPlatformLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/Cpu.h>

/** Check an address is within 40 bits.

  The ARM Mali DP framebuffer address size can not be wider than 40 bits
**/
#define  DP_VALID_BASE_ADDR(Address)  ((Address >> 40) == 0)

typedef struct {
  UINT32                      OscFreq;
  SCAN_TIMINGS                Horizontal;
  SCAN_TIMINGS                Vertical;
} DISPLAY_MODE;

/** The display modes implemented by this driver.

  On Models, the OSC frequencies (listed for each mode below) are not used.
  However these frequencies are useful on hardware plaforms where related
  clock (or PLL) settings are based on these pixel clocks.

  Since the clock settings are defined externally, the driver must
  communicate pixel clock frequencies to relevant modules
  responsible for setting clocks. e.g. SCP.
**/
STATIC DISPLAY_MODE mDisplayModes[] = {
  {
    // Mode 0 : VGA : 640 x 480 x 24 bpp.
    VGA_OSC_FREQUENCY,
    {VGA_H_RES_PIXELS, VGA_H_SYNC, VGA_H_BACK_PORCH, VGA_H_FRONT_PORCH},
    {VGA_V_RES_PIXELS, VGA_V_SYNC, VGA_V_BACK_PORCH, VGA_V_FRONT_PORCH}
  },
  {
    // Mode 1 : WVGA : 800 x 480 x 24 bpp.
    WVGA_OSC_FREQUENCY,
    {WVGA_H_RES_PIXELS, WVGA_H_SYNC, WVGA_H_BACK_PORCH, WVGA_H_FRONT_PORCH},
    {WVGA_V_RES_PIXELS, WVGA_V_SYNC, WVGA_V_BACK_PORCH, WVGA_V_FRONT_PORCH}
  },
  {
    // Mode 2 : SVGA : 800 x 600 x 24 bpp.
    SVGA_OSC_FREQUENCY,
    {SVGA_H_RES_PIXELS, SVGA_H_SYNC, SVGA_H_BACK_PORCH, SVGA_H_FRONT_PORCH},
    {SVGA_V_RES_PIXELS, SVGA_V_SYNC, SVGA_V_BACK_PORCH, SVGA_V_FRONT_PORCH}
  },
  {
    // Mode 3 : QHD : 960 x 540 x 24 bpp.
    QHD_OSC_FREQUENCY,
    {QHD_H_RES_PIXELS, QHD_H_SYNC, QHD_H_BACK_PORCH, QHD_H_FRONT_PORCH},
    {QHD_V_RES_PIXELS, QHD_V_SYNC, QHD_V_BACK_PORCH, QHD_V_FRONT_PORCH}
  },
  {
    // Mode 4 : WSVGA : 1024 x 600 x 24 bpp.
    WSVGA_OSC_FREQUENCY,
    {WSVGA_H_RES_PIXELS, WSVGA_H_SYNC, WSVGA_H_BACK_PORCH, WSVGA_H_FRONT_PORCH},
    {WSVGA_V_RES_PIXELS, WSVGA_V_SYNC, WSVGA_V_BACK_PORCH, WSVGA_V_FRONT_PORCH}
  },
  {
    // Mode 5 : XGA : 1024 x 768 x 24 bpp.
    XGA_OSC_FREQUENCY,
    {XGA_H_RES_PIXELS, XGA_H_SYNC, XGA_H_BACK_PORCH, XGA_H_FRONT_PORCH},
    {XGA_V_RES_PIXELS, XGA_V_SYNC, XGA_V_BACK_PORCH, XGA_V_FRONT_PORCH}
  },
  {
    // Mode 6 : HD : 1280 x 720 x 24 bpp.
    HD720_OSC_FREQUENCY,
    {HD720_H_RES_PIXELS, HD720_H_SYNC, HD720_H_BACK_PORCH, HD720_H_FRONT_PORCH},
    {HD720_V_RES_PIXELS, HD720_V_SYNC, HD720_V_BACK_PORCH, HD720_V_FRONT_PORCH}
  },
  {
    // Mode 7 : WXGA : 1280 x 800 x 24 bpp.
    WXGA_OSC_FREQUENCY,
    {WXGA_H_RES_PIXELS, WXGA_H_SYNC, WXGA_H_BACK_PORCH, WXGA_H_FRONT_PORCH},
    {WXGA_V_RES_PIXELS, WXGA_V_SYNC, WXGA_V_BACK_PORCH, WXGA_V_FRONT_PORCH}
  },
  { // Mode 8 : SXGA : 1280 x 1024 x 24 bpp.
    SXGA_OSC_FREQUENCY,
    {SXGA_H_RES_PIXELS, SXGA_H_SYNC, SXGA_H_BACK_PORCH, SXGA_H_FRONT_PORCH},
    {SXGA_V_RES_PIXELS, SXGA_V_SYNC, SXGA_V_BACK_PORCH, SXGA_V_FRONT_PORCH}
  },
  { // Mode 9 : WSXGA+ : 1680 x 1050 x 24 bpp.
    WSXGA_OSC_FREQUENCY,
    {WSXGA_H_RES_PIXELS, WSXGA_H_SYNC, WSXGA_H_BACK_PORCH, WSXGA_H_FRONT_PORCH},
    {WSXGA_V_RES_PIXELS,WSXGA_V_SYNC, WSXGA_V_BACK_PORCH, WSXGA_V_FRONT_PORCH}
  },
  {
    // Mode 10 : HD : 1920 x 1080 x 24 bpp.
    HD_OSC_FREQUENCY,
    {HD_H_RES_PIXELS, HD_H_SYNC, HD_H_BACK_PORCH, HD_H_FRONT_PORCH},
    {HD_V_RES_PIXELS, HD_V_SYNC, HD_V_BACK_PORCH, HD_V_FRONT_PORCH}
  }
};

/** If PcdArmMaliDpMaxMode is 0, platform supports full range of modes
  else platform supports modes from 0 to PcdArmMaliDpMaxMode - 1
**/
STATIC CONST UINT32 mMaxMode = ((FixedPcdGet32 (PcdArmMaliDpMaxMode) != 0)
                                   ? FixedPcdGet32 (PcdArmMaliDpMaxMode)
                                   : sizeof (mDisplayModes) / sizeof (DISPLAY_MODE));

/** Platform related initialization function.

  @param[in] Handle             Handle to the instance of the device.

  @retval EFI_SUCCESS           Initialization of platform library successful.
  @retval EFI_UNSUPPORTED       PcdGopPixelFormat must be
                                PixelRedGreenBlueReserved8BitPerColor OR
                                PixelBlueGreenRedReserved8BitPerColor
                                any other format is not supported.
**/
EFI_STATUS
LcdPlatformInitializeDisplay (
  IN EFI_HANDLE  Handle
  )
{
  EFI_GRAPHICS_PIXEL_FORMAT PixelFormat;

  (VOID)Handle;

  // PixelBitMask and PixelBltOnly pixel formats are not supported
  PixelFormat = FixedPcdGet32 (PcdGopPixelFormat);
  if (PixelFormat != PixelRedGreenBlueReserved8BitPerColor &&
      PixelFormat != PixelBlueGreenRedReserved8BitPerColor) {

    ASSERT (PixelFormat == PixelRedGreenBlueReserved8BitPerColor ||
      PixelFormat == PixelBlueGreenRedReserved8BitPerColor);
   return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/* Internal helper function to allocate memory if memory is not already
  reserved for framebuffer

  @param[in]  VramSize          Requested framebuffer size in bytes.
  @param[out] VramBaseAddress   Pointer to memory allocated for framebuffer.

  @retval EFI_SUCCESS           Framebuffer memory allocated successfully.
  @retval EFI_UNSUPPORTED       Allocated address wider than 40 bits.
  @retval !EFI_SUCCESS          Other errors.
**/
STATIC
EFI_STATUS
GetVram (
  IN  UINTN                   VramSize,
  OUT EFI_PHYSICAL_ADDRESS *  VramBaseAddress
  )
{
  EFI_STATUS             Status;
  EFI_CPU_ARCH_PROTOCOL  *Cpu;

  *VramBaseAddress = FixedPcdGet64 (PcdArmLcdDdrFrameBufferBase);

  // Check if memory is already reserved for the framebuffer.
  if (*VramBaseAddress != 0) {
    // ARM Mali DP framebuffer base address can not be wider than 40 bits.
    if (!DP_VALID_BASE_ADDR (*VramBaseAddress)) {
      ASSERT (DP_VALID_BASE_ADDR (*VramBaseAddress));
      Status = EFI_UNSUPPORTED;
    } else {
      Status = EFI_SUCCESS;
    }
  } else {
    // If not already reserved, attempt to allocate the VRAM from the DRAM.
    Status = gBS->AllocatePages (
                    AllocateAnyPages,
                    EfiReservedMemoryType,
                    EFI_SIZE_TO_PAGES (VramSize),
                    VramBaseAddress
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "ArmMaliDpLib: Failed to allocate framebuffer.\n"));
      ASSERT_EFI_ERROR (Status);
      return Status;
    }

    // ARM Mali DP framebuffer base address can not be wider than 40 bits.
    if (!DP_VALID_BASE_ADDR (*VramBaseAddress)) {
      gBS->FreePages (*VramBaseAddress, EFI_SIZE_TO_PAGES (VramSize));
      ASSERT (DP_VALID_BASE_ADDR (*VramBaseAddress));
      return EFI_UNSUPPORTED;
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
                      VramSize,
                      EFI_MEMORY_WC | EFI_MEMORY_XP
                      );
    }
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      gBS->FreePages (*VramBaseAddress, EFI_SIZE_TO_PAGES (VramSize));
    }
  }

  return Status;
}

/** Allocate VRAM memory in DRAM for the framebuffer
  (unless it is reserved already).

  The allocated address can be used to set the framebuffer as a base buffer
  address for any layer of the ARM Mali DP.

  @param[out] VramBaseAddress     A pointer to the framebuffer address.
  @param[out] VramSize            A pointer to the size of the frame
                                  buffer in bytes

  @retval EFI_SUCCESS             Framebuffer memory allocation success.
  @retval EFI_UNSUPPORTED         Allocated address wider than 40 bits
  @retval !EFI_SUCCESS            Other errors.
**/
EFI_STATUS
LcdPlatformGetVram (
  OUT EFI_PHYSICAL_ADDRESS * VramBaseAddress,
  OUT UINTN                * VramSize
  )
{
  ASSERT (VramBaseAddress != NULL);
  ASSERT (VramSize != NULL);

  // Set the VRAM size.
  *VramSize = (UINTN)FixedPcdGet32 (PcdArmLcdDdrFrameBufferSize);

  return GetVram (*VramSize, VramBaseAddress);
}

/** Return total number of modes supported.

  Note: Valid mode numbers are 0 to MaxMode - 1
  See Section 12.9 of the UEFI Specification 2.7

  @retval UINT32             Mode Number.
**/
UINT32
LcdPlatformGetMaxMode (VOID)
{
  return  mMaxMode;
}

/** Set the requested display mode.

  @param[in] ModeNumber            Mode Number.

  @retval EFI_SUCCESS              Mode set successful.
  @retval EFI_INVALID_PARAMETER    Requested mode not found.
**/
EFI_STATUS
LcdPlatformSetMode (
  IN UINT32 ModeNumber
  )
{
  if (ModeNumber >= mMaxMode) {
    ASSERT (ModeNumber < mMaxMode);
    return EFI_INVALID_PARAMETER;
  }

  // On models, platform specific clock/mux settings are not required.
  // Display controller specific settings for Mali DP are done in LcdSetMode.
  return EFI_SUCCESS;
}

/** Return information for the requested mode number.

  @param[in]  ModeNumber          Mode Number.
  @param[out] Info                Pointer for returned mode information
                                  (on success).

  @retval EFI_SUCCESS             Display mode information of the requested
                                  mode returned successfully.
  @retval EFI_INVALID_PARAMETER   Requested mode not found.
**/
EFI_STATUS
LcdPlatformQueryMode (
  IN  UINT32                                 ModeNumber,
  OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION * Info
  )
{
  if (ModeNumber >= mMaxMode) {
    ASSERT (ModeNumber < mMaxMode);
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

/** Return the display timing information for the requested mode number.

  @param[in]  ModeNumber         Mode Number.

  @param[out] Horizontal         Pointer to horizontal timing parameters.
                                 (Resolution, Sync, Back porch, Front porch)
  @param[out] Vertical           Pointer to vertical timing parameters.
                                 (Resolution, Sync, Back porch, Front porch)

  @retval EFI_SUCCESS             Display timing information of the requested
                                  mode returned successfully.
  @retval EFI_INVALID_PARAMETER   Requested mode not found.
**/
EFI_STATUS
LcdPlatformGetTimings (
  IN  UINT32                ModeNumber,
  OUT SCAN_TIMINGS          ** Horizontal,
  OUT SCAN_TIMINGS          ** Vertical
  )
{
  ASSERT (Horizontal != NULL);
  ASSERT (Vertical != NULL);

  if (ModeNumber >= mMaxMode) {
    ASSERT (ModeNumber < mMaxMode);
    return EFI_INVALID_PARAMETER;
  }

  *Horizontal = &mDisplayModes[ModeNumber].Horizontal;
  *Vertical = &mDisplayModes[ModeNumber].Vertical;

  return EFI_SUCCESS;
}

/** Return bits per pixel information for a mode number.

  @param[in]  ModeNumber          Mode Number.
  @param[out] Bpp                 Pointer to bits per pixel information.

  @retval EFI_SUCCESS             Bits per pixel information of the display
                                  mode returned successfully.
  @retval EFI_INVALID_PARAMETER   Requested mode not found.
**/
EFI_STATUS
LcdPlatformGetBpp (
  IN  UINT32    ModeNumber,
  OUT LCD_BPP * Bpp
  )
{
  ASSERT (Bpp != NULL);
  // Check valid ModeNumber.
  if (ModeNumber >= mMaxMode) {
    ASSERT (ModeNumber < mMaxMode);
    return EFI_INVALID_PARAMETER;
  }

  *Bpp = LcdBitsPerPixel_24;

  return EFI_SUCCESS;
}
