/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/CpldIoLib.h>


VOID WriteCpldReg(UINTN ulRegAddr, UINT8 ulValue)
{
    MmioWrite8 (ulRegAddr + PcdGet64(PcdCpldBaseAddress), ulValue);
}


UINT8 ReadCpldReg(UINTN ulRegAddr)
{
    return MmioRead8 (ulRegAddr + PcdGet64(PcdCpldBaseAddress));
}


VOID ReadCpldBytes(UINT16 Addr, UINT8 *Data, UINT8 Bytes)
{
    UINT8 i;

    for(i = 0;i < Bytes; i++)
    {
        *(Data + i) = ReadCpldReg(Addr + i);
    }
}

VOID WriteCpldBytes(UINT16 Addr, UINT8 *Data, UINT8 Bytes)
{
    UINT8 i;

    for(i = 0; i < Bytes; i++)
    {
        WriteCpldReg(Addr + i, *(Data + i));
    }
}
