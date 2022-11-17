/** @file
  VT-d policy definitions.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _VTD_CONFIG_H_
#define _VTD_CONFIG_H_

#include <Library/VtdInfoLib.h>
#pragma pack(push, 1)

#define VTD_CONFIG_REVISION 1
#define VTD_DXE_CONFIG_REVISION 2

/**
  The data elements should be initialized by a Platform Module.
  The data structure is for VT-d driver initialization\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;                      ///< Offset 0-27 Config Block Header
  /**
    Offset 28:
    VT-D Support can be verified by reading CAP ID register as expalined in BIOS Spec.
    This policy is for debug purpose only.
    If VT-D is not supported, all other policies in this config block will be ignored.
    <b>0 = To use Vt-d</b>;
    1 = Avoids programming Vtd bars, Vtd overrides and DMAR table.
  **/
  UINT8        VtdDisable;
  UINT8        X2ApicOptOut;        ///< Offset 29 :This field is used to enable the X2APIC_OPT_OUT bit in the DMAR table. 1=Enable/Set and <b>0=Disable/Clear</b>
  UINT8        DmaControlGuarantee; ///< Offset 30 :This field is used to enable the DMA_CONTROL_GUARANTEE bit in the DMAR table. 1=Enable/Set and <b>0=Disable/Clear</b>
  UINT8        VtdIgdEnable;        ///< Offset 31 :This field is used to enable the VtdIgdEnable Policy. 1=Enable/Set and <b>0=Disable/Clear</b>
  UINT8        VtdIpuEnable;        ///< Offset 32 :This field is used to enable the VtdIpuEnable Policy. 1=Enable/Set and <b>0=Disable/Clear</b>
  UINT8        VtdIopEnable;        ///< Offset 33 :This field is used to enable the VtdIopEnable Policy. 1=Enable/Set and <b>0=Disable/Clear</b>
  UINT8        VtdItbtEnable;       ///< Offset 34 :This field is used to enable the VtdItbtEnable Policy. 1=Enable/Set and <b>0=Disable/Clear</b>
  UINT8        PreBootDmaMask;      ///< Offset 35 :Convey PcdVTdPolicyPropertyMask value from EDK2 IntelSiliconPkg
  /**
    Offset 36:
    This field is used to describe the base addresses for VT-d function:\n
    VTD BAR for Gfx if IGfx is supported : <b>BaseAddress[0]=0xFED90000,\n
    VTD BAR for IPU if IPU is supporrted : BaseAddress[1]=0xFED92000,\n
    VTD BAR for other DMA Agents (except Igfx and IPU) : BaseAddress[2]=0xFED91000,\n
    VTD BAR for iTBT if iTBT is supported : BaseAddress[3]=0xFED84000, BaseAddress[4]=0xFED85000, BaseAddress[5]=0xFED86000,BaseAddress[6]=0xFED87000</b>
  **/
  UINT32       BaseAddress[VTD_ENGINE_NUMBER];
  UINT32       DmaBufferSize;      ///< Offset 64 :Protect Memory Region (PMR) DMA buffer size


} VTD_CONFIG;

/**
  The data structure is for VT-d driver initialization in DXE\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                    ///< Offset 0-27 Config Block Header
} VTD_DXE_CONFIG;
#pragma pack(pop)

#endif   //  _VTD_CONFIG_H_
