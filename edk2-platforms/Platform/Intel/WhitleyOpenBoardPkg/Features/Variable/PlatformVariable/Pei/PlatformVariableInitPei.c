/** @file
  Platform variable initialization PEIM.

  This PEIM determines whether to load variable defaults. Ordinarily, the
  decision is based on the boot mode, but an OEM hook is provided to override
  that. The appropriate HOBs and PCDs are created to signal DXE code to update
  the variable default values.

  @copyright
  Copyright 2012 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PlatformVariableInitPei.h"
#include <Guid/PlatformVariableCommon.h>
#include <Uefi/UefiInternalFormRepresentation.h>

UINT16 BoardId = BOARD_ID_DEFAULT;

EFI_PEI_PPI_DESCRIPTOR  mPpiListPlatformVariableInit = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPlatformVariableInitPpiGuid,
  NULL
};

/**
Apply platform variable defaults.

Create HOBs and set PCDs to prompt the (re-)loading of variable defaults.
Each step is attempted regardless of whether the previous steps succeeded.
If multiple errors occur, only the last error code is returned.

@param[in]  Events            Bitmap of events that occurred.
@param[in]  DefaultId         Default store ID, STANDARD or MANUFACTURING.

@retval EFI_SUCCESS           All steps completed successfully.
@retval EFI_OUT_OF_RESOURCES  One of the HOBs could not be created.
@retval EFI_NOT_FOUND         The default data could not be found in FFS.
**/

EFI_STATUS
ApplyPlatformVariableDefaults(
  IN UINT8  Events,
  IN UINT16 DefaultId
  )
{
  VOID        *Hob;
  EFI_STATUS  Status;
  EFI_STATUS  ReturnStatus;

  DEBUG((DEBUG_INFO, "Applying platform variable defaults:\n"));
  DEBUG((DEBUG_INFO, "  Events    = 0x%02x\n", Events));
  DEBUG((DEBUG_INFO, "  DefaultId = 0x%04x\n", DefaultId));

  //
  // Assume success up front. This will be overwritten if errors occur.
  //
  ReturnStatus = EFI_SUCCESS;

  //
  // Send the bitmap of events to the platform variable DXE driver.
  //
  Hob = BuildGuidDataHob(&gPlatformVariableHobGuid, &Events, sizeof(Events));
  if (Hob == NULL) {
    DEBUG((DEBUG_ERROR, "Create platform var event HOB: %r!\n", EFI_OUT_OF_RESOURCES));
    ReturnStatus = EFI_OUT_OF_RESOURCES;
  }

  //
  // Locate variable default data in FFS and send it to the core variable DXE
  // driver to write.
  //
  Status = CreateDefaultVariableHob(DefaultId, BoardId);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "create default var HOB: %r!\n", Status));
    ReturnStatus = Status;
  }

  //
  // Set the PCD SKU ID.
  //
  LibPcdSetSku(BoardId);

  //
  // Set the PCD default store ID.
  //
  Status = PcdSet16S(PcdSetNvStoreDefaultId, DefaultId);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "setNVstore default ID PCD: %r!\n", Status));
    ReturnStatus = Status;
  }

  return ReturnStatus;
}

/**
Perform the default variable initializations after variable service is ready.

@param[in]  PeiServices       General purpose services available to every PEIM.
@param[in]  NotifyDescriptor  Pointer to Notify PPI descriptor.
@param[in]  Interface         Pointer to PPI.

@retval EFI_SUCCESS   Default setting is initialized into variable.
@retval Other values  Can't find the matched default setting.
**/
EFI_STATUS
EFIAPI
PlatformVariablePeiInit(
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Interface
)
{
  EFI_STATUS                      Status;
  UINT8                           *SystemConfiguration;
  EFI_GUID                        *SystemConfigurationGuid;
  UINTN                           DataSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  UINT8                           Events;
  UINT16                          DefaultId;
  BOOLEAN                         ApplyDefaults;

  SystemConfigurationGuid = PcdGetPtr(PcdSetupVariableGuid);
  Events = 0;
  DefaultId = EFI_HII_DEFAULT_CLASS_STANDARD;

  if (PlatformVariableHookForHobGeneration(Interface, &Events, &DefaultId)) {
    //
    // Use events bitmap and default ID returned by PlatformVariableHook.
    //
    ApplyDefaults = TRUE;
  }
  else {
    //
    // If the setup variable does not exist (yet), defaults should be applied.
    //
    VariableServices = (EFI_PEI_READ_ONLY_VARIABLE2_PPI *)Interface;
    SystemConfiguration = NULL;
    DataSize = 0;
    Status = VariableServices->GetVariable(
      VariableServices,
      PLATFORM_SETUP_VARIABLE_NAME,
      SystemConfigurationGuid,
      NULL,
      &DataSize,
      SystemConfiguration
    );
    //
    // Setup variable is not found. So, set the default setting.
    //
    if (Status == EFI_NOT_FOUND) {
      Events = NULL_VARIABLE_EVENT;
      DefaultId = EFI_HII_DEFAULT_CLASS_STANDARD;
      ApplyDefaults = TRUE;
    }
    else {
      ApplyDefaults = FALSE;
    }
  }


  if (ApplyDefaults) {
    Status = ApplyPlatformVariableDefaults(Events, DefaultId);
  }
  else {
    //
    // Normal case boot flow
    //
    Events = 0; // no events occurred
    BuildGuidDataHob (&gPlatformVariableHobGuid, &Events, sizeof (UINT8));

    //
    // Patch RP variable value with PC variable in the begining of PEI
    //
    Status = CreateRPVariableHob (EFI_HII_DEFAULT_CLASS_STANDARD, BoardId);
  }

  PeiServicesInstallPpi (&mPpiListPlatformVariableInit);
  return Status;
}


/**
Variable Init BootMode CallBack
Prepare Knob values based on boot mode
Execute after discovering BootMode

@param[in]  PeiServices       General purpose services available to every PEIM.
@param[in]  NotifyDescriptor  Pointer to Notify PPI descriptor.
@param[in]  Interface         Pointer to PPI.

@retval EFI_SUCCESS   Knob Values.
@retval Other values
**/
EFI_STATUS
EFIAPI
VariableInitBootModeCallBack(
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Interface
) {
  EFI_BOOT_MODE                   BootMode;
  BOOLEAN                         ApplyDefaults;
  UINT8                           Events;
  UINT16                          DefaultId;
  EFI_STATUS                      Status;

  Events = 0;
  DefaultId = EFI_HII_DEFAULT_CLASS_STANDARD;
  ApplyDefaults = FALSE;

  //
  // Certain boot modes require defaults to be (re-)applied.
  //
  Status = PeiServicesGetBootMode(&BootMode);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    BootMode = BOOT_WITH_DEFAULT_SETTINGS;
  }
  if (BootMode == BOOT_WITH_MFG_MODE_SETTINGS) {
    Events = MFG_MODE_EVENT;
    DefaultId = EFI_HII_DEFAULT_CLASS_MANUFACTURING;
    ApplyDefaults = TRUE;
  }
  else if (BootMode == BOOT_IN_RECOVERY_MODE) {
    Events = RECOVERY_MODE_EVENT;
    DefaultId = EFI_HII_DEFAULT_CLASS_STANDARD;
    ApplyDefaults = TRUE;
  }
  else if (BootMode == BOOT_WITH_DEFAULT_SETTINGS) {
    Events = CMOS_CLEAR_EVENT;
    DefaultId = EFI_HII_DEFAULT_CLASS_STANDARD;
    ApplyDefaults = TRUE;
  }
  if (ApplyDefaults) {
    Status = ApplyPlatformVariableDefaults(Events, DefaultId);
  }
  return Status;
}

EFI_PEI_NOTIFY_DESCRIPTOR mVariableNotifyList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK),
    &gEfiPeiReadOnlyVariable2PpiGuid,
    PlatformVariablePeiInit
  },
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gUpdateBootModePpiGuid,
    VariableInitBootModeCallBack
  }
};

EFI_STATUS
EFIAPI
PlatformVariableInitPeiEntry (
  IN EFI_PEI_FILE_HANDLE            FileHandle,
  IN CONST EFI_PEI_SERVICES          **PeiServices
  )
/*++

--*/
{
  EFI_STATUS                    Status;

  PlatformVariableHookForEntry();

  // Register notify to set default variable once variable service is ready.
  //
  Status = PeiServicesNotifyPpi(&mVariableNotifyList[0]);

  return Status;
}
