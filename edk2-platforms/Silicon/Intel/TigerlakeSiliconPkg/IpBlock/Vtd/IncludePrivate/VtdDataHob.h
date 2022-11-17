/** @file
  The GUID definition for Vtd Data Hob

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _VTD_DATA_HOB_H_
#define _VTD_DATA_HOB_H_

#include <Library/VtdInfoLib.h>
#include <Base.h>

extern EFI_GUID gVtdDataHobGuid;

#pragma pack (push,1)

///
/// The data elements should be initialized by a Platform Module.
/// The data structure is for VT-d driver initialization
///
typedef struct {
  EFI_HOB_GUID_TYPE     EfiHobGuidType;                    ///< GUID Hob type structure for gVtdDataHobGuid
  BOOLEAN               VtdDisable;                        ///< 1 = Avoids programming Vtd bars, Vtd overrides and DMAR table
  UINT32                BaseAddress[VTD_ENGINE_NUMBER];    ///< This field is used to describe the base addresses for VT-d function
  BOOLEAN               X2ApicOptOut;                      ///< This field is used to enable the X2APIC_OPT_OUT bit in the DMAR table. <b>1=Enable/Set</b> and 0=Disable/Clear
  BOOLEAN               DmaControlGuarantee;               ///< This field is used to enable the DMA_CONTROL_GUARANTEE bit in the DMAR table. <b>1=Enable/Set</b> and 0=Disable/Clear
  BOOLEAN               InterruptRemappingSupport;         ///< This field is used to indicate Interrupt Remapping supported or not
  UINT32                DmaBufferBase;                     ///< Iommu PEI DMA buffer base in low memory region, in Mbytes units
} VTD_DATA_HOB;

#pragma pack (pop)
#endif
