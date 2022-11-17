/** @file
  A Library to support all BMC access via IPMI command during PEI Phase.

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <Library/IpmiBaseLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Ppi/IpmiTransportPpi.h>


/**
  Initialize the global varible with the pointer of IpmiTransport Protocol.

  @return EFI_SUCCESS - Always return success

**/
EFI_STATUS
InitializeIpmiBase (
  VOID
  )
{
  EFI_STATUS                                Status;
  PEI_IPMI_TRANSPORT_PPI                    *IpmiTransport;

  Status = PeiServicesLocatePpi (&gPeiIpmiTransportPpiGuid, 0, NULL, (VOID **) &IpmiTransport);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
  }
  return Status;
}

/**
  Routine to send commands to BMC.

  @param NetFunction       - Net function of the command
  @param Command           - IPMI Command
  @param CommandData       - Command Data
  @param CommandDataSize   - Size of CommandData
  @param ResponseData      - Response Data
  @param ResponseDataSize  - Response Data Size

  @return EFI_NOT_AVAILABLE_YET - IpmiTransport Protocol is not installed yet

**/
EFI_STATUS
IpmiSubmitCommand (
  IN UINT8        NetFunction,
  IN UINT8        Command,
  IN UINT8        *CommandData,
  IN UINT32       CommandDataSize,
  OUT UINT8       *ResponseData,
  IN OUT UINT32   *ResponseDataSize
  )
{
  EFI_STATUS                                Status;
  PEI_IPMI_TRANSPORT_PPI                    *IpmiTransport;

  Status = PeiServicesLocatePpi (&gPeiIpmiTransportPpiGuid, 0, NULL, (VOID **) &IpmiTransport);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  Status = IpmiTransport->IpmiSubmitCommand (
                            IpmiTransport,
                            NetFunction,
                            0,
                            Command,
                            CommandData,
                            CommandDataSize,
                            ResponseData,
                            ResponseDataSize
                            );
  return Status;
}

/**
  Routine to send commands to BMC.

  @param BmcStatus       - Ststus of Bmc
  @param ComAddress      - IPMI Address

  @return EFI_NOT_AVAILABLE_YET - IpmiTransport Protocol is not installed yet

**/
EFI_STATUS
GetBmcStatus (
  OUT BMC_STATUS                       *BmcStatus,
  OUT SM_COM_ADDRESS                   *ComAddress
  )
{
  EFI_STATUS                                Status;
  PEI_IPMI_TRANSPORT_PPI                    *IpmiTransport;

  Status = PeiServicesLocatePpi (&gPeiIpmiTransportPpiGuid, 0, NULL, (VOID **) &IpmiTransport);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  Status = IpmiTransport->GetBmcStatus (
                            IpmiTransport,
                            BmcStatus,
                            ComAddress
                            );
  return Status;
}

