/** @file
 *
 *  Copyright (c) 2018, Pete Batard <pete@akeo.ie>
 *  Copyright (c) 2017-2018, Andrei Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) 2016, Linaro Ltd. All rights reserved.
 *  Copyright (c) 2015-2016, Red Hat, Inc.
 *  Copyright (c) 2014-2020, ARM Ltd. All rights reserved.
 *  Copyright (c) 2004-2016, Intel Corporation. All rights reserved.
 *  Copyright (c) 2021, Semihalf All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/BootLogoLib.h>
#include <Library/CapsuleLib.h>
#include <Library/DevicePathLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Protocol/BootManagerPolicy.h>
#include <Protocol/DevicePath.h>
#include <Protocol/EsrtManagement.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/LoadedImage.h>
#include <Guid/BootDiscoveryPolicy.h>
#include <Guid/EventGroup.h>
#include <Guid/TtyTerm.h>

#include "PlatformBm.h"

#define BOOT_PROMPT L"ESC (setup), F1 (shell), ENTER (boot)"

#define DP_NODE_LEN(Type) { (UINT8)sizeof (Type), (UINT8)(sizeof (Type) >> 8) }

#pragma pack (1)
typedef struct {
  VENDOR_DEVICE_PATH         SerialDxe;
  UART_DEVICE_PATH           Uart;
  VENDOR_DEFINED_DEVICE_PATH TermType;
  EFI_DEVICE_PATH_PROTOCOL   End;
} PLATFORM_SERIAL_CONSOLE;
#pragma pack ()

typedef struct {
  VENDOR_DEVICE_PATH         Custom;
  USB_DEVICE_PATH            Hub;
  USB_DEVICE_PATH            Dev;
  EFI_DEVICE_PATH_PROTOCOL   EndDevicePath;
} PLATFORM_USB_DEV;

typedef struct {
  VENDOR_DEVICE_PATH         Custom;
  EFI_DEVICE_PATH_PROTOCOL   EndDevicePath;
} PLATFORM_SD_DEV;

#define ARASAN_MMC_DXE_FILE_GUID  \
  { 0x100c2cfa, 0xb586, 0x4198, { 0x9b, 0x4c, 0x16, 0x83, 0xd1, 0x95, 0xb1, 0xda } }

#define SDHOST_MMC_DXE_FILE_GUID  \
  { 0x58abd787, 0xf64d, 0x4ca2, { 0xa0, 0x34, 0xb9, 0xac, 0x2d, 0x5a, 0xd0, 0xcf } }

#define SERIAL_DXE_FILE_GUID      \
  { 0xD3987D4B, 0x971A, 0x435F, { 0x8C, 0xAF, 0x49, 0x67, 0xEB, 0x62, 0x72, 0x41 } }

STATIC PLATFORM_SD_DEV mArasan = {
  //
  // VENDOR_DEVICE_PATH ArasanMMCHostDxe
  //
  {
    { HARDWARE_DEVICE_PATH, HW_VENDOR_DP, DP_NODE_LEN (VENDOR_DEVICE_PATH) },
    ARASAN_MMC_DXE_FILE_GUID
  },

  //
  // EFI_DEVICE_PATH_PROTOCOL End
  //
  {
    END_DEVICE_PATH_TYPE, END_ENTIRE_DEVICE_PATH_SUBTYPE,
    DP_NODE_LEN (EFI_DEVICE_PATH_PROTOCOL)
  }
};

STATIC PLATFORM_SD_DEV mSDHost = {
  //
  // VENDOR_DEVICE_PATH SdHostDxe
  //
  {
    { HARDWARE_DEVICE_PATH, HW_VENDOR_DP, DP_NODE_LEN (VENDOR_DEVICE_PATH) },
    SDHOST_MMC_DXE_FILE_GUID
  },

  //
  // EFI_DEVICE_PATH_PROTOCOL End
  //
  {
    END_DEVICE_PATH_TYPE, END_ENTIRE_DEVICE_PATH_SUBTYPE,
    DP_NODE_LEN (EFI_DEVICE_PATH_PROTOCOL)
  }
};

STATIC PLATFORM_SERIAL_CONSOLE mSerialConsole = {
  //
  // VENDOR_DEVICE_PATH SerialDxe
  //
  {
    { HARDWARE_DEVICE_PATH, HW_VENDOR_DP, DP_NODE_LEN (VENDOR_DEVICE_PATH) },
    SERIAL_DXE_FILE_GUID
  },

  //
  // UART_DEVICE_PATH Uart
  //
  {
    { MESSAGING_DEVICE_PATH, MSG_UART_DP, DP_NODE_LEN (UART_DEVICE_PATH) },
    0,                                      // Reserved
    FixedPcdGet64 (PcdUartDefaultBaudRate), // BaudRate
    FixedPcdGet8 (PcdUartDefaultDataBits),  // DataBits
    FixedPcdGet8 (PcdUartDefaultParity),    // Parity
    FixedPcdGet8 (PcdUartDefaultStopBits)   // StopBits
  },

  //
  // VENDOR_DEFINED_DEVICE_PATH TermType
  //
  {
    {
      MESSAGING_DEVICE_PATH, MSG_VENDOR_DP,
      DP_NODE_LEN (VENDOR_DEFINED_DEVICE_PATH)
    }
    //
    // Guid to be filled in dynamically
    //
  },

  //
  // EFI_DEVICE_PATH_PROTOCOL End
  //
  {
    END_DEVICE_PATH_TYPE, END_ENTIRE_DEVICE_PATH_SUBTYPE,
    DP_NODE_LEN (EFI_DEVICE_PATH_PROTOCOL)
  }
};


#pragma pack (1)
typedef struct {
  USB_CLASS_DEVICE_PATH    Keyboard;
  EFI_DEVICE_PATH_PROTOCOL End;
} PLATFORM_USB_KEYBOARD;
#pragma pack ()

STATIC PLATFORM_USB_KEYBOARD mUsbKeyboard = {
  //
  // USB_CLASS_DEVICE_PATH Keyboard
  //
  {
    {
      MESSAGING_DEVICE_PATH, MSG_USB_CLASS_DP,
      DP_NODE_LEN (USB_CLASS_DEVICE_PATH)
    },
    0xFFFF, // VendorId: any
    0xFFFF, // ProductId: any
    3,      // DeviceClass: HID
    1,      // DeviceSubClass: boot
    1       // DeviceProtocol: keyboard
  },

  //
  // EFI_DEVICE_PATH_PROTOCOL End
  //
  {
    END_DEVICE_PATH_TYPE, END_ENTIRE_DEVICE_PATH_SUBTYPE,
    DP_NODE_LEN (EFI_DEVICE_PATH_PROTOCOL)
  }
};

STATIC EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *mSerialConProtocol;

/**
  Check if the handle satisfies a particular condition.

  @param[in] Handle      The handle to check.
  @param[in] ReportText  A caller-allocated string passed in for reporting
                         purposes. It must never be NULL.

  @retval TRUE   The condition is satisfied.
  @retval FALSE  Otherwise. This includes the case when the condition could not
                 be fully evaluated due to an error.
**/
typedef
BOOLEAN
(EFIAPI *FILTER_FUNCTION) (
  IN EFI_HANDLE   Handle,
  IN CONST CHAR16 *ReportText
  );


/**
  Process a handle.

  @param[in] Handle      The handle to process.
  @param[in] ReportText  A caller-allocated string passed in for reporting
                         purposes. It must never be NULL.
**/
typedef
VOID
(EFIAPI *CALLBACK_FUNCTION)  (
  IN EFI_HANDLE   Handle,
  IN CONST CHAR16 *ReportText
  );

/**
  Locate all handles that carry the specified protocol, filter them with a
  callback function, and pass each handle that passes the filter to another
  callback.

  @param[in] ProtocolGuid  The protocol to look for.

  @param[in] Filter        The filter function to pass each handle to. If this
                           parameter is NULL, then all handles are processed.

  @param[in] Process       The callback function to pass each handle to that
                           clears the filter.
**/
STATIC
VOID
FilterAndProcess (
  IN EFI_GUID          *ProtocolGuid,
  IN FILTER_FUNCTION   Filter         OPTIONAL,
  IN CALLBACK_FUNCTION Process
  )
{
  EFI_STATUS Status;
  EFI_HANDLE *Handles;
  UINTN      NoHandles;
  UINTN      Idx;

  Status = gBS->LocateHandleBuffer (ByProtocol, ProtocolGuid,
                  NULL /* SearchKey */, &NoHandles, &Handles);
  if (EFI_ERROR (Status)) {
    //
    // This is not an error, just an informative condition.
    //
    DEBUG ((DEBUG_VERBOSE, "%a: %g: %r\n", __FUNCTION__, ProtocolGuid,
      Status));
    return;
  }

  ASSERT (NoHandles > 0);
  for (Idx = 0; Idx < NoHandles; ++Idx) {
    CHAR16        *DevicePathText;
    STATIC CHAR16 Fallback[] = L"<device path unavailable>";

    //
    // The ConvertDevicePathToText() function handles NULL input transparently.
    //
    DevicePathText = ConvertDevicePathToText (
                       DevicePathFromHandle (Handles[Idx]),
                       FALSE, // DisplayOnly
                       FALSE  // AllowShortcuts
                     );
    if (DevicePathText == NULL) {
      DevicePathText = Fallback;
    }

    if (Filter == NULL || Filter (Handles[Idx], DevicePathText)) {
      Process (Handles[Idx], DevicePathText);
    }

    if (DevicePathText != Fallback) {
      FreePool (DevicePathText);
    }
  }
  gBS->FreePool (Handles);
}

/**
  This CALLBACK_FUNCTION retrieves the EFI_DEVICE_PATH_PROTOCOL from the
  handle, and adds it to ConOut and ErrOut.
**/
STATIC
VOID
EFIAPI
AddOutput (
  IN EFI_HANDLE   Handle,
  IN CONST CHAR16 *ReportText
  )
{
  EFI_STATUS               Status;
  EFI_DEVICE_PATH_PROTOCOL *DevicePath;

  DevicePath = DevicePathFromHandle (Handle);
  if (DevicePath == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: %s: handle %p: device path not found\n",
      __FUNCTION__, ReportText, Handle));
    return;
  }

  Status = EfiBootManagerUpdateConsoleVariable (ConOut, DevicePath, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: %s: adding to ConOut: %r\n", __FUNCTION__,
      ReportText, Status));
    return;
  }

  Status = EfiBootManagerUpdateConsoleVariable (ErrOut, DevicePath, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: %s: adding to ErrOut: %r\n", __FUNCTION__,
      ReportText, Status));
    return;
  }

  DEBUG ((DEBUG_VERBOSE, "%a: %s: added to ConOut and ErrOut\n", __FUNCTION__,
    ReportText));
}

/**
  This CALLBACK_FUNCTION attempts to connect a handle non-recursively, asking
  the matching driver to produce all first-level child handles.
**/
STATIC
VOID
EFIAPI
Connect (
  IN EFI_HANDLE   Handle,
  IN CONST CHAR16 *ReportText
  )
{
  EFI_STATUS Status;

  Status = gBS->ConnectController (
                  Handle, // ControllerHandle
                  NULL,   // DriverImageHandle
                  NULL,   // RemainingDevicePath -- produce all children
                  FALSE   // Recursive
                  );
  DEBUG ((EFI_ERROR (Status) ? EFI_D_ERROR : EFI_D_VERBOSE, "%a: %s: %r\n",
    __FUNCTION__, ReportText, Status));
}

STATIC
INTN
PlatformRegisterBootOption (
  EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  CHAR16                   *Description,
  UINT32                   Attributes
  )
{
  EFI_STATUS                        Status;
  INTN                              OptionIndex;
  EFI_BOOT_MANAGER_LOAD_OPTION      NewOption;
  EFI_BOOT_MANAGER_LOAD_OPTION      *BootOptions;
  UINTN                             BootOptionCount;

  Status = EfiBootManagerInitializeLoadOption (
             &NewOption,
             LoadOptionNumberUnassigned,
             LoadOptionTypeBoot,
             Attributes,
             Description,
             DevicePath,
             NULL,
             0
           );
  ASSERT_EFI_ERROR (Status);

  BootOptions = EfiBootManagerGetLoadOptions (&BootOptionCount, LoadOptionTypeBoot);

  OptionIndex = EfiBootManagerFindLoadOption (&NewOption, BootOptions, BootOptionCount);

  if (OptionIndex == -1) {
    Status = EfiBootManagerAddLoadOptionVariable (&NewOption, MAX_UINTN);
    ASSERT_EFI_ERROR (Status);
    OptionIndex = BootOptionCount;
  }

  EfiBootManagerFreeLoadOption (&NewOption);
  EfiBootManagerFreeLoadOptions (BootOptions, BootOptionCount);

  return OptionIndex;
}

STATIC
INTN
PlatformRegisterFvBootOption (
  CONST EFI_GUID                   *FileGuid,
  CHAR16                           *Description,
  UINT32                           Attributes
  )
{
  EFI_STATUS                        Status;
  MEDIA_FW_VOL_FILEPATH_DEVICE_PATH FileNode;
  EFI_LOADED_IMAGE_PROTOCOL         *LoadedImage;
  EFI_DEVICE_PATH_PROTOCOL          *DevicePath;
  INTN OptionIndex;

  Status = gBS->HandleProtocol (
                  gImageHandle,
                  &gEfiLoadedImageProtocolGuid,
                  (VOID**)&LoadedImage
                );
  ASSERT_EFI_ERROR (Status);

  EfiInitializeFwVolDevicepathNode (&FileNode, FileGuid);
  DevicePath = DevicePathFromHandle (LoadedImage->DeviceHandle);
  ASSERT (DevicePath != NULL);
  DevicePath = AppendDevicePathNode (DevicePath, (EFI_DEVICE_PATH_PROTOCOL*)&FileNode);
  ASSERT (DevicePath != NULL);

  OptionIndex = PlatformRegisterBootOption (DevicePath, Description, Attributes);
  FreePool (DevicePath);

  return OptionIndex;
}

STATIC
VOID
RemoveStaleBootOptions (
  VOID
)
{
  EFI_BOOT_MANAGER_LOAD_OPTION *BootOptions;
  UINTN                        BootOptionCount;
  UINTN                        Index;
  EFI_STATUS                   Status;

  BootOptions = EfiBootManagerGetLoadOptions (&BootOptionCount,
    LoadOptionTypeBoot);

  for (Index = 0; Index < BootOptionCount; ++Index) {
    EFI_DEVICE_PATH_PROTOCOL *DevicePath = BootOptions[Index].FilePath;

    if (CompareMem (&mArasan, DevicePath, GetDevicePathSize (DevicePath)) == 0) {
      if (PcdGet32 (PcdSdIsArasan) || RPI_MODEL == 4) {
        continue;
      }
    } else if (CompareMem (&mSDHost, DevicePath, GetDevicePathSize (DevicePath)) == 0) {
      if (!PcdGet32 (PcdSdIsArasan)) {
        continue;
      }
    } else {
      continue;
    }

    //
    // Delete the boot options corresponding to stale SD controllers.
    //
    Status = EfiBootManagerDeleteLoadOptionVariable (
      BootOptions[Index].OptionNumber, LoadOptionTypeBoot);
    DEBUG_CODE (
      CHAR16 *DevicePathString;

      DevicePathString = ConvertDevicePathToText(BootOptions[Index].FilePath, FALSE, FALSE);
      DEBUG ((
        EFI_ERROR (Status) ? EFI_D_WARN : EFI_D_INFO,
        "%a: removing stale Boot#%04x %s: %r\n",
        __FUNCTION__,
        (UINT32)BootOptions[Index].OptionNumber,
        DevicePathString == NULL ? L"<unavailable>" : DevicePathString,
        Status
      ));
      if (DevicePathString != NULL) {
        FreePool (DevicePathString);
      }
    );
  }

  EfiBootManagerFreeLoadOptions (BootOptions, BootOptionCount);
}

STATIC
VOID
PlatformRegisterOptionsAndKeys (
  VOID
  )
{
  EFI_STATUS                   Status;
  EFI_INPUT_KEY                Enter;
  EFI_INPUT_KEY                F1;
  EFI_INPUT_KEY                Esc;
  EFI_BOOT_MANAGER_LOAD_OPTION BootOption;
  INTN ShellOption;

  RemoveStaleBootOptions ();

  ShellOption = PlatformRegisterFvBootOption (&gUefiShellFileGuid,
                  L"UEFI Shell", 0);
  if (ShellOption != -1) {
    //
    // F1 boots Shell.
    //
    F1.ScanCode = SCAN_F1;
    F1.UnicodeChar = CHAR_NULL;
    Status = EfiBootManagerAddKeyOptionVariable (NULL, (UINT16)ShellOption, 0, &F1, NULL);
    ASSERT (Status == EFI_SUCCESS || Status == EFI_ALREADY_STARTED);
  }

  //
  // Register ENTER as CONTINUE key
  //
  Enter.ScanCode = SCAN_NULL;
  Enter.UnicodeChar = CHAR_CARRIAGE_RETURN;
  Status = EfiBootManagerRegisterContinueKeyOption (0, &Enter, NULL);
  ASSERT_EFI_ERROR (Status);

  //
  // Map ESC to Boot Manager Menu
  //
  Esc.ScanCode = SCAN_ESC;
  Esc.UnicodeChar = CHAR_NULL;
  Status = EfiBootManagerGetBootManagerMenu (&BootOption);
  ASSERT_EFI_ERROR (Status);
  Status = EfiBootManagerAddKeyOptionVariable (NULL, (UINT16)BootOption.OptionNumber, 0, &Esc, NULL);
  ASSERT (Status == EFI_SUCCESS || Status == EFI_ALREADY_STARTED);
}

STATIC VOID
SerialConPrint (
  IN CHAR16 *Text
  )
{
  if (mSerialConProtocol != NULL) {
    mSerialConProtocol->OutputString (mSerialConProtocol, Text);
  }
}

//
// BDS Platform Functions
//
/**
  Do the platform init, can be customized by OEM/IBV
  Possible things that can be done in PlatformBootManagerBeforeConsole:
  > Update console variable: 1. include hot-plug devices;
  >                          2. Clear ConIn and add SOL for AMT
  > Register new Driver#### or Boot####
  > Register new Key####: e.g.: F12
  > Signal ReadyToLock event
  > Authentication action: 1. connect Auth devices;
  >                        2. Identify auto logon user.
**/
VOID
EFIAPI
PlatformBootManagerBeforeConsole (
  VOID
  )
{
  EFI_STATUS Status;
  ESRT_MANAGEMENT_PROTOCOL *EsrtManagement;

  if (GetBootModeHob () == BOOT_ON_FLASH_UPDATE) {
    DEBUG ((DEBUG_INFO, "ProcessCapsules Before EndOfDxe ......\n"));
    Status = ProcessCapsules ();
    DEBUG ((DEBUG_INFO, "ProcessCapsules returned %r\n", Status));
  } else {
    Status = gBS->LocateProtocol (&gEsrtManagementProtocolGuid, NULL, (VOID**)&EsrtManagement);
    if (!EFI_ERROR (Status)) {
      EsrtManagement->SyncEsrtFmp ();
    }
  }

  //
  // Now add the device path of all handles with GOP on them to ConOut and
  // ErrOut.
  //
  FilterAndProcess (&gEfiGraphicsOutputProtocolGuid, NULL, AddOutput);

  //
  // Add the hardcoded short-form USB keyboard device path to ConIn.
  //
  EfiBootManagerUpdateConsoleVariable (ConIn, (EFI_DEVICE_PATH_PROTOCOL*)&mUsbKeyboard, NULL);

  //
  // Add the hardcoded serial console device path to ConIn, ConOut, ErrOut.
  //
  ASSERT (FixedPcdGet8 (PcdDefaultTerminalType) == 4);
  CopyGuid (&mSerialConsole.TermType.Guid, &gEfiTtyTermGuid);

  EfiBootManagerUpdateConsoleVariable (ConIn, (EFI_DEVICE_PATH_PROTOCOL*)&mSerialConsole, NULL);
  EfiBootManagerUpdateConsoleVariable (ConOut, (EFI_DEVICE_PATH_PROTOCOL*)&mSerialConsole, NULL);
  EfiBootManagerUpdateConsoleVariable (ErrOut, (EFI_DEVICE_PATH_PROTOCOL*)&mSerialConsole, NULL);

  //
  // Signal EndOfDxe PI Event
  //
  EfiEventGroupSignal (&gEfiEndOfDxeEventGroupGuid);

  //
  // Dispatch deferred images after EndOfDxe event and ReadyToLock installation.
  //
  EfiBootManagerDispatchDeferredImages ();

  //
  // Ensure that USB is initialized by connecting the PCI root bridge so
  // that the xHCI PCI controller gets enumerated (Pi 4) or by connecting
  // to the DesignWare USB OTG controller directly.
  FilterAndProcess (&gEfiPciRootBridgeIoProtocolGuid, NULL, Connect);
  FilterAndProcess (&gEfiUsb2HcProtocolGuid, NULL, Connect);
}

/**
  Connect device specified by BootDiscoverPolicy variable and refresh
  Boot order for newly discovered boot device.

  @retval  EFI_SUCCESS  Devices connected succesfully or connection
                        not required.
  @retval  others       Return values from GetVariable(), LocateProtocol()
                        and ConnectDeviceClass().
--*/
STATIC
EFI_STATUS
BootDiscoveryPolicyHandler (
  VOID
  )
{
  EFI_STATUS                       Status;
  UINT32                           DiscoveryPolicy;
  UINTN                            Size;
  EFI_BOOT_MANAGER_POLICY_PROTOCOL *BMPolicy;
  EFI_GUID                         *Class;

  Size = sizeof (DiscoveryPolicy);
  Status = gRT->GetVariable (
                  BOOT_DISCOVERY_POLICY_VAR,
                  &gBootDiscoveryPolicyMgrFormsetGuid,
                  NULL,
                  &Size,
                  &DiscoveryPolicy
                  );
  if (Status == EFI_NOT_FOUND) {
    Status = PcdSet32S (PcdBootDiscoveryPolicy, PcdGet32 (PcdBootDiscoveryPolicy));
    DiscoveryPolicy = PcdGet32 (PcdBootDiscoveryPolicy);
    if (Status == EFI_NOT_FOUND) {
      return EFI_SUCCESS;
    } else if (EFI_ERROR (Status)) {
      return Status;
    }
  } else if (EFI_ERROR (Status)) {
    return Status;
  }

  if (DiscoveryPolicy == BDP_CONNECT_MINIMAL) {
    return EFI_SUCCESS;
  }

  switch (DiscoveryPolicy) {
    case BDP_CONNECT_NET:
      Class = &gEfiBootManagerPolicyNetworkGuid;
      break;
    case BDP_CONNECT_ALL:
      Class = &gEfiBootManagerPolicyConnectAllGuid;
      break;
    default:
      DEBUG ((
        DEBUG_INFO,
        "%a - Unexpected DiscoveryPolicy (0x%x). Run Minimal Discovery Policy\n",
        __FUNCTION__,
        DiscoveryPolicy
        ));
      return EFI_SUCCESS;
  }

  Status = gBS->LocateProtocol (
                  &gEfiBootManagerPolicyProtocolGuid,
                  NULL,
                  (VOID **)&BMPolicy
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a - Failed to locate gEfiBootManagerPolicyProtocolGuid - %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = BMPolicy->ConnectDeviceClass (BMPolicy, Class);
  if (EFI_ERROR (Status)){
    DEBUG ((DEBUG_ERROR, "%a - ConnectDeviceClass returns - %r\n", __FUNCTION__, Status));
    return Status;
  }

  EfiBootManagerRefreshAllBootOption();

  return EFI_SUCCESS;
}

/**
  Do the platform specific action after the console is ready
  Possible things that can be done in PlatformBootManagerAfterConsole:
  > Console post action:
    > Dynamically switch output mode from 100x31 to 80x25 for certain senarino
    > Signal console ready platform customized event
  > Run diagnostics like memory testing
  > Connect certain devices
  > Dispatch aditional option roms
  > Special boot: e.g.: USB boot, enter UI
**/
VOID
EFIAPI
PlatformBootManagerAfterConsole (
  VOID
  )
{
  ESRT_MANAGEMENT_PROTOCOL      *EsrtManagement;
  EFI_STATUS                    Status;
  EFI_HANDLE SerialHandle;

  Status = EfiBootManagerConnectDevicePath ((EFI_DEVICE_PATH_PROTOCOL*)&mSerialConsole, &SerialHandle);
  if (Status == EFI_SUCCESS) {
    gBS->HandleProtocol (SerialHandle, &gEfiSimpleTextOutProtocolGuid, (VOID**)&mSerialConProtocol);
  }

  //
  // Show the splash screen.
  //
  Status = BootLogoEnableLogo ();
  if (Status == EFI_SUCCESS) {
    SerialConPrint (BOOT_PROMPT);
  } else {
    Print (BOOT_PROMPT);
  }

  Status = BootDiscoveryPolicyHandler ();
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "Error applying Boot Discovery Policy:%r\n", Status));
  }

  Status = BootDiscoveryPolicyHandler ();
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "Error applying Boot Discovery Policy:%r\n", Status));
  }

  Status = gBS->LocateProtocol (&gEsrtManagementProtocolGuid, NULL, (VOID**)&EsrtManagement);
  if (!EFI_ERROR (Status)) {
    EsrtManagement->SyncEsrtFmp ();
  }

  if (GetBootModeHob () == BOOT_ON_FLASH_UPDATE) {
    DEBUG ((DEBUG_INFO, "ProcessCapsules After EndOfDxe ......\n"));
    Status = ProcessCapsules ();
    DEBUG ((DEBUG_INFO, "ProcessCapsules returned %r\n", Status));
  }

  PlatformRegisterOptionsAndKeys ();
}

/**
  This function is called each second during the boot manager waits the
  timeout.

  @param TimeoutRemain  The remaining timeout.
**/
VOID
EFIAPI
PlatformBootManagerWaitCallback (
  IN UINT16  TimeoutRemain
  )
{
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL_UNION Black;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL_UNION White;
  UINT16                              Timeout;
  EFI_STATUS                          Status;

  Timeout = PcdGet16 (PcdPlatformBootTimeOut);

  Black.Raw = 0x00000000;
  White.Raw = 0x00FFFFFF;

  Status = BootLogoUpdateProgress (
             White.Pixel,
             Black.Pixel,
             BOOT_PROMPT,
             White.Pixel,
             (Timeout - TimeoutRemain) * 100 / Timeout,
             0
           );
  if (Status == EFI_SUCCESS) {
    SerialConPrint (L".");
  } else {
    Print (L".");
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
  EFI_INPUT_KEY                Key;
  EFI_BOOT_MANAGER_LOAD_OPTION BootManagerMenu;
  UINTN                        Index;
  EFI_BOOT_MANAGER_LOAD_OPTION *BootOptions;
  UINTN                        OldBootOptionCount;
  UINTN                        NewBootOptionCount;

  //
  // Record the total number of boot configured boot options
  //
  BootOptions = EfiBootManagerGetLoadOptions (&OldBootOptionCount,
                  LoadOptionTypeBoot);
  EfiBootManagerFreeLoadOptions (BootOptions, OldBootOptionCount);

  //
  // Connect all devices, and regenerate all boot options
  //
  EfiBootManagerConnectAll ();
  EfiBootManagerRefreshAllBootOption ();

  //
  // Record the updated number of boot configured boot options
  //
  BootOptions = EfiBootManagerGetLoadOptions (&NewBootOptionCount,
                  LoadOptionTypeBoot);
  EfiBootManagerFreeLoadOptions (BootOptions, NewBootOptionCount);

  //
  // If the number of configured boot options has changed, reboot
  // the system so the new boot options will be taken into account
  // while executing the ordinary BDS bootflow sequence.
  //
  if (NewBootOptionCount != OldBootOptionCount) {
    DEBUG ((DEBUG_WARN, "%a: rebooting after refreshing all boot options\n",
      __FUNCTION__));
    gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
  }

  //
  // BootManagerMenu doesn't contain the correct information when return status
  // is EFI_NOT_FOUND.
  //
  Status = EfiBootManagerGetBootManagerMenu (&BootManagerMenu);
  if (EFI_ERROR (Status)) {
    return;
  }
  //
  // Normally BdsDxe does not print anything to the system console, but this is
  // a last resort -- the end-user will likely not see any DEBUG messages
  // logged in this situation.
  //
  // AsciiPrint() will NULL-check gST->ConOut internally. We check gST->ConIn
  // here to see if it makes sense to request and wait for a keypress.
  //
  if (gST->ConIn != NULL) {
    AsciiPrint (
      "%a: No bootable option or device was found.\n"
      "%a: Press any key to enter the Boot Manager Menu.\n",
      gEfiCallerBaseName,
      gEfiCallerBaseName);
    Status = gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &Index);
    ASSERT_EFI_ERROR (Status);
    ASSERT (Index == 0);

    //
    // Drain any queued keys.
    //
    while (!EFI_ERROR (gST->ConIn->ReadKeyStroke (gST->ConIn, &Key))) {
      //
      // just throw away Key
      //
    }
  }

  for (;;) {
    EfiBootManagerBoot (&BootManagerMenu);
  }
}
