/** @file
*
*  Copyright (c) 2017, Linaro, Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <PiDxe.h>

#include <libfdt.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/OpteeLib.h>
#include <Platform/VarStore.h>

STATIC
VOID
EnableDtNode (
  IN  VOID                        *Dtb,
  IN  CONST CHAR8                 *NodePath
  )
{
  INT32                           Node;
  INT32                           Rc;

  Node = fdt_path_offset (Dtb, NodePath);
  if (Node < 0) {
    DEBUG ((DEBUG_ERROR, "%a: failed to locate DT path '%a': %a\n",
      __FUNCTION__, NodePath, fdt_strerror (Node)));
    return;
  }
  Rc = fdt_setprop_string (Dtb, Node, "status", "okay");
  if (Rc < 0) {
    DEBUG ((DEBUG_ERROR, "%a: failed to set status to 'disabled' on '%a': %a\n",
      __FUNCTION__, NodePath, fdt_strerror (Rc)));
  }
}

/**
  Return a pool allocated copy of the DTB image that is appropriate for
  booting the current platform via DT.

  @param[out]   Dtb                   Pointer to the DTB copy
  @param[out]   DtbSize               Size of the DTB copy

  @retval       EFI_SUCCESS           Operation completed successfully
  @retval       EFI_NOT_FOUND         No suitable DTB image could be located
  @retval       EFI_OUT_OF_RESOURCES  No pool memory available

**/
EFI_STATUS
EFIAPI
DtPlatformLoadDtb (
  OUT   VOID        **Dtb,
  OUT   UINTN       *DtbSize
  )
{
  EFI_STATUS                        Status;
  VOID                              *OrigDtb;
  VOID                              *CopyDtb;
  UINTN                             OrigDtbSize;
  UINTN                             CopyDtbSize;
  INT32                             Rc;
  UINT64                            SettingsVal;
  SYNQUACER_PLATFORM_VARSTORE_DATA  *Settings;

  Status = GetSectionFromAnyFv (&gDtPlatformDefaultDtbFileGuid,
             EFI_SECTION_RAW, 0, &OrigDtb, &OrigDtbSize);
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  CopyDtbSize = OrigDtbSize;
  CopyDtb = AllocatePool (CopyDtbSize);
  if (CopyDtb == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Rc = fdt_open_into (OrigDtb, CopyDtb, CopyDtbSize);
  if (Rc < 0) {
    DEBUG ((DEBUG_ERROR, "%a: fdt_open_into () failed: %a\n", __FUNCTION__,
      fdt_strerror (Rc)));
    return EFI_NOT_FOUND;
  }

  if (PcdGet8 (PcdPcieEnableMask) & BIT0) {
    EnableDtNode (CopyDtb, "/pcie@60000000");
  }
  if (PcdGet8 (PcdPcieEnableMask) & BIT1) {
    EnableDtNode (CopyDtb, "/pcie@70000000");
  }

  SettingsVal = PcdGet64 (PcdPlatformSettings);
  Settings = (SYNQUACER_PLATFORM_VARSTORE_DATA *)&SettingsVal;
  if (Settings->EnableEmmc == EMMC_ENABLED) {
    EnableDtNode (CopyDtb, "/sdhci@52300000");
  }

  if (IsOpteePresent()) {
    EnableDtNode (CopyDtb, "/firmware/optee");
  }

  *Dtb = CopyDtb;
  *DtbSize = CopyDtbSize;

  return EFI_SUCCESS;
}
