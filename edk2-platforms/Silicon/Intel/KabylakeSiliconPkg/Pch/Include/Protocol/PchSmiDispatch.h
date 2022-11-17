/** @file
  APIs of PCH SMI Dispatch Protocol.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PCH_SMI_DISPATCH_PROTOCOL_H_
#define _PCH_SMI_DISPATCH_PROTOCOL_H_

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                         gPchSmiDispatchProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _PCH_SMI_DISPATCH_PROTOCOL         PCH_SMI_DISPATCH_PROTOCOL;

//
// Member functions
//

/**
  Callback function for an PCH SMI handler dispatch.

  @param[in] DispatchHandle             The unique handle assigned to this handler by register function.

**/
typedef
VOID
(EFIAPI *PCH_SMI_DISPATCH_CALLBACK) (
  IN EFI_HANDLE                         DispatchHandle
  );

/**
  Register a child SMI source dispatch function for specific PCH SMI dispatch event.

  @param[in] This                       Protocol instance pointer.
  @param[in] DispatchFunction           Pointer to dispatch function to be invoked for
                                        this SMI source
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
(EFIAPI *PCH_SMI_DISPATCH_REGISTER) (
  IN  PCH_SMI_DISPATCH_PROTOCOL         *This,
  IN  PCH_SMI_DISPATCH_CALLBACK         DispatchFunction,
  OUT EFI_HANDLE                        *DispatchHandle
  );

/**
  Unregister a child SMI source dispatch function with a parent SMM driver

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
(EFIAPI *PCH_SMI_DISPATCH_UNREGISTER) (
  IN  PCH_SMI_DISPATCH_PROTOCOL         *This,
  IN  EFI_HANDLE                        DispatchHandle
  );

/**
  Interface structure for PCH specific SMIs Dispatch Protocol
  The PCH SMI DISPATCH PROTOCOL provides the ability to dispatch function for PCH misc SMIs.
  It contains legacy SMIs and new PCH SMI types like:
  SerialIrq, McSmi, Smbus, ...
**/
struct _PCH_SMI_DISPATCH_PROTOCOL {
  /**
    This member specifies the revision of this structure. This field is used to
    indicate backwards compatible changes to the protocol.
  **/
  UINT8                                 Revision;
  /**
    Smi unregister function for PCH SMI DISPATCH PROTOCOL.
  **/
  PCH_SMI_DISPATCH_UNREGISTER           UnRegister;
  /**
    SerialIrq
    The event is triggered while the SMI# was caused by the SERIRQ decoder.
  **/
  PCH_SMI_DISPATCH_REGISTER             SerialIrqRegister;
  /**
    McSmi
    The event is triggered if there has been an access to the power management
    microcontroller range (62h or 66h) and the Microcontroller Decode Enable #1 bit
    in the LPC Bridge I/O Enables configuration register is 1 .
  **/
  PCH_SMI_DISPATCH_REGISTER             McSmiRegister;
  /**
    SmBus
    The event is triggered while the SMI# was caused by:
    1. The SMBus Slave receiving a message that an SMI# should be caused, or
    2. The SMBALERT# signal goes active and the SMB_SMI_EN bit is set and the
       SMBALERT_DIS bit is cleared, or
    3. The SMBus Slave receiving a Host Notify message and the HOST_NOTIFY_INTREN and
       the SMB_SMI_EN bits are set, or
    4. The PCH detecting the SMLINK_SLAVE_SMI command while in the S0 state.
  **/
  PCH_SMI_DISPATCH_REGISTER             SmbusRegister;
  /**
    SPI Asynchronous
    When registered, the flash controller will generate an SMI when it blocks a BIOS write or erase.
  **/
  PCH_SMI_DISPATCH_REGISTER             SpiAsyncRegister;
};

/**
  PCH SMI dispatch revision number

  Revision 1:   Initial version
**/
#define PCH_SMI_DISPATCH_REVISION                 1

#endif
