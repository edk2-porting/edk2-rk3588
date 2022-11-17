/** @file
  APIs of PCH PCIE SMI Dispatch Protocol.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_PCIE_SMI_DISPATCH_PROTOCOL_H_
#define _PCH_PCIE_SMI_DISPATCH_PROTOCOL_H_

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                         gPchPcieSmiDispatchProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _PCH_PCIE_SMI_DISPATCH_PROTOCOL    PCH_PCIE_SMI_DISPATCH_PROTOCOL;

typedef enum {
  PchRpIndex0      = 0,
  PchRpIndex1      = 1,
  PchRpIndex2      = 2,
  PchRpIndex3      = 3,
  PchRpIndex4      = 4,
  PchRpIndex5      = 5,
  PchRpIndex6      = 6,
  PchRpIndex7      = 7,
  PchRpIndex8      = 8,
  PchRpIndex9      = 9,
  PchRpIndex10     = 10,
  PchRpIndex11     = 11,
  PchRpIndex12     = 12,
  PchRpIndex13     = 13,
  PchRpIndex14     = 14,
  PchRpIndex15     = 15,
  PchRpIndex16     = 16,
  PchRpIndex17     = 17,
  PchRpIndex18     = 18,
  PchRpIndex19     = 19,
  PchRpIndex20     = 20,
  PchRpIndex21     = 21,
  PchRpIndex22     = 22,
  PchRpIndex23     = 23,
  /**
    Quantity of PCH and CPU PCIe ports, as well as their encoding in this enum, may change between
    silicon generations and series. Do not assume that PCH port 0 will be always encoded by 0.
    Instead, it is recommended to use (PchRpIndex0 + PchPortIndex) style to be forward-compatible
  **/
  CpuRpIndex0      = 0x40,
  CpuRpIndex1      = 0x41,
  CpuRpIndex2      = 0x42,
  CpuRpIndex3      = 0x43
} PCIE_COMBINED_RPINDEX;

//
// Member functions
//

typedef struct {
  UINT8                                 RpIndex; ///< Root port index (0-based), 0: RP1, 1: RP2, n: RP(N+1)
  UINT8                                 BusNum;  ///< Root port pci bus number
  UINT8                                 DevNum;  ///< Root port pci device number
  UINT8                                 FuncNum; ///< Root port pci function number
} PCH_PCIE_SMI_RP_CONTEXT;

/**
  Callback function for an PCH PCIE RP SMI handler dispatch.

  @param[in] DispatchHandle             The unique handle assigned to this handler by register function.
  @param[in] RpContext                  Pointer of PCH PCIE Root Port context.

**/
typedef
VOID
(EFIAPI *PCH_PCIE_SMI_RP_DISPATCH_CALLBACK) (
  IN EFI_HANDLE                         DispatchHandle,
  IN PCH_PCIE_SMI_RP_CONTEXT            *RpContext
  );

/**
  Register a child SMI source dispatch function for PCH PCIERP SMI events.

  @param[in] This                       Protocol instance pointer.
  @param[in] DispatchFunction           Pointer to dispatch function to be invoked for
                                        this SMI source
  @param[in] RpIndex                    Refer PCIE_COMBINED_RPINDEX for PCH RP index and CPU RP index.
                                        0: RP1, 1: RP2, n: RP(N+1)
  @param[out] DispatchHandle            Handle of dispatch function, for when interfacing
                                        with the parent SMM driver.

  @retval EFI_SUCCESS                   The dispatch function has been successfully
                                        registered and the SMI source has been enabled.
  @retval EFI_DEVICE_ERROR              The driver was unable to enable the SMI source.
  @retval EFI_OUT_OF_RESOURCES          Not enough memory (system or SMM) to manage this child.
  @retval EFI_ACCESS_DENIED             Return access denied if the EndOfDxe event has been triggered
**/
typedef
EFI_STATUS
(EFIAPI *PCH_PCIE_SMI_RP_DISPATCH_REGISTER) (
  IN  PCH_PCIE_SMI_DISPATCH_PROTOCOL    *This,
  IN  PCH_PCIE_SMI_RP_DISPATCH_CALLBACK DispatchFunction,
  IN  UINTN                             RpIndex,
  OUT EFI_HANDLE                        *DispatchHandle
  );

/**
  Unregister a child SMI source dispatch function with a parent PCIE SMM driver

  @param[in] This                       Protocol instance pointer.
  @param[in] DispatchHandle             Handle of dispatch function to deregister.

  @retval EFI_SUCCESS                   The dispatch function has been successfully
                                        unregistered and the SMI source has been disabled
                                        if there are no other registered child dispatch
                                        functions for this SMI source.
  @retval EFI_INVALID_PARAMETER         Handle is invalid.
  @retval EFI_ACCESS_DENIED             Return access denied if the EndOfDxe event has been triggered
**/
typedef
EFI_STATUS
(EFIAPI *PCH_PCIE_SMI_DISPATCH_UNREGISTER) (
  IN  PCH_PCIE_SMI_DISPATCH_PROTOCOL    *This,
  IN  EFI_HANDLE                        DispatchHandle
  );

/**
  Interface structure for PCH PCIE SMIs Dispatch Protocol
  The PCH PCIE SMI DISPATCH PROTOCOL provides the ability to dispatch function for PCH PCIE related SMIs.
  It contains SMI types of HotPlug, LinkActive, and Link EQ.
**/
struct _PCH_PCIE_SMI_DISPATCH_PROTOCOL {
  /**
    This member specifies the revision of this structure. This field is used to
    indicate backwards compatible changes to the protocol.
  **/
  UINT8                                 Revision;
  /**
    Smi unregister function for PCH PCIE SMI DISPATCH PROTOCOL.
  **/
  PCH_PCIE_SMI_DISPATCH_UNREGISTER      UnRegister;
  /**
    PcieRpXHotPlug
    The event is triggered when PCIE root port Hot-Plug Presence Detect.
  **/
  PCH_PCIE_SMI_RP_DISPATCH_REGISTER     HotPlugRegister;
  /**
    PcieRpXLinkActive
    The event is triggered when Hot-Plug Link Active State Changed.
  **/
  PCH_PCIE_SMI_RP_DISPATCH_REGISTER     LinkActiveRegister;
  /**
    PcieRpXLinkEq
    The event is triggered when Device Requests Software Link Equalization.
  **/
  PCH_PCIE_SMI_RP_DISPATCH_REGISTER     LinkEqRegister;
};

/**
  PCH PCIE SMI dispatch revision number

  Revision 1:   Initial version
**/
#define PCH_PCIE_SMI_DISPATCH_REVISION            1

#endif
