/*******************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#ifndef __MVCOMPHYLIB_H__
#define __MVCOMPHYLIB_H__

typedef enum {
  MvComPhyTypeCp110,
  MvComPhyTypeMax,
} MV_COMPHY_CHIP_TYPE;

EFI_STATUS
MvComPhyInit (
  VOID
  );

#endif
