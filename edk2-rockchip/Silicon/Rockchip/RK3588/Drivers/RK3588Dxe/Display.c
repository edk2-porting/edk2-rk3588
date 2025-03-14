/** @file
 *
 *  Copyright (c) 2025, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <VarStoreData.h>

#include "RK3588DxeFormSetGuid.h"
#include "Display.h"

STATIC VOID  *mTextInExInstallRegistration;

STATIC
EFI_STATUS
InitializeDisplayVariables (
  IN BOOLEAN  Reset
  )
{
  EFI_STATUS                                 Status;
  UINTN                                      Index;
  UINT32                                     *Connectors;
  UINTN                                      ConnectorsCount;
  UINT32                                     ConnectorsMask;
  UINTN                                      Size;
  UINT8                                      Var8;
  UINT16                                     Var16;
  VOID                                       *PcdData;
  DISPLAY_MODE_PRESET_VARSTORE_DATA          ModePreset;
  DISPLAY_MODE                               ModeCustom;
  DISPLAY_CONNECTORS_PRIORITY_VARSTORE_DATA  ConnectorsPriority;

  Connectors      = PcdGetPtr (PcdDisplayConnectors);
  ConnectorsCount = PcdGetSize (PcdDisplayConnectors) / sizeof (*Connectors);
  ASSERT (ConnectorsCount <= VOP_OUTPUT_IF_NUMS);

  ConnectorsMask = 0;
  for (Index = 0; Index < ConnectorsCount; Index++ ) {
    ConnectorsMask |= Connectors[Index];
  }

  if ((ConnectorsMask & (VOP_OUTPUT_IF_HDMI0 | VOP_OUTPUT_IF_EDP0)) ==
      (VOP_OUTPUT_IF_HDMI0 | VOP_OUTPUT_IF_EDP0))
  {
    DEBUG ((DEBUG_ERROR, "%a: Cannot support both HDMI0 and EDP0 simultaneously!\n", __func__));
    ASSERT (FALSE);
    ConnectorsMask &= ~(VOP_OUTPUT_IF_HDMI0 | VOP_OUTPUT_IF_EDP0);
  }

  if ((ConnectorsMask & (VOP_OUTPUT_IF_HDMI1 | VOP_OUTPUT_IF_EDP1)) ==
      (VOP_OUTPUT_IF_HDMI1 | VOP_OUTPUT_IF_EDP1))
  {
    DEBUG ((DEBUG_ERROR, "%a: Cannot support both HDMI1 and EDP1 simultaneously!\n", __func__));
    ASSERT (FALSE);
    ConnectorsMask &= ~(VOP_OUTPUT_IF_HDMI1 | VOP_OUTPUT_IF_EDP1);
  }

  Status = PcdSet32S (PcdDisplayConnectorsMask, ConnectorsMask);
  ASSERT_EFI_ERROR (Status);

  if (ConnectorsMask == 0) {
    return EFI_UNSUPPORTED;
  }

  PcdData = PcdGetPtr (PcdDisplayModePresetDefault);
  ASSERT (PcdData != NULL);
  if (PcdData != NULL) {
    Size   = sizeof (ModePreset);
    Status = gRT->SetVariable (
                    L"DisplayModeDefault",
                    &gRK3588DxeFormSetGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    Size,
                    PcdData
                    );
    ASSERT_EFI_ERROR (Status);

    Status = !Reset ? gRT->GetVariable (
                             L"DisplayModePreset",
                             &gRK3588DxeFormSetGuid,
                             NULL,
                             &Size,
                             &ModePreset
                             ) : EFI_NOT_FOUND;
    if (EFI_ERROR (Status)) {
      Status = PcdSetPtrS (PcdDisplayModePreset, &Size, PcdData);
      ASSERT_EFI_ERROR (Status);
    }
  }

  PcdData = PcdGetPtr (PcdDisplayModeCustomDefault);
  ASSERT (PcdData != NULL);
  if (PcdData != NULL) {
    Size   = sizeof (ModeCustom);
    Status = gRT->SetVariable (
                    L"DisplayModeCustomDefault",
                    &gRK3588DxeFormSetGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    Size,
                    PcdData
                    );
    ASSERT_EFI_ERROR (Status);

    Status = !Reset ? gRT->GetVariable (
                             L"DisplayModeCustom",
                             &gRK3588DxeFormSetGuid,
                             NULL,
                             &Size,
                             &ModeCustom
                             ) : EFI_NOT_FOUND;
    if (EFI_ERROR (Status)) {
      Status = PcdSetPtrS (PcdDisplayModeCustom, &Size, PcdData);
      ASSERT_EFI_ERROR (Status);
    }
  }

  Size   = sizeof (ConnectorsPriority);
  Status = !Reset ? gRT->GetVariable (
                           L"DisplayConnectorsPriority",
                           &gRK3588DxeFormSetGuid,
                           NULL,
                           &Size,
                           &ConnectorsPriority
                           ) : EFI_NOT_FOUND;
  if (EFI_ERROR (Status)) {
    ZeroMem (&ConnectorsPriority, sizeof (ConnectorsPriority));
    CopyMem (&ConnectorsPriority.Order, Connectors, ConnectorsCount * sizeof (*Connectors));

    Status = PcdSetPtrS (PcdDisplayConnectorsPriority, &Size, &ConnectorsPriority);
    ASSERT_EFI_ERROR (Status);
  }

  Size   = sizeof (Var8);
  Status = !Reset ? gRT->GetVariable (
                           L"DisplayForceOutput",
                           &gRK3588DxeFormSetGuid,
                           NULL,
                           &Size,
                           &Var8
                           ) : EFI_NOT_FOUND;
  if (EFI_ERROR (Status)) {
    Status = PcdSetBoolS (PcdDisplayForceOutput, FixedPcdGetBool (PcdDisplayForceOutputDefault));
    ASSERT_EFI_ERROR (Status);
  }

  Size   = sizeof (Var8);
  Status = !Reset ? gRT->GetVariable (
                           L"DisplayDuplicateOutput",
                           &gRK3588DxeFormSetGuid,
                           NULL,
                           &Size,
                           &Var8
                           ) : EFI_NOT_FOUND;
  if (EFI_ERROR (Status)) {
    Status = PcdSetBoolS (PcdDisplayDuplicateOutput, FixedPcdGetBool (PcdDisplayDuplicateOutputDefault));
    ASSERT_EFI_ERROR (Status);
  }

  Size   = sizeof (Var16);
  Status = !Reset ? gRT->GetVariable (
                           L"DisplayRotation",
                           &gRK3588DxeFormSetGuid,
                           NULL,
                           &Size,
                           &Var16
                           ) : EFI_NOT_FOUND;
  if (EFI_ERROR (Status)) {
    Status = PcdSet16S (PcdDisplayRotation, FixedPcdGet16 (PcdDisplayRotationDefault));
    ASSERT_EFI_ERROR (Status);
  }

  Size   = sizeof (Var8);
  Status = !Reset ? gRT->GetVariable (
                           L"HdmiSignalingMode",
                           &gRK3588DxeFormSetGuid,
                           NULL,
                           &Size,
                           &Var8
                           ) : EFI_NOT_FOUND;
  if (EFI_ERROR (Status)) {
    Status = PcdSet8S (PcdHdmiSignalingMode, FixedPcdGet8 (PcdHdmiSignalingModeDefault));
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
ResetKeyHandler (
  IN EFI_KEY_DATA  *KeyData
  )
{
  DEBUG ((DEBUG_INFO, "Display reset key pressed - restoring default settings.\n"));

  InitializeDisplayVariables (TRUE);

  gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);

  return EFI_SUCCESS;
}

STATIC
VOID
EFIAPI
OnTextInExInstallNotify (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                         Status;
  UINTN                              HandleCount;
  EFI_HANDLE                         *HandleBuffer;
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL  *SimpleTextInEx;
  EFI_HANDLE                         KeyNotifyHandle;
  EFI_KEY_DATA                       ResetKey;

  while (TRUE) {
    Status = gBS->LocateHandleBuffer (
                    ByRegisterNotify,
                    NULL,
                    mTextInExInstallRegistration,
                    &HandleCount,
                    &HandleBuffer
                    );
    if (EFI_ERROR (Status)) {
      if (Status != EFI_NOT_FOUND) {
        ASSERT_EFI_ERROR (Status);
      }

      break;
    }

    ASSERT (HandleCount == 1);

    Status = gBS->HandleProtocol (
                    HandleBuffer[0],
                    &gEfiSimpleTextInputExProtocolGuid,
                    (VOID **)&SimpleTextInEx
                    );
    FreePool (HandleBuffer);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      continue;
    }

    // LCtrl+LShift+F6
    ResetKey.Key.ScanCode           = SCAN_F6;
    ResetKey.Key.UnicodeChar        = CHAR_NULL;
    ResetKey.KeyState.KeyShiftState = EFI_SHIFT_STATE_VALID |
                                      EFI_LEFT_CONTROL_PRESSED |
                                      EFI_LEFT_SHIFT_PRESSED;
    ResetKey.KeyState.KeyToggleState = 0;

    KeyNotifyHandle = NULL;

    Status = SimpleTextInEx->RegisterKeyNotify (
                               SimpleTextInEx,
                               &ResetKey,
                               ResetKeyHandler,
                               &KeyNotifyHandle
                               );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      DEBUG ((
        DEBUG_ERROR,
        "%a: Failed to register display reset key. Status=%r\n",
        __func__,
        Status
        ));
      continue;
    }
  }
}

STATIC
VOID
RegisterResetHandlers (
  VOID
  )
{
  EFI_EVENT  Event;

  Event = EfiCreateProtocolNotifyEvent (
            &gEfiSimpleTextInputExProtocolGuid,
            TPL_CALLBACK,
            OnTextInExInstallNotify,
            NULL,
            &mTextInExInstallRegistration
            );

  ASSERT (Event != NULL);
}

VOID
EFIAPI
ApplyDisplayVariables (
  VOID
  )
{
}

VOID
EFIAPI
SetupDisplayVariables (
  VOID
  )
{
  EFI_STATUS  Status;

  Status = InitializeDisplayVariables (FALSE);
  if (EFI_ERROR (Status)) {
    return;
  }

  RegisterResetHandlers ();
}
