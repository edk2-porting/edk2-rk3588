/*
#  Copyright (c) 2014-2018, Linaro Limited. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
*/
#include <RdkBootManagerLib.h>

typedef enum KEY
{
  PK_KEY=1,
  KEK_KEY,
  DB_KEY,
  DBX_KEY
} eKey;
/* ************************** */

STATIC
EFI_STATUS
OpenFileByDevicePath(
  IN OUT  EFI_DEVICE_PATH_PROTOCOL  **FilePath,
  OUT     EFI_FILE_HANDLE           *FileHandle,
  IN      UINT64                    OpenMode,
  IN      UINT64                    Attributes
)
{
  EFI_STATUS                        Status;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL   *EfiSimpleFileSystemProtocol;
  EFI_FILE_PROTOCOL                 *Handle1;
  EFI_FILE_PROTOCOL                 *Handle2;
  EFI_HANDLE                        DeviceHandle;

  if (FilePath == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateDevicePath (
    &gEfiSimpleFileSystemProtocolGuid,
    FilePath,
    &DeviceHandle
    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->OpenProtocol (
    DeviceHandle,
    &gEfiSimpleFileSystemProtocolGuid,
    (VOID**)&EfiSimpleFileSystemProtocol,
    gImageHandle,
    NULL,
    EFI_OPEN_PROTOCOL_GET_PROTOCOL
    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = EfiSimpleFileSystemProtocol->OpenVolume (\
    EfiSimpleFileSystemProtocol, &Handle1);
  if (EFI_ERROR (Status)) {
    FileHandle = NULL;
    return Status;
  }

  //
  // go down directories one node at a time.
  //
  while (!IsDevicePathEnd (*FilePath)) {
    //
    // For file system access each node should be a file path component
    //
    if (DevicePathType (*FilePath) != MEDIA_DEVICE_PATH ||
      DevicePathSubType (*FilePath) != MEDIA_FILEPATH_DP
      ) {
      FileHandle = NULL;
      return (EFI_INVALID_PARAMETER);
    }
    //
    // Open this file path node
    //
    Handle2  = Handle1;
    Handle1 = NULL;

    //
    // Try to test opening an existing file
    //
    Status = Handle2->Open (
      Handle2,
      &Handle1,
      ((FILEPATH_DEVICE_PATH*)*FilePath)->PathName,
      OpenMode &~EFI_FILE_MODE_CREATE,
      0
      );

    //
    // see if the error was that it needs to be created
    //
    if ((EFI_ERROR (Status)) &&
      (OpenMode != (OpenMode &~EFI_FILE_MODE_CREATE))) {
      Status = Handle2->Open (
      Handle2,
      &Handle1,
      ((FILEPATH_DEVICE_PATH*)*FilePath)->PathName,
      OpenMode,
      Attributes
      );
    }
    //
    // Close the last node
    //
    Handle2->Close (Handle2);

    if (EFI_ERROR (Status)) {
        return (Status);
    }

    //
    // Get the next node
    //
    *FilePath = NextDevicePathNode (*FilePath);
  }

  //
  // This is a weak spot since if the undefined
  // SHELL_FILE_HANDLE format changes this must change also!
  //
  *FileHandle = (VOID*)Handle1;

  return EFI_SUCCESS;
}

EFI_STATUS
GetFileHandler (
  OUT EFI_FILE_HANDLE *FileHandle,
  IN  CONST CHAR16    *Path,
  IN  UINT64          OpenMode
)
{
  EFI_STATUS                          Status;
  EFI_DEVICE_PATH_PROTOCOL            *KeyFileDevicePath;
  EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL  *DevicePathFromTextProtocol;

  Status        = EFI_SUCCESS;
  KeyFileDevicePath   = NULL;

  Status = gBS->LocateProtocol (
               &gEfiDevicePathFromTextProtocolGuid,
               NULL,
               (VOID**)&DevicePathFromTextProtocol
           );
  ASSERT_EFI_ERROR (Status);

  KeyFileDevicePath = DevicePathFromTextProtocol->ConvertTextToDevicePath (Path);
  if (KeyFileDevicePath != NULL)
  {
    Status = OpenFileByDevicePath (&KeyFileDevicePath, FileHandle, OpenMode, 0);
    if (Status != EFI_SUCCESS)
    {
      DEBUG ((DEBUG_ERROR, "Getting FileHandle of %s Failed\n",Path));
    }
  }
  return Status;
}

STATIC
EFI_STATUS
CreateTimeBasedPayload (
  IN OUT UINTN  *DataSize,
  IN OUT UINT8  **Data
)
{
  EFI_STATUS                       Status;
  UINT8                            *NewData;
  UINT8                            *Payload;
  UINTN                            PayloadSize;
  EFI_VARIABLE_AUTHENTICATION_2    *DescriptorData;
  UINTN                            DescriptorSize;
  EFI_TIME                         Time;

  if (Data == NULL || DataSize == NULL) {
      return EFI_INVALID_PARAMETER;
  }

  //
  // In Setup mode or Custom mode, the variable does not need to be signed but the
  // parameters to the SetVariable() call still need to be prepared as authenticated
  // variable. So we create EFI_VARIABLE_AUTHENTICATED_2 descriptor without certificate
  // data in it.
  //

  Payload     = *Data;
  PayloadSize = *DataSize;

  DescriptorSize = OFFSET_OF (EFI_VARIABLE_AUTHENTICATION_2, AuthInfo) + \
    OFFSET_OF (WIN_CERTIFICATE_UEFI_GUID, CertData);
  NewData = (UINT8*) AllocateZeroPool (DescriptorSize + PayloadSize);
  if (NewData == NULL) {
      return EFI_OUT_OF_RESOURCES;
  }

  if ((Payload != NULL) && (PayloadSize != 0)) {
      CopyMem (NewData + DescriptorSize, Payload, PayloadSize);
  }

  DescriptorData = (EFI_VARIABLE_AUTHENTICATION_2 *) (NewData);

  ZeroMem (&Time, sizeof (EFI_TIME));
  Status = gRT->GetTime (&Time, NULL);
  if (EFI_ERROR (Status)) {
    FreePool(NewData);
    return Status;
  }
  Time.Pad1       = 0;
  Time.Nanosecond = 0;
  Time.TimeZone   = 0;
  Time.Daylight   = 0;
  Time.Pad2       = 0;
  CopyMem (&DescriptorData->TimeStamp, &Time, sizeof (EFI_TIME));

  DescriptorData->AuthInfo.Hdr.dwLength         = \
    OFFSET_OF (WIN_CERTIFICATE_UEFI_GUID, CertData);
  DescriptorData->AuthInfo.Hdr.wRevision        = 0x0200;
  DescriptorData->AuthInfo.Hdr.wCertificateType = WIN_CERT_TYPE_EFI_GUID;
  CopyGuid (&DescriptorData->AuthInfo.CertType, &gEfiCertPkcs7Guid);

  if (Payload != NULL) {
    FreePool(Payload);
  }

  *DataSize = DescriptorSize + PayloadSize;
  *Data     = NewData;
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
SetBootMode (
  IN UINT8  SecureBootMode
)
{
  return gRT->SetVariable (
    EFI_CUSTOM_MODE_NAME,
    &gEfiCustomModeEnableGuid,
    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
    sizeof (UINT8),
    &SecureBootMode
  );
}

STATIC
EFI_STATUS
SetVariable (
  IN EFI_SIGNATURE_LIST *PkCert,
  IN UINTN              DataSize,
  IN eKey               KeyType
)
{
  UINT32       Attr;
  EFI_STATUS   Status;

  Status = EFI_SUCCESS;
  Attr = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS |
    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS;
  if (KeyType == PK_KEY)
  {
    DEBUG ((DEBUG_INFO, "Setting PK Key\n"));
    Status = gRT->SetVariable (
      EFI_PLATFORM_KEY_NAME,
      &gEfiGlobalVariableGuid,
      Attr,
      DataSize,
      PkCert
    );
  }
  else if (KeyType == KEK_KEY)
  {
    DEBUG ((DEBUG_INFO, "Setting KEK Key\n"));
    Status = gRT->SetVariable (
      EFI_KEY_EXCHANGE_KEY_NAME,
      &gEfiGlobalVariableGuid,
      Attr,
      DataSize,
      PkCert
    );

    Status = gRT->SetVariable (
      EFI_IMAGE_SECURITY_DATABASE,
      &gEfiImageSecurityDatabaseGuid,
      Attr,
      DataSize,
      PkCert
    );
  }
  else
  {
    ASSERT(FALSE);
  }
  return Status;
}

STATIC
VOID
PopulateCert (
  OUT EFI_SIGNATURE_LIST  **Cert,
  IN  UINTN               DataSize,
  IN  UINT8               *Data
)
{
  EFI_SIGNATURE_DATA  *CertData = NULL;

  if ((*Cert) == NULL)
  {
    (*Cert) = (EFI_SIGNATURE_LIST*) AllocateZeroPool (sizeof (EFI_SIGNATURE_LIST)
      + sizeof(EFI_SIGNATURE_DATA) - 1
      + DataSize );

    ASSERT ((*Cert) != NULL);
  }
  (*Cert)->SignatureListSize   = (UINT32) (sizeof (EFI_SIGNATURE_LIST)
    + sizeof (EFI_SIGNATURE_DATA) - 1
    + DataSize);
  (*Cert)->SignatureSize       = (UINT32) (sizeof (EFI_SIGNATURE_DATA) - 1 + DataSize);
  (*Cert)->SignatureHeaderSize = 0;
  CopyGuid (&(*Cert)->SignatureType, &gEfiCertX509Guid);


  CertData = (EFI_SIGNATURE_DATA*) ((UINTN)(*Cert) + \
    sizeof(EFI_SIGNATURE_LIST) + (*Cert)->SignatureHeaderSize);
  ASSERT (CertData != NULL);

  CopyGuid (&CertData->SignatureOwner, &gEfiGlobalVariableGuid);
  CopyMem (&CertData->SignatureData, Data, DataSize);
}

STATIC
EFI_STATUS
RegisterCert (
  IN  UINT8   *KeyData,
  IN  UINTN   KeySize,
  IN  eKey    KeyType
)
{
  EFI_STATUS          Status;
  EFI_SIGNATURE_LIST  *Cert;

  Cert = NULL;

  Status = SetBootMode (CUSTOM_SECURE_BOOT_MODE);
  ASSERT_EFI_ERROR (Status);

  PopulateCert (&Cert, KeySize, KeyData);

  KeySize = Cert->SignatureListSize;

  Status = CreateTimeBasedPayload (&KeySize, (UINT8**) &Cert);
  ASSERT_EFI_ERROR (Status);

  Status = SetVariable (Cert, KeySize, KeyType);
  return Status;
}

STATIC
VOID
RdkSetVariable (
  VOID
)
{
  CONST CHAR16	*KeyPath;
  EFI_STATUS  	Status;
  UINT8         *KekKey,    *PkKey,    *KekCrtData;
  UINTN         KekKeySize, PkKeySize, KekCrtSize;
  INT8*         SetupMode;
  eKey          KeyType;

  KeyPath    = NULL;
  SetupMode  = NULL;
  KekKey     = PkKey     = KekCrtData = NULL;
  KekKeySize = PkKeySize = KekCrtSize = 0;

  Status = GetRdkVariable (L"ROOTCERT", &KeyPath);

  //set only if the Kek Crt file mentioned in the configuration file
  if (KeyPath != NULL) {
    Status = RdkReadFile (
      KeyPath,
      (VOID **)&KekCrtData,
      &KekCrtSize
      );
    ASSERT_EFI_ERROR (Status);

    Status = gRT->SetVariable (
      L"RdkRootCertificate",
      &gRdkGlobalVariableGuid,
      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
      KekCrtSize,
      KekCrtData
      );
    ASSERT_EFI_ERROR (Status);

    if (KekCrtData) FreePool (KekCrtData);
  }

  Status = GetRdkVariable (L"KEKCERT", &KeyPath);
  ASSERT_EFI_ERROR (Status);

  Status = RdkReadFile (
    KeyPath,
    (VOID **)&KekKey,
    &KekKeySize
    );
  ASSERT_EFI_ERROR (Status);

  Status = GetRdkVariable (L"PKCERT", &KeyPath);
  ASSERT_EFI_ERROR (Status);

  Status = RdkReadFile (
    KeyPath,
    (VOID **)&PkKey,
    &PkKeySize
    );
  ASSERT_EFI_ERROR (Status);

  KeyType = PK_KEY;
  Status = RegisterCert (PkKey, PkKeySize, KeyType);
  GetEfiGlobalVariable2 (L"SetupMode", (VOID**)&SetupMode, NULL);

  if (*SetupMode == 0)
  {
    DEBUG ((DEBUG_INFO, "PK Key Got Registered. Now System in User Mode\n"));
    KeyType = KEK_KEY;
    Status = RegisterCert (KekKey, KekKeySize, KeyType);
  }
  else if(*SetupMode == 1)
  {
    DEBUG ((DEBUG_INFO, "System in Standard System Mode ::: Secure Boot Not enabled\n"));
    ASSERT_EFI_ERROR (Status);
  }

  if ( PkKey ) FreePool (PkKey);
  if ( KekKey ) FreePool (KekKey);
}

EFI_STATUS
RdkSecureBoot (
  EFI_HANDLE        ImageHandle,
  EFI_BOOT_SERVICES *BootServices
)
{
  UINTN                               ExitDataSize;
  CHAR16                              *ExitData;
  CHAR16                      	      LoadOption[128];
  CONST CHAR8		              *CmdLine;
  CHAR16	                      *ImagePath;
  UINT8                               *FdtData;
  CONST CHAR16                        *DtbPath;
  EFI_STATUS                          Status;
  EFI_HANDLE                          Handle;
  EFI_DEVICE_PATH_PROTOCOL            *FilePath;
  EFI_LOADED_IMAGE_PROTOCOL           *ImageInfo;
  EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL  *DevicePathFromTextProtocol;

  FilePath      = NULL;
  ExitData      = NULL;
  CmdLine	= (CONST CHAR8 *)FixedPcdGetPtr (PcdRdkCmdLineArgs);

  if (FixedPcdGetBool (PcdDtbAvailable)) {
    FdtData = NULL;
    DtbPath = NULL;

    Status = GetRdkVariable (L"DTB", &DtbPath);
    ASSERT_EFI_ERROR (Status);

    Status = RdkReadFile (DtbPath, (VOID**) &FdtData, NULL);
    ASSERT_EFI_ERROR (Status);

    Status = gBS->InstallConfigurationTable (&gFdtTableGuid,(VOID*)FdtData);
    ASSERT_EFI_ERROR (Status);
  }

  RdkSetVariable ();

  Status = GetRdkVariable (L"IMAGE", (CONST CHAR16**)&ImagePath);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (
    &gEfiDevicePathFromTextProtocolGuid,
    NULL,
    (VOID**)&DevicePathFromTextProtocol
    );
  ASSERT_EFI_ERROR (Status);

  FilePath = DevicePathFromTextProtocol->ConvertTextToDevicePath (ImagePath);
  ASSERT(NULL != FilePath);

  Status = BootServices->LoadImage (
    TRUE,
    ImageHandle,
    FilePath,
    NULL,
    0,
    &Handle
    );
  ASSERT_EFI_ERROR (Status);

  UnicodeSPrintAsciiFormat (LoadOption, sizeof (LoadOption), CmdLine);

  Status = BootServices->HandleProtocol (\
    Handle, &gEfiLoadedImageProtocolGuid, (VOID **)&ImageInfo);
  ASSERT_EFI_ERROR (Status);
  ImageInfo->LoadOptionsSize  = sizeof (LoadOption);
  ImageInfo->LoadOptions      = LoadOption;

  Status = BootServices->StartImage (Handle, &ExitDataSize, &ExitData);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
