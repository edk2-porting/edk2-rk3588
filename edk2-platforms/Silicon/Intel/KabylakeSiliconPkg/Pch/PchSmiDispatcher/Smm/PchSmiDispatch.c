/** @file
  This function handle the register/unregister of PCH specific SMI events.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "PchSmmHelpers.h"

/**
  The internal function used to create and insert a database record
  for SMI record of Pch Smi types.

  @param[in]  SrcDesc                   The pointer to the SMI source description
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[in]  PchSmiType                Specific SMI type of PCH SMI
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
**/
EFI_STATUS
PchSmiRecordInsert (
  IN  EFI_GUID                          *ProtocolGuid,
  IN  PCH_SMM_SOURCE_DESC               *SrcDesc,
  IN  PCH_SMI_CALLBACK_FUNCTIONS        DispatchFunction,
  IN  PCH_SMI_TYPES                     PchSmiType,
  OUT EFI_HANDLE                        *DispatchHandle
  )
{
  EFI_STATUS                            Status;
  DATABASE_RECORD                       *Record;
  PCH_SMM_SOURCE_DESC                   NullSourceDesc;

  //
  // Initialize NullSourceDesc
  //
  NullInitSourceDesc (&NullSourceDesc);

  if (CompareSources (SrcDesc, &NullSourceDesc)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (DATABASE_RECORD), (VOID **) &Record);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }
  SetMem (Record, sizeof (DATABASE_RECORD), 0);
  //
  // Gather information about the registration request
  //
  Record->Signature                      = DATABASE_RECORD_SIGNATURE;
  Record->PchSmiCallback                 = DispatchFunction;
  Record->ProtocolType                   = PchSmiDispatchType;
  Record->ProtocolGuid                   = ProtocolGuid;
  Record->PchSmiType                     = PchSmiType;

  Record->ContextFunctions.GetContext    = NULL;
  Record->ContextFunctions.CmpContext    = NULL;
  Record->ContextFunctions.GetCommBuffer = NULL;
  Record->ClearSource                    = NULL;

  CopyMem ((VOID *) &(Record->SrcDesc), (VOID *) (SrcDesc), sizeof (PCH_SMM_SOURCE_DESC));

  //
  // After ensuring the source of event is not null, we will insert the record into the database
  //
  InsertTailList (&mPrivateData.CallbackDataBase, &Record->Link);

  //
  // Child's handle will be the address linked list link in the record
  //
  *DispatchHandle = (EFI_HANDLE) (&Record->Link);

  if (ProtocolGuid != NULL) {
    SmiHandlerProfileRegisterHandler (ProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), &PchSmiType, sizeof(PchSmiType));
  }

  return EFI_SUCCESS;
}

EFI_STATUS
PchSmiRecordUnRegister (
  IN EFI_HANDLE                                         DispatchHandle
  )
{
  return PchSmmCoreUnRegister(NULL, DispatchHandle);
}

//
// TCO_STS bit that needs to be cleared
//
PCH_SMM_SOURCE_DESC mDescSrcTcoSts = {
  PCH_SMM_NO_FLAGS,
  {
    NULL_BIT_DESC_INITIALIZER,
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_SMI_STS}
      },
      S_PCH_SMI_STS,
      N_PCH_SMI_STS_TCO
    }
  },
  NULL_BIT_DESC_INITIALIZER
};

/**
  Clear the TCO SMI status bit and block after the SMI handling is done

  @param[in] SrcDesc                    Pointer to the PCH SMI source description table

**/
VOID
EFIAPI
PchTcoSmiClearSourceAndBlock (
  PCH_SMM_SOURCE_DESC                   *SrcDesc
  )
{
  PchSmmClearSourceAndBlock (SrcDesc);
  //
  // Any TCO-based status bits require special handling.
  // SMI_STS.TCO_STS must be cleared in addition to the status bit in the TCO registers
  //
  PchSmmClearSource (&mDescSrcTcoSts);
}

/**
  Clear the TCO SMI status bit after the SMI handling is done

  @param[in] SrcDesc                    Pointer to the PCH SMI source description table

**/
VOID
EFIAPI
PchTcoSmiClearSource (
  PCH_SMM_SOURCE_DESC                   *SrcDesc
  )
{
  PchSmmClearSource (SrcDesc);
  //
  // Any TCO-based status bits require special handling.
  // SMI_STS.TCO_STS must be cleared in addition to the status bit in the TCO registers
  //
  PchSmmClearSource (&mDescSrcTcoSts);
}

/**
  Initialize Source descriptor structure

  @param[in] SrcDesc                    Pointer to the PCH SMI source description table

**/
VOID
NullInitSourceDesc (
  PCH_SMM_SOURCE_DESC                   *SrcDesc
  )
{
  ZeroMem (SrcDesc, sizeof (PCH_SMM_SOURCE_DESC));
  SrcDesc->En[0].Reg.Type     = PCH_SMM_ADDR_TYPE_NULL;
  SrcDesc->En[1].Reg.Type     = PCH_SMM_ADDR_TYPE_NULL;
  SrcDesc->Sts[0].Reg.Type    = PCH_SMM_ADDR_TYPE_NULL;
  SrcDesc->PmcSmiSts.Reg.Type = PCH_SMM_ADDR_TYPE_NULL;
}

//
// Mch srcdesc
//
PCH_SMM_SOURCE_DESC mSrcDescMch = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_SMI_EN}
      },
      S_PCH_SMI_EN,
      N_PCH_SMI_EN_TCO
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        TCO_ADDR_TYPE,
        {R_PCH_TCO1_STS}
      },
      S_PCH_TCO1_STS,
      N_PCH_TCO1_STS_DMISMI
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_PCH_SMI_STS}
    },
    S_PCH_SMI_STS,
    N_PCH_SMI_STS_TCO
  }
};

/**
  The register function used to register SMI handler of MCH event.

  @param[in]  This                      The pointer to the protocol itself
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchTcoSmiMchRegister (
  IN  PCH_TCO_SMI_DISPATCH_PROTOCOL     *This,
  IN  PCH_TCO_SMI_DISPATCH_CALLBACK     DispatchFunction,
  OUT EFI_HANDLE                        *DispatchHandle
  )
{
  EFI_STATUS                            Status;
  DATABASE_RECORD                       *Record;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = PchSmiRecordInsert (
             &gPchTcoSmiDispatchProtocolGuid,
             &mSrcDescMch,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchTcoSmiMchType,
             DispatchHandle
             );
  if (!EFI_ERROR (Status)) {
    Record = DATABASE_RECORD_FROM_LINK (*DispatchHandle);
    Record->ClearSource = PchTcoSmiClearSource;
    PchSmmClearSource (&Record->SrcDesc);
    PchSmmEnableSource (&Record->SrcDesc);
  }
  return Status;
}

//
// TcoTimeout srcdesc
//
PCH_SMM_SOURCE_DESC mSrcDescTcoTimeout = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_SMI_EN}
      },
      S_PCH_SMI_EN,
      N_PCH_SMI_EN_TCO
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        TCO_ADDR_TYPE,
        {R_PCH_TCO1_STS}
      },
      S_PCH_TCO1_STS,
      N_PCH_TCO1_STS_TIMEOUT
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_PCH_SMI_STS}
    },
    S_PCH_SMI_STS,
    N_PCH_SMI_STS_TCO
  }
};

/**
  The register function used to register SMI handler of TcoTimeout event.

  @param[in]  This                      The pointer to the protocol itself
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchTcoSmiTcoTimeoutRegister (
  IN  PCH_TCO_SMI_DISPATCH_PROTOCOL     *This,
  IN  PCH_TCO_SMI_DISPATCH_CALLBACK     DispatchFunction,
  OUT EFI_HANDLE                        *DispatchHandle
  )
{
  EFI_STATUS                            Status;
  DATABASE_RECORD                       *Record;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = PchSmiRecordInsert (
             &gPchTcoSmiDispatchProtocolGuid,
             &mSrcDescTcoTimeout,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchTcoSmiTcoTimeoutType,
             DispatchHandle
             );
  if (!EFI_ERROR (Status)) {
    Record = DATABASE_RECORD_FROM_LINK (*DispatchHandle);
    Record->ClearSource = PchTcoSmiClearSource;
    PchSmmClearSource (&Record->SrcDesc);
    PchSmmEnableSource (&Record->SrcDesc);
  }
  return Status;
}

//
// OsTco srcdesc
//
PCH_SMM_SOURCE_DESC mSrcDescOsTco = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_SMI_EN}
      },
      S_PCH_SMI_EN,
      N_PCH_SMI_EN_TCO
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        TCO_ADDR_TYPE,
        {R_PCH_TCO1_STS}
      },
      S_PCH_TCO1_STS,
      N_PCH_TCO1_STS_SW_TCO_SMI
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_PCH_SMI_STS}
    },
    S_PCH_SMI_STS,
    N_PCH_SMI_STS_TCO
  }
};

/**
  The register function used to register SMI handler of OS TCO event.

  @param[in]  This                      The pointer to the protocol itself
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchTcoSmiOsTcoRegister (
  IN  PCH_TCO_SMI_DISPATCH_PROTOCOL     *This,
  IN  PCH_TCO_SMI_DISPATCH_CALLBACK     DispatchFunction,
  OUT EFI_HANDLE                        *DispatchHandle
  )
{
  EFI_STATUS                            Status;
  DATABASE_RECORD                       *Record;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = PchSmiRecordInsert (
             &gPchTcoSmiDispatchProtocolGuid,
             &mSrcDescOsTco,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchTcoSmiOsTcoType,
             DispatchHandle
             );
  if (!EFI_ERROR (Status)) {
    Record = DATABASE_RECORD_FROM_LINK (*DispatchHandle);
    Record->ClearSource = PchTcoSmiClearSource;
    PchSmmClearSource (&Record->SrcDesc);
    PchSmmEnableSource (&Record->SrcDesc);
  }
  return Status;
}

//
// Nmi
//
PCH_SMM_SOURCE_DESC mSrcDescNmi = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        TCO_ADDR_TYPE,
        {R_PCH_TCO1_CNT}
      },
      S_PCH_TCO1_CNT,
      N_PCH_TCO_CNT_NMI2SMI_EN
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        TCO_ADDR_TYPE,
        {R_PCH_TCO1_STS}
      },
      S_PCH_TCO1_STS,
      N_PCH_TCO1_STS_NMI2SMI
    }
  },
  //
  // NOTE: The status of NMI2SMI won't reflect to PMC SMI_STS.
  //       So skip the top level status check and check the TCO1_STS directly.
  //
  NULL_BIT_DESC_INITIALIZER
};

/**
  The register function used to register SMI handler of NMI event.

  @param[in]  This                      The pointer to the protocol itself
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchTcoSmiNmiRegister (
  IN  PCH_TCO_SMI_DISPATCH_PROTOCOL     *This,
  IN  PCH_TCO_SMI_DISPATCH_CALLBACK     DispatchFunction,
  OUT EFI_HANDLE                        *DispatchHandle
  )
{
  EFI_STATUS                            Status;
  DATABASE_RECORD                       *Record;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = PchSmiRecordInsert (
             &gPchTcoSmiDispatchProtocolGuid,
             &mSrcDescNmi,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchTcoSmiNmiType,
             DispatchHandle
             );
  if (!EFI_ERROR (Status)) {
    Record = DATABASE_RECORD_FROM_LINK (*DispatchHandle);
    Record->ClearSource = PchTcoSmiClearSource;
    PchSmmClearSource (&Record->SrcDesc);
    PchSmmEnableSource (&Record->SrcDesc);
  }
  return Status;
}

//
// IntruderDetect srcdesc
//
PCH_SMM_SOURCE_DESC mSrcDescIntruderDet = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_SMI_EN}
      },
      S_PCH_SMI_EN,
      N_PCH_SMI_EN_TCO
    },
    {
      {
        TCO_ADDR_TYPE,
        {R_PCH_TCO2_CNT}
      },
      S_PCH_TCO2_CNT,
      N_PCH_TCO2_CNT_INTRD_SEL
    }
  },
  {
    {
      {
        TCO_ADDR_TYPE,
        {R_PCH_TCO2_STS}
      },
      S_PCH_TCO2_STS,
      N_PCH_TCO2_STS_INTRD_DET
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_PCH_SMI_STS}
    },
    S_PCH_SMI_STS,
    N_PCH_SMI_STS_TCO
  }
};

/**
  The register function used to register SMI handler of Intruder Detect event.

  @param[in]  This                      The pointer to the protocol itself
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchTcoSmiIntruderDetRegister (
  IN  PCH_TCO_SMI_DISPATCH_PROTOCOL     *This,
  IN  PCH_TCO_SMI_DISPATCH_CALLBACK     DispatchFunction,
  OUT EFI_HANDLE                        *DispatchHandle
  )
{
  EFI_STATUS                            Status;
  DATABASE_RECORD                       *Record;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = PchSmiRecordInsert (
             &gPchTcoSmiDispatchProtocolGuid,
             &mSrcDescIntruderDet,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchTcoSmiIntruderDetectType,
             DispatchHandle
             );
  if (!EFI_ERROR (Status)) {
    Record = DATABASE_RECORD_FROM_LINK (*DispatchHandle);
    Record->ClearSource = PchTcoSmiClearSourceAndBlock;
    PchSmmClearSource (&Record->SrcDesc);
    PchSmmEnableSource (&Record->SrcDesc);
  }
  return Status;
}

//
// SpiBiosWp srcdesc
//
PCH_SMM_SOURCE_DESC mSrcDescSpiBiosWp = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_SMI_EN}
      },
      S_PCH_SMI_EN,
      N_PCH_SMI_EN_TCO
    },
    {
      {
        PCIE_ADDR_TYPE,
        { (
          (DEFAULT_PCI_BUS_NUMBER_PCH << 24) |
          (PCI_DEVICE_NUMBER_PCH_SPI << 19) |
          (PCI_FUNCTION_NUMBER_PCH_SPI << 16) |
          R_PCH_SPI_BC
        ) }
      },
      S_PCH_SPI_BC,
      N_PCH_SPI_BC_BLE
    },
  },
  {
    {
      {
        PCIE_ADDR_TYPE,
        { (
          (DEFAULT_PCI_BUS_NUMBER_PCH << 24) |
          (PCI_DEVICE_NUMBER_PCH_SPI << 19) |
          (PCI_FUNCTION_NUMBER_PCH_SPI << 16) |
          R_PCH_SPI_BC
        ) }
      },
      S_PCH_SPI_BC,
      N_PCH_SPI_BC_SYNC_SS
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_PCH_SMI_STS}
    },
    S_PCH_SMI_STS,
    N_PCH_SMI_STS_TCO
  }
};

/**
  Special handling for SPI Write Protect

  @param[in]  SrcDesc   Not used
**/
VOID
EFIAPI
PchTcoSpiWpClearSource (
  PCH_SMM_SOURCE_DESC                   *SrcDesc
  )
{
  UINTN SpiRegBase;

  SpiRegBase = MmPciBase (
                 DEFAULT_PCI_BUS_NUMBER_PCH,
                 PCI_DEVICE_NUMBER_PCH_SPI,
                 PCI_FUNCTION_NUMBER_PCH_SPI
                 );
  MmioAndThenOr32 (
    SpiRegBase + R_PCH_SPI_BC,
    (UINT32) ~B_PCH_SPI_BC_ASYNC_SS,
    B_PCH_SPI_BC_SYNC_SS
    );

  //
  // Any TCO-based status bits require special handling.
  // SMI_STS.TCO_STS must be cleared in addition to the status bit in the TCO registers
  //
  PchSmmClearSource (&mDescSrcTcoSts);
}

/**
  Special handling to enable SPI Write Protect
**/
VOID
PchTcoSpiWpEnableSource (
  VOID
  )
{
  UINTN SpiRegBase;

  SpiRegBase = MmPciBase (
                 DEFAULT_PCI_BUS_NUMBER_PCH,
                 PCI_DEVICE_NUMBER_PCH_SPI,
                 PCI_FUNCTION_NUMBER_PCH_SPI
                 );
  MmioAndThenOr32 (
    SpiRegBase + R_PCH_SPI_BC,
    (UINT32) ~(B_PCH_SPI_BC_ASYNC_SS | B_PCH_SPI_BC_SYNC_SS),
    B_PCH_SPI_BC_LE
    );

  IoOr32 (mAcpiBaseAddr + R_PCH_SMI_EN, B_PCH_SMI_EN_TCO);

  //
  // Clear the source
  //
  PchTcoSpiWpClearSource (NULL);
}

/**
  The register function used to register SMI handler of BIOS write protect event.

  @param[in]  This                      The pointer to the protocol itself
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchTcoSmiSpiBiosWpRegister (
  IN  PCH_TCO_SMI_DISPATCH_PROTOCOL     *This,
  IN  PCH_TCO_SMI_DISPATCH_CALLBACK     DispatchFunction,
  OUT EFI_HANDLE                        *DispatchHandle
  )
{
  EFI_STATUS                            Status;
  DATABASE_RECORD                       *Record;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = PchSmiRecordInsert (
             &gPchTcoSmiDispatchProtocolGuid,
             &mSrcDescSpiBiosWp,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchTcoSmiSpiBiosWpType,
             DispatchHandle
             );
  if (!EFI_ERROR (Status)) {
    Record = DATABASE_RECORD_FROM_LINK (*DispatchHandle);
    Record->ClearSource = PchTcoSpiWpClearSource;
    PchTcoSpiWpClearSource (NULL);
    PchTcoSpiWpEnableSource ();
  }
  return Status;
}

//
// LpcBiosWp srcdesc
//
PCH_SMM_SOURCE_DESC mSrcDescLpcBiosWp = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_SMI_EN}
      },
      S_PCH_SMI_EN,
      N_PCH_SMI_EN_TCO
    },
    {
      {
        PCIE_ADDR_TYPE,
        { (
          (DEFAULT_PCI_BUS_NUMBER_PCH << 24) |
          (PCI_DEVICE_NUMBER_PCH_LPC << 19) |
          (PCI_FUNCTION_NUMBER_PCH_LPC << 16) |
          R_PCH_LPC_BC
        ) }
      },
      S_PCH_LPC_BC,
      N_PCH_LPC_BC_LE
    }
  },
  {
    {
      {
        TCO_ADDR_TYPE,
        {R_PCH_TCO1_STS}
      },
      S_PCH_TCO1_STS,
      N_PCH_TCO1_STS_BIOSWR
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_PCH_SMI_STS}
    },
    S_PCH_SMI_STS,
    N_PCH_SMI_STS_TCO
  }
};

/**
  The register function used to register SMI handler of LPC BIOS write protect event.

  @param[in]  This                      The pointer to the protocol itself
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchTcoSmiLpcBiosWpRegister (
  IN  PCH_TCO_SMI_DISPATCH_PROTOCOL     *This,
  IN  PCH_TCO_SMI_DISPATCH_CALLBACK     DispatchFunction,
  OUT EFI_HANDLE                        *DispatchHandle
  )
{
  EFI_STATUS                            Status;
  DATABASE_RECORD                       *Record;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }


  Status = PchSmiRecordInsert (
             &gPchTcoSmiDispatchProtocolGuid,
             &mSrcDescLpcBiosWp,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchTcoSmiLpcBiosWpType,
             DispatchHandle
             );
  if (!EFI_ERROR (Status)) {
    Record = DATABASE_RECORD_FROM_LINK (*DispatchHandle);
    Record->ClearSource = PchTcoSmiClearSource;
    PchSmmClearSource (&Record->SrcDesc);
    PchSmmEnableSource (&Record->SrcDesc);
  }
  return Status;
}

//
// NEWCENTURY_STS bit that needs to be cleared
//
PCH_SMM_SOURCE_DESC mSrcDescNewCentury = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_SMI_EN}
      },
      S_PCH_SMI_EN,
      N_PCH_SMI_EN_TCO
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        TCO_ADDR_TYPE,
        {R_PCH_TCO1_STS}
      },
      S_PCH_TCO1_STS,
      N_PCH_TCO1_STS_NEWCENTURY
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_PCH_SMI_STS}
    },
    S_PCH_SMI_STS,
    N_PCH_SMI_STS_TCO
  }
};

/**
  Check and clear NEWCENTURY_STS.
  It will clear the SMI status once it happens.

  @param[in] SrcDesc                    Pointer to the PCH SMI source description table
**/
VOID
EFIAPI
PchTcoClearNewCenturySts (
  PCH_SMM_SOURCE_DESC                   *SrcDesc
  )
{
  UINT16 TcoSts1;
  TcoSts1 = IoRead16 (mTcoBaseAddr + R_PCH_TCO1_STS);

  if (TcoSts1 & B_PCH_TCO1_STS_NEWCENTURY) {
    PchTcoSmiClearSourceAndBlock (&mSrcDescNewCentury);
  }
}

/**
  The register function used to register SMI handler of NEW CENTURY event.

  @param[in]  This                      The pointer to the protocol itself
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchTcoSmiNewCenturyRegister (
  IN  PCH_TCO_SMI_DISPATCH_PROTOCOL     *This,
  IN  PCH_TCO_SMI_DISPATCH_CALLBACK     DispatchFunction,
  OUT EFI_HANDLE                        *DispatchHandle
  )
{
  EFI_STATUS                            Status;
  DATABASE_RECORD                       *Record;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = PchSmiRecordInsert (
             &gPchTcoSmiDispatchProtocolGuid,
             &mSrcDescNewCentury,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchTcoSmiNewCenturyType,
             DispatchHandle
             );
  if (!EFI_ERROR (Status)) {
    Record = DATABASE_RECORD_FROM_LINK (*DispatchHandle);
    Record->ClearSource = PchTcoClearNewCenturySts;
    PchTcoClearNewCenturySts (&Record->SrcDesc);
    PchSmmEnableSource (&Record->SrcDesc);
  }
  return Status;
}

/**
  Unregister a child SMI source dispatch function with a parent SMM driver

  @param[in] This                       Protocol instance pointer.
  @param[in] DispatchHandle             Handle of dispatch function to deregister.

  @retval EFI_SUCCESS                   The dispatch function has been successfully
                                        unregistered and the SMI source has been disabled
                                        if there are no other registered child dispatch
                                        functions for this SMI source.
  @retval EFI_INVALID_PARAMETER         Handle is invalid.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchTcoSmiUnRegister (
  IN  PCH_TCO_SMI_DISPATCH_PROTOCOL     *This,
  IN  EFI_HANDLE                        DispatchHandle
  )
{
  DATABASE_RECORD                       *Record;

  Record = DATABASE_RECORD_FROM_LINK (DispatchHandle);
  if ((Record->SrcDesc.En[1].Reg.Type == ACPI_ADDR_TYPE) &&
      (Record->SrcDesc.En[1].Reg.Data.pcie.Fields.Dev == PCI_DEVICE_NUMBER_PCH_SPI) &&
      (Record->SrcDesc.En[1].Reg.Data.pcie.Fields.Fnc == PCI_FUNCTION_NUMBER_PCH_SPI) &&
      (Record->SrcDesc.En[1].Reg.Data.pcie.Fields.Reg == R_PCH_SPI_BC) &&
      (Record->SrcDesc.En[1].Bit == N_PCH_SPI_BC_BLE)) {
    //
    // SPI Write Protect cannot be disabled
    //
    return EFI_ACCESS_DENIED;
  } else if ((Record->SrcDesc.En[1].Reg.Type == ACPI_ADDR_TYPE) &&
             (Record->SrcDesc.En[1].Reg.Data.pcie.Fields.Dev == PCI_DEVICE_NUMBER_PCH_LPC) &&
             (Record->SrcDesc.En[1].Reg.Data.pcie.Fields.Fnc == PCI_FUNCTION_NUMBER_PCH_LPC) &&
             (Record->SrcDesc.En[1].Reg.Data.pcie.Fields.Reg == R_PCH_LPC_BC) &&
             (Record->SrcDesc.En[1].Bit == N_PCH_LPC_BC_LE)) {
    //
    // eSPI/LPC Write Protect cannot be disabled
    //
    return EFI_ACCESS_DENIED;
  }
  return PchSmiRecordUnRegister (DispatchHandle);
}


//
// PcieRpHotPlug srcdesc
//
PCH_SMM_SOURCE_DESC PchPcieSmiRpHotPlugTemplate = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        PCIE_ADDR_TYPE,
        {R_PCH_PCIE_MPC}
      },
      S_PCH_PCIE_MPC,
      N_PCH_PCIE_MPC_HPME
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        PCIE_ADDR_TYPE,
        {R_PCH_PCIE_SMSCS}
      },
      S_PCH_PCIE_SMSCS,
      N_PCH_PCIE_SMSCS_HPPDM
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_PCH_SMI_STS}
    },
    S_PCH_SMI_STS,
    N_PCH_SMI_STS_PCI_EXP
  }
};

/**
  The register function used to register SMI handler of PCIE RP hotplug event.

  @param[in]  This                      The pointer to the protocol itself
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[in]  RpIndex                   Indicate the RP index (0-based)
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchPcieSmiHotPlugRegister (
  IN  PCH_PCIE_SMI_DISPATCH_PROTOCOL    *This,
  IN  PCH_PCIE_SMI_RP_DISPATCH_CALLBACK DispatchFunction,
  IN  UINTN                             RpIndex,
  OUT EFI_HANDLE                        *DispatchHandle
  )
{
  EFI_STATUS                            Status;
  UINTN                                 RpDev;
  UINTN                                 RpFun;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  GetPchPcieRpDevFun (RpIndex, &RpDev, &RpFun);
  //
  // Patch the RP device number and function number of srcdesc.
  //
  PchPcieSmiRpHotPlugTemplate.En[0].Reg.Data.pcie.Fields.Dev = (UINT8) RpDev;
  PchPcieSmiRpHotPlugTemplate.En[0].Reg.Data.pcie.Fields.Fnc = (UINT8) RpFun;
  PchPcieSmiRpHotPlugTemplate.Sts[0].Reg.Data.pcie.Fields.Dev = (UINT8) RpDev;
  PchPcieSmiRpHotPlugTemplate.Sts[0].Reg.Data.pcie.Fields.Fnc = (UINT8) RpFun;

  Status = PchSmiRecordInsert (
             &gPchPcieSmiDispatchProtocolGuid,
             &PchPcieSmiRpHotPlugTemplate,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchPcieSmiRpHotplugType,
             DispatchHandle
             );
  PchSmmClearSource (&PchPcieSmiRpHotPlugTemplate);
  PchSmmEnableSource (&PchPcieSmiRpHotPlugTemplate);

  return Status;
}

//
// PcieRpLinkActive srcdesc
//
PCH_SMM_SOURCE_DESC PchPcieSmiRpLinkActiveTemplate = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        PCIE_ADDR_TYPE,
        {R_PCH_PCIE_MPC}
      },
      S_PCH_PCIE_MPC,
      N_PCH_PCIE_MPC_HPME
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        PCIE_ADDR_TYPE,
        {R_PCH_PCIE_SMSCS}
      },
      S_PCH_PCIE_SMSCS,
      N_PCH_PCIE_SMSCS_HPLAS
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_PCH_SMI_STS}
    },
    S_PCH_SMI_STS,
    N_PCH_SMI_STS_PCI_EXP
  }
};

/**
  The register function used to register SMI handler of PCIE RP link active event.

  @param[in]  This                      The pointer to the protocol itself
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[in]  RpIndex                   Indicate the RP index (0-based)
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchPcieSmiLinkActiveRegister (
  IN  PCH_PCIE_SMI_DISPATCH_PROTOCOL    *This,
  IN  PCH_PCIE_SMI_RP_DISPATCH_CALLBACK DispatchFunction,
  IN  UINTN                             RpIndex,
  OUT EFI_HANDLE                        *DispatchHandle
  )
{
  EFI_STATUS                            Status;
  UINTN                                 RpDev;
  UINTN                                 RpFun;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  GetPchPcieRpDevFun (RpIndex, &RpDev, &RpFun);
  //
  // Patch the RP device number and function number of srcdesc.
  //
  PchPcieSmiRpLinkActiveTemplate.En[0].Reg.Data.pcie.Fields.Dev = (UINT8) RpDev;
  PchPcieSmiRpLinkActiveTemplate.En[0].Reg.Data.pcie.Fields.Fnc = (UINT8) RpFun;
  PchPcieSmiRpLinkActiveTemplate.Sts[0].Reg.Data.pcie.Fields.Dev = (UINT8) RpDev;
  PchPcieSmiRpLinkActiveTemplate.Sts[0].Reg.Data.pcie.Fields.Fnc = (UINT8) RpFun;

  Status = PchSmiRecordInsert (
             &gPchPcieSmiDispatchProtocolGuid,
             &PchPcieSmiRpLinkActiveTemplate,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchPcieSmiRpLinkActiveType,
             DispatchHandle
             );
  PchSmmClearSource (&PchPcieSmiRpLinkActiveTemplate);
  PchSmmEnableSource (&PchPcieSmiRpLinkActiveTemplate);

  return Status;
}

//
// PcieRpLinkEq srcdesc
//
PCH_SMM_SOURCE_DESC PchPcieSmiRpLinkEqTemplate = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        PCIE_ADDR_TYPE,
        {R_PCH_PCIE_EQCFG1}
      },
      S_PCH_PCIE_EQCFG1,
      N_PCH_PCIE_EQCFG1_LERSMIE
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        PCIE_ADDR_TYPE,
        {R_PCH_PCIE_SMSCS}
      },
      S_PCH_PCIE_SMSCS,
      N_PCH_PCIE_SMSCS_LERSMIS
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_PCH_SMI_STS}
    },
    S_PCH_SMI_STS,
    N_PCH_SMI_STS_PCI_EXP
  }
};

/**
  The register function used to register SMI handler of PCIE RP Link Equalization Request event.

  @param[in]  This                      The pointer to the protocol itself
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[in]  RpIndex                   Indicate the RP index (0-based)
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchPcieSmiLinkEqRegister (
  IN  PCH_PCIE_SMI_DISPATCH_PROTOCOL    *This,
  IN  PCH_PCIE_SMI_RP_DISPATCH_CALLBACK DispatchFunction,
  IN  UINTN                             RpIndex,
  OUT EFI_HANDLE                        *DispatchHandle
  )
{
  UINTN                                 RpDev;
  UINTN                                 RpFun;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  GetPchPcieRpDevFun (RpIndex, &RpDev, &RpFun);
  //
  // Patch the RP device number and function number of srcdesc.
  //
  PchPcieSmiRpLinkEqTemplate.En[0].Reg.Data.pcie.Fields.Dev = (UINT8) RpDev;
  PchPcieSmiRpLinkEqTemplate.En[0].Reg.Data.pcie.Fields.Fnc = (UINT8) RpFun;
  PchPcieSmiRpLinkEqTemplate.Sts[0].Reg.Data.pcie.Fields.Dev = (UINT8) RpDev;
  PchPcieSmiRpLinkEqTemplate.Sts[0].Reg.Data.pcie.Fields.Fnc = (UINT8) RpFun;

  return PchSmiRecordInsert (
           &gPchPcieSmiDispatchProtocolGuid,
           &PchPcieSmiRpLinkEqTemplate,
           (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
           PchPcieSmiRpLinkEqType,
           DispatchHandle
           );
}

/**
  Unregister a child SMI source dispatch function with a parent SMM driver

  @param[in] This                       Protocol instance pointer.
  @param[in] DispatchHandle             Handle of dispatch function to deregister.

  @retval EFI_SUCCESS                   The dispatch function has been successfully
                                        unregistered and the SMI source has been disabled
                                        if there are no other registered child dispatch
                                        functions for this SMI source.
  @retval EFI_INVALID_PARAMETER         Handle is invalid.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchPcieSmiUnRegister (
  IN  PCH_PCIE_SMI_DISPATCH_PROTOCOL    *This,
  IN  EFI_HANDLE                        DispatchHandle
  )
{
  return PchSmiRecordUnRegister (DispatchHandle);
}

//
// Pme srcdesc
//
PCH_SMM_SOURCE_DESC mSrcDescPme = {
  PCH_SMM_SCI_EN_DEPENDENT,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_ACPI_GPE0_EN_127_96}
      },
      S_PCH_ACPI_GPE0_EN_127_96,
      N_PCH_ACPI_GPE0_EN_127_96_PME
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_ACPI_GPE0_STS_127_96}
      },
      S_PCH_ACPI_GPE0_STS_127_96,
      N_PCH_ACPI_GPE0_STS_127_96_PME
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_PCH_SMI_STS}
    },
    S_PCH_SMI_STS,
    N_PCH_SMI_STS_GPE0
  }
};

/**
  The register function used to register SMI handler of PME event.

  @param[in]  This                      The pointer to the protocol itself
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchAcpiSmiPmeRegister (
  IN  PCH_ACPI_SMI_DISPATCH_PROTOCOL    *This,
  IN  PCH_ACPI_SMI_DISPATCH_CALLBACK    DispatchFunction,
  OUT EFI_HANDLE                        *DispatchHandle
  )
{
  EFI_STATUS                            Status;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = PchSmiRecordInsert (
             &gPchAcpiSmiDispatchProtocolGuid,
             &mSrcDescPme,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchAcpiSmiPmeType,
             DispatchHandle
             );
  PchSmmClearSource (&mSrcDescPme);
  PchSmmEnableSource (&mSrcDescPme);

  return Status;
}

//
// PmeB0 srcdesc
//
PCH_SMM_SOURCE_DESC mSrcDescPmeB0 = {
  PCH_SMM_SCI_EN_DEPENDENT,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_ACPI_GPE0_EN_127_96}
      },
      S_PCH_ACPI_GPE0_EN_127_96,
      N_PCH_ACPI_GPE0_EN_127_96_PME_B0
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_ACPI_GPE0_STS_127_96}
      },
      S_PCH_ACPI_GPE0_STS_127_96,
      N_PCH_ACPI_GPE0_STS_127_96_PME_B0
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_PCH_SMI_STS}
    },
    S_PCH_SMI_STS,
    N_PCH_SMI_STS_GPE0
  }
};

/**
  The register function used to register SMI handler of PME B0 event.

  @param[in]  This                      The pointer to the protocol itself
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchAcpiSmiPmeB0Register (
  IN  PCH_ACPI_SMI_DISPATCH_PROTOCOL    *This,
  IN  PCH_ACPI_SMI_DISPATCH_CALLBACK    DispatchFunction,
  OUT EFI_HANDLE                        *DispatchHandle
  )
{
  EFI_STATUS                            Status;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = PchSmiRecordInsert (
             &gPchAcpiSmiDispatchProtocolGuid,
             &mSrcDescPmeB0,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchAcpiSmiPmeB0Type,
             DispatchHandle
             );
  PchSmmClearSource (&mSrcDescPmeB0);
  PchSmmEnableSource (&mSrcDescPmeB0);

  return Status;
}

//
// RtcAlarm srcdesc
//
PCH_SMM_SOURCE_DESC mSrcDescRtcAlarm = {
  PCH_SMM_SCI_EN_DEPENDENT,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_ACPI_PM1_EN}
      },
      S_PCH_ACPI_PM1_EN,
      N_PCH_ACPI_PM1_EN_RTC
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_ACPI_PM1_STS}
      },
      S_PCH_ACPI_PM1_STS,
      N_PCH_ACPI_PM1_STS_RTC
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_PCH_SMI_STS}
    },
    S_PCH_SMI_STS,
    N_PCH_SMI_STS_PM1_STS_REG
  }
};

/**
  The register function used to register SMI handler of RTC alarm event.

  @param[in]  This                      The pointer to the protocol itself
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchAcpiSmiRtcAlarmRegister (
  IN  PCH_ACPI_SMI_DISPATCH_PROTOCOL    *This,
  IN  PCH_ACPI_SMI_DISPATCH_CALLBACK    DispatchFunction,
  OUT EFI_HANDLE                        *DispatchHandle
  )
{
  EFI_STATUS                            Status;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = PchSmiRecordInsert (
             &gPchAcpiSmiDispatchProtocolGuid,
             &mSrcDescRtcAlarm,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchAcpiSmiRtcAlarmType,
             DispatchHandle
             );
  PchSmmClearSource (&mSrcDescRtcAlarm);
  PchSmmEnableSource (&mSrcDescRtcAlarm);

  return Status;
}

//
// TmrOverflow srcdesc
//
PCH_SMM_SOURCE_DESC mSrcDescTmrOverflow = {
  PCH_SMM_SCI_EN_DEPENDENT,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_ACPI_PM1_EN}
      },
      S_PCH_ACPI_PM1_EN,
      N_PCH_ACPI_PM1_EN_TMROF
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_ACPI_PM1_STS}
      },
      S_PCH_ACPI_PM1_STS,
      N_PCH_ACPI_PM1_STS_TMROF
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_PCH_SMI_STS}
    },
    S_PCH_SMI_STS,
    N_PCH_SMI_STS_PM1_STS_REG
  }
};

/**
  The register function used to register SMI handler of Timer Overflow event.

  @param[in]  This                      The pointer to the protocol itself
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchAcpiSmiTmrOverflowRegister (
  IN  PCH_ACPI_SMI_DISPATCH_PROTOCOL    *This,
  IN  PCH_ACPI_SMI_DISPATCH_CALLBACK    DispatchFunction,
  OUT EFI_HANDLE                        *DispatchHandle
  )
{
  EFI_STATUS                            Status;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = PchSmiRecordInsert (
             &gPchAcpiSmiDispatchProtocolGuid,
             &mSrcDescTmrOverflow,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchAcpiSmiTmrOverflowType,
             DispatchHandle
             );
  PchSmmClearSource (&mSrcDescTmrOverflow);
  PchSmmEnableSource (&mSrcDescTmrOverflow);

  return Status;
}

/**
  Unregister a child SMI source dispatch function with a parent SMM driver

  @param[in] This                       Protocol instance pointer.
  @param[in] DispatchHandle             Handle of dispatch function to deregister.

  @retval EFI_SUCCESS                   The dispatch function has been successfully
                                        unregistered and the SMI source has been disabled
                                        if there are no other registered child dispatch
                                        functions for this SMI source.
  @retval EFI_INVALID_PARAMETER         Handle is invalid.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchAcpiSmiUnRegister (
  IN  PCH_ACPI_SMI_DISPATCH_PROTOCOL    *This,
  IN  EFI_HANDLE                        DispatchHandle
  )
{
  return PchSmiRecordUnRegister (DispatchHandle);
}

//
// GpioUnlock srcdesc
//
PCH_SMM_SOURCE_DESC mSrcDescGpioUnlock = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_SMI_EN}
      },
      S_PCH_SMI_EN,
      N_PCH_SMI_EN_GPIO_UNLOCK
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_SMI_STS}
      },
      S_PCH_SMI_STS,
      N_PCH_SMI_STS_GPIO_UNLOCK
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_PCH_SMI_STS}
    },
    S_PCH_SMI_STS,
    N_PCH_SMI_STS_GPIO_UNLOCK
  }
};

/**
  The register function used to register SMI handler of GPIO Unlock event.

  @param[in]  This                      The pointer to the protocol itself
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchGpioUnlockSmiRegister (
  IN  PCH_GPIO_UNLOCK_SMI_DISPATCH_PROTOCOL       *This,
  IN  PCH_GPIO_UNLOCK_SMI_DISPATCH_CALLBACK       DispatchFunction,
  OUT EFI_HANDLE                                  *DispatchHandle
  )
{
  EFI_STATUS                                      Status;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = PchSmiRecordInsert (
             &gPchGpioUnlockSmiDispatchProtocolGuid,
             &mSrcDescGpioUnlock,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchGpioUnlockSmiType,
             DispatchHandle
             );
  PchSmmClearSource (&mSrcDescGpioUnlock);
  PchSmmEnableSource (&mSrcDescGpioUnlock);

  return Status;
}

/**
  Unregister a child SMI source dispatch function with a parent GPIO UNLOCK SMM driver

  @param[in] This                       Protocol instance pointer.
  @param[in] DispatchHandle             Handle of dispatch function to deregister.

  @retval EFI_SUCCESS                   The dispatch function has been successfully
                                        unregistered and the SMI source has been disabled
                                        if there are no other registered child dispatch
                                        functions for this SMI source.
  @retval EFI_INVALID_PARAMETER         Handle is invalid.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchGpioUnlockSmiUnRegister (
  IN  PCH_GPIO_UNLOCK_SMI_DISPATCH_PROTOCOL       *This,
  IN  EFI_HANDLE                                  DispatchHandle
  )
{
  return PchSmiRecordUnRegister (DispatchHandle);
}

//
// SerialIrq srcdesc
//
PCH_SMM_SOURCE_DESC mSrcDescSerialIrq = {
  PCH_SMM_NO_FLAGS,
  {
    NULL_BIT_DESC_INITIALIZER,
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_SMI_STS}
      },
      S_PCH_SMI_STS,
      N_PCH_SMI_STS_SERIRQ
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_PCH_SMI_STS}
    },
    S_PCH_SMI_STS,
    N_PCH_SMI_STS_SERIRQ
  }
};

/**
  The register function used to register SMI handler of Serial IRQ event.

  @param[in]  This                      The pointer to the protocol itself
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchSmiSerialIrqRegister (
  IN  PCH_SMI_DISPATCH_PROTOCOL         *This,
  IN  PCH_SMI_DISPATCH_CALLBACK         DispatchFunction,
  OUT EFI_HANDLE                        *DispatchHandle
  )
{
  EFI_STATUS                            Status;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = PchSmiRecordInsert (
             &gPchSmiDispatchProtocolGuid,
             &mSrcDescSerialIrq,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchSmiSerialIrqType,
             DispatchHandle
             );
  PchSmmClearSource (&mSrcDescSerialIrq);
  PchSmmEnableSource (&mSrcDescSerialIrq);

  return Status;
}

//
// McSmi srcdesc
//
PCH_SMM_SOURCE_DESC mSrcDescMcSmi = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_SMI_EN}
      },
      S_PCH_SMI_EN,
      N_PCH_SMI_EN_MCSMI
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_SMI_STS}
      },
      S_PCH_SMI_STS,
      N_PCH_SMI_STS_MCSMI
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_PCH_SMI_STS}
    },
    S_PCH_SMI_STS,
    N_PCH_SMI_STS_MCSMI
  }
};

/**
  The register function used to register SMI handler of MCSMI event.

  @param[in]  This                      The pointer to the protocol itself
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchSmiMcSmiRegister (
  IN  PCH_SMI_DISPATCH_PROTOCOL         *This,
  IN  PCH_SMI_DISPATCH_CALLBACK         DispatchFunction,
  OUT EFI_HANDLE                        *DispatchHandle
  )
{
  EFI_STATUS                            Status;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = PchSmiRecordInsert (
             &gPchSmiDispatchProtocolGuid,
             &mSrcDescMcSmi,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchSmiMcSmiType,
             DispatchHandle
             );
  PchSmmClearSource (&mSrcDescMcSmi);
  PchSmmEnableSource (&mSrcDescMcSmi);

  return Status;
}

//
// SmBus srcdesc
//
PCH_SMM_SOURCE_DESC mSrcDescSmbus = {
  PCH_SMM_NO_FLAGS,
  {
    NULL_BIT_DESC_INITIALIZER,
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_SMI_STS}
      },
      S_PCH_SMI_STS,
      N_PCH_SMI_STS_SMBUS
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_PCH_SMI_STS}
    },
    S_PCH_SMI_STS,
    N_PCH_SMI_STS_SMBUS
  }
};

/**
  The register function used to register SMI handler of SMBUS event.

  @param[in]  This                      The pointer to the protocol itself
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchSmiSmbusRegister (
  IN  PCH_SMI_DISPATCH_PROTOCOL         *This,
  IN  PCH_SMI_DISPATCH_CALLBACK         DispatchFunction,
  OUT EFI_HANDLE                        *DispatchHandle
  )
{
  EFI_STATUS                            Status;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = PchSmiRecordInsert (
             &gPchSmiDispatchProtocolGuid,
             &mSrcDescSmbus,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchSmiSmBusType,
             DispatchHandle
             );
  PchSmmClearSource (&mSrcDescSmbus);
  PchSmmEnableSource (&mSrcDescSmbus);

  return Status;
}

//
// SpiAsyncSmi srcdesc
//
PCH_SMM_SOURCE_DESC mSrcDescSpiAsyncSmi = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        PCIE_ADDR_TYPE,
        { (
          (DEFAULT_PCI_BUS_NUMBER_PCH << 24) |
          (PCI_DEVICE_NUMBER_PCH_SPI << 19) |
          (PCI_FUNCTION_NUMBER_PCH_SPI << 16) |
          R_PCH_SPI_BC
        ) }
      },
      S_PCH_SPI_BC,
      N_PCH_SPI_BC_ASE_BWP
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        PCIE_ADDR_TYPE,
        { (
          (DEFAULT_PCI_BUS_NUMBER_PCH << 24) |
          (PCI_DEVICE_NUMBER_PCH_SPI << 19) |
          (PCI_FUNCTION_NUMBER_PCH_SPI << 16) |
          R_PCH_SPI_BC
        ) }
      },
      S_PCH_SPI_BC,
      N_PCH_SPI_BC_ASYNC_SS
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_PCH_SMI_STS}
    },
    S_PCH_SMI_STS,
    N_PCH_SMI_STS_SPI
  }
};

/**
  Special handling for SPI Asynchronous SMI.
  If SPI ASYNC SMI is enabled, De-assert SMI is sent when Flash Cycle Done
  transitions from 1 to 0 or when the SMI enable becomes false.

  @param[in]  SrcDesc   Not used
**/
VOID
EFIAPI
PchSmiSpiAsyncClearSource (
  PCH_SMM_SOURCE_DESC                   *SrcDesc
  )
{
  UINTN                                 SpiRegBase;
  UINT32                                SpiBar0;

  SpiRegBase = MmPciBase (
                 DEFAULT_PCI_BUS_NUMBER_PCH,
                 PCI_DEVICE_NUMBER_PCH_SPI,
                 PCI_FUNCTION_NUMBER_PCH_SPI
                 );
  SpiBar0 = MmioRead32 (SpiRegBase + R_PCH_SPI_BAR0) & ~(B_PCH_SPI_BAR0_MASK);
  if (SpiBar0 != PCH_SPI_BASE_ADDRESS) {
    //
    // Temporary disable MSE, and override with SPI reserved MMIO address, then enable MSE.
    //
    SpiBar0 = PCH_SPI_BASE_ADDRESS;
    MmioAnd8 (SpiRegBase + PCI_COMMAND_OFFSET, (UINT8) ~EFI_PCI_COMMAND_MEMORY_SPACE);
    MmioWrite32 (SpiRegBase + R_PCH_SPI_BAR0, SpiBar0);
    MmioOr8 (SpiRegBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
  }

  MmioOr32 (SpiBar0 + R_PCH_SPI_HSFSC, B_PCH_SPI_HSFSC_FDONE);
}

/**
  Special handling to enable SPI Asynchronous SMI
**/
VOID
PchSmiSpiAsyncEnableSource (
  VOID
  )
{
  UINTN SpiRegBase;
  SpiRegBase = MmPciBase (
                 DEFAULT_PCI_BUS_NUMBER_PCH,
                 PCI_DEVICE_NUMBER_PCH_SPI,
                 PCI_FUNCTION_NUMBER_PCH_SPI
                 );
  MmioAndThenOr32 (
    SpiRegBase + R_PCH_SPI_BC,
    (UINT32) ~B_PCH_SPI_BC_SYNC_SS,
    B_PCH_SPI_BC_ASE_BWP
    );

  //
  // Clear the source
  //
  PchSmiSpiAsyncClearSource (NULL);
}

/**
  The register function used to register SMI handler of SPI Asynchronous event.

  @param[in]  This                      The pointer to the protocol itself
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchSmiSpiAsyncRegister (
  IN  PCH_SMI_DISPATCH_PROTOCOL         *This,
  IN  PCH_SMI_DISPATCH_CALLBACK         DispatchFunction,
  OUT EFI_HANDLE                        *DispatchHandle
  )
{
  EFI_STATUS                            Status;
  DATABASE_RECORD                       *Record;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = PchSmiRecordInsert (
             &gPchSmiDispatchProtocolGuid,
             &mSrcDescSpiAsyncSmi,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchSmiSpiAsyncType,
             DispatchHandle
             );
  if (!EFI_ERROR (Status)) {
    Record = DATABASE_RECORD_FROM_LINK (*DispatchHandle);
    Record->ClearSource = PchSmiSpiAsyncClearSource;
    PchSmiSpiAsyncClearSource (NULL);
    PchSmiSpiAsyncEnableSource ();
  }
  return Status;
}

/**
  Unregister a child SMI source dispatch function with a parent SMM driver

  @param[in] This                       Protocol instance pointer.
  @param[in] DispatchHandle             Handle of dispatch function to deregister.

  @retval EFI_SUCCESS                   The dispatch function has been successfully
                                        unregistered and the SMI source has been disabled
                                        if there are no other registered child dispatch
                                        functions for this SMI source.
  @retval EFI_INVALID_PARAMETER         Handle is invalid.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
  @retval EFI_ACCESS_DENIED             Return access denied since SPI aync SMI handler is not able to disabled.
**/
EFI_STATUS
EFIAPI
PchSmiUnRegister (
  IN  PCH_SMI_DISPATCH_PROTOCOL         *This,
  IN  EFI_HANDLE                        DispatchHandle
  )
{
  DATABASE_RECORD                       *Record;
  UINTN                                 SpiRegBase;

  Record = DATABASE_RECORD_FROM_LINK (DispatchHandle);
  if ((Record->SrcDesc.En[0].Reg.Type == PCIE_ADDR_TYPE) &&
      (Record->SrcDesc.En[0].Reg.Data.pcie.Fields.Dev == PCI_DEVICE_NUMBER_PCH_SPI) &&
      (Record->SrcDesc.En[0].Reg.Data.pcie.Fields.Fnc == PCI_FUNCTION_NUMBER_PCH_SPI) &&
      (Record->SrcDesc.En[0].Reg.Data.pcie.Fields.Reg == R_PCH_SPI_BC) &&
      (Record->SrcDesc.En[0].Bit == N_PCH_SPI_BC_ASE_BWP)) {
    SpiRegBase = MmPciBase (
                   DEFAULT_PCI_BUS_NUMBER_PCH,
                   PCI_DEVICE_NUMBER_PCH_SPI,
                   PCI_FUNCTION_NUMBER_PCH_SPI
                   );
    if (MmioRead8 (SpiRegBase + R_PCH_SPI_BC) & B_PCH_SPI_BC_BILD) {
      //
      // SPI Asynchronous SMI cannot be disabled
      //
      return EFI_ACCESS_DENIED;
    }
  }

  return PchSmiRecordUnRegister (DispatchHandle);
}


/**
  Declaration of PCH TCO SMI DISPATCH PROTOCOL instance
**/
PCH_TCO_SMI_DISPATCH_PROTOCOL mPchTcoSmiDispatchProtocol = {
  PCH_TCO_SMI_DISPATCH_REVISION,        // Revision
  PchTcoSmiUnRegister,                  // Unregister
  PchTcoSmiMchRegister,                 // Mch
  PchTcoSmiTcoTimeoutRegister,          // TcoTimeout
  PchTcoSmiOsTcoRegister,               // OsTco
  PchTcoSmiNmiRegister,                 // Nmi
  PchTcoSmiIntruderDetRegister,         // IntruderDectect
  PchTcoSmiSpiBiosWpRegister,           // SpiBiosWp
  PchTcoSmiLpcBiosWpRegister,           // LpcBiosWp
  PchTcoSmiNewCenturyRegister           // NewCentury
};

/**
  Declaration of PCH PCIE SMI DISPATCH PROTOCOL instance
**/
PCH_PCIE_SMI_DISPATCH_PROTOCOL mPchPcieSmiDispatchProtocol = {
  PCH_PCIE_SMI_DISPATCH_REVISION,       // Revision
  PchPcieSmiUnRegister,                 // Unregister
  PchPcieSmiHotPlugRegister,            // PcieRpXHotPlug
  PchPcieSmiLinkActiveRegister,         // PcieRpXLinkActive
  PchPcieSmiLinkEqRegister              // PcieRpXLinkEq
};

/**
  Declaration of PCH ACPI SMI DISPATCH PROTOCOL instance
**/
PCH_ACPI_SMI_DISPATCH_PROTOCOL mPchAcpiSmiDispatchProtocol = {
  PCH_ACPI_SMI_DISPATCH_REVISION,       // Revision
  PchAcpiSmiUnRegister,                 // Unregister
  PchAcpiSmiPmeRegister,                // Pme
  PchAcpiSmiPmeB0Register,              // PmeB0
  PchAcpiSmiRtcAlarmRegister,           // RtcAlarm
  PchAcpiSmiTmrOverflowRegister         // TmrOverflow
};

/**
  Declaration of PCH GPIO UNLOCK SMI DISPATCH PROTOCOL instance
**/
PCH_GPIO_UNLOCK_SMI_DISPATCH_PROTOCOL mPchGpioUnlockSmiDispatchProtocol = {
  PCH_GPIO_UNLOCK_SMI_DISPATCH_REVISION,// Revision
  PchGpioUnlockSmiUnRegister,           // Unregister
  PchGpioUnlockSmiRegister              // GpioUnlock
};

/**
  Declaration of MISC PCH SMI DISPATCH PROTOCOL instance
**/
PCH_SMI_DISPATCH_PROTOCOL mPchSmiDispatchProtocol = {
  PCH_SMI_DISPATCH_REVISION,            // Revision
  PchSmiUnRegister,                     // Unregister
  PchSmiSerialIrqRegister,              // SerialIrq
  PchSmiMcSmiRegister,                  // McSmi
  PchSmiSmbusRegister,                  // SmBus
  PchSmiSpiAsyncRegister                // SpiAsync
};

/**
  Install protocols of PCH specifics SMI types, including
  PCH TCO SMI types, PCH PCIE SMI types, PCH ACPI SMI types, PCH MISC SMI types.

  @retval                               the result of protocol installation
**/
EFI_STATUS
InstallPchSmiDispatchProtocols (
  VOID
  )
{
  EFI_HANDLE                            Handle;
  EFI_STATUS                            Status;

  Handle = NULL;
  Status = gSmst->SmmInstallProtocolInterface (
                    &Handle,
                    &gPchTcoSmiDispatchProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mPchTcoSmiDispatchProtocol
                    );
  Status = gSmst->SmmInstallProtocolInterface (
                    &Handle,
                    &gPchPcieSmiDispatchProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mPchPcieSmiDispatchProtocol
                    );
  Status = gSmst->SmmInstallProtocolInterface (
                    &Handle,
                    &gPchAcpiSmiDispatchProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mPchAcpiSmiDispatchProtocol
                    );
  Status = gSmst->SmmInstallProtocolInterface (
                    &Handle,
                    &gPchGpioUnlockSmiDispatchProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mPchGpioUnlockSmiDispatchProtocol
                    );
  Status = gSmst->SmmInstallProtocolInterface (
                    &Handle,
                    &gPchSmiDispatchProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mPchSmiDispatchProtocol
                    );

  return Status;
}

/**
  The function to dispatch all callback function of PCH SMI types.

  @retval EFI_SUCCESS                   Function successfully completed
  @retval EFI_UNSUPPORTED               no
**/
EFI_STATUS
PchSmiTypeCallbackDispatcher (
  IN  DATABASE_RECORD                   *Record
  )
{
  EFI_STATUS                            Status;
  PCH_SMI_TYPES                         PchSmiType;
  UINTN                                 RpIndex;
  PCH_PCIE_SMI_RP_CONTEXT               RpContext;

  PchSmiType = Record->PchSmiType;
  Status     = EFI_SUCCESS;

  switch (PchSmiType) {
    case PchTcoSmiMchType:
    case PchTcoSmiTcoTimeoutType:
    case PchTcoSmiOsTcoType:
    case PchTcoSmiNmiType:
    case PchTcoSmiIntruderDetectType:
    case PchTcoSmiSpiBiosWpType:
    case PchTcoSmiLpcBiosWpType:
    case PchTcoSmiNewCenturyType:
      ((PCH_TCO_SMI_DISPATCH_CALLBACK) (Record->PchSmiCallback)) ((EFI_HANDLE)&Record->Link);
      break;
    case PchPcieSmiRpHotplugType:
    case PchPcieSmiRpLinkActiveType:
    case PchPcieSmiRpLinkEqType:
      RpContext.BusNum  = DEFAULT_PCI_BUS_NUMBER_PCH;
      RpContext.DevNum  = (UINT8) Record->SrcDesc.En[0].Reg.Data.pcie.Fields.Dev;
      RpContext.FuncNum = (UINT8) Record->SrcDesc.En[0].Reg.Data.pcie.Fields.Fnc;
      GetPchPcieRpNumber (RpContext.DevNum, RpContext.FuncNum, &RpIndex);
      RpContext.RpIndex = (UINT8) RpIndex;
      ((PCH_PCIE_SMI_RP_DISPATCH_CALLBACK) (Record->PchSmiCallback)) ((EFI_HANDLE)&Record->Link, &RpContext);
      break;
    case PchAcpiSmiPmeType:
    case PchAcpiSmiPmeB0Type:
    case PchAcpiSmiRtcAlarmType:
    case PchAcpiSmiTmrOverflowType:
      ((PCH_ACPI_SMI_DISPATCH_CALLBACK) (Record->PchSmiCallback)) ((EFI_HANDLE)&Record->Link);
      break;
    case PchGpioUnlockSmiType:
      ((PCH_GPIO_UNLOCK_SMI_DISPATCH_CALLBACK) (Record->PchSmiCallback)) ((EFI_HANDLE)&Record->Link);
      break;
    case PchEspiSmiEspiSlaveType:
      ((PCH_ESPI_SMI_DISPATCH_CALLBACK) (Record->PchSmiCallback)) ((EFI_HANDLE)&Record->Link);
      break;
    case PchSmiSerialIrqType:
    case PchSmiMcSmiType:
    case PchSmiSmBusType:
    case PchSmiSpiAsyncType:
    case PchIoTrapSmiType:                ///< internal type for IoTrap
      ((PCH_SMI_DISPATCH_CALLBACK) (Record->PchSmiCallback)) ((EFI_HANDLE)&Record->Link);
      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
  }

  return Status;
}

PCH_SMM_SOURCE_DESC mSrcDescIoTrap[4] = {
  //
  // PCH I/O Trap register 0 monitor
  //
  {
    PCH_SMM_NO_FLAGS,
    {
      {
        {
          PCR_ADDR_TYPE,
          {PCH_PCR_ADDRESS (PID_PSTH, R_PCH_PCR_PSTH_TRPREG0) }
        },
        4,
        0
      },
      NULL_BIT_DESC_INITIALIZER
    },
    {
      {
        {
          PCR_ADDR_TYPE,
          {PCH_PCR_ADDRESS (PID_PSTH, R_PCH_PCR_PSTH_TRPST) }
        },
        1,
        0
      }
    },
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_SMI_STS}
      },
      S_PCH_SMI_STS,
      N_PCH_SMI_STS_MONITOR
    }
  },
  //
  // PCH I/O Trap register 1 monitor
  //
  {
    PCH_SMM_NO_FLAGS,
    {
      {
        {
          PCR_ADDR_TYPE,
          {PCH_PCR_ADDRESS (PID_PSTH, R_PCH_PCR_PSTH_TRPREG1) }
        },
        4,
        0
      },
      NULL_BIT_DESC_INITIALIZER
    },
    {
      {
        {
          PCR_ADDR_TYPE,
          {PCH_PCR_ADDRESS (PID_PSTH, R_PCH_PCR_PSTH_TRPST) }
        },
        1,
        1
      }
    },
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_SMI_STS}
      },
      S_PCH_SMI_STS,
      N_PCH_SMI_STS_MONITOR
    }
  },
  //
  // PCH I/O Trap register 2 monitor
  //
  {
    PCH_SMM_NO_FLAGS,
    {
      {
        {
          PCR_ADDR_TYPE,
          {PCH_PCR_ADDRESS (PID_PSTH, R_PCH_PCR_PSTH_TRPREG2) }
        },
        4,
        0
      },
      NULL_BIT_DESC_INITIALIZER
    },
    {
      {
        {
          PCR_ADDR_TYPE,
          {PCH_PCR_ADDRESS (PID_PSTH, R_PCH_PCR_PSTH_TRPST) }
        },
        1,
        2
      }
    },
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_SMI_STS}
      },
      S_PCH_SMI_STS,
      N_PCH_SMI_STS_MONITOR
    }
  },
  //
  // PCH I/O Trap register 3 monitor,
  //
  {
    PCH_SMM_NO_FLAGS,
    {
      {
        {
          PCR_ADDR_TYPE,
          {PCH_PCR_ADDRESS (PID_PSTH, R_PCH_PCR_PSTH_TRPREG3) }
        },
        4,
        0
      },
      NULL_BIT_DESC_INITIALIZER
    },
    {
      {
        {
          PCR_ADDR_TYPE,
          {PCH_PCR_ADDRESS (PID_PSTH, R_PCH_PCR_PSTH_TRPST) }
        },
        1,
        3
      }
    },
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_SMI_STS}
      },
      S_PCH_SMI_STS,
      N_PCH_SMI_STS_MONITOR
    }
  }
};

/**
  The register function used to register SMI handler of IoTrap event.
  This is internal function and only used by Iotrap module.

  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[in]  IoTrapIndex               Index number of IOTRAP register
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
**/
EFI_STATUS
PchInternalIoTrapSmiRegister (
  IN  PCH_SMI_DISPATCH_CALLBACK         DispatchFunction,
  IN  UINTN                             IoTrapIndex,
  OUT EFI_HANDLE                        *DispatchHandle
  )
{
  EFI_STATUS                            Status;

  Status = PchSmiRecordInsert (
             NULL,
             &mSrcDescIoTrap[IoTrapIndex],
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchIoTrapSmiType,
             DispatchHandle
             );
  PchSmmClearSource (&mSrcDescIoTrap[IoTrapIndex]);
  PchSmmEnableSource (&mSrcDescIoTrap[IoTrapIndex]);

  return Status;
}

/**
  Unregister a child SMI source dispatch function with a parent SMM driver

  @param[in] DispatchHandle             Handle of dispatch function to deregister.

  @retval EFI_SUCCESS                   The dispatch function has been successfully
                                        unregistered and the SMI source has been disabled
                                        if there are no other registered child dispatch
                                        functions for this SMI source.
  @retval EFI_INVALID_PARAMETER         Handle is invalid.
**/
EFI_STATUS
PchInternalIoTrapSmiUnRegister (
  IN  EFI_HANDLE                        DispatchHandle
  )
{
  return PchSmmCoreUnRegister (NULL, DispatchHandle);
}
