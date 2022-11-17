/** @file
  Virtural Keyboard Control Engine

  Copyright (c) 2012 - 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "VirtualKeyboard.h"

/**
  Notify all keyboards pressed specific key need to hidden Virtual
  Keyboard.

  @param[in] KeyData          Keyboard pressed specific key data

  @retval EFI_SUCCESS         Success for the function.
  @retval EFI_NOT_FOUND       Not found Virtual Keyboard protocol.

**/
EFI_STATUS
EFIAPI
VkNotifyKeyCallback (
  IN EFI_KEY_DATA *KeyData
  )
{
  EFI_STATUS Status;
  VK_CONTEXT *VkContext;
  UINTN      MaxColumn;
  UINTN      MaxRow;
  UINT32     HorizontalPixel;
  UINT32     VerticalPixel;

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkNotifyKeyCallback Start\n"));

  Status = gBS->LocateProtocol (&gEfiCallerIdGuid, NULL, (VOID **) &VkContext);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  gST->ConOut->QueryMode (
                 gST->ConOut,
                 gST->ConOut->Mode->Mode,
                 &MaxColumn,
                 &MaxRow
                 );
  HorizontalPixel = VkContext->GraphicsOutput->Mode->Info->HorizontalResolution;
  VerticalPixel   = VkContext->GraphicsOutput->Mode->Info->VerticalResolution;

  //
  // CursorRow and CursorColumn are started from 0, so need to add 2 not 1.
  //
  if (((UINTN)(gST->ConOut->Mode->CursorColumn * EFI_GLYPH_WIDTH) < VkContext->SimIconBackWidth) &&
      ((UINTN)((gST->ConOut->Mode->CursorRow + 2) * EFI_GLYPH_HEIGHT) > (VerticalPixel - VkContext->SimIconBackHeight))) {
    SaveVkIconBackgroundBltBuffer (VkContext, VkDisplayAttributeSimpleBottom);
  } else if (((UINTN)((gST->ConOut->Mode->CursorColumn + 2) * EFI_GLYPH_WIDTH) > (HorizontalPixel - VkContext->FullIconBackWidth)) &&
             ((UINTN)((gST->ConOut->Mode->CursorRow + 2) * EFI_GLYPH_HEIGHT) > (VerticalPixel - VkContext->FullIconBackHeight))){
    SaveVkIconBackgroundBltBuffer (VkContext, VkDisplayAttributeFullBottom);
  }

  //
  // Hide icon/keyboard to prevent screen scroll up.
  //
  if ((KeyData->Key.UnicodeChar         == CHAR_CARRIAGE_RETURN)    ||
      (KeyData->Key.ScanCode            == SCAN_ESC)                ||
      ((gST->ConOut->Mode->CursorColumn >= (INT32) (MaxColumn - 2)) &&
       (gST->ConOut->Mode->CursorRow    == (INT32) (MaxRow - 1)))) {
    HideVkBody (VkContext);
    HideVkIcon (VkContext);
    VkContext->IconReDrawCheck = 0;
  }

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkNotifyKeyCallback End\n"));
  return EFI_SUCCESS;
}

/**
  Judge whether is a registed key.

  @param[in] RegsiteredData   A pointer to a buffer that is filled in with the keystroke
                              state data for the key that was registered.
  @param[in] InputData        A pointer to a buffer that is filled in with the keystroke
                              state data for the key that was pressed.

  @retval TRUE                Key be pressed matches a registered key.
  @retval FLASE               Match failed.

**/
BOOLEAN
IsKeyRegistered (
  IN EFI_KEY_DATA  *RegsiteredData,
  IN EFI_KEY_DATA  *InputData
  )
{
  ASSERT (RegsiteredData != NULL && InputData != NULL);

  if ((RegsiteredData->Key.ScanCode    != InputData->Key.ScanCode) ||
      (RegsiteredData->Key.UnicodeChar != InputData->Key.UnicodeChar)) {
    return FALSE;
  }

  //
  // Assume KeyShiftState/KeyToggleState = 0 in Registered key data means these state could be ignored.
  //
  if (RegsiteredData->KeyState.KeyShiftState != 0 &&
      RegsiteredData->KeyState.KeyShiftState != InputData->KeyState.KeyShiftState) {
    return FALSE;
  }
  if (RegsiteredData->KeyState.KeyToggleState != 0 &&
      RegsiteredData->KeyState.KeyToggleState != InputData->KeyState.KeyToggleState) {
    return FALSE;
  }

  return TRUE;
}

/**
  Pop the key from the keybuffer.

  @param[in, out] VkContext    Address of an VK_CONTEXT structure.
  @param[out]     KeyData      The EFI stand key infomation when code get the key will put
                               correct mapping key.

  @retval EFI_SUCCESS          The keystroke information was returned.
  @retval EFI_NOT_READY        The keystroke data is empty.

**/
EFI_STATUS
VkPopTheKey (
  IN OUT VK_CONTEXT   *VkContext,
     OUT EFI_KEY_DATA *KeyData OPTIONAL
  )
{
  EFI_STATUS Status;
  UINT8      IndexSt;
  UINT8      IndexEd;

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkPopTheKey Start\n"));
  //
  // KeyBuffer
  // [         0          ]
  // [         1          ]<-*IndexSt/IndexEd
  // [         2          ]
  // [        ...         ]
  // [MAX_KEY_BUF_SIZE - 1]
  //
  Status  = EFI_SUCCESS;
  IndexSt = VkContext->KeyStartIndex;
  IndexEd = VkContext->KeyEndIndex;

  if (IndexEd == IndexSt) {
    DEBUG ((DEBUG_VK_KEYS, "ERROR - Keyboard buffer is empty.\n"));
    Status = EFI_NOT_READY;
    goto Error;
  }

  DEBUG ((DEBUG_VK_KEYS | DEBUG_INFO, "VkPopTheKey Unicode:    %08x\n", VkContext->Keybuffer[IndexSt].Key.UnicodeChar));
  DEBUG ((DEBUG_VK_KEYS | DEBUG_INFO, "VkPopTheKey ScanCode:   %08x\n", VkContext->Keybuffer[IndexSt].Key.ScanCode));
  DEBUG ((DEBUG_VK_KEYS | DEBUG_INFO, "VkPopTheKey ShiftState: %08x\n", VkContext->Keybuffer[IndexSt].KeyState.KeyShiftState));
  DEBUG ((DEBUG_VK_KEYS | DEBUG_INFO, "VkPopTheKey ToggleState:%08x\n", VkContext->Keybuffer[IndexSt].KeyState.KeyToggleState));

  if (KeyData != NULL) {
    *KeyData = VkContext->Keybuffer[IndexSt];
  }
  VkContext->KeyStartIndex = ++IndexSt % MAX_KEY_BUF_SIZE;

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkPopTheKey Success, Status: %r\n", Status));
  goto End;

Error:
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkPopTheKey Failed, Status: %r\n", Status));

End:
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkPopTheKey End\n"));
  return Status;
}

/**
  Push the event mapping key index to the input key buffer.

  @param[in, out] VkContext    Address of an VK_CONTEXT structure.
  @param[in]      Unicode      The font's unicode number.

  @retval EFI_SUCCESS          The maping key index did put the key buffer.

**/
EFI_STATUS
VkPushTheKey (
  IN OUT VK_CONTEXT *VkContext,
  IN     UINT16     Unicode
  )
{
  UINT8        IndexSt;
  UINT8        IndexEd;
  EFI_KEY_DATA KeyData;
  EFI_STATUS   Status;
  LIST_ENTRY   *Link;
  VK_NOTIFY    *CurrentNotify;

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkPushTheKey Start\n"));
  //
  // KeyBuffer
  // [         0          ]
  // [         1          ]<-IndexSt/*IndexEd
  // [         2          ]
  // [        ...         ]
  // [MAX_KEY_BUF_SIZE - 1]
  //
  IndexSt = VkContext->KeyStartIndex;
  IndexEd = VkContext->KeyEndIndex;

  Status = EFI_SUCCESS;

  //
  // Check Keyboard Buffer if is full that will pop the first key
  //
  if (VkContext->IsSupportPartialKey &&
    ((VkContext->KeyEndIndex + 2) % MAX_KEY_BUF_SIZE) == IndexSt) {
    VkPopTheKey (VkContext, NULL);
  }

  if (((VkContext->KeyEndIndex + 1) % MAX_KEY_BUF_SIZE) == IndexSt) {
    VkPopTheKey (VkContext, NULL);
  }

  KeyData.Key.ScanCode             = SCAN_NULL;
  KeyData.Key.UnicodeChar          = CHAR_NULL;
  KeyData.KeyState.KeyShiftState   = EFI_SHIFT_STATE_VALID;
  KeyData.KeyState.KeyToggleState  = VkContext->KeyToggleState;
  KeyData.KeyState.KeyToggleState &= ~EFI_CAPS_LOCK_ACTIVE;
  KeyData.KeyState.KeyToggleState |= VkContext->IsCapsLockFlag ? EFI_CAPS_LOCK_ACTIVE : 0;

  switch (Unicode) {
  case VkKeyShift:
    VkContext->IsShiftKeyFlag = !VkContext->IsShiftKeyFlag;
    if (VkContext->PageNumber >= VkPage2 && VkContext->PageNumber <= VkPage3) {
      VkContext->IsShiftKeyFlag ? VkContext->PageNumber++ : VkContext->PageNumber--;
    }
    VkContext->IsRedrawUpdateUI = TRUE;
    break;

  case VkKeyTwoPage:
    switch (VkContext->PageNumber) {
    case VkPage0:
    case VkPage1:
      VkContext->IsShiftKeyFlag = FALSE;
      VkContext->PageNumber     = VkContext->IsShiftKeyFlag ? VkPage3 : VkPage2;
      break;

    case VkPage2:
    case VkPage3:
      VkContext->IsShiftKeyFlag = FALSE;
      VkContext->PageNumber     = VkContext->IsCapsLockFlag ? VkPage1 : VkPage0;
      break;

    default:
      break;
    }
    VkContext->IsRedrawUpdateUI = TRUE;
    break;

  case VkKeyCapslock:
    VkContext->IsCapsLockFlag = !VkContext->IsCapsLockFlag;
    if (VkContext->PageNumber >= VkPage0 && VkContext->PageNumber <= VkPage1) {
      VkContext->IsCapsLockFlag ? VkContext->PageNumber++ : VkContext->PageNumber--;
    }
    KeyData.KeyState.KeyToggleState &= ~EFI_CAPS_LOCK_ACTIVE;
    KeyData.KeyState.KeyToggleState |= VkContext->IsCapsLockFlag ? EFI_CAPS_LOCK_ACTIVE : 0;
    VkContext->IsRedrawUpdateUI      = TRUE;
    break;

  default:
    if ((Unicode & VkKeyScanMask) != 0) {
      KeyData.Key.ScanCode    = Unicode & ~VkKeyScanMask;
    } else {
      KeyData.Key.UnicodeChar = Unicode;
    }
    break;
  }

  if (KeyData.Key.ScanCode == SCAN_NULL && KeyData.Key.UnicodeChar == CHAR_NULL) {
    if (!VkContext->IsSupportPartialKey) {
      goto End;
    }
  }

  for (Link = GetFirstNode (&VkContext->NotifyList); !IsNull (&VkContext->NotifyList, Link); Link = GetNextNode (&VkContext->NotifyList, Link)) {
    CurrentNotify = CR (Link, VK_NOTIFY, NotifyEntry, VK_NOTIFY_SIGNATURE);
    //
    // The key notification function needs to run at TPL_CALLBACK
    // while current TPL is TPL_NOTIFY. It will be invoked in
    // VkKeyNotifyProcessHandler() which runs at TPL_CALLBACK.
    //
    if (IsKeyRegistered (&CurrentNotify->KeyData, &KeyData)) {
      VkContext->Keybuffer[IndexEd] = KeyData;
      VkContext->KeyEndIndex        = ++IndexEd % MAX_KEY_BUF_SIZE;
      gBS->SignalEvent (VkContext->KeyNotifyProcessEvent);
    }
  }
  VkContext->Keybuffer[IndexEd] = KeyData;
  VkContext->KeyEndIndex        = ++IndexEd % MAX_KEY_BUF_SIZE;

  DEBUG ((DEBUG_VK_TIMER_ENTRY_EXIT, "VkPushTheKey Success, Status: %r\n", Status));
  goto End;

End:
  DEBUG ((DEBUG_VK_TIMER_ENTRY_EXIT, "VkPushTheKey End\n"));
  return Status;
}

/**
  Process key notify.

  @param[in] Event                 Indicates the event that invoke this function.
  @param[in] Context               Indicates the calling context.

**/
VOID
EFIAPI
VkKeyNotifyProcessHandler (
  IN  EFI_EVENT Event,
  IN  VOID      *Context
  )
{
  EFI_STATUS   Status;
  VK_CONTEXT   *VkContext;
  EFI_KEY_DATA KeyData;
  LIST_ENTRY   *Link;
  LIST_ENTRY   *NotifyList;
  VK_NOTIFY    *CurrentNotify;
  EFI_TPL      OldTpl;

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyNotifyProcessHandler Start\n"));
  VkContext = (VK_CONTEXT *) Context;
  OldTpl = gBS->RaiseTPL (TPL_NOTIFY);

  //
  // Invoke notification functions.
  //
  NotifyList = &VkContext->NotifyList;
  Status = VkPopTheKey (VkContext, &KeyData);
  if (EFI_ERROR (Status)) {
    goto Error;
  }

  for (Link = GetFirstNode (NotifyList); !IsNull (NotifyList, Link); Link = GetNextNode (NotifyList, Link)) {
    CurrentNotify = CR (Link, VK_NOTIFY, NotifyEntry, VK_NOTIFY_SIGNATURE);
    if (IsKeyRegistered (&CurrentNotify->KeyData, &KeyData)) {
      CurrentNotify->KeyNotificationFn (&KeyData);
    }
  }

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyNotifyProcessHandler Success, Status: %r\n", Status));
  goto End;

Error:
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyNotifyProcessHandler Failed, Status: %r\n", Status));

End:
  gBS->RestoreTPL (OldTpl);
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyNotifyProcessHandler End\n"));
}

/**
  Push the event mapping key index to the input key buffer.

  @param[in]  VkContext        Address of an VK_CONTEXT structure.
  @param[out] KeyItem          Key Item.
  @param[out] Index            Index of Key Item.
  @param[in]  TouchX           X position of finger touch.
  @param[in]  TouchY           Y position of finger touch.

  @retval TRUE                 Touch on Virtual Keyboard.
  @retval FALSE                Not touch on Virtual Keyboard.

**/
BOOLEAN
IsTouchVk (
  IN  VK_CONTEXT *VkContext,
  OUT VK_STRUCT  *KeyItem,
  OUT UINT32     *Index,
  IN  UINT32     TouchX,
  IN  UINT32     TouchY
  )
{
  for (*Index = 0; *Index < VkContext->NumOfKeysInfo; (*Index)++) {
    if (VkContext->KeyboardBodyPtr[*Index].DisStartX < TouchX &&
        VkContext->KeyboardBodyPtr[*Index].DisEndX   > TouchX &&
        VkContext->KeyboardBodyPtr[*Index].DisStartY < TouchY &&
        VkContext->KeyboardBodyPtr[*Index].DisEndY   > TouchY) {
      break;
    }
  }
  if (*Index != VkContext->NumOfKeysInfo) {
    *KeyItem = VkContext->KeyboardBodyPtr[*Index];
  }

  return *Index != VkContext->NumOfKeysInfo;
}

/**
  Call back function when ready to boot into OS.

  It will clear the Virtual Keyboard.

  @param[in] Event    Event whose notification function is being invoked.
  @param[in] Context  Pointer to the VkContext.

**/
VOID
EFIAPI
VkReadyToBootCallBack (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  VK_CONTEXT                    *VkContext;
  VkContext = (VK_CONTEXT *)Context;

  HideVkBody (VkContext);
  VkContext->TargetKeyboardDisplay  = VkDisplayAttributeNone;
}

/**
  Timer event

  This routine is called at TPL_VK_SYNC.

  This routine polls for touch panel input.

  @param[in] Event    Event whose notification function is being invoked.
  @param[in] Context  Pointer to the notification function's context,
                      which is implementation-dependent. Context corresponds
                      to NotifyContext in CreateEventEx().

**/
VOID
EFIAPI
VkTimer (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_ABSOLUTE_POINTER_PROTOCOL *AbsolutePointer;
  EFI_ABSOLUTE_POINTER_STATE    Point;
  EFI_STATUS                    Status;
  UINT32                        TouchX;
  UINT32                        TouchY;
  VK_CONTEXT                    *VkContext;
  UINT32                        Index;
  VK_STRUCT                     KeyItem;
  UINT32                        Font;

  DEBUG ((DEBUG_VK_TIMER_ENTRY_EXIT, "VkTimer Start\n"));
  VkContext = (VK_CONTEXT *)Context;

  //
  // Update keyboard UI layout
  //
  CheckIconCleared (VkContext);
  CheckScreenCleared (VkContext);
  CheckBackgroundChanged (VkContext);
  if (VkContext->IsRedrawUpdateUI) {
    HideVkBody (VkContext);
    DrawKeyboardLayout (VkContext);
    VkContext->IsRedrawUpdateUI = FALSE;
  }

  //
  // Error handle for invalid information
  //
  AbsolutePointer = VkContext->AbsolutePointer;
  Status = gBS->CheckEvent (AbsolutePointer->WaitForInput);
  if (EFI_ERROR (Status)) {
    if (Status != EFI_NOT_READY) {
      DEBUG ((DEBUG_VK_TIMER_ENTRY_EXIT | DEBUG_VK_POINTS, "ERROR - VkContext->AbsolutePointer->WaitForInput->CheckEvent failed! Status: %r\n", Status));
    }
    goto Error;
  }

  Status = AbsolutePointer->GetState (AbsolutePointer, &Point);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_TIMER_ENTRY_EXIT | DEBUG_VK_POINTS, "ERROR - GetState failed, Status: %r\n", Status));
    goto Error;
  }

  if (VkContext->AbsolutePointer->Mode->AbsoluteMaxX <= Point.CurrentX) {
    DEBUG ((
      DEBUG_VK_TIMER_ENTRY_EXIT | DEBUG_VK_POINTS,
      "ERROR - X value exceeds maximum: X: 0x%016Lx, MaxX: 0x%016Lx\n",
      Point.CurrentX,
      VkContext->AbsolutePointer->Mode->AbsoluteMaxX
      ));
    Status = EFI_PROTOCOL_ERROR;
    goto Error;
  }
  if (VkContext->AbsolutePointer->Mode->AbsoluteMaxY <= Point.CurrentY) {
    DEBUG ((
      DEBUG_VK_TIMER_ENTRY_EXIT | DEBUG_VK_POINTS,
      "ERROR - Y value exceeds maximum: Y: 0x%016Lx, MaxY: 0x%016Lx\n",
      Point.CurrentY,
      VkContext->AbsolutePointer->Mode->AbsoluteMaxY
      ));
    Status = EFI_PROTOCOL_ERROR;
    goto Error;
  }

  //
  // Update the touch active status
  //
  VkContext->TouchActive = ((Point.ActiveButtons & EFI_ABSP_TouchActive) != 0) ? TRUE : FALSE;
  if (!VkContext->TouchActive) {
    VkContext->KeyPressed = FALSE;
  }
  ConvertCoordinate (VkContext, Point, &TouchX, &TouchY);

  if (!VkContext->KeyPressed &&
      VkContext->TouchActive &&
      IsTouchVk (VkContext, &KeyItem, &Index, TouchX, TouchY)) {
    VkGetMappingFont (VkContext, KeyItem, &Font);
    DEBUG ((DEBUG_VK_TIMER_ENTRY_EXIT | DEBUG_INFO,                   "VK Touch event is trigger!\n" ));
    DEBUG ((DEBUG_VK_TIMER_ENTRY_EXIT | DEBUG_INFO | DEBUG_VK_POINTS, "TouchActive:            0x%04x  \n", VkContext->TouchActive));
    DEBUG ((DEBUG_VK_TIMER_ENTRY_EXIT | DEBUG_INFO | DEBUG_VK_POINTS, "CurrentKeyboardDisplay: 0x%04x  \n", VkContext->CurrentKeyboardDisplay));
    DEBUG ((DEBUG_VK_TIMER_ENTRY_EXIT | DEBUG_INFO | DEBUG_VK_POINTS, "X:                      0x%016Lx\n", Point.CurrentX));
    DEBUG ((DEBUG_VK_TIMER_ENTRY_EXIT | DEBUG_INFO | DEBUG_VK_POINTS, "Y:                      0x%016Lx\n", Point.CurrentY));
    DEBUG ((DEBUG_VK_TIMER_ENTRY_EXIT | DEBUG_INFO | DEBUG_VK_POINTS, "Z:                      0x%016Lx\n", Point.CurrentZ));
    DEBUG ((DEBUG_VK_TIMER_ENTRY_EXIT | DEBUG_INFO | DEBUG_VK_POINTS, "Buttons:                0x%08x  \n", Point.ActiveButtons));
    DEBUG ((DEBUG_VK_TIMER_ENTRY_EXIT | DEBUG_INFO | DEBUG_VK_POINTS, "MapKey Index:           0x%04x  \n", Index));
    DEBUG ((DEBUG_VK_TIMER_ENTRY_EXIT | DEBUG_INFO | DEBUG_VK_POINTS, "Key Unicode:            0x%08x  \n", Font));

    switch (Font) {
    //
    // Touch the small keyboard icon, show/hide the keyboard.
    //
    case VkKeyTypeMaximum:
      KeyboardLayoutHandler (VkContext, Index);
      break;

    //
    // Touch the key raw.
    //
    default:
      if (VkContext->CurrentKeyboardDisplay == VkDisplayAttributeNone) {
        break;
      }
      VkPushTheKey (VkContext, (UINT16) Font);
    }
    VkContext->KeyPressed = TRUE;
  }

  DEBUG ((DEBUG_VK_TIMER_ENTRY_EXIT, "VkTimer Success, Status: %r\n", Status));
  goto End;

Error:
  DEBUG ((DEBUG_VK_TIMER_ENTRY_EXIT, "VkTimer Failed, Status: %r\n", Status));

End:
  DEBUG ((DEBUG_VK_TIMER_ENTRY_EXIT, "VkTimer End\n"));
  return;
}

/**
  VkTouchWaitForKey - SignalEvent when the keybuffer has keys.

  @param[in] Event    Event whose notification function is being invoked.
  @param[in] Context  Pointer to the notification function's context,
                      which is implementation-dependent.  Context corresponds
                      to NotifyContext in CreateEventEx().

**/
VOID
EFIAPI
VkTouchWaitForKey (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  VK_CONTEXT *VkContext;

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkTouchWaitForKey Start\n"));
  VkContext = (VK_CONTEXT*) Context;

  if (VkContext->KeyStartIndex != VkContext->KeyEndIndex) {
    DEBUG ((DEBUG_VK_KEYS, "Signal VkTouchWaitForKey\n"));
    gBS->SignalEvent (Event);
  }

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkTouchWaitForKey End\n"));
}

/**
  Get image file data from BIOS image through specific ImageId.

  @param[in] VkContext      Address of an VK_CONTEXT structure.
  @param[in] ImageId        Guid to get specific image file used.

  @retval EFI_IMAGE_INPUT   Image data on BIOS image.

**/
EFI_IMAGE_INPUT*
VkGetImage (
  IN VK_CONTEXT   *VkContext,
  IN EFI_IMAGE_ID ImageId
  )
{
  EFI_STATUS      Status;
  EFI_IMAGE_INPUT Image;
  EFI_IMAGE_INPUT *VkImage;


  Status = VkContext->HiiImageEx->GetImageEx (
                                    VkContext->HiiImageEx,
                                    VkContext->HiiHandle,
                                    ImageId,
                                    &Image
                                    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
  }

  VkImage = AllocateCopyPool (sizeof (EFI_IMAGE_INPUT), &Image);
  if (VkImage == NULL) {
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return NULL;
  }

  return VkImage;
}

/**
  Dump partial debug message on VkContext.

  @param[in] VkContext        Address of an VK_CONTEXT structure.

**/
VOID
VkDumpContext (
  IN VK_CONTEXT *VkContext
  )
{
  DEBUG ((
    DEBUG_VK_ROUTINE_ENTRY_EXIT,
    "VkContext->Signature: 0x%016x\n",
    VkContext->Signature
    ));
  DEBUG ((
    DEBUG_VK_ROUTINE_ENTRY_EXIT,
    "VkContext->Controller: 0x%016x\n",
    VkContext->Controller
    ));
  DEBUG ((
    DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_VK_GRAPHICS_INFO,
    "VkContext->GraphicsOutput->Mode->MaxMode: 0x%08x\n",
    VkContext->GraphicsOutput->Mode->MaxMode
    ));
  DEBUG ((
    DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_VK_GRAPHICS_INFO,
    "VkContext->GraphicsOutput->Mode->Mode: 0x%08x\n",
    VkContext->GraphicsOutput->Mode->Mode
    ));
  DEBUG ((
    DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_VK_GRAPHICS_INFO,
    "VkContext->GraphicsOutput->Mode->Info->HorizontalResolution: 0x%08x\n",
    VkContext->GraphicsOutput->Mode->Info->HorizontalResolution
    ));
  DEBUG ((
    DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_VK_GRAPHICS_INFO,
    "VkContext->GraphicsOutput->Mode->Info->VerticalResolution: 0x%08x\n",
    VkContext->GraphicsOutput->Mode->Info->VerticalResolution
    ));
  DEBUG ((
    DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_VK_POINTS,
    "VkContext->AbsolutePointer->Mode X: 0x%016Lx - 0x%016Lx\n",
    VkContext->AbsolutePointer->Mode->AbsoluteMinX,
    VkContext->AbsolutePointer->Mode->AbsoluteMaxX
    ));
  DEBUG ((
    DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_VK_POINTS,
    "VkContext->AbsolutePointer->Mode Y: 0x%016Lx - 0x%016Lx\n",
    VkContext->AbsolutePointer->Mode->AbsoluteMinY,
    VkContext->AbsolutePointer->Mode->AbsoluteMaxY
    ));
  DEBUG ((
    DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_VK_POINTS,
    "VkContext->AbsolutePointer->Mode Z: 0x%016Lx - 0x%016Lx\n",
    VkContext->AbsolutePointer->Mode->AbsoluteMinZ,
    VkContext->AbsolutePointer->Mode->AbsoluteMaxZ
    ));
  DEBUG ((
    DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_VK_POINTS,
    "VkContext->AbsolutePointer->Mode->Attributes: 0x%08x\n",
    VkContext->AbsolutePointer->Mode->Attributes
    ));
}

/**
  Start the virtual keyboard driver

  This routine allocates the necessary resources for the driver.

  This routine is called by VirtualKeyboardDriverStart to complete the driver
  initialization.

  @param[in, out] VkContext        Address of an VK_CONTEXT structure
  @param[in]      Controller       Handle of device to work with.

  @retval EFI_SUCCESS              Driver API properly initialized

**/
EFI_STATUS
VkApiStart (
  IN OUT VK_CONTEXT *VkContext,
  IN EFI_HANDLE     Controller
  )
{
  EFI_STATUS                        Status;
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleEx;
  EFI_KEY_DATA                      KeyData;
  EFI_HANDLE                        NotifyHandle;
  EFI_EVENT                         ReadyToBootEvent;

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkApiStart Start\n"));

  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  VkKeyNotifyProcessHandler,
                  VkContext,
                  &VkContext->KeyNotifyProcessEvent
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_ERROR, "ERROR - Failed to create VkContext->KeyNotifyProcessEvent, Status: %r\n", Status));
    goto Error;
  }

  Status = gBS->CreateEvent (
                  EVT_NOTIFY_WAIT,
                  TPL_NOTIFY,
                  VkTouchWaitForKey,
                  VkContext,
                  &(VkContext->SimpleTextIn.WaitForKey)
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_ERROR, "ERROR - Failed to create VkContext->SimpleTextIn.WaitForKey, Status: %r\n", Status));
    goto Error;
  }

  Status = gBS->CreateEvent (
                  EVT_NOTIFY_WAIT,
                  TPL_NOTIFY,
                  VkTouchWaitForKey,
                  VkContext,
                  &(VkContext->SimpleTextInEx.WaitForKeyEx)
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_ERROR, "ERROR - Failed to create VkContext->SimpleTextInEx.WaitForKeyEx, Status %r\n", Status));
    goto Error;
  }

  Status = gBS->CreateEvent (
                  EVT_TIMER | EVT_NOTIFY_SIGNAL,
                  TPL_VK_SYNC,
                  VkTimer,
                  (VOID *)VkContext,
                  &(VkContext->TimerEvent)
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_ERROR, "ERROR - Failed to create the timer event, Status: %r\n", Status));
    goto Error;
  }

  Status = gBS->SetTimer (
                  VkContext->TimerEvent,
                  TimerPeriodic,
                  (UINT64) VK_POLL_INTERVAL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_ERROR, "ERROR - Failed to set the timer event, Status: %r\n", Status));
    goto Error;
  }

  //
  // Create event to clear keyboard before boot into OS.
  //
  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             VkReadyToBootCallBack,
             (VOID *)VkContext,
             &ReadyToBootEvent
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_ERROR, "ERROR - Failed to create ReadyToBootEvent, Status: %r\n", Status));
    goto Error;
  }

  Status = gBS->LocateProtocol (
                  &gEfiHiiImageExProtocolGuid,
                  NULL,
                  (VOID **) &VkContext->HiiImageEx
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_ERROR, "ERROR - Failed to open HiiImageEx protocol, Status: %r\n", Status));
    goto Error;
  }

  //
  // Initialize VkContext
  //
  VkContext->Signature                          = VK_SIGNATURE;
  VkContext->Controller                         = Controller;
  VkContext->SimpleTextIn.Reset                 = VkKeyboardReset;
  VkContext->SimpleTextIn.ReadKeyStroke         = VkKeyboardReadKeyStroke;
  VkContext->SimpleTextInEx.Reset               = VkKeyboardResetEx;
  VkContext->SimpleTextInEx.ReadKeyStrokeEx     = VkKeyboardReadKeyStrokeEx;
  VkContext->SimpleTextInEx.SetState            = VkKeyboardSetState;
  VkContext->SimpleTextInEx.RegisterKeyNotify   = VkKeyboardRegisterKeyNotify;
  VkContext->SimpleTextInEx.UnregisterKeyNotify = VkKeyboardUnregisterKeyNotify;
  VkContext->IsIconShowed                       = FALSE;
  VkContext->IsBackgroundChanged                = FALSE;
  VkContext->PageNumber                         = 0;
  VkContext->CurrentKeyboardDisplay             = VkDisplayAttributeNone;
  VkContext->TargetKeyboardDisplay              = VkDisplayAttributeNone;
  VkContext->VkBodyBackgroundBltBuffer          = NULL;
  VkContext->VkBodyCompoundBltBuffer            = NULL;
  VkContext->VkBodyBltSize                      = 0;
  VkContext->VkBodyBltStartX                    = 0;
  VkContext->VkBodyBltStartY                    = 0;
  VkContext->VkBodyBltHeight                    = 0;
  VkContext->VkBodyBltWidth                     = 0;
  VkContext->IconBltBuffer                      = NULL;
  VkContext->IconBltSize                        = 0;
  VkContext->IconReDrawCheck                    = 0;
  VkContext->FullIconUpdatedFlag                = FALSE;
  VkContext->SimIconUpdatedFlag                 = FALSE;
  VkContext->KeyStartIndex                      = 0;
  VkContext->KeyEndIndex                        = 0;
  VkContext->KeyToggleState                     = EFI_TOGGLE_STATE_VALID;
  VkContext->SmallIcon                          = VkGetImage (VkContext, IMAGE_TOKEN (IMG_VK_SIMPLEICON));
  VkContext->FullIcon                           = VkGetImage (VkContext, IMAGE_TOKEN (IMG_VK_FULLICON));
  VkContext->SimKeyBody                         = VkGetImage (VkContext, IMAGE_TOKEN (IMG_VK_SIMPLEKEYBOARD));
  VkContext->DigKeyBody                         = VkGetImage (VkContext, IMAGE_TOKEN (IMG_VK_DIGITKEYBOARD));
  VkContext->CapLeKeyBody                       = VkGetImage (VkContext, IMAGE_TOKEN (IMG_VK_CAPITALLETTERKEYBOARD));

  InitializeListHead (&VkContext->NotifyList);

  Status = SetCharacterPosition (VkContext, 800, 600);
  ASSERT_EFI_ERROR (Status);

  Status = DrawKeyboardLayout (VkContext);
  ASSERT_EFI_ERROR (Status);

  Status = HideVkBody (VkContext);
  ASSERT_EFI_ERROR (Status);

  Status = HideVkIcon (VkContext);
  ASSERT_EFI_ERROR (Status);

  DEBUG_CODE (
    VkDumpContext (VkContext);
  );

  Status = gBS->HandleProtocol (
                  gST->ConsoleInHandle,
                  &gEfiSimpleTextInputExProtocolGuid,
                  (VOID **)&SimpleEx
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_ERROR, "ERROR - Failed to found ConIn Protocol, Status: %r\n", Status));
    goto Error;
  }

  KeyData.KeyState.KeyToggleState = 0;
  KeyData.KeyState.KeyShiftState  = 0;
  KeyData.Key.ScanCode            = SCAN_ESC;
  KeyData.Key.UnicodeChar         = CHAR_NULL;
  NotifyHandle                    = NULL;
  Status = SimpleEx->RegisterKeyNotify (
                       SimpleEx,
                       &KeyData,
                       VkNotifyKeyCallback,
                       &NotifyHandle
                       );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_ERROR, "ERROR - Failed to register 'Esc', Status: %r\n", Status));
    goto Error;
  }

  KeyData.KeyState.KeyToggleState = 0;
  KeyData.KeyState.KeyShiftState  = 0;
  KeyData.Key.ScanCode            = SCAN_NULL;
  KeyData.Key.UnicodeChar         = CHAR_CARRIAGE_RETURN;
  Status = SimpleEx->RegisterKeyNotify (
                       SimpleEx,
                       &KeyData,
                       VkNotifyKeyCallback,
                       &NotifyHandle
                       );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_ERROR, "ERROR - Failed to register 'Enter', Status: %r\n", Status));
    goto Error;
  }

  for (KeyData.Key.UnicodeChar = L' '; KeyData.Key.UnicodeChar <= L'~'; KeyData.Key.UnicodeChar++) {
    Status = SimpleEx->RegisterKeyNotify (
                         SimpleEx,
                         &KeyData,
                         VkNotifyKeyCallback,
                         &NotifyHandle
                         );
    if (EFI_ERROR (Status)) {
      DEBUG ((
        DEBUG_VK_ROUTINE_ENTRY_EXIT | DEBUG_ERROR,
        "ERROR - Failed to register '%c', Status: %r\n",
        KeyData.Key.UnicodeChar,
        Status
        ));
      break;
    }
  }
  if (EFI_ERROR(Status)) {
    goto Error;
  }

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkApiStart Success, Status: %r\n", Status));
  goto End;

Error:
  VkApiStop (VkContext);
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkApiStart Failed, Status: %r\n", Status));

End:
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkApiStart End\n"));
  return Status;
}

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
  )
{
  EFI_STATUS Status;
  VK_NOTIFY  *NotifyNode;
  LIST_ENTRY *NotifyList;

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkApiStop Start\n"));

  Status = gBS->SetTimer (VkContext->TimerEvent, TimerCancel, 0);
  ASSERT_EFI_ERROR (Status);

  if (VkContext->KeyNotifyProcessEvent != NULL) {
    Status = gBS->CloseEvent (VkContext->KeyNotifyProcessEvent);
    ASSERT_EFI_ERROR (Status);
    VkContext->KeyNotifyProcessEvent = NULL;
  }

  if (VkContext->TimerEvent != NULL) {
    Status = gBS->CloseEvent (VkContext->TimerEvent);
    ASSERT_EFI_ERROR (Status);
    VkContext->TimerEvent = NULL;
  }

  if (VkContext->SimpleTextIn.WaitForKey != NULL) {
    Status = gBS->CloseEvent (VkContext->SimpleTextIn.WaitForKey);
    ASSERT_EFI_ERROR (Status);
    VkContext->SimpleTextIn.WaitForKey = NULL;
  }

  if (VkContext->SimpleTextInEx.WaitForKeyEx != NULL) {
    Status = gBS->CloseEvent (VkContext->SimpleTextInEx.WaitForKeyEx);
    ASSERT_EFI_ERROR (Status);
    VkContext->SimpleTextInEx.WaitForKeyEx = NULL;
  }

  NotifyList = &VkContext->NotifyList;
  if (NotifyList != NULL) {
    while (!IsListEmpty (NotifyList)) {
      NotifyNode = CR (NotifyList->ForwardLink, VK_NOTIFY, NotifyEntry, VK_NOTIFY_SIGNATURE);
      RemoveEntryList (NotifyList->ForwardLink);
      gBS->FreePool (NotifyNode);
    }
  }

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkApiStop End\n"));
}

/**
  Reset the input device and optionally run diagnostics

  @param[in] This                 Protocol instance pointer.
  @param[in] ExtendedVerification Driver may perform diagnostics on reset.

  @retval EFI_SUCCESS             The device was reset.
  @retval EFI_DEVICE_ERROR        The device is not functioning properly and could not be reset.

**/
EFI_STATUS
EFIAPI
VkKeyboardReset (
  IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
  IN BOOLEAN                        ExtendedVerification
  )
{
  EFI_STATUS Status;

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardReset Start\n"));
  Status = EFI_SUCCESS;
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardReset End\n"));
  return Status;
}

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
  )
{
  EFI_STATUS Status;
  VK_CONTEXT *VkContext;

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardResetEx Start\n"));

  Status = EFI_SUCCESS;
  if (This == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Error;
  }

  VkContext = VK_CONTEXT_FROM_SIMPLETEXTINEX_PROTOCOL (This);

  Status = VkContext->SimpleTextIn.Reset (&VkContext->SimpleTextIn, ExtendedVerification);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VK_KEYS, "ERROR - Failed to VK reset, Status: %r\n", Status));
    goto Error;
  }

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardResetEx Success, Status: %r\n", Status));
  goto End;

Error:
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardResetEx Failed, Status: %r\n", Status));

End:
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardResetEx End\n"));
  return Status;
}

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
  )
{
  EFI_STATUS    Status;
  VK_CONTEXT    *VkContext;
  EFI_KEY_DATA  TmpKeyData;
  EFI_TPL       OldTpl;

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardReadKeyStroke Start\n"));

  OldTpl = gBS->RaiseTPL (TPL_NOTIFY);
  if (This == NULL || Key == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Error;
  }

  VkContext = VK_CONTEXT_FROM_PROTOCOL (This);

  Status = VkKeyboardReadKeyStrokeEx (&VkContext->SimpleTextInEx, &TmpKeyData);
  if (EFI_ERROR (Status)) {
    goto Error;
  }
  *Key = TmpKeyData.Key;


  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardReadKeyStroke Success, Status: %r\n", Status));
  goto End;

Error:
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardReadKeyStroke Failed, Status: %r\n", Status));

End:
  gBS->RestoreTPL (OldTpl);
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardReadKeyStroke End\n"));
  return Status;
}

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
  )
{
  VK_CONTEXT        *VkContext;
  EFI_STATUS        Status;
  EFI_TPL           OldTpl;
  EFI_KEY_DATA      TmpKeyData;

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardReadKeyStrokeEx Start\n"));

  OldTpl = gBS->RaiseTPL (TPL_NOTIFY);
  if (This == NULL || KeyData == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Error;
  }

  VkContext = VK_CONTEXT_FROM_SIMPLETEXTINEX_PROTOCOL (This);

  while (TRUE) {
    Status = VkPopTheKey (VkContext, &TmpKeyData);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_VK_KEYS, "ERROR - Failed to VkPopTheKey check whether keybuffer is empty, Status: %r\n", Status));
      goto Error;
    }
    if (TmpKeyData.Key.ScanCode != SCAN_NULL || TmpKeyData.Key.UnicodeChar != CHAR_NULL) {
      break;
    }
  }
  *KeyData = TmpKeyData;

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardReadKeyStrokeEx Success, Status: %r\n", Status));
  goto End;

Error:
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardReadKeyStrokeEx Failed, Status: %r\n", Status));

End:
  gBS->RestoreTPL (OldTpl);
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardReadKeyStrokeEx End\n"));
  return Status;
}

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
  )
{
  EFI_STATUS Status;
  VK_CONTEXT *VkContext;

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardSetState Start\n"));

  Status = EFI_SUCCESS;
  if (This == NULL || KeyToggleState == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Error;
  }

  VkContext = VK_CONTEXT_FROM_SIMPLETEXTINEX_PROTOCOL (This);

  VkContext->KeyToggleState      = *KeyToggleState;
  VkContext->IsCapsLockFlag      = (*KeyToggleState & EFI_CAPS_LOCK_ACTIVE) == EFI_CAPS_LOCK_ACTIVE;
  VkContext->IsSupportPartialKey = (*KeyToggleState & EFI_KEY_STATE_EXPOSED) == EFI_KEY_STATE_EXPOSED;

  if (VkContext->PageNumber >= VkPage0 && VkContext->PageNumber <= VkPage1) {
    VkContext->PageNumber = VkContext->IsCapsLockFlag ? VkPage1 : VkPage0;
  }

  HideVkBody (VkContext);
  DrawKeyboardLayout (VkContext);

  DEBUG ((DEBUG_VK_KEYS | DEBUG_INFO, "VkContext->KeyToggleState:      %02x\n", VkContext->KeyToggleState));
  DEBUG ((DEBUG_VK_KEYS | DEBUG_INFO, "VkContext->IsCapsLockFlag:      %02x\n", VkContext->IsCapsLockFlag));
  DEBUG ((DEBUG_VK_KEYS | DEBUG_INFO, "VkContext->IsSupportPartialKey: %02x\n", VkContext->IsSupportPartialKey));

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardSetState Success, Status: %r\n", Status));
  goto End;

Error:
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardSetState Failed, Status: %r\n", Status));

End:
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardSetState End\n"));
  return Status;
}

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
  )
{
  EFI_STATUS Status;
  EFI_TPL    OldTpl;
  VK_CONTEXT *VkContext;
  LIST_ENTRY *Link;
  VK_NOTIFY  *CurrentNotify;
  VK_NOTIFY  *NewNotify;

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardRegisterKeyNotify Start\n"));

  //
  // Enter critical section
  //
  OldTpl = gBS->RaiseTPL (TPL_NOTIFY);
  Status = EFI_SUCCESS;
  if (This == NULL || KeyData == NULL || NotifyHandle == NULL || KeyNotificationFunction == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Error;
  }
  VkContext = VK_CONTEXT_FROM_SIMPLETEXTINEX_PROTOCOL (This);


  //
  // Return EFI_SUCCESS if the (KeyData, NotificationFunction) is already registered.
  //
  for (Link = VkContext->NotifyList.ForwardLink; Link != &VkContext->NotifyList; Link = Link->ForwardLink) {
    CurrentNotify = CR (Link, VK_NOTIFY, NotifyEntry, VK_NOTIFY_SIGNATURE);
    if (IsKeyRegistered (&CurrentNotify->KeyData, KeyData)) {
      if (CurrentNotify->KeyNotificationFn == KeyNotificationFunction) {
        *NotifyHandle = CurrentNotify;
        Status = EFI_SUCCESS;
        goto End;
      }
    }
  }

  //
  // Allocate resource to save the notification function
  //
  NewNotify = (VK_NOTIFY *) AllocateZeroPool (sizeof (VK_NOTIFY));
  if (NewNotify == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Error;
  }

  NewNotify->Signature         = VK_NOTIFY_SIGNATURE;
  NewNotify->KeyNotificationFn = KeyNotificationFunction;
  CopyMem (&NewNotify->KeyData, KeyData, sizeof (EFI_KEY_DATA));
  InsertTailList (&VkContext->NotifyList, &NewNotify->NotifyEntry);

  *NotifyHandle = NewNotify;
  Status        = EFI_SUCCESS;

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardRegisterKeyNotify Success, Status: %r\n", Status));
  goto End;

Error:
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardRegisterKeyNotify Failed, Status: %r\n", Status));

End:
  //
  // Leave critical section and return
  //
  gBS->RestoreTPL (OldTpl);
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardRegisterKeyNotify End\n"));
  return Status;
}

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
  )
{
  EFI_STATUS Status;
  VK_CONTEXT *VkContext;
  EFI_TPL    OldTpl;
  LIST_ENTRY *Link;
  VK_NOTIFY  *CurrentNotify;
  BOOLEAN    IsFindNotifyHandle;

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardUnregisterKeyNotify Start\n"));

  //
  // Enter critical section
  //
  OldTpl = gBS->RaiseTPL (TPL_NOTIFY);
  Status = EFI_SUCCESS;
  if (This == NULL || NotificationHandle == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Error;
  }
  VkContext = VK_CONTEXT_FROM_SIMPLETEXTINEX_PROTOCOL (This);

  IsFindNotifyHandle = FALSE;
  for (Link = VkContext->NotifyList.ForwardLink; Link != &VkContext->NotifyList; Link = Link->ForwardLink) {
    CurrentNotify = CR (Link, VK_NOTIFY, NotifyEntry, VK_NOTIFY_SIGNATURE);
    if (CurrentNotify == NotificationHandle) {
      //
      // Remove the notification function from NotifyList and free resources
      //
      RemoveEntryList (&CurrentNotify->NotifyEntry);

      gBS->FreePool (CurrentNotify);
      IsFindNotifyHandle = TRUE;
      break;
    }
  }

  if (!IsFindNotifyHandle) {
    Status = EFI_INVALID_PARAMETER;
    goto Error;
  }

  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardUnregisterKeyNotify Success, Status: %r\n", Status));
  goto End;

Error:
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardUnregisterKeyNotify Failed, Status: %r\n", Status));

End:
  //
  // Leave critical section and return
  //
  gBS->RestoreTPL (OldTpl);
  DEBUG ((DEBUG_VK_ROUTINE_ENTRY_EXIT, "VkKeyboardUnregisterKeyNotify End\n"));
  return Status;
}
