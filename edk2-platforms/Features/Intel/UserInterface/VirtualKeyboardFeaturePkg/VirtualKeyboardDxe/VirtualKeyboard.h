/** @file
  Header file for Virtual Keyboard driver.

  Copyright (c) 2012 - 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _VIRTUAL_KEYBOARD_H_
#define _VIRTUAL_KEYBOARD_H_

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/HiiLib.h>
#include <Protocol/AbsolutePointer.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleTextIn.h>
#include <Protocol/SimpleTextInEx.h>
#include <Protocol/SimpleTextOut.h>
#include <Protocol/HiiDatabase.h>
#include <Protocol/HiiImageEx.h>
#include <Protocol/HiiPackageList.h>
#include <Guid/ConsoleInDevice.h>
#include "ComponentName.h"

//
// Global Variables
//
extern EFI_DRIVER_BINDING_PROTOCOL   gVirtualKeyboardDriverBinding;
extern EFI_COMPONENT_NAME_PROTOCOL   gVirtualKeyboardComponentName;
extern EFI_COMPONENT_NAME2_PROTOCOL  gVirtualKeyboardComponentName2;

///
/// Debug raw data points
///
#define DEBUG_VK_POINTS               DEBUG_INFO

///
/// Debug data point scaling
///
#define DEBUG_VK_POINT_SCALING        DEBUG_INFO

///
/// Debug key press
///
#define DEBUG_VK_KEYS                 DEBUG_INFO

///
/// Debug routine entry and exit
///
#define DEBUG_VK_ROUTINE_ENTRY_EXIT   DEBUG_VERBOSE

///
/// Display the graphics info
///
#define DEBUG_VK_GRAPHICS_INFO        DEBUG_INFO

///
/// Display the timer entry and exit
///
#define DEBUG_VK_TIMER_ENTRY_EXIT     DEBUG_VERBOSE

///
/// Signature
///
#define VK_SIGNATURE                  SIGNATURE_32 ('V', 'K', 'e', 'y')
#define VK_NOTIFY_SIGNATURE           SIGNATURE_32 ('V', 'K', 'n', 's')

///
/// Poll interval
///
#define VK_POLL_INTERVAL              (1000 * 1000)

///
/// Define the touch timeout in poll intervals
///
#define VK_REPEAT_TIMEOUT             5

///
/// TPL used to synchronize add/remove from list
///
#define TPL_VK_SYNC                   TPL_NOTIFY

///
/// Dimension of an array ( number of elements )
///
#define DIM(x)      ( sizeof ( x ) / sizeof ( x [ 0 ]))

///
/// Define Key buffer
///
#define MAX_KEY_BUF_SIZE 64

///
/// Define Transparent Weight
///
#define TRANSPARENCY_WEIGHT 50

typedef struct _VK_CONTEXT VK_CONTEXT;

typedef enum _VK_KEY_TYPE {
  VkKeyNull         = 0x0000 | CHAR_NULL,
  VkKeyBackspace    = 0x0000 | CHAR_BACKSPACE,
  VkKeyTab          = 0x0000 | CHAR_TAB,
  VkKeyEnter        = 0x0000 | CHAR_CARRIAGE_RETURN,
  VkKeyScanMask     = 0x1000,
  VkKeyEsc          = 0x1000 | SCAN_ESC,
  VkKeyLeft         = 0x1000 | SCAN_LEFT,
  VkKeyRight        = 0x1000 | SCAN_RIGHT,
  VkKeyUp           = 0x1000 | SCAN_UP,
  VkKeyDown         = 0x1000 | SCAN_DOWN,
  VkKeyF1           = 0x1000 | SCAN_F1,
  VkKeyF2           = 0x1000 | SCAN_F2,
  VkKeyF3           = 0x1000 | SCAN_F3,
  VkKeyF4           = 0x1000 | SCAN_F4,
  VkKeyF5           = 0x1000 | SCAN_F5,
  VkKeyF6           = 0x1000 | SCAN_F6,
  VkKeyF7           = 0x1000 | SCAN_F7,
  VkKeyF8           = 0x1000 | SCAN_F8,
  VkKeyF9           = 0x1000 | SCAN_F9,
  VkKeyF10          = 0x1000 | SCAN_F10,
  VkKeyF11          = 0x1000 | SCAN_F11,
  VkKeyF12          = 0x1000 | SCAN_F12,
  VkKeySpecificMask = 0x2000,
  VkKeyShift        = 0x2000 | 0x0000,
  VkKeyCapslock     = 0x2000 | 0x0001,
  VkKeyTwoPage      = 0x2000 | 0x0002,
  VkKeyTypeMaximum  = 0xFFFF
} VK_KEY_TYPE;

typedef enum _VK_PAGE_TYPE {
  //
  // +---+---+---+---+---+---+---+---+---+---+---+
  // | q | w | e | r | t | y | u | i | o | p |<X|| Line 0
  // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  // | | a | s | d | f | g | h | j | k | l | F2| | Line 1
  // +-+---+---+---+---+---+---+---+---+---+---+-+
  // |Caps | z | x | c | v | b | n | m |aU |Enter| Line 2
  // +-----+---+---+---+---+---+---+---+---+---+-+
  // | Esc |12#|       Space       |aL |aD |aR | | Line 3
  // +-----+---+-------------------+---+---+---+-+
  //
  VkPage0,

  //
  // +---+---+---+---+---+---+---+---+---+---+---+
  // | Q | W | E | R | T | Y | U | I | O | P |<X|| Line 0
  // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  // | | A | S | D | F | G | H | J | K | L | F2| | Line 1
  // +-+---+---+---+---+---+---+---+---+---+---+-+
  // |Caps | Z | X | C | V | B | N | M |aU |Enter| Line 2
  // +-----+---+---+---+---+---+---+---+---+---+-+
  // | Esc |12#|       Space       |aL |aD |aR | | Line 3
  // +-----+---+-------------------+---+---+---+-+
  //
  VkPage1,

  //
  // +---+---+---+---+---+---+---+---+---+---+---+
  // | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 0 |<X|| Line 0
  // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  // | |F1 |F2 |F3 |F4 |F5 |F6 |F7 |F8 |F9 |F10| | Line 1
  // +-+---+---+---+---+---+---+---+---+---+---+-+
  // |Shift| . | ; | ' | , | . | / |F11|F12|Enter| Line 2
  // +-----+---+---+---+---+---+---+---+---+---+-+
  // | Esc |12#|       Space       | \ | - | = | | Line 3
  // +-----+---+-------------------+---+---+---+-+
  //
  VkPage2,

  //
  // +---+---+---+---+---+---+---+---+---+---+---+
  // | ! | @ | # | $ | % | ^ | & | * | ( | ) |<X|| Line 0
  // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  // | |F1 |F2 |F3 |F4 |F5 |F6 |F7 |F8 |F9 |F10| | Line 1
  // +-+---+---+---+---+---+---+---+---+---+---+-+
  // |Shift| ~ | : | " | < | > | ? |F11|F12|Enter| Line 2
  // +-----+---+---+---+---+---+---+---+---+---+-+
  // | Esc |12#|       Space       | | | _ | + | | Line 3
  // +-----+---+-------------------+---+---+---+-+
  //
  VkPage3,
  VkPageMaximum
} VK_PAGE_TYPE;

typedef enum VK_DISPLAY_ATTRIBUTE {
  VkDisplayAttributeNone,         /// No keyboard displayed
  VkDisplayAttributeFullTop,      /// Full keyboard display at top
  VkDisplayAttributeFullBottom,   /// Full keyboard display at bottom
  VkDisplayAttributeSimpleTop,    /// Simple keyboard display at top
  VkDisplayAttributeSimpleBottom, /// Simple keyboard display at bottom
  VkDisplayAttributeMaximum
} VK_DISPLAY_ATTRIBUTE;

typedef struct _VK_STRUCT {
  UINT16      DisStartX;
  UINT16      DisStartY;
  UINT16      DisEndX;
  UINT16      DisEndY;
  VK_KEY_TYPE PageFont[VkPageMaximum];
} VK_STRUCT;

typedef struct _VK_NOTIFY {
  UINTN                               Signature;
  EFI_KEY_DATA                        KeyData;
  EFI_KEY_NOTIFY_FUNCTION             KeyNotificationFn;
  LIST_ENTRY                          NotifyEntry;
} VK_NOTIFY;

///
/// Virtual Keyboard context
///
struct _VK_CONTEXT {
  ///
  /// Structure identification
  ///
  UINTN                             Signature;

  ///
  /// Controller Handle
  ///
  EFI_HANDLE                        Controller;

  ///
  /// Upper level API
  ///
  EFI_SIMPLE_TEXT_INPUT_PROTOCOL    SimpleTextIn;

  ///
  /// Simple Text In EX
  ///
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL SimpleTextInEx;

  ///
  /// Lower level APIs
  ///
  EFI_ABSOLUTE_POINTER_PROTOCOL     *AbsolutePointer;
  EFI_GRAPHICS_OUTPUT_PROTOCOL      *GraphicsOutput;

  ///
  /// Flag when the last poll indicated a touch event
  ///
  BOOLEAN                           TouchActive;

  ///
  /// Time to poll for touch input
  ///
  EFI_EVENT                         TimerEvent;

  ///
  /// HII handle to get image data used
  ///
  EFI_HII_HANDLE                    HiiHandle;
  EFI_HII_IMAGE_EX_PROTOCOL         *HiiImageEx;

  ///
  /// Keyboard body background buffer information
  ///
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL     *VkBodyBackgroundBltBuffer;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL     *VkBodyCompoundBltBuffer;
  UINTN                             VkBodyBltSize;
  UINTN                             VkBodyBltStartX;
  UINTN                             VkBodyBltStartY;
  UINTN                             VkBodyBltHeight;
  UINTN                             VkBodyBltWidth;
  BOOLEAN                           IsBackgroundChanged;

  ///
  /// Icon buffer information
  ///
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL     *IconBltBuffer;
  UINTN                             IconBltSize;
  UINTN                             IconBltHeight;
  UINTN                             IconBltWidth;

  ///
  /// Full icon background buffer information
  ///
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL     *FullIconBackBuffer;
  UINTN                             FullIconBackStartX;
  UINTN                             FullIconBackStartY;
  UINTN                             FullIconBackHeight;
  UINTN                             FullIconBackWidth;
  UINTN                             FullIconBackSize;
  BOOLEAN                           FullIconUpdatedFlag;

  ///
  /// Simple icon background buffer information
  ///
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL     *SimIconBackBuffer;
  UINTN                             SimIconBackStartX;
  UINTN                             SimIconBackStartY;
  UINTN                             SimIconBackHeight;
  UINTN                             SimIconBackWidth;
  UINTN                             SimIconBackSize;
  BOOLEAN                           SimIconUpdatedFlag;

  ///
  /// Small Icon
  ///
  EFI_IMAGE_INPUT                   *SmallIcon;

  ///
  /// Full Icon
  ///
  EFI_IMAGE_INPUT                   *FullIcon;

  ///
  /// Simple Key body
  ///
  EFI_IMAGE_INPUT                   *SimKeyBody;

  ///
  /// Digital key body
  ///
  EFI_IMAGE_INPUT                   *DigKeyBody;

  ///
  /// Capital Letter Key board
  ///
  EFI_IMAGE_INPUT                   *CapLeKeyBody;

  ///
  /// Screen check buffer.
  /// This is used to check if screen is kept scrolling up.
  ///
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL     *ScreenCheckBuffer;
  UINTN                             ScreenCheckBufferSize;

  ///
  /// Key state
  ///
  BOOLEAN                           KeyPressed;

  ///
  /// Keyboard display status
  ///
  VK_DISPLAY_ATTRIBUTE              CurrentKeyboardDisplay;
  VK_DISPLAY_ATTRIBUTE              TargetKeyboardDisplay;

  ///
  /// Keyboard icon display status
  ///
  BOOLEAN                           IsIconShowed;
  UINT8                             IconReDrawCheck;

  ///
  /// Keyboard body Image address
  /// Size of KeyboardBodyPtr must larger than mFullKeyboardBody
  ///
  UINT32                            NumOfKeysInfo;
  VK_STRUCT                         KeyboardBodyPtr[50];

  ///
  /// KeyBuffer
  ///
  EFI_EVENT                         KeyNotifyProcessEvent;
  EFI_KEY_TOGGLE_STATE              KeyToggleState;
  EFI_KEY_DATA                      Keybuffer[MAX_KEY_BUF_SIZE];
  UINT8                             KeyStartIndex;
  UINT8                             KeyEndIndex;
  UINT16                            KeyTouchedTimeOut;
  BOOLEAN                           IsShiftKeyFlag;
  BOOLEAN                           IsCapsLockFlag;
  BOOLEAN                           IsSupportPartialKey;
  BOOLEAN                           IsRedrawUpdateUI;
  VK_PAGE_TYPE                      PageNumber;
  LIST_ENTRY                        NotifyList;
};

///
/// Locate VK_CONTEXT from protocol
///
#define VK_CONTEXT_FROM_PROTOCOL(a)                CR (a, VK_CONTEXT, SimpleTextIn, VK_SIGNATURE)
#define VK_CONTEXT_FROM_SIMPLETEXTINEX_PROTOCOL(a) CR (a, VK_CONTEXT, SimpleTextInEx, VK_SIGNATURE)
#define VK_CONTEXT_FROM_VKBD_PROTOCOL(a)           CR (a, VK_CONTEXT, VkbdProtocol, VK_SIGNATURE)

/**
  Start the virtual keyboard driver

  This routine allocates the necessary resources for the driver.

  This routine is called by VirtualKeyboardDriverStart to complete the driver
  initialization.

  @param[in, out] VkContext        Address of an VK_CONTEXT structure
  @param[in] Controller            Handle of device to work with.

  @retval EFI_SUCCESS              Driver API properly initialized

**/
EFI_STATUS
VkApiStart (
  IN OUT VK_CONTEXT *VkContext,
  IN EFI_HANDLE Controller
  );

/**
  Stop the virtual keyboard driver

  This routine releases the resources allocated by VKApiStart.

  This routine is called by VirtualKeyboardDriverStop to initiate the driver
  shutdown.

  @param[in] VkContext        Address of an VK_CONTEXT structure

**/
VOID
VkApiStop (
  IN VK_CONTEXT *VkContext
  );

/**
  Resets the input device hardware.

  The Reset() function resets the input device hardware. As part
  of initialization process, the firmware/device will make a quick
  but reasonable attempt to verify that the device is functioning.
  If the ExtendedVerification flag is TRUE the firmware may take
  an extended amount of time to verify the device is operating on
  reset. Otherwise the reset operation is to occur as quickly as
  possible. The hardware verification process is not defined by
  this specification and is left up to the platform firmware or
  driver to implement.

  @param[in] This                 A pointer to the EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL instance.

  @param[in] ExtendedVerification Indicates that the driver may perform a more exhaustive
                                  verification operation of the device during reset.

  @retval EFI_SUCCESS             The device was reset.
  @retval EFI_DEVICE_ERROR        The device is not functioning correctly and could not be reset.

**/
EFI_STATUS
EFIAPI
VkKeyboardReset (
  IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
  IN BOOLEAN                        ExtendedVerification
  );

/**
  Reads the next keystroke from the input device. The WaitForKey Event can
  be used to test for existence of a keystroke via WaitForEvent () call.

  @param[in]  This         Protocol instance pointer.
  @param[out] Key          Driver may perform diagnostics on reset.

  @retval EFI_SUCCESS      The keystroke information was returned.
  @retval EFI_NOT_READY    There was no keystroke data available.
  @retval EFI_DEVICE_ERROR The keystroke information was not returned due to
                           hardware errors.

**/
EFI_STATUS
EFIAPI
VkKeyboardReadKeyStroke (
  IN  EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
  OUT EFI_INPUT_KEY                  *Key
  );

/**
  Resets the input device hardware.

  The Reset() function resets the input device hardware. As part
  of initialization process, the firmware/device will make a quick
  but reasonable attempt to verify that the device is functioning.
  If the ExtendedVerification flag is TRUE the firmware may take
  an extended amount of time to verify the device is operating on
  reset. Otherwise the reset operation is to occur as quickly as
  possible. The hardware verification process is not defined by
  this specification and is left up to the platform firmware or
  driver to implement.

  @param[in] This                 A pointer to the EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL instance.

  @param[in] ExtendedVerification Indicates that the driver may perform a more exhaustive
                                  verification operation of the device during reset.

  @retval EFI_SUCCESS             The device was reset.
  @retval EFI_DEVICE_ERROR        The device is not functioning correctly and could not be reset.

**/
EFI_STATUS
EFIAPI
VkKeyboardResetEx (
  IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
  IN BOOLEAN                           ExtendedVerification
  );

/**
  Reads the next keystroke from the input device.

  @param[in]  This               Protocol instance pointer.
  @param[out] KeyData            A pointer to a buffer that is filled in with the keystroke
                                 state data for the key that was pressed.

  @retval EFI_SUCCESS            The keystroke information was returned.
  @retval EFI_NOT_READY          There was no keystroke data available.
  @retval EFI_INVALID_PARAMETER  This or KeyData is NULL.

**/
EFI_STATUS
EFIAPI
VkKeyboardReadKeyStrokeEx (
  IN  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
  OUT EFI_KEY_DATA                      *KeyData
  );

/**
  Set certain state for the input device.

  @param[in] This                 Protocol instance pointer.
  @param[in] KeyToggleState       A pointer to the EFI_KEY_TOGGLE_STATE to set the
                                  state for the input device.

  @retval EFI_SUCCESS             The device state was set appropriately.
  @retval EFI_INVALID_PARAMETER   This or KeyToggleState is NULL.

**/
EFI_STATUS
EFIAPI
VkKeyboardSetState (
  IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
  IN EFI_KEY_TOGGLE_STATE              *KeyToggleState
  );

/**
  Register a notification function for a particular keystroke for the input device.

  @param[in]  This                        Protocol instance pointer.
  @param[in]  KeyData                     A pointer to a buffer that is filled in with the keystroke
                                          information data for the key that was pressed.
  @param[in]  KeyNotificationFunction     Points to the function to be called when the key
                                          sequence is typed specified by KeyData.
  @param[out] NotifyHandle                Points to the unique handle assigned to the registered notification.

  @retval EFI_SUCCESS                     The notification function was registered successfully.
  @retval EFI_OUT_OF_RESOURCES            Unable to allocate resources for necessary data structures.
  @retval EFI_INVALID_PARAMETER           KeyData or NotifyHandle or KeyNotificationFunction is NULL.

**/
EFI_STATUS
EFIAPI
VkKeyboardRegisterKeyNotify (
  IN  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
  IN  EFI_KEY_DATA                      *KeyData,
  IN  EFI_KEY_NOTIFY_FUNCTION           KeyNotificationFunction,
  OUT EFI_HANDLE                        *NotifyHandle
  );

/**
  Remove a registered notification function from a particular keystroke.

  @param[in] This                Protocol instance pointer.
  @param[in] NotificationHandle  The handle of the notification function being unregistered.

  @retval EFI_SUCCESS            The notification function was unregistered successfully.
  @retval EFI_INVALID_PARAMETER  The NotificationHandle is invalid

**/
EFI_STATUS
EFIAPI
VkKeyboardUnregisterKeyNotify (
  IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
  IN EFI_HANDLE                        NotificationHandle
  );

/**
  Draw key board on the display

  @param[in] VkContext          Graphic Protocol for draw the alphabet.

  @retval EFI_SUCCESS           Draw keyboard was done.
  @retval EFI_UNSUPPORTED       Did not get key mapping table.

**/
EFI_STATUS
DrawKeyboardLayout (
  IN VK_CONTEXT *VkContext
  );

/**
  Clear the keyboard body

  @param[in] VkContext          Code context.

  @retval EFI_SUCCESS           Clear rectangle is done.

**/
EFI_STATUS
HideVkBody (
  IN VK_CONTEXT *VkContext
  );

/**
  Clear the keyboard icon

  @param[in] VkContext          Code context.

  @retval EFI_SUCCESS           Clear rectangle is done.

**/
EFI_STATUS
HideVkIcon (
  IN VK_CONTEXT *VkContext
  );

/**
  Use to draw the keyboard icon.

  @param[in] VkContext  Pointer to virtual keyboard's context
  @param[in] VkImage    Image of keyboard to display on the screen.
  @param[in] Attribute  Attribute of keyboard to display on the screen.

  @retval EFI_SUCCESS           ConsoleControl has been flipped to graphics and keyboard icon displayed.
  @retval EFI_UNSUPPORTED       KeyboardFile not found
  @retval EFI_INVALID_PARAMETER Attribute is unknown.

**/
EFI_STATUS
EFIAPI
DrawVkIcon (
  IN VK_CONTEXT                    *VkContext,
  IN EFI_IMAGE_INPUT               *VkImage,
  IN VK_DISPLAY_ATTRIBUTE          Attribute
  );

/**
  Use to draw the keyboard.

  @param[in] VkContext  Pointer to virtual keyboard's context
  @param[in] VkImage    Image of keyboard to display on the screen.
  @param[in] Attribute  Attribute of keyboard to display on the screen.

  @retval EFI_SUCCESS           ConsoleControl has been flipped to graphics and keyboard displayed.
  @retval EFI_UNSUPPORTED       KeyboardFile not found
  @retval EFI_INVALID_PARAMETER Attribute is unknown.

**/
EFI_STATUS
EFIAPI
DrawVkBody (
  IN VK_CONTEXT                    *VkContext,
  IN EFI_IMAGE_INPUT               *VkImage,
  IN VK_DISPLAY_ATTRIBUTE          Attribute
  );

/**
  Get unicode by VkContext->PageNumber and VkContext->KeyboardBodyPtr.

  @param[in]  VkContext            Address of an VK_CONTEXT structure.
  @param[in]  KeyItem              Key Item.
  @param[out] FontPtr              Follow VkContext->PageNumber to translate
                                   font unicode.

  @retval EFI_SUCCESS              Finish translating FontPtr.
  @retval EFI_INVALID_PARAMETER    VkContext or FontPtr is NULL.

**/
EFI_STATUS
VkGetMappingFont (
  IN  VK_CONTEXT *VkContext,
  IN  VK_STRUCT  KeyItem,
  OUT UINT32     *FontPtr
  );

/**
  This routine is used to check if icon has been cleared.

  @param[in] VkContext  Pointer to virtual keyboard's context

  @retval EFI_SUCCESS   Function completed.

**/
EFI_STATUS
CheckIconCleared (
  IN VK_CONTEXT *VkContext
  );

/**
  ConvertCoordinate - Convert the touch panel's coordinate to display's coordinate.

  @param[in]  VkContext             Virtual Keyboard context.
  @param[in]  Point                 The coordinate reported from touch panel.
  @param[out] TouchX                The coordinate X converted to display panel.
  @param[out] TouchY                The coordinate Y converted to display panel..

  @retval EFI_SUCCESS               Convert success.

**/
EFI_STATUS
ConvertCoordinate (
  IN  VK_CONTEXT                 *VkContext,
  IN  EFI_ABSOLUTE_POINTER_STATE Point,
  OUT UINT32                     *TouchX,
  OUT UINT32                     *TouchY
  );

/**
  This routine is used to check if screen has been cleared.

  @param[in] VkContext  Pointer to virtual keyboard's context

  @retval EFI_SUCCESS   Function completed.

**/
EFI_STATUS
CheckScreenCleared (
  IN VK_CONTEXT *VkContext
  );

/**
  This routine is used to check if background beneath virtual keyboard has been cleared.

  @param[in] VkContext  Pointer to virtual keyboard's context

  @retval EFI_SUCCESS   Function completed.

**/
EFI_STATUS
CheckBackgroundChanged (
  IN VK_CONTEXT *VkContext
  );

/**
  To prevent screen keyboard layout occur scroll up

  @param[in, out]  VkContext            Address of an VK_CONTEXT structure.

**/
VOID
PreventScreenScrollUp (
  IN OUT VK_CONTEXT *VkContext
  );

/**
  Set the position of character.

  @param[in] VkContext      Address of an VK_CONTEXT structure.
  @param[in] DestX          X position.
  @param[in] DestY          Y position.

  @retval EFI_SUCCESS       Success for the function.
  @retval Others            An unexpected error occurred.

**/
EFI_STATUS
SetCharacterPosition (
  IN VK_CONTEXT *VkContext,
  IN UINT32     DestX,
  IN UINT32     DestY
  );

/**
  Set the keyboard layout.

  @param[in] VkContext          Graphic Protocol for draw the alphabet.
  @param[in] Index              The layout selected.

  @retval EFI_SUCCESS           Draw keyboard was done.
  @retval Others                An unexpected error occurred.

**/
EFI_STATUS
KeyboardLayoutHandler (
  IN VK_CONTEXT *VkContext,
  IN UINT32     Index
  );

/**
  Save the background blt buffer.

  @param[in] VkContext           Address of an VK_CONTEXT structure.
  @param[in] BltSize             Size of blt.

  @retval EFI_SUCCESS            Success for the function.
  @retval EFI_OUT_OF_RESOURCES   Allocate memory failed.

**/
EFI_STATUS
EFIAPI
SaveVkBodyBackgroundBltBuffer (
  IN VK_CONTEXT *VkContext,
  IN UINTN      BltSize
  );

/**
  Restore the background blt buffer.

  @param[in] VkContext           Address of an VK_CONTEXT structure.

  @retval EFI_SUCCESS            Success for the function.
  @retval EFI_UNSUPPORTED        Input blt buffer is NULL.
  @retval Others                 An unexpected error occurred.

**/
EFI_STATUS
EFIAPI
RestoreVkBodyBackgroundBltBuffer (
  IN VK_CONTEXT *VkContext
  );

/**
  Save the icon background blt buffer.

  @param[in] VkContext      Address of an VK_CONTEXT structure.
  @param[in] IconType       The icon type.

  @retval EFI_SUCCESS            Success for the function.
  @retval EFI_OUT_OF_RESOURCES   Allocate memory failed.
  @retval Others                 An unexpected error occurred.

**/
EFI_STATUS
EFIAPI
SaveVkIconBackgroundBltBuffer (
  IN VK_CONTEXT           *VkContext,
  IN VK_DISPLAY_ATTRIBUTE IconType
  );
#endif
