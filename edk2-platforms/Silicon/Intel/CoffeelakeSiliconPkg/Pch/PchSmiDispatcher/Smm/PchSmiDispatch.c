/** @file
  This function handle the register/unregister of PCH specific SMI events.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PchSmmHelpers.h"
#include <Library/SmiHandlerProfileLib.h>
#include <Private/Library/PmcPrivateLib.h>
#include <Register/PchRegs.h>
#include <Register/PchRegsPcr.h>
#include <Register/PchRegsPmc.h>
#include <Register/PchRegsLpc.h>
#include <Register/PchRegsSpi.h>
#include <Register/PchRegsPcie.h>
#include <Register/PchRegsPsth.h>
#include <Register/PchRegsItss.h>

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
  IN  CONST PCH_SMM_SOURCE_DESC         *SrcDesc,
  IN  PCH_SMI_CALLBACK_FUNCTIONS        DispatchFunction,
  IN  PCH_SMI_TYPES                     PchSmiType,
  OUT EFI_HANDLE                        *DispatchHandle
  )
{
  EFI_STATUS                            Status;
  DATABASE_RECORD                       Record;

  if (SrcDesc == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&Record, sizeof (DATABASE_RECORD));
  //
  // Gather information about the registration request
  //
  Record.Signature                      = DATABASE_RECORD_SIGNATURE;
  Record.PchSmiCallback                 = DispatchFunction;
  Record.ProtocolType                   = PchSmiDispatchType;
  Record.PchSmiType                     = PchSmiType;

  CopyMem (&Record.SrcDesc, SrcDesc, sizeof (PCH_SMM_SOURCE_DESC));
  Status = SmmCoreInsertRecord (
             &Record,
             DispatchHandle
             );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}


//
// TCO_STS bit that needs to be cleared
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mDescSrcTcoSts = {
  PCH_SMM_NO_FLAGS,
  {
    NULL_BIT_DESC_INITIALIZER,
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_SMI_STS}
      },
      S_ACPI_IO_SMI_STS,
      N_ACPI_IO_SMI_STS_TCO
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
  CONST PCH_SMM_SOURCE_DESC             *SrcDesc
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
  CONST PCH_SMM_SOURCE_DESC             *SrcDesc
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
EFIAPI
NullInitSourceDesc (
   PCH_SMM_SOURCE_DESC                   *SrcDesc
   )
{
  ZeroMem (SrcDesc, sizeof (PCH_SMM_SOURCE_DESC));
  SrcDesc->En[0].Reg.Type = PCH_SMM_ADDR_TYPE_NULL;
  SrcDesc->En[1].Reg.Type = PCH_SMM_ADDR_TYPE_NULL;
  SrcDesc->Sts[0].Reg.Type = PCH_SMM_ADDR_TYPE_NULL;
  SrcDesc->PmcSmiSts.Reg.Type = PCH_SMM_ADDR_TYPE_NULL;
}

//
// Mch srcdesc
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mSrcDescMch = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_SMI_EN}
      },
      S_ACPI_IO_SMI_EN,
      N_ACPI_IO_SMI_EN_TCO
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        TCO_ADDR_TYPE,
        {R_TCO_IO_TCO1_STS}
      },
      S_TCO_IO_TCO1_STS,
      N_TCO_IO_TCO1_STS_DMISMI
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_TCO
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
    SmiHandlerProfileRegisterHandler (&gPchTcoSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

//
// TcoTimeout srcdesc
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mSrcDescTcoTimeout = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_SMI_EN}
      },
      S_ACPI_IO_SMI_EN,
      N_ACPI_IO_SMI_EN_TCO
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        TCO_ADDR_TYPE,
        {R_TCO_IO_TCO1_STS}
      },
      S_TCO_IO_TCO1_STS,
      N_TCO_IO_TCO1_STS_TIMEOUT
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_TCO
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
    SmiHandlerProfileRegisterHandler (&gPchTcoSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

//
// OsTco srcdesc
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mSrcDescOsTco = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_SMI_EN}
      },
      S_ACPI_IO_SMI_EN,
      N_ACPI_IO_SMI_EN_TCO
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        TCO_ADDR_TYPE,
        {R_TCO_IO_TCO1_STS}
      },
      S_TCO_IO_TCO1_STS,
      N_TCO_IO_TCO1_STS_SW_TCO_SMI
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_TCO
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
    SmiHandlerProfileRegisterHandler (&gPchTcoSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

//
// Nmi
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mSrcDescNmi = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        PCR_ADDR_TYPE,
        {PCH_PCR_ADDRESS (PID_ITSS, R_ITSS_PCR_NMI)}
      },
      4,
      N_ITSS_PCR_NMI_NMI2SMI_EN
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        PCR_ADDR_TYPE,
        {PCH_PCR_ADDRESS (PID_ITSS, R_ITSS_PCR_NMI)}
      },
      4,
      N_ITSS_PCR_NMI_NMI2SMI_STS
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_TCO
  }
};

/**
  Enable the Nmi2Smi source
**/
VOID
PchNmi2SmiEnableSource (
  VOID
  )
{
  //
  // The PCR[ITSS].NMI register can only be accessed with BOOT_SAI and SMM_SAI.
  // Since in CFL there is no SMM_SAI it needs PMC assistance to access this register.
  //
  UINT32  ItssNmi;
  ItssNmi = PmcGetNmiControl ();
  PmcSetNmiControl (ItssNmi | B_ITSS_PCR_NMI_NMI2SMI_EN);
}

/**
  Clear the NMI status bit after the SMI handling is done

  @param[in] SrcDesc                    Pointer to the PCH SMI source description table
**/
VOID
EFIAPI
PchNmi2SmiClearSource (
  CONST PCH_SMM_SOURCE_DESC             *SrcDesc
  )
{
  // No need to clear NMI2SMI_STS since it's cleared when NMI source is cleared.
  // Clear TCO status only.
  PchSmmClearSource (&mDescSrcTcoSts);
}

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
             &mSrcDescNmi,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchTcoSmiNmiType,
             DispatchHandle
             );
  if (!EFI_ERROR (Status)) {
    Record = DATABASE_RECORD_FROM_LINK (*DispatchHandle);
    //
    // Since the NMI2SMI status and enable bit are at the same register,
    // it needs separate function to handle the source enable and clear.
    //
    Record->ClearSource = PchNmi2SmiClearSource;
    PchNmi2SmiEnableSource ();
    SmiHandlerProfileRegisterHandler (&gPchTcoSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

//
// IntruderDetect srcdesc
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mSrcDescIntruderDet = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_SMI_EN}
      },
      S_ACPI_IO_SMI_EN,
      N_ACPI_IO_SMI_EN_TCO
    },
    {
      {
        TCO_ADDR_TYPE,
        {R_TCO_IO_TCO2_CNT}
      },
      S_TCO_IO_TCO2_CNT,
      N_TCO_IO_TCO2_CNT_INTRD_SEL
    }
  },
  {
    {
      {
        TCO_ADDR_TYPE,
        {R_TCO_IO_TCO2_STS}
      },
      S_TCO_IO_TCO2_STS,
      N_TCO_IO_TCO2_STS_INTRD_DET
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_TCO
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
    SmiHandlerProfileRegisterHandler (&gPchTcoSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

//
// SpiBiosWp srcdesc
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mSrcDescSpiBiosWp = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_SMI_EN}
      },
      S_ACPI_IO_SMI_EN,
      N_ACPI_IO_SMI_EN_TCO
    },
    {
      {
        PCIE_ADDR_TYPE,
        { (
          (DEFAULT_PCI_BUS_NUMBER_PCH << 24) |
          (PCI_DEVICE_NUMBER_PCH_SPI << 19) |
          (PCI_FUNCTION_NUMBER_PCH_SPI << 16) |
          R_SPI_CFG_BC
        ) }
      },
      S_SPI_CFG_BC,
      N_SPI_CFG_BC_BLE
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
          R_SPI_CFG_BC
        ) }
      },
      S_SPI_CFG_BC,
      N_SPI_CFG_BC_SYNC_SS
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_TCO
  }
};

/**
  Special handling for SPI Write Protect

  @param[in]  SrcDesc   Not used
**/
VOID
EFIAPI
PchTcoSpiWpClearSource (
  CONST PCH_SMM_SOURCE_DESC             *SrcDesc
  )
{
  UINT64 SpiRegBase;
  UINT32 BiosControl;
  UINT32 Timeout;

  SpiRegBase = PCI_SEGMENT_LIB_ADDRESS (
                 DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                 DEFAULT_PCI_BUS_NUMBER_PCH,
                 PCI_DEVICE_NUMBER_PCH_SPI,
                 PCI_FUNCTION_NUMBER_PCH_SPI,
                 0
                 );
  PciSegmentAndThenOr32 (
    SpiRegBase + R_SPI_CFG_BC,
    (UINT32) ~B_SPI_CFG_BC_ASYNC_SS,
    B_SPI_CFG_BC_SYNC_SS
    );
  //
  // Ensure the SYNC is cleared
  //
  Timeout = 1000;
  do {
    BiosControl = PciSegmentRead32 (SpiRegBase + R_SPI_CFG_BC);
    Timeout--;
  } while ((BiosControl & B_SPI_CFG_BC_SYNC_SS) && (Timeout > 0));
  //
  // Any TCO-based status bits require special handling.
  // SMI_STS.TCO_STS must be cleared in addition to the status bit in the TCO registers
  //
  PchSmmClearSource (&mDescSrcTcoSts);
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
             &mSrcDescSpiBiosWp,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchTcoSmiSpiBiosWpType,
             DispatchHandle
             );
  if (!EFI_ERROR (Status)) {
    Record = DATABASE_RECORD_FROM_LINK (*DispatchHandle);
    Record->ClearSource = PchTcoSpiWpClearSource;
    PchTcoSpiWpClearSource (NULL);
    //
    // It doesn't enable the BIOSLOCK here. Enable it by policy in DXE.
    //
    SmiHandlerProfileRegisterHandler (&gPchTcoSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

//
// LpcBiosWp srcdesc
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mSrcDescLpcBiosWp = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_SMI_EN}
      },
      S_ACPI_IO_SMI_EN,
      N_ACPI_IO_SMI_EN_TCO
    },
    {
      {
        PCIE_ADDR_TYPE,
        { (
          (DEFAULT_PCI_BUS_NUMBER_PCH << 24) |
          (PCI_DEVICE_NUMBER_PCH_LPC << 19) |
          (PCI_FUNCTION_NUMBER_PCH_LPC << 16) |
          R_LPC_CFG_BC
        ) }
      },
      S_LPC_CFG_BC,
      N_LPC_CFG_BC_LE
    }
  },
  {
    {
      {
        TCO_ADDR_TYPE,
        {R_TCO_IO_TCO1_STS}
      },
      S_TCO_IO_TCO1_STS,
      N_TCO_IO_TCO1_STS_BIOSWR
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_TCO
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

  if (IsEspiEnabled ()) {
    //
    // Status is D31F0's PCBC.BWPDS
    //
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  Status = PchSmiRecordInsert (
             &mSrcDescLpcBiosWp,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchTcoSmiLpcBiosWpType,
             DispatchHandle
             );
  if (!EFI_ERROR (Status)) {
    Record = DATABASE_RECORD_FROM_LINK (*DispatchHandle);
    Record->ClearSource = PchTcoSmiClearSource;
    PchSmmClearSource (&Record->SrcDesc);
    //
    // It doesn't enable the BIOSLOCK here. Enable it by policy in DXE.
    //
    SmiHandlerProfileRegisterHandler (&gPchTcoSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

//
// NEWCENTURY_STS bit that needs to be cleared
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mSrcDescNewCentury = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_SMI_EN}
      },
      S_ACPI_IO_SMI_EN,
      N_ACPI_IO_SMI_EN_TCO
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        TCO_ADDR_TYPE,
        {R_TCO_IO_TCO1_STS}
      },
      S_TCO_IO_TCO1_STS,
      N_TCO_IO_TCO1_STS_NEWCENTURY
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_TCO
  }
};

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
             &mSrcDescNewCentury,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchTcoSmiNewCenturyType,
             DispatchHandle
             );
  if (!EFI_ERROR (Status)) {
    Record = DATABASE_RECORD_FROM_LINK (*DispatchHandle);
    Record->ClearSource = PchTcoSmiClearSourceAndBlock;
    PchSmmClearSource (&Record->SrcDesc);
    PchSmmEnableSource (&Record->SrcDesc);
    SmiHandlerProfileRegisterHandler (&gPchTcoSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
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
  EFI_STATUS                            Status;

  Record = DATABASE_RECORD_FROM_LINK (DispatchHandle);
  if ((Record->SrcDesc.En[1].Reg.Type == ACPI_ADDR_TYPE) &&
      (Record->SrcDesc.En[1].Reg.Data.pcie.Fields.Dev == PCI_DEVICE_NUMBER_PCH_SPI) &&
      (Record->SrcDesc.En[1].Reg.Data.pcie.Fields.Fnc == PCI_FUNCTION_NUMBER_PCH_SPI) &&
      (Record->SrcDesc.En[1].Reg.Data.pcie.Fields.Reg == R_SPI_CFG_BC) &&
      (Record->SrcDesc.En[1].Bit == N_SPI_CFG_BC_BLE)) {
    //
    // SPI Write Protect cannot be disabled
    //
    return EFI_ACCESS_DENIED;
  } else if ((Record->SrcDesc.En[1].Reg.Type == ACPI_ADDR_TYPE) &&
             (Record->SrcDesc.En[1].Reg.Data.pcie.Fields.Dev == PCI_DEVICE_NUMBER_PCH_LPC) &&
             (Record->SrcDesc.En[1].Reg.Data.pcie.Fields.Fnc == PCI_FUNCTION_NUMBER_PCH_LPC) &&
             (Record->SrcDesc.En[1].Reg.Data.pcie.Fields.Reg == R_LPC_CFG_BC) &&
             (Record->SrcDesc.En[1].Bit == N_LPC_CFG_BC_LE)) {
    //
    // eSPI/LPC Write Protect cannot be disabled
    //
    return EFI_ACCESS_DENIED;
  }

  Status = PchSmmCoreUnRegister (NULL, DispatchHandle);
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileUnregisterHandler (&gPchTcoSmiDispatchProtocolGuid, Record->Callback, NULL, 0);
  }
  return Status;
}


//
// PcieRpHotPlug srcdesc
//
GLOBAL_REMOVE_IF_UNREFERENCED PCH_SMM_SOURCE_DESC PchPcieSmiRpHotPlugTemplate = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        PCIE_ADDR_TYPE,
        {R_PCH_PCIE_CFG_MPC}
      },
      S_PCH_PCIE_CFG_MPC,
      N_PCH_PCIE_CFG_MPC_HPME
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        PCIE_ADDR_TYPE,
        {R_PCH_PCIE_CFG_SMSCS}
      },
      S_PCH_PCIE_CFG_SMSCS,
      N_PCH_PCIE_CFG_SMSCS_HPPDM
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_PCI_EXP
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
             &PchPcieSmiRpHotPlugTemplate,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchPcieSmiRpHotplugType,
             DispatchHandle
             );
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileRegisterHandler (&gPchPcieSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  PchSmmClearSource (&PchPcieSmiRpHotPlugTemplate);
  PchSmmEnableSource (&PchPcieSmiRpHotPlugTemplate);

  return Status;
}

//
// PcieRpLinkActive srcdesc
//
GLOBAL_REMOVE_IF_UNREFERENCED PCH_SMM_SOURCE_DESC PchPcieSmiRpLinkActiveTemplate = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        PCIE_ADDR_TYPE,
        {R_PCH_PCIE_CFG_MPC}
      },
      S_PCH_PCIE_CFG_MPC,
      N_PCH_PCIE_CFG_MPC_HPME
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        PCIE_ADDR_TYPE,
        {R_PCH_PCIE_CFG_SMSCS}
      },
      S_PCH_PCIE_CFG_SMSCS,
      N_PCH_PCIE_CFG_SMSCS_HPLAS
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_PCI_EXP
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
             &PchPcieSmiRpLinkActiveTemplate,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchPcieSmiRpLinkActiveType,
             DispatchHandle
             );
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileRegisterHandler (&gPchPcieSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  PchSmmClearSource (&PchPcieSmiRpLinkActiveTemplate);
  PchSmmEnableSource (&PchPcieSmiRpLinkActiveTemplate);

  return Status;
}

//
// PcieRpLinkEq srcdesc
//
GLOBAL_REMOVE_IF_UNREFERENCED PCH_SMM_SOURCE_DESC PchPcieSmiRpLinkEqTemplate = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        PCIE_ADDR_TYPE,
        {R_PCH_PCIE_CFG_EQCFG1}
      },
      S_PCH_PCIE_CFG_EQCFG1,
      N_PCH_PCIE_CFG_EQCFG1_LERSMIE
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        PCIE_ADDR_TYPE,
        {R_PCH_PCIE_CFG_SMSCS}
      },
      S_PCH_PCIE_CFG_SMSCS,
      N_PCH_PCIE_CFG_SMSCS_LERSMIS
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_PCI_EXP
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
  EFI_STATUS                            Status;

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

  Status = PchSmiRecordInsert (
           &PchPcieSmiRpLinkEqTemplate,
           (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
           PchPcieSmiRpLinkEqType,
           DispatchHandle
           );
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileRegisterHandler (&gPchPcieSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2) DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
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
PchPcieSmiUnRegister (
  IN  PCH_PCIE_SMI_DISPATCH_PROTOCOL    *This,
  IN  EFI_HANDLE                        DispatchHandle
  )
{
  DATABASE_RECORD                       *RecordToDelete;
  EFI_STATUS                            Status;

  RecordToDelete = DATABASE_RECORD_FROM_LINK (DispatchHandle);
  Status = PchSmmCoreUnRegister (NULL, DispatchHandle);
  if (!EFI_ERROR (Status)) {
      SmiHandlerProfileUnregisterHandler (&gPchPcieSmiDispatchProtocolGuid, RecordToDelete->Callback, NULL, 0);
  }
  return Status;
}

//
// Pme srcdesc
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mSrcDescPme = {
  PCH_SMM_SCI_EN_DEPENDENT,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_GPE0_EN_127_96}
      },
      S_ACPI_IO_GPE0_EN_127_96,
      N_ACPI_IO_GPE0_EN_127_96_PME
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_GPE0_STS_127_96}
      },
      S_ACPI_IO_GPE0_STS_127_96,
      N_ACPI_IO_GPE0_STS_127_96_PME
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_GPE0
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
             &mSrcDescPme,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchAcpiSmiPmeType,
             DispatchHandle
             );
  PchSmmClearSource (&mSrcDescPme);
  PchSmmEnableSource (&mSrcDescPme);
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileRegisterHandler (&gPchAcpiSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

//
// PmeB0 srcdesc
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mSrcDescPmeB0 = {
  PCH_SMM_SCI_EN_DEPENDENT,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_GPE0_EN_127_96}
      },
      S_ACPI_IO_GPE0_EN_127_96,
      N_ACPI_IO_GPE0_EN_127_96_PME_B0
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_GPE0_STS_127_96}
      },
      S_ACPI_IO_GPE0_STS_127_96,
      N_ACPI_IO_GPE0_STS_127_96_PME_B0
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_GPE0
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
             &mSrcDescPmeB0,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchAcpiSmiPmeB0Type,
             DispatchHandle
             );
  PchSmmClearSource (&mSrcDescPmeB0);
  PchSmmEnableSource (&mSrcDescPmeB0);
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileRegisterHandler (&gPchAcpiSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

//
// RtcAlarm srcdesc
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mSrcDescRtcAlarm = {
  PCH_SMM_SCI_EN_DEPENDENT,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_PM1_EN}
      },
      S_ACPI_IO_PM1_EN,
      N_ACPI_IO_PM1_EN_RTC
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_PM1_STS}
      },
      S_ACPI_IO_PM1_STS,
      N_ACPI_IO_PM1_STS_RTC
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_PM1_STS_REG
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
             &mSrcDescRtcAlarm,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchAcpiSmiRtcAlarmType,
             DispatchHandle
             );

  PchSmmClearSource (&mSrcDescRtcAlarm);
  PchSmmEnableSource (&mSrcDescRtcAlarm);
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileRegisterHandler (&gPchAcpiSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

//
// TmrOverflow srcdesc
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mSrcDescTmrOverflow = {
  PCH_SMM_SCI_EN_DEPENDENT,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_PM1_EN}
      },
      S_ACPI_IO_PM1_EN,
      N_ACPI_IO_PM1_EN_TMROF
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_PM1_STS}
      },
      S_ACPI_IO_PM1_STS,
      N_ACPI_IO_PM1_STS_TMROF
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_PM1_STS_REG
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
             &mSrcDescTmrOverflow,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchAcpiSmiTmrOverflowType,
             DispatchHandle
             );
  PchSmmClearSource (&mSrcDescTmrOverflow);
  PchSmmEnableSource (&mSrcDescTmrOverflow);
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileRegisterHandler (&gPchAcpiSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
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
PchAcpiSmiUnRegister (
  IN  PCH_ACPI_SMI_DISPATCH_PROTOCOL    *This,
  IN  EFI_HANDLE                        DispatchHandle
  )
{
  DATABASE_RECORD                   *RecordToDelete;
  EFI_STATUS                        Status;

  RecordToDelete = DATABASE_RECORD_FROM_LINK (DispatchHandle);
  Status = PchSmmCoreUnRegister (NULL, DispatchHandle);
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileUnregisterHandler (&gPchAcpiSmiDispatchProtocolGuid, RecordToDelete->Callback, NULL, 0);
  }
  return Status;
}

//
// SerialIrq srcdesc
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mSrcDescSerialIrq = {
  PCH_SMM_NO_FLAGS,
  {
    NULL_BIT_DESC_INITIALIZER,
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_SMI_STS}
      },
      S_ACPI_IO_SMI_STS,
      N_ACPI_IO_SMI_STS_SERIRQ
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_SERIRQ
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
             &mSrcDescSerialIrq,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchSmiSerialIrqType,
             DispatchHandle
             );
  PchSmmClearSource (&mSrcDescSerialIrq);
  PchSmmEnableSource (&mSrcDescSerialIrq);
  if (!EFI_ERROR (Status)) {
     SmiHandlerProfileRegisterHandler (&gPchSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

//
// McSmi srcdesc
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mSrcDescMcSmi = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_SMI_EN}
      },
      S_ACPI_IO_SMI_EN,
      N_ACPI_IO_SMI_EN_MCSMI
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_SMI_STS}
      },
      S_ACPI_IO_SMI_STS,
      N_ACPI_IO_SMI_STS_MCSMI
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_MCSMI
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
             &mSrcDescMcSmi,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchSmiMcSmiType,
             DispatchHandle
             );
  PchSmmClearSource (&mSrcDescMcSmi);
  PchSmmEnableSource (&mSrcDescMcSmi);
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileRegisterHandler (&gPchSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

//
// SmBus srcdesc
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mSrcDescSmbus = {
  PCH_SMM_NO_FLAGS,
  {
    NULL_BIT_DESC_INITIALIZER,
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_SMI_STS}
      },
      S_ACPI_IO_SMI_STS,
      N_ACPI_IO_SMI_STS_SMBUS
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_SMBUS
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
             &mSrcDescSmbus,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchSmiSmBusType,
             DispatchHandle
             );
  PchSmmClearSource (&mSrcDescSmbus);
  PchSmmEnableSource (&mSrcDescSmbus);
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileRegisterHandler (&gPchSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

//
// SpiAsyncSmi srcdesc
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mSrcDescSpiAsyncSmi = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        PCIE_ADDR_TYPE,
        { (
          (DEFAULT_PCI_BUS_NUMBER_PCH << 24) |
          (PCI_DEVICE_NUMBER_PCH_SPI << 19) |
          (PCI_FUNCTION_NUMBER_PCH_SPI << 16) |
          R_SPI_CFG_BC
        ) }
      },
      S_SPI_CFG_BC,
      N_SPI_CFG_BC_ASE_BWP
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
          R_SPI_CFG_BC
        ) }
      },
      S_SPI_CFG_BC,
      N_SPI_CFG_BC_ASYNC_SS
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_SPI
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
  CONST PCH_SMM_SOURCE_DESC             *SrcDesc
  )
{
  UINT64                                SpiRegBase;
  UINT32                                SpiBar0;

  SpiRegBase = PCI_SEGMENT_LIB_ADDRESS (
                 DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                 DEFAULT_PCI_BUS_NUMBER_PCH,
                 PCI_DEVICE_NUMBER_PCH_SPI,
                 PCI_FUNCTION_NUMBER_PCH_SPI,
                 0
                 );
  SpiBar0 = PciSegmentRead32 (SpiRegBase + R_SPI_CFG_BAR0) & ~(B_SPI_CFG_BAR0_MASK);
  if (SpiBar0 != PCH_SPI_BASE_ADDRESS) {
    //
    // Temporary disable MSE, and override with SPI reserved MMIO address, then enable MSE.
    //
    SpiBar0 = PCH_SPI_BASE_ADDRESS;
    PciSegmentAnd8 (SpiRegBase + PCI_COMMAND_OFFSET, (UINT8) ~EFI_PCI_COMMAND_MEMORY_SPACE);
    PciSegmentWrite32 (SpiRegBase + R_SPI_CFG_BAR0, SpiBar0);
    PciSegmentOr8 (SpiRegBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
  }

  MmioOr32 (SpiBar0 + R_SPI_MEM_HSFSC, B_SPI_MEM_HSFSC_FDONE);
}

/**
  Special handling to enable SPI Asynchronous SMI
**/
VOID
PchSmiSpiAsyncEnableSource (
  VOID
  )
{
  UINT64 SpiRegBase;
  SpiRegBase = PCI_SEGMENT_LIB_ADDRESS (
                 DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                 DEFAULT_PCI_BUS_NUMBER_PCH,
                 PCI_DEVICE_NUMBER_PCH_SPI,
                 PCI_FUNCTION_NUMBER_PCH_SPI,
                 0
                 );
  PciSegmentAndThenOr32 (
    SpiRegBase + R_SPI_CFG_BC,
    (UINT32) ~B_SPI_CFG_BC_SYNC_SS,
    B_SPI_CFG_BC_ASE_BWP
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
    SmiHandlerProfileRegisterHandler (&gPchSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
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
  UINT64                                SpiRegBase;
  EFI_STATUS                            Status;

  Record = DATABASE_RECORD_FROM_LINK (DispatchHandle);
  if ((Record->SrcDesc.En[0].Reg.Type == PCIE_ADDR_TYPE) &&
      (Record->SrcDesc.En[0].Reg.Data.pcie.Fields.Dev == PCI_DEVICE_NUMBER_PCH_SPI) &&
      (Record->SrcDesc.En[0].Reg.Data.pcie.Fields.Fnc == PCI_FUNCTION_NUMBER_PCH_SPI) &&
      (Record->SrcDesc.En[0].Reg.Data.pcie.Fields.Reg == R_SPI_CFG_BC) &&
      (Record->SrcDesc.En[0].Bit == N_SPI_CFG_BC_ASE_BWP)) {
    SpiRegBase = PCI_SEGMENT_LIB_ADDRESS (
                   DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                   DEFAULT_PCI_BUS_NUMBER_PCH,
                   PCI_DEVICE_NUMBER_PCH_SPI,
                   PCI_FUNCTION_NUMBER_PCH_SPI,
                   0
                   );
    if (PciSegmentRead8 (SpiRegBase + R_SPI_CFG_BC) & B_SPI_CFG_BC_BILD) {
      //
      // SPI Asynchronous SMI cannot be disabled
      //
      return EFI_ACCESS_DENIED;
    }
  }
  Status = PchSmmCoreUnRegister (NULL, DispatchHandle);
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileUnregisterHandler (&gPchSmiDispatchProtocolGuid, Record->Callback, NULL, 0);
  }
  return Status;
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

GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mSrcDescIoTrap[4] = {
  //
  // PCH I/O Trap register 0 monitor
  //
  {
    PCH_SMM_NO_FLAGS,
    {
      {
        {
          PCR_ADDR_TYPE,
          {PCH_PCR_ADDRESS (PID_PSTH, R_PSTH_PCR_TRPREG0) }
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
          {PCH_PCR_ADDRESS (PID_PSTH, R_PSTH_PCR_TRPST) }
        },
        1,
        0
      }
    },
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_SMI_STS}
      },
      S_ACPI_IO_SMI_STS,
      N_ACPI_IO_SMI_STS_MONITOR
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
          {PCH_PCR_ADDRESS (PID_PSTH, R_PSTH_PCR_TRPREG1) }
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
          {PCH_PCR_ADDRESS (PID_PSTH, R_PSTH_PCR_TRPST) }
        },
        1,
        1
      }
    },
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_SMI_STS}
      },
      S_ACPI_IO_SMI_STS,
      N_ACPI_IO_SMI_STS_MONITOR
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
          {PCH_PCR_ADDRESS (PID_PSTH, R_PSTH_PCR_TRPREG2) }
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
          {PCH_PCR_ADDRESS (PID_PSTH, R_PSTH_PCR_TRPST) }
        },
        1,
        2
      }
    },
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_SMI_STS}
      },
      S_ACPI_IO_SMI_STS,
      N_ACPI_IO_SMI_STS_MONITOR
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
          {PCH_PCR_ADDRESS (PID_PSTH, R_PSTH_PCR_TRPREG3) }
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
          {PCH_PCR_ADDRESS (PID_PSTH, R_PSTH_PCR_TRPST) }
        },
        1,
        3
      }
    },
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_SMI_STS}
      },
      S_ACPI_IO_SMI_STS,
      N_ACPI_IO_SMI_STS_MONITOR
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
             &mSrcDescIoTrap[IoTrapIndex],
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchIoTrapSmiType,
             DispatchHandle
             );
  PchSmmClearSource (&mSrcDescIoTrap[IoTrapIndex]);
  PchSmmEnableSource (&mSrcDescIoTrap[IoTrapIndex]);
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileRegisterHandler (&gEfiSmmIoTrapDispatch2ProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
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
  DATABASE_RECORD                   *RecordToDelete;
  EFI_STATUS                        Status;

  RecordToDelete = DATABASE_RECORD_FROM_LINK (DispatchHandle);
  Status = PchSmmCoreUnRegister (NULL, DispatchHandle);
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileUnregisterHandler (&gEfiSmmIoTrapDispatch2ProtocolGuid, RecordToDelete->Callback, NULL, 0);
  }
  return Status;
}
