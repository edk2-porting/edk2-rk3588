/** @file
  The definition for VTD PMR Regions Information Hob.

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/


#ifndef _VTD_PMR_INFO_HOB_H_
#define _VTD_PMR_INFO_HOB_H_

///
/// This interface is to report the PMR regions information
/// PMR regions means PLMR/PHMR base and limit
/// When gVtdPmrInfoDataHobGuid exists, it means:
///  1. Dma buffer is reserved by memory initialize code
///  2. PeiGetVtdPmrAlignmentLib is used to get alignment
///  3. PMR regions are determined by the system memory map
///  4. PMR regions will be conveyed through VTD_PMR_INFO_HOB
///
typedef struct {
  UINT32             ProtectedLowBase;   //PLMR Base
  UINT32             ProtectedLowLimit;  //PLMR Limit
  UINT64             ProtectedHighBase;  //PHMR Base
  UINT64             ProtectedHighLimit; //PHMR Limit
} VTD_PMR_INFO_HOB;

#endif // _VTD_PMR_INFO_HOB_H_

