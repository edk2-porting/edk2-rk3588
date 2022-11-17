/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <SysHost.h>
#include "CpuPciAccess.h"

#if !defined(MINIBIOS_BUILD) && !defined(KTI_SW_SIMULATION) && !defined(SIM_BUILD)
#include <Library/DebugLib.h>
#endif




#ifndef IA32

#include "Library/IoLib.h"
#include <Protocol/IioUds.h>
#include <Library/UefiBootServicesTableLib.h>


static EFI_IIO_UDS_PROTOCOL            *mIioUds;
IIO_UDS                         *mIioUdsDataPtr;
CPU_CSR_ACCESS_VAR *PCpuCsrAccessVarGlobal = NULL;
#endif
#ifndef IA32
CPU_CSR_ACCESS_VAR CpuCsrAccessVarGlobal;
#endif

//
// Disable warning for unsued input parameters
//
#ifdef _MSC_VER
#pragma warning(disable : 4100)
#pragma warning(disable : 4013)
#pragma warning(disable : 4306)
#endif

//
// PCI function translation table; note that this table doesn't capture the function
// information for all instances of a box. It only captures for the first instance.
// It has to be translated for other instances after the look up is done.
//
STATIC UINT8  FunTbl[MAX_CPU_TYPES][MAX_BOX_TYPES][8] = {
  {
    {0,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // CHA MISC             0
    {0,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // CHA PMA              1
    {0,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // CHA CMS              2
    {0,    1,    2,    3,    0xFF, 0xFF, 0xFF, 0xFF}, // CHABC                3
    {0,    1,    2,    3,    4,    5,    6,    7   }, // PCU                  4
    {0,    1,    2,    3,    4,    5,    6,    7   }, // VCU                  5
    {0,    1,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // M2MEM                6
    {0,    4,    0,    0,    4,    0,    0xFF, 0xFF}, // MC                   7  //SKX:Should be {0,    1,    4,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
    {0,    2,    4,    0,    2,    4,    0xFF, 0xFF}, // MCIO DDRIO           8  //SKX:should be {0,    1,    6,    7,    0xFF, 0xFF, 0xFF, 0xFF}
    {0,    1,    2,    3,    0xFF, 0xFF, 0xFF, 0xFF}, // KTI                  9
    {0,    1,    2,    3,    0xFF, 0xFF, 0xFF, 0xFF}, // M3KTI               10
    {2,    6,    2,    2,    6,    2,    0xFF, 0xFF}, // MCDDC               11 //SKX:SHould be {2,    3,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // MCDDC These entries all seem wrong but work
    {0,    1,    3,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // M2UPCIE             12
    {0,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIO DMI             13
    {0,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIO PCIE            14
    {0,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // IIO PCIENTB         15
    {0,    1,    2,    3,    4,    5,    6,    7   }, // IIOCB               16
    {0,    1,    2,    4,    5,    6,    0xFF, 0xFF}, // IIO VTD             17
    {0,    0,    7,    7,    4,    4,    0xFF, 0xFF}, // IIO_RTO             18
    {0,    1,    2,    0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // UBOX                19
  }, // SKX
};

STATIC UINT8 m2pcieDevTable[MAX_SKX_M2PCIE] = { 22, 21, 22, 23, 21};

/**

    Populate CpuCsrAccessVar structure.

    @param host            - pointer to the system host root structure
    @param CpuCsrAccessVar - pointer to CpuCsrAccessVar structure to be populated

    @retval None

**/
VOID
GetCpuCsrAccessVar_RC (
  PSYSHOST host,
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar
  )
{
#ifndef IA32
  EFI_STATUS  Status;
#endif

  if (host != NULL) {
    CpuDeadLoop ();
  }

#ifndef IA32
  if (host == NULL) {
    if(PCpuCsrAccessVarGlobal == NULL){     //check if 1st time, if yes, then need to update
      // Locate the IIO Protocol Interface
      Status = gBS->LocateProtocol (&gEfiIioUdsProtocolGuid, NULL, &mIioUds);
      mIioUdsDataPtr = (IIO_UDS *)mIioUds->IioUdsPtr;
      //ASSERT_EFI_ERROR (Status);

      PCpuCsrAccessVarGlobal = &CpuCsrAccessVarGlobal;
      for (socket = 0; socket < MAX_SOCKET; socket++) {
        CpuCsrAccessVarGlobal.stackPresentBitmap[socket] = mIioUdsDataPtr->PlatformData.CpuQpiInfo[socket].stackPresentBitmap;
        CpuCsrAccessVarGlobal.SocketFirstBus[socket] = mIioUdsDataPtr->PlatformData.CpuQpiInfo[socket].SocketFirstBus;
        CpuCsrAccessVarGlobal.SocketLastBus[socket] = mIioUdsDataPtr->PlatformData.CpuQpiInfo[socket].SocketLastBus;
        CpuCsrAccessVarGlobal.segmentSocket[socket] = mIioUdsDataPtr->PlatformData.CpuQpiInfo[socket].segmentSocket;

        for (ctr = 0; ctr < MAX_IIO_STACK; ctr++) {
          CpuCsrAccessVarGlobal.StackBus[socket][ctr] = mIioUdsDataPtr->PlatformData.CpuQpiInfo[socket].StackBus[ctr];
        }
      }

      CpuCsrAccessVarGlobal.cpuType = mIioUdsDataPtr->SystemStatus.cpuType;
      CpuCsrAccessVarGlobal.stepping = mIioUdsDataPtr->SystemStatus.MinimumCpuStepping;
      CpuCsrAccessVarGlobal.socketPresentBitMap = mIioUdsDataPtr->SystemStatus.socketPresentBitMap;
      CpuCsrAccessVarGlobal.FpgaPresentBitMap = mIioUdsDataPtr->SystemStatus.FpgaPresentBitMap;
      CpuCsrAccessVarGlobal.mmCfgBase = (UINT32)mIioUdsDataPtr->PlatformData.PciExpressBase;
      CpuCsrAccessVarGlobal.numChPerMC = mIioUdsDataPtr->SystemStatus.numChPerMC;
      CpuCsrAccessVarGlobal.maxCh = mIioUdsDataPtr->SystemStatus.maxCh;
      CpuCsrAccessVarGlobal.maxIMC = mIioUdsDataPtr->SystemStatus.maxIMC;
    }

    if ((PCpuCsrAccessVarGlobal != NULL) && (CpuCsrAccessVarGlobal.socketPresentBitMap != mIioUdsDataPtr->SystemStatus.socketPresentBitMap)) {
      for (socket = 0; socket < MAX_SOCKET; socket++) {
        CpuCsrAccessVarGlobal.stackPresentBitmap[socket] = mIioUdsDataPtr->PlatformData.CpuQpiInfo[socket].stackPresentBitmap;
        CpuCsrAccessVarGlobal.SocketFirstBus[socket] = mIioUdsDataPtr->PlatformData.CpuQpiInfo[socket].SocketFirstBus;
        CpuCsrAccessVarGlobal.SocketLastBus[socket] = mIioUdsDataPtr->PlatformData.CpuQpiInfo[socket].SocketLastBus;
        CpuCsrAccessVarGlobal.segmentSocket[socket] = mIioUdsDataPtr->PlatformData.CpuQpiInfo[socket].segmentSocket;

        for (ctr = 0; ctr < MAX_IIO_STACK; ctr++) {
          CpuCsrAccessVarGlobal.StackBus[socket][ctr] = mIioUdsDataPtr->PlatformData.CpuQpiInfo[socket].StackBus[ctr];
        }
      }

      CpuCsrAccessVarGlobal.cpuType = mIioUdsDataPtr->SystemStatus.cpuType;
      CpuCsrAccessVarGlobal.stepping = mIioUdsDataPtr->SystemStatus.MinimumCpuStepping;
      CpuCsrAccessVarGlobal.socketPresentBitMap = mIioUdsDataPtr->SystemStatus.socketPresentBitMap;
      CpuCsrAccessVarGlobal.FpgaPresentBitMap = mIioUdsDataPtr->SystemStatus.FpgaPresentBitMap;
      CpuCsrAccessVarGlobal.mmCfgBase = (UINT32)mIioUdsDataPtr->PlatformData.PciExpressBase;
      CpuCsrAccessVarGlobal.numChPerMC = mIioUdsDataPtr->SystemStatus.numChPerMC;
      CpuCsrAccessVarGlobal.maxCh = mIioUdsDataPtr->SystemStatus.maxCh;
      CpuCsrAccessVarGlobal.maxIMC = mIioUdsDataPtr->SystemStatus.maxIMC;
   }

   for (socket = 0; socket < MAX_SOCKET; socket++) {
      CpuCsrAccessVar->stackPresentBitmap[socket] = CpuCsrAccessVarGlobal.stackPresentBitmap[socket];


      CopyMem (&CpuCsrAccessVar->StackBus[socket], &CpuCsrAccessVarGlobal.StackBus[socket], MAX_IIO_STACK);
    }
    CpuCsrAccessVar->cpuType = CpuCsrAccessVarGlobal.cpuType;
    //CpuCsrAccessVar->stepping = CpuCsrAccessVarGlobal.stepping;
    CpuCsrAccessVar->socketPresentBitMap = CpuCsrAccessVarGlobal.socketPresentBitMap;
    CpuCsrAccessVar->FpgaPresentBitMap = CpuCsrAccessVarGlobal.FpgaPresentBitMap;
    //CpuCsrAccessVar->mmCfgBase = CpuCsrAccessVarGlobal.mmCfgBase;
    CpuCsrAccessVar->numChPerMC = CpuCsrAccessVarGlobal.numChPerMC;
    CpuCsrAccessVar->maxCh = CpuCsrAccessVarGlobal.maxCh;
    //CpuCsrAccessVar->maxIMC = CpuCsrAccessVarGlobal.maxIMC;
  }
#endif
}

/**

    Stall execution after internal assertion fails

    @param haltOnError - 1 stalls in infinite loop; 0 returns to caller

    @retval None

**/
VOID RcDeadLoop (
  UINT8 haltOnError
  )
{
  //
  // Prevent from optimizing out
  //
  while (*(volatile UINT8 *) &haltOnError);
}

/**

  CsrAccess specific print to serial output

  @param host      - Pointer to the system host (root) structure
  @param Format    - string format

  @retval N/A

**/
VOID
CpuCsrAccessError (
  PSYSHOST host,
  char* Format,
  ...
  )
{
  UINT8           haltOnError;
#ifdef SERIAL_DBG_MSG
#if !defined(MINIBIOS_BUILD) && !defined(KTI_SW_SIMULATION) && !defined(SIM_BUILD)
  UINT32          *pData32;
#endif
  va_list  Marker;
  va_start (Marker, Format);
#if !defined(MINIBIOS_BUILD) && !defined(KTI_SW_SIMULATION) && !defined(SIM_BUILD)
   if (host != NULL) {
    pData32 = (UINT32 *)Marker;
    if( (*pData32 & 0xFFFFFF00) == 0xFFFFFF00){      // check if input is one byte only
       *pData32 = *pData32 & 0x000000FF;
    } if( (*pData32 & 0xFFFF0000) == 0xFFFF0000){    // check if input is word only
       *pData32 = *pData32 & 0x0000FFFF;
    }
    DEBUG ((
      DEBUG_ERROR, Format, *pData32
    ));
  }
#else
  if (host != NULL) {
    rcVprintf (host, Format, Marker);
  }
#endif
  va_end (Marker);
#endif
  haltOnError = 1;
  RcDeadLoop (haltOnError);

  return;
}

/**

  Returns the CPU Index for MC func tbl lookup based on CPU type and CPU sub type.
  This index will be used for MC box instance -> function mapping look-up

  @param host      - Pointer to the system host (root) structure

  @retval Index for CPU type

**/
STATIC
UINT8
GetCpuIndex (
    PSYSHOST host
    )
{
  UINT8  cpuIndex = 0xFF;

  cpuIndex = 0;
  return cpuIndex;
}

/**

  Indetifies the bus number for given SocId & BoxType

  @param host      - Pointer to the system host (root) structure
  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxType   - Box Type; values come from CpuPciAccess.h

  @retval PCI bus number

**/
UINT32
GetBusNumber (
  PSYSHOST host,
  UINT8    SocId,
  UINT8    BoxType,
  UINT8    BoxInst,
  UINT8    FuncBlk,
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar
  )
{
  UINT32   Bus = 0;
  UINT8    TempStack = 0;



  // Make sure SocId or Fpga is valid
  if ((!((CpuCsrAccessVar->socketPresentBitMap & (1 << SocId)) && (BoxType != BOX_FPGA)))) {
    if ((!((CpuCsrAccessVar->FpgaPresentBitMap & (1 << SocId)) && (BoxType == BOX_FPGA)))) {
      CpuCsrAccessError (host, "\nInvalid Socket Id %d. \n", SocId);
    }
  }

  //
  // Each socket is assigned multiple buses
  // Check the box type and return the appropriate bus number.
  //
  if ((BoxType == BOX_MC)      ||
      (BoxType == BOX_MCDDC)   ||
      (BoxType == BOX_MCIO)    ||
      (BoxType == BOX_M2MEM)) {
    Bus = CpuCsrAccessVar->StackBus[SocId][IIO_PSTACK1];

  } else if (BoxType == BOX_UBOX) {
    Bus = CpuCsrAccessVar->StackBus[SocId][IIO_CSTACK];
  } else if ((BoxType == BOX_IIO_PCIE_DMI) ||
             (BoxType == BOX_IIO_CB)) {
    Bus = CpuCsrAccessVar->StackBus[SocId][IIO_CSTACK];
  } else if (BoxType == BOX_IIO_PCIE) {
    //
    // IIO_PCIE is used to access all pcie ports in all stacks
    //
    if (BoxInst == 0) {
      Bus = CpuCsrAccessVar->StackBus[SocId][IIO_CSTACK];
    } else {
      TempStack = IIO_PSTACK0 + ((BoxInst-1) / 4);
      if (TempStack < MAX_IIO_STACK) {
        Bus = CpuCsrAccessVar->StackBus[SocId][TempStack];
      } else {
        CpuCsrAccessError (host, "\nInvalid IIO_PCIE BoxInstance %d. \n", BoxInst);
      }
    }
  } else if (BoxType == BOX_IIO_VTD) {
    TempStack = IIO_CSTACK + BoxInst;
    if (TempStack < MAX_IIO_STACK) {
      Bus = CpuCsrAccessVar->StackBus[SocId][TempStack];
    } else {
      CpuCsrAccessError (host, "\nInvalid BOX_IIO_VTD BoxInstance %d. \n", BoxInst);
    }
  } else if (BoxType == BOX_IIO_PCIE_NTB) {
    if (BoxInst > 0) {
      TempStack = IIO_PSTACK0 + ((BoxInst-1) / 4);
      if (TempStack < MAX_IIO_STACK) {
        Bus = CpuCsrAccessVar->StackBus[SocId][TempStack];
      } else {
        CpuCsrAccessError (host, "\nInvalid BOX_IIO_PCIE_NTB BoxInstance %d. \n", BoxInst);
      }
    } else {
      CpuCsrAccessError (host, "\nInvalid BOX_IIO_PCIE_NTB BoxInstance %d. \n", BoxInst);
    }
  } else if (BoxType == BOX_IIO_RTO) {
    if (FuncBlk == IIO_RTO) {
      //
      // IIO_RTO is used to access all pcie ports in all stacks same as iio_pcie
      //
      if (BoxInst == 0) {
        Bus = CpuCsrAccessVar->StackBus[SocId][IIO_CSTACK];
      } else {
        TempStack = IIO_PSTACK0 + ((BoxInst-1) / 4);
        if (TempStack < MAX_IIO_STACK) {
          Bus = CpuCsrAccessVar->StackBus[SocId][TempStack];
        } else {
          CpuCsrAccessError (host, "\nInvalid IIO_PCIE BoxInstance %d. \n", BoxInst);
        }
      }
    } else if ((FuncBlk == IIO_RTO_GLOBAL) || (FuncBlk == IIO_RTO_VTD)) {
      //
      // IIO_RTO_GLOBAL and IIO_RTO_VTD maps 1 instance per c/p/m stack
      //
      if ((IIO_CSTACK + BoxInst) < MAX_IIO_STACK) {
        Bus = CpuCsrAccessVar->StackBus[SocId][IIO_CSTACK + BoxInst];
      }
    } else if ((FuncBlk == IIO_RTO_VTD_DMI) ||
              (FuncBlk == IIO_RTO_DMI)      ||
              (FuncBlk == IIO_RTO_GLOBAL_DMI)) {
      Bus = CpuCsrAccessVar->StackBus[SocId][IIO_CSTACK];
    } else {
      CpuCsrAccessError (host, "\nInvalid BoxType %d, Functional block %d. \n", BoxType, FuncBlk);
    }
  } else if ((BoxType == BOX_CHA_MISC)  ||
             (BoxType == BOX_CHA_PMA)   ||
             (BoxType == BOX_CHA_CMS)   ||
             (BoxType == BOX_CHABC)     ||
             (BoxType == BOX_PCU)       ||
             (BoxType == BOX_VCU)) {
    Bus = CpuCsrAccessVar->StackBus[SocId][IIO_PSTACK0];
  } else if ((BoxType == BOX_M2UPCIE)    ||
             (BoxType == BOX_KTI)        ||
             (BoxType == BOX_M3KTI)) {
    Bus = CpuCsrAccessVar->StackBus[SocId][IIO_PSTACK2];
  } else if (BoxType == BOX_FPGA) {
    Bus = CpuCsrAccessVar->StackBus[SocId][IIO_CSTACK];

  } else {
    // Error
    CpuCsrAccessError (host, "\nInvalid BoxType %d. \n", BoxType);
  }

  return Bus;
}

/**

  Indetifies the device number for given Box Type & Box Instance

  @param host      - Pointer to the system host (root) structure
  @param BoxType   - Box Type; values come from CpuPciAccess.h
  @param BoxInst   - Box Instance, 0 based
  @param FuncBlk   - Functional Block; values come from CpuPciAccess.h

  @retval PCI Device number

**/
UINT32
GetDeviceNumber (
  PSYSHOST host,
  UINT8    BoxType,
  UINT8    BoxInst,
  UINT8    FuncBlk,
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar
  )
{
  UINT32   Dev = 0;
  UINT8    CpuType, NumChPerMC;

  CpuType = CpuCsrAccessVar->cpuType;
  NumChPerMC = CpuCsrAccessVar->numChPerMC;


  //
  // Translate the Box Type & Instance into PCI Device number.
  //
  switch (BoxType) {
  case BOX_MC:
  case BOX_MCDDC:
    if (CpuType == CPU_SKX) {
      switch (BoxInst) {
      case 0:
      case 1:
        Dev = 10;
        break;
      case 2:
        Dev = 11;
        break;
      case 3:
      case 4:
        Dev = 12;
        break;
      case 5:
        Dev = 13;
        break;
      }
    } else {
      CpuCsrAccessError (host, "\nInvalid Cpu type.\n");
    }
    break;

  case BOX_MCIO:
    if (CpuType == CPU_SKX) {
      Dev = 22 + (BoxInst / NumChPerMC);
    } else {
      CpuCsrAccessError (host, "\nInvalid Cpu type.\n");
    }
    break;

  case BOX_M2MEM:
    if ((CpuType == CPU_SKX) && (BoxInst < MAX_SKX_M2MEM)) {
      Dev = 8 + BoxInst;
    } else {
      CpuCsrAccessError (host, "\nInvalid M2MEM Box Instance Number %d. \n", BoxInst);
    }
    break;

  case BOX_CHA_MISC:
    if ((CpuType == CPU_SKX) && (BoxInst < MAX_SKX_CHA)) {
      if (BoxInst < 8) {
        Dev = 8;
      } else if (BoxInst < 16) {
        Dev = 9;
      } else if (BoxInst < 24) {
        Dev = 10;
      } else if (BoxInst < 28) {
        Dev = 11;
      }
    } else {
      CpuCsrAccessError (host, "\nInvalid CHA Box Instance Number %d. \n", BoxInst);
    }
    break;

  case BOX_CHA_PMA:
    if ((CpuType == CPU_SKX) && (BoxInst < MAX_SKX_CHA)) {
      if (BoxInst < 8) {
        Dev = 14;
      } else if (BoxInst < 16) {
        Dev = 15;
      } else if (BoxInst < 24) {
        Dev = 16;
      } else if (BoxInst < 28) {
        Dev = 17;
      }
    } else {
      CpuCsrAccessError (host, "\nInvalid CHA Box Instance Number %d. \n", BoxInst);
    }
    break;

  case BOX_CHA_CMS:
    if ((CpuType == CPU_SKX) && (BoxInst < MAX_SKX_CHA)) {
      if (BoxInst < 8) {
        Dev = 20;
      } else if (BoxInst < 16) {
        Dev = 21;
      } else if (BoxInst < 24) {
        Dev = 22;
      } else if (BoxInst < 28) {
        Dev = 23;
      }
    } else {
      CpuCsrAccessError (host, "\nInvalid CHA Box Instance Number %d. \n", BoxInst);
    }
    break;

  case BOX_CHABC:
    if ((CpuType == CPU_SKX) && (BoxInst == 0)) {
      Dev = 29;
    } else {
      CpuCsrAccessError (host, "\nInvalid CHABC Box Instance Number %d. \n", BoxInst);
    }
    break;

  case BOX_PCU:
    if ((CpuType == CPU_SKX) && (BoxInst < MAX_ALL_PCU)) {
      Dev = 30;
    } else {
      CpuCsrAccessError (host, "\nInvalid PCU Box Instance Number %d. \n", BoxInst);
    }
    break;

  case BOX_VCU:
    if ((CpuType == CPU_SKX) && (BoxInst < MAX_ALL_VCU)) {
      Dev = 31;
    } else {
      CpuCsrAccessError (host, "\nInvalid VCU Box Instance Number %d. \n", BoxInst);
    }
    break;

  case BOX_KTI:
    /*
       Dev #  KTI(phy,logic)#
        14       0      0
        15       1      1
        16       2      2
    */
    if (CpuType == CPU_SKX) {
      if (BoxInst < MAX_SKX_KTIAGENT ) {
        Dev = 14 + BoxInst;
      } else {
        CpuCsrAccessError (host, "\nInvalid KTI Box Instance Number %d. \n", BoxInst);
      }
    } else {
      CpuCsrAccessError (host, "\nInvalid Cpu type.\n");
    }
    break;

  case BOX_M3KTI:
  /*
          Logical M3KTI #        Dev #      Fun #
  KTI01           0               18          0
  KTI23           1               18          4
  */

    if (CpuType == CPU_SKX) {
      if (BoxInst < 2 ) {
        Dev = 18;
      } else {
        CpuCsrAccessError (host, "\nInvalid Box instance.\n");
      }
    } else {
      CpuCsrAccessError (host, "\nInvalid Cpu type.\n");
    }
    break;

  case BOX_M2UPCIE:
    if (CpuType == CPU_SKX) {
      if (BoxInst < MAX_SKX_M2PCIE) {
        Dev = m2pcieDevTable[BoxInst];
      } else {
        CpuCsrAccessError (host, "\nInvalid KTI Box Instance Number %d. \n", BoxInst);
      }
    } else {
      CpuCsrAccessError (host, "\nInvalid Cpu type.\n");
    }
    break;

  case BOX_IIO_PCIE_DMI:
    if ((CpuType == CPU_SKX) && (BoxInst < MAX_ALL_IIO)) {
      Dev = 0;
    } else {
      CpuCsrAccessError (host, "\nInvalid IIO PCIE DMI Box Instance Number %d. \n", BoxInst);
    }
    break;

  case BOX_IIO_PCIE:
    if ((CpuType == CPU_SKX) && (BoxInst < MAX_ALL_IIO_PCIE)) {
      if (BoxInst == 0) {
        // Cstack
        Dev = 0;
      } else {
        // M/Pstacks
        Dev = 0 + ((BoxInst-1) % 4);
      }
    } else {
      CpuCsrAccessError (host, "\nInvalid IIO PCIE Box Instance Number %d. \n", BoxInst);
    }
    break;

  case BOX_IIO_PCIE_NTB:
    if ((CpuType == CPU_SKX)) {
      Dev = 0;
    } else {
      CpuCsrAccessError (host, "\nInvalid IIO PCIE Box Instance Number %d. \n", BoxInst);
    }
    break;

  case BOX_IIO_CB:
    if ((CpuType == CPU_SKX)) {
      Dev = 4;
    } else {
      CpuCsrAccessError (host, "\nInvalid IIO CB Box Instance Number %d. \n", BoxInst);
    }
    break;

  case BOX_IIO_VTD:
    if ((CpuType == CPU_SKX)) {
      Dev = 5;
    } else {
      CpuCsrAccessError (host, "\nInvalid IIO VTD Box Instance Number %d. \n", BoxInst);
    }
    break;

  case BOX_IIO_RTO:
    if ((CpuType == CPU_SKX) && (BoxInst < MAX_ALL_IIO_RTO)) {
      Dev = 7;
    } else {
      CpuCsrAccessError (host, "\nInvalid IIO RTO Box Instance Number %d. \n", BoxInst);
    }
    break;

  case BOX_UBOX:
    if ((CpuType == CPU_SKX) && (BoxInst < MAX_ALL_UBOX)) {
      Dev = 8;
    } else {
      CpuCsrAccessError (host, "\nInvalid Ubox Instance Number %d. \n", BoxInst);
      //Note: the fatal error function writes to UBOX CSR and recurses forever (until stack is gone).
    }
    break;
  case BOX_FPGA:
    if ((CpuType == CPU_SKX) && (BoxInst == 0)) {
      Dev = 16;
    } else {
      CpuCsrAccessError (host, "\nInvalid FPGA Instance number %d. \n", BoxInst);
    }
    break;

  default:
    CpuCsrAccessError (host, "\nInvalid Box Type %d. \n", BoxType);
  }

  if (Dev > 31) {
    CpuCsrAccessError (host, "\nInvalid Device %d accessed for Box Type %d and Box Instance %d. \n", Dev, BoxType, BoxInst);
  }
  return Dev;
}

/**

  Indetifies the function number for given BoxType, BoxInst & Functional Block

  @param host      - Pointer to the system host (root) structure
  @param BoxType   - Box Type; values come from CpuPciAccess.h
  @param BoxInst   - Box Instance, 0 based
  @param FuncBlk   - Functional Block; values come from CpuPciAccess.h

  @retval PCI Function number

**/
UINT32
GetFunctionNumber (
  PSYSHOST host,
  UINT8    BoxType,
  UINT8    BoxInst,
  UINT8    FuncBlk,
  CPU_CSR_ACCESS_VAR *CpuCsrAccessVar
  )
{
  UINT32   Fun = 0;
  UINT8    CpuIndex, CpuType, NumChPerMC;

  CpuType = CpuCsrAccessVar->cpuType;
  NumChPerMC = CpuCsrAccessVar->numChPerMC;

  // Get the CPU type, sub type
  CpuIndex  = GetCpuIndex(host);

  //
  // Translate the Box Type & Functional Block into PCI function number. Note that the box type & instance number
  // passed to this routine are assumed to be valid; here we only need to validate if the function number is correct
  // after the look up is done.
  //

  switch (BoxType) {

  case BOX_MC:

    if (FuncBlk == 0 || FuncBlk == 1) {
      Fun = FunTbl[CpuType][BoxType][BoxInst % NumChPerMC] + FuncBlk;
    } else {
      Fun = 4;
    }
    break;

  case BOX_MCDDC:

    Fun = FunTbl[CpuType][BoxType][BoxInst % NumChPerMC] + FuncBlk;
    break;

  case BOX_MCIO:

    if (FuncBlk == 2) {
      Fun = FunTbl[CpuType][BoxType][BoxInst % NumChPerMC] + 3;

    } else {
      Fun = FunTbl[CpuType][BoxType][BoxInst % NumChPerMC] + FuncBlk;
    }
    break;

  case BOX_CHA_MISC:
  case BOX_CHA_PMA:
  case BOX_CHA_CMS:
    //
    // For Cha, no table look up is needed; the function number can be obtained from instance number.
    //
    if ((CpuType == CPU_SKX) && (BoxInst < MAX_SKX_CHA)) {
      Fun = (BoxInst % 8);
    }
    break;

  case BOX_M3KTI:
  /*
          Logical M3KTI #        Dev #      Fun #
  KTI01           0               18          0
  KTI23           1               18          4
  */

    Fun = FunTbl[CpuType][BoxType][FuncBlk];
    if (BoxInst == 1) {
      Fun = Fun + 4;
    }
    break;

  case BOX_M2MEM:
  case BOX_CHABC:
  case BOX_PCU:
  case BOX_VCU:
  case BOX_IIO_PCIE_DMI:
  case BOX_IIO_PCIE:
  case BOX_IIO_PCIE_NTB:
  case BOX_IIO_CB:
  case BOX_IIO_VTD:
  case BOX_UBOX:
    Fun = FunTbl[CpuType][BoxType][FuncBlk];
    break;

  case BOX_M2UPCIE:
    Fun = FunTbl[CpuType][BoxType][FuncBlk];
    if (BoxInst == 2 ||  BoxInst == 4) {         // M2PCIE2 & M2MCP1
       Fun = Fun + 4;
    }
    break;

  case BOX_KTI:
    Fun = FunTbl[CpuType][BoxType][FuncBlk];
    if (BoxInst >=9 ) {
      Fun = Fun + 4;
    }
    break;

  case BOX_IIO_RTO:
    if ((BoxInst < MAX_ALL_IIO_RTO) && (FunTbl[CpuType][BoxType][FuncBlk] != 0xFF)) {
      if (FuncBlk == IIO_RTO) {
        if (BoxInst == 0) {
          // Cstack
          Fun = 0;
        } else {
          // M/Pstacks
          Fun = 0 + ((BoxInst-1) % 4);
        }
      } else {
        Fun = FunTbl[CpuType][BoxType][FuncBlk];
      }
    } else {
      CpuCsrAccessError (host, "\nInvalid IIO RTO Box Instance Number %d. \n", BoxInst);
    }
    break;

  case BOX_FPGA:
    if (BoxInst == 0) {
      Fun = 0;
    } else {
      CpuCsrAccessError (host, "\nInvalid FPGA Box Instance Number %d. \n", BoxInst);
    }
    break;

  default:
    CpuCsrAccessError (host, "\nInvalid Box Type %d. \n", BoxType);
  }

  if (Fun > 7) {
    CpuCsrAccessError (host, "\nInvalid Functional Block %d accessed for CPUType %d CPUIndex %d Box Type %d and Box Instance %d. \n",
              FuncBlk, CpuType, CpuIndex, BoxType, BoxInst);
  }

  return Fun;
}


