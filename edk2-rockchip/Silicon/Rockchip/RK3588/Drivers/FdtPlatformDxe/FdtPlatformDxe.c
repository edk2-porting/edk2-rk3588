/** @file
 *
 *  Flattened Device Tree platform driver
 *
 *  Copyright (c) 2023-2024, Mario Bălănică <mariobalanica02@gmail.com>
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
#include <Library/RockchipPlatformLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <libfdt.h>

#include <Guid/Fdt.h>
#include <Guid/FileInfo.h>

#include <VarStoreData.h>

#include <dt-bindings/clock/rockchip,rk3588-cru.h>
#include <dt-bindings/power/rk3588-power.h>

// Expand and stringize
#define XS(x)  S(x)
#define S(x)   #x

#define MAX_PATH_LENGTH  512

STATIC  VOID  *mPlatformFdt;
STATIC  VOID  *mLoadedImageEventRegistration;

STATIC
INTN
EFIAPI
StriCmp (
  IN CONST  CHAR16  *String1,
  IN CONST  CHAR16  *String2
  )
{
  while ((*String1 != L'\0') &&
         (CharToUpper (*String1) == CharToUpper (*String2)))
  {
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
  UINTN  StringLength    = StrLen (String);
  UINTN  ExtensionLength = StrLen (Extension);

  if (StringLength < ExtensionLength) {
    return FALSE;
  }

  return StriCmp (String + StringLength - ExtensionLength, Extension) == 0;
}

STATIC
EFI_STATUS
EFIAPI
FdtOpenIntoAlloc (
  IN OUT  VOID **Fdt,
  IN OUT  VOID **DestinationFdt, OPTIONAL
  IN      UINTN   Size
  )
{
  VOID  *NewFdt;
  INTN  Ret;

  NewFdt = AllocatePool (Size);
  if (NewFdt == NULL) {
    DEBUG ((
      DEBUG_ERROR,
      "FdtPlatform: Not enough resources to reallocate FDT (%d bytes).\n",
      Size
      ));
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
  IN      EFI_FILE_PROTOCOL *Root,
  IN      CHAR16 *Path,
  IN OUT  UINTN *FileSize, OPTIONAL
  IN OUT  VOID                **Fdt
  )
{
  EFI_STATUS         Status;
  EFI_FILE_PROTOCOL  *File;
  UINTN              FileBufferSize;
  UINTN              FileInfoSize;
  EFI_FILE_INFO      *FileInfo = NULL;
  INT32              Ret;

  Status = Root->Open (Root, &File, Path, EFI_FILE_MODE_READ, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FdtPlatform: Couldn't open '%s'. Status=%r\n", Path, Status));
    return Status;
  }

  if (FileSize == NULL) {
    FileInfoSize = 0;
    Status       = File->GetInfo (File, &gEfiFileInfoGuid, &FileInfoSize, NULL);
    if (EFI_ERROR (Status) && (FileInfoSize == 0)) {
      DEBUG ((
        DEBUG_ERROR,
        "FdtPlatform: Failed to get '%s' file info size. Status=%r\n",
        Path,
        Status
        ));
      goto Exit;
    }

    FileInfo = AllocatePool (FileInfoSize);
    if (FileInfo == NULL) {
      DEBUG ((
        DEBUG_ERROR,
        "FdtPlatform: Not enough resources for '%s' file info (%d bytes).\n",
        Path,
        FileInfoSize
        ));
      Status = EFI_OUT_OF_RESOURCES;
      goto Exit;
    }

    Status = File->GetInfo (File, &gEfiFileInfoGuid, &FileInfoSize, FileInfo);
    if (EFI_ERROR (Status)) {
      DEBUG ((
        DEBUG_ERROR,
        "FdtPlatform: Failed to get '%s' file info. Status=%r\n",
        Path,
        Status
        ));
      goto Exit;
    }

    FileBufferSize = FileInfo->FileSize;
  } else {
    FileBufferSize = *FileSize;
  }

  *Fdt = AllocatePool (FileBufferSize);
  if (*Fdt == NULL) {
    DEBUG ((
      DEBUG_ERROR,
      "FdtPlatform: Not enough resources for '%s' file buffer (%d bytes).\n",
      Path,
      FileBufferSize
      ));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Status = File->Read (File, &FileBufferSize, *Fdt);
  if (EFI_ERROR (Status)) {
    if (FileSize != NULL) {
      *FileSize = FileBufferSize;
    }

    DEBUG ((
      DEBUG_ERROR,
      "FdtPlatform: Failed to read '%s' (%d bytes). Status=%r\n",
      Path,
      FileBufferSize,
      Status
      ));
    goto Exit;
  }

  Ret = fdt_check_header (*Fdt);
  if (Ret) {
    DEBUG ((
      DEBUG_ERROR,
      "FdtPlatform: '%s' has an invalid header! Ret=%a\n",
      Path,
      fdt_strerror (Ret)
      ));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

Exit:
  Root->Close (File);
  if (FileInfo != NULL) {
    FreePool (FileInfo);
  }

  if (EFI_ERROR (Status) && (*Fdt != NULL)) {
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
  IN      EFI_FILE_PROTOCOL  *Root,
  IN      CHAR16             *Path,
  IN OUT  VOID               **Fdt,
  IN OUT  UINTN              *OverlaysCount
  )
{
  EFI_STATUS         Status;
  EFI_FILE_PROTOCOL  *Dir;
  UINTN              DirEntryInfoSize;
  UINTN              CurrentInfoSize;
  EFI_FILE_INFO      *DirEntryInfo;
  VOID               *FdtOverlay;
  UINTN              FdtSize;
  INT32              Ret;

  Status = Root->Open (Root, &Dir, Path, EFI_FILE_MODE_READ, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "FdtPlatform: Couldn't open directory '%s'. Status=%r\n",
      Path,
      Status
      ));
    return Status;
  }

  DirEntryInfoSize = sizeof (EFI_FILE_INFO) + MAX_PATH_LENGTH;
  DirEntryInfo     = AllocatePool (DirEntryInfoSize);
  if (DirEntryInfo == NULL) {
    DEBUG ((
      DEBUG_ERROR,
      "FdtPlatform: Not enough resources for '%s' directory entry info.\n",
      Path
      ));
    Root->Close (Dir);
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, "FdtPlatform: Processing overlays in sub-directory '%s'\n", Path));

  while (TRUE) {
    CurrentInfoSize = DirEntryInfoSize;
    Status          = Dir->Read (Dir, &CurrentInfoSize, (VOID *)DirEntryInfo);
    if (EFI_ERROR (Status) || (CurrentInfoSize == 0)) {
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

    DEBUG ((
      DEBUG_INFO,
      "FdtPlatform: Installing overlay '%s'\n",
      DirEntryInfo->FileName
      ));

    Status = ReadFdtFromFilePath (
               Dir,
               DirEntryInfo->FileName,
               &DirEntryInfo->FileSize,
               &FdtOverlay
               );
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
      Status  = FdtOpenIntoAlloc (Fdt, NULL, FdtSize);
      if (EFI_ERROR (Status)) {
        FreePool (FdtOverlay);
        break;
      }
    }

    Ret = fdt_overlay_apply (*Fdt, FdtOverlay);
    FreePool (FdtOverlay);
    if (Ret) {
      DEBUG ((
        DEBUG_ERROR,
        "FdtPlatform: Failed to apply overlay '%s' (%d bytes). Ret=%a\n",
        DirEntryInfo->FileName,
        DirEntryInfo->FileSize,
        fdt_strerror (Ret)
        ));

      if (Ret == -FDT_ERR_NOSPACE) {
        DEBUG ((
          DEBUG_ERROR,
          "FdtPlatform:   FDT bytes used: %d, total: %d\n",
          FDT_GET_USED_SIZE (*Fdt),
          fdt_totalsize (*Fdt)
          ));
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

STATIC  CHAR16  mDtbOverrideRootPath[] = L"\\dtb";

STATIC
EFI_STATUS
EFIAPI
FdtPlatformProcessFileSystem (
  IN  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *FileSystem
  )
{
  EFI_STATUS         Status;
  EFI_FILE_PROTOCOL  *Root;
  EFI_FILE_PROTOCOL  *DtbDir;
  UINTN              PathSize;
  CHAR16             *Path;
  CHAR8              *FdtName;
  VOID               *Fdt          = NULL;
  VOID               *NewFdt       = NULL;
  VOID               *FdtToInstall = NULL;
  UINTN              OverlaysCount = 0;

  Status = FileSystem->OpenVolume (FileSystem, &Root);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FdtPlatform: Failed to open volume. Status=%r\n", Status));
    return Status;
  }

  Status = Root->Open (Root, &DtbDir, mDtbOverrideRootPath, EFI_FILE_MODE_READ, 0);
  if (EFI_ERROR (Status)) {
    if (Status != EFI_NOT_FOUND) {
      DEBUG ((
        DEBUG_ERROR,
        "FdtPlatform: Failed to open directory '%s'. Status=%r\n",
        mDtbOverrideRootPath,
        Status
        ));
    }

    return Status;
  } else {
    DEBUG ((
      DEBUG_INFO,
      "FdtPlatform: Found override directory '%s'.\n",
      mDtbOverrideRootPath
      ));
  }

  PathSize = MAX_PATH_LENGTH * sizeof (CHAR16);
  Path     = AllocatePool (PathSize);
  if (Path == NULL) {
    DEBUG ((
      DEBUG_ERROR,
      "FdtPlatform: Not enough resources for '%s' access path.\n",
      mDtbOverrideRootPath
      ));
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
  if ((Status == EFI_LOAD_ERROR) || (Status == EFI_OUT_OF_RESOURCES)) {
    goto Exit;
  }

  //
  // Process the platform-specific overlays.
  //
  UnicodeSPrint (Path, PathSize, L"overlays\\%a", FdtName);
  Status = InstallOverlaysFromDirectoryPath (DtbDir, Path, &NewFdt, &OverlaysCount);
  if ((Status == EFI_LOAD_ERROR) || (Status == EFI_OUT_OF_RESOURCES)) {
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

  if ((Fdt != NULL) && (Fdt != mPlatformFdt)) {
    if ((FdtToInstall == NULL) && (fdt_check_header (Fdt) == 0)) {
      FdtToInstall = Fdt;
      DEBUG ((DEBUG_INFO, "FdtPlatform: Using original FDT without overlays.\n"));
    } else {
      FreePool (Fdt);
    }
  }

  if (FdtToInstall != NULL) {
    Status = gBS->InstallConfigurationTable (&gFdtTableGuid, FdtToInstall);
    if (EFI_ERROR (Status)) {
      DEBUG ((
        DEBUG_ERROR,
        "FdtPlatform: Failed to install the new FDT as config table. Status=%r\n",
        Status
        ));
    }
  }

  return Status;
}

STATIC
VOID
EFIAPI
NotifyLoadedImage (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                       Status;
  EFI_HANDLE                       *Handles;
  UINTN                            HandleCount;
  EFI_LOADED_IMAGE_PROTOCOL        *LoadedImage;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *FileSystem;

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
        DEBUG ((
          DEBUG_ERROR,
          "FdtPlatform: Failed to locate gEfiLoadedImageProtocolGuid. Status=%r\n",
          Status
          ));
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
      DEBUG ((
        DEBUG_ERROR,
        "FdtPlatform: Failed to get gEfiLoadedImageProtocolGuid. Status=%r\n",
        Status
        ));
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
        DEBUG ((
          DEBUG_ERROR,
          "FdtPlatform: Failed to get gEfiSimpleFileSystemProtocolGuid. Status=%r\n",
          Status
          ));
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
  IN EFI_EVENT  Event,
  IN VOID       *Context
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
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS  Status;
  VOID        *Dtb;

  Status = EfiGetSystemConfigurationTable (&gFdtTableGuid, &Dtb);
  if (EFI_ERROR (Status) || (Dtb == NULL)) {
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
  IN         VOID     *Fdt,
  IN CONST   CHAR8    *NodePath,
  IN         BOOLEAN  Enable
  )
{
  INT32  Node;
  INT32  Ret;
  CHAR8  *NodeStatus;

  Node = fdt_path_offset (Fdt, NodePath);
  if (Node < 0) {
    DEBUG ((
      DEBUG_ERROR,
      "FdtPlatform: Couldn't locate FDT node path '%a'. Ret=%a\n",
      NodePath,
      fdt_strerror (Node)
      ));
    return EFI_NOT_FOUND;
  }

  NodeStatus = Enable ? "okay" : "disabled";
  Ret        = fdt_setprop_string (Fdt, Node, "status", NodeStatus);
  if (Ret) {
    DEBUG ((
      DEBUG_ERROR,
      "FdtPlatform: Failed to set '%a' status to '%a'. Ret=%a\n",
      NodePath,
      NodeStatus,
      fdt_strerror (Ret)
      ));
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

STATIC
VOID
EFIAPI
FdtFixupComboPhyDevices (
  IN VOID    *Fdt,
  IN UINT32  CompatMode
  )
{
  DEBUG ((DEBUG_INFO, "FdtPlatform: Fixing up Combo PHY devices (PCIe, SATA, USB)\n"));

  FdtEnableNode (
    Fdt,
    "/pcie@fe170000",
    PcdGet32 (PcdComboPhy1Mode) == COMBO_PHY_MODE_PCIE
    );
  FdtEnableNode (
    Fdt,
    "/pcie@fe180000",
    PcdGet32 (PcdComboPhy2Mode) == COMBO_PHY_MODE_PCIE
    );
  FdtEnableNode (
    Fdt,
    "/pcie@fe190000",
    PcdGet32 (PcdComboPhy0Mode) == COMBO_PHY_MODE_PCIE
    );

  FdtEnableNode (
    Fdt,
    "/sata@fe210000",
    PcdGet32 (PcdComboPhy0Mode) == COMBO_PHY_MODE_SATA
    );
  FdtEnableNode (
    Fdt,
    "/sata@fe220000",
    PcdGet32 (PcdComboPhy1Mode) == COMBO_PHY_MODE_SATA
    );
  FdtEnableNode (
    Fdt,
    "/sata@fe230000",
    PcdGet32 (PcdComboPhy2Mode) == COMBO_PHY_MODE_SATA
    );

  switch (CompatMode) {
    case FDT_COMPAT_MODE_VENDOR:
      FdtEnableNode (
        Fdt,
        "/usbhost3_0",
        PcdGet32 (PcdComboPhy2Mode) == COMBO_PHY_MODE_USB3
        );
      FdtEnableNode (
        Fdt,
        "/usbhost3_0/usb@fcd00000",
        PcdGet32 (PcdComboPhy2Mode) == COMBO_PHY_MODE_USB3
        );
      break;
    case FDT_COMPAT_MODE_MAINLINE:
      FdtEnableNode (
        Fdt,
        "/usb@fcd00000",
        PcdGet32 (PcdComboPhy2Mode) == COMBO_PHY_MODE_USB3
        );
      break;
  }
}

STATIC
VOID
EFIAPI
FdtFixupPcie3Devices (
  IN VOID    *Fdt,
  IN UINT32  CompatMode
  )
{
  if (!FixedPcdGetBool (PcdPcie30Supported)) {
    return;
  }

  DEBUG ((DEBUG_INFO, "FdtPlatform: Fixing up PCIe 3 devices\n"));

  FdtEnableNode (
    Fdt,
    "/pcie@fe150000",
    PcdGet32 (PcdPcie30State) == PCIE30_STATE_ENABLED
    );

  FdtEnableNode (
    Fdt,
    "/pcie@fe160000",
    PcdGet32 (PcdPcie30State) == PCIE30_STATE_ENABLED &&
    FixedPcdGetBool (PcdPcie30x2Supported) &&
    PcdGet8 (PcdPcie30PhyMode) != PCIE30_PHY_MODE_AGGREGATION
    );
}

STATIC
VOID
EFIAPI
FdtFixupVopDevices (
  IN VOID    *Fdt,
  IN UINT32  CompatMode
  )
{
  //
  // This fixup allows Linux to reuse the UEFI GOP framebuffer in cases
  // where the kernel lacks native support for display output.
  //
  // It works by:
  // 1) disabling VOP device nodes to prevent any existing drivers from
  //    resetting the hardware.
  // 2) disabling related power domain nodes to prevent the kernel from
  //    turning them off for being otherwise unused.
  // 3) creating fake "regulator-fixed-clock" nodes whose sole purpose is
  //    to reference and keep the required clocks enabled - again, to
  //    prevent the kernel from gating them.
  //

  STATIC CHAR8  *VopNodesToDisable[] = {
    "/vop@fdd90000",
    "/iommu@fdd97e00",
    "/hdmi@fde80000",
    "/hdmi@fdea0000",
    "/phy@fed60000",
    "/phy@fed70000",
    "/dp@fde50000",
    "/dp@fde60000",
    "/edp@fdec0000",
    "/edp@fded0000",
    "/dsi@fde20000",
    "/dsi@fde30000",
    "/display-subsystem",
    "/power-management@fd8d8000/power-controller/power-domain@"XS (RK3588_PD_VOP),
    "/power-management@fd8d8000/power-controller/power-domain@"XS (RK3588_PD_VO1),
  };

  STATIC UINT32  VopRequiredCruClocks[] = {
    ACLK_VOP,
    HCLK_VOP,
    DCLK_VOP0,
    DCLK_VOP1,
    DCLK_VOP2,
    DCLK_VOP3,
    PCLK_VOP_ROOT,
  };

  UINTN  Index;
  INT32  Root;
  INT32  Node;
  INT32  Ret;
  INT32  CruPhandle;

  if ((CompatMode != FDT_COMPAT_MODE_MAINLINE) || !PcdGet8 (PcdFdtForceGop)) {
    return;
  }

  DEBUG ((DEBUG_INFO, "FdtPlatform: Fixing up VOP devices (force GOP)\n"));

  for (Index = 0; Index < ARRAY_SIZE (VopNodesToDisable); Index++) {
    FdtEnableNode (Fdt, VopNodesToDisable[Index], FALSE);
  }

  Root = fdt_path_offset (Fdt, "/");
  ASSERT (Root >= 0);
  if (Root < 0) {
    DEBUG ((DEBUG_ERROR, "FdtPlatform: Couldn't locate FDT root. Ret=%a\n", fdt_strerror (Root)));
    return;
  }

  Node = fdt_path_offset (Fdt, "/clock-controller@fd7c0000");
  if (Node < 0) {
    DEBUG ((DEBUG_ERROR, "FdtPlatform: Couldn't locate CRU node. Ret=%a\n", fdt_strerror (Node)));
    return;
  }

  CruPhandle = fdt_get_phandle (Fdt, Node);
  if (CruPhandle <= 0) {
    DEBUG ((DEBUG_ERROR, "FdtPlatform: Failed to get CRU phandle.\n"));
    return;
  }

  for (Index = 0; Index < ARRAY_SIZE (VopRequiredCruClocks); Index++) {
    UINT32  ClockId = VopRequiredCruClocks[Index];

    CHAR8  NodeName[34];
    AsciiSPrint (NodeName, sizeof (NodeName), "clk-cru-%d-keep-alive-reg", ClockId);

    Node = fdt_add_subnode (Fdt, Root, NodeName);
    if (Node < 0) {
      DEBUG ((
        DEBUG_ERROR,
        "FdtPlatform: Couldn't create FDT node '%a'. Ret=%a\n",
        NodeName,
        fdt_strerror (Node)
        ));
      return;
    }

    Ret = fdt_setprop_string (Fdt, Node, "compatible", "regulator-fixed-clock");
    if (Ret < 0) {
      DEBUG ((
        DEBUG_ERROR,
        "FdtPlatform: Failed to set 'compatible' property for '%a'. Ret=%a\n",
        NodeName,
        fdt_strerror (Ret)
        ));
      return;
    }

    Ret = fdt_setprop_string (Fdt, Node, "regulator-name", NodeName);
    if (Ret < 0) {
      DEBUG ((
        DEBUG_ERROR,
        "FdtPlatform: Failed to set 'regulator-name' property for '%a'. Ret=%a\n",
        NodeName,
        fdt_strerror (Ret)
        ));
      return;
    }

    Ret = fdt_setprop_empty (Fdt, Node, "regulator-always-on");
    if (Ret < 0) {
      DEBUG ((
        DEBUG_ERROR,
        "FdtPlatform: Failed to set 'regulator-always-on' property for '%a'. Ret=%a\n",
        NodeName,
        fdt_strerror (Ret)
        ));
      return;
    }

    UINT32  ClockPair[] = { cpu_to_fdt32 (CruPhandle), cpu_to_fdt32 (ClockId) };
    Ret = fdt_setprop (Fdt, Node, "clocks", ClockPair, sizeof (ClockPair));
    if (Ret < 0) {
      DEBUG ((
        DEBUG_ERROR,
        "FdtPlatform: Failed to set 'clocks' property for '%a'. Ret=%a\n",
        NodeName,
        fdt_strerror (Ret)
        ));
      return;
    }
  }
}

STATIC
VOID
EFIAPI
ApplyPlatformFdtFixups (
  IN VOID    *Fdt,
  IN UINT32  CompatMode
  )
{
  FdtFixupComboPhyDevices (Fdt, CompatMode);
  FdtFixupPcie3Devices (Fdt, CompatMode);
  FdtFixupVopDevices (Fdt, CompatMode);
}

STATIC
EFI_STATUS
EFIAPI
LoadPlatformFdt (
  OUT   VOID    **PlatformFdt,
  IN    UINT32  CompatMode
  )
{
  EFI_STATUS      Status;
  CONST EFI_GUID  *DtbFileGuid;
  VOID            *Fdt;
  UINTN           FdtSize;
  INT32           Ret;

  switch (CompatMode) {
    case FDT_COMPAT_MODE_VENDOR:
      DEBUG ((DEBUG_INFO, "FdtPlatform: Using vendor FDT.\n"));
      break;
    case FDT_COMPAT_MODE_MAINLINE:
      DEBUG ((DEBUG_INFO, "FdtPlatform: Using mainline FDT.\n"));
      break;
    case FDT_COMPAT_MODE_UNSUPPORTED:
      DEBUG ((DEBUG_WARN, "FdtPlatform: No firmware FDT found!\n"));
      return EFI_NOT_FOUND;
    default:
      DEBUG ((DEBUG_ERROR, "FdtPlatform: Invalid compat mode: %u\n", CompatMode));
      ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
  }

  DtbFileGuid = PlatformGetDtbFileGuid (CompatMode);
  if (DtbFileGuid == NULL) {
    DEBUG ((DEBUG_ERROR, "FdtPlatform: Compat mode unsupported by platform!\n"));
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  Status = GetSectionFromAnyFv (
             DtbFileGuid,
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
    DEBUG ((
      DEBUG_ERROR,
      "FdtPlatform: Firmware FDT has an invalid header! Ret=%a\n",
      fdt_strerror (Ret)
      ));
    return EFI_NOT_FOUND;
  }

  // Expand the FDT a bit to give room for any additions.
  Status = FdtOpenIntoAlloc (&Fdt, NULL, fdt_totalsize (Fdt) + SIZE_4KB);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *PlatformFdt = Fdt;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
FdtPlatformDxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   Event;
  UINT32      CompatMode;

  if ((PcdGet32 (PcdConfigTableMode) & CONFIG_TABLE_MODE_FDT) == 0) {
    DEBUG ((DEBUG_WARN, "FdtPlatform: FDT support is disabled by the settings.\n"));
    return EFI_UNSUPPORTED;
  }

  if (PcdGet8 (PcdFdtSupportOverrides)) {
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,                // Type
                    TPL_CALLBACK,                     // NotifyTpl
                    NotifyReadyToBoot,                // NotifyFunction
                    NULL,                             // NotifyContext
                    &gEfiEventReadyToBootGuid,        // EventGroup
                    &Event                            // Event
                    );
    ASSERT_EFI_ERROR (Status);
  }

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,                  // Type
                  TPL_CALLBACK,                       // NotifyTpl
                  NotifyExitBootServices,             // NotifyFunction
                  NULL,                               // NotifyContext
                  &gEfiEventExitBootServicesGuid,     // EventGroup
                  &Event                              // Event
                  );
  ASSERT_EFI_ERROR (Status);

  CompatMode = PcdGet32 (PcdFdtCompatMode);

  Status = LoadPlatformFdt (&mPlatformFdt, CompatMode);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "FdtPlatform: Failed to load firmware FDT. Status=%r\n", Status));
    return EFI_SUCCESS;
  }

  ApplyPlatformFdtFixups (mPlatformFdt, CompatMode);

  Status = gBS->InstallConfigurationTable (&gFdtTableGuid, mPlatformFdt);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_WARN,
      "FdtPlatform: Failed to install firmware DTB as config table. Status=%r\n",
      Status
      ));
  }

  return EFI_SUCCESS;
}
