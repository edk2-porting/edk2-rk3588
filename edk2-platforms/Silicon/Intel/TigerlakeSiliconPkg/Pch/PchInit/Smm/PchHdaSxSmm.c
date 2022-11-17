/** @file
  PCH HD Audio Sx handler implementation.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PchInitSmm.h"
#include <Register/PchRegs.h>
#include <Register/HdaRegs.h>
#include <Library/PchPciBdfLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED UINT64  mHdaPciBase;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN mHdaCodecSxWakeCapability = FALSE;

/**
  Updates Codec Sx Wake Capability setting: disabled/enabled
**/
VOID
UpdateHdaCodecSxWakeCapabilitySetting (
  VOID
  )
{
  mHdaPciBase = HdaPciCfgBase ();

  if ((mPchConfigHob->HdAudio.CodecSxWakeCapability == FALSE) ||
      (PciSegmentRead16 (mHdaPciBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF)) {
    mHdaCodecSxWakeCapability = FALSE;
  } else {
    mHdaCodecSxWakeCapability = TRUE;
  }
}

