/** @file
  Prototypes and defines for the PCH SMM Dispatcher.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_SMM_H_
#define _PCH_SMM_H_

#include <Uefi.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SmmControl2.h>
#include <Protocol/SmmUsbDispatch2.h>
#include <Protocol/SmmSxDispatch2.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Protocol/SmmGpiDispatch2.h>
#include <Protocol/SmmPowerButtonDispatch2.h>
#include <Protocol/SmmPeriodicTimerDispatch2.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/PerformanceLib.h>
#include <Protocol/SmmReadyToLock.h>
#include <IndustryStandard/Pci30.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/GpioLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchEspiLib.h>
#include <Private/Library/GpioPrivateLib.h>
#include <Protocol/PchTcoSmiDispatch.h>
#include <Protocol/PchPcieSmiDispatch.h>
#include <Protocol/PchAcpiSmiDispatch.h>
#include <Protocol/PchSmiDispatch.h>
#include <Protocol/PchEspiSmiDispatch.h>
#include <Protocol/IoTrapExDispatch.h>
#include <Library/PmcLib.h>
#include "IoTrap.h"

#define EFI_BAD_POINTER          0xAFAFAFAFAFAFAFAFULL

extern BOOLEAN                   mReadyToLock;

///
/// Define an enumeration for all the supported protocols
///
#define PCH_SMM_PROTOCOL_TYPE_MAX       6

typedef enum {
  UsbType,
  SxType,
  SwType,
  GpiType,
  PowerButtonType,
  PeriodicTimerType,
  PchSmiDispatchType,
  PchSmmProtocolTypeMax
} PCH_SMM_PROTOCOL_TYPE;

///
/// Define all the supported types of PCH SMI
///
typedef enum {
  PchTcoSmiMchType,
  PchTcoSmiTcoTimeoutType,
  PchTcoSmiOsTcoType,
  PchTcoSmiNmiType,
  PchTcoSmiIntruderDetectType,
  PchTcoSmiSpiBiosWpType,
  PchTcoSmiLpcBiosWpType,
  PchTcoSmiNewCenturyType,
  PchPcieSmiRpHotplugType,
  PchPcieSmiRpLinkActiveType,
  PchPcieSmiRpLinkEqType,
  PchAcpiSmiPmeType,
  PchAcpiSmiPmeB0Type,
  PchAcpiSmiRtcAlarmType,
  PchAcpiSmiTmrOverflowType,
  PchEspiSmiEspiSlaveType,
  PchSmiSerialIrqType,
  PchSmiMcSmiType,
  PchSmiSmBusType,
  PchSmiSpiAsyncType,
  PchIoTrapSmiType                      ///< internal SMI type
} PCH_SMI_TYPES;

///
/// Generic funciton pointer to cover all Pch SMI function pointer types
///
typedef
VOID
(EFIAPI *PCH_SMI_CALLBACK_FUNCTIONS) (
  IN EFI_HANDLE                         DispatchHandle,
  ...
  );


///
/// SPECIFYING A REGISTER
/// We want a general way of referring to addresses.  For this case, we'll only
/// need addresses in the ACPI table (and the TCO entries within the ACPI table).
/// However, it's interesting to consider what it would take to support other types
/// of addresses.  To address Will's concern, I think it prudent to accommodate it
/// early on in the design.
///
/// Addresses we need to consider:
///
///  Type:                           Required:
///  I/O                             Yes
///    ACPI (special case of I/O)    Only if we want to
///    TCO  (special case of I/O)    Only if we want to
///  GPIO  (special case of MMIO)    Only if we want to
///  Memory (or Memory Mapped I/O)   Only if we want to
///  PCIE                            Yes, for BiosWp
///
typedef enum {
  ///
  ///  IO_ADDR_TYPE, /// unimplemented
  ///
  ACPI_ADDR_TYPE,
  TCO_ADDR_TYPE,
  ///
  ///  MEMORY_ADDR_TYPE, /// unimplemented
  ///
  GPIO_ADDR_TYPE,
  MEMORY_MAPPED_IO_ADDRESS_TYPE,
  PCIE_ADDR_TYPE,
  PCR_ADDR_TYPE,
  NUM_ADDR_TYPES,                     ///< count of items in this enum
  PCH_SMM_ADDR_TYPE_NULL        = -1  ///< sentinel to indicate NULL or to signal end of arrays
} ADDR_TYPE;

//
// Assumption: 32-bits -- enum's evaluate to integer
// Assumption: This code will only run on IA-32.  Justification: IA-64 doesn't have SMIs.
// We don't have to worry about 64-bit addresses.
// Typedef the size of addresses in case the numbers I'm using are wrong or in case
// this changes.  This is a good idea because PCI_ADDR will change, for example, when
// we add support for PciExpress.
//
typedef UINT16 IO_ADDR;
typedef IO_ADDR ACPI_ADDR;  ///< can omit
typedef IO_ADDR TCO_ADDR;   ///< can omit
typedef UINTN MEM_ADDR;
typedef MEM_ADDR *MEMORY_MAPPED_IO_ADDRESS;
typedef MEM_ADDR *GPIO_ADDR;
typedef union {
  UINT32  Raw;
  struct {
    UINT32 Reg: 16;
    UINT32 Fnc: 3;
    UINT32 Dev: 5;
    UINT32 Bus: 8;
  } Fields;
} PCIE_ADDR;

typedef union {
  UINT32  Raw;
  struct {
    UINT16 Offset;
    UINT8  Pid;
    UINT8  Base;
  } Fields;
} PCR_ADDR;

typedef struct {
  ADDR_TYPE Type;
  union {
    ///
    /// used to initialize during declaration/definition
    ///
    UINT32                    raw;

    ///
    /// used to access useful data
    ///
    IO_ADDR                   io;
    ACPI_ADDR                 acpi;
    TCO_ADDR                  tco;
    GPIO_ADDR                 gpio;
    MEM_ADDR                  mem;
    MEMORY_MAPPED_IO_ADDRESS  Mmio;
    PCIE_ADDR                 pcie;
    PCR_ADDR                  Pcr;

  } Data;

} PCH_SMM_ADDRESS;

///
/// SPECIFYING BITS WITHIN A REGISTER
/// Here's a struct that helps us specify a source or enable bit.
///
typedef struct {
  PCH_SMM_ADDRESS Reg;
  UINT8           SizeInBytes;  ///< of the register
  UINT8           Bit;
} PCH_SMM_BIT_DESC;

//
// Sometimes, we'll have bit descriptions that are unused.  It'd be great to have a
// way to easily identify them:
//
#define IS_BIT_DESC_NULL(BitDesc)   ((BitDesc).Reg.Type == PCH_SMM_ADDR_TYPE_NULL)  ///< "returns" true when BitDesc is NULL
#define NULL_THIS_BIT_DESC(BitDesc) ((BitDesc).Reg.Type = PCH_SMM_ADDR_TYPE_NULL)   ///< will "return" an integer w/ value of 0
#define NULL_BIT_DESC_INITIALIZER \
  { \
    { \
      PCH_SMM_ADDR_TYPE_NULL, \
      { \
        0 \
      } \
    }, \
    0, 0 \
  }
//
// I'd like a type to specify the callback's Sts & En bits because they'll
// be commonly used together:
//
#define NUM_EN_BITS   2
#define NUM_STS_BITS  1

//
// Flags
//
typedef UINT8 PCH_SMM_SOURCE_FLAGS;

//
// Flags required to describe the event source
//
#define PCH_SMM_NO_FLAGS          0
#define PCH_SMM_SCI_EN_DEPENDENT  1

typedef struct {
  PCH_SMM_SOURCE_FLAGS  Flags;
  PCH_SMM_BIT_DESC      En[NUM_EN_BITS];    ///< Describes the enable bit(s) for the SMI event
  PCH_SMM_BIT_DESC      Sts[NUM_STS_BITS];  ///< Describes the secondary status bit for the SMI event. Might be the same as TopLevelSmi
  PCH_SMM_BIT_DESC      PmcSmiSts;          ///< Refereing to the top level status bit in PMC SMI_STS, i.e. R_PCH_SMI_STS
} PCH_SMM_SOURCE_DESC;

///
/// Used to initialize null source descriptor
///
#define NULL_SOURCE_DESC_INITIALIZER \
  { \
    PCH_SMM_NO_FLAGS, \
    { \
      NULL_BIT_DESC_INITIALIZER, NULL_BIT_DESC_INITIALIZER \
    }, \
    { \
      NULL_BIT_DESC_INITIALIZER \
    }, \
    NULL_BIT_DESC_INITIALIZER \
  }

///
/// CHILD CONTEXTS
/// To keep consistent w/ the architecture, we'll need to provide the context
/// to the child when we call its callback function.  After talking with Will,
/// we agreed that we'll need functions to "dig" the context out of the hardware
/// in many cases (Sx, Trap, Gpi, etc), and we'll need a function to compare those
/// contexts to prevent unnecessary dispatches.  I'd like a general type for these
/// "GetContext" functions, so I'll need a union of all the protocol contexts for
/// our internal use:
///
typedef union {
  //
  // (in no particular order)
  //
  EFI_SMM_SX_REGISTER_CONTEXT             Sx;
  EFI_SMM_PERIODIC_TIMER_REGISTER_CONTEXT PeriodicTimer;
  EFI_SMM_SW_REGISTER_CONTEXT             Sw;
  EFI_SMM_POWER_BUTTON_REGISTER_CONTEXT   PowerButton;
  EFI_SMM_USB_REGISTER_CONTEXT            Usb;
  EFI_SMM_GPI_REGISTER_CONTEXT            Gpi;
} PCH_SMM_CONTEXT;

///
/// Misc data for PchDispatcher usage.
/// For PeriodicTimer, since the ElapsedTime is removed from EFI_SMM_PERIODIC_TIMER_REGISTER_CONTEXT of EDKII,
/// and PchDispatcher needs it for every record. Thus move it here to support ElapsedTime.
///
typedef union {
  UINTN ElapsedTime;
} PCH_SMM_MISC_DATA;

//
// Assumption: PeriodicTimer largest at 3x64-bits or 24 bytes
//
typedef struct _DATABASE_RECORD DATABASE_RECORD;

///
/// Assumption: the GET_CONTEXT function will be as small and simple as possible.
/// Assumption: We don't need to pass in an enumeration for the protocol because each
///    GET_CONTEXT function is written for only one protocol.
/// We also need a function to compare contexts to see if the child should be dispatched
/// In addition, we need a function to acquire CommBuffer and CommBufferSize for
///    dispatch callback function of EDKII native support.
///
typedef
VOID
(EFIAPI *GET_CONTEXT) (
  IN  DATABASE_RECORD    * Record,
  OUT PCH_SMM_CONTEXT    * Context
  );

typedef
BOOLEAN
(EFIAPI *CMP_CONTEXT) (
  IN PCH_SMM_CONTEXT     * Context1,
  IN PCH_SMM_CONTEXT     * Context2
  );

typedef
VOID
(EFIAPI *GET_COMMBUFFER) (
  IN  DATABASE_RECORD    * Record,
  OUT VOID               **CommBuffer,
  OUT UINTN              * CommBufferSize
  );

///
/// Finally, every protocol will require a "Get Context" and "Compare Context" call, so
/// we may as well wrap that up in a table, too.
///
typedef struct {
  GET_CONTEXT     GetContext;
  CMP_CONTEXT     CmpContext;
  GET_COMMBUFFER  GetCommBuffer;
} CONTEXT_FUNCTIONS;

extern CONTEXT_FUNCTIONS          ContextFunctions[PCH_SMM_PROTOCOL_TYPE_MAX];

///
/// MAPPING CONTEXT TO BIT DESCRIPTIONS
/// I'd like to have a general approach to mapping contexts to bit descriptions.
/// Sometimes, we'll find that we can use table lookups or constant assignments;
/// other times, we'll find that we'll need to use a function to perform the mapping.
/// If we define a macro to mask that process, we'll never have to change the code.
/// I don't know if this is desirable or not -- if it isn't, then we can get rid
/// of the macros and just use function calls or variable assignments.  Doesn't matter
/// to me.
/// Mapping complex contexts requires a function
///

/**
  Maps a USB context to a source description.

  @param[in] Context              The context we need to map.  Type must be USB.
  @param[out] SrcDesc             The source description that corresponds to the given context.

**/
VOID
MapUsbToSrcDesc (
  IN  PCH_SMM_CONTEXT         *Context,
  OUT PCH_SMM_SOURCE_DESC     *SrcDesc
  );

/**
  Figure out which timer the child is requesting and
  send back the source description

  @param[in] DispatchContext      The pointer to the Dispatch Context instances
  @param[out] SrcDesc             The pointer to the source description

**/
VOID
MapPeriodicTimerToSrcDesc (
  IN  PCH_SMM_CONTEXT                                         *DispatchContext,
  OUT PCH_SMM_SOURCE_DESC                                     *SrcDesc
  );

//
// Mapping simple contexts can be done by assignment or lookup table
//
extern CONST PCH_SMM_SOURCE_DESC  mSxSourceDesc;
extern CONST PCH_SMM_SOURCE_DESC  mPowerButtonSourceDesc;
extern CONST PCH_SMM_SOURCE_DESC  mSrcDescNewCentury;
extern CONST PCH_SMM_SOURCE_DESC  mGpiSourceDescTemplate;

///
/// For PCHx, APMC is UINT8 port, so the MAX SWI Value is 0xFF.
///
#define MAXIMUM_SWI_VALUE 0xFF
///
/// Open: Need to make sure this kind of type cast will actually work.
///   May need an intermediate form w/ two VOID* arguments.  I'll figure
///   that out when I start compiling.
///
typedef
VOID
(EFIAPI *PCH_SMM_CLEAR_SOURCE) (
  CONST PCH_SMM_SOURCE_DESC * SrcDesc
  );

///
/// "DATABASE" RECORD
/// Linked list data structures
///
#define DATABASE_RECORD_SIGNATURE SIGNATURE_32 ('D', 'B', 'R', 'C')

struct _DATABASE_RECORD {
  UINT32                        Signature;
  LIST_ENTRY                    Link;
  BOOLEAN                       Processed;
  ///
  /// Status and Enable bit description
  ///
  PCH_SMM_SOURCE_DESC           SrcDesc;

  ///
  /// Callback function
  ///
  EFI_SMM_HANDLER_ENTRY_POINT2  Callback;
  PCH_SMM_CONTEXT               ChildContext;
  UINTN                         ContextSize;

  ///
  /// Special handling hooks -- init them to NULL if unused/unneeded
  ///
  PCH_SMM_CLEAR_SOURCE          ClearSource;

  ///
  /// Functions required to make callback code general
  ///
  CONTEXT_FUNCTIONS             ContextFunctions;

  ///
  /// The protocol that this record dispatches
  ///
  PCH_SMM_PROTOCOL_TYPE         ProtocolType;

  ///
  /// Misc data for private usage
  ///
  PCH_SMM_MISC_DATA             MiscData;

  ///
  /// PCH SMI callback function
  ///
  PCH_SMI_CALLBACK_FUNCTIONS    PchSmiCallback;
  ///
  /// Indicate the PCH SMI types.
  ///
  PCH_SMI_TYPES                 PchSmiType;
};

#define DATABASE_RECORD_FROM_LINK(_record)  CR (_record, DATABASE_RECORD, Link, DATABASE_RECORD_SIGNATURE)
#define DATABASE_RECORD_FROM_CHILDCONTEXT(_record)  CR (_record, DATABASE_RECORD, ChildContext, DATABASE_RECORD_SIGNATURE)

///
/// HOOKING INTO THE ARCHITECTURE
///
typedef
EFI_STATUS
(EFIAPI *PCH_SMM_GENERIC_REGISTER) (
  IN  VOID                                    **This,
  IN  VOID                                    *DispatchFunction,
  IN  VOID                                    *DispatchContext,
  OUT EFI_HANDLE                              *DispatchHandle
  );
typedef
EFI_STATUS
(EFIAPI *PCH_SMM_GENERIC_UNREGISTER) (
  IN  VOID                                    **This,
  IN  EFI_HANDLE                              DispatchHandle
  );

///
/// Define a memory "stamp" equivalent in size and function to most of the protocols
///
typedef struct {
  PCH_SMM_GENERIC_REGISTER    Register;
  PCH_SMM_GENERIC_UNREGISTER  Unregister;
  UINTN                       Extra1;
  UINTN                       Extra2; ///< may not need this one
} PCH_SMM_GENERIC_PROTOCOL;

/**
  Register a child SMI dispatch function with a parent SMM driver.

  @param[in] This                 Pointer to the PCH_SMM_GENERIC_PROTOCOL instance.
  @param[in] DispatchFunction     Pointer to dispatch function to be invoked for this SMI source.
  @param[in] DispatchContext      Pointer to the dispatch function's context.
  @param[out] DispatchHandle      Handle of dispatch function, for when interfacing
                                  with the parent SMM driver, will be the address of linked
                                  list link in the call back record.

  @retval EFI_OUT_OF_RESOURCES    Insufficient resources to create database record
  @retval EFI_INVALID_PARAMETER   The input parameter is invalid
  @retval EFI_SUCCESS             The dispatch function has been successfully
                                  registered and the SMI source has been enabled.
**/
EFI_STATUS
EFIAPI
PchPiSmmCoreRegister (
  IN  PCH_SMM_GENERIC_PROTOCOL                          *This,
  IN  EFI_SMM_HANDLER_ENTRY_POINT2                      DispatchFunction,
  IN  PCH_SMM_CONTEXT                                   *DispatchContext,
  OUT EFI_HANDLE                                        *DispatchHandle
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
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchPiSmmCoreUnRegister (
  IN PCH_SMM_GENERIC_PROTOCOL                           *This,
  IN EFI_HANDLE                                         *DispatchHandle
  );

/**
  Unregister a child SMI source dispatch function with a parent SMM driver.

  @param[in] This                 Pointer to the PCH_SMM_GENERIC_PROTOCOL instance.
  @param[in] DispatchHandle       Handle of dispatch function to deregister.

  @retval EFI_SUCCESS             The dispatch function has been successfully
                                  unregistered and the SMI source has been disabled
                                  if there are no other registered child dispatch
                                  functions for this SMI source.
  @retval EFI_INVALID_PARAMETER   Handle is invalid.
**/
EFI_STATUS
EFIAPI
PchSmmCoreUnRegister (
  IN  PCH_SMM_GENERIC_PROTOCOL                         *This,
  IN  EFI_HANDLE                                       *DispatchHandle
  );

typedef union {
  PCH_SMM_GENERIC_PROTOCOL                    Generic;
  EFI_SMM_USB_DISPATCH2_PROTOCOL              Usb;
  EFI_SMM_SX_DISPATCH2_PROTOCOL               Sx;
  EFI_SMM_SW_DISPATCH2_PROTOCOL               Sw;
  EFI_SMM_GPI_DISPATCH2_PROTOCOL              Gpi;
  EFI_SMM_POWER_BUTTON_DISPATCH2_PROTOCOL     PowerButton;
  EFI_SMM_PERIODIC_TIMER_DISPATCH2_PROTOCOL   PeriodicTimer;
} PCH_SMM_PROTOCOL;

///
/// Define a structure to help us identify the generic protocol
///
#define PROTOCOL_SIGNATURE  SIGNATURE_32 ('P', 'R', 'O', 'T')

typedef struct {
  UINTN                 Signature;

  PCH_SMM_PROTOCOL_TYPE Type;
  EFI_GUID              *Guid;
  PCH_SMM_PROTOCOL      Protocols;
} PCH_SMM_QUALIFIED_PROTOCOL;

#define QUALIFIED_PROTOCOL_FROM_GENERIC(_generic) \
  CR ( \
  _generic, \
  PCH_SMM_QUALIFIED_PROTOCOL, \
  Protocols, \
  PROTOCOL_SIGNATURE \
  )

///
/// Create private data for the protocols that we'll publish
///
typedef struct {
  LIST_ENTRY                  CallbackDataBase;
  EFI_HANDLE                  SmiHandle;
  EFI_HANDLE                  InstallMultProtHandle;
  PCH_SMM_QUALIFIED_PROTOCOL  Protocols[PCH_SMM_PROTOCOL_TYPE_MAX];
} PRIVATE_DATA;

extern PRIVATE_DATA           mPrivateData;
extern UINT16                 mAcpiBaseAddr;
extern UINT16                 mTcoBaseAddr;

/**
  The internal function used to create and insert a database record

  @param[in]  InsertRecord              Record to insert to database.
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
**/
EFI_STATUS
SmmCoreInsertRecord (
  IN  DATABASE_RECORD                   *NewRecord,
  OUT EFI_HANDLE                        *DispatchHandle
  );

/**
  Get the Sleep type

  @param[in] Record               No use
  @param[out] Context             The context that includes SLP_TYP bits to be filled
**/
VOID
EFIAPI
SxGetContext (
  IN  DATABASE_RECORD    *Record,
  OUT PCH_SMM_CONTEXT    *Context
  );

/**
  Register a child SMI source dispatch function for the specified software SMI.

  This service registers a function (DispatchFunction) which will be called when the software
  SMI source specified by RegisterContext->SwSmiCpuIndex is detected. On return,
  DispatchHandle contains a unique handle which may be used later to unregister the function
  using UnRegister().

  @param[in]  This                 Pointer to the EFI_SMM_SW_DISPATCH2_PROTOCOL instance.
  @param[in]  DispatchFunction     Function to register for handler when the specified software
                                   SMI is generated.
  @param[in, out] RegisterContext  Pointer to the dispatch function's context.
                                   The caller fills this context in before calling
                                   the register function to indicate to the register
                                   function which Software SMI input value the
                                   dispatch function should be invoked for.
  @param[out] DispatchHandle       Handle generated by the dispatcher to track the
                                   function instance.

  @retval EFI_SUCCESS            The dispatch function has been successfully
                                 registered and the SMI source has been enabled.
  @retval EFI_DEVICE_ERROR       The SW driver was unable to enable the SMI source.
  @retval EFI_INVALID_PARAMETER  RegisterContext is invalid. The SW SMI input value
                                 is not within a valid range or is already in use.
  @retval EFI_OUT_OF_RESOURCES   There is not enough memory (system or SMM) to manage this
                                 child.
  @retval EFI_OUT_OF_RESOURCES   A unique software SMI value could not be assigned
                                 for this dispatch.
**/
EFI_STATUS
EFIAPI
PchSwSmiRegister (
  IN  EFI_SMM_SW_DISPATCH2_PROTOCOL       *This,
  IN  EFI_SMM_HANDLER_ENTRY_POINT2        DispatchFunction,
  IN  EFI_SMM_SW_REGISTER_CONTEXT         *DispatchContext,
  OUT EFI_HANDLE                          *DispatchHandle
  );

/**
  Unregister a child SMI source dispatch function for the specified software SMI.

  This service removes the handler associated with DispatchHandle so that it will no longer be
  called in response to a software SMI.

  @param[in] This                Pointer to the EFI_SMM_SW_DISPATCH2_PROTOCOL instance.
  @param[in] DispatchHandle      Handle of dispatch function to deregister.

  @retval EFI_SUCCESS            The dispatch function has been successfully unregistered.
  @retval EFI_INVALID_PARAMETER  The DispatchHandle was not valid.
**/
EFI_STATUS
EFIAPI
PchSwSmiUnRegister (
  IN CONST EFI_SMM_SW_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                     DispatchHandle
  );

/**
  Init required protocol for Pch Sw Dispatch protocol.
**/
VOID
PchSwDispatchInit (
  VOID
  );

/**
  Check whether sleep type of two contexts match

  @param[in] Context1             Context 1 that includes sleep type 1
  @param[in] Context2             Context 2 that includes sleep type 2

  @retval FALSE                   Sleep types match
  @retval TRUE                    Sleep types don't match
**/
BOOLEAN
EFIAPI
SxCmpContext (
  IN PCH_SMM_CONTEXT     *Context1,
  IN PCH_SMM_CONTEXT     *Context2
  );

/**
  Update the elapsed time from the Interval data of DATABASE_RECORD

  @param[in] Record               The pointer to the DATABASE_RECORD.
  @param[out] HwContext           The Context to be updated.
**/
VOID
EFIAPI
PeriodicTimerGetContext (
  IN  DATABASE_RECORD    *Record,
  OUT PCH_SMM_CONTEXT    *Context
  );

/**
  Check whether Periodic Timer of two contexts match

  @param[in] Context1             Context 1 that includes Periodic Timer  1
  @param[in] Context2             Context 2 that includes Periodic Timer  2

  @retval FALSE                   Periodic Timer match
  @retval TRUE                    Periodic Timer don't match
**/
BOOLEAN
EFIAPI
PeriodicTimerCmpContext (
  IN PCH_SMM_CONTEXT     *Context1,
  IN PCH_SMM_CONTEXT     *Context2
  );

/**
  Gather the CommBuffer information of SmmPeriodicTimerDispatch2.

  @param[in]  Record              No use
  @param[out] CommBuffer          Point to the CommBuffer structure
  @param[out] CommBufferSize      Point to the Size of CommBuffer structure
**/
VOID
EFIAPI
PeriodicTimerGetCommBuffer (
  IN  DATABASE_RECORD    *Record,
  OUT VOID               **CommBuffer,
  OUT UINTN              *CommBufferSize
  );

/**
  Get the power button status.

  @param[in] Record               The pointer to the DATABASE_RECORD.
  @param[out] Context             Calling context from the hardware, will be updated with the current power button status.
**/
VOID
EFIAPI
PowerButtonGetContext (
  IN  DATABASE_RECORD    *Record,
  OUT PCH_SMM_CONTEXT    *Context
  );

/**
  Check whether Power Button status of two contexts match

  @param[in] Context1             Context 1 that includes Power Button status 1
  @param[in] Context2             Context 2 that includes Power Button status 2

  @retval FALSE                   Power Button status match
  @retval TRUE                    Power Button status don't match
**/
BOOLEAN
EFIAPI
PowerButtonCmpContext (
  IN PCH_SMM_CONTEXT     *Context1,
  IN PCH_SMM_CONTEXT     *Context2
  );

/**
  This function is responsible for calculating and enabling any timers that are required
  to dispatch messages to children. The SrcDesc argument isn't acutally used.

  @param[in] SrcDesc              Pointer to the PCH_SMM_SOURCE_DESC instance.
**/
VOID
EFIAPI
PchSmmPeriodicTimerClearSource (
  IN CONST PCH_SMM_SOURCE_DESC    *SrcDesc
  );

/**
  This services returns the next SMI tick period that is supported by the chipset.
  The order returned is from longest to shortest interval period.

  @param[in] This                 Pointer to the EFI_SMM_PERIODIC_TIMER_DISPATCH2_PROTOCOL instance.
  @param[in, out] SmiTickInterval Pointer to pointer of the next shorter SMI interval period that is supported by the child.

  @retval EFI_SUCCESS             The service returned successfully.
  @retval EFI_INVALID_PARAMETER   The parameter SmiTickInterval is invalid.
**/
EFI_STATUS
PchSmmPeriodicTimerDispatchGetNextShorterInterval (
  IN CONST EFI_SMM_PERIODIC_TIMER_DISPATCH2_PROTOCOL    *This,
  IN OUT UINT64                                         **SmiTickInterval
  );

/**
  Install PCH SMM periodic timer control protocol

  @param[in] Handle                     handle for this driver

  @retval EFI_SUCCESS                   Driver initialization completed successfully
**/
EFI_STATUS
EFIAPI
InstallPchSmmPeriodicTimerControlProtocol (
  IN EFI_HANDLE                         Handle
  );

/**
  When we get an SMI that indicates that we are transitioning to a sleep state,
  we need to actually transition to that state.  We do this by disabling the
  "SMI on sleep enable" feature, which generates an SMI when the operating system
  tries to put the system to sleep, and then physically putting the system to sleep.
**/
VOID
PchSmmSxGoToSleep (
  VOID
  );

/**
  Install protocols of PCH specifics SMI types, including
  PCH TCO SMI types, PCH PCIE SMI types, PCH ACPI SMI types, PCH MISC SMI types.

  @retval                               the result of protocol installation
**/
EFI_STATUS
InstallPchSmiDispatchProtocols (
  VOID
  );

/**
  The function to dispatch all callback function of PCH SMI types.

  @retval EFI_SUCCESS                   Function successfully completed
  @retval EFI_UNSUPPORTED               no
**/
EFI_STATUS
PchSmiTypeCallbackDispatcher (
  IN  DATABASE_RECORD                   *Record
  );

/**
  The register function used to register SMI handler of IoTrap event.
  This is internal function and only used by Iotrap module.

  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[in]  IoTrapIndex               Index number of IOTRAP register
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
**/
EFI_STATUS
PchInternalIoTrapSmiRegister (
  IN  PCH_SMI_DISPATCH_CALLBACK         DispatchFunction,
  IN  UINTN                             IoTrapIndex,
  OUT EFI_HANDLE                        *DispatchHandle
  );

/**
  Unregister a child SMI source dispatch function with a parent SMM driver

  @param[in] DispatchHandle             Handle of dispatch function to deregister.

  @retval EFI_SUCCESS                   The dispatch function has been successfully
                                        unregistered and the SMI source has been disabled
                                        if there are no other registered child dispatch
                                        functions for this SMI source.
  @retval EFI_INVALID_PARAMETER         Handle is invalid.
**/
EFI_STATUS
PchInternalIoTrapSmiUnRegister (
  IN  EFI_HANDLE                        DispatchHandle
  );

/**
  Register an eSPI SMI handler based on the type

  @param[in]  DispatchFunction        Callback in an event of eSPI SMI
  @param[in]  PchSmiTypes             The eSPI type published by PchSmiDispatch
  @param[out] DispatchHandle          The callback handle

  @retval     EFI_INVALID_PARAMETER   Error with NULL SMI source description
  @retval     EFI_OUT_OF_RESOURCES    Fail to allocate pool for database record
  @retval     EFI_SUCCESS             Registration is successful.
**/
EFI_STATUS
PchInternalEspiSmiRegister (
  IN  PCH_SMI_DISPATCH_CALLBACK         DispatchFunction,
  IN  PCH_SMI_TYPES                     PchSmiTypes,
  OUT EFI_HANDLE                        *DispatchHandle
  );

/**
  Unregister an eSPI SMI handler

  @param[in] DispatchHandle             Handle of dispatch function to deregister.

  @retval EFI_SUCCESS                   The dispatch function has been successfully
                                        unregistered and the SMI source has been disabled
                                        if there are no other registered child dispatch
                                        functions for this SMI source.
  @retval EFI_INVALID_PARAMETER         Handle is invalid.
**/
EFI_STATUS
PchInternalEspiSmiUnRegister (
  IN  EFI_HANDLE                        DispatchHandle
  );

/**
  The internal function used to create and insert a database record
  for SMI record of Pch Smi types.

  @param[in]  SrcDesc                   The pointer to the SMI source description
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[in]  PchSmiType                Specific SMI type of PCH SMI
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
**/
EFI_STATUS
PchSmiRecordInsert (
  IN  CONST PCH_SMM_SOURCE_DESC         *SrcDesc,
  IN  PCH_SMI_CALLBACK_FUNCTIONS        DispatchFunction,
  IN  PCH_SMI_TYPES                     PchSmiType,
  OUT EFI_HANDLE                        *DispatchHandle
  );

extern CONST PCH_SMM_SOURCE_DESC mSrcDescSerialIrq;
extern CONST PCH_SMM_SOURCE_DESC mSrcDescLpcBiosWp;

/**
  Clear the TCO SMI status bit and block after the SMI handling is done

  @param[in] SrcDesc                    Pointer to the PCH SMI source description table

**/
VOID
EFIAPI
PchTcoSmiClearSourceAndBlock (
  CONST PCH_SMM_SOURCE_DESC             *SrcDesc
  );

/**
  Clear the TCO SMI status bit after the SMI handling is done

  @param[in] SrcDesc                    Pointer to the PCH SMI source description table

**/
VOID
EFIAPI
PchTcoSmiClearSource (
  CONST PCH_SMM_SOURCE_DESC             *SrcDesc
  );

/**
  Initialize Source descriptor structure

  @param[in] SrcDesc                    Pointer to the PCH SMI source description table
**/
VOID
EFIAPI
NullInitSourceDesc (
  PCH_SMM_SOURCE_DESC                   *SrcDesc
  );

/**
  The register function used to register SMI handler of GPI SMI event.

  @param[in]  This               Pointer to the EFI_SMM_GPI_DISPATCH2_PROTOCOL instance.
  @param[in]  DispatchFunction   Function to register for handler when the specified GPI causes an SMI.
  @param[in]  RegisterContext    Pointer to the dispatch function's context.
                                 The caller fills this context in before calling
                                 the register function to indicate to the register
                                 function the GPI(s) for which the dispatch function
                                 should be invoked.
  @param[out] DispatchHandle     Handle generated by the dispatcher to track the
                                 function instance.

  @retval EFI_SUCCESS            The dispatch function has been successfully
                                 registered and the SMI source has been enabled.
  @retval EFI_ACCESS_DENIED      Register is not allowed
  @retval EFI_INVALID_PARAMETER  RegisterContext is invalid. The GPI input value
                                 is not within valid range.
  @retval EFI_OUT_OF_RESOURCES   There is not enough memory (system or SMM) to manage this child.
**/
EFI_STATUS
EFIAPI
PchGpiSmiRegister (
  IN CONST EFI_SMM_GPI_DISPATCH2_PROTOCOL  *This,
  IN       EFI_SMM_HANDLER_ENTRY_POINT2    DispatchFunction,
  IN CONST EFI_SMM_GPI_REGISTER_CONTEXT    *RegisterContext,
  OUT      EFI_HANDLE                      *DispatchHandle
  );

/**
  Unregister a GPI SMI source dispatch function with a parent SMM driver

  @param[in] This                 Pointer to the EFI_SMM_GPI_DISPATCH2_PROTOCOL instance.
  @param[in] DispatchHandle       Handle of dispatch function to deregister.

  @retval EFI_SUCCESS             The dispatch function has been successfully
                                  unregistered and the SMI source has been disabled
                                  if there are no other registered child dispatch
                                  functions for this SMI source.
  @retval EFI_INVALID_PARAMETER   Handle is invalid.
**/
EFI_STATUS
EFIAPI
PchGpiSmiUnRegister (
  IN CONST EFI_SMM_GPI_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                      DispatchHandle
  );

#endif
