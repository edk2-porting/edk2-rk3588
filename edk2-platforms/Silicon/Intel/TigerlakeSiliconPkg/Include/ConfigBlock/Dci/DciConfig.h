/** @file
  Dci policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _DCI_CONFIG_H_
#define _DCI_CONFIG_H_

#define DCI_PREMEM_CONFIG_REVISION 2
extern EFI_GUID gDciPreMemConfigGuid;

#pragma pack (push,1)

typedef enum {
  DciDbcDisabled       = 0x0,
  DciDbcUsb2           = 0x1,
  DciDbcUsb3           = 0x2,
  DciDbcBoth           = 0x3,
  DciDbcNoChange       = 0x4,
  DciDbcMax
} DCI_DBC_MODE;

typedef enum {
  Usb3TcDbgDisabled    = 0x0,
  Usb3TcDbgEnabled     = 0x1,
  Usb3TcDbgNoChange    = 0x2,
  Usb3TcDbgMax
} DCI_USB3_TYPE_C_DEBUG_MODE;

/**
  The PCH_DCI_PREMEM_CONFIG block describes policies related to Direct Connection Interface (DCI)

  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Added DciModphyPg
  - change to use data in byte unit rather than bit-field
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  /**
    DCI enable.
    Determine if to enable DCI debug from host.
    <b>0:Disabled</b>; 1:Enabled
  **/
  UINT8    DciEn;
  /**
    USB DbC enable mode.
    Disabled: Clear both USB2/3DBCEN; USB2: Set USB2DBCEN; USB3: Set USB3DBCEN; Both: Set both USB2/3DBCEN; No Change: Comply with HW value
    Refer to definition of DCI_USB_DBC_MODE for supported settings.
    0:Disabled; 1:USB2; 2:USB3; 3:Both; <b>4:No Change</b>
  **/
  UINT8    DciDbcMode;
  /**
    Enable Modphy power gate when DCI is enable. It must be disabled for 4-wire DCI OOB. Set default to HW default : Disabled
    <b>0:Disabled</b>; 1:Enabled
  **/
  UINT8    DciModphyPg;
  /**
    USB3 Type-C UFP2DFP kenel / platform debug support. No change will do nothing to UFP2DFP configuration.
    When enabled, USB3 Type C UFP (upstream-facing port) may switch to DFP (downstream-facing port) for first connection.
    It must be enabled for USB3 kernel(kernel mode debug) and platform debug(DFx, DMA, Trace) over UFP Type-C receptacle.
    Refer to definition of DCI_USB_TYPE_C_DEBUG_MODE for supported settings.
    0:Disabled; 1:Enabled; <b>2:No Change</b>
  **/
  UINT8    DciUsb3TypecUfpDbg;
} PCH_DCI_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _DCI_CONFIG_H_
