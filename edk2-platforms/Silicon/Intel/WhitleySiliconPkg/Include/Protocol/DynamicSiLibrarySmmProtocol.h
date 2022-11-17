/** @file
  Dynamic link silicon library service access Protocol

  This protocol abstracts silicon static library accesses via an SMM protocol

  @copyright
  Copyright 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DYNAMIC_SI_LIBARY_SMM_PROTOCOL_H_
#define _DYNAMIC_SI_LIBARY_SMM_PROTOCOL_H_


#define DYNAMIC_SI_LIBARY_SMM_PROTOCOL_GUID \
  { 0xb235fbed, 0x3b25, 0x4cb3, { 0x98, 0x9c, 0x8c, 0xe7, 0xec, 0x49, 0x8b, 0x7e } }

#define DYNAMIC_SI_LIBARY_SMM_PROTOCOL_SIGNATURE  SIGNATURE_32('D', 'S', 'L', 'S')
#define DYNAMIC_SI_LIBARY_SMM_PROTOCOL_VERSION    0x01

//
// Functions
//

typedef
UINTN
(EFIAPI *SMM_MmPciBase) (
  IN UINT32                       Bus,
  IN UINT32                       Device,
  IN UINT32                       Function
  );

typedef
UINT16
(EFIAPI *SMM_PmcGetAcpiBase) (
  VOID
  );

typedef
UINTN
(EFIAPI *SMM_PchGetPmcBaseByPchId) (
  IN  UINT8             PchId
  );

//
// UBA specific silicon abstraction protocol
//
typedef struct {
  UINT32                                  Signature;
  UINT32                                  Version;

  SMM_MmPciBase                           MmPciBase;
  SMM_PmcGetAcpiBase                      PmcGetAcpiBase;
  SMM_PchGetPmcBaseByPchId                PchGetPmcBaseByPchId;
} DYNAMIC_SI_LIBARY_SMM_PROTOCOL;

extern EFI_GUID gDynamicSiLibrarySmmProtocolGuid;

#endif // _DYNAMIC_SI_LIBARY_SMM_PROTOCOL_H_
