/** @file
*
*  Copyright (c) 2016, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2016, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Uefi.h>
#include <Library/ArmArchTimer.h>
#include <Library/BaseLib.h>
#include <libfdt.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/FdtUpdateLib.h>
#include <PlatformArch.h>
#include <Library/PcdLib.h>
#include <Library/PlatformSysCtrlLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/HisiBoardNicProtocol.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/OemMiscLib.h>

typedef union AA_DAW
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sysdaw_id           : 7  ; /* [6:0] */
        unsigned int    interleave_en       : 1  ; /* [7] */
        unsigned int    sysdaw_size         : 4  ; /* [11:8] */
        unsigned int    reserved            : 4  ; /* [15:12] */
        unsigned int    sysdaw_addr         : 16 ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} AA_DAW_U;



MAC_ADDRESS gMacAddress[1];


CHAR8  *EthName[8]=
{
 "ethernet@0","ethernet@1",
 "ethernet@2","ethernet@3",
 "ethernet@4","ethernet@5",
 "ethernet@6","ethernet@7"
};

UINT8 DawNum[4] = {0, 0, 0, 0};
PHY_MEM_REGION    *NodemRegion[4] = {NULL, NULL, NULL, NULL};
UINTN  NumaPages[4] = {0, 0, 0, 0};

CHAR8  *NumaNodeName[4]=
{
 "p0-ta","p0-tc",
 "p1-ta","p1-tc",
};

STATIC
BOOLEAN
IsMemMapRegion (
  IN EFI_MEMORY_TYPE MemoryType
  )
{
  switch(MemoryType)
    {
        case EfiRuntimeServicesCode:
        case EfiRuntimeServicesData:
        case EfiConventionalMemory:
        case EfiACPIReclaimMemory:
        case EfiACPIMemoryNVS:
        case EfiLoaderCode:
        case EfiLoaderData:
        case EfiBootServicesCode:
        case EfiBootServicesData:
        case EfiPalCode:
                return TRUE;
        default:
                return FALSE;
  }
}


EFI_STATUS
GetMacAddress (UINT32 Port)
{
    EFI_MAC_ADDRESS Mac;
    EFI_STATUS Status;
    HISI_BOARD_NIC_PROTOCOL *OemNic = NULL;

    Status = gBS->LocateProtocol(&gHisiBoardNicProtocolGuid, NULL, (VOID **)&OemNic);
    if(EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "[%a]:[%dL] LocateProtocol failed %r\n", __FUNCTION__, __LINE__, Status));
        return Status;
    }

    Status = OemNic->GetMac(&Mac, Port);
    if(EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR, "[%a]:[%dL] GetMac failed %r\n", __FUNCTION__, __LINE__, Status));
        return Status;
    }

    gMacAddress[0].data0=Mac.Addr[0];
    gMacAddress[0].data1=Mac.Addr[1];
    gMacAddress[0].data2=Mac.Addr[2];
    gMacAddress[0].data3=Mac.Addr[3];
    gMacAddress[0].data4=Mac.Addr[4];
    gMacAddress[0].data5=Mac.Addr[5];
    DEBUG((EFI_D_INFO, "Port%d:0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
        Port,gMacAddress[0].data0,gMacAddress[0].data1,gMacAddress[0].data2,
        gMacAddress[0].data3,gMacAddress[0].data4,gMacAddress[0].data5));

    return EFI_SUCCESS;
}

STATIC
EFI_STATUS
DelPhyhandleUpdateMacAddress(IN VOID* Fdt)
{
    UINT8               port;
    INTN                ethernetnode;
    INTN                node;
    INTN                Error;
    struct              fdt_property *m_prop;
    int                 m_oldlen;
    EFI_STATUS          Status = EFI_SUCCESS;
    EFI_STATUS          GetMacStatus = EFI_SUCCESS;

    node = fdt_subnode_offset(Fdt, 0, "soc");
    if (node < 0)
    {
        DEBUG ((EFI_D_ERROR, "can not find soc root node\n"));
        return EFI_INVALID_PARAMETER;
    }
    else
    {
        for( port=0; port<8; port++ )
        {
            GetMacStatus= GetMacAddress(port);
            ethernetnode = fdt_subnode_offset(Fdt, node,EthName[port]);
            if(!EFI_ERROR(GetMacStatus))
            {

                if (ethernetnode < 0)
                {
                    DEBUG ((EFI_D_WARN, "Can not find ethernet@%d node\n",port));
                    DEBUG ((EFI_D_WARN, "Suppose port %d is not enabled.\n", port));
                    continue;
                }
                m_prop = fdt_get_property_w(Fdt, ethernetnode, "local-mac-address", &m_oldlen);
                if(m_prop)
                {
                    Error = fdt_delprop(Fdt, ethernetnode, "local-mac-address");
                    if (Error)
                    {
                        DEBUG ((EFI_D_ERROR, "ERROR:fdt_delprop() Local-mac-address: %a\n", fdt_strerror (Error)));
                        Status = EFI_INVALID_PARAMETER;
                    }
                    Error = fdt_setprop(Fdt, ethernetnode, "local-mac-address",gMacAddress,sizeof(MAC_ADDRESS));
                    if (Error)
                    {
                        DEBUG ((EFI_D_ERROR, "ERROR:fdt_setprop():local-mac-address %a\n", fdt_strerror (Error)));
                        Status = EFI_INVALID_PARAMETER;
                    }
                }
            }
        }
    }
    return Status;
}

STATIC
EFI_STATUS
UpdateRefClk (IN VOID* Fdt)
{
  INTN                node;
  INTN                Error;
  struct              fdt_property *m_prop;
  int                 m_oldlen;
  UINTN               ArchTimerFreq = 0;
  UINT32              Data;
  CONST CHAR8         *Property = "clock-frequency";

  ArmArchTimerReadReg (CntFrq, &ArchTimerFreq);
  if (!ArchTimerFreq) {
    DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Get timer frequency failed!\n", __FUNCTION__, __LINE__));
    return EFI_INVALID_PARAMETER;
  }

  node = fdt_subnode_offset(Fdt, 0, "soc");
  if (node < 0) {
    DEBUG ((DEBUG_ERROR, "can not find soc node\n"));
    return EFI_INVALID_PARAMETER;
  }

  node = fdt_subnode_offset(Fdt, node, "refclk");
  if (node < 0) {
    DEBUG ((DEBUG_ERROR, "can not find refclk node\n"));
    return EFI_INVALID_PARAMETER;
  }

  m_prop = fdt_get_property_w(Fdt, node, Property, &m_oldlen);
  if(!m_prop) {
    DEBUG ((DEBUG_ERROR, "[%a]:[%dL] Can't find property %a\n", __FUNCTION__, __LINE__, Property));
    return EFI_INVALID_PARAMETER;
  }

  Error = fdt_delprop(Fdt, node, Property);
  if (Error) {
    DEBUG ((DEBUG_ERROR, "ERROR: fdt_delprop() %a: %a\n", Property, fdt_strerror (Error)));
    return EFI_INVALID_PARAMETER;
  }

  // UINT32 is enough for refclk data length
  Data = (UINT32) ArchTimerFreq;
  Data = cpu_to_fdt32 (Data);
  Error = fdt_setprop(Fdt, node, Property, &Data, sizeof(Data));
  if (Error) {
    DEBUG ((DEBUG_ERROR, "ERROR:fdt_setprop() %a: %a\n", Property, fdt_strerror (Error)));
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "Update refclk successfully.\n"));
  return EFI_SUCCESS;
}

INTN
GetMemoryNode(VOID* Fdt)
{
    INTN                node;
    int                 m_oldlen;
    struct              fdt_property *m_prop;
    INTN                Error = 0;


    node = fdt_subnode_offset(Fdt, 0, "memory");
    if (node < 0)
    {
        // Create the memory node
        node = fdt_add_subnode(Fdt, 0, "memory");

        if(node < 0)
        {
          DEBUG((EFI_D_ERROR, "[%a]:[%dL] fdt add subnode error\n", __FUNCTION__, __LINE__));

          return node;
        }

    }
    //find the memory node property
    m_prop = fdt_get_property_w(Fdt, node, "memory", &m_oldlen);
    if(m_prop)
    {
        Error = fdt_delprop(Fdt, node, "reg");

        if (Error)
        {
            DEBUG ((EFI_D_ERROR, "ERROR:fdt_delprop(): %a\n", fdt_strerror (Error)));
            node = -1;
            return node;
        }
    }

    return node;
}


EFI_STATUS UpdateMemoryNode(VOID* Fdt)
{
    INTN                Error = 0;
    EFI_STATUS          Status = EFI_SUCCESS;
    UINT32              Index = 0;
    UINT32              MemIndex;
    INTN                node;
    EFI_MEMORY_DESCRIPTOR *MemoryMap;
    EFI_MEMORY_DESCRIPTOR *MemoryMapPtr;
    EFI_MEMORY_DESCRIPTOR *MemoryMapPtrCurrent;
    UINTN                 MemoryMapSize;
    UINTN                 Pages0 = 0;
    UINTN                 Pages1 = 0;
    UINTN                 MapKey;
    UINTN                 DescriptorSize;
    UINT32                DescriptorVersion;
    PHY_MEM_REGION        *mRegion;
    UINTN                 MemoryMapLastEndAddress ;
    UINTN                 MemoryMapcontinuousStartAddress ;
    UINTN                 MemoryMapCurrentStartAddress;
    BOOLEAN               FindMemoryRegionFlag = FALSE;

    node = GetMemoryNode(Fdt);
    if (node < 0)
    {
        DEBUG((EFI_D_ERROR, "Can not find memory node\n"));
        return EFI_NOT_FOUND;
    }
    MemoryMap = NULL;
    MemoryMapSize = 0;
    MemIndex = 0;

    Status = gBS->GetMemoryMap (&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
    if (Status == EFI_BUFFER_TOO_SMALL)
    {
        // The UEFI specification advises to allocate more memory for the MemoryMap buffer between successive
        // calls to GetMemoryMap(), since allocation of the new buffer may potentially increase memory map size.
        Pages0 = EFI_SIZE_TO_PAGES (MemoryMapSize) + 1;
        MemoryMap = AllocatePages (Pages0);
        if (MemoryMap == NULL)
        {
            Status = EFI_OUT_OF_RESOURCES;
            return Status;
        }
        Status = gBS->GetMemoryMap (&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);

        if (EFI_ERROR(Status))
        {
            DEBUG ((EFI_D_ERROR, "FdtUpdateLib GetMemoryMap Error\n"));
            FreePages (MemoryMap, Pages0);
            return Status;
        }
    }
    else
    {
        DEBUG ((EFI_D_ERROR, "FdtUpdateLib GetmemoryMap Status: %r\n",Status));
        return EFI_ABORTED;
    }

    mRegion = NULL;
    Pages1 = EFI_SIZE_TO_PAGES (sizeof(PHY_MEM_REGION) *( MemoryMapSize / DescriptorSize));

    mRegion = (PHY_MEM_REGION*)AllocatePool(Pages1);
    if (mRegion == NULL)
    {
      Status = EFI_OUT_OF_RESOURCES;
      FreePages (MemoryMap, Pages0);
      return Status;
    }


    MemoryMapPtr = MemoryMap;
    MemoryMapPtrCurrent = MemoryMapPtr;
    MemoryMapLastEndAddress = 0;
    MemoryMapcontinuousStartAddress = 0;
    MemoryMapCurrentStartAddress = 0;
    for (Index = 0; Index < (MemoryMapSize / DescriptorSize); Index++)
    {
        MemoryMapPtrCurrent = (EFI_MEMORY_DESCRIPTOR*)((UINTN)MemoryMapPtr + Index*DescriptorSize);
        MemoryMapCurrentStartAddress = (UINTN)MemoryMapPtrCurrent->PhysicalStart;

        if (!IsMemMapRegion ((EFI_MEMORY_TYPE)MemoryMapPtrCurrent->Type))
        {
            continue;
        }
        else
        {
            FindMemoryRegionFlag = TRUE;
            if(MemoryMapCurrentStartAddress != MemoryMapLastEndAddress)
            {
                mRegion[MemIndex].BaseHigh= cpu_to_fdt32(MemoryMapcontinuousStartAddress>>32);
                mRegion[MemIndex].BaseLow=cpu_to_fdt32(MemoryMapcontinuousStartAddress);
                mRegion[MemIndex].LengthHigh= cpu_to_fdt32((MemoryMapLastEndAddress-MemoryMapcontinuousStartAddress)>>32);
                mRegion[MemIndex].LengthLow=cpu_to_fdt32(MemoryMapLastEndAddress-MemoryMapcontinuousStartAddress);
                MemIndex+=1;
                MemoryMapcontinuousStartAddress=MemoryMapCurrentStartAddress;
            }
        }
        MemoryMapLastEndAddress = (UINTN)(MemoryMapPtrCurrent->PhysicalStart + MemoryMapPtrCurrent->NumberOfPages * EFI_PAGE_SIZE);
    }
    if (FindMemoryRegionFlag)
    {
        mRegion[MemIndex].BaseHigh = cpu_to_fdt32(MemoryMapcontinuousStartAddress>>32);
        mRegion[MemIndex].BaseLow = cpu_to_fdt32(MemoryMapcontinuousStartAddress);
        mRegion[MemIndex].LengthHigh = cpu_to_fdt32((MemoryMapLastEndAddress-MemoryMapcontinuousStartAddress)>>32);
        mRegion[MemIndex].LengthLow = cpu_to_fdt32(MemoryMapLastEndAddress-MemoryMapcontinuousStartAddress);
    }

    Error = fdt_setprop(Fdt, node, "reg",mRegion,sizeof(PHY_MEM_REGION) *(MemIndex+1));

    FreePool (mRegion);
    FreePages (MemoryMap, Pages0);
    if (Error)
    {
        DEBUG ((EFI_D_ERROR, "ERROR:fdt_setprop(): %a\n", fdt_strerror (Error)));
        Status = EFI_INVALID_PARAMETER;
        return Status;
    }

  return Status;
}


EFI_STATUS
UpdateNumaNode(VOID* Fdt)
{
    //TODO: Need to update numa node
    return EFI_SUCCESS;
}
/*
 * Entry point for fdtupdate lib.
 */

EFI_STATUS EFIFdtUpdate(UINTN FdtFileAddr)
{
    INTN                Error;
    VOID*               Fdt;
    UINT32              Size;
    UINTN               NewFdtBlobSize;
    UINTN               NewFdtBlobBase;
    EFI_STATUS          Status = EFI_SUCCESS;
    EFI_STATUS          UpdateNumaStatus = EFI_SUCCESS;


    Error = fdt_check_header ((VOID*)(FdtFileAddr));
    if (0 != Error)
    {
        DEBUG ((EFI_D_ERROR,"ERROR: Device Tree header not valid (%a)\n", fdt_strerror(Error)));
        return EFI_INVALID_PARAMETER;
    }

    Size = (UINTN)fdt_totalsize ((VOID*)(UINTN)(FdtFileAddr));
    NewFdtBlobSize = Size + ADD_FILE_LENGTH;
    Fdt = (VOID*)(UINTN)FdtFileAddr;

    Status = gBS->AllocatePages (AllocateAnyPages, EfiBootServicesData, EFI_SIZE_TO_PAGES(NewFdtBlobSize), &NewFdtBlobBase);
    if (EFI_ERROR (Status))
    {
        return EFI_OUT_OF_RESOURCES;
    }


    Error = fdt_open_into(Fdt,(VOID*)(UINTN)(NewFdtBlobBase), (NewFdtBlobSize));
    if (Error) {
        DEBUG ((EFI_D_ERROR, "ERROR:fdt_open_into(): %a\n", fdt_strerror (Error)));
        Status = EFI_INVALID_PARAMETER;
        goto EXIT;
    }

    Fdt = (VOID*)(UINTN)NewFdtBlobBase;
    Status = DelPhyhandleUpdateMacAddress(Fdt);
    if (EFI_ERROR (Status))
    {
        DEBUG ((EFI_D_ERROR, "DelPhyhandleUpdateMacAddress fail:\n"));
        Status = EFI_SUCCESS;
    }

    Status =  UpdateRefClk (Fdt);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "UpdateiRefClk fail.\n"));
    }

    Status = UpdateMemoryNode(Fdt);
    if (EFI_ERROR (Status))
    {
        DEBUG ((EFI_D_ERROR, "UpdateMemoryNode Error\n"));
        goto EXIT;
    }

    UpdateNumaStatus = UpdateNumaNode(Fdt);
    if (EFI_ERROR (UpdateNumaStatus))
    {
        DEBUG ((EFI_D_ERROR, "Update NumaNode fail\n"));
    }

    gBS->CopyMem(((VOID*)(UINTN)(FdtFileAddr)),((VOID*)(UINTN)(NewFdtBlobBase)),NewFdtBlobSize);

EXIT:
    gBS->FreePages(NewFdtBlobBase,EFI_SIZE_TO_PAGES(NewFdtBlobSize));

    return Status;



}
