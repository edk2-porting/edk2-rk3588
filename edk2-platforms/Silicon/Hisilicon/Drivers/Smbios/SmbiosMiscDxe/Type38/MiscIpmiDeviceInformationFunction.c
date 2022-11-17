/*++

Copyright (c) 2006 - 2009, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2015, Hisilicon Limited. All rights reserved.<BR>
Copyright (c) 2015, Linaro Limited. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

  MiscIpmiDeviceInformationFunction.c

Abstract:

  This driver parses the mMiscSubclassDataTable structure and reports
  any generated data to smbios.

Based on files under Nt32Pkg/MiscSubClassPlatformDxe/
**/
/* Modify list
DATA        AUTHOR            REASON
*/

#include "SmbiosMisc.h"

#include <Protocol/IpmiInterfaceProtocol.h>

/**
  This function makes the attributes of IPMI to the contents of the
  MiscChassisManufacturer structure.

  @param  RecordData                 Pointer to copy of RecordData from the Data Table.

  @retval EFI_SUCCESS                All parameters were valid.
  @retval EFI_UNSUPPORTED            Unexpected RecordType value.
  @retval EFI_INVALID_PARAMETER      Invalid parameter was found.

**/
MISC_SMBIOS_TABLE_FUNCTION(MiscIpmiDeviceInformation)
{
    EFI_STATUS                      Status;
    EFI_SMBIOS_HANDLE               SmbiosHandle;
    SMBIOS_TABLE_TYPE38             *SmbiosRecord;
    SMBIOS_TABLE_TYPE38             *InputData = NULL;

    IPMI_INTERFACE_PROTOCOL         *Ipmi;

    //
    // First check for invalid parameters.
    //
    if (RecordData == NULL) {
        return EFI_INVALID_PARAMETER;
    }

    InputData = (SMBIOS_TABLE_TYPE38*)RecordData;

    SmbiosRecord = AllocateZeroPool(sizeof (SMBIOS_TABLE_TYPE38) + 1 + 1);
    if(NULL == SmbiosRecord) {
        return EFI_OUT_OF_RESOURCES;
    }

    (VOID)CopyMem(SmbiosRecord, InputData, sizeof (SMBIOS_TABLE_TYPE38));

    SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE38);

    Status = gBS->LocateProtocol (&gIpmiInterfaceProtocolGuid, NULL, (VOID **)&Ipmi);
    if (!EFI_ERROR (Status)) {
        SmbiosRecord->InterfaceType = Ipmi->GetIpmiInterfaceType (Ipmi);
        SmbiosRecord->BaseAddress = (UINT64)Ipmi->GetIpmiBaseAddress (Ipmi) | Ipmi->GetIpmiBaseAddressType (Ipmi);
        SmbiosRecord->IPMISpecificationRevision = Ipmi->GetIpmiVersion (Ipmi);
    }
    //
    // Now we have got the full smbios record, call smbios protocol to add this record.
    //
    Status = LogSmbiosData((UINT8*)SmbiosRecord, &SmbiosHandle);
    if(EFI_ERROR(Status)) {
        DEBUG((EFI_D_ERROR, "[%a]:[%dL] Smbios Type38 Table Log Failed! %r \n", __FUNCTION__, __LINE__, Status));
    }

    FreePool(SmbiosRecord);
    return Status;
}
