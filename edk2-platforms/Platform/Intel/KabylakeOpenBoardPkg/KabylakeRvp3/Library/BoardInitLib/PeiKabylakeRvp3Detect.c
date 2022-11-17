/** @file

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <SaPolicyCommon.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PciLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>

#include <Library/PeiSaPolicyLib.h>
#include <Library/BoardInitLib.h>
#include <PchAccess.h>
#include <Library/GpioNativeLib.h>
#include <Library/GpioLib.h>
#include <GpioPinsSklLp.h>
#include <GpioPinsSklH.h>
#include <Library/GpioExpanderLib.h>
#include <SioRegs.h>
#include <Library/PchPcrLib.h>
#include <Library/SiliconInitLib.h>

#include "PeiKabylakeRvp3InitLib.h"

#include <ConfigBlock.h>
#include <ConfigBlock/MemoryConfig.h>
#include <Library/EcLib.h>
#include <EcCommands.h>

#define BOARD_ID_MASK_8BIT                  0xff

/**
  Get board fab ID.

  @param[out] DataBuffer

  @retval     EFI_SUCCESS       Command success
  @retval     EFI_DEVICE_ERROR  Command error
**/
EFI_STATUS
GetBoardFabId (
  OUT UINT8       *DataBuffer
  )
{
  UINT8   DataSize;

  //
  // For 'EC_C_FAB_ID' command NumberOfSendData = 0, NumberOfReceiveData =2.
  //
  DataSize = 2;
  return (LpcEcInterface (EC_C_FAB_ID, &DataSize, DataBuffer));
}

/**
  Get RVP3 board ID.
  There are 2 different RVP3 boards having different ID.
  This function will return board ID to caller.

  @param[out] DataBuffer

  @retval     EFI_SUCCESS       Command success
  @retval     EFI_DEVICE_ERROR  Command error
**/
EFI_STATUS
GetRvp3BoardId (
  UINT8    *BoardId
  )
{
  EFI_STATUS    Status;
  UINT16        EcBoardInfo;
  UINT8         DataBuffer[2];

  Status = GetBoardFabId (DataBuffer);
  if (Status == EFI_SUCCESS) {
    EcBoardInfo = DataBuffer[0];
    EcBoardInfo = (EcBoardInfo << 8) | DataBuffer[1];
    //
    // Get the following data:
    // [7:0]  -  BOARD_IDx
    // [8]    -  GEN_ID
    // [11:9] -  REV_FAB_IDx
    // [12]   -  TP_SPD_PRSNT
    // [15:13] - BOM_IDx
    //
    *BoardId = (UINT8) (EcBoardInfo & BOARD_ID_MASK_8BIT);
    DEBUG ((DEBUG_INFO, "BoardId = %X\n", *BoardId));
  }
  return Status;
}

EFI_STATUS
EFIAPI
KabylakeRvp3BoardDetect (
  VOID
  )
{
  UINT8     BoardId;

  if (LibPcdGetSku () != 0) {
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "KabylakeRvp3DetectionCallback\n"));
  if (GetRvp3BoardId (&BoardId) == EFI_SUCCESS) {
    if (BoardId == BoardIdKabyLakeYLpddr3Rvp3) {
      LibPcdSetSku (BoardIdKabyLakeYLpddr3Rvp3);
      ASSERT (LibPcdGetSku() == BoardIdKabyLakeYLpddr3Rvp3);
    } else if (BoardId == BoardIdSkylakeRvp3) {
      LibPcdSetSku (BoardIdSkylakeRvp3);
      ASSERT (LibPcdGetSku() == BoardIdSkylakeRvp3);
    }
    DEBUG ((DEBUG_INFO, "SKU_ID: 0x%x\n", LibPcdGetSku()));
  }
  return EFI_SUCCESS;
}
