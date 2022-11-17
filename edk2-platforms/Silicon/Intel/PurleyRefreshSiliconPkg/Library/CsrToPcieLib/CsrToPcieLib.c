/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "CpuCsrAccessDefine.h"
#include "CpuPciAccess.h"

CPU_CSR_ACCESS_VAR  *gCpuCsrAccessVarPtr = NULL;
CPU_CSR_ACCESS_VAR gCpuCsrAccessVar;


UINT32
GetSegmentNumber (
  IN USRA_ADDRESS    *Address
  )
{
  return 0;
};

UINT32
GetBDFNumber (
  IN USRA_ADDRESS    *Address,
  CPU_CSR_ACCESS_VAR          *CpuCsrAccessVar,
  IN UINT8                    BDFType
//  UINT8    SocId,
//  UINT8    BoxType
  )
/*++

Routine Description:
  Indetifies the bus number for given SocId & BoxType

Arguments:
  Address   - A pointer of the address of the USRA Address Structure with Csr or CsrEx type

Returns:
  PCI bus number

--*/
{
  UINT32   Data32 =0 ;
  UINT8    SocId;
  UINT8    BoxType;
  UINT8    BoxInst;
  UINT8    FuncBlk;
  PSYSHOST host;

  SocId = (UINT8)Address->Csr.SocketId;
  BoxType = (UINT8)((CSR_OFFSET *)(&Address->Csr.Offset))->Bits.boxtype;
  BoxInst = (UINT8)Address->Csr.InstId;
  FuncBlk = (UINT8)((CSR_OFFSET *)(&Address->Csr.Offset))->Bits.funcblk;
#ifdef IA32
  host = (PSYSHOST)Address->Attribute.HostPtr;
#else
  host = (PSYSHOST)NULL;
#endif
  if(BDFType == BUS_CLASS){
    Data32 = GetBusNumber(host, SocId, BoxType, BoxInst, FuncBlk, CpuCsrAccessVar);
  } else if(BDFType == DEVICE_CLASS){
    Data32 = GetDeviceNumber(host, BoxType, BoxInst, FuncBlk, CpuCsrAccessVar);
  } else {
    Data32 = GetFunctionNumber(host, BoxType, BoxInst, FuncBlk, CpuCsrAccessVar);
  }
  return Data32;

}

VOID
GetBDF (
  IN USRA_ADDRESS    *Address,
  CPU_CSR_ACCESS_VAR        *CpuCsrAccessVar,
  USRA_PCIE_ADDR_TYPE       *PcieAddress
  )
/*++

Routine Description:
  Indetifies the bus number for given SocId & BoxType

Arguments:
  Address   - A pointer of the address of the USRA Address Structure with Csr or CsrEx type

Returns:
  PCI bus number

--*/
{
  UINT8    SocId;
  UINT8    BoxType;
  UINT8    BoxInst;
  UINT8    FuncBlk;
  PSYSHOST host;

  SocId = (UINT8)Address->Csr.SocketId;
  BoxType = (UINT8)((CSR_OFFSET *)(&Address->Csr.Offset))->Bits.boxtype;
  BoxInst = (UINT8)Address->Csr.InstId;
  FuncBlk = (UINT8)((CSR_OFFSET *)(&Address->Csr.Offset))->Bits.funcblk;
#ifdef IA32
  host = (PSYSHOST)Address->Attribute.HostPtr;
#else
  host = (PSYSHOST)NULL;
#endif
    PcieAddress->Bus = GetBusNumber(host, SocId, BoxType, BoxInst, FuncBlk, CpuCsrAccessVar);
    PcieAddress->Dev = GetDeviceNumber(host, BoxType, BoxInst, FuncBlk, CpuCsrAccessVar);
    PcieAddress->Func = GetFunctionNumber(host, BoxType, BoxInst, FuncBlk, CpuCsrAccessVar);
    PcieAddress->Seg = SocId;  // Refcode and EFI data structure difference.   Refcode treats this array as 1 entry per socket, and not per segment, thus we index by SocId for now..
}
//////////////////////////////////////////////////////////////////////////
//
// USRA Silicon Address Library
// This Lib provide the way use platform Library instance
//
//////////////////////////////////////////////////////////////////////////

/**
  This Lib Convert the logical address (CSR type, e.g. CPU ID, Boxtype, Box instance etc.) into physical address

  @param[in] Global               Global pointer
  @param[in] Virtual              Virtual address
  @param[in] Address              A pointer of the address of the USRA Address Structure
  @param[out] AlignedAddress      A pointer of aligned address converted from USRA address

  @retval NULL                    The function completed successfully.
  @retval <>NULL                  Return Error
**/
UINTN
EFIAPI
CsrGetPcieAlignAddress (
  IN VOID                     *Global,
  IN BOOLEAN                  Virtual,
  IN USRA_ADDRESS             *Address,
  OUT UINTN                   *AlignedAddress
  )
{
  CPU_CSR_ACCESS_VAR *pCpuCsrAccessVar;
  USRA_ADDRESS                UsraAddress;
  INTN                        MmCfgBase;

  /*********************************************************************************************************
    ToDo:
        For now, this implementation only covers the Bus/Dev/Fun number generation for IVT and HSX CPUs.
        Register offset and size information comes from the HSX style register offset passed to this function.
        When the auto generation of header files using the new format is available, then we need to implement
        the logic to translate the register pseudo offset into real offset.
   *********************************************************************************************************/
    Address->Attribute.AccessWidth = (UINT8) (((((CSR_OFFSET *) &Address->Csr.Offset)->Bits.size) & 0x06) >> 1);

#if defined (IA32) || defined (SIM_BUILD) || defined(KTI_SW_SIMULATION) || defined (HEADLESS_MRC)
  CpuDeadLoop();
  pCpuCsrAccessVar = NULL;

#else
  gCpuCsrAccessVarPtr = &gCpuCsrAccessVar;
  GetCpuCsrAccessVar_RC ((PSYSHOST)host, &gCpuCsrAccessVar);

  pCpuCsrAccessVar = &gCpuCsrAccessVar;
#endif // defined

  //
  // Identify the PCI Bus/Device/Function number for the access
  //
  USRA_ZERO_ADDRESS_TYPE(UsraAddress, AddrTypePCIE);
  GetBDF(Address, pCpuCsrAccessVar, &UsraAddress.Pcie);

  UsraAddress.Pcie.Offset = (UINT16)((CSR_OFFSET *) &Address->Csr.Offset)->Bits.offset;
  UsraAddress.Attribute.HostPtr = Address->Attribute.HostPtr;

  MmCfgBase = GetPcieSegMmcfgBaseAddress(&UsraAddress);
  *AlignedAddress = MmCfgBase + (UINTN)(UsraAddress.Attribute.RawData32[0] & 0x0fffffff);

#if defined (IA32) || defined (SIM_BUILD) || defined(KTI_SW_SIMULATION) || defined (HEADLESS_MRC)
  CpuDeadLoop();
#endif

  return 0;
};

