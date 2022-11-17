/** @file

  @copyright
  Copyright 2019 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCIREBALANCE_H_
#define _PCIREBALANCE_H_

/******************************************************************************
 * Definitions.
 ******************************************************************************/
/**
  Uncomment the PCIDEBUG macro to enable tracing the library activity in a test build.
 **/
#define PCIDEBUG(...) // { DEBUG((DEBUG_INFO, "[PCI] " __VA_ARGS__)); }

typedef enum {
  SocketResourceRatioChanged,
  SocketResourceRatioNotChanged,
  SocketResourceAdjustMax
} SOCKET_RESOURCE_ADJUSTMENT_RESULT;

typedef struct {
  UINT16        IoBase;       // IO base of each stack
  UINT16        IoLimit;      // IO limit for each stack
  UINT16        NumIoPortsDesired;
  UINT64        IoAlignment;
  BOOLEAN       NeedIoUpdate; // Resource allocation required.
  UINT32        MmiolBase;    // Mmiol base of each stack
  UINT32        MmiolLimit;   // Mmiol limit of each stack
  UINT32        MmiolLength;
  UINT64        MmiolAlignment;
  UINT8         MmiolUpdate;  // Resource allocation required.
  UINT64        MmiohBase;    // Mmioh base of each stack
  UINT64        MmiohLimit;   // Mmioh limit of each stack
  UINT64        MmiohLength;
  UINT64        MmiohAlignment;
  UINT8         MmiohUpdate;  // Resource allocation required.
} STACK_RESOURCE;

typedef struct{
  UINT16      IoBase;              // Io base of each socket
  UINT16      IoLimit;             // Io limit for each socket
  UINT16      IoResourcesLeft;     // Io resources left over in socket
  UINT16      IoResourceNeeds;     // Io resources lacking in socket
  UINT32      MmiolBase;           // Mmiol base of each socket
  UINT32      MmiolLimit;          // Mmiol limit of each socket
  UINT32      MmiolResourcesLeft;  // Mmiol resources left over in socket
  UINT32      MmiolResourceNeeds;  // Mmiol resources lacking in socket
  UINT64      MmiohBase;           // Mmioh base of each socket
  UINT64      MmiohLimit;          // Mmioh limit of each socket
  UINT64      MmiohResourcesLeft;  // Mmioh resources left over in socket
  UINT64      MmiohResourceNeeds;  // Mmioh resources lacking in socket
  STACK_RESOURCE  StackRes[MAX_LOGIC_IIO_STACK];
} CPU_RESOURCE;


/******************************************************************************
 * Function prototypes.
 ******************************************************************************/
extern PCI_ROOT_BRIDGE_INSTANCE *mPciRootBridgeTable[MAX_SOCKET][MAX_LOGIC_IIO_STACK];
extern PCI_ROOT_BRIDGE_INSTANCE *mPciRootBridgeTableReserved[MAX_SOCKET][IIO_RESERVED_1];


/******************************************************************************
 * Function prototypes.
 ******************************************************************************/

/**
  Adjust resource ratio assignment among CPU sockets to fit the resource needs from PCI devices.
  Update Setup variable if there are changes from the existing ratio requests for this boot.

  @param HostBridgeInstance    -  The Host Bridge Instance where the resource adjustment happens.
  @param Result                -  Output parameter. Indicates whether changes have been made.
**/
VOID
AdjustResourceAmongRootBridges (
  IN  PCI_HOST_BRIDGE_INSTANCE          *HostBridgeInstance,
  OUT SOCKET_RESOURCE_ADJUSTMENT_RESULT *Result
  );

EFI_STATUS
AdjustSocketIo (
  IN OUT CPU_RESOURCE *SocketResources,
  IN     UINT8         ResourceType,
  IN     UINT8         ValidSockets
  );

EFI_STATUS
AdjustSocketMmioH (
  IN OUT CPU_RESOURCE *SocketResources,
  IN     UINT8         ResourceType,
  IN     UINT8         ValidSockets
  );

EFI_STATUS
AdjustSocketMmioL (
  IN OUT CPU_RESOURCE *SocketResources,
  IN     UINT8         ResourceType,
  IN     UINT8         ValidSockets
  );

/**
  Determine the last stack for a given socket

  @param  Socket the socket for which the last socket is desired

  @return The number of the last stack is returned.
*/
UINT8
LastStackOfSocket (
  IN UINT8 Socket
  );

/**
  Determine the last stack for a given socket with resources

  @param SocketResources   - CPU_RESOURCE structure pointer that stores all resources need per stack
  @param Socket            - Index of the Socket
  @param ResourceType      - Type of resource that requires alignment
  @param LastStack         - Pointer that will store the value of the last stack with resources allocated to it
  @param ResourceSize      - Pointer that will store the sum of the requested resource type

  @return The last stack with resources allocated to it and the
          total amount of resoures requested of the type
          requested.
*/
VOID
LastStackWithResources (
  IN     CPU_RESOURCE      *SocketResources,
  IN     UINT8              Socket,
  IN     PCI_RESOURCE_TYPE  ResourceType,
     OUT UINT8             *LastStack,
     OUT UINT64            *ResourceSize
  );

/**
 Find socket and stack index for given PCI Root Bridge protocol pointer.

 @param[out] PciResConfigPtr - Buffer for the resource configuration variable.

 @retval EFI_SUCCESS            The function completed successfully.
 @retval EFI_NOT_FOUND          The variable was not found.
 @retval EFI_DEVICE_ERROR       The variable could not be retrieved due to a hardware error.
 @retval EFI_SECURITY_VIOLATION The variable could not be retrieved due to an authentication failure.
**/
EFI_STATUS
PciRootBridge2SocketStack (
  IN     PCI_ROOT_BRIDGE_INSTANCE *RootBridgePtr,
     OUT UINT8                    *SocketPtr,
     OUT UINT8                    *StackPtr
  );

#endif // _PCIREBALANCE_H_

