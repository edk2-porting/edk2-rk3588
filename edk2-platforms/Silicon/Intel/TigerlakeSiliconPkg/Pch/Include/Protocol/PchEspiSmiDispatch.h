/** @file
  SmmEspiDispatch Protocol

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_ESPI_SMI_DISPATCH_PROTOCOL_H_
#define _PCH_ESPI_SMI_DISPATCH_PROTOCOL_H_

//
// Extern the GUID for protocol users.
//
extern EFI_GUID gPchEspiSmiDispatchProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _PCH_ESPI_SMI_DISPATCH_PROTOCOL PCH_ESPI_SMI_DISPATCH_PROTOCOL;

//
// Member functions
//

/**
  Callback function for an PCH eSPI SMI handler dispatch.

  @param[in] DispatchHandle             The unique handle assigned to this handler by register function.
**/
typedef
VOID
(EFIAPI *PCH_ESPI_SMI_DISPATCH_CALLBACK) (
  IN EFI_HANDLE DispatchHandle
  );

/**
  Generic function to register different types of eSPI SMI types

  @param[in]  This              Not used
  @param[in]  DispatchFunction  The callback to execute
  @param[out] DispatchHandle    The handle for this callback registration

  @retval     EFI_SUCCESS       Registration successful
  @retval     EFI_ACCESS_DENIED Return access denied if the EndOfDxe event has been triggered
  @retval     others            Registration failed
**/
typedef
EFI_STATUS
(EFIAPI *PCH_ESPI_SMI_REGISTER) (
  IN PCH_ESPI_SMI_DISPATCH_PROTOCOL *This,
  IN PCH_ESPI_SMI_DISPATCH_CALLBACK DispatchFunction,
  OUT EFI_HANDLE                    *DispatchHandle
  );

/**
  eSPI SMI Dispatch Protocol instance to unregister a callback based on handle

  @param[in]  This                    Not used
  @param[in]  DispatchHandle          Handle acquired during registration

  @retval     EFI_SUCCESS             Unregister successful
  @retval     EFI_INVALID_PARAMETER   DispatchHandle is null
  @retval     EFI_INVALID_PARAMETER   DispatchHandle's forward link has bad pointer
  @retval     EFI_INVALID_PARAMETER   DispatchHandle does not exist in database
  @retval     EFI_ACCESS_DENIED       Unregistration is done after end of DXE
**/

typedef
EFI_STATUS
(EFIAPI *PCH_ESPI_SMI_UNREGISTER) (
  IN PCH_ESPI_SMI_DISPATCH_PROTOCOL *This,
  IN EFI_HANDLE                     DispatchHandle
  );

/**
  Interface structure for PCH eSPI SMIs Dispatch Protocol
  The PCH ESPI SMI DISPATCH PROTOCOL provides the ability to dispatch function for PCH eSPI related SMIs.
  It contains SMI types of BiosWr, EcAssertedVw, and eSPI Master asserted SMIs
**/
struct _PCH_ESPI_SMI_DISPATCH_PROTOCOL {
  /**
    This member specifies the revision of this structure. This field is used to
    indicate backwards compatible changes to the protocol.
  **/
  UINT8                   Revision;
  /**
    Unregister eSPI SMI events
  **/
  PCH_ESPI_SMI_UNREGISTER UnRegister;
  /**
    Register a BIOS Write Protect event
  **/
  PCH_ESPI_SMI_REGISTER   BiosWrProtectRegister;
  /**
    Register a BIOS Write Report event
  **/
  PCH_ESPI_SMI_REGISTER   BiosWrReportRegister;
  /**
    Register a Peripheral Channel Non Fatal Error event
  **/
  PCH_ESPI_SMI_REGISTER   PcErrNonFatalRegister;
  /**
    Register a Peripheral Channel Fatal Error event
  **/
  PCH_ESPI_SMI_REGISTER   PcErrFatalRegister;
  /**
    Register a Virtual Wire Non Fatal Error event
  **/
  PCH_ESPI_SMI_REGISTER   VwErrNonFatalRegister;
  /**
    Register a Virtual Wire Fatal Error event
  **/
  PCH_ESPI_SMI_REGISTER   VwErrFatalRegister;
  /**
    Register a Flash Channel Non Fatal Error event
  **/
  PCH_ESPI_SMI_REGISTER   FlashErrNonFatalRegister;
  /**
    Register a Flash Channel Fatal Error event
  **/
  PCH_ESPI_SMI_REGISTER   FlashErrFatalRegister;
  /**
    Register a Link Error event
  **/
  PCH_ESPI_SMI_REGISTER   LnkErrType1Register;
  /**
    Register a SMI handler for Espi slaver
    This routine will also lock down ESPI_SMI_LOCK bit after registration and prevent
    this handler from unregistration.
    On platform that supports more than 1 device through another chip select (SPT-H),
    the SMI handler itself needs to inspect both the eSPI devices' interrupt status registers
    (implementation specific for each Slave) in order to identify and service the cause.
    After servicing it, it has to clear the Slaves' internal SMI# status registers
  **/
  PCH_ESPI_SMI_REGISTER   EspiSlaveSmiRegister;
};

/**
  PCH ESPI SMI dispatch revision number

  Revision 1:   Initial version
**/
#define PCH_ESPI_SMI_DISPATCH_REVISION            1

#endif
