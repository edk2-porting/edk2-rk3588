/** @file

  @copyright
  Copyright 2018 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiBoardInit.h"
#include <ImonVrSvid.h>
#include <Library/MemVrSvidMapLib.h>
#include <Guid/PlatformInfo.h>
#include <Library/UbaPcdUpdateLib.h>
#include <Library/PcdLib.h>
#include <UncoreCommonIncludes.h>
#include <Ppi/DynamicSiLibraryPpi.h>
#include <CpuAndRevisionDefines.h>

#define GPIO_SKL_H_GPP_B20      0x01010014

VOID TypeWilsonCitySMTPlatformUpdateVrIdAddress (VOID);

/**
  Update WilsonCity IMON SVID Information

  retval N/A
**/
VOID
TypeWilsonCitySMTPlatformUpdateImonAddress (
  VOID
  )
{
  VCC_IMON *VccImon = NULL;
  UINTN Size = 0;

  Size = sizeof (VCC_IMON);
  VccImon = (VCC_IMON *) PcdGetPtr (PcdImonAddr);
  if (VccImon == NULL) {
    DEBUG ((DEBUG_ERROR, "UpdateImonAddress() - PcdImonAddr == NULL\n"));
    return;
  }

  VccImon->VrSvid[0] = PcdGet8 (PcdWilsonCitySvidVrP1V8);
  VccImon->VrSvid[1] = PcdGet8 (PcdWilsonCitySvidVrVccAna);
  VccImon->VrSvid[2] = IMON_ADDR_LIST_END; // End array with 0xFF

  PcdSetPtrS (PcdImonAddr, &Size, (VOID *) VccImon);
}

/**
  Update WilsonCity VR ID SVID Information

  retval N/A
**/
VOID
TypeWilsonCitySMTPlatformUpdateVrIdAddress (
  VOID
  )
{
  MEM_SVID_MAP *MemSvidMap = NULL;
  UINTN Size = 0;

  Size = sizeof (MEM_SVID_MAP);
  MemSvidMap = (MEM_SVID_MAP *) PcdGetPtr (PcdMemSrvidMap);
  if (MemSvidMap == NULL) {
    DEBUG ((DEBUG_ERROR, "UpdateVrIdAddress() - PcdMemSrvidMap == NULL\n"));
    return;
  }
  /*
    Map VR ID Address to Memory controller
    The mailbox command can support up to 4 DDR VR ID's, 0x10, 0x12, 0x14, and 0x16.
    Whitley PHAS indicates that Whitley (like Purley) only connects 2 VRs (VR ID's 0x10 and 0x12).
    Those are typically shared such that MC0/MC2 share the same DDR VR (as they are on the same side of the CPU)
    and MC1/MC3 share the other. Depending on motherboard layout and other design constraints, this could change
    BIT   4 => 0 or 1, SVID BUS\Interface 0 or 1 respectively
    BIT 0:3 => SVID ADDRESS
  */
  MemSvidMap->Socket[0].Mc[0] = 0x10;  //SVID BUS 1, ADDR 0
  MemSvidMap->Socket[0].Mc[1] = 0x12;  //SVID BUS 1, ADDR 2
  MemSvidMap->Socket[1].Mc[0] = 0x10;  //SVID BUS 1, ADDR 0
  MemSvidMap->Socket[1].Mc[1] = 0x12;  //SVID BUS 1, ADDR 2
  MemSvidMap->Socket[2].Mc[0] = 0x10;  //SVID BUS 1, ADDR 0
  MemSvidMap->Socket[2].Mc[1] = 0x12;  //SVID BUS 1, ADDR 2
  MemSvidMap->Socket[3].Mc[0] = 0x10;  //SVID BUS 1, ADDR 0
  MemSvidMap->Socket[3].Mc[1] = 0x12;  //SVID BUS 1, ADDR 2
  MemSvidMap->Socket[4].Mc[0] = 0x10;  //SVID BUS 1, ADDR 0
  MemSvidMap->Socket[4].Mc[1] = 0x12;  //SVID BUS 1, ADDR 2
  MemSvidMap->Socket[5].Mc[0] = 0x10;  //SVID BUS 1, ADDR 0
  MemSvidMap->Socket[5].Mc[1] = 0x12;  //SVID BUS 1, ADDR 2
  MemSvidMap->Socket[6].Mc[0] = 0x10;  //SVID BUS 1, ADDR 0
  MemSvidMap->Socket[6].Mc[1] = 0x12;  //SVID BUS 1, ADDR 2
  MemSvidMap->Socket[7].Mc[0] = 0x10;  //SVID BUS 1, ADDR 0
  MemSvidMap->Socket[7].Mc[1] = 0x12;  //SVID BUS 1, ADDR 2

  PcdSetPtrS (PcdMemSrvidMap, &Size, (VOID *) MemSvidMap);
}

EFI_STATUS
TypeWilsonCitySMTPlatformPcdUpdateCallback (
  VOID
)
{
  CHAR8     FamilyName[]  = "Whitley";

  CHAR8     BoardName[]   = "EPRP";
  UINT32    Data32;
  UINTN     Size;
  UINTN     PlatformFeatureFlag = 0;

  CHAR16    PlatformName[]   = L"TypeWilsonCitySMT";
  UINTN     PlatformNameSize = 0;
  EFI_STATUS Status;

  //#Integer for BoardID, must match the SKU number and be unique.
  Status = PcdSet16S (PcdOemSkuBoardID                      , TypeWilsonCitySMT);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  Status = PcdSet16S (PcdOemSkuBoardFamily                  , 0x30);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  // Number of Sockets on Board.
  Status = PcdSet32S (PcdOemSkuBoardSocketCount, 2);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  // Max channel and max DIMM
  Status = PcdSet32S (PcdOemSkuMaxChannel , 8);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  Status = PcdSet32S (PcdOemSkuMaxDimmPerChannel , 2);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  Status = PcdSetBoolS (PcdOemSkuDimmLayout, TRUE);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //Update Onboard Video Controller PCI Ven_id, Dev_id
  Status = PcdSet16S (PcdOnboardVideoPciVendorId, 0x1A03);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = PcdSet16S (PcdOnboardVideoPciDeviceId, 0x2000);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //#
  //# Misc.
  //#
  //# V_PCIE_PORT_PXPSLOTCTRL_ATNLED_OFF
  Status = PcdSet16S (PcdOemSkuMrlAttnLed                   , 0xc0);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //SDP Active Flag
  Status = PcdSet8S (PcdOemSkuSdpActiveFlag                , 0x0);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //# Zero terminated string to ID family
  Size = AsciiStrSize (FamilyName);
  Status = PcdSetPtrS (PcdOemSkuFamilyName             , &Size, FamilyName);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //# Zero terminated string to Board Name
  Size = AsciiStrSize (BoardName);
  Status = PcdSetPtrS (PcdOemSkuBoardName              , &Size, BoardName);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  PlatformNameSize = sizeof (PlatformName);
  Status = PcdSet32S (PcdOemSkuPlatformNameSize           , (UINT32)PlatformNameSize);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  Status = PcdSetPtrS (PcdOemSkuPlatformName              , &PlatformNameSize, PlatformName);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //# FeaturesBasedOnPlatform
  Status = PcdSet32S (PcdOemSkuPlatformFeatureFlag           , (UINT32)PlatformFeatureFlag);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //# Assert GPIO
  Data32 = 0;
  Status = PcdSet32S (PcdOemSkuAssertPostGPIOValue, Data32);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  Status = PcdSet32S (PcdOemSkuAssertPostGPIO, GPIO_SKL_H_GPP_B20);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //# UplinkPortIndex
  Status = PcdSet8S (PcdOemSkuUplinkPortIndex, 5);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "Uba Callback: PlatformPcdUpdateCallback is called!\n"));
  Status = TypeWilsonCitySMTPlatformUpdateAcpiTablePcds ();
  //# BMC Pcie Port Number
  PcdSet8S (PcdOemSkuBmcPciePortNumber, 5);
  ASSERT_EFI_ERROR(Status);

  //# Board Type Bit Mask
  PcdSet32S (PcdBoardTypeBitmask, CPU_TYPE_F_MASK | (CPU_TYPE_F_MASK << 4));
  ASSERT_EFI_ERROR(Status);

  //Update IMON Address
  TypeWilsonCitySMTPlatformUpdateImonAddress ();

  return Status;
}

PLATFORM_PCD_UPDATE_TABLE    TypeWilsonCitySMTPcdUpdateTable =
{
  PLATFORM_PCD_UPDATE_SIGNATURE,
  PLATFORM_PCD_UPDATE_VERSION,
  TypeWilsonCitySMTPlatformPcdUpdateCallback
};

EFI_STATUS
TypeWilsonCitySMTInstallPcdData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
)
{
  EFI_STATUS                            Status;

  Status = UbaConfigPpi->AddData (
                                 UbaConfigPpi,
                                 &gPlatformPcdConfigDataGuid,
                                 &TypeWilsonCitySMTPcdUpdateTable,
                                 sizeof(TypeWilsonCitySMTPcdUpdateTable)
                                 );

  return Status;
}

