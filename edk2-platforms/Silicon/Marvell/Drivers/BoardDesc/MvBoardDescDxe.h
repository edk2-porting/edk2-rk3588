/*******************************************************************************
Copyright (C) 2018 Marvell International Ltd.
SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/
#ifndef __MV_BOARD_DESC_H__
#define __MV_BOARD_DESC_H__

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include <Protocol/BoardDesc.h>

#include <Uefi/UefiBaseType.h>

#define MV_BOARD_DESC_SIGNATURE SIGNATURE_64 ('M', 'V', 'B', 'R', 'D', 'D', 'S', 'C')

typedef struct {
  MARVELL_BOARD_DESC_PROTOCOL   BoardDescProtocol;
  UINT64                  Signature;
  EFI_HANDLE              Handle;
  EFI_LOCK                Lock;
} MV_BOARD_DESC;

#endif // __MV_BOARD_DESC_H__
