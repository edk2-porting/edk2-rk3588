/** @file
  APIs of PCH PCIE SMI Dispatch Protocol.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

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
  @param[in] RpIndex                    Refer to PCH PCIE Root Port index.
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
