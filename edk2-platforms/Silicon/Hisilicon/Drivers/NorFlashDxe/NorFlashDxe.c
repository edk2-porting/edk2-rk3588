/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/


#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/ArmLib.h>
#include <Library/PcdLib.h>
#include <Protocol/NorFlashProtocol.h>
#include <Library/DxeServicesTableLib.h>
#include <Protocol/Cpu.h>
#include "NorFlashHw.h"


EFI_STATUS Erase(
   IN UNI_NOR_FLASH_PROTOCOL   *This,
   IN  UINT32                   Offset,
   IN  UINT32                   Length
  );

EFI_STATUS  Write(
  IN UNI_NOR_FLASH_PROTOCOL   *This,
  IN  UINT32                  Offset,
  IN  UINT8                  *Buffer,
  UINT32                     ulLength
  );

EFI_STATUS Read(
  IN UNI_NOR_FLASH_PROTOCOL   *This,
  IN UINT32                   Offset,
  IN OUT UINT8               *Buffer,
  IN UINT32                   ulLen
  );

UNI_NOR_FLASH_PROTOCOL gUniNorFlash = {
    Erase,
    Write,
    Read
};


EFI_STATUS
EFIAPI Read(
  IN UNI_NOR_FLASH_PROTOCOL   *This,
  IN UINT32                   Offset,
  IN OUT UINT8               *Buffer,
  IN UINT32                    ulLen
  )
{
    UINT32       index;
    UINT64 ullAddr;
    UINT32 ullCnt = 0;
    UINT32 *puiBuffer32 = NULL;
    UINT32 *puiDst32 = NULL;
    UINT8 *pucBuffer8 = NULL;
    UINT8 *pucDst8 = NULL;

    if (Offset + ulLen > (gFlashInfo[gIndex.InfIndex].SingleChipSize * gFlashInfo[gIndex.InfIndex].ParallelNum))
    {
        DEBUG ((EFI_D_ERROR, "[%a]:[%dL]:Exceed the flash scope!\n", __FUNCTION__,__LINE__));
        return EFI_INVALID_PARAMETER;
    }
    if (0 == ulLen)
    {
        DEBUG ((EFI_D_ERROR, "[%a]:[%dL]:Length is Zero!\n", __FUNCTION__,__LINE__));
        return EFI_INVALID_PARAMETER;
    }
    if (NULL == Buffer)
    {
        DEBUG ((EFI_D_ERROR, "[%a]:[%dL]:Buffer is NULL!\n", __FUNCTION__,__LINE__));
        return EFI_BAD_BUFFER_SIZE;
    }


    ullAddr = gIndex.Base + Offset;

    pucBuffer8 = (UINT8 *)Buffer;
    pucDst8    = (UINT8 *)((UINTN)ullAddr);


    if (ulLen < FOUR_BYTE_UNIT)
    {
        for(index = 0; index< ulLen; index++)
        {
            *pucBuffer8++ = *pucDst8++;
        }
    }
    else
    {

        ullCnt = Offset % FOUR_BYTE_UNIT;
        ullCnt = FOUR_BYTE_UNIT - ullCnt;

        for(index = 0; index < ullCnt; index++)
        {
            *pucBuffer8++ = *pucDst8++;
        }

        ulLen -= ullCnt;

        puiBuffer32 = (UINT32 *)pucBuffer8;
        puiDst32    = (UINT32 *)pucDst8;
        ullCnt      = ulLen / FOUR_BYTE_UNIT;

        for(index = 0; index < ullCnt; index++)
        {
            *puiBuffer32++ = *puiDst32++;
        }

        ullCnt     = ulLen % FOUR_BYTE_UNIT;
        pucBuffer8 = (UINT8 *)puiBuffer32;
        pucDst8    = (UINT8 *)puiDst32;

        for(index = 0; index < ullCnt; index++)
        {
            *pucBuffer8++ = *pucDst8++;
        }
    }

    return EFI_SUCCESS;
}



static EFI_STATUS WriteAfterErase_Fill(
    IN  const UINT32       Offset,
    IN  const UINT8       *Buffer,
    IN  const UINT32       Length
    )
{
    EFI_STATUS Status;
    UINT32 Loop;
    UINT32 DataOffset;
    UINT32 NewOffset;
    UINT8 *NewDataUnit;

    UINT32 FlashUnitLength;

    FlashUnitLength = gFlashInfo[gIndex.InfIndex].BufferProgramSize << gFlashInfo[gIndex.InfIndex].ParallelNum;

    if (0 == Length)
    {
        return EFI_SUCCESS;
    }
    if ((Offset % FlashUnitLength + Length) > FlashUnitLength)
    {
        DEBUG ((EFI_D_INFO, "[%a]:[%dL]:Exceed the Flash Size!\n", __FUNCTION__,__LINE__));
        return EFI_UNSUPPORTED;
    }


    Status = gBS->AllocatePool(EfiBootServicesData, FlashUnitLength, (VOID *)&NewDataUnit);
    if (EFI_ERROR(Status))
    {
        DEBUG ((EFI_D_ERROR, "[%a]:[%dL]:Allocate Pool failed, %r!\n", __FUNCTION__,__LINE__, Status));
        return Status;
    }


    NewOffset = Offset - (Offset % FlashUnitLength);

    gBS->CopyMem((VOID *)NewDataUnit, (VOID *)(UINTN)(gIndex.Base + NewOffset), FlashUnitLength);

    DataOffset = Offset % FlashUnitLength;
    for (Loop = 0; Loop < Length; Loop ++)
    {
        NewDataUnit[(UINT32)(DataOffset + Loop)] = Buffer[Loop];
    }

    Status = BufferWrite(NewOffset, (void *)NewDataUnit, FlashUnitLength);
    if (EFI_ERROR(Status))
    {
        DEBUG ((EFI_D_ERROR, "[%a]:[%dL]:BufferWrite %r!\n", __FUNCTION__,__LINE__, Status));
        return Status;
    }

    (void)gBS->FreePool((VOID *)NewDataUnit);
    return Status;
}


static EFI_STATUS WriteAfterErase_Final(
    IN  UINT32       Offset,
    IN  UINT8       *Buffer,
    IN  UINT32       Length
    )
{
    EFI_STATUS Status;
    UINT32 Loop;
    UINT32 FlashUnitLength;

    FlashUnitLength = gFlashInfo[gIndex.InfIndex].BufferProgramSize << gFlashInfo[gIndex.InfIndex].ParallelNum;

    if (0 == Length)
    {
        return EFI_SUCCESS;
    }

    if (0 != (Offset % FlashUnitLength))
    {
        DEBUG ((EFI_D_ERROR, "[%a]:[%dL]: Offset must be a multiple of 0x%x!\n", __FUNCTION__,__LINE__,FlashUnitLength));
        return EFI_UNSUPPORTED;
    }


    Loop = Length / FlashUnitLength;
    while (Loop --)
    {
        Status = BufferWrite(Offset, (void *)Buffer, FlashUnitLength);
        if (EFI_ERROR(Status))
        {
            DEBUG ((EFI_D_ERROR, "[%a]:[%dL]:BufferWrite Failed: %r!\n", __FUNCTION__,__LINE__, Status));
            return EFI_DEVICE_ERROR;
        }
        Offset += FlashUnitLength;
        Buffer += FlashUnitLength;
    }


    Length = Length % FlashUnitLength;
    if (Length)
    {
        Status = WriteAfterErase_Fill(Offset, Buffer, Length);
        if (EFI_ERROR(Status))
        {
            DEBUG ((EFI_D_ERROR, "[%a]:[%dL]:WriteAfterErase_Fill failed,%r!\n", __FUNCTION__,__LINE__, Status));
            return Status;
        }
    }

    return EFI_SUCCESS;
}

EFI_STATUS
WriteAfterErase(
    UINT32       TempBase,
    UINT32       Offset,
    UINT8       *Buffer,
    UINT32       Length
  )
{
    EFI_STATUS Status;
    UINT32 FlashUnitLength;

    FlashUnitLength = gFlashInfo[gIndex.InfIndex].BufferProgramSize << gFlashInfo[gIndex.InfIndex].ParallelNum;

    if (0 == Length)
    {
        return EFI_SUCCESS;
    }


    if (Offset % FlashUnitLength)
    {
        UINT32 TempLength;


        TempLength = FlashUnitLength - (Offset % FlashUnitLength);
        if (TempLength > Length)
        {
            TempLength = Length;
        }
        Status = WriteAfterErase_Fill(Offset, Buffer, TempLength);
        if (EFI_ERROR(Status))
        {
            DEBUG ((EFI_D_ERROR, "[%a]:[%dL]: %r!\n", __FUNCTION__,__LINE__, Status));
            return Status;
        }

        Offset += TempLength;
        Length -= TempLength;
        Buffer += TempLength;

        //Desc:if Offset >= gOneFlashSize,modify base
        if (0 < (Offset / gFlashInfo[gIndex.InfIndex].SingleChipSize))
        {
            TempBase += gFlashInfo[gIndex.InfIndex].SingleChipSize;
            gIndex.Base = TempBase;
            Offset = 0;
        }
    }


    Status = WriteAfterErase_Final(Offset, Buffer, Length);
    if (EFI_ERROR(Status))
    {
        DEBUG ((EFI_D_ERROR, "[%a]:[%dL]: %r!\n", __FUNCTION__,__LINE__, Status));
        return Status;
    }

    return EFI_SUCCESS;
}


EFI_STATUS
FlashSectorErase(
    UINT32      TempBase,
    UINT32      Offset,
    UINT32      Length
  )
{
    EFI_STATUS  Status;
    UINT32 SectorOffset;
    UINT8 *StaticBuffer;
    UINT8 *Buffer;
    UINT32 TempOffset;
    UINT32 TempLength;
    UINT32 LeftLength;


    if (0 == Length)
    {
        return EFI_SUCCESS;
    }

    LeftLength = gFlashInfo[gIndex.InfIndex].BlockSize * gFlashInfo[gIndex.InfIndex].ParallelNum - (Offset % (gFlashInfo[gIndex.InfIndex].BlockSize * gFlashInfo[gIndex.InfIndex].ParallelNum));
    if (LeftLength < Length)
    {
        return EFI_UNSUPPORTED;
    }


    SectorOffset = Offset - (Offset % (gFlashInfo[gIndex.InfIndex].BlockSize * gFlashInfo[gIndex.InfIndex].ParallelNum));

    Status = gBS->AllocatePool(EfiBootServicesData, gFlashInfo[gIndex.InfIndex].BlockSize * (UINTN)gFlashInfo[gIndex.InfIndex].ParallelNum, (VOID *)&StaticBuffer);
    if (EFI_ERROR(Status))
    {
        return Status;
    }

    Buffer = StaticBuffer;

    gBS->CopyMem((VOID *)Buffer, (VOID *)(UINTN)(TempBase + SectorOffset),
                 (gFlashInfo[gIndex.InfIndex].BlockSize * (UINTN)gFlashInfo[gIndex.InfIndex].ParallelNum));


    Status = SectorErase(TempBase, SectorOffset);
    if (EFI_ERROR(Status))
    {
        goto DO;
    }


    TempOffset = SectorOffset;
    TempLength = Offset % (gFlashInfo[gIndex.InfIndex].BlockSize * gFlashInfo[gIndex.InfIndex].ParallelNum);

    Status = WriteAfterErase(TempBase, TempOffset, Buffer, TempLength);
    if (EFI_ERROR(Status))
    {
        goto DO;
    }


    Buffer = Buffer + TempLength + Length;
    TempOffset = Offset + Length;
    TempLength = SectorOffset + (gFlashInfo[gIndex.InfIndex].BlockSize * gFlashInfo[gIndex.InfIndex].ParallelNum) - TempOffset;

    Status = WriteAfterErase(TempBase, TempOffset, Buffer, TempLength);
    if (EFI_ERROR(Status))
    {
        DEBUG ((EFI_D_ERROR, "[%a]:[%dL]: %r!\n", __FUNCTION__,__LINE__,Status));
        goto DO;
    }

    (void)gBS->FreePool((VOID *)StaticBuffer);
    return EFI_SUCCESS;

DO:
    (void)gBS->FreePool((VOID *)StaticBuffer);
    return Status;
}


EFI_STATUS
EFIAPI Erase(
   IN UNI_NOR_FLASH_PROTOCOL   *This,
   IN UINT32                   Offset,
   IN UINT32                   Length
  )
{
    EFI_STATUS  Status = EFI_SUCCESS;
    UINT32 Sectors;
    UINT32 TempLength;
    UINT32 TempBase;
    UINT32 Loop;


    if (Offset + Length > (gFlashInfo[gIndex.InfIndex].SingleChipSize * gFlashInfo[gIndex.InfIndex].ParallelNum))
    {
        DEBUG ((EFI_D_ERROR, "[%a]:[%dL]:Exceed the Flash Size!\n", __FUNCTION__,__LINE__));
        return EFI_ABORTED;
    }
    if (0 == Length)
    {
        return EFI_SUCCESS;
    }


    Sectors = ((Offset + Length - 1) / (gFlashInfo[gIndex.InfIndex].BlockSize * gFlashInfo[gIndex.InfIndex].ParallelNum)) - (Offset / (gFlashInfo[gIndex.InfIndex].BlockSize * gFlashInfo[gIndex.InfIndex].ParallelNum)) + 1;
    TempBase = gIndex.Base;

    //if Offset >= gOneFlashSize,modify base
    if(0 < (Offset / gFlashInfo[gIndex.InfIndex].SingleChipSize))
    {
        TempBase +=  gFlashInfo[gIndex.InfIndex].SingleChipSize * (Offset/gFlashInfo[gIndex.InfIndex].SingleChipSize);
        Offset = Offset - (Offset & gFlashInfo[gIndex.InfIndex].SingleChipSize);
    }

    for (Loop = 0; Loop <= Sectors; Loop ++)
    {

        TempLength = gFlashInfo[gIndex.InfIndex].BlockSize * gFlashInfo[gIndex.InfIndex].ParallelNum - (Offset % (gFlashInfo[gIndex.InfIndex].BlockSize * gFlashInfo[gIndex.InfIndex].ParallelNum));


        if (TempLength > Length)
        {
            TempLength = Length;
        }

        Status = FlashSectorErase(TempBase, Offset, TempLength);
        if (EFI_ERROR(Status))
        {
            DEBUG ((EFI_D_ERROR, "[%a]:[%dL]: FlashErase One Sector Error, Status = %r!\n", __FUNCTION__,__LINE__,Status));
            return Status;
        }

        Offset += TempLength;

         //if Offset >= gOneFlashSize,modify base
        if (0 < (Offset / gFlashInfo[gIndex.InfIndex].SingleChipSize))
        {
            TempBase += gFlashInfo[gIndex.InfIndex].SingleChipSize;
            Offset = 0;
        }
        Length -= TempLength;
    }

    return Status;
}


EFI_STATUS
EFIAPI Write(
  IN UNI_NOR_FLASH_PROTOCOL   *This,
  IN UINT32                   Offset,
  IN UINT8                   *Buffer,
  UINT32                     ulLength
  )
{
    EFI_STATUS  Status;
    UINT32     TempLength;
    UINT32       TempBase;
    UINT32           Loop;
    UINT32        Sectors;

    if((Offset + ulLength) > (gFlashInfo[gIndex.InfIndex].SingleChipSize * gFlashInfo[gIndex.InfIndex].ParallelNum))
    {
        DEBUG ((EFI_D_ERROR, "[%a]:[%dL]:Exceed the Flash Size!\n", __FUNCTION__,__LINE__));
        return EFI_INVALID_PARAMETER;
    }
    if (0 == ulLength)
    {
        return EFI_SUCCESS;
    }


    Sectors = ((Offset + ulLength - 1) / (gFlashInfo[gIndex.InfIndex].BlockSize * gFlashInfo[gIndex.InfIndex].ParallelNum)) - (Offset / (gFlashInfo[gIndex.InfIndex].BlockSize * gFlashInfo[gIndex.InfIndex].ParallelNum)) + 1;
    TempBase = gIndex.Base;

    //if Offset >= gOneFlashSize,modify base
    if(0 < (Offset / gFlashInfo[gIndex.InfIndex].SingleChipSize))
    {
        TempBase +=  gFlashInfo[gIndex.InfIndex].SingleChipSize * (Offset/gFlashInfo[gIndex.InfIndex].SingleChipSize);
        Offset = Offset - (Offset & gFlashInfo[gIndex.InfIndex].SingleChipSize);
    }

    for (Loop = 0; Loop <= Sectors; Loop ++)
    {

        TempLength = gFlashInfo[gIndex.InfIndex].BlockSize * gFlashInfo[gIndex.InfIndex].ParallelNum - (Offset % (gFlashInfo[gIndex.InfIndex].BlockSize * gFlashInfo[gIndex.InfIndex].ParallelNum));


        if (TempLength > ulLength)
        {
            TempLength = ulLength;
        }


        if (TRUE == IsNeedToWrite(TempBase, Offset, Buffer, TempLength))
        {
            Status = FlashSectorErase(TempBase, Offset, TempLength);
            if (EFI_ERROR(Status))
            {
                DEBUG ((EFI_D_ERROR, "[%a]:[%dL]:FlashErase One Sector Error, Status = %r!\n", __FUNCTION__,__LINE__,Status));
                return Status;
            }


            Status = WriteAfterErase(TempBase, Offset, Buffer, TempLength);
            if (EFI_ERROR(Status))
            {
                DEBUG ((EFI_D_ERROR, "[%a]:[%dL]:WriteAfterErase Status = %r!\n", __FUNCTION__,__LINE__,Status));
                return Status;
            }
        }

        Offset += TempLength;
        Buffer += TempLength;

         //if Offset >= gOneFlashSize,modify base
        if (0 < (Offset / gFlashInfo[gIndex.InfIndex].SingleChipSize))
        {
            TempBase += gFlashInfo[gIndex.InfIndex].SingleChipSize;
            Offset = 0;
        }
        ulLength -= TempLength;
    }

    return EFI_SUCCESS;
}


VOID SetFlashAttributeToUncache(VOID)
{
    EFI_CPU_ARCH_PROTOCOL             *gCpu           = NULL;
    EFI_STATUS Status;

    Status = gBS->LocateProtocol (&gEfiCpuArchProtocolGuid, NULL, (VOID **)&gCpu);
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "LocateProtocol gEfiCpuArchProtocolGuid Status = %r !\n", Status));
    }

    Status = gCpu->SetMemoryAttributes(
                     gCpu,
                     PcdGet64(PcdNORFlashBase),
                     PcdGet32(PcdNORFlashCachableSize),
                     EFI_MEMORY_UC
                     );

    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "gCpu->SetMemoryAttributes Status = %r !\n", Status));
    }

}

EFI_STATUS
EFIAPI InitializeFlash (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable)
{
    EFI_STATUS Status;


    gIndex.Base = (UINT32)PcdGet64(PcdNORFlashBase);

    SetFlashAttributeToUncache();
    Status = FlashInit(gIndex.Base);
    if (EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "Init Flash Error !\n"));
        return Status;
    }
    else
    {
        DEBUG((EFI_D_ERROR, "Init Flash OK!\n"));
    }

    Status = gBS->InstallProtocolInterface (
                            &ImageHandle,
                            &gUniNorFlashProtocolGuid,
                            EFI_NATIVE_INTERFACE,
                            &gUniNorFlash);
    if(EFI_SUCCESS != Status)
    {
        DEBUG ((EFI_D_ERROR, "[%a]:[%dL]:Install Protocol Interface %r!\n", __FUNCTION__,__LINE__,Status));
    }

    return Status;
}
