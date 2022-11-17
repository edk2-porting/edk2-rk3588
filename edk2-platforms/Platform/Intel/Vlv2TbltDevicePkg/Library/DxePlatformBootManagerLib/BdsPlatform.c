/** @file
  This file include all platform action which can be customized by IBV/OEM.

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "BdsPlatform.h"
#include <Guid/EventGroup.h>
#include <Protocol/DxeSmmReadyToLock.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/PciRootBridgeIo.h>

#include <Protocol/BlockIo.h>
#include <Protocol/PciIo.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Guid/EventGroup.h>

#include <Library/Tcg2PhysicalPresenceLib.h>

#include <Library/HobLib.h>
#include <Protocol/UsbIo.h>
#include <Library/BootLogoLib.h>

#include <Library/UefiBootManagerLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED EFI_BOOT_MODE                 gBootMode;

BOOLEAN                      gPPRequireUIConfirm;

extern UINTN                                      mBootMenuOptionNumber;

GLOBAL_REMOVE_IF_UNREFERENCED USB_CLASS_FORMAT_DEVICE_PATH gUsbClassKeyboardDevicePath = {
  {
    {
      MESSAGING_DEVICE_PATH,
      MSG_USB_CLASS_DP,
      {
        (UINT8) (sizeof (USB_CLASS_DEVICE_PATH)),
        (UINT8) ((sizeof (USB_CLASS_DEVICE_PATH)) >> 8)
      }
    },
    0xffff,           // VendorId
    0xffff,           // ProductId
    CLASS_HID,        // DeviceClass
    SUBCLASS_BOOT,    // DeviceSubClass
    PROTOCOL_KEYBOARD // DeviceProtocol
  },
  gEndEntire
};

//
// Platform specific ISA serial device path
//
GLOBAL_REMOVE_IF_UNREFERENCED PLATFORM_ISA_SERIAL_DEVICE_PATH gIsaSerialDevicePath = {
  gPciRootBridge,
  gPciIsaBridge,
  gPnp16550ComPort,
  gUart(115200, 8, 1, 1),
  gPcAnsiTerminal,
  gEndEntire
};

//
// Internal shell mode
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT32         mShellModeColumn;
GLOBAL_REMOVE_IF_UNREFERENCED UINT32         mShellModeRow;
GLOBAL_REMOVE_IF_UNREFERENCED UINT32         mShellHorizontalResolution;
GLOBAL_REMOVE_IF_UNREFERENCED UINT32         mShellVerticalResolution;
//
// BDS Platform Functions
//

BOOLEAN
IsMorBitSet (
  VOID
  )
{
  UINTN                     MorControl;
  EFI_STATUS                Status;
  UINTN                     DataSize;

  //
  // Check if the MOR bit is set.
  //
  DataSize = sizeof (MorControl);
  Status = gRT->GetVariable (
                  MEMORY_OVERWRITE_REQUEST_VARIABLE_NAME,
                  &gEfiMemoryOverwriteControlDataGuid,
                  NULL,
                  &DataSize,
                  &MorControl
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, " PlatformBootMangerLib: gEfiMemoryOverwriteControlDataGuid doesn't exist!!***\n"));
    MorControl = 0;
  } else {
    DEBUG ((DEBUG_INFO, " PlatformBootMangerLib: Get the gEfiMemoryOverwriteControlDataGuid = %x!!***\n", MorControl));
  }

  return (BOOLEAN) (MorControl & 0x01);
}

VOID
DumpDevicePath (
  IN CHAR16           *Name,
  IN EFI_DEVICE_PATH  *DevicePath
  )
{
  CHAR16 *Str;

  Str = ConvertDevicePathToText(DevicePath, TRUE, TRUE);
  DEBUG ((DEBUG_INFO, "%s: %s\n", Name, Str));
  if (Str != NULL) {
    FreePool (Str);
  }
}

/**
  An empty function to pass error checking of CreateEventEx ().

  This empty function ensures that EVT_NOTIFY_SIGNAL_ALL is error
  checked correctly since it is now mapped into CreateEventEx() in UEFI 2.0.

  @param  Event                 Event whose notification function is being invoked.
  @param  Context               The pointer to the notification function's context,
                                which is implementation-dependent.
**/
VOID
EFIAPI
InternalBdsEmptyCallbackFuntion (
  IN EFI_EVENT                Event,
  IN VOID                     *Context
  )
{
  return;
}

VOID
ExitPmAuth (
  VOID
  )
{
  EFI_HANDLE                 Handle;
  EFI_STATUS                 Status;
  EFI_EVENT                  EndOfDxeEvent;

  DEBUG((DEBUG_INFO,"ExitPmAuth ()- Start\n"));
  //
  // Prepare S3 information, this MUST be done before ExitPmAuth/EndOfDxe
  //
  //
  // Since PI1.2.1, we need signal EndOfDxe as ExitPmAuth
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  InternalBdsEmptyCallbackFuntion,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);
  gBS->SignalEvent (EndOfDxeEvent);
  gBS->CloseEvent (EndOfDxeEvent);
  DEBUG((DEBUG_INFO,"All EndOfDxe callbacks have returned successfully\n"));

  //
  // NOTE: We need install DxeSmmReadyToLock directly here because many boot script is added via ExitPmAuth/EndOfDxe callback.
  // If we install them at same callback, these boot script will be rejected because BootScript Driver runs first to lock them done.
  // So we seperate them to be 2 different events, ExitPmAuth is last chance to let platform add boot script. DxeSmmReadyToLock will
  // make boot script save driver lock down the interface.
  //
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEfiDxeSmmReadyToLockProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
  DEBUG((DEBUG_INFO,"ExitPmAuth ()- End\n"));
}

VOID
ConnectRootBridge (
  BOOLEAN Recursive
  )
{
  UINTN                            RootBridgeHandleCount;
  EFI_HANDLE                       *RootBridgeHandleBuffer;
  UINTN                            RootBridgeIndex;

  RootBridgeHandleCount = 0;
  gBS->LocateHandleBuffer (
         ByProtocol,
         &gEfiPciRootBridgeIoProtocolGuid,
         NULL,
         &RootBridgeHandleCount,
         &RootBridgeHandleBuffer
         );
  for (RootBridgeIndex = 0; RootBridgeIndex < RootBridgeHandleCount; RootBridgeIndex++) {
    gBS->ConnectController (RootBridgeHandleBuffer[RootBridgeIndex], NULL, NULL, Recursive);
  }
}


/**
  Return whether the device is trusted console.

  @param Device  The device to be tested.

  @retval TRUE   The device can be trusted.
  @retval FALSE  The device cannot be trusted.
**/
BOOLEAN
IsTrustedConsole (
  IN CONSOLE_TYPE              ConsoleType,
  IN EFI_DEVICE_PATH_PROTOCOL  *Device
  )
{
  VOID                      *TrustedConsoleDevicepath;
  EFI_DEVICE_PATH_PROTOCOL  *TempDevicePath;
  EFI_DEVICE_PATH_PROTOCOL  *Instance;
  UINTN                     Size;
  EFI_DEVICE_PATH_PROTOCOL  *ConsoleDevice;

  if (Device == NULL) {
    return FALSE;
  }

  ConsoleDevice = DuplicateDevicePath(Device);

  TrustedConsoleDevicepath = NULL;

  switch (ConsoleType) {
  case ConIn:
    TrustedConsoleDevicepath = PcdGetPtr (PcdTrustedConsoleInputDevicePath);
    break;
  case ConOut:
    //
    // Check GOP and remove last node
    //
    TempDevicePath = ConsoleDevice;
    while (!IsDevicePathEndType (TempDevicePath)) {
      if (DevicePathType (TempDevicePath) == ACPI_DEVICE_PATH &&
          DevicePathSubType (TempDevicePath) == ACPI_ADR_DP) {
        SetDevicePathEndNode (TempDevicePath);
        break;
      }
      TempDevicePath = NextDevicePathNode (TempDevicePath);
    }

    TrustedConsoleDevicepath = PcdGetPtr (PcdTrustedConsoleOutputDevicePath);
    break;
  default:
    ASSERT(FALSE);
    break;
  }

  TempDevicePath = TrustedConsoleDevicepath;
  do {
    Instance = GetNextDevicePathInstance (&TempDevicePath, &Size);
    if (Instance == NULL) {
      break;
    }

    if (CompareMem (ConsoleDevice, Instance, Size - END_DEVICE_PATH_LENGTH) == 0) {
      FreePool (Instance);
      FreePool (ConsoleDevice);
      return TRUE;
    }

    FreePool (Instance);
  } while (TempDevicePath != NULL);

  FreePool (ConsoleDevice);

  return FALSE;
}

BOOLEAN
IsUsbShortForm (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  if ((DevicePathType (DevicePath) == MESSAGING_DEVICE_PATH) &&
      ((DevicePathSubType (DevicePath) == MSG_USB_CLASS_DP) || (DevicePathSubType (DevicePath) == MSG_USB_WWID_DP)) ) {
    return TRUE;
  }

  return FALSE;
}

/**
  Connect the USB short form device path.

  @param DevicePath   USB short form device path

  @retval EFI_SUCCESS           Successfully connected the USB device
  @retval EFI_NOT_FOUND         Cannot connect the USB device
  @retval EFI_INVALID_PARAMETER The device path is invalid.
**/
EFI_STATUS
ConnectUsbShortFormDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL   *DevicePath
  )
{
  EFI_STATUS                            Status;
  EFI_HANDLE                            *Handles;
  UINTN                                 HandleCount;
  UINTN                                 Index;
  EFI_PCI_IO_PROTOCOL                   *PciIo;
  UINT8                                 Class[3];
  BOOLEAN                               AtLeastOneConnected;

  //
  // Check the passed in parameters
  //
  if (DevicePath == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (!IsUsbShortForm (DevicePath)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Find the usb host controller firstly, then connect with the remaining device path
  //
  AtLeastOneConnected = FALSE;
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &Handles
                  );
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    Handles[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID **) &PciIo
                    );
    if (!EFI_ERROR (Status)) {
      //
      // Check whether the Pci device is the wanted usb host controller
      //
      Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint8, 0x09, 3, &Class);
      if (!EFI_ERROR (Status) &&
          ((PCI_CLASS_SERIAL == Class[2]) && (PCI_CLASS_SERIAL_USB == Class[1]))
         ) {
        Status = gBS->ConnectController (
                        Handles[Index],
                        NULL,
                        DevicePath,
                        FALSE
                        );
        if (!EFI_ERROR(Status)) {
          AtLeastOneConnected = TRUE;
        }
      }
    }
  }

  return AtLeastOneConnected ? EFI_SUCCESS : EFI_NOT_FOUND;
}

/**
  Update the ConIn/ConOut variable with ISA Serial device path,if its not already exists in ConIn/ConOut
**/
VOID
EnumIsaSerial (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "[EnumIsaSerial]\n"));
  EfiBootManagerUpdateConsoleVariable (ConIn, (EFI_DEVICE_PATH_PROTOCOL *) &gIsaSerialDevicePath, NULL);
  EfiBootManagerUpdateConsoleVariable (ConOut, (EFI_DEVICE_PATH_PROTOCOL *) &gIsaSerialDevicePath, NULL);

  //
  // Append ISA Serial DevicePath to "ConInDev" and "ConOutDev"
  //
  EfiBootManagerUpdateConsoleVariable (ConInDev, (EFI_DEVICE_PATH_PROTOCOL *) &gIsaSerialDevicePath, NULL);
  EfiBootManagerUpdateConsoleVariable (ConOutDev, (EFI_DEVICE_PATH_PROTOCOL *) &gIsaSerialDevicePath, NULL);
}

/**
  Update the ConIn variable with USB Keyboard device path,if its not already exists in ConIn
**/
VOID
EnumUsbKeyboard (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "[EnumUsbKeyboard]\n"));
  EfiBootManagerUpdateConsoleVariable (ConIn, (EFI_DEVICE_PATH_PROTOCOL *) &gUsbClassKeyboardDevicePath, NULL);

  //
  // Append Usb Keyboard short form DevicePath into "ConInDev"
  //
  EfiBootManagerUpdateConsoleVariable (ConInDev, (EFI_DEVICE_PATH_PROTOCOL *) &gUsbClassKeyboardDevicePath, NULL);
}

BOOLEAN
IsVgaHandle (
  IN EFI_HANDLE Handle
  )
{
  EFI_PCI_IO_PROTOCOL *PciIo;
  PCI_TYPE00          Pci;
  EFI_STATUS          Status;

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiPciIoProtocolGuid,
                  (VOID **)&PciIo
                  );
  if (!EFI_ERROR (Status)) {
    Status = PciIo->Pci.Read (
                          PciIo,
                          EfiPciIoWidthUint32,
                          0,
                          sizeof (Pci) / sizeof (UINT32),
                          &Pci
                          );
    if (!EFI_ERROR (Status)) {
      if (IS_PCI_VGA (&Pci) || IS_PCI_OLD_VGA (&Pci)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

EFI_HANDLE
IsVideoController (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *DupDevicePath;
  EFI_DEVICE_PATH_PROTOCOL  *TempDevicePath;
  EFI_STATUS                Status;
  EFI_HANDLE                DeviceHandle;

  DupDevicePath = DuplicateDevicePath (DevicePath);
  ASSERT (DupDevicePath != NULL);
  if (DupDevicePath == NULL) {
    return NULL;
  }

  TempDevicePath = DupDevicePath;
  Status = gBS->LocateDevicePath (
                  &gEfiDevicePathProtocolGuid,
                  &TempDevicePath,
                  &DeviceHandle
                  );
  FreePool (DupDevicePath);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  if (IsVgaHandle (DeviceHandle)) {
    return DeviceHandle;
  } else {
    return NULL;
  }
}

BOOLEAN
IsGopDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  while (!IsDevicePathEndType (DevicePath)) {
    if (DevicePathType (DevicePath) == ACPI_DEVICE_PATH &&
        DevicePathSubType (DevicePath) == ACPI_ADR_DP) {
      return TRUE;
    }
    DevicePath = NextDevicePathNode (DevicePath);
  }
  return FALSE;
}

/**
  Remove all GOP device path instance from DevicePath and add the Gop to the DevicePath.
**/
EFI_DEVICE_PATH_PROTOCOL *
UpdateGopDevicePath (
  EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  EFI_DEVICE_PATH_PROTOCOL *Gop
  )
{
  UINTN                    Size;
  UINTN                    GopSize;
  EFI_DEVICE_PATH_PROTOCOL *Temp;
  EFI_DEVICE_PATH_PROTOCOL *Return;
  EFI_DEVICE_PATH_PROTOCOL *Instance;
  BOOLEAN                  Exist;

  Exist = FALSE;
  Return = NULL;
  GopSize = GetDevicePathSize (Gop);
  do {
    Instance = GetNextDevicePathInstance (&DevicePath, &Size);
    if (Instance == NULL) {
      break;
    }
    if (!IsGopDevicePath (Instance) ||
        (Size == GopSize && CompareMem (Instance, Gop, GopSize) == 0)
       ) {
      if (Size == GopSize && CompareMem (Instance, Gop, GopSize) == 0) {
        Exist = TRUE;
      }
      Temp = Return;
      Return = AppendDevicePathInstance (Return, Instance);
      if (Temp != NULL) {
        FreePool (Temp);
      }
    }
    FreePool (Instance);
  } while (DevicePath != NULL);

  if (!Exist) {
    Temp = Return;
    Return = AppendDevicePathInstance (Return, Gop);
    if (Temp != NULL) {
      FreePool (Temp);
    }
  }
  return Return;
}

/**
  Get Graphics Controller Handle.

  @retval GraphicsController    Successfully located
  @retval NULL                  Failed to locate
**/
EFI_HANDLE
EFIAPI
GetGraphicsController (
  IN BOOLEAN    NeedTrustedConsole
  )
{
  EFI_STATUS                Status;
  UINTN                     Index;
  EFI_HANDLE                *PciHandles;
  UINTN                     PciHandlesSize;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &PciHandlesSize,
                  &PciHandles
                  );
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  for (Index = 0; Index < PciHandlesSize; Index++) {
    Status = gBS->HandleProtocol (
                    PciHandles[Index],
                    &gEfiDevicePathProtocolGuid,
                    (VOID **) &DevicePath
                    );
    if (EFI_ERROR(Status)) {
      continue;
    }
    if (!IsVgaHandle (PciHandles[Index])) {
      continue;
    }
    if ((NeedTrustedConsole && IsTrustedConsole (ConOut, DevicePath)) ||
        ((!NeedTrustedConsole) && (!IsTrustedConsole (ConOut, DevicePath)))) {
      return PciHandles[Index];
    }
  }

  return NULL;
}

VOID
UpdateGraphicConOut (
  IN BOOLEAN    NeedTrustedConsole
  )
{
  EFI_HANDLE                          GraphicsControllerHandle;
  EFI_DEVICE_PATH_PROTOCOL            *GopDevicePath;
  EFI_DEVICE_PATH_PROTOCOL            *ConOutDevicePath;
  EFI_DEVICE_PATH_PROTOCOL            *UpdatedConOutDevicePath;

  //
  // Update ConOut variable
  //
  GraphicsControllerHandle = GetGraphicsController (NeedTrustedConsole);
  if (GraphicsControllerHandle != NULL) {
    //
    // Connect the GOP driver
    //
    gBS->ConnectController (GraphicsControllerHandle, NULL, NULL, TRUE);

    //
    // Get the GOP device path
    // NOTE: We may get a device path that contains Controller node in it.
    //
    GopDevicePath = EfiBootManagerGetGopDevicePath (GraphicsControllerHandle);
    if (GopDevicePath != NULL) {
      GetEfiGlobalVariable2 (L"ConOut", (VOID **)&ConOutDevicePath, NULL);
      UpdatedConOutDevicePath = UpdateGopDevicePath (ConOutDevicePath, GopDevicePath);
      if (ConOutDevicePath != NULL) {
        FreePool (ConOutDevicePath);
      }
      FreePool (GopDevicePath);
      gRT->SetVariable (
                      L"ConOut",
                      &gEfiGlobalVariableGuid,
                      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                      GetDevicePathSize (UpdatedConOutDevicePath),
                      UpdatedConOutDevicePath
                      );
    }
  }
}

VOID
AddConsoleVariable (
  IN CONSOLE_TYPE              ConsoleType,
  IN EFI_DEVICE_PATH           *ConsoleDevicePath
  )
{
  EFI_DEVICE_PATH           *TempDevicePath;
  EFI_DEVICE_PATH           *Instance;
  UINTN                     Size;
  EFI_HANDLE                GraphicsControllerHandle;
  EFI_DEVICE_PATH           *GopDevicePath;

  TempDevicePath = ConsoleDevicePath;
  do {
    Instance = GetNextDevicePathInstance (&TempDevicePath, &Size);
    if (Instance == NULL) {
      break;
    }

    switch (ConsoleType) {
    case ConIn:
      if (IsUsbShortForm (Instance)) {
        //
        // Append Usb Keyboard short form DevicePath into "ConInDev"
        //
        EfiBootManagerUpdateConsoleVariable (ConInDev, Instance, NULL);
      }
      EfiBootManagerUpdateConsoleVariable (ConsoleType, Instance, NULL);
      break;
    case ConOut:
      GraphicsControllerHandle = IsVideoController (Instance);
      if (GraphicsControllerHandle == NULL) {
        EfiBootManagerUpdateConsoleVariable (ConsoleType, Instance, NULL);
      } else {
        //
        // Connect the GOP driver
        //
        gBS->ConnectController (GraphicsControllerHandle, NULL, NULL, TRUE);
        //
        // Get the GOP device path
        // NOTE: We may get a device path that contains Controller node in it.
        //
        GopDevicePath = EfiBootManagerGetGopDevicePath (GraphicsControllerHandle);
        if (GopDevicePath != NULL) {
          EfiBootManagerUpdateConsoleVariable (ConsoleType, GopDevicePath, NULL);
        }
      }
      break;
    default:
      ASSERT(FALSE);
      break;
    }

    FreePool (Instance);
  } while (TempDevicePath != NULL);
}

/**
  The function connects the trusted consoles.
**/
VOID
ConnectTrustedConsole (
  VOID
  )
{
  EFI_DEVICE_PATH_PROTOCOL     *Consoles;
  EFI_DEVICE_PATH_PROTOCOL     *TempDevicePath;
  EFI_DEVICE_PATH_PROTOCOL     *Instance;
  EFI_DEVICE_PATH_PROTOCOL     *Next;
  UINTN                        Size;
  UINTN                        Index;
  EFI_HANDLE                   Handle;
  EFI_STATUS                   Status;
  CHAR16                       *ConsoleVar[] = {L"ConIn", L"ConOut"};
  VOID                         *TrustedConsoleDevicepath;

  TrustedConsoleDevicepath = PcdGetPtr (PcdTrustedConsoleInputDevicePath);
  DumpDevicePath (L"TrustedConsoleIn", TrustedConsoleDevicepath);
  TrustedConsoleDevicepath = PcdGetPtr (PcdTrustedConsoleOutputDevicePath);
  DumpDevicePath (L"TrustedConsoleOut", TrustedConsoleDevicepath);

  for (Index = 0; Index < sizeof (ConsoleVar) / sizeof (ConsoleVar[0]); Index++) {

    GetEfiGlobalVariable2 (ConsoleVar[Index], (VOID **)&Consoles, NULL);

    TempDevicePath = Consoles;
    do {
      Instance = GetNextDevicePathInstance (&TempDevicePath, &Size);
      if (Instance == NULL) {
        break;
      }
      if (IsTrustedConsole (Index, Instance)) {
        if (IsUsbShortForm (Instance)) {
          ConnectUsbShortFormDevicePath (Instance);
        } else {
          for (Next = Instance; !IsDevicePathEnd (Next); Next = NextDevicePathNode (Next)) {
            if (DevicePathType (Next) == ACPI_DEVICE_PATH && DevicePathSubType (Next) == ACPI_ADR_DP) {
              break;
            } else if (DevicePathType (Next) == HARDWARE_DEVICE_PATH &&
                       DevicePathSubType (Next) == HW_CONTROLLER_DP &&
                       DevicePathType (NextDevicePathNode (Next)) == ACPI_DEVICE_PATH &&
                       DevicePathSubType (NextDevicePathNode (Next)) == ACPI_ADR_DP
                       ) {
              break;
            }
          }
          if (!IsDevicePathEnd (Next)) {
            SetDevicePathEndNode (Next);
            Status = EfiBootManagerConnectDevicePath (Instance, &Handle);
            if (!EFI_ERROR (Status)) {
              gBS->ConnectController (Handle, NULL, NULL, TRUE);
            }
          } else {
            EfiBootManagerConnectDevicePath (Instance, NULL);
          }
        }
      }
      FreePool (Instance);
    } while (TempDevicePath != NULL);

    if (Consoles != NULL) {
      FreePool (Consoles);
    }
  }
}

/**
  The function connects the trusted Storages.
**/
VOID
ConnectTrustedStorage (
  VOID
  )
{
  VOID                      *TrustedStorageDevicepath;
  EFI_DEVICE_PATH_PROTOCOL  *TempDevicePath;
  EFI_DEVICE_PATH_PROTOCOL  *Instance;
  UINTN                     Size;
  EFI_DEVICE_PATH_PROTOCOL  *TempStorageDevicePath;
  EFI_STATUS                Status;
  EFI_HANDLE                DeviceHandle;

  TrustedStorageDevicepath = PcdGetPtr (PcdTrustedStorageDevicePath);
  DumpDevicePath (L"TrustedStorage", TrustedStorageDevicepath);

  TempDevicePath = TrustedStorageDevicepath;
  do {
    Instance = GetNextDevicePathInstance (&TempDevicePath, &Size);
    if (Instance == NULL) {
      break;
    }

    EfiBootManagerConnectDevicePath (Instance, NULL);

    TempStorageDevicePath = Instance;

    Status = gBS->LocateDevicePath (
                    &gEfiDevicePathProtocolGuid,
                    &TempStorageDevicePath,
                    &DeviceHandle
                    );
    if (!EFI_ERROR (Status)) {
      gBS->ConnectController (DeviceHandle, NULL, NULL, FALSE);
    }

    FreePool (Instance);
  } while (TempDevicePath != NULL);
}

/**
  The function connects the trusted consoles and then call the PP processing library interface.
**/
VOID
ProcessTcgPp (
  VOID
  )
{
  gPPRequireUIConfirm |= Tcg2PhysicalPresenceLibNeedUserConfirm();

  if (gPPRequireUIConfirm) {
    ConnectTrustedConsole ();
  }

  Tcg2PhysicalPresenceLibProcessRequest (NULL);
}

/**
  The function connects the trusted storage to perform TPerReset.
**/
VOID
ProcessTcgMor (
  VOID
  )
{
  if (IsMorBitSet ()) {
    ConnectTrustedConsole();
    ConnectTrustedStorage();
  }
}

/**
  Check if current BootCurrent variable is internal shell boot option.

  @retval  TRUE         BootCurrent is internal shell.
  @retval  FALSE        BootCurrent is not internal shell.
**/
BOOLEAN
BootCurrentIsInternalShell (
  VOID
  )
{
  UINTN                         VarSize;
  UINT16                        BootCurrent;
  CHAR16                        BootOptionName[16];
  UINT8                         *BootOption;
  UINT8                         *Ptr;
  BOOLEAN                       Result;
  EFI_STATUS                    Status;
  EFI_DEVICE_PATH_PROTOCOL      *TempDevicePath;
  EFI_DEVICE_PATH_PROTOCOL      *LastDeviceNode;
  EFI_GUID                      *GuidPoint;

  BootOption = NULL;
  Result     = FALSE;

  //
  // Get BootCurrent variable
  //
  VarSize = sizeof (UINT16);
  Status = gRT->GetVariable (
                  L"BootCurrent",
                  &gEfiGlobalVariableGuid,
                  NULL,
                  &VarSize,
                  &BootCurrent
                  );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  //
  // Create boot option Bootxxxx from BootCurrent
  //
  UnicodeSPrint (BootOptionName, sizeof(BootOptionName), L"Boot%04X", BootCurrent);

  GetEfiGlobalVariable2 (BootOptionName, (VOID **) &BootOption, &VarSize);
  if (BootOption == NULL || VarSize == 0) {
    return FALSE;
  }

  Ptr = BootOption;
  Ptr += sizeof (UINT32);
  Ptr += sizeof (UINT16);
  Ptr += StrSize ((CHAR16 *) Ptr);
  TempDevicePath = (EFI_DEVICE_PATH_PROTOCOL *) Ptr;
  LastDeviceNode = TempDevicePath;
  while (!IsDevicePathEnd (TempDevicePath)) {
    LastDeviceNode = TempDevicePath;
    TempDevicePath = NextDevicePathNode (TempDevicePath);
  }
  GuidPoint = EfiGetNameGuidFromFwVolDevicePathNode (
                (MEDIA_FW_VOL_FILEPATH_DEVICE_PATH *) LastDeviceNode
                );
  if ((GuidPoint != NULL) &&
      ((CompareGuid (GuidPoint, &gUefiShellFileGuid)))
    ) {
    //
    // if this option is internal shell, return TRUE
    //
    Result = TRUE;
  }

  if (BootOption != NULL) {
    FreePool (BootOption);
    BootOption = NULL;
  }

  return Result;
}

/**
  This function will change video resolution and text mode
  for internl shell when internal shell is launched.

  @param   None.

  @retval  EFI_SUCCESS  Mode is changed successfully.
  @retval  Others       Mode failed to changed.
**/
EFI_STATUS
EFIAPI
ChangeModeForInternalShell (
  VOID
  )
{
  EFI_GRAPHICS_OUTPUT_PROTOCOL          *GraphicsOutput;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL       *SimpleTextOut;
  UINTN                                 SizeOfInfo;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  *Info;
  UINT32                                MaxGopMode;
  UINT32                                MaxTextMode;
  UINT32                                ModeNumber;
  UINTN                                 HandleCount;
  EFI_HANDLE                            *HandleBuffer;
  EFI_STATUS                            Status;
  UINTN                                 Index;
  UINTN                                 CurrentColumn;
  UINTN                                 CurrentRow;

  Status = gBS->HandleProtocol (
                  gST->ConsoleOutHandle,
                  &gEfiGraphicsOutputProtocolGuid,
                  (VOID**)&GraphicsOutput
                  );
  if (EFI_ERROR (Status)) {
    GraphicsOutput = NULL;
  }

  Status = gBS->HandleProtocol (
                  gST->ConsoleOutHandle,
                  &gEfiSimpleTextOutProtocolGuid,
                  (VOID**)&SimpleTextOut
                  );
  if (EFI_ERROR (Status)) {
    SimpleTextOut = NULL;
  }

  if ((GraphicsOutput == NULL) || (SimpleTextOut == NULL)) {
    return EFI_UNSUPPORTED;
  }

  MaxGopMode  = GraphicsOutput->Mode->MaxMode;
  MaxTextMode = SimpleTextOut->Mode->MaxMode;

  //
  // 1. If current video resolution is same with new video resolution,
  //    video resolution need not be changed.
  //    1.1. If current text mode is same with new text mode, text mode need not be change.
  //    1.2. If current text mode is different with new text mode, text mode need be change to new text mode.
  // 2. If current video resolution is different with new video resolution, we need restart whole console drivers.
  //
  for (ModeNumber = 0; ModeNumber < MaxGopMode; ModeNumber++) {
    Status = GraphicsOutput->QueryMode (
                       GraphicsOutput,
                       ModeNumber,
                       &SizeOfInfo,
                       &Info
                       );
    if (!EFI_ERROR (Status)) {
      if ((Info->HorizontalResolution == mShellHorizontalResolution) &&
          (Info->VerticalResolution == mShellVerticalResolution)) {
        if ((GraphicsOutput->Mode->Info->HorizontalResolution == mShellHorizontalResolution) &&
            (GraphicsOutput->Mode->Info->VerticalResolution == mShellVerticalResolution)) {
          //
          // If current video resolution is same with new resolution,
          // then check if current text mode is same with new text mode.
          //
          Status = SimpleTextOut->QueryMode (SimpleTextOut, SimpleTextOut->Mode->Mode, &CurrentColumn, &CurrentRow);
          ASSERT_EFI_ERROR (Status);
          if (CurrentColumn == mShellModeColumn && CurrentRow == mShellModeRow) {
            //
            // Current text mode is same with new text mode, text mode need not be change.
            //
            FreePool (Info);
            return EFI_SUCCESS;
          } else {
            //
            // Current text mode is different with new text mode, text mode need be change to new text mode.
            //
            for (Index = 0; Index < MaxTextMode; Index++) {
              Status = SimpleTextOut->QueryMode (SimpleTextOut, Index, &CurrentColumn, &CurrentRow);
              if (!EFI_ERROR(Status)) {
                if ((CurrentColumn == mShellModeColumn) && (CurrentRow == mShellModeRow)) {
                  //
                  // New text mode is supported, set it.
                  //
                  Status = SimpleTextOut->SetMode (SimpleTextOut, Index);
                  ASSERT_EFI_ERROR (Status);
                  //
                  // Update text mode PCD.
                  //
                  Status = PcdSet32S (PcdConOutColumn, mShellModeColumn);
                  ASSERT_EFI_ERROR (Status);

                  Status = PcdSet32S (PcdConOutRow, mShellModeRow);
                  ASSERT_EFI_ERROR (Status);

                  FreePool (Info);
                  return EFI_SUCCESS;
                }
              }
            }
            if (Index == MaxTextMode) {
              //
              // If new text mode is not supported, return error.
              //
              FreePool (Info);
              return EFI_UNSUPPORTED;
            }
          }
        } else {
          FreePool (Info);
          //
          // If current video resolution is not same with the new one, set new video resolution.
          // In this case, the driver which produces simple text out need be restarted.
          //
          Status = GraphicsOutput->SetMode (GraphicsOutput, ModeNumber);
          if (!EFI_ERROR (Status)) {
            //
            // Set PCD to restart GraphicsConsole and Consplitter to change video resolution
            // and produce new text mode based on new resolution.
            //
            Status = PcdSet32S (PcdVideoHorizontalResolution, mShellHorizontalResolution);
            ASSERT_EFI_ERROR (Status);

            Status = PcdSet32S (PcdVideoVerticalResolution, mShellVerticalResolution);
            ASSERT_EFI_ERROR (Status);

            Status = PcdSet32S (PcdConOutColumn, mShellModeColumn);
            ASSERT_EFI_ERROR (Status);

            Status = PcdSet32S (PcdConOutRow, mShellModeRow);
            ASSERT_EFI_ERROR (Status);

            Status = gBS->LocateHandleBuffer (
                             ByProtocol,
                             &gEfiSimpleTextOutProtocolGuid,
                             NULL,
                             &HandleCount,
                             &HandleBuffer
                             );
            if (!EFI_ERROR (Status)) {
              for (Index = 0; Index < HandleCount; Index++) {
                gBS->DisconnectController (HandleBuffer[Index], NULL, NULL);
              }
              for (Index = 0; Index < HandleCount; Index++) {
                gBS->ConnectController (HandleBuffer[Index], NULL, NULL, TRUE);
              }
              if (HandleBuffer != NULL) {
                FreePool (HandleBuffer);
              }
              break;
            }
          }
        }
      }
      FreePool (Info);
    }
  }

  if (ModeNumber == MaxGopMode) {
    //
    // If the new resolution is not supported, return error.
    //
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  ReadyToBoot callback to set video and text mode for internal shell boot.
  That will not connect USB controller while CSM and FastBoot are disabled, we need to connect them
  before booting to Shell for showing USB devices in Shell.

  When FastBoot is enabled and Windows Console is the chosen Console behavior, input devices will not be connected
  by default. Hence, when booting to EFI shell, connecting input consoles are required.

  @param  Event   Pointer to this event
  @param  Context Event hanlder private data

  @retval None.
**/
VOID
EFIAPI
OnReadyToBootCallBack (
  IN  EFI_EVENT                 Event,
  IN  VOID                      *Context
  )
{
  DEBUG ((DEBUG_INFO, "OnReadyToBootCallBack\n"));

  if (BootCurrentIsInternalShell ()) {

    ChangeModeForInternalShell ();
    EfiBootManagerConnectAllDefaultConsoles();
    gDS->Dispatch ();
  }
}

/**
  Platform Bds init. Incude the platform firmware vendor, revision
  and so crc check.
**/
VOID
EFIAPI
PlatformBootManagerBeforeConsole (
  VOID
  )
{
  EFI_STATUS                          Status;
  EFI_DEVICE_PATH_PROTOCOL            *VarConOut;
  EFI_DEVICE_PATH_PROTOCOL            *VarConIn;
  EFI_EVENT                           Event;

  DEBUG ((DEBUG_INFO, "PlatformBootManagerBeforeConsole\n"));

  Status = EFI_SUCCESS;

  //
  // Get user defined text mode for internal shell only once.
  //
  mShellHorizontalResolution = PcdGet32 (PcdSetupVideoHorizontalResolution);
  mShellVerticalResolution   = PcdGet32 (PcdSetupVideoVerticalResolution);
  mShellModeColumn           = PcdGet32 (PcdSetupConOutColumn);
  mShellModeRow              = PcdGet32 (PcdSetupConOutRow);

  //
  // Create event to set proper video resolution and text mode for internal shell.
  //
  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             OnReadyToBootCallBack,
             NULL,
             &Event
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Connect Root Bridge to make PCI BAR resource allocated and all PciIo created
  //
  ConnectRootBridge (FALSE);

  //
  // Fill ConIn/ConOut in Full Configuration boot mode
  //
  gBootMode = GetBootModeHob();
  DEBUG ((DEBUG_INFO, "PlatformBootManagerBeforeConsole: BootMode = %x\n", gBootMode));

  if (gBootMode == BOOT_WITH_FULL_CONFIGURATION ||
      gBootMode == BOOT_WITH_DEFAULT_SETTINGS ||
      gBootMode == BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS ||
      gBootMode == BOOT_IN_RECOVERY_MODE) {

    GetEfiGlobalVariable2 (L"ConOut", (VOID **)&VarConOut, NULL);   if (VarConOut != NULL) { FreePool (VarConOut); }
    GetEfiGlobalVariable2 (L"ConIn", (VOID **)&VarConIn, NULL);    if (VarConIn  != NULL) { FreePool (VarConIn);  }

    //
    // Only fill ConIn/ConOut when ConIn/ConOut is empty because we may drop to Full Configuration boot mode in non-first boot
    //
    if (VarConOut == NULL || VarConIn == NULL) {
      if (PcdGetSize (PcdTrustedConsoleOutputDevicePath) >= sizeof(EFI_DEVICE_PATH_PROTOCOL)) {
        AddConsoleVariable (ConOut, PcdGetPtr (PcdTrustedConsoleOutputDevicePath));
      }
      if (PcdGetSize (PcdTrustedConsoleInputDevicePath) >= sizeof(EFI_DEVICE_PATH_PROTOCOL)) {
        AddConsoleVariable (ConIn, PcdGetPtr (PcdTrustedConsoleInputDevicePath));
      }
    }
  }

  EnumIsaSerial ();

  EnumUsbKeyboard ();
  //
  // For trusted console it must be handled here.
  //
  UpdateGraphicConOut (TRUE);

  if (gBootMode == BOOT_ON_FLASH_UPDATE) {
    //
    // Logo show
    //
    BootLogoEnableLogo ();

    DEBUG((DEBUG_INFO, "ProcessCapsules Before EndOfDxe......\n"));
    ProcessCapsules ();
    DEBUG((DEBUG_INFO, "ProcessCapsules Done\n"));
  }

  //
  // Dynamically register hot key: F2/F7/Enter
  //
  RegisterDefaultBootOption ();
  RegisterStaticHotkey ();

  PERF_START_EX(NULL,"EventRec", NULL, AsmReadTsc(), 0x7010);
  if (PcdGetBool (PcdTpm2Enable)) {
    ProcessTcgPp ();
    ProcessTcgMor ();
  }
  PERF_END_EX(NULL,"EventRec", NULL, AsmReadTsc(), 0x7011);

  //
  // We should make all UEFI memory and GCD information populated before ExitPmAuth.
  // SMM may consume these information.
  //
  MemoryTest((EXTENDMEM_COVERAGE_LEVEL) PcdGet32 (PcdPlatformMemoryCheckLevel));

  PERF_START_EX(NULL,"EventRec", NULL, AsmReadTsc(), 0x7020);
  ExitPmAuth ();
  PERF_END_EX(NULL,"EventRec", NULL, AsmReadTsc(), 0x7021);

  //
  // Dispatch the deferred 3rd party images.
  //
  EfiBootManagerDispatchDeferredImages ();

  //
  // For non-trusted console it must be handled here.
  //
  UpdateGraphicConOut (FALSE);

  if (gBootMode == BOOT_ON_FLASH_UPDATE) {
    //
    // Logo show
    //
    BootLogoEnableLogo ();

    DEBUG((DEBUG_INFO, "ProcessCapsules After ConnectAll......\n"));
    ProcessCapsules();
    DEBUG((DEBUG_INFO, "ProcessCapsules Done\n"));
  }
}


/**
  Connect with predeined platform connect sequence,
  the OEM/IBV can customize with their own connect sequence.

  @param[in] BootMode          Boot mode of this boot.
**/
VOID
ConnectSequence (
  IN EFI_BOOT_MODE         BootMode
  )
{
  switch (BootMode) {
  case BOOT_ASSUMING_NO_CONFIGURATION_CHANGES:
  case BOOT_WITH_MINIMAL_CONFIGURATION:
  case BOOT_ON_S4_RESUME:
    break;
  case BOOT_WITH_FULL_CONFIGURATION:
  case BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS:
  case BOOT_WITH_DEFAULT_SETTINGS:
  default:
    EfiBootManagerConnectAll ();
    break;
  }
}

/**
  The function is to consider the boot order which is not in our expectation.
  In the case that we need to re-sort the boot option.

  @retval  TRUE         Need to sort Boot Option.
  @retval  FALSE        Don't need to sort Boot Option.
**/
BOOLEAN
IsNeedSortBootOption (
  VOID
  )
{
  EFI_BOOT_MANAGER_LOAD_OPTION  *BootOptions;
  UINTN                         BootOptionCount;

  BootOptions = EfiBootManagerGetLoadOptions (&BootOptionCount, LoadOptionTypeBoot);

  //
  // If setup is the first priority in boot option, we need to sort boot option.
  //
  if ((BootOptionCount > 1) &&
      (((StrnCmp (BootOptions->Description, L"Enter Setup", StrLen (L"Enter Setup"))) == 0) ||
       ((StrnCmp (BootOptions->Description, L"Boot Device List", StrLen (L"Boot Device List"))) == 0))) {
    return TRUE;
  }

  return FALSE;
}

/**
  The function will excute with as the platform policy, current policy
  is driven by boot mode. IBV/OEM can customize this code for their specific
  policy action.

  @param DriverOptionList - The header of the driver option link list
  @param BootOptionList   - The header of the boot option link list
  @param ProcessCapsules  - A pointer to ProcessCapsules()
  @param BaseMemoryTest   - A pointer to BaseMemoryTest()
**/
VOID
EFIAPI
PlatformBootManagerAfterConsole (
  VOID
  )
{
  EFI_STATUS                    Status;
  EFI_BOOT_MODE                 LocalBootMode;

  DEBUG ((DEBUG_INFO, "PlatformBootManagerAfterConsole\n"));

  //
  // Get current Boot Mode
  //
  LocalBootMode = gBootMode;
  DEBUG ((DEBUG_INFO, "PlatformBootManagerAfterConsole: BootMode = %x\n", gBootMode));

  //
  // Logo show
  //
  BootLogoEnableLogo ();

  //
  // Go the different platform policy with different boot mode
  // Notes: this part code can be change with the table policy
  //
  switch (LocalBootMode) {


  case BOOT_ASSUMING_NO_CONFIGURATION_CHANGES:
  case BOOT_WITH_MINIMAL_CONFIGURATION:
  case BOOT_ON_S4_RESUME:
    //
    // Perform some platform specific connect sequence
    //
    PERF_START_EX(NULL,"EventRec", NULL, AsmReadTsc(), 0x7050);
    ConnectSequence (LocalBootMode);
    PERF_END_EX(NULL,"EventRec", NULL, AsmReadTsc(), 0x7051);

    break;

  case BOOT_WITH_FULL_CONFIGURATION:
  case BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS:
  case BOOT_WITH_DEFAULT_SETTINGS:
  default:
    //
    // Perform some platform specific connect sequence
    //
    ConnectSequence (LocalBootMode);

    //
    // Only in Full Configuration boot mode we do the enumeration of boot device
    //
    //
    // Dispatch all but Storage Oprom explicitly, because we assume Int13Thunk driver is there.
    //
    EfiBootManagerRefreshAllBootOption ();

    //
    // PXE boot option may appear after boot option enumeration
    //

    break;
  }

  //
  // Use a DynamicHii type pcd to save the boot status, which is used to
  // control configuration mode, such as FULL/MINIMAL/NO_CHANGES configuration.
  //
  DEBUG ((DEBUG_INFO, "PcdBootState = %d\n", PcdGetBool(PcdBootState)));
  if (PcdGetBool(PcdBootState)) {
    Status = PcdSetBoolS(PcdBootState, FALSE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Set PcdBootState to FALSE failed.\n"));
    }
    DEBUG ((DEBUG_INFO, "PcdBootState = %d\n", PcdGetBool(PcdBootState)));
  }

  Print (L"Press F7 for BootMenu!\n");

  EfiBootManagerRefreshAllBootOption ();
  if (IsNeedSortBootOption()) {
    EfiBootManagerSortLoadOptionVariable (LoadOptionTypeBoot, CompareBootOption);
  }
}

/**
  The function is called when no boot option could be launched,
  including platform recovery options and options pointing to applications
  built into firmware volumes.

  If this function returns, BDS attempts to enter an infinite loop.
**/
VOID
EFIAPI
PlatformBootManagerUnableToBoot (
  VOID
  )
{
  EFI_STATUS                   Status;
  EFI_BOOT_MANAGER_LOAD_OPTION BootDeviceList;
  CHAR16                       OptionName[sizeof ("Boot####")];

  if (mBootMenuOptionNumber == LoadOptionNumberUnassigned) {
    return;
  }
  UnicodeSPrint (OptionName, sizeof (OptionName), L"Boot%04x", mBootMenuOptionNumber);
  Status = EfiBootManagerVariableToLoadOption (OptionName, &BootDeviceList);
  if (EFI_ERROR (Status)) {
    return;
  }
  for (;;) {
    EfiBootManagerBoot (&BootDeviceList);
  }
}
