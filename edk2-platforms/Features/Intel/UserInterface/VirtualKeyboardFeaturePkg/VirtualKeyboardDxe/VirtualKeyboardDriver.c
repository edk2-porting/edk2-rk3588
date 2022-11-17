/** @file
  Virtual Keyboard driver.

  Copyright (c) 2012 - 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "VirtualKeyboard.h"

UINT32         mOrigConOutRow      = 0;
UINT32         mOrigSetupConOutRow = 0;
EFI_HII_HANDLE mHiiHandle          = NULL;
/**
  Verify the controller type

  This routine determines if the pointer and GOP are available.

  This routine is called by the UEFI driver framework during connect
  processing.

  @param[in] This                 Protocol instance pointer.
  @param[in] Controller           Handle of device to test.
  @param[in] RemainingDevicePath  Not used.

  @retval EFI_SUCCESS             This driver supports this device.
  @retval other                   This driver does not support this device.

**/
EFI_STATUS
EFIAPI
VirtualKeyboardDriverSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN EFI_HANDLE                  Controller,
  IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
  )
{
  EFI_GRAPHICS_OUTPUT_PROTOCOL  *GraphicsOutput;
  EFI_STATUS                    Status;

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VirtualKeyboardDriverSupported Start\n"));

  //
  // Verify that the driver is not already started
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiCallerIdGuid,
                  NULL,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                  );
  if (!EFI_ERROR (Status)) {
    Status = EFI_ALREADY_STARTED;
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "ERROR - VK has already initialized\n"));
    goto Error;
  }

  //
  // Determine if the pointer protocol is available.
  // This should be installed in touch driver.
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiAbsolutePointerProtocolGuid,
                  NULL,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "ERROR - VK Absolute pointer protocol not found\n"));
    goto Error;
  }

  Status = gBS->OpenProtocol (
                  Controller,
                  &gEdkiiTouchPanelGuid,
                  NULL,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "ERROR - VK Touch Panel Guid not found\n"));
    goto Error;
  }

  //
  // Determine if the graphics output protocol is available
  //
  Status = gBS->HandleProtocol (
                  gST->ConsoleOutHandle,
                  &gEfiGraphicsOutputProtocolGuid,
                  (VOID **)&GraphicsOutput
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "ERROR - VK Graphics output protocol not found\n"));
    goto Error;
  }

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_INFO, "VirtualKeyboardDriverSupported Success, Status: %r\n", Status));
  goto End;

Error:
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VirtualKeyboardDriverSupported Failed, Status: %r\n", Status));

End:
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VirtualKeyboardDriverSupported End\n"));
  return Status;
}

/**
  Connect to the controller

  This routine initializes an instance of the virutal keyboard driver for this
  controller.

  This routine is called by the UEFI driver framework during connect
  processing if the controller passes the tests in I2cBusDriverSupported.

  @param[in] This                 Protocol instance pointer.
  @param[in] Controller           Handle of device to work with.
  @param[in] RemainingDevicePath  Not used, always produce all possible children.

  @retval EFI_SUCCESS             This driver is added to Controller.
  @retval other                   This driver does not support this device.

**/
EFI_STATUS
EFIAPI
VirtualKeyboardDriverStart (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN EFI_HANDLE                  Controller,
  IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
  )
{
  VK_CONTEXT *VkContext;
  EFI_STATUS Status;

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_INFO, "VirtualKeyboardDriverStart Start\n"));

  VkContext = AllocateZeroPool (sizeof (VK_CONTEXT));
  if (VkContext == NULL) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_ERROR, "ERROR - No memory for virtual keyboard driver\n"));
    Status = EFI_OUT_OF_RESOURCES;
    ASSERT_EFI_ERROR (Status);
    goto Error;
  }

  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiAbsolutePointerProtocolGuid,
                  (VOID**) &VkContext->AbsolutePointer,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_ERROR, "ERROR - Failed to open absolute pointer protocol, Status: %r\n", Status));
    goto Error;
  }

  Status = gBS->OpenProtocol (
                  Controller,
                  &gEdkiiTouchPanelGuid,
                  NULL,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_ERROR, "ERROR - TouchPanel GUID not found, Status: %r\n", Status));
    goto Error;
  }

  Status = gBS->HandleProtocol (
                  gST->ConsoleOutHandle,
                  &gEfiGraphicsOutputProtocolGuid,
                  (VOID**) &VkContext->GraphicsOutput
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_ERROR, "ERROR - Graphics output protocol not available, Status: %r\n", Status));
    goto Error;
  }

  VkContext->HiiHandle = mHiiHandle;
  Status = VkApiStart (VkContext, Controller);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_ERROR, "ERROR - Failed to VkApiStart, Status: %r\n", Status));
    goto Error;
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Controller,
                  &gEfiCallerIdGuid,
                  VkContext,
                  &gEfiSimpleTextInProtocolGuid,
                  &VkContext->SimpleTextIn,
                  &gEfiSimpleTextInputExProtocolGuid,
                  &VkContext->SimpleTextInEx,
                  &gEfiConsoleInDeviceGuid,
                  NULL,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_ERROR, "ERROR - Failed to install VK protocols, Status: %r\n", Status));
    ASSERT_EFI_ERROR (Status);
    goto Error;
  }

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_INFO, "VirtualKeyboardDriverStart Success, Status: %r\n", Status));
  goto End;

Error:
  if (VkContext != NULL) {
    FreePool (VkContext);
  }

  gBS->CloseProtocol (
         Controller,
         &gEfiAbsolutePointerProtocolGuid,
         This->DriverBindingHandle,
         Controller
         );

  //
  // Restore setting if connect device fail.
  //
  PcdSet32S (PcdConOutRow, mOrigConOutRow);
  PcdSet32S (PcdSetupConOutRow, mOrigSetupConOutRow);

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VirtualKeyboardDriverStart Failed, Status: %r\n", Status));

End:
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VirtualKeyboardDriverStart End\n"));

  return Status;
}

/**
  Disconnect from the controller.

  This routine disconnects from the controller.

  This routine is called by DriverUnload when the I2C bus driver
  is being unloaded.

  @param[in] This                 Protocol instance pointer.
  @param[in] Controller           Handle of device to stop driver on.
  @param[in] NumberOfChildren     How many children need to be stopped.
  @param[in] ChildHandleBuffer    Not used.

  @retval EFI_SUCCESS             This driver is removed Controller.
  @retval EFI_DEVICE_ERROR        The device could not be stopped due to a device error.
  @retval other                   This driver was not removed from this device.

**/
EFI_STATUS
EFIAPI
VirtualKeyboardDriverStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL *This,
  IN  EFI_HANDLE                  Controller,
  IN  UINTN                       NumberOfChildren,
  IN  EFI_HANDLE                  *ChildHandleBuffer
  )
{
  VK_CONTEXT                          *VkContext;
  EFI_STATUS                          Status;

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VirtualKeyboardDriverStop Start\n"));

  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiCallerIdGuid,
                  (VOID**)&VkContext,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER | EFI_OPEN_PROTOCOL_EXCLUSIVE
                  );
  if (EFI_ERROR (Status)) {
    Status = EFI_SUCCESS;
    goto Success;
  }

  //
  // Done with the driver protocol
  //
  Status = gBS->CloseProtocol (
                  Controller,
                  &gEfiCallerIdGuid,
                  This->DriverBindingHandle,
                  Controller
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "ERROR - Failed to close the VK protocol, Status: %r\n", Status));
    goto Error;
  }

  //
  // Remove ConsoleIn protocols first to close the link in ConSplitter
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiConsoleInDeviceGuid,
                  NULL,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "ERROR - Failed to uninstall the protocols, Status: %r\n", Status));
    goto Error;
  }

  //
  // Remove the remaining protocols
  //
  Status = gBS->UninstallMultipleProtocolInterfaces (
                  Controller,
                  &gEfiCallerIdGuid,
                  VkContext,
                  &gEfiSimpleTextInProtocolGuid,
                  &VkContext->SimpleTextIn,
                  &gEfiSimpleTextInputExProtocolGuid,
                  &VkContext->SimpleTextInEx,
                  &gEfiConsoleInDeviceGuid,
                  NULL,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "ERROR - Failed to uninstall the protocols, Status: %r\n", Status));
    goto Error;
  }

  //
  // Stop the driver
  //
  VkApiStop (VkContext);

  //
  // Release the pointer protocol upon failure
  //
  Status = gBS->CloseProtocol (
                  Controller,
                  &gEfiAbsolutePointerProtocolGuid,
                  This->DriverBindingHandle,
                  Controller
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "ERROR - Failed to close absolute pointer protocol, Status: %r\n", Status));
    goto Error;
  }

  //
  // Release VkContext
  //
  if (VkContext != NULL) {
    FreePool (VkContext);
  }

Success:
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VirtualKeyboardDriverStop Success, Status: %r\n", Status));
  goto End;

Error:
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VirtualKeyboardDriverStop Failed, Status: %r\n", Status));

End:
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VirtualKeyboardDriverStop End\n"));
  return Status;
}

/**
  Unloads an image.

  @param[in] ImageHandle        Handle that identifies the image to be unloaded.

  @retval EFI_SUCCESS           The image has been unloaded.
  @retval EFI_INVALID_PARAMETER ImageHandle is not a valid image handle.

**/
EFI_STATUS
EFIAPI
VirtualKeyboardDriverUnload (
  IN EFI_HANDLE ImageHandle
  )
{
  EFI_STATUS                 Status;
  EFI_HANDLE                 *HandleBuffer;
  UINTN                      HandleCount;
  UINTN                      Index;

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VirtualKeyboardDriverUnload Start\n"));

  //
  // Retrieve array of all handles in the handle database
  //
  Status = gBS->LocateHandleBuffer (
                  AllHandles,
                  NULL,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "ERROR - Failed to locate handle buffer, Status: %r\n", Status));
    ASSERT_EFI_ERROR (Status);
    goto Error;
  }

  //
  // Disconnect the current driver from handles in the handle database
  //
  for (Index = 0; Index < HandleCount; Index++) {
    gBS->DisconnectController (HandleBuffer[Index], ImageHandle, NULL);
  }

  //
  // Free the array of handles
  //
  if (HandleBuffer != NULL) {
    FreePool (HandleBuffer);
  }

  //
  // Uninstall protocols installed in the driver entry point
  //
  Status = gBS->UninstallMultipleProtocolInterfaces (
                  ImageHandle,
                  &gEfiDriverBindingProtocolGuid,
                  &gVirtualKeyboardDriverBinding,
                  &gEfiComponentNameProtocolGuid,
                  &gVirtualKeyboardComponentName,
                  &gEfiComponentName2ProtocolGuid,
                  &gVirtualKeyboardComponentName2,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    goto Error;
  }

  HiiRemovePackages (mHiiHandle);

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VirtualKeyboardDriverUnload Success, Status: %r\n", Status));
  goto End;

Error:
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VirtualKeyboardDriverUnload Failed, Status: %r\n", Status));

End:
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VirtualKeyboardDriverUnload End\n"));
  return Status;
}

EFI_DRIVER_BINDING_PROTOCOL gVirtualKeyboardDriverBinding = {
  VirtualKeyboardDriverSupported,
  VirtualKeyboardDriverStart,
  VirtualKeyboardDriverStop,
  0x10,
  NULL,
  NULL
};

/**
  This is the declaration of an EFI image entry point. This entry point is
  the same for UEFI Applications, UEFI OS Loaders, and UEFI Drivers including
  both device drivers and bus drivers.

  @param  ImageHandle           The firmware allocated handle for the UEFI image.
  @param  SystemTable           A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.

**/
EFI_STATUS
EFIAPI
VirtualKeyboardDriverEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS                     Status;

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VirtualKeyboardDriverEntryPoint Start\n"));
  //
  // Install UEFI Driver Model protocol(s).
  //
  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &gVirtualKeyboardDriverBinding,
             ImageHandle,
             &gVirtualKeyboardComponentName,
             &gVirtualKeyboardComponentName2
             );
  ASSERT_EFI_ERROR (Status);

  mHiiHandle = HiiAddPackages (
                 &gEfiCallerIdGuid,
                 NULL,
                 VirtualKeyboardDxeImages,
                 NULL
                 );
  ASSERT (mHiiHandle != NULL);

  mOrigConOutRow      = PcdGet32 (PcdConOutRow);
  mOrigSetupConOutRow = PcdGet32 (PcdSetupConOutRow);

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VirtualKeyboardDriverEntryPoint End\n"));
  return Status;
}
