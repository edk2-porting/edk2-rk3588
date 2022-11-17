/** @file
  eSPI SMI implementation

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PchSmmEspi.h"
#include <Library/PmcPrivateLib.h>
#include <Library/EspiLib.h>
#include <Library/SmiHandlerProfileLib.h>
#include <Register/PchRegs.h>
#include <Register/PchPcrRegs.h>
#include <Register/PchRegsLpc.h>
#include <Library/PchPciBdfLib.h>
#include <PchBdfAssignment.h>

GLOBAL_REMOVE_IF_UNREFERENCED ESPI_SMI_INSTANCE mEspiSmiInstance = {
  //
  // Signature
  //
  ESPI_SMI_INSTANCE_SIGNATURE,
  //
  // Handle
  //
  NULL,
  //
  // PchEspiSmiDispatchProtocol
  //
  {
    PCH_ESPI_SMI_DISPATCH_REVISION,
    EspiSmiUnRegister,
    BiosWrProtectRegister,
    BiosWrReportRegister,
    PcNonFatalErrRegister,
    PcFatalErrRegister,
    VwNonFatalErrRegister,
    VwFatalErrRegister,
    FlashNonFatalErrRegister,
    FlashFatalErrRegister,
    LnkType1ErrRegister,
    EspiSlaveSmiRegister
  },
  //
  // PchSmiEspiHandle[EspiTopLevelTypeMax]
  //
  {
    NULL, NULL, NULL
  },
  //
  // CallbackDataBase[EspiSmiTypeMax]
  //
  {
    {NULL, NULL}, {NULL, NULL}, {NULL, NULL}, {NULL, NULL}, {NULL, NULL},
    {NULL, NULL}, {NULL, NULL}, {NULL, NULL}, {NULL, NULL}, {NULL, NULL}
  },
  //
  // EspiSmiEventCounter[EspiSmiTypeMax]
  //
  {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  },
  //
  // Barrier[EspiTopLevelTypeMax]
  //
  {
    {
      BiosWrProtect,
      BiosWrProtect
    },
    {
      BiosWrReport,
      LnkType1Err
    },
    {
      EspiSlaveSmi,
      EspiSlaveSmi
    }
  }
};

GLOBAL_REMOVE_IF_UNREFERENCED CONST ESPI_DESCRIPTOR mEspiDescriptor[EspiSmiTypeMax] = {
  //
  // BiosWrProtect
  //
  {
    {
      PCIE_ADDR_TYPE,
      { (
        (DEFAULT_PCI_BUS_NUMBER_PCH << 24) |
        (PCI_DEVICE_NUMBER_PCH_LPC << 19) |
        (PCI_FUNCTION_NUMBER_PCH_LPC << 16) |
        R_ESPI_CFG_PCBC
      ) }
    },
    //
    // SourceIsActiveAndMask and SourceIsActiveValue
    //
    B_ESPI_CFG_PCBC_BWPDS | B_ESPI_CFG_PCBC_LE,
    B_ESPI_CFG_PCBC_BWPDS | B_ESPI_CFG_PCBC_LE,
    //
    // ClearStatusAndMask and ClearStatusOrMask
    //
    (UINT32) ~B_ESPI_CFG_PCBC_BWRS,
    B_ESPI_CFG_PCBC_BWPDS
  },
  //
  // BiosWrReport
  //
  {
    {
      PCIE_ADDR_TYPE,
      { (
        (DEFAULT_PCI_BUS_NUMBER_PCH << 24) |
        (PCI_DEVICE_NUMBER_PCH_LPC << 19) |
        (PCI_FUNCTION_NUMBER_PCH_LPC << 16) |
        R_ESPI_CFG_PCBC
      ) }
    },
    B_ESPI_CFG_PCBC_BWRS | B_ESPI_CFG_PCBC_BWRE,
    B_ESPI_CFG_PCBC_BWRS | B_ESPI_CFG_PCBC_BWRE,
    (UINT32) ~B_ESPI_CFG_PCBC_BWPDS,
    B_ESPI_CFG_PCBC_BWRS
  },
  //
  // PcNonFatalErr
  //
  {
    {
      PCR_ADDR_TYPE,
      {PCH_PCR_ADDRESS (PID_ESPISPI, R_ESPI_PCR_PCERR_SLV0) }
    },
    (B_ESPI_PCR_XERR_XNFES | B_ESPI_PCR_XERR_XNFEE),
    (B_ESPI_PCR_XERR_XNFES | (V_ESPI_PCR_XERR_XNFEE_SMI << N_ESPI_PCR_XERR_XNFEE)),
    (UINT32) ~(B_ESPI_PCR_PCERR_SLV0_PCURD | B_ESPI_PCR_XERR_XFES),
    B_ESPI_PCR_XERR_XNFES
  },
  //
  // PcFatalErr
  //
  {
    {
      PCR_ADDR_TYPE,
      {PCH_PCR_ADDRESS (PID_ESPISPI, R_ESPI_PCR_PCERR_SLV0) }
    },
    (B_ESPI_PCR_XERR_XFES | B_ESPI_PCR_XERR_XFEE),
    (B_ESPI_PCR_XERR_XFES | (V_ESPI_PCR_XERR_XFEE_SMI << N_ESPI_PCR_XERR_XFEE)),
    (UINT32) ~(B_ESPI_PCR_PCERR_SLV0_PCURD | B_ESPI_PCR_XERR_XNFES),
    B_ESPI_PCR_XERR_XFES
  },
  //
  // VwNonFatalErr
  //
  {
    {
      PCR_ADDR_TYPE,
      {PCH_PCR_ADDRESS (PID_ESPISPI, R_ESPI_PCR_VWERR_SLV0) }
    },
    (B_ESPI_PCR_XERR_XNFES | B_ESPI_PCR_XERR_XNFEE),
    (B_ESPI_PCR_XERR_XNFES | (V_ESPI_PCR_XERR_XNFEE_SMI << N_ESPI_PCR_XERR_XNFEE)),
    (UINT32) ~B_ESPI_PCR_XERR_XFES,
    B_ESPI_PCR_XERR_XNFES
  },
  //
  // VwFatalErr
  //
  {
    {
      PCR_ADDR_TYPE,
      {PCH_PCR_ADDRESS (PID_ESPISPI, R_ESPI_PCR_VWERR_SLV0) }
    },
    (B_ESPI_PCR_XERR_XFES | B_ESPI_PCR_XERR_XFEE),
    (B_ESPI_PCR_XERR_XFES | (V_ESPI_PCR_XERR_XFEE_SMI << N_ESPI_PCR_XERR_XFEE)),
    (UINT32) ~B_ESPI_PCR_XERR_XNFES,
    B_ESPI_PCR_XERR_XFES
  },
  //
  // FlashNonFatalErr
  //
  {
    {
      PCR_ADDR_TYPE,
      {PCH_PCR_ADDRESS (PID_ESPISPI, R_ESPI_PCR_FCERR_SLV0) }
    },
    (B_ESPI_PCR_XERR_XNFES | B_ESPI_PCR_XERR_XNFEE),
    (B_ESPI_PCR_XERR_XNFES | (V_ESPI_PCR_XERR_XNFEE_SMI << N_ESPI_PCR_XERR_XNFEE)),
    (UINT32) ~B_ESPI_PCR_XERR_XFES,
    B_ESPI_PCR_XERR_XNFES
  },
  //
  // FlashFatalErr
  //
  {
    {
      PCR_ADDR_TYPE,
      {PCH_PCR_ADDRESS (PID_ESPISPI, R_ESPI_PCR_FCERR_SLV0) }
    },
    (B_ESPI_PCR_XERR_XFES | B_ESPI_PCR_XERR_XFEE),
    (B_ESPI_PCR_XERR_XFES | (V_ESPI_PCR_XERR_XFEE_SMI << N_ESPI_PCR_XERR_XFEE)),
    (UINT32) ~B_ESPI_PCR_XERR_XNFES,
    B_ESPI_PCR_XERR_XFES
  },
  //
  // LnkType1Err
  //
  {
    {
      PCR_ADDR_TYPE,
      {PCH_PCR_ADDRESS (PID_ESPISPI, R_ESPI_PCR_LNKERR_SLV0) }
    },
    B_ESPI_PCR_LNKERR_SLV0_LFET1S | B_ESPI_PCR_LNKERR_SLV0_LFET1E,
    B_ESPI_PCR_LNKERR_SLV0_LFET1S | (V_ESPI_PCR_LNKERR_SLV0_LFET1E_SMI << N_ESPI_PCR_LNKERR_SLV0_LFET1E),
    (UINT32) ~B_ESPI_PCR_LNKERR_SLV0_SLCRR,
    B_ESPI_PCR_LNKERR_SLV0_LFET1S
  },
};

/**
  Enable eSPI SMI source

  @param[in]  EspiSmiType  Type based on ESPI_SMI_TYPE
**/
STATIC
VOID
EspiSmiEnableSource (
  IN CONST  ESPI_SMI_TYPE EspiSmiType
  )
{
  UINT64      PciBaseAddress;

  switch (EspiSmiType) {
    case BiosWrProtect:
      //
      // It doesn't enable the BIOSLOCK here. Enable it by policy in DXE.
      //
      break;
    case BiosWrReport:
      PciBaseAddress = EspiPciCfgBase ();
      PciSegmentAndThenOr32 (
        PciBaseAddress + R_ESPI_CFG_PCBC,
        (UINT32) ~(B_ESPI_CFG_PCBC_BWRS | B_ESPI_CFG_PCBC_BWPDS),
        B_ESPI_CFG_PCBC_BWRE
        );
      break;
    case PcNonFatalErr:
      PchPcrAndThenOr32 (
        PID_ESPISPI,
        (UINT16) R_ESPI_PCR_PCERR_SLV0,
        (UINT32) ~(B_ESPI_PCR_PCERR_SLV0_PCURD | B_ESPI_PCR_XERR_XNFES | B_ESPI_PCR_XERR_XFES),
        B_ESPI_PCR_XERR_XNFEE
        );
      break;

    case PcFatalErr:
      PchPcrAndThenOr32 (
        PID_ESPISPI,
        (UINT16) R_ESPI_PCR_PCERR_SLV0,
        (UINT32) ~(B_ESPI_PCR_PCERR_SLV0_PCURD | B_ESPI_PCR_XERR_XNFES | B_ESPI_PCR_XERR_XFES),
        B_ESPI_PCR_XERR_XFEE
        );
      break;

    case VwNonFatalErr:
      PchPcrAndThenOr32 (
        PID_ESPISPI,
        (UINT16) R_ESPI_PCR_VWERR_SLV0,
        (UINT32) ~(B_ESPI_PCR_XERR_XNFES | B_ESPI_PCR_XERR_XFES),
        B_ESPI_PCR_XERR_XNFEE
        );
      break;

    case VwFatalErr:
      PchPcrAndThenOr32 (
        PID_ESPISPI,
        (UINT16) R_ESPI_PCR_VWERR_SLV0,
        (UINT32) ~(B_ESPI_PCR_XERR_XNFES | B_ESPI_PCR_XERR_XFES),
        B_ESPI_PCR_XERR_XFEE
        );
      break;

    case FlashNonFatalErr:
      PchPcrAndThenOr32 (
        PID_ESPISPI,
        (UINT16) R_ESPI_PCR_FCERR_SLV0,
        (UINT32) ~(B_ESPI_PCR_XERR_XNFES | B_ESPI_PCR_XERR_XFES),
        B_ESPI_PCR_XERR_XNFEE
        );
      break;

    case FlashFatalErr:
      PchPcrAndThenOr32 (
        PID_ESPISPI,
        (UINT16) R_ESPI_PCR_FCERR_SLV0,
        (UINT32) ~(B_ESPI_PCR_XERR_XNFES | B_ESPI_PCR_XERR_XFES),
        B_ESPI_PCR_XERR_XFEE
        );
      break;

    case LnkType1Err:
      PchPcrAndThenOr32 (
        PID_ESPISPI,
        (UINT16) R_ESPI_PCR_LNKERR_SLV0,
        (UINT32) ~(B_ESPI_PCR_LNKERR_SLV0_SLCRR | B_ESPI_PCR_LNKERR_SLV0_LFET1S),
        (UINT32) (V_ESPI_PCR_LNKERR_SLV0_LFET1E_SMI << N_ESPI_PCR_LNKERR_SLV0_LFET1E)
        );

      if (IsEspiSecondSlaveSupported ()) {
        PchPcrAndThenOr32 (
          PID_ESPISPI,
          (UINT16) R_ESPI_PCR_LNKERR_SLV1,
          (UINT32) ~(B_ESPI_PCR_LNKERR_SLV0_SLCRR | B_ESPI_PCR_LNKERR_SLV0_LFET1S),
          (UINT32) (V_ESPI_PCR_LNKERR_SLV0_LFET1E_SMI << N_ESPI_PCR_LNKERR_SLV0_LFET1E)
          );
      }
      break;

    default:
      DEBUG ((DEBUG_ERROR, "Unsupported EspiSmiType \n"));
      ASSERT (FALSE);
      break;
  }
}


/**
  Disable eSPI SMI source

  @param[in]  EspiSmiType  Type based on ESPI_SMI_TYPE
**/
STATIC
VOID
EspiSmiDisableSource (
  IN CONST  ESPI_SMI_TYPE EspiSmiType
  )
{

  switch (EspiSmiType) {
    case BiosWrProtect:
    case BiosWrReport:
      DEBUG ((DEBUG_ERROR, "Bit is write lock, thus BWRE/BWPDS source cannot be disabled \n"));
      ASSERT (FALSE);
      break;
    case PcNonFatalErr:
      PchPcrAndThenOr32 (
        PID_ESPISPI,
        (UINT16) R_ESPI_PCR_PCERR_SLV0,
        (UINT32) ~(B_ESPI_PCR_PCERR_SLV0_PCURD | B_ESPI_PCR_XERR_XNFES | B_ESPI_PCR_XERR_XFES | B_ESPI_PCR_XERR_XNFEE),
        0
        );
      break;

    case PcFatalErr:
      PchPcrAndThenOr32 (
        PID_ESPISPI,
        (UINT16) R_ESPI_PCR_PCERR_SLV0,
        (UINT32) ~(B_ESPI_PCR_PCERR_SLV0_PCURD | B_ESPI_PCR_XERR_XNFES | B_ESPI_PCR_XERR_XFES | B_ESPI_PCR_XERR_XFEE),
        0
        );
      break;

    case VwNonFatalErr:
      PchPcrAndThenOr32 (
        PID_ESPISPI,
        (UINT16) R_ESPI_PCR_VWERR_SLV0,
        (UINT32) ~(B_ESPI_PCR_XERR_XNFES | B_ESPI_PCR_XERR_XFES | B_ESPI_PCR_XERR_XNFEE),
        0
        );
      break;

    case VwFatalErr:
      PchPcrAndThenOr32 (
        PID_ESPISPI,
        (UINT16) R_ESPI_PCR_VWERR_SLV0,
        (UINT32) ~(B_ESPI_PCR_XERR_XNFES | B_ESPI_PCR_XERR_XFES | B_ESPI_PCR_XERR_XFEE),
        0
        );
      break;

    case FlashNonFatalErr:
      PchPcrAndThenOr32 (
        PID_ESPISPI,
        (UINT16) R_ESPI_PCR_FCERR_SLV0,
        (UINT32) ~(B_ESPI_PCR_XERR_XNFES | B_ESPI_PCR_XERR_XFES | B_ESPI_PCR_XERR_XNFEE),
        0
        );
      break;

    case FlashFatalErr:
      PchPcrAndThenOr32 (
        PID_ESPISPI,
       (UINT16) R_ESPI_PCR_FCERR_SLV0,
       (UINT32) ~(B_ESPI_PCR_XERR_XNFES | B_ESPI_PCR_XERR_XFES | B_ESPI_PCR_XERR_XFEE),
       0
       );
      break;

    case LnkType1Err:
      PchPcrAndThenOr32 (
        PID_ESPISPI,
        (UINT16) R_ESPI_PCR_LNKERR_SLV0,
        (UINT32) ~(B_ESPI_PCR_LNKERR_SLV0_SLCRR | B_ESPI_PCR_LNKERR_SLV0_LFET1S),
        0
        );

      if (IsEspiSecondSlaveSupported ()) {
        PchPcrAndThenOr32 (
          PID_ESPISPI,
          (UINT16) R_ESPI_PCR_LNKERR_SLV1,
          (UINT32) ~(B_ESPI_PCR_LNKERR_SLV0_SLCRR | B_ESPI_PCR_LNKERR_SLV0_LFET1S),
          0
          );
      }
      break;

    default:
      DEBUG ((DEBUG_ERROR, "Unsupported EspiSmiType \n"));
      ASSERT (FALSE);
      break;
  }
}

/**
  Clear a status for the SMI event

  @param[in]  EspiSmiType  Type based on ESPI_SMI_TYPE
**/
STATIC
VOID
EspiSmiClearStatus (
  IN CONST  ESPI_SMI_TYPE EspiSmiType
  )
{
  UINT32                  PciBus;
  UINT32                  PciDev;
  UINT32                  PciFun;
  UINT32                  PciReg;
  UINT64                  PciBaseAddress;
  CONST ESPI_DESCRIPTOR   *Desc;

  Desc = &mEspiDescriptor[EspiSmiType];

  switch (Desc->Address.Type) {
    case PCIE_ADDR_TYPE:
      PciBus  = Desc->Address.Data.pcie.Fields.Bus;
      PciDev  = Desc->Address.Data.pcie.Fields.Dev;
      PciFun  = Desc->Address.Data.pcie.Fields.Fnc;
      PciReg  = Desc->Address.Data.pcie.Fields.Reg;
      PciBaseAddress = PCI_SEGMENT_LIB_ADDRESS (DEFAULT_PCI_SEGMENT_NUMBER_PCH, PciBus, PciDev, PciFun, 0);
      PciSegmentAndThenOr32 (PciBaseAddress + PciReg, Desc->ClearStatusAndMask, Desc->ClearStatusOrMask);
      break;
    case PCR_ADDR_TYPE:
      PchPcrAndThenOr32 (
        Desc->Address.Data.Pcr.Fields.Pid,
        Desc->Address.Data.Pcr.Fields.Offset,
        Desc->ClearStatusAndMask,
        Desc->ClearStatusOrMask
        );
      break;
    default:
      DEBUG ((DEBUG_ERROR, "Address type for eSPI SMI is invalid \n"));
      ASSERT (FALSE);
      break;
  }
}

/**
  Checks if a source is active by looking at the enable and status bits

  @param[in]  EspiSmiType  Type based on ESPI_SMI_TYPE
**/
STATIC
BOOLEAN
EspiSmiSourceIsActive (
  IN CONST  ESPI_SMI_TYPE EspiSmiType
  )
{
  BOOLEAN                 Active;
  UINT32                  PciBus;
  UINT32                  PciDev;
  UINT32                  PciFun;
  UINT32                  PciReg;
  UINT64                  PciBaseAddress;
  UINT32                  Data32;
  CONST ESPI_DESCRIPTOR   *Desc;

  Desc = &mEspiDescriptor[EspiSmiType];

  Active = FALSE;
  switch (Desc->Address.Type) {
    case PCIE_ADDR_TYPE:
      PciBus  = Desc->Address.Data.pcie.Fields.Bus;
      PciDev  = Desc->Address.Data.pcie.Fields.Dev;
      PciFun  = Desc->Address.Data.pcie.Fields.Fnc;
      PciReg  = Desc->Address.Data.pcie.Fields.Reg;
      PciBaseAddress = PCI_SEGMENT_LIB_ADDRESS (DEFAULT_PCI_SEGMENT_NUMBER_PCH, PciBus, PciDev, PciFun, 0);
      Data32 = PciSegmentRead32 (PciBaseAddress + PciReg);
      break;

    case PCR_ADDR_TYPE:
      Data32 = PchPcrRead32 (
                 Desc->Address.Data.Pcr.Fields.Pid,
                 Desc->Address.Data.Pcr.Fields.Offset
                 );
      break;

    default:
      Data32 = 0;
      DEBUG ((DEBUG_ERROR, "Address type for eSPI SMI is invalid \n"));
      ASSERT (FALSE);
      break;
  }

  if ((Data32 & Desc->SourceIsActiveAndMask) == Desc->SourceIsActiveValue) {
    Active = TRUE;
  }

  return Active;
}

/**
  Insert a handler into the corresponding linked list based on EspiSmiType

  @param[in]  DispatchFunction      The callback to execute
  @param[in]  EspiSmiType           Type based on ESPI_SMI_TYPE to determine which linked list to use
  @param[out] DispatchHandle        The link to the record in the database

  @retval     EFI_SUCCESS           Record was successfully inserted into master database
  @retval     EFI_OUT_OF_RESOURCES  Cannot allocate pool to insert record
**/
STATIC
EFI_STATUS
InsertEspiRecord (
  IN        PCH_ESPI_SMI_DISPATCH_CALLBACK  DispatchFunction,
  IN        ESPI_SMI_TYPE                   EspiSmiType,
  OUT       EFI_HANDLE                      *DispatchHandle
  )
{
  EFI_STATUS      Status;
  ESPI_SMI_RECORD *Record;

  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (ESPI_SMI_RECORD), (VOID **) &Record);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }
  SetMem (Record, sizeof (ESPI_SMI_RECORD), 0);

  Record->Callback  = DispatchFunction;
  Record->Signature = ESPI_SMI_RECORD_SIGNATURE;

  InsertTailList (&mEspiSmiInstance.CallbackDataBase[EspiSmiType], &Record->Link);
  EspiSmiClearStatus (EspiSmiType);
  EspiSmiEnableSource (EspiSmiType);

  ++mEspiSmiInstance.EspiSmiEventCounter[EspiSmiType];

  *DispatchHandle = (EFI_HANDLE) (&Record->Link);

  return EFI_SUCCESS;
}

/**
  This callback is registered to PchSmiDispatch

  @param[in]  DispatchHandle  Used to determine which source have been triggered
**/
VOID
EspiSmiCallback (
  IN  EFI_HANDLE                      DispatchHandle
  )
{
  DATABASE_RECORD     *PchSmiRecord;
  ESPI_TOP_LEVEL_TYPE EspiTopLevelType;
  ESPI_SMI_TYPE       EspiSmiType;
  ESPI_SMI_RECORD     *RecordInDb;
  LIST_ENTRY          *LinkInDb;

  PchSmiRecord = DATABASE_RECORD_FROM_LINK (DispatchHandle);

  if (PchSmiRecord->PchSmiType == PchTcoSmiLpcBiosWpType) {
    EspiTopLevelType = EspiBiosWrProtect;
  } else if (PchSmiRecord->PchSmiType == PchSmiSerialIrqType) {
    EspiTopLevelType = EspiSerialIrq;
  } else {
    DEBUG ((DEBUG_ERROR, "EspiSmiCallback was dispatched with a wrong DispatchHandle"));
    ASSERT (FALSE);
    return;
  }

  for (EspiSmiType = mEspiSmiInstance.Barrier[EspiTopLevelType].Start; EspiSmiType <= mEspiSmiInstance.Barrier[EspiTopLevelType].End; ++EspiSmiType) {
    if (!EspiSmiSourceIsActive (EspiSmiType)) {
      continue;
    }
    //
    // The source is active, so walk the callback database and dispatch
    //
    if (!IsListEmpty (&mEspiSmiInstance.CallbackDataBase[EspiSmiType])) {
      //
      // We have children registered w/ us -- continue
      //
      LinkInDb = GetFirstNode (&mEspiSmiInstance.CallbackDataBase[EspiSmiType]);

      while (!IsNull (&mEspiSmiInstance.CallbackDataBase[EspiSmiType], LinkInDb)) {
        RecordInDb = ESPI_RECORD_FROM_LINK (LinkInDb);

        //
        // RecordInDb->Link might be removed (unregistered) by Callback function, and then the
        // system will hang in ASSERT() while calling GetNextNode().
        // To prevent the issue, we need to get next record in DB here (before Callback function).
        //
        LinkInDb = GetNextNode (&mEspiSmiInstance.CallbackDataBase[EspiSmiType], &RecordInDb->Link);

        //
        // Callback
        //
        if (RecordInDb->Callback != NULL) {
          RecordInDb->Callback ((EFI_HANDLE) &RecordInDb->Link);
        } else {
          ASSERT (FALSE);
        }
      }
    } else if (EspiSmiType == LnkType1Err) {
      //
      // If no proper handler registered for Link Type 1 Error
      // Call default SMI handler recover otherwise
      //
      EspiDefaultFatalErrorHandler ();
    }

    //
    // Finish walking the linked list for the EspiSmiType, so clear status
    //
    EspiSmiClearStatus (EspiSmiType);
  }
}

//
// EspiBiosWp srcdesc
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mSrcDescEspiBiosWp = {
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
          R_ESPI_CFG_PCBC
        ) }
      },
      S_ESPI_CFG_PCBC,
      N_ESPI_CFG_PCBC_LE
    }
  },
  {
    {
      {
        PCIE_ADDR_TYPE,
        { (
          (DEFAULT_PCI_BUS_NUMBER_PCH << 24) |
          (PCI_DEVICE_NUMBER_PCH_LPC << 19) |
          (PCI_FUNCTION_NUMBER_PCH_LPC << 16) |
          R_ESPI_CFG_PCBC
        ) }
      },
      S_ESPI_CFG_PCBC,
      N_ESPI_CFG_PCBC_BWPDS
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
  This function will register EspiSmiCallback with mSrcDescEspiBiosWp source decriptor
  This function make sure there is only one BIOS WP SMI handler is registered.
  While any ESPI sub BIOS WP SMI type is registered, all the BIOS WP SMI
  will go to callback function EspiSmiCallback first, and then dispatchs the callbacks
  recorded in mEspiSmiInstance.

  @retval EFI_SUCCESS Registration succeed
  @retval others      Registration failed
**/
STATIC
EFI_STATUS
RegisterBiosWrProtectIfNull (
  VOID
  )
{
  EFI_STATUS      Status;
  DATABASE_RECORD *Record;

  if (mEspiSmiInstance.PchSmiEspiHandle[EspiBiosWrProtect] == NULL) {
    Status = PchSmiRecordInsert (
               &mSrcDescEspiBiosWp,
               (PCH_SMI_CALLBACK_FUNCTIONS) EspiSmiCallback,
               PchTcoSmiLpcBiosWpType,
               &mEspiSmiInstance.PchSmiEspiHandle[EspiBiosWrProtect]
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Fail to register BIOS WP SMI handler \n"));
      return Status;
    }
    Record = DATABASE_RECORD_FROM_LINK (mEspiSmiInstance.PchSmiEspiHandle[EspiBiosWrProtect]);
    Record->ClearSource = PchTcoSmiClearSource;
  }

  return EFI_SUCCESS;
}

/**
  This function will register EspiSmiCallback with mSrcDescSerialIrq source decriptor
  This function make sure there is only one Serial IRQ SMI handler is registered.
  While any ESPI sub Serial IRQ SMI type is registered, all the Serial IRQ SMI
  will go to callback function EspiSmiCallback first, and then dispatchs the callbacks
  recorded in mEspiSmiInstance.

  @retval EFI_SUCCESS Registration succeed
  @retval others      Registration failed
**/
STATIC
EFI_STATUS
RegisterSerialIrqIfNull (
  VOID
  )
{
  EFI_STATUS  Status;

  if (mEspiSmiInstance.PchSmiEspiHandle[EspiSerialIrq] == NULL) {
    Status = PchSmiRecordInsert (
               &mSrcDescSerialIrq,
               (PCH_SMI_CALLBACK_FUNCTIONS) EspiSmiCallback,
               PchSmiSerialIrqType,
               &mEspiSmiInstance.PchSmiEspiHandle[EspiSerialIrq]
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Fail to register Serial IRQ SMI handler \n"));
      return Status;
    }
  }

  return EFI_SUCCESS;
}

/**
  Installs and initialize this protocol

  @param[in]  ImageHandle   Not used

  @retval     EFI_SUCCESS   Installation succeed
  @retval     others        Installation failed
**/
EFI_STATUS
EFIAPI
InstallEspiSmi (
  IN EFI_HANDLE           ImageHandle
  )
{
  EFI_STATUS    Status;
  ESPI_SMI_TYPE EspiSmiType;

  DEBUG ((DEBUG_INFO, "[InstallEspiSmi] Enter\n"));

  //
  // InitializeListHead for mEspiSmiInstance.CallBackDataBase[EspiTopLevelTypeMax]
  //
  for (EspiSmiType = 0; EspiSmiType < EspiSmiTypeMax; ++EspiSmiType) {
    InitializeListHead (&mEspiSmiInstance.CallbackDataBase[EspiSmiType]);
  }

  //
  // Install EfiEspiSmiDispatchProtocol
  //
  Status = gSmst->SmmInstallProtocolInterface (
                    &mEspiSmiInstance.Handle,
                    &gPchEspiSmiDispatchProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mEspiSmiInstance.PchEspiSmiDispatchProtocol
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to install eSPI SMI Dispatch Protocol\n"));
    ASSERT (FALSE);
    return Status;
  }

  // Register eSPI SMM callback to enable Fatal Error handling by default handler
  Status = RegisterSerialIrqIfNull ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Enable LnkType1Err SMI generation for default handler
  EspiSmiClearStatus (LnkType1Err);
  EspiSmiEnableSource (LnkType1Err);

  return EFI_SUCCESS;
}

/**
  eSPI SMI Dispatch Protocol instance to register a BIOS Write Protect event

  @param[in]  This              Not used
  @param[in]  DispatchFunction  The callback to execute
  @param[out] DispatchHandle    The handle for this callback registration

  @retval     EFI_SUCCESS       Registration succeed
  @retval     EFI_ACCESS_DENIED Return access denied if the SmmReadyToLock event has been triggered
  @retval     others            Registration failed
**/
EFI_STATUS
EFIAPI
BiosWrProtectRegister (
  IN  PCH_ESPI_SMI_DISPATCH_PROTOCOL  *This,
  IN  PCH_ESPI_SMI_DISPATCH_CALLBACK  DispatchFunction,
  OUT EFI_HANDLE                      *DispatchHandle
  )
{
  EFI_STATUS Status;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = RegisterBiosWrProtectIfNull ();
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Insert a record
  //
  Status = InsertEspiRecord (DispatchFunction, BiosWrProtect, DispatchHandle);
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileRegisterHandler (&gPchEspiSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

/**
  eSPI SMI Dispatch Protocol instance to register a BIOS Write Report event

  @param[in]  This              Not used
  @param[in]  DispatchFunction  The callback to execute
  @param[out] DispatchHandle    The handle for this callback registration

  @retval     EFI_SUCCESS       Registration succeed
  @retval     EFI_ACCESS_DENIED Return access denied if the SmmReadyToLock event has been triggered
  @retval     others            Registration failed
**/
EFI_STATUS
EFIAPI
BiosWrReportRegister (
  IN  PCH_ESPI_SMI_DISPATCH_PROTOCOL  *This,
  IN  PCH_ESPI_SMI_DISPATCH_CALLBACK  DispatchFunction,
  OUT EFI_HANDLE                      *DispatchHandle
  )
{
  EFI_STATUS Status;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = RegisterSerialIrqIfNull ();
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Insert a record
  //
  Status = InsertEspiRecord (DispatchFunction, BiosWrReport, DispatchHandle);
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileRegisterHandler (&gPchEspiSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

/**
  eSPI SMI Dispatch Protocol instance to register a Peripheral Channel Non Fatal Error event

  @param[in]  This              Not used
  @param[in]  DispatchFunction  The callback to execute
  @param[out] DispatchHandle    The handle for this callback registration

  @retval     EFI_SUCCESS       Registration succeed
  @retval     EFI_ACCESS_DENIED Return access denied if the SmmReadyToLock event has been triggered
  @retval     others            Registration failed
**/
EFI_STATUS
EFIAPI
PcNonFatalErrRegister (
  IN  PCH_ESPI_SMI_DISPATCH_PROTOCOL  *This,
  IN  PCH_ESPI_SMI_DISPATCH_CALLBACK  DispatchFunction,
  OUT EFI_HANDLE                      *DispatchHandle
  )
{
  EFI_STATUS Status;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = RegisterSerialIrqIfNull ();
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Insert a record
  //
  Status = InsertEspiRecord (DispatchFunction, PcNonFatalErr, DispatchHandle);
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileRegisterHandler (&gPchEspiSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

/**
  eSPI SMI Dispatch Protocol instance to register a Peripheral Channel Fatal Error event

  @param[in]  This              Not used
  @param[in]  DispatchFunction  The callback to execute
  @param[out] DispatchHandle    The handle for this callback registration

  @retval     EFI_SUCCESS       Registration succeed
  @retval     EFI_ACCESS_DENIED Return access denied if the SmmReadyToLock event has been triggered
  @retval     others            Registration failed
**/
EFI_STATUS
EFIAPI
PcFatalErrRegister (
  IN  PCH_ESPI_SMI_DISPATCH_PROTOCOL  *This,
  IN  PCH_ESPI_SMI_DISPATCH_CALLBACK  DispatchFunction,
  OUT EFI_HANDLE                      *DispatchHandle
  )
{
  EFI_STATUS Status;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = RegisterSerialIrqIfNull ();
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Insert a record
  //
  Status = InsertEspiRecord (DispatchFunction, PcFatalErr, DispatchHandle);
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileRegisterHandler (&gPchEspiSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

/**
  eSPI SMI Dispatch Protocol instance to register a Virtual Wire Non Fatal Error event

  @param[in]  This              Not used
  @param[in]  DispatchFunction  The callback to execute
  @param[out] DispatchHandle    The handle for this callback registration

  @retval     EFI_SUCCESS       Registration succeed
  @retval     EFI_ACCESS_DENIED Return access denied if the SmmReadyToLock event has been triggered
  @retval     others            Registration failed
**/
EFI_STATUS
EFIAPI
VwNonFatalErrRegister (
  IN  PCH_ESPI_SMI_DISPATCH_PROTOCOL  *This,
  IN  PCH_ESPI_SMI_DISPATCH_CALLBACK  DispatchFunction,
  OUT EFI_HANDLE                      *DispatchHandle
  )
{
  EFI_STATUS  Status;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = RegisterSerialIrqIfNull ();
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Insert a record
  //
  Status = InsertEspiRecord (DispatchFunction, VwNonFatalErr, DispatchHandle);
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileRegisterHandler (&gPchEspiSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

/**
  eSPI SMI Dispatch Protocol instance to register a Virtual Wire Fatal Error event

  @param[in]  This              Not used
  @param[in]  DispatchFunction  The callback to execute
  @param[out] DispatchHandle    The handle for this callback registration

  @retval     EFI_SUCCESS       Registration succeed
  @retval     EFI_ACCESS_DENIED Return access denied if the SmmReadyToLock event has been triggered
  @retval     others            Registration failed
**/
EFI_STATUS
EFIAPI
VwFatalErrRegister (
  IN  PCH_ESPI_SMI_DISPATCH_PROTOCOL  *This,
  IN  PCH_ESPI_SMI_DISPATCH_CALLBACK  DispatchFunction,
  OUT EFI_HANDLE                      *DispatchHandle
  )
{
  EFI_STATUS  Status;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = RegisterSerialIrqIfNull ();
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Insert a record
  //
  Status = InsertEspiRecord (DispatchFunction, VwFatalErr, DispatchHandle);
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileRegisterHandler (&gPchEspiSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

/**
  eSPI SMI Dispatch Protocol instance to register a Flash Channel Non Fatal Error event

  @param[in]  This              Not used
  @param[in]  DispatchFunction  The callback to execute
  @param[out] DispatchHandle    The handle for this callback registration

  @retval     EFI_SUCCESS       Registration succeed
  @retval     EFI_ACCESS_DENIED Return access denied if the SmmReadyToLock event has been triggered
  @retval     others            Registration failed
**/
EFI_STATUS
EFIAPI
FlashNonFatalErrRegister (
  IN  PCH_ESPI_SMI_DISPATCH_PROTOCOL  *This,
  IN  PCH_ESPI_SMI_DISPATCH_CALLBACK  DispatchFunction,
  OUT EFI_HANDLE                      *DispatchHandle
  )
{
  EFI_STATUS Status;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = RegisterSerialIrqIfNull ();
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Insert a record
  //
  Status = InsertEspiRecord (DispatchFunction, FlashNonFatalErr, DispatchHandle);
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileRegisterHandler (&gPchEspiSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

/**
  eSPI SMI Dispatch Protocol instance to register a Flash Channel Fatal Error event

  @param[in]  This              Not used
  @param[in]  DispatchFunction  The callback to execute
  @param[out] DispatchHandle    The handle for this callback registration

  @retval     EFI_SUCCESS       Registration succeed
  @retval     EFI_ACCESS_DENIED Return access denied if the SmmReadyToLock event has been triggered
  @retval     others            Registration failed
**/
EFI_STATUS
EFIAPI
FlashFatalErrRegister (
  IN  PCH_ESPI_SMI_DISPATCH_PROTOCOL  *This,
  IN  PCH_ESPI_SMI_DISPATCH_CALLBACK  DispatchFunction,
  OUT EFI_HANDLE                      *DispatchHandle
  )
{
  EFI_STATUS Status;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = RegisterSerialIrqIfNull ();
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Insert a record
  //
  Status = InsertEspiRecord (DispatchFunction, FlashFatalErr, DispatchHandle);
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileRegisterHandler (&gPchEspiSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

/**
  eSPI SMI Dispatch Protocol instance to register a Link Error event

  @param[in]  This              Not used
  @param[in]  DispatchFunction  The callback to execute
  @param[out] DispatchHandle    The handle for this callback registration

  @retval     EFI_SUCCESS       Registration succeed
  @retval     EFI_ACCESS_DENIED Return access denied if the SmmReadyToLock event has been triggered
  @retval     others            Registration failed
**/
EFI_STATUS
EFIAPI
LnkType1ErrRegister (
  IN  PCH_ESPI_SMI_DISPATCH_PROTOCOL  *This,
  IN  PCH_ESPI_SMI_DISPATCH_CALLBACK  DispatchFunction,
  OUT EFI_HANDLE                      *DispatchHandle
  )
{
  EFI_STATUS Status;

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "Register is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  Status = RegisterSerialIrqIfNull ();
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Insert a record
  //
  Status = InsertEspiRecord (DispatchFunction, LnkType1Err, DispatchHandle);
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileRegisterHandler (&gPchEspiSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }
  return Status;
}

//
// EspiSlave srcdesc
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mSrcDescEspiSlave = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_SMI_EN}
      },
      S_ACPI_IO_SMI_EN,
      N_ACPI_IO_SMI_EN_ESPI
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
      N_ACPI_IO_SMI_STS_ESPI
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_ESPI
  }
};

/**
  eSPI SMI Dispatch Protocol instance to register a eSPI slave SMI
  This routine will also lock down ESPI_SMI_LOCK bit after registration and prevent
  this handler from unregistration.
  On platform that supports more than 1 device through another chip select (SPT-H),
  the SMI handler itself needs to inspect both the eSPI devices' interrupt status registers
  (implementation specific for each Slave) in order to identify and service the cause.
  After servicing it, it has to clear the Slaves' internal SMI# status registers

  @param[in]  This                      Not used
  @param[in]  DispatchFunction          The callback to execute
  @param[out] DispatchHandle            The handle for this callback registration

  @retval     EFI_SUCCESS               Registration succeed
  @retval     EFI_ACCESS_DENIED         Return access denied if the SmmReadyToLock event has been triggered
  @retval     EFI_ACCESS_DENIED         The ESPI_SMI_LOCK is set and register is blocked.
  @retval     others                    Registration failed
**/
EFI_STATUS
EFIAPI
EspiSlaveSmiRegister (
  IN  PCH_ESPI_SMI_DISPATCH_PROTOCOL    *This,
  IN  PCH_ESPI_SMI_DISPATCH_CALLBACK    DispatchFunction,
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

  //
  // If ESPI_SMI_LOCK is set, the register is blocked.
  //
  if (PmcIsEspiSmiLockSet ()) {
    return EFI_ACCESS_DENIED;
  }

  //
  // @note: This service doesn't utilize the data base of mEspiSmiInstance.
  //        While SMI is triggered it directly goes to the registing DispatchFunction
  //        instead of EspiSmiCallback.
  //
  Status = PchSmiRecordInsert (
             &mSrcDescEspiSlave,
             (PCH_SMI_CALLBACK_FUNCTIONS) DispatchFunction,
             PchEspiSmiEspiSlaveType,
             DispatchHandle
             );
  PchSmmClearSource (&mSrcDescEspiSlave);
  PchSmmEnableSource (&mSrcDescEspiSlave);

  //
  // Lock down the ESPI_SMI_LOCK after ESPI SMI is enabled.
  //
  PmcLockEspiSmiWithS3BootScript ();
  //
  // Keep the DispatchHandle which will be used for unregister function.
  //
  mEspiSmiInstance.PchSmiEspiHandle[EspiPmc] = *DispatchHandle;

  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileRegisterHandler (&gPchEspiSmiDispatchProtocolGuid, (EFI_SMM_HANDLER_ENTRY_POINT2)DispatchFunction, (UINTN)RETURN_ADDRESS (0), NULL, 0);
  }

  return Status;
}

/**
  eSPI SMI Dispatch Protocol instance to unregister a callback based on handle

  @param[in]  This                    Not used
  @param[in]  DispatchHandle          Handle acquired during registration

  @retval     EFI_SUCCESS             Unregister successful
  @retval     EFI_INVALID_PARAMETER   DispatchHandle is null
  @retval     EFI_INVALID_PARAMETER   DispatchHandle's forward link has bad pointer
  @retval     EFI_INVALID_PARAMETER   DispatchHandle does not exist in database
  @retval     EFI_ACCESS_DENIED       Unregistration is done after end of DXE
  @retval     EFI_ACCESS_DENIED       DispatchHandle is not allowed to unregistered
**/
EFI_STATUS
EFIAPI
EspiSmiUnRegister (
  IN  PCH_ESPI_SMI_DISPATCH_PROTOCOL  *This,
  IN  EFI_HANDLE                      DispatchHandle
  )
{
  EFI_STATUS          Status;
  ESPI_TOP_LEVEL_TYPE EspiTopLevelType;
  ESPI_SMI_TYPE       EspiSmiType;
  BOOLEAN             SafeToDisable;
  LIST_ENTRY          *LinkInDb;
  ESPI_SMI_RECORD     *RecordPointer;
  DATABASE_RECORD    *RecordToDelete;

  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Return access denied if the SmmReadyToLock event has been triggered
  //
  if (mReadyToLock == TRUE) {
    DEBUG ((DEBUG_ERROR, "UnRegister is not allowed if the SmmReadyToLock event has been triggered! \n"));
    return EFI_ACCESS_DENIED;
  }

  if (((LIST_ENTRY *) DispatchHandle)->ForwardLink == (LIST_ENTRY *) EFI_BAD_POINTER) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // For DispatchHandle belongs to Espi Slave SMI, refuses the request of unregistration.
  //
  if (mEspiSmiInstance.PchSmiEspiHandle[EspiPmc] == DispatchHandle) {
    DEBUG ((DEBUG_ERROR, "UnRegister is not allowed for ESPI Slave SMI handle! \n"));
    return EFI_ACCESS_DENIED;
  }

  //
  // Iterate through all the database to find the record
  //
  for (EspiSmiType = 0; EspiSmiType < EspiSmiTypeMax; ++EspiSmiType) {
    LinkInDb = GetFirstNode (&mEspiSmiInstance.CallbackDataBase[EspiSmiType]);

    while (!IsNull (&mEspiSmiInstance.CallbackDataBase[EspiSmiType], LinkInDb)) {
      if (LinkInDb != (LIST_ENTRY *) DispatchHandle) {
        LinkInDb = GetNextNode (&mEspiSmiInstance.CallbackDataBase[EspiSmiType], LinkInDb);

      } else {
        //
        // Found the source to be from this list
        //
        RemoveEntryList (LinkInDb);
        RecordPointer = (ESPI_RECORD_FROM_LINK (LinkInDb));

        if (mEspiSmiInstance.EspiSmiEventCounter[EspiSmiType] != 0) {
          if (--mEspiSmiInstance.EspiSmiEventCounter[EspiSmiType] == 0) {
            EspiSmiDisableSource (EspiSmiType);
          }
        }

        Status = gSmst->SmmFreePool (RecordPointer);
        if (EFI_ERROR (Status)) {
          ASSERT (FALSE);
        }

        goto EspiSmiUnRegisterEnd;
      }
    }
  }
  //
  // If the code reach here, the handle passed in cannot be found
  //
  DEBUG ((DEBUG_ERROR, "eSPI SMI handle is not in record database \n"));
  ASSERT (FALSE);
  return EFI_INVALID_PARAMETER;

EspiSmiUnRegisterEnd:

  //
  // Unregister and clear the handle from PchSmiDispatch
  //
  for (EspiTopLevelType = 0; EspiTopLevelType < EspiTopLevelTypeMax; ++EspiTopLevelType) {
    SafeToDisable = TRUE;
    //
    // Checks all the child events that belongs to a top level status in PMC
    //
    for (EspiSmiType = mEspiSmiInstance.Barrier[EspiTopLevelType].Start; EspiSmiType <= mEspiSmiInstance.Barrier[EspiTopLevelType].End; ++EspiSmiType) {
      if (mEspiSmiInstance.EspiSmiEventCounter[EspiSmiType] != 0) {
        SafeToDisable = FALSE;
      }
    }
    //
    // Finally, disable the top level event in PMC
    //
    if (SafeToDisable) {
      if (mEspiSmiInstance.PchSmiEspiHandle[EspiTopLevelType] != NULL) {
        Status = PchSmmCoreUnRegister (NULL, mEspiSmiInstance.PchSmiEspiHandle[EspiTopLevelType]);
        ASSERT_EFI_ERROR (Status);
        mEspiSmiInstance.PchSmiEspiHandle[EspiTopLevelType] = NULL;
      }
    }
  }
  RecordToDelete = DATABASE_RECORD_FROM_LINK (DispatchHandle);
  if (!EFI_ERROR (Status)) {
    SmiHandlerProfileUnregisterHandler (&gPchEspiSmiDispatchProtocolGuid, RecordToDelete->Callback, NULL, 0);
  }
  return EFI_SUCCESS;
}

/**
  Returns AND maks for clearing eSPI channel registers errors statuses
  In addition to common status bit we add channel specific erro bits to avoid clearing them

  @param[in]  ChannelNumber           Channel number (0 for PC, 1 for VW, 2 for OOB, 3 for FA)

  @retval     UINT32                  AND mask with all the status bit masked to not clear them by mistake
**/
UINT32
GetEspiChannelStatusClearMask (
  UINT8       ChannelNumber
  )
{
  UINT32    Data32;

  // Common error status bits for all channel registers
  Data32 = B_ESPI_PCR_XERR_XNFES | B_ESPI_PCR_XERR_XFES;

  // Check channels for channel specific status bits
  switch(ChannelNumber) {
    case 0:   // Peripheral Channel specific status bits
      Data32 |= B_ESPI_PCR_PCERR_PCURD;
      break;
    case 3:   // Flash Access Channel specific status bits
      Data32 |= B_ESPI_PCR_FCERR_SAFBLK;
      break;
  }

  // Return the expected AND mask
  return (UINT32)~(Data32);
}

/**
  Checks if channel error register data has Fatal Error bit set
  If yes then reset the channel on slave

  @param[in]  ChannelBaseAddress      Base address
  @param[in]  ChannelNumber           Channel number (0 for PC, 1 for VW, 2 for OOB, 3 for FA)
  @param[in]  SlaveId                 Slave ID of which channel is to be reset
**/
VOID
CheckSlaveChannelErrorAndReset (
  UINT16      ChannelBaseAddress,
  UINT8       ChannelNumber,
  UINT8       SlaveId
  )
{
  UINT32      Data32;
  UINT16      ChannelAddress;
  EFI_STATUS  Status;

  if (ChannelNumber == 2) {
    DEBUG ((DEBUG_INFO, "Channel %d is not supported by this function due to lack of error register\n", ChannelNumber));
    return;
  }

  if (!IsEspiSlaveChannelSupported (SlaveId, ChannelNumber)) {
    DEBUG ((DEBUG_WARN, "Channel %d is not supported by slave device\n", ChannelNumber));
    return;
  }

  // Calculate channel address based on slave id
  ChannelAddress = (UINT16) (ChannelBaseAddress + (SlaveId * S_ESPI_PCR_XERR));

  // Reading channel error register data
  Data32 = PchPcrRead32 (PID_ESPISPI, ChannelAddress);

  DEBUG ((DEBUG_INFO, "eSPI channel error register (0x%4X) has value 0x%8X\n", ChannelAddress, Data32));

  // Check Fatal Error status bit in channel error register data
  if ((Data32 & B_ESPI_PCR_XERR_XFES) != 0) {
    Status = PchEspiSlaveChannelReset (SlaveId, ChannelNumber);

    if (EFI_ERROR (Status)) {
      switch (Status) {
        case EFI_UNSUPPORTED:
          DEBUG ((DEBUG_ERROR, "Slave doesn't support channel %d\n", ChannelNumber));
          break;
        case EFI_TIMEOUT:
          DEBUG ((DEBUG_ERROR, "Timeout occured during channel %d reset on slave %d\n", ChannelNumber, SlaveId));
          break;
        default:
          DEBUG ((DEBUG_ERROR, "Error occured during channel %d reset\n", ChannelNumber));
          break;
      }
    } else {
      DEBUG ((DEBUG_INFO, "eSPI Slave %d channel %d reset ended successfully\n", SlaveId, ChannelNumber));
      // If channel reset was successfull clear the fatal error flag by writing one
      // we should be aware not to clear other status bits by mistake and mask them
      PchPcrAndThenOr32 (
        PID_ESPISPI,
        ChannelAddress,
        GetEspiChannelStatusClearMask (ChannelNumber),
        B_ESPI_PCR_XERR_XFES
        );
    }
  }
}

/**
  eSPI SMI handler for Fatal Error recovery flow
**/
VOID
EspiDefaultFatalErrorHandler (
  VOID
  )
{
  UINT32      Data32;
  UINT8       SlaveId;
  UINT8       MaxSlavesCount;

  DEBUG ((DEBUG_INFO, "[EspiRecoverFromFatalError] Enter\n"));

  MaxSlavesCount = IsEspiSecondSlaveSupported () ? 2 : 1;

  DEBUG ((DEBUG_INFO, "[EspiRecoverFromFatalError] MaxSlavesCount %d\n", MaxSlavesCount));

  for (SlaveId = 0; SlaveId < MaxSlavesCount; ++SlaveId) {
    //
    // Check if slave has SLCRR bit set. If it does it means it needs recovery.
    //
    Data32 = PchPcrRead32 (PID_ESPISPI, (UINT16) (R_ESPI_PCR_LNKERR_SLV0 + (SlaveId * S_ESPI_PCR_XERR)));

    DEBUG ((DEBUG_INFO, "[EspiRecoverFromFatalError] Slave %d LNKERR reg 0x%8X\n", SlaveId, Data32));
    //
    // If SLCRR[31] bit is set we need to recover that slave
    //
    if ((Data32 & B_ESPI_PCR_LNKERR_SLV0_SLCRR) != 0) {
      // 1. Perform in-band reset
      PchEspiSlaveInBandReset (SlaveId);

      // 2. Channels reset
      CheckSlaveChannelErrorAndReset (R_ESPI_PCR_PCERR_SLV0, 0, SlaveId); // Peripheral channel reset
      CheckSlaveChannelErrorAndReset (R_ESPI_PCR_VWERR_SLV0, 1, SlaveId); // Virtual Wire channel reset

      // Flash Access channel is not supported for CS1#
      if (SlaveId == 0) {
        CheckSlaveChannelErrorAndReset (R_ESPI_PCR_PCERR_SLV0, 3, SlaveId); // Flash Access channel reset
      }

      // Clear SLCRR bit of slave after all channels recovery was done
      PchPcrAndThenOr32 (
        PID_ESPISPI,
        (UINT16) (R_ESPI_PCR_LNKERR_SLV0 + (SlaveId * S_ESPI_PCR_XERR)),
        (UINT32)~(B_ESPI_PCR_LNKERR_SLV0_LFET1S),
        (UINT32) (B_ESPI_PCR_LNKERR_SLV0_SLCRR)
        );
    }
  }

  DEBUG ((DEBUG_INFO, "[EspiRecoverFromFatalError] Exit\n"));
}


