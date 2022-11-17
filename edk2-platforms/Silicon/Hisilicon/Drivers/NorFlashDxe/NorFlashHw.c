/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>
#include "NorFlashHw.h"


BOOLEAN  gFlashBusy = FALSE;
FLASH_INDEX gIndex = {
    0,
    0,
    0,
    0,
    0,
    0
};


UINT32 PortReadData (
    UINT32 Index,
    UINT32 FlashAddr
  )
{

    switch (gFlashInfo[Index].ParallelNum)
    {
        case 2:
            return MmioRead32 (FlashAddr);
        case 1:
            return MmioRead16 (FlashAddr);

        default:
            DEBUG ((EFI_D_ERROR, "[%a]:[%dL]:illegal PortWidth!\n", __FUNCTION__,__LINE__));
            return 0xffffffff;
    }
}

EFI_STATUS
PortWriteData (
    UINT32 Index,
    UINT32 FlashAddr,
    UINT32 InputData
  )
{

    switch (gFlashInfo[Index].ParallelNum)
    {
        case 2:
             MmioWrite32 (FlashAddr, InputData);
             break;
        case 1:
             MmioWrite16 (FlashAddr, InputData);
             break;
        default:
             DEBUG ((EFI_D_ERROR, "[%a]:[%dL]:illegal PortWidth!\n", __FUNCTION__,__LINE__));
             return EFI_DEVICE_ERROR;
    }
    return EFI_SUCCESS;
}

UINT32 PortAdjustData(
    UINT32 Index,
    UINT32 ulInputData
  )
{

    switch (gFlashInfo[Index].ParallelNum)
    {
        case 2:
             return ulInputData;
        case 1:
             return (0x0000ffff & ulInputData );
        default:
            DEBUG((EFI_D_ERROR,"[FLASH_S29GL256N_PortAdjustData]: Error--illegal g_ulFlashS29Gl256NPortWidth!\n\r"));
            return 0xffffffff;
    }
}


EFI_STATUS GetCommandIndex(
    UINT32 Index
  )
{
    UINT32 CommandCount = 0;
    UINT32 i;
    UINT8 Flag = 1;

    CommandCount = sizeof(gFlashCommandReset) / sizeof(FLASH_COMMAND_RESET);
    for(i = 0;i < CommandCount; i ++ )
    {
        if(gFlashInfo[Index].CommandType & gFlashCommandReset[i].CommandType)
        {
            Flag = 0;
            gIndex.ReIndex = i;
            break;
        }
    }

    if(Flag)
    {
        DEBUG ((EFI_D_ERROR, "[%a]:[%dL]:Can not Get Reset Command!\n", __FUNCTION__,__LINE__));
        return EFI_DEVICE_ERROR;
    }

    CommandCount = sizeof(gFlashCommandId) / sizeof(FLASH_COMMAND_ID);
    for(Flag = 1,i = 0;i < CommandCount; i ++ )
    {
        if(gFlashInfo[Index].CommandType & gFlashCommandId[i].CommandType)
        {
            Flag = 0;
            gIndex.IdIndex = i;
            break;
        }
    }

    if(Flag)
    {
        DEBUG ((EFI_D_ERROR, "[%a]:[%dL]:Can not Get ID Command!\n", __FUNCTION__,__LINE__));
        return EFI_DEVICE_ERROR;
    }

    CommandCount = sizeof(gFlashCommandWrite) / sizeof(FLASH_COMMAND_WRITE);
    for(Flag = 1, i = 0;i < CommandCount; i ++ )
    {
        if(gFlashInfo[Index].CommandType & gFlashCommandWrite[i].CommandType)
        {
            Flag = 0;
            gIndex.WIndex = i;
            break;
        }
    }

    if(Flag)
    {
        DEBUG ((EFI_D_ERROR, "[%a]:[%dL]:Can not Get Write Command!\n", __FUNCTION__,__LINE__));
        return EFI_DEVICE_ERROR;
    }

    CommandCount = sizeof(gFlashCommandErase) / sizeof(FLASH_COMMAND_ERASE);
    for(Flag = 1, i = 0;i < CommandCount; i ++ )
    {
        if(gFlashInfo[Index].CommandType & gFlashCommandErase[i].CommandType)
        {
            Flag = 0;
            gIndex.WIndex = i;
            break;
        }
    }

    if(Flag)
    {
        DEBUG ((EFI_D_ERROR, "[%a]:[%dL]:Can not Get Erase Command!\n", __FUNCTION__,__LINE__));
        return EFI_DEVICE_ERROR;
    }

    return EFI_SUCCESS;
}


VOID FlashReset(UINT32 Base)
{
    (VOID)PortWriteData(gIndex.InfIndex, Base, gFlashCommandReset[gIndex.ReIndex].ResetData);
    (void)gBS->Stall(20000);
}


void GetManufacturerID(UINT32 Index, UINT32 Base, UINT8 *pbyData)
{

    UINT32 dwAddr;

    FlashReset(Base);

    dwAddr = Base +  (gFlashCommandId[gIndex.IdIndex].ManuIDAddressStep1 << gFlashInfo[Index].ParallelNum);
    (VOID)PortWriteData(Index, dwAddr, gFlashCommandId[gIndex.IdIndex].ManuIDDataStep1);

    dwAddr = Base + (gFlashCommandId[gIndex.IdIndex].ManuIDAddressStep2 << gFlashInfo[Index].ParallelNum);
    (VOID)PortWriteData(Index, dwAddr, gFlashCommandId[gIndex.IdIndex].ManuIDDataStep2);

    dwAddr = Base + (gFlashCommandId[gIndex.IdIndex].ManuIDAddressStep3 << gFlashInfo[Index].ParallelNum);
    (VOID)PortWriteData(Index, dwAddr, gFlashCommandId[gIndex.IdIndex].ManuIDDataStep3);

    *pbyData = (UINT8)PortReadData(Index, Base + (gFlashCommandId[gIndex.IdIndex].ManuIDAddress << gFlashInfo[Index].ParallelNum));

    FlashReset(Base);    //must reset to return to the read mode
}


EFI_STATUS FlashInit(UINT32 Base)
{
    UINT32 FlashCount = 0;
    UINT32 i = 0;
    EFI_STATUS Status;
    UINT8 Flag = 1;
    UINT32 TempData = 0;
    UINT32 TempDev1 = 0;
    UINT32 TempDev2 = 0;
    UINT32 TempDev3 = 0;
    UINT32 dwAddr;

    FlashCount = sizeof(gFlashInfo) / sizeof(NOR_FLASH_INFO_TABLE);
    for(;i < FlashCount; i ++ )
    {

        Status = GetCommandIndex(i);
        if (EFI_ERROR(Status))
         {
             DEBUG ((EFI_D_ERROR, "[%a]:[%dL]:Get Command Index %r!\n", __FUNCTION__,__LINE__, Status));
             return Status;
         }

        FlashReset(Base);

        dwAddr = Base +  (gFlashCommandId[gIndex.IdIndex].ManuIDAddressStep1 << gFlashInfo[i].ParallelNum);
        (VOID)PortWriteData(i, dwAddr, gFlashCommandId[gIndex.IdIndex].ManuIDDataStep1);

        dwAddr = Base + (gFlashCommandId[gIndex.IdIndex].ManuIDAddressStep2 << gFlashInfo[i].ParallelNum);
        (VOID)PortWriteData(i, dwAddr, gFlashCommandId[gIndex.IdIndex].ManuIDDataStep2);

        dwAddr = Base + (gFlashCommandId[gIndex.IdIndex].ManuIDAddressStep3 << gFlashInfo[i].ParallelNum);
        (VOID)PortWriteData(i, dwAddr, gFlashCommandId[gIndex.IdIndex].ManuIDDataStep3);
        //Get manufacture ID
        TempData = PortReadData(i, Base + (gFlashCommandId[gIndex.IdIndex].ManuIDAddress << gFlashInfo[i].ParallelNum));

        //Get Device Id
        TempDev1 = PortReadData(i, Base + (gFlashCommandId[gIndex.IdIndex].DeviceIDAddress1 << gFlashInfo[i].ParallelNum));
        TempDev2 = PortReadData(i, Base + (gFlashCommandId[gIndex.IdIndex].DeviceIDAddress2 << gFlashInfo[i].ParallelNum));
        TempDev3 = PortReadData(i, Base + (gFlashCommandId[gIndex.IdIndex].DeviceIDAddress3 << gFlashInfo[i].ParallelNum));
        DEBUG ((EFI_D_ERROR, "[cdtest]manufactor ID 0x%x!\n",TempData));
        DEBUG ((EFI_D_ERROR, "[cdtest]Device ID 1 0x%x!\n",TempDev1));
        DEBUG ((EFI_D_ERROR, "[cdtest]Device ID 2 0x%x!\n",TempDev2));
        DEBUG ((EFI_D_ERROR, "[cdtest]Device ID 3 0x%x!\n",TempDev3));

        FlashReset(Base);


        if((0xffffffff != TempData)
            && (PortAdjustData(i, gFlashInfo[i].ManufacturerID) == TempData))
        {
            if((0xffffffff != TempDev1)
                && (PortAdjustData(i, gFlashInfo[i].DeviceID1) == TempDev1))
            {
                if((0xffffffff != TempDev2)
                    && (PortAdjustData(i, gFlashInfo[i].DeviceID2) == TempDev2))
                {
                    if((0xffffffff != TempDev3)
                        && (PortAdjustData(i, gFlashInfo[i].DeviceID3) == TempDev3))
                    {
                        Flag = 0;
                        gIndex.InfIndex = i;
                        break;
                    }
                }
            }
         }
    }

    if(Flag)
    {
        return EFI_DEVICE_ERROR;
    }

    return EFI_SUCCESS;
}


static BOOLEAN width8IsAll(
    const UINT64       Base,
    const UINT64       Offset,
    const UINT64       Length,
    const UINT8        Value
)
{
    UINT64 NewAddr = Base + Offset;
    UINT64 NewLength = Length;
    while (NewLength --)
    {
        if (*(UINT8 *)(UINTN)NewAddr == Value)
        {
            NewAddr ++;
            continue;
        }
        else
        {
            return FALSE;
        }
    }
    return TRUE;
}



EFI_STATUS BufferWriteCommand(UINTN Base, UINTN Offset, void *pData)
{
    UINT32 dwCommAddr;
    UINT32 *pdwData;
    UINT16 *pwData;
    UINT32 dwLoop;
    UINT32 ulWriteWordCount;
    UINT32 dwAddr;

    if(gFlashBusy)
    {
        DEBUG((EFI_D_ERROR, "[%a]:[%dL]:Flash is busy!\n", __FUNCTION__,__LINE__));
        return EFI_NOT_READY;
    }
    gFlashBusy = TRUE;

    if(2 == gFlashInfo[gIndex.InfIndex].ParallelNum)
    {
        pdwData = (UINT32 *)pData;

        dwAddr = (UINT32)Base + (gFlashCommandWrite[gIndex.WIndex].BufferProgramAddressStep1 << gFlashInfo[gIndex.InfIndex].ParallelNum);
        (VOID)PortWriteData(gIndex.InfIndex, dwAddr, gFlashCommandWrite[gIndex.WIndex].BufferProgramDataStep1);

        dwAddr = (UINT32)Base + (gFlashCommandWrite[gIndex.WIndex].BufferProgramAddressStep2 << gFlashInfo[gIndex.InfIndex].ParallelNum);
        (VOID)PortWriteData(gIndex.InfIndex, dwAddr,  gFlashCommandWrite[gIndex.WIndex].BufferProgramDataStep2);

        //dwAddr = Base + (Offset << gFlashInfo[gIndex.InfIndex].ParallelNum);
        dwAddr = (UINT32)Base + Offset;
        (VOID)PortWriteData(gIndex.InfIndex, dwAddr, gFlashCommandWrite[gIndex.WIndex].BufferProgramDataStep3);


       ulWriteWordCount = ((gFlashInfo[gIndex.InfIndex].BufferProgramSize - 1) << 16) | (gFlashInfo[gIndex.InfIndex].BufferProgramSize - 1);
       (VOID)PortWriteData(gIndex.InfIndex, dwAddr, ulWriteWordCount);


       for (dwLoop = 0; dwLoop < gFlashInfo[gIndex.InfIndex].BufferProgramSize; dwLoop ++)
       {
           dwCommAddr = (UINT32)Base + (UINT32)Offset + (dwLoop << gFlashInfo[gIndex.InfIndex].ParallelNum);
           MmioWrite32 (dwCommAddr, *pdwData);
           pdwData ++;
       }

       dwAddr = (UINT32)Base + (UINT32)Offset + ((gFlashInfo[gIndex.InfIndex].BufferProgramSize - 1) << gFlashInfo[gIndex.InfIndex].ParallelNum);
       (VOID)PortWriteData(gIndex.InfIndex, dwAddr, gFlashCommandWrite[gIndex.WIndex].BufferProgramtoFlash);



    }
    else
    {
        pwData  = (UINT16 *)pData;

        dwAddr = (UINT32)Base + (gFlashCommandWrite[gIndex.WIndex].BufferProgramAddressStep1 << gFlashInfo[gIndex.InfIndex].ParallelNum);
        (VOID)PortWriteData(gIndex.InfIndex, dwAddr, gFlashCommandWrite[gIndex.WIndex].BufferProgramDataStep1);

        dwAddr = (UINT32)Base + (gFlashCommandWrite[gIndex.WIndex].BufferProgramAddressStep2 << gFlashInfo[gIndex.InfIndex].ParallelNum);
        (VOID)PortWriteData(gIndex.InfIndex, dwAddr,  gFlashCommandWrite[gIndex.WIndex].BufferProgramDataStep2);

        //dwAddr = Base + (Offset << gFlashInfo[gIndex.InfIndex].ParallelNum);
        dwAddr = (UINT32)Base + Offset;
        (VOID)PortWriteData(gIndex.InfIndex, dwAddr, gFlashCommandWrite[gIndex.WIndex].BufferProgramDataStep3);


        ulWriteWordCount = gFlashInfo[gIndex.InfIndex].BufferProgramSize - 1;
        (VOID)PortWriteData(gIndex.InfIndex, dwAddr, ulWriteWordCount);


        for (dwLoop = 0; dwLoop < gFlashInfo[gIndex.InfIndex].BufferProgramSize; dwLoop ++)
        {
            dwCommAddr = (UINT32)Base + (UINT32)Offset + (dwLoop << gFlashInfo[gIndex.InfIndex].ParallelNum);
            MmioWrite16 (dwCommAddr, *pwData);
            pwData ++;
        }

        dwAddr = (UINT32)Base + (UINT32)Offset + ((gFlashInfo[gIndex.InfIndex].BufferProgramSize - 1) << gFlashInfo[gIndex.InfIndex].ParallelNum);
        (VOID)PortWriteData(gIndex.InfIndex, dwAddr, gFlashCommandWrite[gIndex.WIndex].BufferProgramtoFlash);

    }

    (void)gBS->Stall(200);

    gFlashBusy = FALSE;
    return EFI_SUCCESS;

}


EFI_STATUS SectorEraseCommand(UINTN Base, UINTN Offset)
{
    UINT32 dwAddr;

    if(gFlashBusy)
    {
        DEBUG((EFI_D_ERROR, "[%a]:[%dL]:Flash is busy!\n", __FUNCTION__,__LINE__));
        return EFI_NOT_READY;
    }

    gFlashBusy = TRUE;

    dwAddr = (UINT32)Base + (gFlashCommandErase[gIndex.EIndex].SectorEraseAddressStep1 << gFlashInfo[gIndex.InfIndex].ParallelNum);
    (VOID)PortWriteData(gIndex.InfIndex, dwAddr, gFlashCommandErase[gIndex.EIndex].SectorEraseDataStep1);

    dwAddr = (UINT32)Base + (gFlashCommandErase[gIndex.EIndex].SectorEraseAddressStep2 << gFlashInfo[gIndex.InfIndex].ParallelNum);
    (VOID)PortWriteData(gIndex.InfIndex, dwAddr, gFlashCommandErase[gIndex.EIndex].SectorEraseDataStep2);

    dwAddr = (UINT32)Base + (gFlashCommandErase[gIndex.EIndex].SectorEraseAddressStep3 << gFlashInfo[gIndex.InfIndex].ParallelNum);
    (VOID)PortWriteData(gIndex.InfIndex, dwAddr, gFlashCommandErase[gIndex.EIndex].SectorEraseDataStep3);

    dwAddr = (UINT32)Base + (gFlashCommandErase[gIndex.EIndex].SectorEraseAddressStep4 << gFlashInfo[gIndex.InfIndex].ParallelNum);
    (VOID)PortWriteData(gIndex.InfIndex, dwAddr, gFlashCommandErase[gIndex.EIndex].SectorEraseDataStep4);

    dwAddr = (UINT32)Base + (gFlashCommandErase[gIndex.EIndex].SectorEraseAddressStep5 << gFlashInfo[gIndex.InfIndex].ParallelNum);
    (VOID)PortWriteData(gIndex.InfIndex, dwAddr, gFlashCommandErase[gIndex.EIndex].SectorEraseDataStep5);

    dwAddr = (UINT32)Base + Offset;
    (VOID)PortWriteData(gIndex.InfIndex, dwAddr, gFlashCommandErase[gIndex.EIndex].SectorEraseDataStep6);

    (void)gBS->Stall(500000);

    gFlashBusy = FALSE;
    return EFI_SUCCESS;
}


EFI_STATUS CompleteCheck(UINT32 Base, UINT32 Offset, void *pData, UINT32 Length)
{
    UINT32 dwTestAddr;
    UINT32 dwTestData;
    UINT32 dwTemp = 0;
    UINT32 dwTemp1 = 0;
    UINT32 i;
    UINT32 dwTimeOut = 3000000;

    if(gFlashBusy)
    {
        DEBUG((EFI_D_ERROR, "[%a]:[%dL]:Flash is busy!\n", __FUNCTION__,__LINE__));
        return EFI_NOT_READY;
    }
    gFlashBusy = TRUE;

    if(2 == gFlashInfo[gIndex.InfIndex].ParallelNum)
    {
        dwTestAddr = Base + Offset + Length - sizeof(UINT32);
        dwTestData = *((UINT32 *)((UINT8 *)pData + Length - sizeof(UINT32)));

        while(dwTimeOut--)
        {
            dwTemp1 = MmioRead32 (dwTestAddr);
            if (dwTestData == dwTemp1)
            {
                dwTemp = MmioRead32 (dwTestAddr);
                dwTemp1 = MmioRead32 (dwTestAddr);
                if ((dwTemp == dwTemp1) && (dwTestData == dwTemp1))
                {
                    gFlashBusy = FALSE;
                    return EFI_SUCCESS;
                }
            }

            (void)gBS->Stall(1);
        }

        if((UINT16)(dwTemp1 >> 16) != (UINT16)(dwTestData >> 16))
        {
            DEBUG((EFI_D_ERROR, "CompleteCheck ERROR: chip1 address %x, buffer %x, flash %x!\n", Offset, dwTestData, dwTemp1));
        }
        if((UINT16)(dwTemp1) != (UINT16)(dwTestData))
        {
            DEBUG((EFI_D_ERROR, "CompleteCheck ERROR: chip2 address %x, buffer %x, flash %x!\n", Offset, dwTestData, dwTemp1));
        }
    }
    else
    {
        dwTestAddr = Base + Offset + Length - sizeof(UINT16);
        dwTestData = *((UINT16 *)((UINT8 *)pData + Length - sizeof(UINT16)));

        while(dwTimeOut--)
        {
            dwTemp1 = MmioRead16 (dwTestAddr);
            if (dwTestData == dwTemp1)
            {
                dwTemp = MmioRead16 (dwTestAddr);
                dwTemp1 = MmioRead16 (dwTestAddr);
                if ((dwTemp == dwTemp1) && (dwTestData == dwTemp1))
                {
                    gFlashBusy = FALSE;
                    return EFI_SUCCESS;
                }
            }

            (void)gBS->Stall(1);
        }
    }

    for(i = 0; i < 5; i ++)
    {
        DEBUG((EFI_D_ERROR, "CompleteCheck ERROR: flash %x\n",PortReadData(gIndex.InfIndex, dwTestAddr)));
    }

    FlashReset(Base);

    gFlashBusy = FALSE;
    DEBUG((EFI_D_ERROR, "CompleteCheck ERROR: timeout address %x, buffer %x, flash %x\n", Offset, dwTestData, dwTemp1));
    return EFI_TIMEOUT;
}

EFI_STATUS IsNeedToWrite(
    IN  UINT32         Base,
    IN  UINT32       Offset,
    IN  UINT8       *Buffer,
    IN  UINT32       Length
  )
{
    UINTN NewAddr = Base + Offset;
    UINT8 FlashData = 0;
    UINT8 BufferData = 0;

    for(; Length > 0; Length --)
    {
        BufferData = *Buffer;
        //lint -epn -e511
        FlashData = *(UINT8 *)NewAddr;
        if (BufferData != FlashData)
        {
            return TRUE;
        }
        NewAddr ++;
        Buffer ++;
    }

    return FALSE;
}


EFI_STATUS BufferWrite(UINT32 Offset, void *pData, UINT32 Length)
{
    EFI_STATUS Status;
    UINT32 dwLoop;
    UINT32 Retry = 3;

    if (FALSE == IsNeedToWrite(gIndex.Base, Offset, (UINT8 *)pData, Length))
    {
        return EFI_SUCCESS;
    }

    do
    {
        (void)BufferWriteCommand(gIndex.Base, Offset, pData);
        Status = CompleteCheck(gIndex.Base, Offset, pData, Length);


        if (EFI_SUCCESS == Status)
        {
            for (dwLoop = 0; dwLoop < Length; dwLoop ++)
            {
                if (*(UINT8 *)(UINTN)(gIndex.Base + Offset + dwLoop) != *((UINT8 *)pData + dwLoop))
                {
                    DEBUG((EFI_D_ERROR, "Flash_WriteUnit ERROR: address %x, buffer %x, flash %x\n", Offset, *((UINT8 *)pData + dwLoop), *(UINT8 *)(UINTN)(gIndex.Base + Offset + dwLoop)));
                    Status = EFI_ABORTED;
                    continue;
                }
            }
        }
        else
        {
            DEBUG((EFI_D_ERROR, "Flash_WriteUnit ERROR: complete check failed, %r\n", Status));
            continue;
        }
    } while ((Retry--) && EFI_ERROR(Status));

    return Status;
}


EFI_STATUS SectorErase(UINT32 Base, UINT32 Offset)
{
    UINT8 gTemp[FLASH_MAX_UNIT];
    UINT64 dwLoop = FLASH_MAX_UNIT - 1;
    UINT32 Retry = 3;
    EFI_STATUS Status;

    do
    {
        gTemp[dwLoop] = 0xFF;
    }while (dwLoop --);

    do
    {
        (void)SectorEraseCommand(Base, Offset);
        Status = CompleteCheck(Base, Offset, (void *)gTemp, FLASH_MAX_UNIT);


        if (EFI_SUCCESS == Status)
        {

            if (width8IsAll(Base,Offset - (Offset % gFlashInfo[gIndex.InfIndex].BlockSize), gFlashInfo[gIndex.InfIndex].BlockSize, 0xFF))
            {
                return EFI_SUCCESS;
            }
            else
            {
                DEBUG((EFI_D_ERROR, "Flash_SectorErase ERROR: not all address equal 0xFF\n"));

                Status = EFI_ABORTED;
                continue;
            }
        }
        else
        {
            DEBUG((EFI_D_ERROR, "Flash_SectorErase ERROR: complete check failed, %r\n", Status));
            continue;
        }
    }while ((Retry--) && EFI_ERROR(Status));

    if(Retry)
    {
        //do nothing for pclint
    }

    return Status;
}
