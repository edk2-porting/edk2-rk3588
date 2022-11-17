/** @file
  uba central config database PPI

  @copyright
  Copyright 2012 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _UBA_CONFIG_DATABASE_PPI_H_
#define _UBA_CONFIG_DATABASE_PPI_H_

// {C1176733-159F-42d5-BCB9-320660B17310}
#define UBA_CONFIG_DATABASE_PPI_GUID \
  { 0xc1176733, 0x159f, 0x42d5, { 0xbc, 0xb9, 0x32, 0x6, 0x60, 0xb1, 0x73, 0x10 } }

typedef struct _UBA_CONFIG_DATABASE_PPI UBA_CONFIG_DATABASE_PPI;

#define UBA_CONFIG_PPI_VERSION    01
#define UBA_CONFIG_PPI_SIGNATURE  SIGNATURE_32('U', 'S', 'K', 'U')

//
// Functions
//

/**
  Set board's GUID and user friendly name by BoardId.

  If the BoardId is not exist in database, it will create a new platform.

  @param This                   uba Ppi instance.
  @param BoardId           The platform type, same define as Platform.h.
  @param BoardGuid             The GUID for this platform.
  @param BoardName           The user friendly name for this platform.

  @retval EFI_ALREADY_STARTED   Create new for an exist platform.
  @retval EFI_OUT_OF_RESOURCES  Resource not enough.
  @retval EFI_NOT_FOUND         Platform not found.
  @retval EFI_SUCCESS           Operation success.

**/
typedef
EFI_STATUS
(EFIAPI *PEI_UBA_CONFIG_INIT_BOARD) (
  IN  UBA_CONFIG_DATABASE_PPI             *This,
  IN  UINT32                              BoardId,
  IN  EFI_GUID                            *BoardGuid,    OPTIONAL
  IN  CHAR8                               *BoardName     OPTIONAL
  );

/**
  Get board's GUID and user friendly name by BoardId.

  This is used when you need a BoardGuid to Add/Get platform data

  Core will create a new platform for you if the BoardId is not
  recorded in database, and assgin a unique GUID for this platform.

  @param This                   uba Ppi instance.
  @param BoardId           The platform type, same define as Platform.h.
  @param BoardGuid             The GUID for this platform.
  @param BoardName           The user friendly name for this platform.

  @retval EFI_ALREADY_STARTED   Create new for an exist platform.
  @retval EFI_OUT_OF_RESOURCES  Resource not enough.
  @retval EFI_NOT_FOUND         Platform not found.
  @retval EFI_SUCCESS           Operation success.

**/
typedef
EFI_STATUS
(EFIAPI *PEI_UBA_CONFIG_GET_BOARD) (
  IN  UBA_CONFIG_DATABASE_PPI             *This,
  OUT  UINT32                             *BoardId,
  OUT EFI_GUID                            *BoardGuid,    OPTIONAL
  OUT CHAR8                               *BoardName     OPTIONAL
  );

/**
  Add configuration data to uba configuration database.

  @param This                   uba Ppi instance.
  @param BoardGuid             The GUID for this platform.
  @param ResId                  The configuration data resource id.
  @param Data                   The data buffer pointer.
  @param DataSize               Size of data want to add into database.

  @retval EFI_INVALID_PARAMETER Required parameters not correct.
  @retval EFI_OUT_OF_RESOURCES  Resource not enough.
  @retval EFI_NOT_FOUND         Platform not found.
  @retval EFI_SUCCESS           Operation success.

**/
typedef
EFI_STATUS
(EFIAPI *PEI_UBA_CONFIG_ADD_DATA) (
  IN  UBA_CONFIG_DATABASE_PPI         *This,
  IN  EFI_GUID                        *ResId,
  IN  VOID                            *Data,
  IN  UINTN                           DataSize
  );

/**
  Get configuration data from uba configuration database.

  @param This                   uba Ppi instance.
  @param ResId                  The configuration data resource id.
  @param Data                   The data buffer pointer.
  @param DataSize               IN:Size of data want to get, OUT: Size of data in database.

  @retval EFI_INVALID_PARAMETER Required parameters not correct.
  @retval EFI_BUFFER_TOO_SMALL  The DataSize of Data buffer is too small to get this configuration data
  @retval EFI_OUT_OF_RESOURCES  Resource not enough.
  @retval EFI_NOT_FOUND         Platform or data not found.
  @retval EFI_SUCCESS           Operation success.

**/
typedef
EFI_STATUS
(EFIAPI *PEI_UBA_CONFIG_GET_DATA) (
  IN  UBA_CONFIG_DATABASE_PPI         *This,
  IN  EFI_GUID                        *ResId,
  OUT VOID                            *Data,
  IN  OUT UINTN                       *DataSize
  );


//
// Multi Sku config database PPI
//
struct _UBA_CONFIG_DATABASE_PPI {
  UINT32                             Signature;
  UINT32                             Version;

  PEI_UBA_CONFIG_INIT_BOARD          InitSku;
  PEI_UBA_CONFIG_GET_BOARD           GetSku;

  PEI_UBA_CONFIG_ADD_DATA            AddData;
  PEI_UBA_CONFIG_GET_DATA            GetData;
};

extern EFI_GUID gUbaConfigDatabasePpiGuid;

#endif // _UBA_CONFIG_DATABASE_PPI_H_
