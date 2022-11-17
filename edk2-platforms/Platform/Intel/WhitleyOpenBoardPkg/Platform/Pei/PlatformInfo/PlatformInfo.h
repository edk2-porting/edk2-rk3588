/** @file
  Platform Info Driver.

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PLATFORM_INFO_INTERNAL_H_
#define _PLATFORM_INFO_INTERNAL_H_

#include <PiPei.h>
#include <Ppi/CpuIo.h>
#include <Ppi/Spi.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PlatformHooksLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Guid/SocketVariable.h>
#include <Guid/SetupVariable.h>
#include <Guid/PlatformInfo.h>
#include <IndustryStandard/Pci22.h>
#include <GpioPinsSklH.h>
#include <Library/GpioLib.h>
#include <Platform.h>
#include "SioRegs.h"
#include <Register/PchRegsSpi.h>
#include <PchAccess.h>
#include <Register/PchRegsLpc.h>
#include <Library/ReportStatusCodeLib.h>
#include <Register/Cpuid.h>

#define EFI_PLATFORMINFO_DRIVER_PRIVATE_SIGNATURE SIGNATURE_32 ('P', 'I', 'N', 'F')

//
// CPU Model
//
#define  INVALID_MODEL             0x0

#define R_SB_SPI_FDOC         0xB0
#define R_SB_SPI_FDOD         0xB4
#define SPI_OPCODE_READ_INDEX            4
#define PDR_REGION_START_OFFSET    0x0

typedef union BOARD_ID
{
   struct{
      UINT8  BoardID0            :1;
      UINT8  BoardID1            :1;
      UINT8  BoardID2            :1;
      UINT8  BoardID3            :1;
      UINT8  BoardID4            :1;
      UINT8  BoardRev0           :1;
      UINT8  BoardRev1           :1;
      UINT8  Rsvd                :1;
   }BoardID;
}BOARD_ID;

typedef union RISER_ID
{
   struct{
      UINT8  RiserID0            :1;
      UINT8  RiserID1            :1;
      UINT8  RiserID2            :1;
      UINT8  RiserID3            :1;
      UINT8  Rsvd                :4;
   }RiserID;
}RISER_ID;



EFI_STATUS
PdrGetPlatformInfo (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  OUT EFI_PLATFORM_INFO   *PlatformInfoHob
  );

EFI_STATUS
GPIOGetPlatformInfo (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  OUT EFI_PLATFORM_INFO   *PlatformInfoHob
);

#endif
