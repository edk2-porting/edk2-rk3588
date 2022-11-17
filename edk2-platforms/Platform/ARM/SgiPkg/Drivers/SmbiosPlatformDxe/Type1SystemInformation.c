/** @file
  SMBIOS Type 1 (System information) table for ARM RD platforms.

  This file installs SMBIOS Type 1 (System information) table for Arm's
  Reference Design platforms. Type 1 table defines attributes of the
  overall system such as manufacturer, product name, UUID etc.

  Copyright (c) 2021, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Specification Reference:
    - SMBIOS Reference Specification 3.4.0, Chapter 7.2
**/

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Protocol/Smbios.h>

#include "SgiPlatform.h"

#define TYPE1_STRINGS                                   \
  "ARM LTD\0"                   /* Manufacturer */      \
  "Version not set\0"           /* Version */           \
  "Serial not set\0"            /* Serial number */     \
  "Not Applicable\0"            /* SKU */               \
  "Not Applicable\0"            /* Family */            \
  "SGI575\0"                    /* Product Names */     \
  "RdN1Edge\0"                                          \
  "RdN1EdgeX2\0"                                        \
  "RdE1Edge\0"                                          \
  "RdV1\0"                                              \
  "RdV1Mc\0"                                            \
  "RdN2\0"                                              \
  "RdN2Cfg1\0"

typedef enum {
  ManufacturerName = 1,
  Version,
  SerialNumber,
  Sku,
  Family,
  ProductNameBase
} TYPE1_STRING_ELEMENTS;

/* SMBIOS Type1 structure */
#pragma pack(1)
typedef struct {
  SMBIOS_TABLE_TYPE1  Base;
  CHAR8               Strings[sizeof (TYPE1_STRINGS)];
} ARM_RD_SMBIOS_TYPE1;
#pragma pack()

STATIC GUID mSmbiosUid[] = {
  /* Sgi575        */
  {0xdd7cad0a, 0x227c, 0x4ed4, {0x9f, 0x42, 0xa9, 0x8b, 0xd6, 0xa2, 0x42, 0x6c}},
  /* Rd-N1-Edge    */
  {0x80984efe, 0x404a, 0x43e0, {0xad, 0xa4, 0x63, 0xa0, 0xe0, 0xc4, 0x5e, 0x60}},
  /* Rd-N1-Edge-X2 */
  {0x2cc4f916, 0x267a, 0x4251, {0x95, 0x6e, 0xf0, 0x49, 0x82, 0xbe, 0x94, 0x58}},
  /* Rd-E1-Edge    */
  {0x567f35c4, 0x104f, 0x447b, {0xa0, 0x94, 0x89, 0x2f, 0xbd, 0xb6, 0x5a, 0x55}},
  /* Rd-V1         */
  {0xc481f0b1, 0x237c, 0x42d7, {0x98, 0xb2, 0xb4, 0xb4, 0x8d, 0xb5, 0x4f, 0x50}},
  /* Rd-V1Mc       */
  {0x1f3a0806, 0x18b5, 0x4eca, {0xad, 0xcd, 0xba, 0x9b, 0x07, 0xb1, 0x0a, 0xcf}},
  /* Rd-N2         */
  {0xf2cded73, 0x37f9, 0x4ec9, {0xd9, 0xf9, 0x89, 0x9b, 0x74, 0x91, 0x20, 0x49}},
  /* Rd-N2-Cfg1    */
  {0xa4941d3d, 0xfac3, 0x4ace, {0x9a, 0x7e, 0xce, 0x26, 0x76, 0x64, 0x5e, 0xda}},
};

/* System information */
STATIC ARM_RD_SMBIOS_TYPE1 mArmRdSmbiosType1 = {
  {
    {
      // SMBIOS header
      EFI_SMBIOS_TYPE_SYSTEM_INFORMATION, // Type 1
      sizeof (SMBIOS_TABLE_TYPE1),        // Length
      SMBIOS_HANDLE_PI_RESERVED,          // Assign an unused handle number
    },
    ManufacturerName,       // Manufacturer
    ProductNameBase,        // Product Name, update dynamically
    Version,                // Version
    SerialNumber,           // Serial
    {0},                    // UUID, Update dymanically
    1,                      // Wakeup type other
    Sku,                    // SKU
    Family,                 // Family
  },
  // Text strings (unformatted)
  TYPE1_STRINGS
};

/**
  Install SMBIOS System information Table.

  Install the SMBIOS system information (type 1) table for Arm's reference
  design platforms.

  @param[in]  Smbios   SMBIOS protocol.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_NOT_FOUND         Unknown product id.
  @retval EFI_OUT_OF_RESOURCES  Record was not added.
  @retval EFI_ALREADY_STARTED   The SmbiosHandle passed is already in use.
**/
EFI_STATUS
InstallType1SystemInformation (
  IN     EFI_SMBIOS_PROTOCOL    *Smbios
  )
{
  EFI_STATUS Status;
  EFI_SMBIOS_HANDLE SmbiosHandle;
  UINT8 ProductId;

  SmbiosHandle = ((EFI_SMBIOS_TABLE_HEADER *)&mArmRdSmbiosType1)->Handle;
  ProductId = SgiGetProductId ();

  /* Choose the product name from TYPE1_STRINGS based on the product ID */
  if (ProductId != UnknownId) {
    mArmRdSmbiosType1.Base.ProductName =
      ProductNameBase + (ProductId - 1);
    CopyGuid (&mArmRdSmbiosType1.Base.Uuid,
              &mSmbiosUid[ProductId - 1]);
  } else {
    return EFI_NOT_FOUND;
  }

  /* Install type 1 table */
  Status = Smbios->Add (
                     Smbios,
                     NULL,
                     &SmbiosHandle,
                     (EFI_SMBIOS_TABLE_HEADER *)&mArmRdSmbiosType1
                     );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "SMBIOS: Failed to install Type1 SMBIOS table.\n"
      ));
  }

  return Status;
}
