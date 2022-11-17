/** @file
  CmosAccessLib implementation.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "CmosAccessLibInternal.h"

/**
  Return the entry for the specified address in entries returned
  from platform.

  @param [in] Address  The CMOS address to find.

  @return A pointer to CMOS_ENTRY for the specified address,
          or NULL if not found.
**/
CMOS_ENTRY *
CmosAccessLibLocateEntry (
  IN UINT8      Address
  )
{
  UINTN              Index;
  UINTN              Count;
  CMOS_ENTRY         *Entries;

  Entries = PlatformCmosGetEntry (&Count);
  for (Index = 0; Index < Count; Index++) {
    if (Entries[Index].Address == Address) {
      return &Entries[Index];
    }
  }

  return NULL;
}

/**
  Test the attributes of the Entry and return ValueIfSet if test passes
  or !ValueIfSet if test fails. It returns DefaultValue if the Entry is
  NULL.

  @param [in] Address      The CMOS address.
  @param [in] Attributes   The attributes to test.
  @param [in] ValueIfSet   The value to return if test passes.
  @param [in] DefaultValue The value to return if Entry is NULL.
  @param [in] Entry        Pointer to entry.

  @retval ValueIfSet   If test passes.
  @retval !ValueIfSet  If test fails.
  @retval DefaultValue If the Entry is NULL.
**/
BOOLEAN
CmosAccessLibCheckAttribute (
  IN UINT8      Address,
  IN UINT8      Attributes,
  IN BOOLEAN    ValueIfSet,
  IN BOOLEAN    DefaultValue,
  IN CMOS_ENTRY *Entry        OPTIONAL
  )
{
  if (Entry != NULL) {
    ASSERT (Entry->Address == Address);
    if ((Entry->Attributes & Attributes) == Attributes) {
      return ValueIfSet;
    } else {
      return !ValueIfSet;
    }
  }

  return DefaultValue;
}

/**
  Check if the CMOS address needs Checksum calculation.

  @param [in] Address CMOS address to be checked
  @param [in] Entry   Pointer to entry.

  @retval TRUE       CMOS address needs Checksum calculation.
  @retval FALSE      CMOS address doesn't need Checksum calculation.
**/
BOOLEAN
CmosAccessLibNeedChecksum (
  IN UINT8       Address,
  IN CMOS_ENTRY  *Entry OPTIONAL
  )
{
  return CmosAccessLibCheckAttribute (Address, CMOS_EXCLUDE_FROM_CHECKSUM, FALSE, FALSE, Entry);
}


/**
  Check if the CMOS address needs to fill default data.

  @param [in] Address CMOS address to be checked
  @param [in] Entry   Pointer to entry.

  @retval TRUE       CMOS address need to fill default data.
  @retval FALSE      CMOS address doesn't need to fill default data.
**/
BOOLEAN
CmosAccessLibNeedFillDefault (
  IN UINT8       Address,
  IN CMOS_ENTRY  *Entry OPTIONAL
  )
{
  return CmosAccessLibCheckAttribute (Address, CMOS_EXCLUDE_FROM_INIT_DATA, FALSE, TRUE, Entry);
}

/**
  Check if the CMOS address is accessible.

  @param [in] Address CMOS address to be checked.
  @param [in] Entry   Pointer to entry.

  @retval TRUE       CMOS address is accessible.
  @retval FALSE      CMOS address isn't accessible.
**/
BOOLEAN
CmosAccessLibIsAccessible (
  IN UINT8       Address,
  IN CMOS_ENTRY  *Entry OPTIONAL
  )
{
  //
  // CMOS 0-9, A, B, C, D are for RTC.
  //
  if (Address <= 0xD) {
    return FALSE;
  }
  return CmosAccessLibCheckAttribute (Address, CMOS_EXCLUDE_FROM_ACCESS, FALSE, TRUE, Entry);
}

/**
  Return the CMOS location to store checksum.

  @param [out] Location Return the CMOS location to store the checksum.
**/
VOID
CmosAccessLibGetChecksumLocation (
  OUT CMOS_CHECKSUM_LOCATION_INFO *Location
  )
{
  UINTN                       Index;
  UINTN                       Count;
  CMOS_ENTRY                  *Entries;

  Location->Length = 0;

  Entries = PlatformCmosGetEntry (&Count);
  for (Index = 0; Index < Count; Index++) {
    if ((Entries[Index].Attributes & CMOS_CHECKSUM_LOCATION) == CMOS_CHECKSUM_LOCATION) {
      Location->Length++;
      if (Location->Length == 1) {
        Location->LowByteAddress = Entries[Index].Address;
      } else if (Location->Length == 2) {
        Location->HighByteAddress = Entries[Index].Address;
        break;
      }
    }
  }

  ASSERT (Location->Length <= 2);
}

/**
  Calculate the sum of CMOS values who need checksum calculation.

  @param [in] Location The CMOS location to store the checksum.

  @return The sum.
**/
UINT16
CmosAccessLibCalculateSum (
  IN CMOS_CHECKSUM_LOCATION_INFO *Location
  )
{
  UINT16                      Sum;
  UINTN                       Index;
  UINTN                       Count;
  CMOS_ENTRY                  *Entries;

  if (Location->Length == 0) {
    return 0;
  }

  Sum = 0;
  Entries = PlatformCmosGetEntry (&Count);
  for (Index = 0; Index < Count; Index++) {
    if (CmosAccessLibNeedChecksum (Entries[Index].Address, &Entries[Index])) {
      Sum += CmosRead8 (Entries[Index].Address);
    }
  }

  if (Location->Length == 1) {
    return (UINT8) Sum;
  } else {
    return Sum;
  }
}

/**
  Return the checksum value stored in CMOS.

  @param [in] Location The CMOS location to store the checksum.

  @return The checksum value.
**/
UINT16
CmosAccessLibReadChecksum (
  IN CMOS_CHECKSUM_LOCATION_INFO *Location
  )
{
  UINT16                      Checksum;

  Checksum = 0;

  switch (Location->Length) {
  case 2:
    Checksum = (CmosRead8 (Location->HighByteAddress) << 8);
    //
    // Fall to case 1 to get the low byte value
    //
  case 1:
    Checksum += CmosRead8 (Location->LowByteAddress);
    break;

  default:
    break;
  }
  return Checksum;
}


/**
  Write the Checksum to appropriate address.

  @param [in] Location The CMOS location to store the checksum.
  @param [in] Checksum The checksum value.
**/
VOID
CmosAccessLibWriteChecksum (
  CMOS_CHECKSUM_LOCATION_INFO *Location,
  IN UINT16                   Checksum
  )
{

  switch (Location->Length) {
  case 0:
    break;
  case 2:
    CmosWrite8 (Location->HighByteAddress, Checksum >> 8);
    //
    // Fall to case 1 to update low byte value
    //
  case 1:
    CmosWrite8 (Location->LowByteAddress, (UINT8) Checksum);
    break;
  }
}

/**
  Read a byte value from a CMOS address.

  @param [in] Address   Location to read from CMOS

  @return The byte value read from the CMOS address.
**/
UINT8
EFIAPI
CmosRead8 (
  IN  UINT8 Address
  )
{
  if (!CmosAccessLibIsAccessible (Address, CmosAccessLibLocateEntry (Address))) {
    return 0xFF;
  }

  if (Address <= CMOS_BANK0_LIMIT) {
    if (PlatformCmosGetNmiState ()) {
      Address |= BIT7;
    }
    IoWrite8 (PORT_70, Address);
    return IoRead8 (PORT_71);
  } else {
    IoWrite8 (PORT_72, Address);
    return IoRead8 (PORT_73);
  }
}

/**
  Write a byte value to a CMOS address.

  It's an internal function that doesn't update the checksum.

  @param [in] Address Location to write to CMOS.
  @param [in] Data    The byte value write to the CMOS address.
**/
VOID
CmosAccessLibICmosWrite8 (
  IN UINT8 Address,
  IN UINT8 Data
  )
{
  if (Address <= CMOS_BANK0_LIMIT) {
    if (PlatformCmosGetNmiState ()) {
      Address |= BIT7;
    }
    IoWrite8 (PORT_70, Address);
    IoWrite8 (PORT_71, Data);
  } else {
    IoWrite8 (PORT_72, Address);
    IoWrite8 (PORT_73, Data);
  }
}

/**
  Write a byte value to a CMOS address.

  @param [in] Address Location to write to CMOS.
  @param [in] Data    The byte value write to the CMOS address.
**/
VOID
EFIAPI
CmosWrite8 (
  IN UINT8 Address,
  IN UINT8 Data
  )
{
  UINT8                       OriginalData;
  CMOS_ENTRY                  *Entry;
  CMOS_CHECKSUM_LOCATION_INFO ChecksumLocation;

  Entry = CmosAccessLibLocateEntry (Address);

  if (!CmosAccessLibIsAccessible (Address, Entry)) {
    return;
  }

  OriginalData = CmosRead8 (Address);

  CmosAccessLibICmosWrite8 (Address, Data);

  if (CmosAccessLibNeedChecksum (Address, Entry)) {
    //
    // Sum of Data + Checksum = New Sum of Data + New Checksum = 0
    // New Sum of Data - Sum of Data = Checksum - New Checksum
    // New Checksum = Checksum - (New Sum of Data - Sum of Data)
    //
    CmosAccessLibGetChecksumLocation (&ChecksumLocation);
    CmosAccessLibWriteChecksum (
      &ChecksumLocation,
      CmosAccessLibReadChecksum (&ChecksumLocation) - (Data - OriginalData)
      );
  }
}

/**
  Read a word value from a CMOS address.

  @param [in] Address   Location to read from CMOS

  @return The word value read from the CMOS address.
**/
UINT16
EFIAPI
CmosRead16 (
  IN  UINT8  Address
  )
{
  return CmosRead8 (Address) + (CmosRead8 (Address + 1) << 8);
}

/**
  Write a word value to a CMOS address.

  @param [in] Address Location to write to CMOS.
  @param [in] Data    The word value write to the CMOS address.
**/
VOID
EFIAPI
CmosWrite16 (
  IN UINT8  Address,
  IN UINT16 Data
  )
{
  CmosWrite8 (Address, (UINT8) Data);
  CmosWrite8 (Address + 1, (UINT8) (Data >> 8));
}

/**
  Read a dword value from a CMOS address.

  @param [in] Address   Location to read from CMOS

  @return The dword value read from the CMOS address.
**/
UINT32
EFIAPI
CmosRead32 (
  IN  UINT8  Address
  )
{
  return CmosRead16 (Address) + (CmosRead16 (Address + 2) << 16);
}

/**
  Write a dword value to a CMOS address.

  @param [in] Address Location to write to CMOS.
  @param [in] Data    The dword value write to the CMOS address.
**/
VOID
EFIAPI
CmosWrite32 (
  IN UINT8  Address,
  IN UINT32 Data
  )
{
  CmosWrite16 (Address, (UINT16) Data);
  CmosWrite16 (Address + 2, (UINT16) (Data >> 16));
}


/**
  Initialize the CMOS.

  It initialize the CMOS area when Force is TRUE or the checksum is incorrect.

  @param[in]  Force  TRUE indicating initializing the CMOS area without checking the checksum.

  @retval TRUE  The CMOS is initialized to default value.
  @retval FALSE The CMOS isn't initialized to default value.
**/
BOOLEAN
EFIAPI
CmosInit (
  IN  BOOLEAN     Force
  )
{
  UINTN                       Address;
  CMOS_ENTRY                  *Entry;
  CMOS_CHECKSUM_LOCATION_INFO ChecksumLocation;
  UINT16                      Checksum;

  CmosAccessLibGetChecksumLocation (&ChecksumLocation);

  if (!Force) {
    //
    // Initialize the CMOS area when checksum is incorrect.
    //
    Checksum = CmosAccessLibCalculateSum (&ChecksumLocation) + CmosAccessLibReadChecksum (&ChecksumLocation);
    if (ChecksumLocation.Length == 1) {
      Checksum = (UINT8) Checksum;
    }

    if (Checksum != 0) {
      Force = TRUE;
    }
  }

  if (Force) {
    //
    // Traverse through entire CMOS location and fill it with zero
    //
    for (Address = 0; Address <= CMOS_BANK1_LIMIT; Address++) {
      Entry = CmosAccessLibLocateEntry ((UINT8) Address);
      if (CmosAccessLibNeedFillDefault ((UINT8) Address, Entry)) {
        CmosAccessLibICmosWrite8 ((UINT8) Address, (Entry == NULL) ? CMOS_DEFAULT_VALUE : Entry->DefaultValue);
      }
    }

    //
    // Write the New checksum to the Checksum field
    //
    CmosAccessLibWriteChecksum (
      &ChecksumLocation,
      (UINT16) (0x10000 - CmosAccessLibCalculateSum (&ChecksumLocation))
      );
    return TRUE;
  }

  return FALSE;
}
