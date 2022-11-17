/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef MINIBIOS_BUILD
#include <PiPei.h>
#include <Platform.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#endif

#include <SysHost.h>

#define SPEED_REC_96GT     0
#define SPEED_REC_104GT    1
#define ADAPTIVE_CTLE 0x3f

#pragma pack(1)

ALL_LANES_EPARAM_LINK_INFO  KtiMtOlympusAllLanesEparamTable[] = {
  //
  // SocketID, Freq, Link, TXEQL, CTLEPEAK
  //

  //
  // Socket 0
  //
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK0), 0x2E39343F, ADAPTIVE_CTLE},
  {0x0, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK1), 0x2F39353F, ADAPTIVE_CTLE},

  //
  // Socket 1
  //
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK0), 0x2D37353F, ADAPTIVE_CTLE},
  {0x1, (1 << SPEED_REC_96GT) | (1 << SPEED_REC_104GT), (1 << KTI_LINK1), 0x2F3A343F, ADAPTIVE_CTLE}
};

#pragma pack()

UINT32 KtiMtOlympusAllLanesEparamTableSize = sizeof(KtiMtOlympusAllLanesEparamTable);
