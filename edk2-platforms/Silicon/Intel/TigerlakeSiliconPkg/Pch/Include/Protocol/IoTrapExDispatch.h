/** @file
  PCH IO TrapEx Dispatch Protocol

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _IO_TRAP_EX_DISPATCH_H_
#define _IO_TRAP_EX_DISPATCH_H_

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                                       gIoTrapExDispatchProtocolGuid;

typedef struct _IO_TRAP_EX_DISPATCH_PROTOCOL          IO_TRAP_EX_DISPATCH_PROTOCOL;

/**
  IO Trap Ex valid types
**/
typedef enum {
  IoTrapExTypeWrite,
  IoTrapExTypeRead,
  IoTrapExTypeReadWrite,
  IoTrapExTypeMaximum
} IO_TRAP_EX_DISPATCH_TYPE;

/**
  IO Trap Ex context structure containing information about the
  IO trap Ex event that should invoke the handler.
  ByteEnableMask bitwise to ignore the ByteEnable setting. E.g. 1111b for any byte access.

  Here are some examples for the usage.
  1.  To trigger the TRAP for the IO address from 0x2000 to 0x20FF with BYTE/WORD/DWORD read/write access:
      Address        = 0x2000
      Length         = 0x100
      Type           = IoTrapExTypeReadWrite
      ByteEnable     = 0x00 (BE is not matter)
      ByteEnableMask = 0x0F (BEM 0xF for any BYTE/WORD/DWORD access)
  2.  To trigger the TRAP for port 0x61 with BYTE read access:
      Address        = 0x60
      Length         = 4
      Type           = IoTrapExTypeRead
      ByteEnable     = 0x02 (BE is 0010b to trap only second byte of every DWORD)
      ByteEnableMask = 0x00 (BEM doesn't mask any BE bit)
  3.  To trigger the TRAP for port 0x60 and 0x64 with BYTE write access:
      Address        = 0x60
      Length         = 8
      Type           = IoTrapExTypeWrite
      ByteEnable     = 0x01 (BE is 0001b to trap only first byte of every DWORD)
      ByteEnableMask = 0x00 (BEM doesn't mask any BE bit)
**/
typedef struct {
  /**
    The Address must be dword alignment.
  **/
  UINT16                                Address;
  UINT16                                Length;
  IO_TRAP_EX_DISPATCH_TYPE              Type;
  /**
    Bitmap to enable trap for each byte of every dword alignment address.
    The Io Trap Address must be dword alignment for ByteEnable.
    E.g. 0001b for first byte, 0010b for second byte, 1100b for third and fourth byte.
  **/
  UINT8                                 ByteEnable;
  /**
    ByteEnableMask bitwise to ignore the ByteEnable setting. E.g. 1111b for any byte access.
    The Io Trap Address must be dword alignment for ByteEnableMask.
  **/
  UINT8                                 ByteEnableMask;
} IO_TRAP_EX_REGISTER_CONTEXT;

/**
  Callback function for an PCH IO TRAP EX handler dispatch.

  @param[in] Address                    DWord-aligned address of the trapped cycle.
  @param[in] ByteEnable                 This is the DWord-aligned byte enables associated with the trapped cycle.
                                        A 1 in any bit location indicates that the corresponding byte is enabled in the cycle.
  @param[in] WriteCycle                 TRUE = Write cycle; FALSE = Read cycle
  @param[in] WriteData                  DWord of I/O write data. This field is undefined after trapping a read cycle.
                                        The byte of WriteData is only valid if the corresponding bits in ByteEnable is 1.
                                        E.g.
                                        If ByteEnable is 0001b, then only first byte of WriteData is valid.
                                        If ByteEnable is 0010b, then only second byte of WriteData is valid.
**/
typedef
VOID
(EFIAPI *IO_TRAP_EX_DISPATCH_CALLBACK) (
  IN UINT16                             Address,
  IN UINT8                              ByteEnable,
  IN BOOLEAN                            WriteCycle,
  IN UINT32                             WriteData
  );

/**
  Register a new IO Trap Ex SMI dispatch function.
  The caller will provide information of IO trap setting via the context.
  Please consider to use EfiSmmIoTrapDispatch2Protocol as possible.
  This is the function to extend the IoTrap capability, and it's expected
  to handle the special ByteEnable and ByteEnableMask setting.
  This register function will occupy one IoTrap register if possible.
  And it only support one handler for one IoTrap event.
  The Address of context MUST NOT be 0, and MUST be dword alignment.
  The Length of context MUST not less than 4, and MUST be power of 2.
  The ByteEnable and ByteEnableMask MUST not be zero at the same time.
  if the IO Trap handler is not used. It also enable the IO Trap Range to generate
  SMI.
  Caller must take care of reserving the IO addresses in ACPI.

  @param[in] This                 Pointer to the IO_TRAP_EX_DISPATCH_PROTOCOL instance.
  @param[in] DispatchFunction     Pointer to dispatch function to be invoked for
                                  this SMI source.
  @param[in] RegisterContext      Pointer to the dispatch function's context.
                                  The caller fills this context in before calling
                                  the register function to indicate to the register
                                  function the IO trap Ex SMI source for which the dispatch
                                  function should be invoked.  This MUST not be NULL.
  @param[out] DispatchHandle      Handle of dispatch function.

  @retval EFI_SUCCESS             The dispatch function has been successfully
                                  registered and the SMI source has been enabled.
  @retval EFI_OUT_OF_RESOURCES    Insufficient resources are available
  @retval EFI_INVALID_PARAMETER   Address requested is already in use.
  @retval EFI_ACCESS_DENIED       Return access denied if the SmmReadyToLock event has been triggered
**/
typedef
EFI_STATUS
(EFIAPI *IO_TRAP_EX_DISPATCH_REGISTER) (
  IN  IO_TRAP_EX_DISPATCH_PROTOCOL  *This,
  IN  IO_TRAP_EX_DISPATCH_CALLBACK  DispatchFunction,
  IN  IO_TRAP_EX_REGISTER_CONTEXT   *RegisterContext,
  OUT EFI_HANDLE                    *DispatchHandle
  );

/**
  Unregister a SMI source dispatch function.
  This function is unsupported.

  @param[in] This                 Pointer to the IO_TRAP_EX_DISPATCH_PROTOCOL instance.
  @param[in] DispatchHandle       Handle of dispatch function to deregister.

  @retval EFI_UNSUPPORTED         The function is unsupported.
**/
typedef
EFI_STATUS
(EFIAPI *IO_TRAP_EX_DISPATCH_UNREGISTER) (
  IN  IO_TRAP_EX_DISPATCH_PROTOCOL  *This,
  IN  EFI_HANDLE                    DispatchHandle
  );

/**
  Interface structure for the IO trap Extention protocol.
  This protocol exposes full IO TRAP capability for ByteEnable and ByteEnableMask setting.
  Platform code should fully control the ByteEnable and ByteEnableMake while using this protocol.

  Please consider to use EfiSmmIoTrapDispatch2Protocol as possible.
  This is the function to extend the IoTrap capability, and it's expected
  to handle the special ByteEnable and ByteEnableMask setting.

  The protocol is low level, It returns PSTH trapped cycle. This might not be safe for multithread
  if more than one thread triggers the same IOTRAP at the same time.
**/
struct _IO_TRAP_EX_DISPATCH_PROTOCOL {
  /**
    Register function for PCH IO TRAP EX DISPATCH PROTOCOL.
    The caller will provide information of IO trap setting via the context.
    Please consider to use EfiSmmIoTrapDispatch2Protocol as possible.
    This is the function to extend the IoTrap capability, and it's expected
    to handle the special ByteEnable and ByteEnableMask setting.
    This register function will occupy one IoTrap register if possible.
    And it only support one handler for one IoTrap event.
    The Address of context MUST NOT be 0, and MUST be dword alignment.
    The Length of context MUST not less than 4, and MUST be power of 2.
    The ByteEnable and ByteEnableMask MUST not be zero at the same time.
    if the IO Trap handler is not used. It also enable the IO Trap Range to
    generate SMI.
  **/
  IO_TRAP_EX_DISPATCH_REGISTER      Register;
  /**
    Unregister function for PCH IO TRAP EX DISPATCH PROTOCOL.
  **/
  IO_TRAP_EX_DISPATCH_UNREGISTER    UnRegister;
};

#endif
