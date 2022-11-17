/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/


#ifndef _MEMORY_SUBCLASS_DRIVER_H
#define _MEMORY_SUBCLASS_DRIVER_H

#include <Uefi.h>
#include <PiDxe.h>
#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBios.h>
#include <Library/HiiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>

#include <Library/HwMemInitLib.h>
#include <Guid/DebugMask.h>
#include <Guid/MemoryMapData.h>
#include <Library/PlatformSysCtrlLib.h>
#include <Library/OemMiscLib.h>

//
// This is the generated header file which includes whatever needs to be exported (strings + IFR)
//

extern UINT8 MemorySubClassStrings[];

struct SPD_JEDEC_MANUFACTURER
{
    UINT8  MfgIdLSB;
    UINT8  MfgIdMSB;
    CHAR16 *Name;
};

struct SPD_JEDEC_MANUFACTURER JEP106[] = {
    {    0, 0x10, L"NEC"},
    {    0, 0x2c, L"Micron"},
    {    0, 0x3d, L"Tektronix"},
    {    0, 0x97, L"TI"},
    {    0, 0xad, L"Hynix"},
    {    0, 0xb3, L"IDT"},
    {    0, 0xc1, L"Infineon"},
    {    0, 0xce, L"Samsung"},
    {    1, 0x94, L"Smart"},
    {    1, 0x98, L"Kingston"},
    {    2, 0xc8, L"Agilent"},
    {    2, 0xfe, L"Elpida"},
    {    3, 0x0b, L"Nanya"},
    {    4, 0x43, L"Ramaxel"},
    {    4, 0xb3, L"Inphi"},
    {    5, 0x51, L"Qimonda"},
    {    5, 0x57, L"AENEON"},
    { 0xFF, 0xFF, L""}
};



#endif
