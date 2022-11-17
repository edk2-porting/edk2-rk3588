/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef _NOR_FLASH_HW_H_
#define _NOR_FLASH_HW_H_

#include <Uefi/UefiBaseType.h>


#define FOUR_BYTE_UNIT    4
#define FLASH_MAX_UNIT    4

#define FLASH_DEVICE_NUM  0x10



typedef struct {
    UINT32 ManufacturerID;
    UINT32 DeviceID1;
    UINT32 DeviceID2;
    UINT32 DeviceID3;
    UINT8  ParallelNum;
    UINT32 SingleChipSize;
    UINT32 BlockSize;
    UINT32 BufferProgramSize;
    UINT32 CommandType;
}NOR_FLASH_INFO_TABLE;

/*Define Command Address And Data*/
/*reset*/
typedef struct {
    UINT32 CommandType;
    UINT32 ResetData;
}FLASH_COMMAND_RESET;

/*manufacture ID & Device ID*/
typedef struct {
    UINT32 CommandType;
    UINT32 ManuIDAddressStep1;
    UINT32 ManuIDDataStep1;
    UINT32 ManuIDAddressStep2;
    UINT32 ManuIDDataStep2;
    UINT32 ManuIDAddressStep3;
    UINT32 ManuIDDataStep3;
    UINT32 ManuIDAddress;

    UINT32 DeviceIDAddress1;
    UINT32 DeviceIDAddress2;
    UINT32 DeviceIDAddress3;
}FLASH_COMMAND_ID;

/*Write Buffer*/
typedef struct {
    UINT32 CommandType;
    UINT32 BufferProgramAddressStep1;
    UINT32 BufferProgramDataStep1;
    UINT32 BufferProgramAddressStep2;
    UINT32 BufferProgramDataStep2;
    UINT32 BufferProgramDataStep3;
    UINT32 BufferProgramtoFlash;
}FLASH_COMMAND_WRITE;

/*erase*/
typedef struct {
    UINT32 CommandType;
    UINT32 SectorEraseAddressStep1;
    UINT32 SectorEraseDataStep1;
    UINT32 SectorEraseAddressStep2;
    UINT32 SectorEraseDataStep2;
    UINT32 SectorEraseAddressStep3;
    UINT32 SectorEraseDataStep3;
    UINT32 SectorEraseAddressStep4;
    UINT32 SectorEraseDataStep4;
    UINT32 SectorEraseAddressStep5;
    UINT32 SectorEraseDataStep5;
    UINT32 SectorEraseDataStep6;
}FLASH_COMMAND_ERASE;


typedef struct {
    UINT32 Base;
    UINT32 InfIndex;
    UINT32 ReIndex;
    UINT32 IdIndex;
    UINT32 WIndex;
    UINT32 EIndex;
}FLASH_INDEX;


extern EFI_STATUS FlashInit(UINT32 Base);
extern EFI_STATUS SectorErase(UINT32 Base, UINT32 Offset);
extern EFI_STATUS BufferWrite(UINT32 Offset, void *pData, UINT32 Length);
extern EFI_STATUS IsNeedToWrite(UINT32 Base, UINT32 Offset, UINT8 *Buffer, UINT32 Length);


extern NOR_FLASH_INFO_TABLE gFlashInfo[FLASH_DEVICE_NUM];
extern FLASH_COMMAND_RESET gFlashCommandReset[FLASH_DEVICE_NUM];
extern FLASH_COMMAND_ID gFlashCommandId[FLASH_DEVICE_NUM];
extern FLASH_COMMAND_WRITE gFlashCommandWrite[FLASH_DEVICE_NUM];
extern FLASH_COMMAND_ERASE gFlashCommandErase[FLASH_DEVICE_NUM];
extern FLASH_INDEX gIndex;


#endif
