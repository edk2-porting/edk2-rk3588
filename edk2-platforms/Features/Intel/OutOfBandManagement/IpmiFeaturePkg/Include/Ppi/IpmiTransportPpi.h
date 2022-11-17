/** @file
  IPMI Ttransport PPI Header File.

  @copyright
  Copyright 2014 - 2021 Intel Corporation. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _IPMI_TRANSPORT_PPI_H_
#define _IPMI_TRANSPORT_PPI_H_

#include "ServerManagement.h"

typedef struct _PEI_IPMI_TRANSPORT_PPI PEI_IPMI_TRANSPORT_PPI;

#define PEI_IPMI_TRANSPORT_PPI_GUID \
  { \
    0x7bf5fecc, 0xc5b5, 0x4b25, 0x81, 0x1b, 0xb4, 0xb5, 0xb, 0x28, 0x79, 0xf7 \
  }

//
// Common Defines
//
typedef UINT32  BMC_STATUS;

#define BMC_OK        0
#define BMC_SOFTFAIL  1
#define BMC_HARDFAIL  2
#define BMC_UPDATE_IN_PROGRESS  3
#define BMC_NOTREADY  4


//
//  IPMI Function Prototypes
//
typedef
EFI_STATUS
(EFIAPI *PEI_IPMI_SEND_COMMAND) (
  IN PEI_IPMI_TRANSPORT_PPI            *This,
  IN UINT8                             NetFunction,
  IN UINT8                             Lun,
  IN UINT8                             Command,
  IN UINT8                             *CommandData,
  IN UINT32                            CommandDataSize,
  OUT UINT8                            *ResponseData,
  OUT UINT32                           *ResponseDataSize
  );

typedef
EFI_STATUS
(EFIAPI *PEI_IPMI_GET_CHANNEL_STATUS) (
  IN PEI_IPMI_TRANSPORT_PPI       *This,
  OUT BMC_STATUS                  *BmcStatus,
  OUT SM_COM_ADDRESS              *ComAddress
  );

//
// IPMI TRANSPORT PPI
//
struct _PEI_IPMI_TRANSPORT_PPI {
  UINT64                      Revision;
  PEI_IPMI_SEND_COMMAND       IpmiSubmitCommand;
  PEI_IPMI_GET_CHANNEL_STATUS GetBmcStatus;
};

extern EFI_GUID gPeiIpmiTransportPpiGuid;

#endif
