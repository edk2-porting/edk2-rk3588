/** @file

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __SETUP__H__
#define __SETUP__H__

#ifndef MDEPKG_NDEBUG
#define DEBUG_INTERFACE_FORM_ENABLE
#endif // MDEPKG_NDEBUG
//
// Form class guid for the forms those will be showed on first front page.
//
#define FRONT_PAGE_GUID        { 0xe58809f8, 0xfbc1, 0x48e2, { 0x88, 0x3a, 0xa3, 0xf, 0xdc, 0x4b, 0x44, 0x1e } }
//
// Form class guid for the forms those will be showed on boot maintenance manager menu.
//
#define BOOT_MAINTENANCE_GUID  { 0xb2dedc91, 0xd59f, 0x48d2, { 0x89, 0x8a, 0x12, 0x49, 0xc, 0x74, 0xa4, 0xe0 } }

// VFR common Definitions
#define INVENTORY(Name,Value) \
    text \
      help  = STRING_TOKEN(STR_EMPTY), \
      text  = Name, \
      text  = Value, \
      flags = 0, \
      key   = 0;

#define SUBTITLE(Text) subtitle text = Text;
#define SEPARATOR SUBTITLE(STRING_TOKEN(STR_EMPTY))

#define INTERACTIVE_TEXT(HelpToken, CaptionToken, ValueToken, Key)\
  grayoutif TRUE;\
    oneof varid        = SETUP_DATA.InteractiveText,\
      questionid       = Key,\
      prompt           = CaptionToken,\
      help             = HelpToken,\
      option text      = ValueToken, value = 0, flags = INTERACTIVE | DEFAULT;\
      refresh interval = 1 \
    endoneof;\
  endif;

#define SUPPRESS_GRAYOUT_ENDIF endif; endif;
#define DEFAULT_FLAG

#define SYSTEM_ACCESS_KEY_ID            0xF000
//
// System Access defintions.
//
#define SYSTEM_ACCESS_GUID \
 { 0xE770BB69, 0xBCB4, 0x4D04, { 0x9E, 0x97, 0x23, 0xFF, 0x94, 0x56, 0xFE, 0xAC }}

#define SYSTEM_PASSWORD_ADMIN 0
#define SYSTEM_PASSWORD_USER  1
#define ADMIN_PW_CLEAR        0
#define ADMIN_PW_SET          1


typedef struct _SYSTEM_ACCESS
{
  //
  // Passwords
  //
  UINT8       Access;
} SYSTEM_ACCESS;

//
// Record the password status.
//
typedef struct {
  UINT8   AdminName;
  UINT8   UserName;
} EFI_PASSWORD_STATUS;

//
// Config Data
//
typedef struct {
  UINT8 SerialDebug;
  UINT8 SerialDebugBaudRate;
  UINT8 RamDebugInterface;
  UINT8 UartDebugInterface;
  UINT8 Usb3DebugInterface;
  UINT8 SerialIoDebugInterface;
  UINT8 TraceHubDebugInterface;
} DEBUG_CONFIG_DATA;

//
// Config Data Hob
//
#define DEBUG_CONFIG_DATA_HOB DEBUG_CONFIG_DATA

//
// Secure Boot Data
//
typedef struct{
  UINT8   SecureBoot;
} SECURE_BOOT_VARIABLE;

#pragma pack()

//
// Varstore statement
// Setup is EfiVarStore that is related to EFI variable with attribute 0x07
// (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS)
//
#define SETUP_DATA_VARSTORE\
    efivarstore SETUP_DATA, varid = 1,\
        attribute = 0x7, name = Setup, guid = SETUP_GUID;
#define SA_SETUP_VARSTORE\
    efivarstore SA_SETUP, varid = 2,\
        attribute = 0x7, name = SaSetup, guid = SA_SETUP_GUID;
#define CPU_SETUP_VARSTORE\
    efivarstore CPU_SETUP, varid = 3,\
        attribute = 0x7, name = CpuSetup, guid = CPU_SETUP_GUID;
#define ME_SETUP_VARSTORE\
    efivarstore ME_SETUP, varid = 4,\
        attribute = 0x7, name = MeSetup, guid = ME_SETUP_GUID;
#define PCH_SETUP_VARSTORE\
    efivarstore PCH_SETUP, varid = 5,\
        attribute = 0x7, name = PchSetup, guid = PCH_SETUP_GUID;
#define SI_SETUP_VARSTORE\
    efivarstore SI_SETUP, varid = 6,\
        attribute = 0x7, name = SiSetup, guid = SI_SETUP_GUID;
#ifdef DEBUG_INTERFACE_FORM_ENABLE
#define DEBUG_CONFIG_DATA_ID            0xF001
#define DEBUG_CONFIG_DATA_VARSTORE\
    efivarstore DEBUG_CONFIG_DATA, varid = DEBUG_CONFIG_DATA_ID,\
        attribute = 0x7, name = DebugConfigData, guid = DEBUG_CONFIG_GUID;
#endif // DEBUG_INTERFACE_FORM_ENABLE
#define SYSTEM_ACCESS_VARSTORE\
    varstore SYSTEM_ACCESS, varid = SYSTEM_ACCESS_KEY_ID,\
        name = SystemAccess, guid = SYSTEM_ACCESS_GUID;
#define SYSTEM_PASSWORD_VARSTORE\
    varstore EFI_PASSWORD_STATUS,\
        name = PasswordStatus, guid = SYSTEM_ACCESS_GUID;

#define BOOT_FLOW_CONDITION_RECOVERY   2
#define BOOT_FLOW_CONDITION_FIRST_BOOT 4

#endif

