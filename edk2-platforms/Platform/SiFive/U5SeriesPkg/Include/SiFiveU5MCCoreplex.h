/** @file
  SiFive U54 Coreplex library definitions.

  Copyright (c) 2019, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef SIFIVE_U5MC_COREPLEX_H_
#define SIFIVE_U5MC_COREPLEX_H_

#include <PiPei.h>

#include <SmbiosProcessorSpecificData.h>
#include <ProcessorSpecificHobData.h>

#define SIFIVE_U5MC_COREPLEX_MC_HART_ID     0

/**
  Build up U5MC coreplex processor core-specific information.

  @param  UniqueId      U5MC unique ID.

  @return EFI_STATUS

**/
EFI_STATUS
EFIAPI
CreateU5MCCoreplexProcessorSpecificDataHob (
  IN UINTN UniqueId
  );

/**
  Function to build processor related SMBIOS information. RISC-V SMBIOS DXE driver collect
  this information and build SMBIOS Type4 and Type7 record.

  @param  ProcessorUid        Unique ID of pysical processor which owns this core.
  @param  SmbiosDataHobPtr    Pointer to receive RISC_V_PROCESSOR_SMBIOS_DATA_HOB. The pointers
                              maintained in this structure is only valid before memory is discovered.
                              Access to those pointers after memory is installed will cause unexpected issues.

  @return EFI_SUCCESS     The PEIM initialized successfully.

**/
EFI_STATUS
EFIAPI
CreateU5MCProcessorSmbiosDataHob (
  IN UINTN     ProcessorUid,
  OUT RISC_V_PROCESSOR_SMBIOS_HOB_DATA **SmbiosDataHobPtr
  );
#endif
