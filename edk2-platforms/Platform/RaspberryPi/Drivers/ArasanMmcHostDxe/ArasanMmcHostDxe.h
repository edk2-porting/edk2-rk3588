/** @file
 *
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef _MMC_HOST_DXE_H_
#define _MMC_HOST_DXE_H_

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DmaLib.h>

#include <Protocol/EmbeddedExternalDevice.h>
#include <Protocol/BlockIo.h>
#include <Protocol/DevicePath.h>
#include <Protocol/RpiMmcHost.h>
#include <Protocol/RpiFirmware.h>

#include <IndustryStandard/Bcm2836.h>
#include <IndustryStandard/Bcm2836Sdio.h>
#include <IndustryStandard/RpiMbox.h>

#define MAX_RETRY_COUNT (1000 * 20)

#define STALL_AFTER_SEND_CMD_US (200) // in microseconds
#define STALL_AFTER_REC_RESP_US (50)
#define STALL_AFTER_WRITE_US (200)
#define STALL_AFTER_READ_US (20)
#define STALL_AFTER_REG_WRITE_US (10)
#define STALL_AFTER_RETRY_US (20)

#define MAX_DIVISOR_VALUE 1023

#endif
