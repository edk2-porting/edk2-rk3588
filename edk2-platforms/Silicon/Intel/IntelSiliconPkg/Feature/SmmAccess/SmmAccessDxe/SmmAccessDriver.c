/** @file
  This is the driver that publishes the SMM Access Protocol
  instance for System Agent.

  Copyright (c) 2019 - 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "SmmAccessDriver.h"

static SMM_ACCESS_PRIVATE_DATA  mSmmAccess;

/**
  <b>SMM Access Driver Entry Point</b>
  This driver installs an SMM Access Protocol
  - <b>Introduction</b> \n
    This module publishes the SMM access protocol.  The protocol is used by the SMM Base driver to access the SMRAM region when the processor is not in SMM.
    The SMM Base driver uses the services provided by the SMM access protocol to open SMRAM during POST and copy the SMM handler.
    SMM access protocol is also used to close the SMRAM region once the copying is done.
    Finally, the SMM access protocol provides services to "Lock" the SMRAM region.
    Please refer the SMM Protocols section in the attached SMM CIS Specification version 0.9 for further details.
    This driver is required if SMM is supported. Proper configuration of SMM registers is recommended even if SMM is not supported.

  - <b>Porting Recommendations</b> \n
    No modification of this module is recommended.  Any modification should be done in compliance with the _EFI_SMM_ACCESS_PROTOCOL protocol definition.

  @param[in] ImageHandle        - Handle for the image of this driver
  @param[in] SystemTable        - Pointer to the EFI System Table

  @retval EFI_SUCCESS           - Protocol was installed successfully
  @retval EFI_UNSUPPORTED       - Protocol was not installed
  @retval EFI_NOT_FOUND         - Protocol can't be found.
  @retval EFI_OUT_OF_RESOURCES  - Protocol does not have enough resources to initialize the driver.
**/
EFI_STATUS
EFIAPI
SmmAccessDriverEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                      Status;
  UINTN                           Index;
  EFI_SMRAM_HOB_DESCRIPTOR_BLOCK  *DescriptorBlock;
  EFI_PEI_HOB_POINTERS            *Hob;

  //
  // Initialize Global variables
  //
  ZeroMem (&mSmmAccess, sizeof (mSmmAccess));

  mSmmAccess.Signature        = SMM_ACCESS_PRIVATE_DATA_SIGNATURE;
  mSmmAccess.Handle           = NULL;

  //
  // Get Hob list
  //
  Hob = GetFirstGuidHob (&gEfiSmmSmramMemoryGuid);
  if (Hob == NULL) {
    DEBUG ((DEBUG_WARN, "SmramMemoryReserve HOB not found\n"));
    return EFI_NOT_FOUND;
  }

  DescriptorBlock = (VOID *) ((UINT8 *) Hob + sizeof (EFI_HOB_GUID_TYPE));

  //
  // Alloc space for mSmmAccess.SmramDesc
  //
  mSmmAccess.SmramDesc = AllocateZeroPool ((DescriptorBlock->NumberOfSmmReservedRegions) * sizeof (EFI_SMRAM_DESCRIPTOR));
  if (mSmmAccess.SmramDesc == NULL) {
    DEBUG ((DEBUG_WARN, "Alloc mSmmAccess.SmramDesc fail.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, "Alloc mSmmAccess.SmramDesc success.\n"));

  //
  // Use the HOB to publish SMRAM capabilities
  //
  for (Index = 0; Index < DescriptorBlock->NumberOfSmmReservedRegions; Index++) {
    mSmmAccess.SmramDesc[Index].PhysicalStart = DescriptorBlock->Descriptor[Index].PhysicalStart;
    mSmmAccess.SmramDesc[Index].CpuStart      = DescriptorBlock->Descriptor[Index].CpuStart;
    mSmmAccess.SmramDesc[Index].PhysicalSize  = DescriptorBlock->Descriptor[Index].PhysicalSize;
    mSmmAccess.SmramDesc[Index].RegionState   = DescriptorBlock->Descriptor[Index].RegionState;
  }

  mSmmAccess.NumberRegions              = Index;
  mSmmAccess.SmmAccess.Open             = Open;
  mSmmAccess.SmmAccess.Close            = Close;
  mSmmAccess.SmmAccess.Lock             = Lock;
  mSmmAccess.SmmAccess.GetCapabilities  = GetCapabilities;
  mSmmAccess.SmmAccess.LockState        = FALSE;
  mSmmAccess.SmmAccess.OpenState        = FALSE;

  //
  // Install our protocol interfaces on the device's handle
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mSmmAccess.Handle,
                  &gEfiSmmAccess2ProtocolGuid,
                  &mSmmAccess.SmmAccess,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "InstallMultipleProtocolInterfaces returned %r\n", Status));
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  This routine accepts a request to "open" a region of SMRAM.  The
  region could be legacy ABSEG, HSEG, or TSEG near top of physical memory.
  The use of "open" means that the memory is visible from all boot-service
  and SMM agents.

  @param[in] This               - Pointer to the SMM Access Interface.

  @retval EFI_SUCCESS           - The region was successfully opened.
  @retval EFI_DEVICE_ERROR      - The region could not be opened because locked by chipset.
  @retval EFI_INVALID_PARAMETER - The descriptor index was out of bounds.
**/
EFI_STATUS
EFIAPI
Open (
  IN EFI_SMM_ACCESS2_PROTOCOL *This
  )
{
  SMM_ACCESS_PRIVATE_DATA *SmmAccess;
  UINTN                   DescriptorIndex;

  SmmAccess = SMM_ACCESS_PRIVATE_DATA_FROM_THIS (This);
  for (DescriptorIndex = 0; DescriptorIndex < SmmAccess->NumberRegions; DescriptorIndex++) {
    if (SmmAccess->SmramDesc[DescriptorIndex].RegionState & EFI_SMRAM_LOCKED) {
      DEBUG ((DEBUG_WARN, "Cannot open a locked SMRAM region\n"));
      return EFI_DEVICE_ERROR;
    }
  }

  for (DescriptorIndex = 0; DescriptorIndex < SmmAccess->NumberRegions; DescriptorIndex++) {
    SmmAccess->SmramDesc[DescriptorIndex].RegionState &= (UINT64) ~(EFI_SMRAM_CLOSED | EFI_ALLOCATED);
    SmmAccess->SmramDesc[DescriptorIndex].RegionState |= (UINT64) EFI_SMRAM_OPEN;
  }
  SmmAccess->SmmAccess.OpenState = TRUE;
  return EFI_SUCCESS;
}

/**
  This routine accepts a request to "close" a region of SMRAM.  The
  region could be legacy AB or TSEG near top of physical memory.
  The use of "close" means that the memory is only visible from SMM agents,
  not from BS or RT code.

  @param[in] This               - Pointer to the SMM Access Interface.

  @retval EFI_SUCCESS           - The region was successfully closed.
  @retval EFI_DEVICE_ERROR      - The region could not be closed because locked by chipset.
  @retval EFI_INVALID_PARAMETER - The descriptor index was out of bounds.
**/
EFI_STATUS
EFIAPI
Close (
  IN EFI_SMM_ACCESS2_PROTOCOL  *This
  )
{
  SMM_ACCESS_PRIVATE_DATA *SmmAccess;
  BOOLEAN                 OpenState;
  UINT8                   Index;
  UINTN                   DescriptorIndex;

  SmmAccess = SMM_ACCESS_PRIVATE_DATA_FROM_THIS (This);

  for (DescriptorIndex = 0; DescriptorIndex < SmmAccess->NumberRegions; DescriptorIndex++) {
    if (SmmAccess->SmramDesc[DescriptorIndex].RegionState & EFI_SMRAM_LOCKED) {
      DEBUG ((DEBUG_WARN, "Cannot close a locked SMRAM region\n"));
      continue;
    }

    SmmAccess->SmramDesc[DescriptorIndex].RegionState &= (UINT64) ~EFI_SMRAM_OPEN;
    SmmAccess->SmramDesc[DescriptorIndex].RegionState |= (UINT64) (EFI_SMRAM_CLOSED | EFI_ALLOCATED);
  }

  //
  // Find out if any regions are still open
  //
  OpenState = FALSE;
  for (Index = 0; Index < mSmmAccess.NumberRegions; Index++) {
    if ((SmmAccess->SmramDesc[Index].RegionState & EFI_SMRAM_OPEN) == EFI_SMRAM_OPEN) {
      OpenState = TRUE;
    }
  }

  SmmAccess->SmmAccess.OpenState = OpenState;
  return EFI_SUCCESS;
}

/**
  This routine accepts a request to "lock" SMRAM.  The
  region could be legacy AB or TSEG near top of physical memory.
  The use of "lock" means that the memory can no longer be opened
  to BS state..

  @param[in] This               - Pointer to the SMM Access Interface.

  @retval EFI_SUCCESS           - The region was successfully locked.
  @retval EFI_DEVICE_ERROR      - The region could not be locked because at least
                                  one range is still open.
  @retval EFI_INVALID_PARAMETER - The descriptor index was out of bounds.
**/
EFI_STATUS
EFIAPI
Lock (
  IN EFI_SMM_ACCESS2_PROTOCOL *This
  )
{
  SMM_ACCESS_PRIVATE_DATA *SmmAccess;
  UINTN                   DescriptorIndex;

  SmmAccess = SMM_ACCESS_PRIVATE_DATA_FROM_THIS (This);

  if (SmmAccess->SmmAccess.OpenState) {
    DEBUG ((DEBUG_WARN, "Cannot lock SMRAM when SMRAM regions are still open\n"));
    return EFI_DEVICE_ERROR;
  }
  for (DescriptorIndex = 0; DescriptorIndex < SmmAccess->NumberRegions; DescriptorIndex++) {
    SmmAccess->SmramDesc[DescriptorIndex].RegionState |= EFI_SMRAM_LOCKED;
  }
  SmmAccess->SmmAccess.LockState = TRUE;

  return EFI_SUCCESS;
}

/**
  This routine services a user request to discover the SMRAM
  capabilities of this platform.  This will report the possible
  ranges that are possible for SMRAM access, based upon the
  memory controller capabilities.

  @param[in] This                  - Pointer to the SMRAM Access Interface.
  @param[in, out] SmramMapSize     - Pointer to the variable containing size of the
                                     buffer to contain the description information.
  @param[in, out] SmramMap         - Buffer containing the data describing the Smram
                                     region descriptors.

  @retval EFI_BUFFER_TOO_SMALL     - The user did not provide a sufficient buffer.
  @retval EFI_SUCCESS              - The user provided a sufficiently-sized buffer.
**/
EFI_STATUS
EFIAPI
GetCapabilities (
  IN CONST EFI_SMM_ACCESS2_PROTOCOL  *This,
  IN OUT UINTN                       *SmramMapSize,
  IN OUT EFI_SMRAM_DESCRIPTOR        *SmramMap
  )
{
  EFI_STATUS              Status;
  SMM_ACCESS_PRIVATE_DATA *SmmAccess;
  UINTN                   NecessaryBufferSize;

  SmmAccess           = SMM_ACCESS_PRIVATE_DATA_FROM_THIS (This);

  NecessaryBufferSize = SmmAccess->NumberRegions * sizeof (EFI_SMRAM_DESCRIPTOR);

  if (*SmramMapSize < NecessaryBufferSize) {
    DEBUG ((DEBUG_WARN, "SMRAM Map Buffer too small\n"));
    Status = EFI_BUFFER_TOO_SMALL;
  } else {
    CopyMem (SmramMap, SmmAccess->SmramDesc, NecessaryBufferSize);
    Status = EFI_SUCCESS;
  }

  *SmramMapSize = NecessaryBufferSize;

  return Status;
}
