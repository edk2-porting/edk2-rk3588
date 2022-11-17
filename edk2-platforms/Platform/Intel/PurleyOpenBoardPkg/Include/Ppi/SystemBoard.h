/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PEI_SYSTEM_BOARD__H_
#define _PEI_SYSTEM_BOARD__H_

#include <Guid/SetupVariable.h>
#include <Guid/PchRcVariable.h>
#include <Pi/PiBootMode.h>
#include <Platform.h>
#include <SysHost.h>
#include <Ppi/PchPolicy.h>
#include <Library/GpioLib.h>

///
/// The forward declaration for SYSTEM_BOARD_INFO_PPI.
///
typedef struct _SYSTEM_BOARD_PPI  SYSTEM_BOARD_PPI;

/**

  SystemIioPortBifurcationInit is used to updating the IIO_GLOBALS Data Structure with IIO
  SLOT config data
  Bifurcation config data

  @param *mSB - pointer to this protocol

  @retval *IioUds updated with SLOT and Bifurcation information updated.

**/
typedef
VOID
  (EFIAPI *PEI_SYSTEM_IIO_PORT_BIF_INIT) (
    IN IIO_GLOBALS *IioGlobalData
  );
/**

  GetUplinkPortInformation is used to get board based uplink port information

  @param IioIndex - Socket ID

  @retval PortIndex for uplink.

**/
typedef
UINT8
  (EFIAPI *PEI_GET_UPLINK_PORT_INFORMATION) (
    IN UINT8 IioIndex
  );


struct _SYSTEM_BOARD_PPI {
  PEI_SYSTEM_IIO_PORT_BIF_INIT      SystemIioPortBifurcationInit; // Update OEM IIO Port Bifurcation based on PlatformConfiguration
  PEI_GET_UPLINK_PORT_INFORMATION   GetUplinkPortInformation; // Get Uplink port information
};

extern EFI_GUID gEfiPeiSystemBoardPpiGuid;

#endif
