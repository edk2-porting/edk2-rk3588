/** @file

Copyright (c) 2017 - 2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include "PeiAspireVn7Dash572GInitLib.h"
#include <Library/BoardEcLib.h>
#include <Library/DebugLib.h>

#define ADC_3V_10BIT_GRANULARITY_MAX  (3005/1023)
#define PCB_VER_AD                    1
#define MODEL_ID_AD                   3

/**
  Get Aspire V Nitro (Skylake) board ID.
  There are 2 different boards having different ID.
  This function will return board ID to caller.

  @param[out] DataBuffer

  @retval     EFI_SUCCESS       Command success
  @retval     EFI_DEVICE_ERROR  Command error
**/
VOID
GetAspireVn7Dash572GBoardId (
  OUT UINT8    *BoardId
  )
{
  UINT16        DataBuffer;

  ReadEcAdcConverter (MODEL_ID_AD, &DataBuffer);
  DEBUG ((DEBUG_INFO, "BoardId (raw) = 0x%X\n", DataBuffer));
  // Board by max millivoltage range (of 10-bit, 3.005 V ADC)
  if (DataBuffer <= (1374/ADC_3V_10BIT_GRANULARITY_MAX)) {
    // Consider returning an error
    DEBUG ((DEBUG_ERROR, "BoardId is reserved?\n"));
  } else if (DataBuffer <= (2017/ADC_3V_10BIT_GRANULARITY_MAX)) {
    *BoardId = BoardIdNewgateSLx_dGPU;
  } else {
    *BoardId = BoardIdRayleighSLx_dGPU;
  }
  DEBUG ((DEBUG_INFO, "BoardId = 0x%X\n", *BoardId));

  ReadEcAdcConverter (PCB_VER_AD, &DataBuffer);
  DEBUG ((DEBUG_INFO, "PCB version (raw) = 0x%X\n", DataBuffer));
  DEBUG ((DEBUG_INFO, "PCB version: "));
  // PCB by max millivoltage range (of 10-bit, 3.005 V ADC)
  if (DataBuffer <= (2017/ADC_3V_10BIT_GRANULARITY_MAX)) {
    // Consider returning an error
    DEBUG ((DEBUG_ERROR, "Reserved?\n"));
  } else if (DataBuffer <= (2259/ADC_3V_10BIT_GRANULARITY_MAX)) {
    DEBUG ((DEBUG_ERROR, "-1\n"));
  } else if (DataBuffer <= (2493/ADC_3V_10BIT_GRANULARITY_MAX)) {
    DEBUG ((DEBUG_ERROR, "SC\n"));
  } else if (DataBuffer <= (2759/ADC_3V_10BIT_GRANULARITY_MAX)) {
    DEBUG ((DEBUG_ERROR, "SB\n"));
  } else {
    DEBUG ((DEBUG_ERROR, "SA\n"));
  }
}

EFI_STATUS
EFIAPI
AspireVn7Dash572GBoardDetect (
  VOID
  )
{
  UINT8     BoardId;

  if (LibPcdGetSku () != 0) {
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "AspireVn7Dash572GDetectionCallback\n"));
  GetAspireVn7Dash572GBoardId (&BoardId);
  if (BoardId == BoardIdNewgateSLx_dGPU || BoardId == BoardIdRayleighSLx_dGPU) {
    LibPcdSetSku (BoardId);
    ASSERT (LibPcdGetSku() == BoardId);
  } else {
    DEBUG ((DEBUG_INFO, "BoardId not returned or valid!\n"));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "SKU_ID: 0x%x\n", LibPcdGetSku()));
  return EFI_SUCCESS;
}
