/** @file

  Copyright (c) 2013 - 2021, Arm Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/LcdPlatformLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/ArmScmi.h>
#include <Protocol/ArmScmiClockProtocol.h>
#include <Protocol/Cpu.h>

/* Display timings on Juno for 1920x1080.
  On Juno due to instability of the PLLs, we set OSC
  frequency to 138.5MHz which is stable for most monitors.
  Frequency 148.5MHz does not work with some monitors.
  148.5MHz is set by SCP firmware by default.

#define JUNO_HD_OSC_FREQUENCY               148500000
*/
#define JUNO_HD_OSC_FREQUENCY               138500000
#define JUNO_HD_H_SYNC                      ( 32 - 1)
#define JUNO_HD_H_FRONT_PORCH               ( 48 - 1)
#define JUNO_HD_H_BACK_PORCH                ( 80 - 1)
#define JUNO_HD_V_SYNC                      (  5 - 1)
#define JUNO_HD_V_FRONT_PORCH               (  3 - 1)
#define JUNO_HD_V_BACK_PORCH                ( 23 - 1)

/* SCMI defined clock device name and ID. This is not documented but
   obtained using clock management protocol's CLOCK_ATTRIBUTES command.

   Generally we must discover clock device ID using clock name and then
   set/get rate using CLOCK_RATE_SET/CLOCK_RATE_GET commands. However
   because LcdGraphicsOutputDxe is a DXE driver, which gets initialized
   at boot time, for faster boot, in release build we will directly use
   this already known value as an argument to rate get/set functions.

   We expect these values not to change in future SCP firmware releases.

   DEBUG build however will probe SCP firmware and discover clock device
   ID for HDLCD.
*/
#define ARM_JUNO_CSS_CLK_NAME_HDLCD_0       "HDLCD_0"
#define ARM_JUNO_CSS_CLKID_HDLCD_0           3

typedef struct {
  UINT32                      OscFreq;
  SCAN_TIMINGS                Horizontal;
  SCAN_TIMINGS                Vertical;
} DISPLAY_MODE;

STATIC DISPLAY_MODE mDisplayModes[] = {
  {
    // VGA : 640 x 480 x 24 bpp.
    VGA_OSC_FREQUENCY,
    {VGA_H_RES_PIXELS, VGA_H_SYNC, VGA_H_BACK_PORCH, VGA_H_FRONT_PORCH},
    {VGA_V_RES_PIXELS, VGA_V_SYNC, VGA_V_BACK_PORCH, VGA_V_FRONT_PORCH}
  },
  {
    // WVGA : 800 x 480 x 24 bpp.
    WVGA_OSC_FREQUENCY,
    {WVGA_H_RES_PIXELS, WVGA_H_SYNC, WVGA_H_BACK_PORCH, WVGA_H_FRONT_PORCH},
    {WVGA_V_RES_PIXELS, WVGA_V_SYNC, WVGA_V_BACK_PORCH, WVGA_V_FRONT_PORCH}
  },
  {
    // SVGA : 800 x 600 x 24 bpp.
    SVGA_OSC_FREQUENCY,
    {SVGA_H_RES_PIXELS, SVGA_H_SYNC, SVGA_H_BACK_PORCH, SVGA_H_FRONT_PORCH},
    {SVGA_V_RES_PIXELS, SVGA_V_SYNC, SVGA_V_BACK_PORCH, SVGA_V_FRONT_PORCH}
  },
  {
    // QHD : 960 x 540 x 24 bpp.
    QHD_OSC_FREQUENCY,
    {QHD_H_RES_PIXELS, QHD_H_SYNC, QHD_H_BACK_PORCH, QHD_H_FRONT_PORCH},
    {QHD_V_RES_PIXELS, QHD_V_SYNC, QHD_V_BACK_PORCH, QHD_V_FRONT_PORCH}
  },
  {
    // WSVGA : 1024 x 600 x 24 bpp.
    WSVGA_OSC_FREQUENCY,
    {WSVGA_H_RES_PIXELS, WSVGA_H_SYNC, WSVGA_H_BACK_PORCH, WSVGA_H_FRONT_PORCH},
    {WSVGA_V_RES_PIXELS, WSVGA_V_SYNC, WSVGA_V_BACK_PORCH, WSVGA_V_FRONT_PORCH}
  },
  {
    // XGA : 1024 x 768 x 24 bpp.
    XGA_OSC_FREQUENCY,
    {XGA_H_RES_PIXELS, XGA_H_SYNC, XGA_H_BACK_PORCH, XGA_H_FRONT_PORCH},
    {XGA_V_RES_PIXELS, XGA_V_SYNC, XGA_V_BACK_PORCH, XGA_V_FRONT_PORCH}
  },
  {
    // HD : 1280 x 720 x 24 bpp.
    HD720_OSC_FREQUENCY,
    {HD720_H_RES_PIXELS, HD720_H_SYNC, HD720_H_BACK_PORCH, HD720_H_FRONT_PORCH},
    {HD720_V_RES_PIXELS, HD720_V_SYNC, HD720_V_BACK_PORCH, HD720_V_FRONT_PORCH}
  },
  {
    // WXGA : 1280 x 800 x 24 bpp.
    WXGA_OSC_FREQUENCY,
    {WXGA_H_RES_PIXELS, WXGA_H_SYNC, WXGA_H_BACK_PORCH, WXGA_H_FRONT_PORCH},
    {WXGA_V_RES_PIXELS, WXGA_V_SYNC, WXGA_V_BACK_PORCH, WXGA_V_FRONT_PORCH}
  },
  {
    // SXGA : 1280 x 1024 x 24 bpp.
    SXGA_OSC_FREQUENCY,
    {SXGA_H_RES_PIXELS, SXGA_H_SYNC, SXGA_H_BACK_PORCH, SXGA_H_FRONT_PORCH},
    {SXGA_V_RES_PIXELS, SXGA_V_SYNC, SXGA_V_BACK_PORCH, SXGA_V_FRONT_PORCH}
  },
  {
    // WSXGA+ : 1680 x 1050 x 24 bpp.
    WSXGA_OSC_FREQUENCY,
    {WSXGA_H_RES_PIXELS, WSXGA_H_SYNC, WSXGA_H_BACK_PORCH, WSXGA_H_FRONT_PORCH},
    {WSXGA_V_RES_PIXELS, WSXGA_V_SYNC, WSXGA_V_BACK_PORCH, WSXGA_V_FRONT_PORCH}
  },
  {
    // HD : 1920 x 1080 x 24 bpp.
    JUNO_HD_OSC_FREQUENCY,
    {HD_H_RES_PIXELS, JUNO_HD_H_SYNC, JUNO_HD_H_BACK_PORCH, JUNO_HD_H_FRONT_PORCH},
    {HD_V_RES_PIXELS, JUNO_HD_V_SYNC, JUNO_HD_V_BACK_PORCH, JUNO_HD_V_FRONT_PORCH}
  }
};

/* If PcdArmMaliDpMaxMode is 0, platform supports full range of modes
   else platform supports modes from 0 to PcdArmHdLcdMaxMode - 1
*/
STATIC CONST UINT32 mMaxMode = ((FixedPcdGet32 (PcdArmHdLcdMaxMode) != 0)
                                   ? FixedPcdGet32 (PcdArmHdLcdMaxMode)
                                   : sizeof (mDisplayModes) / sizeof (DISPLAY_MODE));

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
  IN  EFI_HANDLE  Handle
  )
{
  (VOID)Handle;
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
  OUT EFI_PHYSICAL_ADDRESS  * VramBaseAddress,
  OUT UINTN                 * VramSize
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;

  EFI_CPU_ARCH_PROTOCOL  *Cpu;

  ASSERT (VramBaseAddress != NULL);
  ASSERT (VramSize != NULL);

  // Set the VRAM size.
  *VramSize = (UINTN)FixedPcdGet32 (PcdArmLcdDdrFrameBufferSize);

  // Check if memory is already reserved for the framebuffer.
  *VramBaseAddress =
     (EFI_PHYSICAL_ADDRESS)FixedPcdGet64 (PcdArmLcdDdrFrameBufferBase);

  if (*VramBaseAddress == 0) {
    // If not already reserved, attempt to allocate the VRAM from the DRAM.
    Status = gBS->AllocatePages (
                    AllocateAnyPages,
                    EfiReservedMemoryType,
                    EFI_SIZE_TO_PAGES (*VramSize),
                    VramBaseAddress
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "HdLcdArmJuno: Failed to allocate frame buffer.\n"));
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
  return  mMaxMode;
}

#if !defined(MDEPKG_NDEBUG)
/** Probe Clock device ID of the HDLCD clock and current pixel clock frequency.
  NOTE: We will probe information only in DEBUG build.

  @param[in]  ClockProtocol   A pointer to SCMI clock protocol
                              interface instance.
  @param[out] ClockId         ID of the clock device

  @retval EFI_SUCCESS         Clock ID of the HDLCD device returned
                              successfully.
  @retval EFI_UNSUPPORTED     SCMI clock management protocol unsupported.
  @retval EFI_DEVICE_ERROR    SCMI error.
  @retval EFI_NOT_FOUND       Not found valid clock device ID of the HDLCD.
**/
STATIC
EFI_STATUS
ProbeHdLcdClock (
  IN  SCMI_CLOCK_PROTOCOL  * ClockProtocol,
  OUT UINT32               * ClockId
  )
{
  EFI_STATUS  Status;
  UINT64      CurrentHdLcdFreq;

  UINT32      TotalClocks;
  UINT32      ClockProtocolVersion;
  BOOLEAN     Enabled;
  CHAR8       ClockName[SCMI_MAX_STR_LEN];
  BOOLEAN     ClockFound = FALSE;

  UINT32                 TotalRates = 0;
  UINT32                 ClockRateSize;
  SCMI_CLOCK_RATE        ClockRate;
  SCMI_CLOCK_RATE_FORMAT ClockRateFormat;

  Status = ClockProtocol->GetVersion (ClockProtocol, &ClockProtocolVersion);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  DEBUG ((DEBUG_ERROR, "SCMI clock management protocol version = %x\n",
    ClockProtocolVersion));

  if (ClockProtocolVersion != SCMI_CLOCK_PROTOCOL_VERSION) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  Status = ClockProtocol->GetTotalClocks (ClockProtocol, &TotalClocks);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_ERROR, "Total number of clocks supported by SCMI clock management protocol = %d\n",
    TotalClocks));

  for (*ClockId = 0; *ClockId < TotalClocks; (*ClockId)++) {
    Status = ClockProtocol->GetClockAttributes (
                              ClockProtocol,
                              *ClockId,
                              &Enabled,
                              ClockName
                              );
    if (EFI_ERROR (Status)) {
      // In current implementation of SCMI, some clocks are not accessible to
      // calling agents (in our case UEFI is an agent) which results in an
      // EFI_DEVICE_ERROR error. A bug fix for this is in discussions and will
      // be fixed in future versions of the SCP firmware. Irrespective of a fix
      // we must iterate over each clock to see if it matches with HDLCD.
      continue;
    }

    if (AsciiStrnCmp ((CONST CHAR8*)ClockName,
          (CONST CHAR8*)ARM_JUNO_CSS_CLK_NAME_HDLCD_0,
          sizeof (ARM_JUNO_CSS_CLK_NAME_HDLCD_0)) == 0) {
      ClockFound = TRUE;
      break;
    }
  }

  if (!ClockFound) {
    return EFI_NOT_FOUND;
  }

  ClockRateSize = sizeof (ClockRate);
  Status = ClockProtocol->DescribeRates (
                            ClockProtocol,
                            *ClockId,
                            &ClockRateFormat,
                            &TotalRates,
                            &ClockRateSize,
                            &ClockRate
                            );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = ClockProtocol->RateGet (ClockProtocol, *ClockId, &CurrentHdLcdFreq);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  DEBUG ((
    DEBUG_ERROR,
    "Clock ID = %d Clock name = %a\n",
    *ClockId, ClockName
    ));
  DEBUG ((
    DEBUG_ERROR,
    "Minimum frequency = %uHz\n",
    ClockRate.ContinuousRate.Min
    ));
  DEBUG ((
    DEBUG_ERROR,
    "Maximum frequency = %uHz\n",
    ClockRate.ContinuousRate.Max
    ));
  DEBUG ((
    DEBUG_ERROR,
    "Clock rate step = %uHz\n",
    ClockRate.ContinuousRate.Step
    ));
  DEBUG ((
    DEBUG_ERROR,
    "HDLCD Current frequency = %uHz\n",
    CurrentHdLcdFreq
    ));

  return EFI_SUCCESS;
}
#endif

/** Set the requested display mode.

  @param[in] ModeNumber             Mode Number.

  @retval EFI_SUCCESS              Mode set successfully.
  @retval EFI_NOT_FOUND            Clock protocol instance not found.
  @retval EFI_DEVICE_ERROR         SCMI error.
  @retval EFI_INVALID_PARAMETER    Requested mode not found.
  @retval !(EFI_SUCCESS)           Other errors.
*/
EFI_STATUS
LcdPlatformSetMode (
  IN  UINT32  ModeNumber
  )
{
  EFI_STATUS          Status;
  SCMI_CLOCK_PROTOCOL *ClockProtocol;
  UINT32              ClockId;

  EFI_GUID ClockProtocolGuid = ARM_SCMI_CLOCK_PROTOCOL_GUID;

  if (ModeNumber >= mMaxMode) {
    ASSERT (ModeNumber < mMaxMode);
    return EFI_INVALID_PARAMETER;
  }

  // Display debug information in boot log.
  DEBUG ((DEBUG_ERROR, "HDLCD Display controller:\n"));

  DEBUG ((DEBUG_ERROR, "Required frequency for resolution %dx%d = %uHz\n",
            mDisplayModes[ModeNumber].Horizontal.Resolution,
            mDisplayModes[ModeNumber].Vertical.Resolution,
            mDisplayModes[ModeNumber].OscFreq));

  Status = gBS->LocateProtocol (
                  &ClockProtocolGuid,
                  NULL,
                  (VOID**)&ClockProtocol
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

#if !defined(MDEPKG_NDEBUG)
  /* Avoid probing clock device ID in RELEASE build */
  Status = ProbeHdLcdClock (ClockProtocol, &ClockId);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ASSERT (ClockId == ARM_JUNO_CSS_CLKID_HDLCD_0);
#else
  ClockId = ARM_JUNO_CSS_CLKID_HDLCD_0;
#endif

  // Set HDLCD clock required for the requested mode
  Status = ClockProtocol->RateSet (
                            ClockProtocol,
                            ClockId,
                            mDisplayModes[ModeNumber].OscFreq
                            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SCMI error: %r\n", Status));
    return Status;
  }

#if !defined(MDEPKG_NDEBUG)
  UINT64  CurrentHdLcdFreq;
  // Actual value set can differ from requested frequency so verify.
  Status = ClockProtocol->RateGet (
                            ClockProtocol,
                            ARM_JUNO_CSS_CLKID_HDLCD_0,
                            &CurrentHdLcdFreq
                            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SCMI Error: %r\n", Status));
  } else {
    DEBUG ((DEBUG_ERROR, "Requested frequency change = %uHz, Actual changed frequency = %uHz\n",
      mDisplayModes[ModeNumber].OscFreq,
      CurrentHdLcdFreq
      ));
  }
#endif

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
  IN  UINT32                                  ModeNumber,
  OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  * Info
  )
{
  if (ModeNumber >= mMaxMode ){
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
  IN  UINT32           ModeNumber,
  OUT SCAN_TIMINGS  ** Horizontal,
  OUT SCAN_TIMINGS  ** Vertical
  )
{
  if (ModeNumber >= mMaxMode ){
    ASSERT (ModeNumber < mMaxMode);
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (Horizontal != NULL);
  ASSERT (Vertical != NULL);

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
  IN  UINT32     ModeNumber,
  OUT LCD_BPP  * Bpp
  )
{
  if (ModeNumber >= mMaxMode) {
    // Check valid ModeNumber and Bpp.
    ASSERT (ModeNumber < mMaxMode);
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (Bpp != NULL);

  *Bpp = LcdBitsPerPixel_24;

  return EFI_SUCCESS;
}
