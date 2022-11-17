/** @file
 *
 *  Copyright (c) 2020, ARM Limited. All rights reserved.
 *  Copyright (c) 2017-2018, Andrei Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Base.h>
#include "DisplayDxe.h"

#define MODE_800_ENABLED      BIT0
#define MODE_640_ENABLED      BIT1
#define MODE_1024_ENABLED     BIT2
#define MODE_720P_ENABLED     BIT3
#define MODE_1080P_ENABLED    BIT4
#define MODE_NATIVE_ENABLED   BIT5
#define JUST_NATIVE_ENABLED   MODE_NATIVE_ENABLED
#define ALL_MODES             (BIT6 - 1)
#define POS_TO_FB(posX, posY) ((UINT8*)                                 \
                               ((UINTN)This->Mode->FrameBufferBase +    \
                                (posY) * This->Mode->Info->PixelsPerScanLine * \
                                PI3_BYTES_PER_PIXEL +                   \
                                (posX) * PI3_BYTES_PER_PIXEL))

STATIC
EFI_STATUS
EFIAPI
DriverSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN EFI_HANDLE                  Controller,
  IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
  );

STATIC
EFI_STATUS
EFIAPI
DriverStart (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN EFI_HANDLE                  Controller,
  IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
  );

STATIC
EFI_STATUS
EFIAPI
DriverStop (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN EFI_HANDLE                  Controller,
  IN UINTN                       NumberOfChildren,
  IN EFI_HANDLE                  *ChildHandleBuffer
  );

STATIC
EFI_STATUS
EFIAPI
DisplayQueryMode (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL          *This,
  IN  UINT32                                ModeNumber,
  OUT UINTN                                 *SizeOfInfo,
  OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  **Info
  );

STATIC
EFI_STATUS
EFIAPI
DisplaySetMode (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
  IN  UINT32                       ModeNumber
  );

STATIC
EFI_STATUS
EFIAPI
DisplayBlt (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL            *This,
  IN  EFI_GRAPHICS_OUTPUT_BLT_PIXEL           *BltBuffer, OPTIONAL
  IN  EFI_GRAPHICS_OUTPUT_BLT_OPERATION       BltOperation,
  IN  UINTN                                   SourceX,
  IN  UINTN                                   SourceY,
  IN  UINTN                                   DestinationX,
  IN  UINTN                                   DestinationY,
  IN  UINTN                                   Width,
  IN  UINTN                                   Height,
  IN  UINTN                                   Delta         OPTIONAL
  );

EFI_DRIVER_BINDING_PROTOCOL mDriverBinding = {
  DriverSupported,
  DriverStart,
  DriverStop,
  0xa,
  NULL,
  NULL
};

typedef struct {
  VENDOR_DEVICE_PATH DisplayDevicePath;
  EFI_DEVICE_PATH EndDevicePath;
} DISPLAY_DEVICE_PATH;

typedef struct {
  UINT32 Width;
  UINT32 Height;
} GOP_MODE_DATA;

STATIC UINT32 mBootWidth;
STATIC UINT32 mBootHeight;
STATIC EFI_HANDLE mDevice;
STATIC RASPBERRY_PI_FIRMWARE_PROTOCOL *mFwProtocol;
STATIC EFI_CPU_ARCH_PROTOCOL *mCpu;

STATIC UINTN mLastMode;
STATIC GOP_MODE_DATA mGopModeTemplate[] = {
  { 800,  600  }, /* Legacy */
  { 640,  480  }, /* Legacy */
  { 1024, 768  }, /* Legacy */
  { 1280, 720  }, /* 720p */
  { 1920, 1080 }, /* 1080p */
  { 0,    0    }, /* Physical */
};

STATIC UINTN mLastMode;
STATIC GOP_MODE_DATA mGopModeData[ARRAY_SIZE (mGopModeTemplate)];

STATIC DISPLAY_DEVICE_PATH mDisplayProtoDevicePath =
{
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8),
      }
    },
    EFI_CALLER_ID_GUID,
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      sizeof (EFI_DEVICE_PATH_PROTOCOL),
      0
    }
  }
};

#define PI3_BITS_PER_PIXEL              (32)
#define PI3_BYTES_PER_PIXEL             (PI3_BITS_PER_PIXEL / 8)

EFI_GRAPHICS_OUTPUT_PROTOCOL gDisplayProto = {
  DisplayQueryMode,
  DisplaySetMode,
  DisplayBlt,
  NULL
};

STATIC
EFI_STATUS
EFIAPI
DisplayQueryMode (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL          *This,
  IN  UINT32                                ModeNumber,
  OUT UINTN                                 *SizeOfInfo,
  OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  **Info
  )
{
  EFI_STATUS Status;
  GOP_MODE_DATA *Mode;

  if (Info == NULL || SizeOfInfo == NULL || ModeNumber >= This->Mode->MaxMode) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION),
                  (VOID**)Info
                );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Mode = &mGopModeData[ModeNumber];

  *SizeOfInfo = sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
  (*Info)->Version = This->Mode->Info->Version;
  (*Info)->HorizontalResolution = Mode->Width;
  (*Info)->VerticalResolution = Mode->Height;
  (*Info)->PixelFormat = This->Mode->Info->PixelFormat;
  (*Info)->PixelInformation.RedMask = This->Mode->Info->PixelInformation.RedMask;
  (*Info)->PixelInformation.GreenMask = This->Mode->Info->PixelInformation.GreenMask;
  (*Info)->PixelInformation.BlueMask = This->Mode->Info->PixelInformation.BlueMask;
  (*Info)->PixelInformation.ReservedMask = This->Mode->Info->PixelInformation.ReservedMask;
  (*Info)->PixelsPerScanLine = Mode->Width;

  return EFI_SUCCESS;
}

STATIC
VOID
ClearScreen (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL *This
  )
{
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL Fill;

  Fill.Red = 0x00;
  Fill.Green = 0x00;
  Fill.Blue = 0x00;
  This->Blt (This, &Fill, EfiBltVideoFill,
          0, 0, 0, 0, This->Mode->Info->HorizontalResolution,
          This->Mode->Info->VerticalResolution,
          This->Mode->Info->HorizontalResolution *
          sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
}

STATIC
EFI_STATUS
EFIAPI
DisplaySetMode (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
  IN  UINT32                       ModeNumber
  )
{
  UINTN FbSize;
  UINTN FbPitch;
  EFI_STATUS Status;
  EFI_PHYSICAL_ADDRESS FbBase;
  GOP_MODE_DATA *Mode = &mGopModeData[ModeNumber];

 if (ModeNumber >= This->Mode->MaxMode) {
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "Setting mode %u from %u: %u x %u\n",
    ModeNumber, This->Mode->Mode, Mode->Width, Mode->Height));
  Status = mFwProtocol->GetFB (Mode->Width, Mode->Height,
                          PI3_BITS_PER_PIXEL, &FbBase,
                          &FbSize, &FbPitch);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Could not set mode %u\n", ModeNumber));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "Mode %u: %u x %u framebuffer is %u bytes at %p\n",
    ModeNumber, Mode->Width, Mode->Height, FbSize, FbBase));

  if (FbPitch / PI3_BYTES_PER_PIXEL != Mode->Width) {
    DEBUG ((DEBUG_ERROR, "Error: Expected width %u, got width %u\n",
      Mode->Width, FbPitch / PI3_BYTES_PER_PIXEL));
    return EFI_DEVICE_ERROR;
  }

  /*
   * WT, because certain OS loaders access the frame buffer directly
   * and we don't want to see corruption due to missing WB cache
   * maintenance. Performance with WT is good.
   */
  Status = mCpu->SetMemoryAttributes (mCpu, FbBase,
                   ALIGN_VALUE (FbSize, EFI_PAGE_SIZE),
                   EFI_MEMORY_WT);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Couldn't set framebuffer attributes: %r\n", Status));
    return Status;
  }

  This->Mode->Mode = ModeNumber;
  This->Mode->Info->Version = 0;
  This->Mode->Info->HorizontalResolution = Mode->Width;
  This->Mode->Info->VerticalResolution = Mode->Height;
  /*
   * NOTE: Windows REQUIRES BGR in 32 or 24 bit format.
   */
  This->Mode->Info->PixelFormat = PixelBlueGreenRedReserved8BitPerColor;
  This->Mode->Info->PixelsPerScanLine = Mode->Width;
  This->Mode->SizeOfInfo = sizeof (*This->Mode->Info);
  This->Mode->FrameBufferBase = FbBase;
  This->Mode->FrameBufferSize = Mode->Width * Mode->Height * PI3_BYTES_PER_PIXEL;
  DEBUG((DEBUG_INFO, "Reported Mode->FrameBufferSize is %u\n", This->Mode->FrameBufferSize));

  ClearScreen (This);
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
DisplayBlt (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL      *This,
  IN  EFI_GRAPHICS_OUTPUT_BLT_PIXEL     *BltBuffer, OPTIONAL
  IN  EFI_GRAPHICS_OUTPUT_BLT_OPERATION BltOperation,
  IN  UINTN                             SourceX,
  IN  UINTN                             SourceY,
  IN  UINTN                             DestinationX,
  IN  UINTN                             DestinationY,
  IN  UINTN                             Width,
  IN  UINTN                             Height,
  IN  UINTN                             Delta         OPTIONAL
  )
{
  UINT8 *VidBuf, *BltBuf, *VidBuf1;
  UINTN i;

  if ((UINTN)BltOperation >= EfiGraphicsOutputBltOperationMax) {
    return EFI_INVALID_PARAMETER;
  }

  if (Width == 0 || Height == 0) {
    return EFI_INVALID_PARAMETER;
  }

  switch (BltOperation) {
  case EfiBltVideoFill:
    BltBuf = (UINT8*)BltBuffer;

    for (i = 0; i < Height; i++) {
      VidBuf = POS_TO_FB (DestinationX, DestinationY + i);

      SetMem32 (VidBuf, Width * PI3_BYTES_PER_PIXEL, *(UINT32*)BltBuf);
    }
    break;

  case EfiBltVideoToBltBuffer:
    if (Delta == 0) {
      Delta = Width * PI3_BYTES_PER_PIXEL;
    }

    for (i = 0; i < Height; i++) {
      VidBuf = POS_TO_FB (SourceX, SourceY + i);

      BltBuf = (UINT8*)((UINTN)BltBuffer + (DestinationY + i) * Delta +
        DestinationX * PI3_BYTES_PER_PIXEL);

      gBS->CopyMem ((VOID*)BltBuf, (VOID*)VidBuf, PI3_BYTES_PER_PIXEL * Width);
    }
    break;

  case EfiBltBufferToVideo:
    if (Delta == 0) {
      Delta = Width * PI3_BYTES_PER_PIXEL;
    }

    for (i = 0; i < Height; i++) {
      VidBuf = POS_TO_FB (DestinationX, DestinationY + i);
      BltBuf = (UINT8*)((UINTN)BltBuffer + (SourceY + i) * Delta +
        SourceX * PI3_BYTES_PER_PIXEL);

      gBS->CopyMem ((VOID*)VidBuf, (VOID*)BltBuf, Width * PI3_BYTES_PER_PIXEL);
    }
    break;

  case EfiBltVideoToVideo:
    for (i = 0; i < Height; i++) {
      VidBuf = POS_TO_FB (SourceX, SourceY + i);
      VidBuf1 = POS_TO_FB (DestinationX, DestinationY + i);

      gBS->CopyMem ((VOID*)VidBuf1, (VOID*)VidBuf, Width * PI3_BYTES_PER_PIXEL);
    }
    break;

  default:
    return EFI_INVALID_PARAMETER;
    break;
  }

  return EFI_SUCCESS;
}

/**
   Initialize the state information for the Display Dxe

   @param  ImageHandle   of the loaded driver
   @param  SystemTable   Pointer to the System Table

   @retval EFI_SUCCESS           Protocol registered
   @retval EFI_OUT_OF_RESOURCES  Cannot allocate protocol data structure
   @retval EFI_DEVICE_ERROR      Hardware problems

**/
EFI_STATUS
EFIAPI
DisplayDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS Status;

  Status = gBS->LocateProtocol (&gRaspberryPiFirmwareProtocolGuid, NULL,
                  (VOID**)&mFwProtocol);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->LocateProtocol (&gEfiCpuArchProtocolGuid, NULL,
                  (VOID**)&mCpu);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Query the current display resolution from mailbox
  Status = mFwProtocol->GetFBSize (&mBootWidth, &mBootHeight);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "Display boot mode is %u x %u\n",
    mBootWidth, mBootHeight));

  Status = gBS->InstallMultipleProtocolInterfaces (
    &mDevice, &gEfiDevicePathProtocolGuid,
    &mDisplayProtoDevicePath, &gEfiCallerIdGuid,
    NULL, NULL);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &mDriverBinding,
             ImageHandle,
             &gComponentName,
             &gComponentName2
           );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return Status;
}

STATIC
EFI_STATUS
EFIAPI
DriverSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN EFI_HANDLE                  Controller,
  IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
  )
{
  VOID *Temp;

  if (Controller != mDevice) {
    return EFI_UNSUPPORTED;
  }

  if (gBS->HandleProtocol (Controller, &gEfiGraphicsOutputProtocolGuid,
             (VOID**)&Temp) == EFI_SUCCESS) {
    return EFI_ALREADY_STARTED;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
DriverStart (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN EFI_HANDLE                  Controller,
  IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
  )
{
  UINTN Index;
  UINTN TempIndex;
  EFI_STATUS Status;
  VOID *Dummy;

  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiCallerIdGuid,
                  (VOID**)&Dummy,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  gDisplayProto.Mode = AllocateZeroPool (sizeof (EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE));
  if (gDisplayProto.Mode == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }

  gDisplayProto.Mode->Info = AllocateZeroPool (sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));
  if (gDisplayProto.Mode->Info == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }

  Status = PcdSet8S (PcdDisplayEnableScaledVModes,
             PcdGet8 (PcdDisplayEnableScaledVModes) & ALL_MODES);
  ASSERT_EFI_ERROR (Status);

  if (PcdGet8 (PcdDisplayEnableScaledVModes) == 0) {
    Status = PcdSet8S (PcdDisplayEnableScaledVModes, JUST_NATIVE_ENABLED);
    ASSERT_EFI_ERROR (Status);
  }

  mLastMode = 0;
  for  (TempIndex = 0, Index = 0;
        TempIndex < ARRAY_SIZE (mGopModeTemplate); TempIndex++) {
    if ((PcdGet8 (PcdDisplayEnableScaledVModes) & (1 << TempIndex)) != 0) {
      DEBUG ((DEBUG_ERROR, "Mode %u: %u x %u present\n",
        TempIndex, mGopModeTemplate[TempIndex].Width,
        mGopModeTemplate[TempIndex].Height));

      CopyMem (&mGopModeData[Index], &mGopModeTemplate[TempIndex],
        sizeof (GOP_MODE_DATA));
      mLastMode = Index;
      Index++;
    }
  }

  if (PcdGet8 (PcdDisplayEnableScaledVModes) == JUST_NATIVE_ENABLED) {
    /*
     * mBootWidth x mBootHeight may not be sensible,
     * so clean it up, since we won't be adding
     * any other extra vmodes.
     */
    if (mBootWidth < 640 || mBootHeight < 480) {
      mBootWidth = 640;
      mBootHeight = 480;
    } else if (mBootWidth == 800 && mBootHeight == 480) {
      /* The Pi 7" screen is close to 800x600, just pretend it is. */
      mBootHeight = 600;
    }
  }

  if ((PcdGet8(PcdDisplayEnableScaledVModes) & MODE_NATIVE_ENABLED) != 0) {
     /*
      * Adjust actual native res only if native res is enabled
      * (so last mode is native res).
      */
     mGopModeData[mLastMode].Width = mBootWidth;
     mGopModeData[mLastMode].Height = mBootHeight;
  }

  for (Index = 0; Index <= mLastMode; Index++) {
    UINTN FbSize;
    UINTN FbPitch;
    EFI_PHYSICAL_ADDRESS FbBase;

    GOP_MODE_DATA *Mode = &mGopModeData[Index];

    Status = mFwProtocol->GetFB (Mode->Width, Mode->Height,
                            PI3_BITS_PER_PIXEL, &FbBase,
                            &FbSize, &FbPitch);
    if (EFI_ERROR (Status)) {
      goto Done;
    }

    //
    // There is no way to communicate pitch back to OS. OS and even UEFI
    // expect a fully linear frame buffer. So the width should
    // be based on the frame buffer's pitch value. In some cases VC
    // firmware would allocate ao frame buffer with some padding
    // presumably to be 8 byte align.
    //
    Mode->Width = FbPitch / PI3_BYTES_PER_PIXEL;

    DEBUG ((DEBUG_INFO, "Mode %u: %u x %u framebuffer is %u bytes at %p\n",
      Index, Mode->Width, Mode->Height, FbSize, FbBase));

    ASSERT (FbPitch != 0);
    ASSERT (FbBase != 0);
    ASSERT (FbSize != 0);
  }

  // Both set the mode and initialize current mode information.
  gDisplayProto.Mode->MaxMode = mLastMode + 1;
  DisplaySetMode (&gDisplayProto, 0);

  Status = gBS->InstallMultipleProtocolInterfaces (
    &Controller, &gEfiGraphicsOutputProtocolGuid,
    &gDisplayProto, NULL);
  if (EFI_ERROR (Status)) {
    goto Done;
  }

  if (PcdGet32 (PcdDisplayEnableSShot)) {
    RegisterScreenshotHandlers ();
  } else {
    DEBUG ((DEBUG_INFO, "Screenshot capture disabled\n"));
  }

Done:
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Could not start DisplayDxe: %r\n", Status));
    if (gDisplayProto.Mode->Info != NULL) {
      FreePool (gDisplayProto.Mode->Info);
      gDisplayProto.Mode->Info = NULL;
    }

    if (gDisplayProto.Mode != NULL) {
      FreePool (gDisplayProto.Mode);
      gDisplayProto.Mode = NULL;
    }

    gBS->CloseProtocol (
           Controller,
           &gEfiCallerIdGuid,
           This->DriverBindingHandle,
           Controller
         );
  }
  return Status;
}

STATIC
EFI_STATUS
EFIAPI
DriverStop (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN EFI_HANDLE                  Controller,
  IN UINTN                       NumberOfChildren,
  IN EFI_HANDLE                  *ChildHandleBuffer
  )
{
  EFI_STATUS Status;

  ClearScreen (&gDisplayProto);

  Status = gBS->UninstallMultipleProtocolInterfaces (
    Controller, &gEfiGraphicsOutputProtocolGuid,
    &gDisplayProto, NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  FreePool (gDisplayProto.Mode->Info);
  gDisplayProto.Mode->Info = NULL;
  FreePool (gDisplayProto.Mode);
  gDisplayProto.Mode = NULL;

  gBS->CloseProtocol (
         Controller,
         &gEfiCallerIdGuid,
         This->DriverBindingHandle,
         Controller
       );

  return Status;
}
