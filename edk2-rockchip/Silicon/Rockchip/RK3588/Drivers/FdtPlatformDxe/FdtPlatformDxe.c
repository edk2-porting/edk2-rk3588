/** @file
 *
 *  Flattened Device Tree platform driver
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <libfdt.h>

#include <Guid/Fdt.h>
#include <Guid/FileInfo.h>

#include <VarStoreData.h>

#define MAX_PATH_LENGTH  512

STATIC  VOID   *mPlatformFdt;
STATIC  VOID   *mLoadedImageEventRegistration;

STATIC
INTN
EFIAPI
StriCmp (
  IN CONST  CHAR16  *String1,
  IN CONST  CHAR16  *String2
  )
{
  while ((*String1 != L'\0') &&
         (CharToUpper (*String1) == CharToUpper (*String2))) {
    String1++;
    String2++;
  }

  return CharToUpper (*String1) - CharToUpper (*String2);
}

STATIC
BOOLEAN
EFIAPI
StrEndsWith (
  IN CONST  CHAR16  *String,
  IN CONST  CHAR16  *Extension
  )
{
  UINTN StringLength = StrLen (String);
  UINTN ExtensionLength = StrLen (Extension);

  if (StringLength < ExtensionLength) {
    return FALSE;
  }

  return StriCmp (String + StringLength - ExtensionLength, Extension) == 0;
}

STATIC
EFI_STATUS
EFIAPI
FdtOpenIntoAlloc (
  IN OUT  VOID    **Fdt,
  IN OUT  VOID    **DestinationFdt,  OPTIONAL
  IN      UINTN   Size
  )
{
  VOID  *NewFdt;
  INTN  Ret;

  NewFdt = AllocatePool (Size);
  if (NewFdt == NULL) {
    DEBUG ((DEBUG_ERROR, "FdtPlatform: Not enough resources to reallocate FDT (%d bytes).\n",
            Size));
    return EFI_OUT_OF_RESOURCES;
  }

  Ret = fdt_open_into (*Fdt, NewFdt, Size);
  if (Ret) {
    DEBUG ((DEBUG_ERROR, "FdtPlatform: Failed to copy FDT. Ret=%a\n", fdt_strerror (Ret)));
    FreePool (NewFdt);
    return EFI_LOAD_ERROR;
  }

  if (DestinationFdt != NULL) {
    *DestinationFdt = NewFdt;
  } else {
    FreePool (*Fdt);
    *Fdt = NewFdt;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
ReadFdtFromFilePath (
  IN      EFI_FILE_PROTOCOL   *Root,
  IN      CHAR16              *Path,
  IN OUT  UINTN               *FileSize,  OPTIONAL
  IN OUT  VOID                **Fdt
  )
{
  EFI_STATUS          Status;
  EFI_FILE_PROTOCOL   *File;
  UINTN               FileBufferSize;
  UINTN               FileInfoSize;
  EFI_FILE_INFO       *FileInfo = NULL;
  INT32               Ret;

  Status = Root->Open (Root, &File, Path, EFI_FILE_MODE_READ, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FdtPlatform: Couldn't open '%s'. Status=%r\n", Path, Status));
    return Status;
  }

  if (FileSize == NULL) {
    FileInfoSize = 0;
    Status = File->GetInfo (File, &gEfiFileInfoGuid, &FileInfoSize, NULL);
    if (EFI_ERROR (Status) && FileInfoSize == 0) {
      DEBUG ((DEBUG_ERROR, "FdtPlatform: Failed to get '%s' file info size. Status=%r\n",
              Path, Status));
      goto Exit;
    }

    FileInfo = AllocatePool (FileInfoSize);
    if (FileInfo == NULL) {
      DEBUG ((DEBUG_ERROR, "FdtPlatform: Not enough resources for '%s' file info (%d bytes).\n",
              Path, FileInfoSize));
      Status = EFI_OUT_OF_RESOURCES;
      goto Exit;
    }

    Status = File->GetInfo (File, &gEfiFileInfoGuid, &FileInfoSize, FileInfo);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "FdtPlatform: Failed to get '%s' file info. Status=%r\n",
              Path, Status));
      goto Exit;
    }
    FileBufferSize = FileInfo->FileSize;
  } else {
    FileBufferSize = *FileSize;
  }

  *Fdt = AllocatePool (FileBufferSize);
  if (*Fdt == NULL) {
    DEBUG ((DEBUG_ERROR, "FdtPlatform: Not enough resources for '%s' file buffer (%d bytes).\n",
            Path, FileBufferSize));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Status = File->Read (File, &FileBufferSize, *Fdt);
  if (EFI_ERROR (Status)) {
    if (FileSize != NULL) {
      *FileSize = FileBufferSize;
    }
    DEBUG ((DEBUG_ERROR, "FdtPlatform: Failed to read '%s' (%d bytes). Status=%r\n",
            Path, FileBufferSize, Status));
    goto Exit;
  }

  Ret = fdt_check_header (*Fdt);
  if (Ret) {
    DEBUG ((DEBUG_ERROR, "FdtPlatform: '%s' has an invalid header! Ret=%a\n",
            Path, fdt_strerror (Ret)));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

Exit:
  Root->Close (File);
  if (FileInfo != NULL) {
    FreePool (FileInfo);
  }
  if (EFI_ERROR (Status) && *Fdt != NULL) {
    FreePool (*Fdt);
    *Fdt = NULL;
  }
  return Status;
}

#define FDT_GET_USED_SIZE(Fdt)  (fdt_off_dt_struct (Fdt)       \
                                 + fdt_size_dt_struct (Fdt)    \
                                 + fdt_size_dt_strings (Fdt))

STATIC
EFI_STATUS
EFIAPI
InstallOverlaysFromDirectoryPath (
  IN      EFI_FILE_PROTOCOL   *Root,
  IN      CHAR16              *Path,
  IN OUT  VOID                **Fdt,
  IN OUT  UINTN               *OverlaysCount
  )
{
  EFI_STATUS            Status;
  EFI_FILE_PROTOCOL     *Dir;
  UINTN                 DirEntryInfoSize;
  UINTN                 CurrentInfoSize;
  EFI_FILE_INFO         *DirEntryInfo;
  VOID                  *FdtOverlay;
  UINTN                 FdtSize;
  INT32                 Ret;

  Status = Root->Open (Root, &Dir, Path, EFI_FILE_MODE_READ, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FdtPlatform: Couldn't open directory '%s'. Status=%r\n",
            Path, Status));
    return Status;
  }

  DirEntryInfoSize = sizeof (EFI_FILE_INFO) + MAX_PATH_LENGTH;
  DirEntryInfo = AllocatePool (DirEntryInfoSize);
  if (DirEntryInfo == NULL) {
    DEBUG ((DEBUG_ERROR, "FdtPlatform: Not enough resources for '%s' directory entry info.\n",
            Path));
    Root->Close (Dir);
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, "FdtPlatform: Processing overlays in sub-directory '%s'\n", Path));

  while (TRUE) {
    CurrentInfoSize = DirEntryInfoSize;
    Status = Dir->Read (Dir, &CurrentInfoSize, (VOID *) DirEntryInfo);
    if (EFI_ERROR (Status) || CurrentInfoSize == 0) {
      // Reached end of directory?
      ASSERT (CurrentInfoSize <= DirEntryInfoSize);
      ASSERT_EFI_ERROR (Status);
      break;
    }

    if (DirEntryInfo->Attribute & EFI_FILE_DIRECTORY) {
      continue;
    }

    if (!StrEndsWith (DirEntryInfo->FileName, L".dtbo")) {
      continue;
    }

    DEBUG ((DEBUG_INFO, "FdtPlatform: Installing overlay '%s'\n",
            DirEntryInfo->FileName));

    Status = ReadFdtFromFilePath (Dir, DirEntryInfo->FileName,
                      &DirEntryInfo->FileSize, &FdtOverlay);
    if (EFI_ERROR (Status)) {
      if (Status == EFI_OUT_OF_RESOURCES) {
        break;
      }
      continue;
    }

    FdtSize = FDT_GET_USED_SIZE (*Fdt);
    if (FdtSize + DirEntryInfo->FileSize >= fdt_totalsize (*Fdt)) {
      //
      // Expand the buffer by at least 8 KB, so we don't end up
      // reallocating for every small overlay.
      //
      FdtSize = fdt_totalsize (*Fdt) + MAX (DirEntryInfo->FileSize, SIZE_8KB);
      Status = FdtOpenIntoAlloc (Fdt, NULL, FdtSize);
      if (EFI_ERROR (Status)) {
        FreePool (FdtOverlay);
        break;
      }
    }

    Ret = fdt_overlay_apply (*Fdt, FdtOverlay);
    FreePool (FdtOverlay);
    if (Ret) {
      DEBUG ((DEBUG_ERROR, "FdtPlatform: Failed to apply overlay '%s' (%d bytes). Ret=%a\n",
              DirEntryInfo->FileName, DirEntryInfo->FileSize, fdt_strerror (Ret)));

      if (Ret == -FDT_ERR_NOSPACE) {
        DEBUG ((DEBUG_ERROR, "FdtPlatform:   FDT bytes used: %d, total: %d\n",
                 FDT_GET_USED_SIZE (*Fdt), fdt_totalsize (*Fdt)));
      }
      //
      // The FDT is damaged at this point, we can't continue.
      //
      Status = EFI_LOAD_ERROR;
      break;
    }

    *OverlaysCount += 1;
  }

  FreePool (DirEntryInfo);
  Root->Close (Dir);

  return Status;
}

STATIC  CHAR16   mDtbOverrideRootPath[] = L"\\dtb";

STATIC
EFI_STATUS
EFIAPI
FdtPlatformProcessFileSystem (
  IN  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem
  )
{
  EFI_STATUS          Status;
  EFI_FILE_PROTOCOL   *Root;
  EFI_FILE_PROTOCOL   *DtbDir;
  UINTN               PathSize;
  CHAR16              *Path;
  CHAR8               *FdtName;
  VOID                *Fdt = NULL;
  VOID                *NewFdt = NULL;
  VOID                *FdtToInstall = NULL;
  UINTN               OverlaysCount = 0;

  Status = FileSystem->OpenVolume (FileSystem, &Root);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FdtPlatform: Failed to open volume. Status=%r\n", Status));
    return Status;
  }

  Status = Root->Open (Root, &DtbDir, mDtbOverrideRootPath, EFI_FILE_MODE_READ, 0);
  if (EFI_ERROR (Status)) {
    if (Status != EFI_NOT_FOUND) {
      DEBUG ((DEBUG_ERROR, "FdtPlatform: Failed to open directory '%s'. Status=%r\n",
              mDtbOverrideRootPath, Status));
    }
    return Status;
  } else {
    DEBUG ((DEBUG_INFO, "FdtPlatform: Found override directory '%s'.\n",
            mDtbOverrideRootPath));
  }

  PathSize = MAX_PATH_LENGTH * sizeof (CHAR16);
  Path = AllocatePool (PathSize);
  if (Path == NULL) {
    DEBUG ((DEBUG_ERROR, "FdtPlatform: Not enough resources for '%s' access path.\n",
            mDtbOverrideRootPath));
    Root->Close (DtbDir);
    return EFI_OUT_OF_RESOURCES;
  }

  FdtName = FixedPcdGetPtr (PcdDeviceTreeName);

  //
  // Try to open the FDT override.
  //
  UnicodeSPrint (Path, PathSize, L"base\\%a.dtb", FdtName);
  Status = ReadFdtFromFilePath (DtbDir, Path, NULL, &Fdt);
  if (EFI_ERROR (Status)) {
    if (mPlatformFdt == NULL) {
      goto Exit;
    }
    Fdt = mPlatformFdt;
  } else {
    DEBUG ((DEBUG_INFO, "FdtPlatform: Loaded FDT override '%s'.\n", Path));
  }

  //
  // Clone the FDT so that we can restore the original one
  // in case it gets damaged.
  //
  Status = FdtOpenIntoAlloc (&Fdt, &NewFdt, fdt_totalsize (Fdt));
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  //
  // Process the overlays common to all platforms.
  //
  Status = InstallOverlaysFromDirectoryPath (DtbDir, L"overlays", &NewFdt, &OverlaysCount);
  if (Status == EFI_LOAD_ERROR || Status == EFI_OUT_OF_RESOURCES) {
    goto Exit;
  }

  //
  // Process the platform-specific overlays.
  //
  UnicodeSPrint (Path, PathSize, L"overlays\\%a", FdtName);
  Status = InstallOverlaysFromDirectoryPath (DtbDir, Path, &NewFdt, &OverlaysCount);
  if (Status == EFI_LOAD_ERROR || Status == EFI_OUT_OF_RESOURCES) {
    goto Exit;
  }

Exit:
  Root->Close (DtbDir);
  FreePool (Path);

  if (NewFdt != NULL) {
    if (fdt_check_header (NewFdt) == 0) {
      FdtToInstall = NewFdt;
      DEBUG ((DEBUG_INFO, "FdtPlatform: Using FDT with %d overlays merged.\n", OverlaysCount));
    } else {
      FreePool (NewFdt);
    }
  }
  if (Fdt != NULL && Fdt != mPlatformFdt) {
    if (FdtToInstall == NULL && fdt_check_header (Fdt) == 0) {
      FdtToInstall = Fdt;
      DEBUG ((DEBUG_INFO, "FdtPlatform: Using original FDT without overlays.\n"));
    } else {
      FreePool (Fdt);
    }
  }

  if (FdtToInstall != NULL) {
    Status = gBS->InstallConfigurationTable (&gFdtTableGuid, FdtToInstall);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "FdtPlatform: Failed to install the new FDT as config table. Status=%r\n",
              Status));
    }
  }

  return Status;
}

STATIC
VOID
EFIAPI
NotifyLoadedImage (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                          Status;
  EFI_HANDLE                          *Handles;
  UINTN                               HandleCount;
  EFI_LOADED_IMAGE_PROTOCOL           *LoadedImage;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL     *FileSystem;

  while (TRUE) {
    Status = gBS->LocateHandleBuffer (
                    ByRegisterNotify,
                    NULL,
                    mLoadedImageEventRegistration,
                    &HandleCount,
                    &Handles
                    );
    if (EFI_ERROR (Status)) {
      if (Status != EFI_NOT_FOUND) {
        DEBUG ((DEBUG_ERROR, "FdtPlatform: Failed to locate gEfiLoadedImageProtocolGuid. Status=%r\n",
                Status));
      }
      break;
    }
    ASSERT (HandleCount == 1);

    Status = gBS->HandleProtocol (
                    Handles[0],
                    &gEfiLoadedImageProtocolGuid,
                    (VOID **)&LoadedImage
                    );
    FreePool (Handles);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "FdtPlatform: Failed to get gEfiLoadedImageProtocolGuid. Status=%r\n",
              Status));
      break;
    }

    if (LoadedImage->DeviceHandle == NULL) {
      continue;
    }

    Status = gBS->HandleProtocol (
                    LoadedImage->DeviceHandle,
                    &gEfiSimpleFileSystemProtocolGuid,
                    (VOID **)&FileSystem
                    );
    if (EFI_ERROR (Status)) {
      if (Status != EFI_UNSUPPORTED) {
        DEBUG ((DEBUG_ERROR, "FdtPlatform: Failed to get gEfiSimpleFileSystemProtocolGuid. Status=%r\n",
                Status));
      }
      continue;
    }

    Status = FdtPlatformProcessFileSystem (FileSystem);
    if (EFI_ERROR (Status)) {
      if (Status != EFI_NOT_FOUND) {
        DEBUG ((DEBUG_ERROR, "FdtPlatform: Failed to process the file system. Status=%r\n", Status));
      }
      continue;
    }
  }
}

STATIC
VOID
EFIAPI
NotifyReadyToBoot (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  gBS->CloseEvent (Event);

  EfiCreateProtocolNotifyEvent (
        &gEfiLoadedImageProtocolGuid,
        TPL_CALLBACK,
        NotifyLoadedImage,
        NULL,
        &mLoadedImageEventRegistration
        );
}

STATIC
VOID
EFIAPI
NotifyExitBootServices (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS    Status;
  VOID          *Dtb;

  Status = EfiGetSystemConfigurationTable (&gFdtTableGuid, &Dtb);
  if (EFI_ERROR (Status) || Dtb == NULL) {
    DEBUG ((DEBUG_WARN, "FdtPlatform: No FDT installed!\n"));
  } else if (fdt_totalsize (Dtb) <= SIZE_4KB) {
    // Some loaders may install a dummy table, warn in this case too.
    DEBUG ((DEBUG_WARN, "FdtPlatform: No usable FDT installed!\n"));
  }
}

STATIC
EFI_STATUS
EFIAPI
FdtEnableNode (
  IN         VOID       *Fdt,
  IN CONST   CHAR8      *NodePath,
  IN         BOOLEAN    Enable
  )
{
  INT32   Node;
  INT32   Ret;
  CHAR8   *NodeStatus;

  Node = fdt_path_offset (Fdt, NodePath);
  if (Node < 0) {
    DEBUG ((DEBUG_ERROR, "FdtPlatform: Couldn't locate FDT node path '%a'. Ret=%a\n",
            NodePath, fdt_strerror (Node)));
    return EFI_NOT_FOUND;
  }

  NodeStatus = Enable ? "okay" : "disabled";
  Ret = fdt_setprop_string (Fdt, Node, "status", NodeStatus);
  if (Ret) {
    DEBUG ((DEBUG_ERROR, "FdtPlatform: Failed to set '%a' status to '%a'. Ret=%a\n",
            NodePath, NodeStatus, fdt_strerror (Ret)));
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

STATIC
VOID
EFIAPI
FdtFixupComboPhyDevices (
  IN VOID  *Fdt
  )
{
  DEBUG ((DEBUG_INFO, "FdtPlatform: Fixing up Combo PHY devices (PCIe, SATA, USB)\n"));

  FdtEnableNode (Fdt, "/pcie@fe170000",
              PcdGet32 (PcdComboPhy1Mode) == COMBO_PHY_MODE_PCIE);
  FdtEnableNode (Fdt, "/pcie@fe180000",
              PcdGet32 (PcdComboPhy2Mode) == COMBO_PHY_MODE_PCIE);
  FdtEnableNode (Fdt, "/pcie@fe190000",
              PcdGet32 (PcdComboPhy0Mode) == COMBO_PHY_MODE_PCIE);

  FdtEnableNode (Fdt, "/sata@fe210000",
              PcdGet32 (PcdComboPhy0Mode) == COMBO_PHY_MODE_SATA);
  FdtEnableNode (Fdt, "/sata@fe220000",
              PcdGet32 (PcdComboPhy1Mode) == COMBO_PHY_MODE_SATA);
  FdtEnableNode (Fdt, "/sata@fe230000",
              PcdGet32 (PcdComboPhy2Mode) == COMBO_PHY_MODE_SATA);

  FdtEnableNode (Fdt, "/usbhost3_0",
              PcdGet32 (PcdComboPhy2Mode) == COMBO_PHY_MODE_USB3);
}

STATIC
VOID
EFIAPI
FdtFixupPcie3Devices (
  IN VOID  *Fdt
  )
{
  if (!FixedPcdGetBool (PcdPcie30Supported)) {
    return;
  }

  DEBUG ((DEBUG_INFO, "FdtPlatform: Fixing up PCIe 3 devices\n"));

  FdtEnableNode (Fdt, "/pcie@fe150000",
              PcdGet32 (PcdPcie30State) == PCIE30_STATE_ENABLED);
}

STATIC
VOID
EFIAPI
ApplyPlatformFdtFixups (
  IN VOID  *Fdt
  )
{
  FdtFixupComboPhyDevices (Fdt);
  FdtFixupPcie3Devices (Fdt);
}

STATIC
EFI_STATUS
EFIAPI
LoadPlatformFdt (
  OUT   VOID  **PlatformFdt
  )
{
  EFI_STATUS    Status;
  VOID          *Fdt;
  UINTN         FdtSize;
  INT32         Ret;

  Status = GetSectionFromAnyFv (
             &gDtPlatformDefaultDtbFileGuid,
             EFI_SECTION_RAW,
             0,
             &Fdt,
             &FdtSize
             );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  Ret = fdt_check_header (Fdt);
  if (Ret) {
    DEBUG ((DEBUG_ERROR, "FdtPlatform: Firmware FDT has an invalid header! Ret=%a\n",
            fdt_strerror (Ret)));
    return EFI_NOT_FOUND;
  }

  // Expand the FDT a bit to give room for any additions.
  Status = FdtOpenIntoAlloc (&Fdt, NULL, fdt_totalsize (Fdt) + SIZE_1KB);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *PlatformFdt = Fdt;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
FdtPlatformDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS    Status;
  EFI_EVENT     Event;

  if ((PcdGet32 (PcdConfigTableMode) & CONFIG_TABLE_MODE_FDT) == 0) {
    DEBUG ((DEBUG_WARN, "FdtPlatform: FDT support is disabled by the settings.\n"));
    return EFI_UNSUPPORTED;
  }

  if (PcdGet8 (PcdFdtSupportOverrides)) {
    Status = gBS->CreateEventEx (
                      EVT_NOTIFY_SIGNAL,              // Type
                      TPL_CALLBACK,                   // NotifyTpl
                      NotifyReadyToBoot,              // NotifyFunction
                      NULL,                           // NotifyContext
                      &gEfiEventReadyToBootGuid,      // EventGroup
                      &Event                          // Event
                      );
    ASSERT_EFI_ERROR (Status);
  }

  Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,                // Type
                    TPL_CALLBACK,                     // NotifyTpl
                    NotifyExitBootServices,           // NotifyFunction
                    NULL,                             // NotifyContext
                    &gEfiEventExitBootServicesGuid,   // EventGroup
                    &Event                            // Event
                    );
  ASSERT_EFI_ERROR (Status);

  Status = LoadPlatformFdt (&mPlatformFdt);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "FdtPlatform: Failed to load firmware FDT. Status=%r\n", Status));
    return EFI_SUCCESS;
  }

  ApplyPlatformFdtFixups (mPlatformFdt);

  Status = gBS->InstallConfigurationTable (&gFdtTableGuid, mPlatformFdt);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "FdtPlatform: Failed to install firmware DTB as config table. Status=%r\n",
            Status));
  }

  return EFI_SUCCESS;
}
