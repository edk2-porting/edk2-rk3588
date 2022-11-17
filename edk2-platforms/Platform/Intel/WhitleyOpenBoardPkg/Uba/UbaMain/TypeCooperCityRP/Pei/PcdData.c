/** @file

  @copyright
  Copyright 2018 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiBoardInit.h"
#include <Library/MemVrSvidMapLib.h>
#include <Guid/PlatformInfo.h>
#include <Library/UbaPcdUpdateLib.h>
#include <Library/PcdLib.h>
#include <UncoreCommonIncludes.h>
#include <Ppi/DynamicSiLibraryPpi.h>
#include <CpuAndRevisionDefines.h>

#define GPIO_SKL_H_GPP_B20      0x01010014

VOID TypeCooperCityRPPlatformUpdateVrIdAddress (VOID);

/**
  Update CooperCity VR ID SVID Information

  retval N/A
**/
VOID
TypeCooperCityRPPlatformUpdateVrIdAddress (
  VOID
  )
{
  MEM_SVID_MAP *MemSvidMap = NULL;
  UINTN Size = 0;

  Size = sizeof (MEM_SVID_MAP);
  MemSvidMap = (MEM_SVID_MAP *) PcdGetPtr (PcdMemSrvidMap);
  if (MemSvidMap == NULL) {
    DEBUG ((EFI_D_ERROR, "UpdateVrIdAddress() - PcdMemSrvidMap == NULL\n"));
    return;
  }
  /*
    Map VR ID Address to Memory controller
    The mailbox command can support up to 4 DDR VR ID's, 0x10, 0x12, 0x14, and 0x16.
    Whitley PHAS indicates that Whitley (like Purley) only connects 2 VRs (VR ID's 0x10 and 0x12).
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
TypeCooperCityRPPlatformPcdUpdateCallback (
  VOID
  )
{
  CHAR8     FamilyName[]  = "WildcatPass";

  CHAR8     BoardName[]   = "S2600WT ";

  UINTN     Size;
  UINT32    Data32;
  CHAR16    PlatformName[]   = L"TypeCooperCityRP";
  UINTN     PlatformNameSize = 0;
  UINTN     PlatformFeatureFlag = 0;
  UINT8     SKUType = 0;
  UINT8     SKUTypeSockets = 0;

  DYNAMIC_SI_LIBARY_PPI  *DynamicSiLibraryPpi = NULL;

  EFI_STATUS Status;

  //#Integer for BoardID, must match the SKU number and be unique.
  Status = PcdSet16S (PcdOemSkuBoardID                      , TypeCooperCityRP);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  Status = PcdSet16S (PcdOemSkuBoardFamily                  , 0x30);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  GetPlatformCapabilitiesInfo(&SKUType, DynamicSiLibraryPpi);
  DEBUG ((DEBUG_INFO, " PcdOemSkuBoardSocketCount sku type %d\n", SKUType));
  if (SKUType == PLATFORM_CAPABILITY_2_SOCKET) {
    SKUTypeSockets = 2;
  } else {
    SKUTypeSockets = (SKUType == PLATFORM_CAPABILITY_4_SOCKET) ? 4: 8;
  }
  //# Number of Sockets on Board.
  Status = PcdSet32S (PcdOemSkuBoardSocketCount             , SKUTypeSockets);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  // Max channel and max DIMM
  Status = PcdSet32S (PcdOemSkuMaxChannel , 6);
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

  DEBUG ((EFI_D_INFO, "Uba Callback: PlatformPcdUpdateCallback is called!\n"));
  Status = TypeCooperCityRPPlatformUpdateAcpiTablePcds ();

  //# Board Type Bit Mask
  PcdSet32S (PcdBoardTypeBitmask, 0);
  ASSERT_EFI_ERROR(Status);

  if (DynamicSiLibraryPpi->IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    // Update VR ID Address
    TypeCooperCityRPPlatformUpdateVrIdAddress ();
  }

  return Status;
}

PLATFORM_PCD_UPDATE_TABLE    TypeCooperCityRPPcdUpdateTable =
{
  PLATFORM_PCD_UPDATE_SIGNATURE,
  PLATFORM_PCD_UPDATE_VERSION,
  TypeCooperCityRPPlatformPcdUpdateCallback
};

EFI_STATUS
TypeCooperCityRPInstallPcdData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
)
{
  EFI_STATUS                            Status;

  Status = UbaConfigPpi->AddData (
                                 UbaConfigPpi,
                                 &gPlatformPcdConfigDataGuid,
                                 &TypeCooperCityRPPcdUpdateTable,
                                 sizeof(TypeCooperCityRPPcdUpdateTable)
                                 );

  return Status;
}

