/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _ACPI_PLATFORM_H_
#define _ACPI_PLATFORM_H_

//
// Statements that include other header files
//
#include <PiDxe.h>
#include <PchAccess.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/Pci.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Acpi/GlobalNvsAreaDef.h>
#include <Protocol/IioUds.h>
#include <Protocol/PciIo.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/SerialIo.h>
#include <Protocol/MpService.h>
#include <Protocol/AcpiTable.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/PcdLib.h>
#include <Guid/SetupVariable.h>
#include <Guid/PchRcVariable.h>
#include <Guid/SocketVariable.h>
#include <Guid/HobList.h>
#include <Guid/MemoryMapData.h>
#include "Platform.h"
#include "Register/PchRegsUsb.h"
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchInfoLib.h>

#include <Uefi/UefiBaseType.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>

#include <IndustryStandard/Acpi.h>

#include <Protocol/MpService.h>

#include <Protocol/AcpiSystemDescriptionTable.h>

#include <Guid/MemoryMapData.h>
#include <Guid/GlobalVariable.h>

#include <Platform.h>
#include <Acpi/GlobalNvsAreaDef.h>
#include <Protocol/IioUds.h>
#include <Protocol/CpuIo2.h>
#include <Protocol/SerialIo.h>
#include <Protocol/DevicePath.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/AcpiAml.h>

#include <Guid/SocketMpLinkVariable.h>
#include <Guid/SocketIioVariable.h>
#include <Guid/SocketPowermanagementVariable.h>
#include <Guid/SocketCommonRcVariable.h>

#include "Register/PchRegsUsb.h"

#include <Register/Cpuid.h>
#define   EFI_FINE_GRAINED_CLOCK_MODULATION      BIT5
#define B_CPUID_POWER_MANAGEMENT_EAX_HWP_LVT_INTERRUPT_SUPPORT        BIT9

#endif
