/* $NoKeywords */
/**
 * @file
 *
 * GionbPpi.h
 *
 * GioNb Protocol-Protocol Interface header file.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: FDK
 * @e sub-project: UEFI
 * @e version: $Revision: 334098 $ @e date: $Date: 2016-01-08 14:21:15 -0600 (Fri, 08 Jan 2016) $
 *
 */
/*****************************************************************************
*
*  Copyright 2013 - 2016 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
*
*  This program and the accompanying materials are licensed and made available
*  under the terms and conditions of the BSD License which accompanies this
*  distribution. The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR
*  IMPLIED.
*
***************************************************************************/


#ifndef _PEI_GIONB_PPI_H_
#define _PEI_GIONB_PPI_H_

///
/// Global ID for the PEI_GIONB_PPI.
///
#define PEI_GIONB_PPI_GUID \
{ \
  0x24b8ebcc, 0x3871, 0x4b39, { 0xaa, 0x1a, 0xf, 0x86, 0x7d, 0xbf, 0x97, 0xc6 } \
}

///
/// Forward declaration for the PEI_CAPSULE_PPI.
///
typedef struct _EFI_PEI_GIONB_PPI EFI_PEI_GIONB_PPI;

/**
 *---------------------------------------------------------------------------------------
 *
 *  PEI_INIT_GIONB_REGISTERS
 *
 *  Description:
 *    Initialize GIONB registers.
 *
 *  Parameters:
 *    @param[in]  **PeiServices        Pointer to the PEI
 *                                     Services Table.
 *
 *    @return     EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
typedef
EFI_STATUS
(EFIAPI *PEI_INIT_GIONB_REGISTERS)(
  IN CONST  EFI_PEI_SERVICES   **PeiServices
  );

///
/// This PPI provides several services in PEI to initialize and configure GIO NB registers.
///
struct _EFI_PEI_GIONB_PPI {
  PEI_INIT_GIONB_REGISTERS       GioNbEarlyInit;
};

extern EFI_GUID gPeiGionbPpiGuid;

#endif // #ifndef _PEI_GIONB_PPI_H_
