/** @file
  EDKII Device Security library for PCI device.
  It follows the Intel PCIe Security Specification.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <IndustryStandard/Spdm.h>
#include <IndustryStandard/IntelPciSecurity.h>
#include <IndustryStandard/Pci.h>
#include <IndustryStandard/Tpm20.h>
#include <IndustryStandard/UefiTcgPlatform.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/TpmMeasurementLib.h>
#include <Protocol/PciIo.h>
#include <Protocol/DeviceSecurity.h>
#include <Protocol/PlatformDeviceSecurityPolicy.h>

typedef struct {
  TCG_DEVICE_SECURITY_EVENT_DATA_HEADER         EventData;
  SPDM_MEASUREMENT_BLOCK_COMMON_HEADER          CommonHeader;
  SPDM_MEASUREMENT_BLOCK_DMTF_HEADER            DmtfHeader;
  UINT8                                         Digest[SHA256_DIGEST_SIZE];
  TCG_DEVICE_SECURITY_EVENT_DATA_PCI_CONTEXT    PciContext;
} EDKII_DEVICE_SECURITY_PCI_EVENT_DATA;

typedef struct {
  UINTN         Signature;
  LIST_ENTRY    Link;
  UINTN         PciSegment;
  UINTN         PciBus;
  UINTN         PciDevice;
  UINTN         PciFunction;
} PCI_DEVICE_INSTANCE;

#define PCI_DEVICE_INSTANCE_SIGNATURE  SIGNATURE_32 ('P', 'D', 'I', 'S')
#define PCI_DEVICE_INSTANCE_FROM_LINK(a)  CR (a, PCI_DEVICE_INSTANCE, Link, PCI_DEVICE_INSTANCE_SIGNATURE)

LIST_ENTRY mSecurityEventMeasurementDeviceList = INITIALIZE_LIST_HEAD_VARIABLE(mSecurityEventMeasurementDeviceList);;
EDKII_DEVICE_SECURITY_POLICY_PROTOCOL  *mDeviceSecurityPolicy;

/**
  Record a PCI device into device list.

  @param PciIo            PciIo instance of the device
  @param PciDeviceList    The list to record the the device
**/
VOID
RecordPciDeviceInList(
  IN EFI_PCI_IO_PROTOCOL          *PciIo,
  IN LIST_ENTRY                   *PciDeviceList
  )
{
  UINTN                 PciSegment;
  UINTN                 PciBus;
  UINTN                 PciDevice;
  UINTN                 PciFunction;
  EFI_STATUS            Status;
  PCI_DEVICE_INSTANCE   *NewPciDevice;

  Status = PciIo->GetLocation (PciIo, &PciSegment, &PciBus, &PciDevice, &PciFunction);
  ASSERT_EFI_ERROR(Status);

  NewPciDevice = AllocateZeroPool(sizeof(*NewPciDevice));
  ASSERT(NewPciDevice != NULL);

  NewPciDevice->Signature   = PCI_DEVICE_INSTANCE_SIGNATURE;
  NewPciDevice->PciSegment  = PciSegment;
  NewPciDevice->PciBus      = PciBus;
  NewPciDevice->PciDevice   = PciDevice;
  NewPciDevice->PciFunction = PciFunction;

  InsertTailList(PciDeviceList, &NewPciDevice->Link);
}

/**
  Check if a PCI device is recorded in device list.

  @param PciIo            PciIo instance of the device
  @param PciDeviceList    The list to record the the device

  @retval TRUE  The PCI device is in the list.
  @retval FALSE The PCI device is NOT in the list.
**/
BOOLEAN
IsPciDeviceInList(
  IN EFI_PCI_IO_PROTOCOL          *PciIo,
  IN LIST_ENTRY                   *PciDeviceList
  )
{
  UINTN                 PciSegment;
  UINTN                 PciBus;
  UINTN                 PciDevice;
  UINTN                 PciFunction;
  EFI_STATUS            Status;
  LIST_ENTRY            *Link;
  PCI_DEVICE_INSTANCE   *CurrentPciDevice;

  Status = PciIo->GetLocation (PciIo, &PciSegment, &PciBus, &PciDevice, &PciFunction);
  ASSERT_EFI_ERROR(Status);

  Link = GetFirstNode(PciDeviceList);
  while (!IsNull(PciDeviceList, Link)) {
    CurrentPciDevice = PCI_DEVICE_INSTANCE_FROM_LINK(Link);

    if (CurrentPciDevice->PciSegment == PciSegment && CurrentPciDevice->PciBus == PciBus &&
        CurrentPciDevice->PciDevice == PciDevice && CurrentPciDevice->PciFunction == PciFunction) {
      DEBUG((DEBUG_INFO, "PCI device duplicated (Loc - %04x:%02x:%02x:%02x)\n", PciSegment, PciBus, PciDevice, PciFunction));
      return TRUE;
    }

    Link = GetNextNode(PciDeviceList, Link);
  }

  return FALSE;
}

/*
  return Offset of the PCI Cap ID.

  @param PciIo            PciIo instance of the device
  @param CapId            The Capability ID of the Pci device

  @return The PCI Capability ID Offset
*/
UINT32
GetPciCapId (
  IN EFI_PCI_IO_PROTOCOL          *PciIo,
  IN UINT8                        CapId
  )
{
  EFI_PCI_CAPABILITY_HDR                    PciCapIdHdr;
  UINT32                                    PciCapIdOffset;
  EFI_STATUS                                Status;

  PciCapIdHdr.CapabilityID = ~CapId;
  Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint8, PCI_CAPBILITY_POINTER_OFFSET, 1, &PciCapIdHdr.NextItemPtr);
  ASSERT_EFI_ERROR(Status);
  if (PciCapIdHdr.NextItemPtr == 0 || PciCapIdHdr.NextItemPtr == 0xFF) {
    return 0;
  }
  PciCapIdOffset = 0;
  do {
    if (PciCapIdHdr.CapabilityID == CapId) {
      break;
    }
    PciCapIdOffset = PciCapIdHdr.NextItemPtr;
    Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, PciCapIdOffset, 1, &PciCapIdHdr);
    ASSERT_EFI_ERROR(Status);
  } while (PciCapIdHdr.NextItemPtr != 0 && PciCapIdHdr.NextItemPtr != 0xFF);

  if (PciCapIdHdr.CapabilityID == CapId) {
    return PciCapIdOffset;
  } else {
    return 0;
  }
}

/*
  return Offset of the PCIe Ext Cap ID.

  @param PciIo            PciIo instance of the device
  @param CapId            The Ext Capability ID of the Pci device

  @return The PCIe Ext Capability ID Offset
*/
UINT32
GetPciExpressExtCapId (
  IN EFI_PCI_IO_PROTOCOL          *PciIo,
  IN UINT16                       CapId
  )
{
  UINT32                                    PcieCapIdOffset;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER  PciExpressExtCapIdHdr;
  EFI_STATUS                                Status;

  PcieCapIdOffset = GetPciCapId (PciIo, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (PcieCapIdOffset == 0) {
    return 0;
  }

  PciExpressExtCapIdHdr.CapabilityId = ~CapId;
  PciExpressExtCapIdHdr.CapabilityVersion = 0xF;
  PciExpressExtCapIdHdr.NextCapabilityOffset = 0x100;
  PcieCapIdOffset = 0;
  do {
    if (PciExpressExtCapIdHdr.CapabilityId == CapId) {
      break;
    }
    PcieCapIdOffset = PciExpressExtCapIdHdr.NextCapabilityOffset;
    Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint32, PcieCapIdOffset, 1, &PciExpressExtCapIdHdr);
    ASSERT_EFI_ERROR(Status);
  } while (PciExpressExtCapIdHdr.NextCapabilityOffset != 0 && PciExpressExtCapIdHdr.NextCapabilityOffset != 0xFFF);

  if (PciExpressExtCapIdHdr.CapabilityId == CapId) {
    return PcieCapIdOffset;
  } else {
    return 0;
  }
}

/**
  Read byte of the PCI device configuration space.

  @param PciIo            PciIo instance of the device
  @param Offset           The offset of the Pci device configuration space

  @return Byte value of the PCI device configuration space.
**/
UINT8
DvSecPciRead8 (
  IN EFI_PCI_IO_PROTOCOL *PciIo,
  IN UINT32              Offset
  )
{
  EFI_STATUS  Status;
  UINT8       Data;

  Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint8, Offset, 1, &Data);
  ASSERT_EFI_ERROR(Status);

  return Data;
}

/**
  Write byte of the PCI device configuration space.

  @param PciIo            PciIo instance of the device
  @param Offset           The offset of the Pci device configuration space
  @param Data             Byte value of the PCI device configuration space.
**/
VOID
DvSecPciWrite8 (
  IN EFI_PCI_IO_PROTOCOL *PciIo,
  IN UINT32              Offset,
  IN UINT8               Data
  )
{
  EFI_STATUS  Status;

  Status = PciIo->Pci.Write (PciIo, EfiPciIoWidthUint8, Offset, 1, &Data);
  ASSERT_EFI_ERROR(Status);
}

/**
  Get the Digest size from the TCG hash Algorithm ID.

  @param TcgAlgId            TCG hash Algorithm ID

  @return Digest size of the TCG hash Algorithm ID
**/
UINTN
DigestSizeFromTcgAlgId (
  IN UINT16                                    TcgAlgId
  )
{
  switch (TcgAlgId) {
  case TPM_ALG_SHA256:
    return SHA256_DIGEST_SIZE;
  case TPM_ALG_SHA384:
  case TPM_ALG_SHA512:
  case TPM_ALG_SM3_256:
  default:
    break;
  }
  return 0;
}

/**
  Convert the SPDM hash algo ID from the TCG hash Algorithm ID.

  @param TcgAlgId            TCG hash Algorithm ID

  @return SPDM hash algo ID
**/
UINT32
TcgAlgIdToSpdmHashAlgo (
  IN UINT16                                    TcgAlgId
  )
{
  switch (TcgAlgId) {
  case TPM_ALG_SHA256:
    return SPDM_ALGORITHMS_BASE_HASH_ALGO_TPM_ALG_SHA_256;
  case TPM_ALG_SHA384:
  case TPM_ALG_SHA512:
  case TPM_ALG_SM3_256:
  default:
    break;
  }
  return 0;
}

/**
  This function extend the PCI digest from the DvSec register.

  @param[in]  PciIo                  The PciIo of the device.
  @param[in]  DeviceSecurityPolicy   The Device Security Policy associated with the device.
  @param[in]  TcgAlgId               TCG hash Algorithm ID
  @param[in]  DigestSel              The digest selector
  @param[in]  Digest                 The digest buffer
  @param[out] DeviceSecurityState    The Device Security state associated with the device.
**/
VOID
ExtendDigestRegister (
  IN EFI_PCI_IO_PROTOCOL          *PciIo,
  IN EDKII_DEVICE_SECURITY_POLICY *DeviceSecurityPolicy,
  IN UINT16                       TcgAlgId,
  IN UINT8                        DigestSel,
  IN UINT8                        *Digest,
  OUT EDKII_DEVICE_SECURITY_STATE *DeviceSecurityState
  )
{
  UINT32                                                   PcrIndex;
  UINT32                                                   EventType;
  EDKII_DEVICE_SECURITY_PCI_EVENT_DATA                     EventLog;
  EFI_STATUS                                               Status;
  PCI_TYPE00                                               PciData;

  Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint8, 0, sizeof(PciData), &PciData);
  ASSERT_EFI_ERROR(Status);

  //
  // Use PCR 2 for Firmware Blob code.
  //
  PcrIndex = 2;
  EventType = EV_EFI_SPDM_FIRMWARE_BLOB;

  CopyMem (EventLog.EventData.Signature, TCG_DEVICE_SECURITY_EVENT_DATA_SIGNATURE, sizeof(EventLog.EventData.Signature));
  EventLog.EventData.Version                  = TCG_DEVICE_SECURITY_EVENT_DATA_VERSION;
  EventLog.EventData.Length                   = sizeof(EDKII_DEVICE_SECURITY_PCI_EVENT_DATA);
  EventLog.EventData.SpdmHashAlgo             = TcgAlgIdToSpdmHashAlgo (TcgAlgId);
  EventLog.EventData.DeviceType               = TCG_DEVICE_SECURITY_EVENT_DATA_DEVICE_TYPE_PCI;

  EventLog.CommonHeader.Index                      = DigestSel;
  EventLog.CommonHeader.MeasurementSpecification   = SPDM_MEASUREMENT_BLOCK_HEADER_SPECIFICATION_DMTF;
  EventLog.CommonHeader.MeasurementSize            = sizeof(SPDM_MEASUREMENT_BLOCK_DMTF_HEADER) + SHA256_DIGEST_SIZE;
  EventLog.DmtfHeader.DMTFSpecMeasurementValueType = SPDM_MEASUREMENT_BLOCK_MEASUREMENT_TYPE_MUTABLE_FIRMWARE;
  EventLog.DmtfHeader.DMTFSpecMeasurementValueSize = SHA256_DIGEST_SIZE;
  CopyMem (&EventLog.Digest, Digest, SHA256_DIGEST_SIZE);

  EventLog.PciContext.Version           = TCG_DEVICE_SECURITY_EVENT_DATA_PCI_CONTEXT_VERSION;
  EventLog.PciContext.Length            = sizeof(TCG_DEVICE_SECURITY_EVENT_DATA_PCI_CONTEXT);
  EventLog.PciContext.VendorId          = PciData.Hdr.VendorId;
  EventLog.PciContext.DeviceId          = PciData.Hdr.DeviceId;
  EventLog.PciContext.RevisionID        = PciData.Hdr.RevisionID;
  EventLog.PciContext.ClassCode[0]      = PciData.Hdr.ClassCode[0];
  EventLog.PciContext.ClassCode[1]      = PciData.Hdr.ClassCode[1];
  EventLog.PciContext.ClassCode[2]      = PciData.Hdr.ClassCode[2];
  if ((PciData.Hdr.HeaderType & HEADER_LAYOUT_CODE) == HEADER_TYPE_DEVICE) {
    EventLog.PciContext.SubsystemVendorID = PciData.Device.SubsystemVendorID;
    EventLog.PciContext.SubsystemID       = PciData.Device.SubsystemID;
  } else {
    EventLog.PciContext.SubsystemVendorID = 0;
    EventLog.PciContext.SubsystemID       = 0;
  }

  Status = TpmMeasureAndLogData (
             PcrIndex,
             EventType,
             &EventLog,
             EventLog.EventData.Length,
             Digest,
             SHA256_DIGEST_SIZE
             );
  DEBUG((DEBUG_INFO, "TpmMeasureAndLogData - %r\n", Status));
  if (EFI_ERROR(Status)) {
    DeviceSecurityState->MeasurementState = EDKII_DEVICE_SECURITY_STATE_ERROR_TCG_EXTEND_TPM_PCR;
  } else {
    RecordPciDeviceInList (PciIo, &mSecurityEventMeasurementDeviceList);
  }
}

/**
  This function reads the PCI digest from the DvSec register and extend to TPM.

  @param[in]  PciIo                  The PciIo of the device.
  @param[in]  DvSecOffset            The DvSec register offset of the device.
  @param[in]  DeviceSecurityPolicy   The Device Security Policy associated with the device.
  @param[out] DeviceSecurityState    The Device Security state associated with the device.
**/
VOID
DoMeasurementsFromDigestRegister (
  IN EFI_PCI_IO_PROTOCOL          *PciIo,
  IN UINT32                       DvSecOffset,
  IN EDKII_DEVICE_SECURITY_POLICY *DeviceSecurityPolicy,
  OUT EDKII_DEVICE_SECURITY_STATE *DeviceSecurityState
  )
{
  UINT8                                     Modified;
  UINT8                                     Valid;
  UINT16                                    TcgAlgId;
  UINT8                                     NumDigest;
  UINT8                                     DigestSel;
  UINT8                                     Digest[SHA256_DIGEST_SIZE];
  UINTN                                     DigestSize;
  EFI_STATUS                                Status;

  TcgAlgId = DvSecPciRead8 (
               PciIo,
               DvSecOffset + sizeof(INTEL_PCI_DIGEST_CAPABILITY_HEADER) + OFFSET_OF(INTEL_PCI_DIGEST_CAPABILITY_STRUCTURE, TcgAlgId)
               );
  DEBUG((DEBUG_INFO, "  TcgAlgId      - 0x%04x\n", TcgAlgId));
  DigestSize = DigestSizeFromTcgAlgId (TcgAlgId);
  if (DigestSize == 0) {
    DEBUG((DEBUG_INFO, "Unsupported Algorithm - 0x%04x\n", TcgAlgId));
    DeviceSecurityState->MeasurementState = EDKII_DEVICE_SECURITY_STATE_ERROR_PCI_NO_CAPABILITIES;
    return ;
  }
  DEBUG((DEBUG_INFO, "  (DigestSize: 0x%x)\n", DigestSize));

  DeviceSecurityState->MeasurementState = EDKII_DEVICE_SECURITY_STATE_SUCCESS;

  NumDigest = DvSecPciRead8 (
                PciIo,
                DvSecOffset + sizeof(INTEL_PCI_DIGEST_CAPABILITY_HEADER) + OFFSET_OF(INTEL_PCI_DIGEST_CAPABILITY_STRUCTURE, FirmwareID)
                );
  DEBUG((DEBUG_INFO, "  NumDigest     - 0x%02x\n", NumDigest));

  Valid = DvSecPciRead8 (
            PciIo,
            DvSecOffset + sizeof(INTEL_PCI_DIGEST_CAPABILITY_HEADER) + OFFSET_OF(INTEL_PCI_DIGEST_CAPABILITY_STRUCTURE, Valid)
            );
  DEBUG((DEBUG_INFO, "  Valid         - 0x%02x\n", Valid));

  //
  // Only 2 are supported as maximum.
  // But hardware may report 3.
  //
  if (NumDigest > 2) {
    NumDigest = 2;
  }

  for (DigestSel = 0; DigestSel < NumDigest; DigestSel++) {
    DEBUG((DEBUG_INFO, "  DigestSel     - 0x%02x\n", DigestSel));
    if ((DigestSel == 0) && ((Valid & INTEL_PCI_DIGEST_0_VALID) == 0)) {
      continue;
    }
    if ((DigestSel == 1) && ((Valid & INTEL_PCI_DIGEST_1_VALID) == 0)) {
      continue;
    }
    while (TRUE) {
      //
      // Host MUST clear DIGEST_MODIFIED before read DIGEST.
      //
      DvSecPciWrite8 (
        PciIo,
        DvSecOffset + sizeof(INTEL_PCI_DIGEST_CAPABILITY_HEADER) + OFFSET_OF(INTEL_PCI_DIGEST_CAPABILITY_STRUCTURE, Modified),
        INTEL_PCI_DIGEST_MODIFIED
        );

      Status = PciIo->Pci.Read (
                            PciIo,
                            EfiPciIoWidthUint8,
                            (UINT32)(DvSecOffset + sizeof(INTEL_PCI_DIGEST_CAPABILITY_HEADER) + sizeof(INTEL_PCI_DIGEST_CAPABILITY_STRUCTURE) + DigestSize * DigestSel),
                            DigestSize,
                            Digest
                            );
      ASSERT_EFI_ERROR(Status);

      //
      // After read DIGEST, Host MUST consult DIGEST_MODIFIED.
      //
      Modified = DvSecPciRead8 (
                   PciIo,
                   DvSecOffset + sizeof(INTEL_PCI_DIGEST_CAPABILITY_HEADER) + OFFSET_OF(INTEL_PCI_DIGEST_CAPABILITY_STRUCTURE, Modified)
                   );
      if ((Modified & INTEL_PCI_DIGEST_MODIFIED) == 0) {
        break;
      }
    }

    //
    // Dump Digest
    //
    {
      UINTN  Index;
      DEBUG((DEBUG_INFO, "  Digest        - "));
      for (Index = 0; Index < DigestSize; Index++) {
        DEBUG((DEBUG_INFO, "%02x", *(Digest + Index)));
      }
      DEBUG((DEBUG_INFO, "\n"));
    }

    DEBUG((DEBUG_INFO, "ExtendDigestRegister...\n", ExtendDigestRegister));
    ExtendDigestRegister (PciIo, DeviceSecurityPolicy, TcgAlgId, DigestSel, Digest, DeviceSecurityState);
  }
}

/**
  The device driver uses this service to measure a PCI device.

  @param[in]  PciIo                  The PciIo of the device.
  @param[in]  DeviceSecurityPolicy   The Device Security Policy associated with the device.
  @param[out] DeviceSecurityState    The Device Security state associated with the device.
**/
VOID
DoDeviceMeasurement (
  IN EFI_PCI_IO_PROTOCOL          *PciIo,
  IN EDKII_DEVICE_SECURITY_POLICY *DeviceSecurityPolicy,
  OUT EDKII_DEVICE_SECURITY_STATE *DeviceSecurityState
  )
{
  UINT32                                    DvSecOffset;
  INTEL_PCI_DIGEST_CAPABILITY_HEADER        DvSecHdr;
  EFI_STATUS                                Status;

  if (IsPciDeviceInList (PciIo, &mSecurityEventMeasurementDeviceList)) {
    DeviceSecurityState->MeasurementState = EDKII_DEVICE_SECURITY_STATE_SUCCESS;
    return ;
  }

  DvSecOffset = GetPciExpressExtCapId (PciIo, INTEL_PCI_CAPID_DVSEC);
  DEBUG((DEBUG_INFO, "DvSec Capability - 0x%x\n", DvSecOffset));
  if (DvSecOffset == 0) {
    DeviceSecurityState->MeasurementState = EDKII_DEVICE_SECURITY_STATE_ERROR_PCI_NO_CAPABILITIES;
    return ;
  }
  Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, DvSecOffset, sizeof(DvSecHdr)/sizeof(UINT16), &DvSecHdr);
  ASSERT_EFI_ERROR(Status);
  DEBUG((DEBUG_INFO, "  CapId         - 0x%04x\n", DvSecHdr.CapId));
  DEBUG((DEBUG_INFO, "  CapVersion    - 0x%01x\n", DvSecHdr.CapVersion));
  DEBUG((DEBUG_INFO, "  NextOffset    - 0x%03x\n", DvSecHdr.NextOffset));
  DEBUG((DEBUG_INFO, "  DvSecVendorId - 0x%04x\n", DvSecHdr.DvSecVendorId));
  DEBUG((DEBUG_INFO, "  DvSecRevision - 0x%01x\n", DvSecHdr.DvSecRevision));
  DEBUG((DEBUG_INFO, "  DvSecLength   - 0x%03x\n", DvSecHdr.DvSecLength));
  DEBUG((DEBUG_INFO, "  DvSecId       - 0x%04x\n", DvSecHdr.DvSecId));
  if ((DvSecHdr.DvSecVendorId != INTEL_PCI_DVSEC_VENDORID_INTEL) &&
      (DvSecHdr.DvSecId != INTEL_PCI_DVSEC_DVSECID_MEASUREMENT)) {
    DeviceSecurityState->MeasurementState = EDKII_DEVICE_SECURITY_STATE_ERROR_PCI_NO_CAPABILITIES;
    return ;
  }

  DoMeasurementsFromDigestRegister (PciIo, DvSecOffset, DeviceSecurityPolicy, DeviceSecurityState);
}

/**
  The device driver uses this service to verify a PCI device.

  @param[in]  PciIo                  The PciIo of the device.
  @param[in]  DeviceSecurityPolicy   The Device Security Policy associated with the device.
  @param[out] DeviceSecurityState    The Device Security state associated with the device.
**/
VOID
DoDeviceAuthentication (
  IN EFI_PCI_IO_PROTOCOL          *PciIo,
  IN EDKII_DEVICE_SECURITY_POLICY *DeviceSecurityPolicy,
  OUT EDKII_DEVICE_SECURITY_STATE *DeviceSecurityState
  )
{
  DeviceSecurityState->AuthenticationState = EDKII_DEVICE_SECURITY_STATE_ERROR_UEFI_UNSUPPORTED;
}

/**
  The device driver uses this service to measure and/or verify a device.

  The flow in device driver is:
  1) Device driver discovers a new device.
  2) Device driver creates an EFI_DEVICE_PATH_PROTOCOL.
  3) Device driver creates a device access protocol. e.g.
     EFI_PCI_IO_PROTOCOL for PCI device.
     EFI_USB_IO_PROTOCOL for USB device.
     EFI_EXT_SCSI_PASS_THRU_PROTOCOL for SCSI device.
     EFI_ATA_PASS_THRU_PROTOCOL for ATA device.
     EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL for NVMe device.
     EFI_SD_MMC_PASS_THRU_PROTOCOL for SD/MMC device.
  4) Device driver installs the EFI_DEVICE_PATH_PROTOCOL with EFI_DEVICE_PATH_PROTOCOL_GUID,
     and the device access protocol with EDKII_DEVICE_IDENTIFIER_TYPE_xxx_GUID.
     Once it is done, a DeviceHandle is returned.
  5) Device driver creates EDKII_DEVICE_IDENTIFIER with EDKII_DEVICE_IDENTIFIER_TYPE_xxx_GUID
     and the DeviceHandle.
  6) Device driver calls DeviceAuthenticate().
  7) If DeviceAuthenticate() returns EFI_SECURITY_VIOLATION, the device driver uninstalls
     all protocols on this handle.
  8) If DeviceAuthenticate() returns EFI_SUCCESS, the device driver installs the device access
     protocol with a real protocol GUID. e.g.
     EFI_PCI_IO_PROTOCOL with EFI_PCI_IO_PROTOCOL_GUID.
     EFI_USB_IO_PROTOCOL with EFI_USB_IO_PROTOCOL_GUID.

  @param[in]  This              The protocol instance pointer.
  @param[in]  DeviceId          The Identifier for the device.

  @retval EFI_SUCCESS              The device specified by the DeviceId passed the measurement
                                   and/or authentication based upon the platform policy.
                                   If TCG measurement is required, the measurement is extended to TPM PCR.
  @retval EFI_SECURITY_VIOLATION   The device fails to return the measurement data.
  @retval EFI_SECURITY_VIOLATION   The device fails to response the authentication request.
  @retval EFI_SECURITY_VIOLATION   The system fails to verify the device based upon the authentication response.
  @retval EFI_SECURITY_VIOLATION   The system fails to extend the measurement to TPM PCR.
**/
EFI_STATUS
EFIAPI
DeviceAuthentication (
  IN EDKII_DEVICE_SECURITY_PROTOCOL  *This,
  IN EDKII_DEVICE_IDENTIFIER         *DeviceId
  )
{
  EDKII_DEVICE_SECURITY_POLICY           DeviceSecurityPolicy;
  EDKII_DEVICE_SECURITY_STATE            DeviceSecurityState;
  EFI_PCI_IO_PROTOCOL                    *PciIo;
  EFI_STATUS                             Status;

  if (mDeviceSecurityPolicy == NULL) {
    return EFI_SUCCESS;
  }

  if (!CompareGuid (&DeviceId->DeviceType, &gEdkiiDeviceIdentifierTypePciGuid)) {
    return EFI_SUCCESS;
  }

  Status = gBS->HandleProtocol (
                  DeviceId->DeviceHandle,
                  &gEdkiiDeviceIdentifierTypePciGuid,
                  (VOID **)&PciIo
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Locate - DeviceIdentifierTypePci - %r\n", Status));
    return EFI_SUCCESS;
  }

  DeviceSecurityState.Revision = EDKII_DEVICE_SECURITY_STATE_REVISION;
  DeviceSecurityState.MeasurementState = 0x0;
  DeviceSecurityState.AuthenticationState = 0x0;

  Status = mDeviceSecurityPolicy->GetDevicePolicy (mDeviceSecurityPolicy, DeviceId, &DeviceSecurityPolicy);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "mDeviceSecurityPolicy->GetDevicePolicy - %r\n", Status));
    DeviceSecurityState.MeasurementState = EDKII_DEVICE_SECURITY_STATE_ERROR_UEFI_GET_POLICY_PROTOCOL;
    DeviceSecurityState.AuthenticationState = EDKII_DEVICE_SECURITY_STATE_ERROR_UEFI_GET_POLICY_PROTOCOL;
  } else {
    if ((DeviceSecurityPolicy.MeasurementPolicy & EDKII_DEVICE_MEASUREMENT_REQUIRED) != 0) {
      DoDeviceMeasurement (PciIo, &DeviceSecurityPolicy, &DeviceSecurityState);
      DEBUG((DEBUG_ERROR, "MeasurementState - 0x%08x\n", DeviceSecurityState.MeasurementState));
    }
    if ((DeviceSecurityPolicy.AuthenticationPolicy & EDKII_DEVICE_AUTHENTICATION_REQUIRED) != 0) {
      DoDeviceAuthentication (PciIo, &DeviceSecurityPolicy, &DeviceSecurityState);
      DEBUG((DEBUG_ERROR, "AuthenticationState - 0x%08x\n", DeviceSecurityState.AuthenticationState));
    }
  }

  Status = mDeviceSecurityPolicy->NotifyDeviceState (mDeviceSecurityPolicy, DeviceId, &DeviceSecurityState);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "mDeviceSecurityPolicy->NotifyDeviceState - %r\n", Status));
  }

  if ((DeviceSecurityState.MeasurementState == 0) &&
      (DeviceSecurityState.AuthenticationState == 0)) {
    return EFI_SUCCESS;
  } else {
    return EFI_SECURITY_VIOLATION;
  }
}

EDKII_DEVICE_SECURITY_PROTOCOL mDeviceSecurity = {
  EDKII_DEVICE_SECURITY_PROTOCOL_REVISION,
  DeviceAuthentication
};

/**
  Entrypoint of the device security driver.

  @param[in]  ImageHandle  ImageHandle of the loaded driver
  @param[in]  SystemTable  Pointer to the System Table

  @retval  EFI_SUCCESS           The Protocol is installed.
  @retval  EFI_OUT_OF_RESOURCES  Not enough resources available to initialize driver.
  @retval  EFI_DEVICE_ERROR      A device error occurred attempting to initialize the driver.

**/
EFI_STATUS
EFIAPI
MainEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_HANDLE  Handle;
  EFI_STATUS  Status;

  Status = gBS->LocateProtocol (&gEdkiiDeviceSecurityPolicyProtocolGuid, NULL, (VOID **)&mDeviceSecurityPolicy);
  ASSERT_EFI_ERROR(Status);

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEdkiiDeviceSecurityProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  (VOID **)&mDeviceSecurity
                  );
  ASSERT_EFI_ERROR(Status);

  return Status;
}
