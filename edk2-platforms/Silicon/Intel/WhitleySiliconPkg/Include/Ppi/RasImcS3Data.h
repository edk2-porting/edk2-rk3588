/** @file
  RAS IMC S3 Data Load PPI

  @copyright
  Copyright 2021 Intel Corporation.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _RAS_IMC_S3_DATA_H_
#define _RAS_IMC_S3_DATA_H_

#include <Uefi/UefiBaseType.h>

typedef struct _RAS_IMC_S3_DATA_PPI RAS_IMC_S3_DATA_PPI;

/**
  Retrieves data for S3 saved memory RAS features from non-volatile storage.

  If the Data buffer is too small to hold the contents of the NVS data,
  the error EFI_BUFFER_TOO_SMALL is returned and DataSize is set to the
  required buffer size to obtain the data.

  @param[in]       This                   A pointer to this instance of the RAS_IMC_S3_DATA_PPI.
  @param[in, out]  DataSize               On entry, points to the size in bytes of the Data buffer.
                                          On return, points to the size of the data returned in Data.
  @param[out]      Data                   Points to the buffer which will hold the returned data.

  @retval          EFI_SUCCESS            The NVS data was read successfully.
  @retval          EFI_NOT_FOUND          The NVS data does not exist.
  @retval          EFI_BUFFER_TOO_SMALL   The DataSize is too small for the NVS data.
                                          DataSize is updated with the size required for
                                          the NVS data.
  @retval          EFI_INVALID_PARAMETER  DataSize or Data is NULL.
  @retval          EFI_DEVICE_ERROR       The NVS data could not be retrieved because of a device error.
  @retval          EFI_UNSUPPORTED        This platform does not support the save/restore of S3 memory data

**/
typedef
EFI_STATUS
(EFIAPI *RAS_IMC_S3_DATA_PPI_GET_IMC_S3_RAS_DATA) (
  IN CONST  RAS_IMC_S3_DATA_PPI             *This,
  IN OUT    UINT32                          *DataSize,
  OUT       VOID                            *Data
  );

/**
 RAS IMC S3 Data PPI
**/
struct _RAS_IMC_S3_DATA_PPI {
  /**
    Retrieves data for S3 saved memory RAS features from non-volatile storage.
  **/
  RAS_IMC_S3_DATA_PPI_GET_IMC_S3_RAS_DATA  GetImcS3RasData;
};

extern EFI_GUID gRasImcS3DataPpiGuid;

#endif // _RAS_IMC_S3_DATA_H_
