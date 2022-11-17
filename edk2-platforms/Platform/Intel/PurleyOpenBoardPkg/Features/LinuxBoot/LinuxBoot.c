/** @file

Copyright (c) 2021, American Megatrends International LLC. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/


#include <PiDxe.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Guid/DxeServices.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/LoadedImage.h>
#include <Guid/MemoryTypeInformation.h>
#include <Pi/PiDxeCis.h>
#include <Pi/PiHob.h>
#include <Library/PcdLib.h>
#include <Library/DxeServicesLib.h>
#include "LinuxBoot.h"

//16b60e5d-f1c5-42f0-9b34-08C81C430473
#define LINUX_BOOT_INITRD_GUID \
  { \
    0x16b60e5d, 0xf1c5, 0x42f0, {0x9b, 0x34, 0x08, 0xc8, 0x1c, 0x43, 0x04, 0x73} \
  }

#define LINUX_BOOT_KERNEL_GUID \
  { \
    0x81339b04, 0xfa8c, 0x4be0, {0x9c, 0xa7, 0x91, 0x6f, 0xc5, 0x31, 0x9e, 0xb5} \
  }


EFI_STATUS
EFIAPI
LoadLinuxCheckKernelSetup (
  IN VOID        *KernelSetup,
  IN UINTN       KernelSetupSize
  );

VOID*
EFIAPI
LoadLinuxAllocateKernelSetupPages (
  IN UINTN                  Pages
  );

EFI_STATUS
EFIAPI
LoadLinuxInitializeKernelSetup (
  IN VOID        *KernelSetup
  );

VOID*
EFIAPI
LoadLinuxAllocateKernelPages (
  IN VOID                   *KernelSetup,
  IN UINTN                  Pages
  );

EFI_STATUS
EFIAPI
LoadLinuxSetCommandLine (
  IN OUT VOID    *KernelSetup,
  IN CHAR8       *CommandLine
  );

EFI_STATUS
EFIAPI
LoadLinux (
  IN VOID      *Kernel,
  IN OUT VOID  *KernelSetup
  );

VOID*
EFIAPI
LoadLinuxAllocateInitrdPages (
  IN VOID                   *KernelSetup,
  IN UINTN                  Pages
  );

EFI_GUID gLinuxBootInitrdFileGuid = LINUX_BOOT_INITRD_GUID;

EFI_GUID gLinuxBootKernelFileGuid = LINUX_BOOT_KERNEL_GUID;

//---------------------------------------------------------------------------

/**
  Dump some hexadecimal data to the screen.

  @note Function taken from ShellPkg/Library/UefiShellCommandLib/UefiShellCommandLib.c in EDKII

  @param[in] Indent     How many spaces to indent the output.
  @param[in] Offset     The offset of the printing.
  @param[in] DataSize   The size in bytes of UserData.
  @param[in] UserData   The data to print out.
**/
static
VOID
DumpHex (
  IN UINTN        Indent,
  IN UINTN        Offset,
  IN UINTN        DataSize,
  IN VOID         *UserData
  )
{
  UINT8 *Data;
  CHAR8 Val[50];
  CHAR8 Str[20];
  UINT8 TempByte;
  UINTN Size;
  UINTN Index;
  CHAR8 Hex[] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
  };

  DEBUG((DEBUG_INFO, "%*a          00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n", Indent, ""));
  DEBUG((DEBUG_INFO, "%*a          ------------------------------------------------\n", Indent, ""));

  Data = UserData;
  while (DataSize != 0) {
    Size = 16;
    if (Size > DataSize) {
      Size = DataSize;
    }

    for (Index = 0; Index < Size; Index += 1) {
      TempByte            = Data[Index];
      Val[Index * 3 + 0]  = Hex[TempByte >> 4];
      Val[Index * 3 + 1]  = Hex[TempByte & 0xF];
      Val[Index * 3 + 2]  = (CHAR8) ((Index == 7) ? '-' : ' ');
      Str[Index]          = (CHAR8) ((TempByte < ' ' || TempByte > 'z') ? '.' : TempByte);
    }

    Val[Index * 3]  = 0;
    Str[Index]      = 0;
    DEBUG((DEBUG_INFO, "%*a%08X: %-48a  %a\n", Indent, "", Offset, Val, Str));

    Data += Size;
    Offset += Size;
    DataSize -= Size;
  }
}


/**
 *  This function completes a minimal amount of the necessary BDS functions to prepare
 *  for booting the kernel.
 *
 *  @param  None
 *
 *  @retval EFI_SUCCESS         Successfully completed remaining tasks
 *  @return EFI_ERROR           Could not complete BDS tasks
 */
EFI_STATUS
CompleteBdsTasks (
    VOID
)
{

    return EFI_SUCCESS;
}

/**
 *  This function will load and launch the Linux kernel from a BIOS FV.
 *
 *  @note   This function is not intended to return. Any exiting from this function indicates
 *          a problem loading or launching the kernel.
 *
 *  @param  None
 *
 *  @return EFI_ERROR           Any error code
 */
EFI_STATUS
LoadAndLaunchKernel (
    VOID
)
{
    EFI_LOADED_IMAGE_PROTOCOL   *LoadedImage = NULL;
    EFI_STATUS                  Status;
    EFI_HANDLE                  KernelHandle = NULL;
    VOID                        *KernelBuffer = NULL;
    VOID                        *KernelFfsBuffer = NULL;
    UINTN                       KernelFfsSize = 0;
    VOID                        *InitrdData = NULL;
    VOID                        *InitrdBuffer = NULL;
    UINTN                       InitrdSize = 0;
    struct BootParams          *BootParams = NULL;
    struct BootParams          *HandoverParams = NULL;
    UINT32                      StartOffset = 0;
    UINT32                      KernelLength = 0;
    UINT8                       *Temp;
    UINT8                       CmdLine[] = " ";

    DEBUG((DEBUG_INFO, "LoadAndLaunchKernel Entry\n"));

    ///
    /// Kernel load and preparation
    ///
    DEBUG((DEBUG_INFO, "Preparing the kernel...\n"));

    // Retrieve the kernel from the firmware volume
    Status = GetSectionFromAnyFv(
        &gLinuxBootKernelFileGuid,
        EFI_SECTION_PE32,
        0,
        &KernelFfsBuffer,
        &KernelFfsSize
    );

    DEBUG((DEBUG_INFO, "Status %r\n",Status));
    DEBUG((DEBUG_INFO, "KernelFfsBuffer %x\n",KernelFfsBuffer));
    DEBUG((DEBUG_INFO, "KernelFfsSize %x\n",KernelFfsSize));

    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_ERROR, "Could not retrieve kernel; %r.\n", Status));
        goto FatalError;
    }

    DEBUG((DEBUG_INFO, "Loaded kernel to buffer at 0x%p with size 0x%X.\n", KernelFfsBuffer, KernelFfsSize));
    DEBUG((DEBUG_INFO, "Printing first 0x%X bytes:\n", MIN(KernelFfsSize, 0x100)));

    DumpHex(2, 0, MIN(0x100, KernelFfsSize), KernelFfsBuffer);

    // Create a LoadImage protocol for the kernel
    Status = gBS->LoadImage(TRUE, gImageHandle, NULL, KernelFfsBuffer, KernelFfsSize, &KernelHandle);
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_ERROR, "Could not create LoadImage for kernel %r\n", Status));
        goto FatalError;
    }

    // Get the new LoadedImage protocol to retrieve information about the kernel
    Status = gBS->HandleProtocol(KernelHandle, &gEfiLoadedImageProtocolGuid, (VOID **) &LoadedImage);
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_ERROR, "Could not get kernel LoadedImage protocol; %r\n", Status));
        goto FatalError;
    }

    DEBUG((DEBUG_INFO, "Kernel LoadedImage information:\n"));
    DEBUG((DEBUG_INFO, "  ImageBase = 0x%p\n", LoadedImage->ImageBase));
    DEBUG((DEBUG_INFO, "  ImageSize = 0x%p\n", LoadedImage->ImageSize));

    // Verify the kernel boot parameters from the LoadedImage and allocate an initalization buffer once verified
    BootParams = (struct BootParams*) LoadedImage->ImageBase;

    Status = LoadLinuxCheckKernelSetup((VOID *) BootParams, sizeof(struct BootParams));
    if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, "LoadLinuxCheckKernelSetup failed; %r.\n", Status));
        goto FatalError;
    }

    HandoverParams = LoadLinuxAllocateKernelSetupPages(EFI_SIZE_TO_PAGES(KERNEL_SETUP_SIZE));
    if (HandoverParams == NULL) {
        DEBUG((DEBUG_ERROR, "Could not allocate memory for kernel handover parameters.\n"));
        goto FatalError;
    }
    DEBUG((DEBUG_INFO, "Handover parameters allocated at 0x%p\n", HandoverParams));

    gBS->CopyMem(&HandoverParams->Hdr, &BootParams->Hdr, sizeof(struct SetupHeader));

    Status = LoadLinuxInitializeKernelSetup(HandoverParams);
    if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, "Unable to initialize the handover parameters; %r.\n", Status));
        goto FatalError;
    }

    // Allocate space for the kernel and copy it into the new buffer
    KernelBuffer = LoadLinuxAllocateKernelPages(HandoverParams, EFI_SIZE_TO_PAGES(HandoverParams->Hdr.InitSize));
    if (KernelBuffer == NULL) {
        DEBUG((DEBUG_ERROR, "Unable to allocate memory for kernel.\n"));
        goto FatalError;
    }

    StartOffset = (HandoverParams->Hdr.SetupSecs + 1) * 512;
    KernelLength = (UINT32) (KernelFfsSize - StartOffset);
    Temp = (UINT8 *) LoadedImage->ImageBase;

    DEBUG((DEBUG_INFO, "Kernel starts at offset 0x%X with length 0x%X\n", StartOffset, KernelLength));

    gBS->CopyMem(KernelBuffer, (Temp + StartOffset), KernelLength);
    DEBUG((DEBUG_INFO, "First 0x%X bytes of new kernel buffer contents:\n", MIN(0x100, KernelLength)));

    DumpHex(2, 0, MIN(0x100, KernelLength), KernelBuffer);

    // Prepare the command line
    Status = LoadLinuxSetCommandLine(HandoverParams, (UINT8 *) &CmdLine);
    if (EFI_ERROR (Status)) {
        DEBUG((EFI_D_INFO, "Unable to set linux command line; %r.\n", Status));
        goto FatalError;
    }

    HandoverParams->Hdr.Code32Start = (UINT32)(UINTN) KernelBuffer;
    HandoverParams->Hdr.LoaderId = 0x21;

    DEBUG((DEBUG_INFO, "Kernel loaded.\n"));

    //
    // Initrd load and preparation
    //
    DEBUG((DEBUG_INFO, "Preparing the initrd...\n"));

    // Retrieve the initrd from the firmware volume
    Status = GetSectionFromAnyFv(
        &gLinuxBootInitrdFileGuid,
        EFI_SECTION_RAW,
        0,
        &InitrdBuffer,
        &InitrdSize
    );

    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_ERROR, "Could not retrieve initrd; %r.\n", Status));
        goto FatalError;
    }

    DEBUG((DEBUG_INFO, "Loaded initrd to buffer at 0x%p with size 0x%X.\n", InitrdBuffer, InitrdSize));
    DEBUG((DEBUG_INFO, "Printing first 0x%X bytes:\n", MIN(0x100, InitrdSize)));
    DumpHex(2, 0, MIN(0x100, InitrdSize), InitrdBuffer);

    // Allocate the initrd for the kernel and copy it in
    InitrdData = LoadLinuxAllocateInitrdPages(HandoverParams, EFI_SIZE_TO_PAGES(InitrdSize));
    if (InitrdData == NULL) {
        DEBUG((DEBUG_ERROR, "Unable to allocate memory for initrd.\n"));
        goto FatalError;
    }

    gBS->CopyMem(InitrdData, InitrdBuffer, InitrdSize);

    HandoverParams->Hdr.RamDiskStart = (UINT32)(UINTN) InitrdData;
    HandoverParams->Hdr.RamDiskLen = (UINT32) InitrdSize;

    DEBUG((DEBUG_INFO, "Initrd loaded.\n"));
    DEBUG((DEBUG_INFO, "Printing first 0x%X bytes of initrd buffer:\n", MIN(0x100, InitrdSize)));
    DumpHex(2, 0, MIN(0x100, InitrdSize), InitrdData);

    // General cleanup before launching the kernel
    gBS->FreePool(InitrdBuffer);
    InitrdBuffer = NULL;

    gBS->UnloadImage(KernelHandle);
    gBS->FreePool(KernelFfsBuffer);
    KernelFfsBuffer = NULL;

    DEBUG((DEBUG_ERROR, "Launching the kernel\n"));


    //
    // Signal the EFI_EVENT_GROUP_READY_TO_BOOT event.
    //
    EfiSignalEventReadyToBoot();


    // Launch the kernel
    Status = LoadLinux(KernelBuffer, HandoverParams);

    ///
    /// LoadLinux should never return if the kernel boots. Anything past here is an error scenario
    ///
    DEBUG((DEBUG_ERROR, "ERROR: LoadLinux has returned with status; %r.\n", Status));

FatalError:
    // Free everything
    if (InitrdData != NULL) gBS->FreePages((EFI_PHYSICAL_ADDRESS) InitrdData, EFI_SIZE_TO_PAGES(InitrdSize));
    if (KernelBuffer != NULL) gBS->FreePages((EFI_PHYSICAL_ADDRESS) KernelBuffer, EFI_SIZE_TO_PAGES(HandoverParams->Hdr.InitSize));
    if (HandoverParams != NULL) gBS->FreePages((EFI_PHYSICAL_ADDRESS) HandoverParams, EFI_SIZE_TO_PAGES(KERNEL_SETUP_SIZE));
    if (InitrdBuffer != NULL) gBS->FreePool(InitrdBuffer);
    if (KernelHandle != NULL) gBS->UnloadImage(KernelHandle);
    if (KernelFfsBuffer != NULL) gBS->FreePool(KernelFfsBuffer);

    return EFI_NOT_FOUND;
}

/**
 *  This is the main function for this feature. This will handle finding and launching
 *  the Linux kernel.
 *
 *  @note   In general, this function will never return to BDS. The LINUXBOOT_ALLOW_RETURN_TO_BDS
 *          token will allow you to return to BDS if the kernel fails to launch for some reason.
 *
 *  @param  None
 *
 *  @retval None
 */

EFI_STATUS
LinuxBootStart (
  VOID
  )
{
    EFI_STATUS  Status = EFI_SUCCESS;

    // Finish BDS and then try to launch the kernel
    //Status = CompleteBdsTasks();

    if (!EFI_ERROR(Status)) {
        LoadAndLaunchKernel();
    }

    DEBUG((DEBUG_ERROR, "-----------------------------------\n"));
    DEBUG((DEBUG_ERROR, "  ERROR: Kernel failed to launch.\n"));
    DEBUG((DEBUG_ERROR, "-----------------------------------\n"));
    return Status;
}

//---------------------------------------------------------------------------
