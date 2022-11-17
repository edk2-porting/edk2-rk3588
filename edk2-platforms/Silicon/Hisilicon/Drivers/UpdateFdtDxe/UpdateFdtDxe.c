/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/


#include <Uefi.h>
#include <Pi/PiDxeCis.h>
#include <Library/DebugLib.h>
#include <libfdt.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Guid/Fdt.h>
#include <Protocol/HisiBoardNicProtocol.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/FdtUpdateLib.h>

STATIC
EFI_STATUS
InstallFdtIntoConfigurationTable (
  IN VOID* FdtBlob,
  IN UINTN FdtSize
  )
{
  EFI_STATUS Status;

  // Check the FDT header is valid. We only make this check in DEBUG mode in case the FDT header change on
  // production device and this ASSERT() becomes not valid.
  if(!(fdt_check_header (FdtBlob) == 0))
  {
      DEBUG ((EFI_D_ERROR,"can not find FdtBlob \n"));
      return EFI_INVALID_PARAMETER;
  }

  // Ensure the Size of the Device Tree is smaller than the size of the read file
  if(!((UINTN)fdt_totalsize (FdtBlob) <= FdtSize))
  {
      DEBUG ((EFI_D_ERROR,"FdtBlob <= FdtSize \n"));
      return EFI_INVALID_PARAMETER;
  }

  // Install the FDT into the Configuration Table
  Status = gBS->InstallConfigurationTable (&gFdtTableGuid, FdtBlob);

  return Status;
}

EFI_STATUS
SetNvramSpace (VOID)
{
    EFI_STATUS          Status;
    EFI_GCD_MEMORY_SPACE_DESCRIPTOR desp = {0};

    if (PcdGet64(PcdReservedNvramSize) == 0) {
      return EFI_SUCCESS;
    }

    Status = gDS->GetMemorySpaceDescriptor(PcdGet64(PcdReservedNvramBase),&desp);
    if(EFI_ERROR(Status)){
         DEBUG ((EFI_D_ERROR,"get memory space error:--------- \n"));
        return Status;
    }
    desp.Attributes |= EFI_MEMORY_RUNTIME | EFI_MEMORY_WB;
    Status = gDS->SetMemorySpaceAttributes(PcdGet64(PcdReservedNvramBase),PcdGet64(PcdReservedNvramSize), desp.Attributes);
    if(EFI_ERROR(Status)){
        DEBUG ((EFI_D_ERROR,"set memory space error:--------- \n"));
        return Status;
    }

    return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI UpdateFdt (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable)
{
    INTN                Error;
    VOID*               Fdt;
    UINT32              Size;
    UINTN               NewFdtBlobSize;
    UINTN               NewFdtBlobBase;
    EFI_STATUS          Status = EFI_SUCCESS;
    UINT32              Index = 0;
    UINTN               FDTConfigTable;

    (VOID) SetNvramSpace ();

    Fdt = (VOID*)(PcdGet64(FdtFileAddress));


    Error = fdt_check_header ((VOID*)(PcdGet64(FdtFileAddress)));
    DEBUG ((EFI_D_ERROR,"fdtfileaddress:--------- 0x%lx\n",PcdGet64(FdtFileAddress)));
    if (Error != 0)
    {
        DEBUG ((EFI_D_ERROR,"ERROR: Device Tree header not valid (%a)\n", fdt_strerror(Error)));
        return EFI_INVALID_PARAMETER;
    }

    Size = (UINTN)fdt_totalsize ((VOID*)(PcdGet64(FdtFileAddress)));
    NewFdtBlobSize = Size + ADD_FILE_LENGTH;

    Status = gBS->AllocatePages (AllocateAnyPages, EfiRuntimeServicesData, EFI_SIZE_TO_PAGES(NewFdtBlobSize), &NewFdtBlobBase);
    if (EFI_ERROR (Status))
    {
        return EFI_OUT_OF_RESOURCES;
    }

    (VOID) CopyMem((VOID*)NewFdtBlobBase, Fdt, Size);

    Status = EFIFdtUpdate(NewFdtBlobBase);
    if (EFI_ERROR (Status))
    {
        DEBUG((EFI_D_ERROR, "%a(%d):EFIFdtUpdate Fail!\n", __FUNCTION__,__LINE__));
        goto EXIT;
    }


    Status = InstallFdtIntoConfigurationTable ((VOID*)(UINTN)NewFdtBlobBase, NewFdtBlobSize);
    DEBUG ((EFI_D_ERROR, "NewFdtBlobBase: 0x%lx  NewFdtBlobSize:0x%lx\n",NewFdtBlobBase,NewFdtBlobSize));
    if (EFI_ERROR (Status))
    {
        DEBUG ((EFI_D_ERROR, "installfdtconfiguration table fail():\n"));
        goto EXIT;
    }


    for (Index = 0; Index < gST->NumberOfTableEntries; Index ++)
    {
        if (CompareGuid (&gFdtTableGuid, &(gST->ConfigurationTable[Index].VendorGuid)))
        {
            FDTConfigTable = (UINTN)gST->ConfigurationTable[Index].VendorTable;
            DEBUG ((EFI_D_ERROR, "FDTConfigTable Address: 0x%lx\n",FDTConfigTable));
            break;
        }
    }

    return Status;

    EXIT:

         gBS->FreePages(NewFdtBlobBase,EFI_SIZE_TO_PAGES(NewFdtBlobSize));

    return Status;

}
