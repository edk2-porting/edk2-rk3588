/** @file
  Library functions for SetupLib.
  This library instance provides methods to access Setup option.

  @copyright
  Copyright 2020 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi/UefiBaseType.h>
#include <SetupTable.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>

/**
  This function provides a means by which to retrieve a value for a given option.

  Returns the data, data type and data size specified by OptionNumber and Guid.

  @param[in]  Guid          Pointer to a 128-bit unique value that designates
                            which namespace to retrieve a value from.
  @param[in]  OptionNumber  The option number to retrieve a current value for.
  @param[in, out] Data      A pointer to the buffer to be retrieved.
  @param[in, out] DataSize  The size, in bytes, of Buffer.

  @retval EFI_SUCCESS           Data is successfully reterieved.
  @retval EFI_INVALID_PARAMETER Guid is NULL or DataSize is NULL or OptionNumber is invalid.
  @retval EFI_BUFFER_TOO_SMALL  Input data buffer is not enough.
  @retval EFI_NOT_FOUND         The given option is not found.

**/
EFI_STATUS
EFIAPI
GetOptionData (
  IN EFI_GUID            *Guid,
  IN UINTN               OptionNumber,
  IN OUT VOID            *Data,
  IN UINTN               DataSize
  )
{
  SETUP_DATA  *SetupData = NULL;

  if (Guid == NULL || DataSize == 0) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Retrieve the variable from PCD
  //
  if (CompareGuid (&gEfiSocketIioVariableGuid, Guid)) {
    SetupData = (SETUP_DATA*) PcdGetPtr (PcdSocketIioConfig);
  } else if (CompareGuid (&gEfiSocketCommonRcVariableGuid, Guid)) {
    SetupData = (SETUP_DATA*) PcdGetPtr (PcdSocketCommonRcConfig);
  } else if (CompareGuid (&gEfiSocketMpLinkVariableGuid, Guid)) {
    SetupData = (SETUP_DATA*) PcdGetPtr (PcdSocketMpLinkConfig);
  } else if (CompareGuid (&gEfiSocketMemoryVariableGuid, Guid)) {
    SetupData = (SETUP_DATA*) PcdGetPtr (PcdSocketMemoryConfig);
  } else if (CompareGuid (&gEfiSocketPowermanagementVarGuid, Guid)) {
    SetupData = (SETUP_DATA*) PcdGetPtr (PcdSocketPowerManagementConfig);
  } else if (CompareGuid (&gEfiSocketProcessorCoreVarGuid, Guid)) {
    SetupData = (SETUP_DATA*) PcdGetPtr (PcdSocketProcessorCoreConfig);
  } else if (CompareGuid (&gEfiSetupVariableGuid, Guid)) {
    SetupData = (SETUP_DATA*) PcdGetPtr (PcdSetup);
  } else if (CompareGuid (&gPchSetupVariableGuid, Guid)) {
    SetupData = (SETUP_DATA*) PcdGetPtr (PcdPchSetup);
  } else if (CompareGuid (&gMemBootHealthGuid, Guid)) {
    SetupData = (SETUP_DATA*) PcdGetPtr (PcdMemBootHealthConfig);
  }

  //
  // Grab the data from the offset
  //
  if (SetupData != NULL) {
    CopyMem (Data, (UINT8*) SetupData + OptionNumber, DataSize);
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_ERROR, "ERROR: Unknown GetOptionData requested\n"));
  ASSERT_EFI_ERROR (EFI_NOT_FOUND);
  return EFI_NOT_FOUND;
}

/**
  This function provides a means by which to set a value for a given option number.

  Sets a buffer for the token specified by OptionNumber to the value specified by
  Data and DataSize.
  If DataSize is greater than the maximum size support by OptionNumber,
  then set DataSize to the maximum size supported by OptionNumber.

  @param[in]  Guid              Pointer to a 128-bit unique value that
                                designates which namespace to set a value from.
  @param[in]  OptionNumber      The option number to set a current value for.
  @param[in]  Data              A pointer to the buffer to set.
  @param[in, out] DataSize      The size, in bytes, of Buffer.

  @retval EFI_SUCCESS           Data is successfully updated.
  @retval EFI_INVALID_PARAMETER OptionNumber is invalid, Guid is NULL, or Data is NULL, or DataSize is NULL.
  @retval EFI_NOT_FOUND         The given option is not found.
  @retval EFI_UNSUPPORTED       Set action is not supported.
**/
EFI_STATUS
EFIAPI
SetOptionData (
  IN EFI_GUID *Guid,
  IN UINTN    OptionNumber,
  IN VOID     *Data,
  IN UINTN    DataSize
  )
{
  DEBUG ((DEBUG_ERROR, "ERROR: SetOptionData not implemented\n"));
  ASSERT_EFI_ERROR (EFI_NOT_FOUND);
  return EFI_NOT_FOUND;
}

/**
  Get all data in the setup

  @retval EFI_SUCCESS           Data is committed successfully.
  @retval EFI_INVALID_PARAMETER Guid is NULL.
  @retval EFI_NOT_FOUND         Guid is not found.
  @retval EFI_DEVICE_ERROR      Data can't be committed.
**/
EFI_STATUS
EFIAPI
GetEntireConfig (
  OUT SETUP_DATA *SetupData
  )
{
  ZeroMem(SetupData, sizeof(SETUP_DATA) );

  CopyMem (&SetupData->SocketConfig.IioConfig, PcdGetPtr (PcdSocketIioConfig), sizeof(SOCKET_IIO_CONFIGURATION));
  CopyMem (&SetupData->SocketConfig.CommonRcConfig, PcdGetPtr (PcdSocketCommonRcConfig), sizeof(SOCKET_COMMONRC_CONFIGURATION));
  CopyMem (&SetupData->SocketConfig.UpiConfig, PcdGetPtr (PcdSocketMpLinkConfig), sizeof(SOCKET_MP_LINK_CONFIGURATION));
  CopyMem (&SetupData->SocketConfig.MemoryConfig, PcdGetPtr (PcdSocketMemoryConfig), sizeof(SOCKET_MEMORY_CONFIGURATION));
  CopyMem (&SetupData->SocketConfig.PowerManagementConfig, PcdGetPtr (PcdSocketPowerManagementConfig), sizeof(SOCKET_POWERMANAGEMENT_CONFIGURATION));
  CopyMem (&SetupData->SocketConfig.SocketProcessorCoreConfiguration, PcdGetPtr (PcdSocketProcessorCoreConfig), sizeof(SOCKET_PROCESSORCORE_CONFIGURATION));
  CopyMem (&SetupData->SystemConfig, PcdGetPtr (PcdSetup), sizeof(SYSTEM_CONFIGURATION));
  CopyMem (&SetupData->PchSetup, PcdGetPtr (PcdPchSetup), sizeof(PCH_SETUP));
  CopyMem (&SetupData->MemBootHealthConfig, PcdGetPtr (PcdMemBootHealthConfig), sizeof(MEM_BOOT_HEALTH_CONFIG));
  return EFI_SUCCESS;
}


/**
  Set all data in the setup

  @retval EFI_SUCCESS           Data is committed successfully.
  @retval EFI_INVALID_PARAMETER Guid is NULL.
  @retval EFI_NOT_FOUND         Guid is not found.
  @retval EFI_DEVICE_ERROR      Data can't be committed.
**/
EFI_STATUS
EFIAPI
SetEntireConfig (
  IN SETUP_DATA *SetupData
  )
{
  DEBUG ((DEBUG_ERROR, "ERROR: SetEntireConfig not implemented\n"));
  ASSERT_EFI_ERROR (EFI_NOT_FOUND);
  return EFI_NOT_FOUND;
}

/**
  This function provides a means by which to retrieve a value for a given option.

  Returns the data, data type and data size specified by OptionNumber and Guid.

  @param[in]  Guid          Pointer to a 128-bit unique value that designates
                            which namespace to retrieve a value from.
  @param[in]  Variable      Pointer to data location where variable is stored.

  @retval EFI_SUCCESS           Data is successfully retrieved.
  @retval EFI_INVALID_PARAMETER Guid or Variable is null.
  @retval EFI_NOT_FOUND         The given option is not found.

**/
EFI_STATUS
EFIAPI
GetSpecificConfigGuid (
  IN EFI_GUID            *Guid,
  OUT VOID               *Variable
  )
{
  if ((Guid == NULL) || (Variable == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (CompareGuid (&gEfiSocketIioVariableGuid, Guid)) {
    CopyMem (Variable, PcdGetPtr (PcdSocketIioConfig), sizeof(SOCKET_IIO_CONFIGURATION));
    return EFI_SUCCESS;
  } else if (CompareGuid (&gEfiSocketCommonRcVariableGuid, Guid)) {
    CopyMem (Variable, PcdGetPtr (PcdSocketCommonRcConfig), sizeof(SOCKET_COMMONRC_CONFIGURATION));
    return EFI_SUCCESS;
  } else if (CompareGuid (&gEfiSocketMpLinkVariableGuid, Guid)) {
    CopyMem (Variable, PcdGetPtr (PcdSocketMpLinkConfig), sizeof(SOCKET_MP_LINK_CONFIGURATION));
    return EFI_SUCCESS;
  } else if (CompareGuid (&gEfiSocketMemoryVariableGuid, Guid)) {
    CopyMem (Variable, PcdGetPtr (PcdSocketMemoryConfig), sizeof(SOCKET_MEMORY_CONFIGURATION));
    return EFI_SUCCESS;
  } else if (CompareGuid (&gEfiSocketPowermanagementVarGuid, Guid)) {
    CopyMem (Variable, PcdGetPtr (PcdSocketPowerManagementConfig), sizeof(SOCKET_POWERMANAGEMENT_CONFIGURATION));
    return EFI_SUCCESS;
  } else if (CompareGuid (&gEfiSocketProcessorCoreVarGuid, Guid)) {
    CopyMem (Variable, PcdGetPtr (PcdSocketProcessorCoreConfig), sizeof(SOCKET_PROCESSORCORE_CONFIGURATION));
    return EFI_SUCCESS;
  } else if (CompareGuid (&gEfiSetupVariableGuid, Guid)) {
    CopyMem (Variable, PcdGetPtr (PcdSetup), sizeof(SYSTEM_CONFIGURATION));
    return EFI_SUCCESS;
  } else if (CompareGuid (&gPchSetupVariableGuid, Guid)) {
    CopyMem (Variable, PcdGetPtr (PcdPchSetup), sizeof(PCH_SETUP));
    return EFI_SUCCESS;
  } else if (CompareGuid (&gMemBootHealthGuid, Guid)) {
    CopyMem (Variable, PcdGetPtr (PcdMemBootHealthConfig), sizeof(MEM_BOOT_HEALTH_CONFIG));
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_ERROR, "ERROR: Unknown GetSpecificConfigGuid requested\n"));
  ASSERT_EFI_ERROR (EFI_NOT_FOUND);
  return EFI_NOT_FOUND;
}

/**
  This function provides a means by which to set a value for a given option number.

  Sets a buffer for the token specified by OptionNumber to the value specified by
  Data and DataSize.
  If DataSize is greater than the maximum size support by OptionNumber,
  then set DataSize to the maximum size supported by OptionNumber.

  @param[in]  Guid              Pointer to a 128-bit unique value that
                                designates which namespace to set a value from.
  @param[in]  Variable      Pointer to data location where variable is stored.

  @retval EFI_SUCCESS           Data is successfully updated.
  @retval EFI_INVALID_PARAMETER OptionNumber is invalid, Guid is NULL, or Data is NULL, or DataSize is NULL.
  @retval EFI_NOT_FOUND         The given option is not found.
  @retval EFI_UNSUPPORTED       Set action is not supported.
**/
EFI_STATUS
EFIAPI
SetSpecificConfigGuid (
  IN EFI_GUID *Guid,
  IN VOID *Variable
  )
{
  DEBUG ((DEBUG_ERROR, "ERROR: SetSpecificConfigGuid not implemented\n"));
  ASSERT_EFI_ERROR (EFI_NOT_FOUND);
  return EFI_NOT_FOUND;
}
