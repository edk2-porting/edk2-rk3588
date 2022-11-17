/** @file
  Header file for PchEspiLib.
  All function in this library is available for PEI, DXE, and SMM,

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_ESPI_LIB_H_
#define _PCH_ESPI_LIB_H_

/**
  Checks if there's second slave connected under CS#1

  @retval TRUE      There's second slave
  @retval FALSE     There's no second slave
**/
BOOLEAN
IsEspiSecondSlaveSupported (
  VOID
  );

/**
  Checks in slave General Capabilities register if it supports channel with requested number

  @param[in]  SlaveId         Id of slave to check
  @param[in]  ChannelNumber   Number of channel of which to check

  @retval TRUE      Channel with requested number is supported by slave device
  @retval FALSE     Channel with requested number is not supported by slave device
**/
BOOLEAN
IsEspiSlaveChannelSupported (
  UINT8   SlaveId,
  UINT8   ChannelNumber
  );

/**
  Is eSPI enabled in strap.

  @retval TRUE          Espi is enabled in strap
  @retval FALSE         Espi is disabled in strap
**/
BOOLEAN
IsEspiEnabled (
  VOID
  );

/**
  Get configuration from eSPI slave

  @param[in]  SlaveId       eSPI slave ID
  @param[in]  SlaveAddress  Slave Configuration Register Address
  @param[out] OutData       Configuration data read

  @retval EFI_SUCCESS           Operation succeed
  @retval EFI_INVALID_PARAMETER Slave ID is not supported
  @retval EFI_INVALID_PARAMETER Slave ID is not supported or SlaveId 1 is used in PchLp
  @retval EFI_INVALID_PARAMETER Slave configuration register address exceed maximum allowed
  @retval EFI_INVALID_PARAMETER Slave configuration register address is not DWord aligned
  @retval EFI_DEVICE_ERROR      Error in SCRS during polling stage of operation
**/
EFI_STATUS
PchEspiSlaveGetConfig (
  IN  UINT32 SlaveId,
  IN  UINT32 SlaveAddress,
  OUT UINT32 *OutData
  );

/**
  Set eSPI slave configuration

  Note: A Set_Configuration must always be followed by a Get_Configuration in order to ensure
  that the internal state of the eSPI-MC is consistent with the Slave's register settings.

  @param[in]  SlaveId       eSPI slave ID
  @param[in]  SlaveAddress  Slave Configuration Register Address
  @param[in]  InData        Configuration data to write

  @retval EFI_SUCCESS           Operation succeed
  @retval EFI_INVALID_PARAMETER Slave ID is not supported or SlaveId 1 is used in PchLp
  @retval EFI_INVALID_PARAMETER Slave configuration register address exceed maximum allowed
  @retval EFI_INVALID_PARAMETER Slave configuration register address is not DWord aligned
  @retval EFI_ACCESS_DENIED     eSPI Slave write to address range 0 to 0x7FF has been locked
  @retval EFI_DEVICE_ERROR      Error in SCRS during polling stage of operation
**/
EFI_STATUS
PchEspiSlaveSetConfig (
  IN  UINT32 SlaveId,
  IN  UINT32 SlaveAddress,
  IN  UINT32 InData
  );

/**
  Get status from eSPI slave

  @param[in]  SlaveId       eSPI slave ID
  @param[out] OutData       Configuration data read

  @retval EFI_SUCCESS           Operation succeed
  @retval EFI_INVALID_PARAMETER Slave ID is not supported or SlaveId 1 is used in PchLp
  @retval EFI_DEVICE_ERROR      Error in SCRS during polling stage of operation
**/
EFI_STATUS
PchEspiSlaveGetStatus (
  IN  UINT32 SlaveId,
  OUT UINT16 *OutData
  );

/**
  eSPI slave in-band reset

  @param[in]  SlaveId       eSPI slave ID

  @retval EFI_SUCCESS           Operation succeed
  @retval EFI_INVALID_PARAMETER Slave ID is not supported or SlaveId 1 is used in PchLp
  @retval EFI_DEVICE_ERROR      Error in SCRS during polling stage of operation
**/
EFI_STATUS
PchEspiSlaveInBandReset (
  IN  UINT32 SlaveId
  );

/**
  eSPI Slave channel reset helper function

  @param[in]  SlaveId           eSPI slave ID
  @param[in]  ChannelNumber     Number of channel to reset

  @retval     EFI_SUCCESS       Operation succeeded
  @retval     EFI_UNSUPPORTED   Slave doesn't support that channel or invalid number specified
  @retval     EFI_TIMEOUT       Operation has timeouted
**/
EFI_STATUS
PchEspiSlaveChannelReset (
  IN  UINT8   SlaveId,
  IN  UINT8   ChannelNumber
  );

#endif // _PEI_DXE_SMM_PCH_ESPI_LIB_H_
