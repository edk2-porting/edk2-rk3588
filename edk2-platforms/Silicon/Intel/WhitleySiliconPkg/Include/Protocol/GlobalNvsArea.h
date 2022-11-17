/** @file
  Definition of the global NVS area protocol.  This protocol
  publishes the address and format of a global ACPI NVS buffer used as a communications
  buffer between SMM code and ASL code.
  Note:  Data structures defined in this protocol are not naturally aligned.

  @copyright
  Copyright 2004 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _EFI_GLOBAL_NVS_AREA_H_
#define _EFI_GLOBAL_NVS_AREA_H_

//
// Global NVS Area Protocol GUID
//
#define EFI_GLOBAL_NVS_AREA_PROTOCOL_GUID { 0x74e1e48, 0x8132, 0x47a1, 0x8c, 0x2c, 0x3f, 0x14, 0xad, 0x9a, 0x66, 0xdc }
//
// Because of ASL constrains cannot use MAX_SOCKET and MAX_LOGIC_IIO_STACK to configure ACPI objects. The symbols
// below are the largest values of MAX_SOCKET or MAX_LOGIC_IIO_STACK currently supported in BiosParameterRegion.asi.
//
#define NVS_MAX_SOCKETS          8
#define NVS_MAX_LOGIC_IIO_STACKS 14

#if NVS_MAX_SOCKETS < MAX_SOCKET
#error "Must update NVS_MAX_SOCKETS and BiosParameterRegion.asi to handle so many sockets"
#endif
#if NVS_MAX_LOGIC_IIO_STACKS < MAX_LOGIC_IIO_STACK
#error "Must update NVS_MAX_LOGIC_IIO_STACKS and BiosParameterRegion.asi to handle so many stacks"
#endif

//
// Extern the GUID for protocol users.
//
extern EFI_GUID gEfiGlobalNvsAreaProtocolGuid;

//
// Global NVS Area definition
// BIOS parameters region provided by POST code to ASL, defined as PSYS in BiosParametersRegion.asi
//
#pragma pack (1)

typedef struct {
  // IOAPIC Start
  UINT32  PlatformId;
  UINT64  IoApicEnable;
  UINT8   ApicIdOverrided     :1;
  UINT8   PchIoApic_24_119    :1;
  UINT8   Cpx4Detect          :1;
  UINT8   Reserved0           :5;
  // IOAPIC End

  // Power Management Start
  UINT8   TpmEnable           :1;
  UINT8   CStateEnable        :1;
  UINT8   C3Enable            :1;
  UINT8   C6Enable            :1;
  UINT8   C7Enable            :1;
  UINT8   MonitorMwaitEnable  :1;
  UINT8   PStateEnable        :1;
  UINT8   EmcaEn              :1;
  UINT8   HWAllEnable         :2;
  UINT8   KBPresent           :1;
  UINT8   MousePresent        :1;
  UINT8   TStateEnable        :1;
  UINT8   TStateFineGrained   :1;
  UINT8   OSCX                :1;
  UINT8   Reserved1           :1;
  // Power Management End

  // RAS Start
  UINT8   CpuChangeMask;
  UINT8   IioChangeMask;
  UINT16  IioPresentBitMask[NVS_MAX_SOCKETS];
  UINT32  SocketBitMask;                       // make sure this is at 4byte boundary
  UINT8   CpuCoreThreadsCount;
  UINT32  ProcessorApicIdBase[NVS_MAX_SOCKETS];
  UINT64  ProcessorBitMask[NVS_MAX_SOCKETS];   // cores 0-63 for each socket
  UINT64  ProcessorBitMaskHi[NVS_MAX_SOCKETS]; // cores 64-127 for each socket
  UINT32  MmCfg;
  UINT32  TsegSize;
  UINT32  SmiRequestParam[4];
  UINT32  SciRequestParam[4];
  UINT64  MigrationActionRegionAddress;
  UINT8   Cpu0Uuid[16];
  UINT8   Cpu1Uuid[16];
  UINT8   Cpu2Uuid[16];
  UINT8   Cpu3Uuid[16];
  UINT8   Cpu4Uuid[16];
  UINT8   Cpu5Uuid[16];
  UINT8   Cpu6Uuid[16];
  UINT8   Cpu7Uuid[16];
  UINT8   CpuSpareMask;
  UINT8   Mem0Uuid[16];
  UINT8   Mem1Uuid[16];
  UINT8   Mem2Uuid[16];
  UINT8   Mem3Uuid[16];
  UINT8   Mem4Uuid[16];
  UINT8   Mem5Uuid[16];
  UINT8   Mem6Uuid[16];
  UINT8   Mem7Uuid[16];
  UINT8   Mem8Uuid[16];
  UINT8   Mem9Uuid[16];
  UINT8   Mem10Uuid[16];
  UINT8   Mem11Uuid[16];
  UINT8   Mem12Uuid[16];
  UINT8   Mem13Uuid[16];
  UINT8   Mem14Uuid[16];
  UINT8   Mem15Uuid[16];
  UINT64  EmcaL1DirAddr;
  UINT32  ProcessorId;
  UINT8   PcieAcpiHotPlugEnable;
  UINT8   WheaEnabled;
  UINT8   WheaSci;
  UINT8   PropagateSerrOption;
  UINT8   PropagatePerrOption;
  // RAS End

  // VTD Start
  UINT64  DrhdAddr[3];
  UINT64  AtsrAddr[3];
  UINT64  RhsaAddr[3];
  // VTD End

  // SR-IOV WA Start
  UINT8   WmaaSICaseValue;
  UINT16  WmaaSISeg;
  UINT8   WmaaSIBus;
  UINT8   WmaaSIDevice;
  UINT8   WmaaSIFunction;
  UINT8   WmaaSISts;
  UINT8   WheaSupportEn;
  // SR-IOV End

  // BIOS Guard Start
  UINT64  BiosGuardMemAddress;
  UINT8   BiosGuardMemSize;
  UINT16  BiosGuardIoTrapAddress;
  UINT8   CpuSkuNumOfBitShift;
  // BIOS Guard End

  // USB3 Start
  UINT8   XhciMode;
  UINT8   HostAlertVector1;
  UINT8   HostAlertVector2;
  // USB3 End

  // HWPM Start
  UINT8   HWPMEnable          :2; // HWPM
  UINT8   Reserved3           :1; // reserved bit
  UINT8   HwpInterrupt        :1; // HWP Interrupt
  UINT8   Reserved2           :4; // reserved bits
  // HWPM End

  // SGX Start
  UINT8  SgxStatus;
  UINT64 EpcLength[8];            // MAX_IMC * MAX_SOCKET
  UINT64 EpcBaseAddress[8];       // MAX_IMC * MAX_SOCKET
  // SGX End

  // PCIe Multi-Seg Start
  UINT8   BusBase[NVS_MAX_SOCKETS][NVS_MAX_LOGIC_IIO_STACKS]; // PCI bus base number for each stack
  UINT8   PcieMultiSegSupport;    // Enable /Disable switch
  UINT8   PcieSegNum[NVS_MAX_SOCKETS]; // PCI segment number array for each socket
  // PCIe Multi-seg end

  UINT8   SncAnd2Cluster;         // 0 - SNC disabled, 2 - SNC enabled (2 clusters), 4 - SNC enabled (4 clusters)

  // XTU Start
  UINT32  XTUBaseAddress;         // 193 XTU Base Address
  UINT32  XTUSize;                // 197 XTU Entries Size
  UINT32  XMPBaseAddress;         // 201 XTU Base Address
  UINT8   DDRReferenceFreq;       // 205 DDR Reference Frequency
  UINT8   Rtd3Support;            // 206 Runtime D3 support.
  UINT8   Rtd3P0dl;               // 207 User selctable Delay for Device D0 transition.
  UINT8   Rtd3P3dl;               // 208 User selctable Delay for Device D0 transition.
  // XTU End

  // FPGA Root Port Bus
  UINT8   FpgaBusBase[8];
  UINT8   FpgaBusLimit[8];

  // FPGA present bit
  UINT8   FpgaPresent[8];

  // FPGA Resource Allocation
  UINT32  VFPBMemBase[8];
  UINT32  VFPBMemLimit[8];

  // FPGA KTI present bitmap
  UINT32  FpgaKtiPresent;
  // FPGA Bus for KTI
  UINT8   FpgaKtiBase[8];

  UINT16  PmBase;                  // ACPI IO Base Address
  UINT8   DebugModeIndicator;      // Debug Mode Indicator
  UINT8   IioPcieRpCapOffset;      // IIO PCIe root port PCIe Capability offset
  UINT8   ArtTscLinkFlag;          // Flag to indicate if TSC is linked to ART
} BIOS_ACPI_PARAM;

#pragma pack ()

//
// Global NVS Area Protocol
//
typedef struct _EFI_GLOBAL_NVS_AREA_PROTOCOL {
  BIOS_ACPI_PARAM     *Area;
} EFI_GLOBAL_NVS_AREA_PROTOCOL;

#endif
