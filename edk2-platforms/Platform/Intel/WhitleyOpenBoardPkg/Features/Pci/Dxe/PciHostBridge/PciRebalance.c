/** @file

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Guid/SocketPciResourceData.h>
#include <Guid/SocketIioVariable.h>
#include "PciHostBridge.h"
#include "PciRootBridge.h"
#include <CpuAndRevisionDefines.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/IioUds.h>

#include "PciRebalance.h"


/******************************************************************************
 * Local definitions.
 ******************************************************************************/
extern CHAR16                           *mAcpiAddressSpaceTypeStr[];
extern CHAR16                           *mPciResourceTypeStr[];

extern EFI_IIO_UDS_PROTOCOL              *mIioUds;

/******************************************************************************
 * Variables.
 ******************************************************************************/
/**
 * The table below is a cache with pointers to protocol instances created at
 * Host Bridge initialization. It also provides mapping of protocol instance
 * to the PCI stack.
 */
PCI_ROOT_BRIDGE_INSTANCE *mPciRootBridgeTable[MAX_SOCKET][MAX_LOGIC_IIO_STACK] = {0};
PCI_ROOT_BRIDGE_INSTANCE *mPciRootBridgeTableReserved[MAX_SOCKET][IIO_RESERVED_1] = {0};


/******************************************************************************
 * Functions.
 ******************************************************************************/

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
  )
{
  UINT8  Socket;
  UINT8  Stack;

  if (RootBridgePtr != NULL) {

    for (Socket = 0; Socket < NELEMENTS(mPciRootBridgeTable); Socket++) {

      for (Stack = 0; Stack < NELEMENTS(mPciRootBridgeTable[Socket]); Stack++) {

        if (mPciRootBridgeTable[Socket][Stack] == RootBridgePtr) {
          if (SocketPtr != NULL) {
            *SocketPtr = Socket;
          }
          if (StackPtr != NULL) {
            *StackPtr = Stack;
          }
          return EFI_SUCCESS;
        }
      }
    }
  }
  return EFI_NOT_FOUND;
}

/**
  Determine the last stack for a given socket

  @param  Socket the socket for which the last socket is desired

  @return the number of the last socket
*/
UINT8
LastStackOfSocket (
  UINT8 Socket
  )
{
  UINT8   LastStack;
  BOOLEAN FoundEnabledStack;
  UINT8   Stack;

  ASSERT (Socket < ARRAY_SIZE(mIioUds->IioUdsPtr->PlatformData.IIO_resource));
  ASSERT (mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].Valid);

  FoundEnabledStack = FALSE;
  LastStack = 0;

  for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
    if (mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1 << Stack)) {
      LastStack = Stack;
      FoundEnabledStack = TRUE;
    }
  }

  ASSERT (FoundEnabledStack);
  return LastStack;
}


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
  IN CPU_RESOURCE       *SocketResources,
  IN UINT8              Socket,
  IN PCI_RESOURCE_TYPE  ResourceType,
  OUT UINT8             *LastStack,
  OUT UINT64            *ResourceSize
  )
{
  UINT8   Stack;

  *LastStack    = 0;
  *ResourceSize = 0;

  for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
    if (!(mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1 << Stack))) {
      continue;
    }

    switch (ResourceType) {
      case TypeIo:
        if (SocketResources->StackRes[Stack].NumIoPortsDesired != 0) {
          *ResourceSize += SocketResources->StackRes[Stack].NumIoPortsDesired + 1;
          *LastStack = Stack;
        }
        break;
      case TypeMem32:
        if (SocketResources->StackRes[Stack].MmiolLength != 0) {
          *ResourceSize += SocketResources->StackRes[Stack].MmiolLength + 1;
          *LastStack = Stack;
        }
        break;
      case TypeMem64:
        if (SocketResources->StackRes[Stack].MmiohLength != 0) {
          *ResourceSize += SocketResources->StackRes[Stack].MmiohLength + 1;
          *LastStack = Stack;
        }
        break;
      default:
        break;
    }
  }
}


/**
  Visit all stacks in this socket and recalculate the resource ranges per stack based on resource
  needs from PCI/PCIe device/functions.

  @param SocketResources   -  CPU_RESOURCE structure pointer that stores all resources need per stack
  @param Socket            -  Index of the Socket
  @param ResourceType      -  type of resource that requires alignment

  @retval EFI_SUCCESS            -  Succeed.
  @retval EFI_OUT_OF_RESOURCES   -  Not enough resources to be adjusted within the socket.
**/
EFI_STATUS
AdjustResources (
  CPU_RESOURCE *SocketResources,
  UINT8         Socket,
  UINT8         ResourceType
  )
{
  UINT8       Stack;
  UINT64      NewLength;
  CONST UINT8 LastStack = LastStackOfSocket (Socket);
  UINT64      PreviousLimit;
  BOOLEAN     FirstStack = TRUE;

  switch (ResourceType) {
    case TypeIo:
      // Return if IoResourceNeeds is not zero which indicates a socket adjustment is needed
      if(SocketResources->IoResourceNeeds != 0){
          return EFI_OUT_OF_RESOURCES;
      }

      for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {

        if (!(mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1 << Stack))){
          continue;
        }
        //
        // For the first enabled stack, use the base I/O address for the socket, otherwise
        // calculate the new base based off the last enabled stack
        //
        if (FirstStack) {
          // stackPresentBitmap doesn't cover if a valid stack was disable due to resource adjustments.
          // Start with valid resources for current socket
          SocketResources->StackRes[Stack].IoBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].PciResourceIoBase;
          FirstStack = FALSE;
        } else {
          // Check to see if the previous stack is disabled by checking for equal base and limit
          if (SocketResources->StackRes[Stack-1].IoBase == SocketResources->StackRes[Stack-1].IoLimit) {
            if (PreviousLimit % 2 == 1) {
              PreviousLimit += 1;
            }
            SocketResources->StackRes[Stack].IoBase = (UINT16) PreviousLimit;
          } else {
            SocketResources->StackRes[Stack].IoBase = (UINT16) PreviousLimit + 1;
          }
        }

        NewLength = SocketResources->StackRes[Stack].NumIoPortsDesired;

        //
        //assign the left space to the last IIO stack. Reserved for across socket resource adjustment.
        //make adjustments if NewLength is zero
        //
        if (Stack == LastStack) {
          if (NewLength != 0) {
            if (SocketResources->IoResourcesLeft != 0) {
              NewLength += SocketResources->IoResourcesLeft + 1;
            }
          } else {
            NewLength = SocketResources->IoResourcesLeft;
          }
        }

        SocketResources->StackRes[Stack].NeedIoUpdate = TRUE;
        SocketResources->StackRes[Stack].IoLimit = (UINT16)(SocketResources->StackRes[Stack].IoBase + NewLength);
        PreviousLimit = SocketResources->StackRes[Stack].IoLimit;
        DEBUG((DEBUG_INFO, "SocketResources[%x].StackRes[%x].IoBase =%x   newLength = %x\n",Socket,Stack,SocketResources->StackRes[Stack].IoBase,NewLength));
        DEBUG((DEBUG_INFO, "SocketResources[%x].StackRes[%x].IoLimit =%x\n",Socket,Stack,SocketResources->StackRes[Stack].IoLimit));
      }
      break;

    case TypeMem32:
      //
      // Return if MmiolResourceNeeds is not zero which indicates a socket adjustment is needed
      //
        if (SocketResources->MmiolResourceNeeds != 0) {
          return EFI_OUT_OF_RESOURCES;
        }
        //
        // Visit all Stacks in this Socket and recalculate the New Mem32 Ranges per Stack
        //
        for (Stack = 0; Stack < MAX_IIO_STACK; Stack++){

          if (!(mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1 << Stack))) {
            continue;
          }
          //
          // For the first enabled stack, use the base low mmio address for the socket, otherwize
          // calculate the new base based off the last enabled stack
          //
          if (FirstStack) {
            // stackPresentBitmap doesn't cover if a valid stack was disable due to resource adjustments.
            // Start with valid resources for current socket
            SocketResources->StackRes[Stack].MmiolBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].Mmio32Base;
            FirstStack = FALSE;
          } else {
            // Check to see if the previous stack is disabled by checking for equal base and limit
            if (SocketResources->StackRes[Stack-1].MmiolBase == SocketResources->StackRes[Stack-1].MmiolLimit) {
              if (PreviousLimit % 2 == 1) {
                PreviousLimit += 1;
              }
              SocketResources->StackRes[Stack].MmiolBase = (UINT32) PreviousLimit;
            } else {
              SocketResources->StackRes[Stack].MmiolBase = (UINT32) PreviousLimit + 1;
            }
          }
          //
          // Verify if this Stack is the one that requires an update and calculate the new Limit
          // otherwise assign the new limit based on the Chunk and Extra Chunk calculation and assign the Newlength
          //
          NewLength =  SocketResources->StackRes[Stack].MmiolLength;
          //
          //assign the left space to the last IIO stack. Reserved for across socket resource adjustment.
          //make adjustments if NewLength is zero
          //
          if (Stack == LastStack) {
            if (NewLength != 0) {
              if (SocketResources->MmiolResourcesLeft){
                NewLength += SocketResources->MmiolResourcesLeft + 1;
              }
            } else {
              NewLength = SocketResources->MmiolResourcesLeft;
            }
          }

          SocketResources->StackRes[Stack].MmiolUpdate = 1;
          SocketResources->StackRes[Stack].MmiolLimit =  (UINT32)(SocketResources->StackRes[Stack].MmiolBase + NewLength);
          PreviousLimit = SocketResources->StackRes[Stack].MmiolLimit;
          DEBUG((DEBUG_INFO, "SocketResources[%x].StackRes[%x].MmiolBase =%x   newLength = %x\n",Socket,Stack,SocketResources->StackRes[Stack].MmiolBase,NewLength));
          DEBUG((DEBUG_INFO, "SocketResources[%x].StackRes[%x].MmiolLimit =%x\n",Socket,Stack,SocketResources->StackRes[Stack].MmiolLimit));
        }
      break;

    case TypeMem64:
      // Return if MmiohResourceNeeds is not zero which indicates a socket adjustment is needed
      if (SocketResources->MmiohResourceNeeds != 0){
        return EFI_OUT_OF_RESOURCES;
      }
      //
      // Visit all Stacks in this Socket and recalculate the New Mem64 Ranges per Stack
      //
      for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {

        if(!(mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1 << Stack))){
          continue;
        }
        //
        // For the first enabled stack, use the base high mmio address for the socket, otherwise
        // calculate the new base based off the last enabled stack
        //
        if (FirstStack) {
          // stackPresentBitmap doesn't cover if a valid stack was disable due to resource adjustments.
          // Start with valid resources for current socket
          SocketResources->StackRes[Stack].MmiohBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].Mmio64Base;
          FirstStack = FALSE;
        } else {
           // Check to see if the previous stack is disabled by checking for equal base and limit
          if (SocketResources->StackRes[Stack-1].MmiohBase == SocketResources->StackRes[Stack-1].MmiohLimit) {
            if (PreviousLimit % 2 == 1) {
              PreviousLimit += 1;
            }
            SocketResources->StackRes[Stack].MmiohBase = PreviousLimit;
          } else {
            SocketResources->StackRes[Stack].MmiohBase = PreviousLimit + 1;
          }
        }

        //
        // Verify if this Stack is the one that requires an update and calculate the new Limit
        // otherwise assign the new limit based on the Chunk and Extra Chunk calculation and assign the Newlength
        //

        NewLength =  SocketResources->StackRes[Stack].MmiohLength;

        //
        //assign the left space to the last IIO stack. Reserved for across socket resource adjustment.
        //make adjustments if NewLength is zero
        //
        if (Stack == LastStack) {
          if (NewLength != 0) {
            if (SocketResources->MmiohResourcesLeft) {
              NewLength += SocketResources->MmiohResourcesLeft + 1;
            }
          } else {
              NewLength = SocketResources->MmiohResourcesLeft;
          }
        }

        SocketResources->StackRes[Stack].MmiohUpdate = 1;
        SocketResources->StackRes[Stack].MmiohLimit =  (SocketResources->StackRes[Stack].MmiohBase + NewLength);
        PreviousLimit = SocketResources->StackRes[Stack].MmiohLimit;
        DEBUG((DEBUG_INFO, "SocketResources[%x].StackRes[%x].MmiohBase =%lx   newLength = %lx\n",Socket,Stack,SocketResources->StackRes[Stack].MmiohBase,NewLength));
        DEBUG((DEBUG_INFO, "SocketResources[%x].StackRes[%x].MmiohLimit =%lx\n",Socket,Stack,SocketResources->StackRes[Stack].MmiohLimit));

      } // for Stack
      break;
    default:
        DEBUG((DEBUG_ERROR, "[PCI] ERROR: Resource Type Unknown = %x\n", ResourceType));
      break;
  }// switch

  return EFI_SUCCESS;
}


/**
  Adjust resource ratio assignment among sockets to fit the resource needs from PCI devices.

  @param SocketResources   -  CPU_RESOURCE structure pointer that stores all resources need per socket
  @param ResourceType      -  type of resource that requires alignment
  @param ValidSockets       -  Number of Valid Sockets, need it to calculate how resources need to be splitted

  @retval EFI_SUCCESS            -  Succeed.
  @retval EFI_OUT_OF_RESOURCES   -  Not enough resources to be adjusted within the socket.
**/
EFI_STATUS
AdjustSocketResources (
  CPU_RESOURCE *SocketResources,
  UINT8         ResourceType,
  UINT8         ValidSockets
  )
{
  EFI_STATUS    Status;

  switch(ResourceType){
    case TypeIo:
      Status = AdjustSocketIo (SocketResources, ResourceType, ValidSockets);
      break;
    case TypeMem32:
      Status = AdjustSocketMmioL (SocketResources, ResourceType, ValidSockets);
      break;
    case TypeMem64:
      Status = AdjustSocketMmioH (SocketResources, ResourceType, ValidSockets);
      break;
    default:
      DEBUG((DEBUG_ERROR, "ERROR: Resource Type Unknown = %x\n",ResourceType));
      Status = EFI_INVALID_PARAMETER;
      break;
  } // switch

  return Status;
}


/**
  Calculate current system resource map with retrieved NVRAM variable to see if stored settings were applied

  @param[in] SocketPciResourceData - Pointer to stored CPU resource map

  @retval TRUE   - SYSTEM_PCI_BASE_LIMITS has been rejected and was not applied or not initialized
  @retval FALSE  - SYSTEM_PCI_BASE_LIMITS has been applied and still has relevant data
**/
BOOLEAN
IsResourceMapRejected (
  SYSTEM_PCI_BASE_LIMITS *SocketPciResourceData
  )
{
  UINT8                  Socket;
  UINT8                  Stack;
  BOOLEAN                Rejected = FALSE;
  PCI_BASE_LIMITS        *StackLimits;
  PCI_BASE_LIMITS        *UboxStackLimits;
  PCI_BASE_LIMITS        *SocketLimits;
  STACK_RES              *IioUdsUboxStackLimits;
  STACK_RES              *IioUdsStackLimits;
  IIO_RESOURCE_INSTANCE  *IioUdsSocketLimits;

  if (SocketPciResourceData == NULL) {
    return TRUE;
  }
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

    if (mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].Valid) {

      IioUdsSocketLimits = &mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket];
      SocketLimits = &SocketPciResourceData->Socket[Socket].SocketLimits;
      IioUdsUboxStackLimits = &mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[UBOX_STACK];
      UboxStackLimits = &SocketPciResourceData->Socket[Socket].StackLimits[UBOX_STACK];

      for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {

        if (mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1 << Stack)) {

          IioUdsStackLimits = &mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack];
          StackLimits = &SocketPciResourceData->Socket[Socket].StackLimits[Stack];
          //
          // Per stack
          //
          if (Socket == 0 && Stack == 0) {
            // First base starts at zero, mIioUds struct reserves 4K of Io for legacy purposes
            if (StackLimits->Io.Base != 0) {
              Rejected = TRUE;
            }
          } else {
            if (IioUdsStackLimits->PciResourceIoBase != StackLimits->Io.Base && StackLimits->Io.Base != 0) {
              Rejected = TRUE;
            }
          }
          if (IioUdsStackLimits->PciResourceIoLimit != StackLimits->Io.Limit && StackLimits->Io.Limit != 0) {
            Rejected = TRUE;
          }
          PCIDEBUG ("[%d.%d] Current I/O: 0x%04X..0x%04X\n", Socket, Stack,
                    IioUdsStackLimits->PciResourceIoBase, IioUdsStackLimits->PciResourceIoLimit);
          PCIDEBUG ("[%d.%d]   Saved I/O: 0x%04X..0x%04X %a\n", Socket, Stack,
                   StackLimits->Io.Base, StackLimits->Io.Limit, Rejected ? "rejected" : "");

          if (IioUdsStackLimits->Mmio32Base != StackLimits->LowMmio.Base && StackLimits->LowMmio.Base != 0) {
            Rejected = TRUE;
          }
          if (IioUdsStackLimits->Mmio32Limit != StackLimits->LowMmio.Limit && StackLimits->LowMmio.Limit != 0) {
            Rejected = TRUE;
          }
          PCIDEBUG ("[%d.%d] Current MMIOL: 0x%08X..0x%08X\n", Socket, Stack,
                    IioUdsStackLimits->Mmio32Base, IioUdsStackLimits->Mmio32Limit);
          PCIDEBUG ("[%d.%d]   Saved MMIOL: 0x%08X..0x%08X %a\n", Socket, Stack,
                    StackLimits->LowMmio.Base, StackLimits->LowMmio.Limit, Rejected ? "rejected" : "");

          if (IioUdsStackLimits->Mmio64Base != StackLimits->HighMmio.Base && StackLimits->HighMmio.Base != 0) {
            Rejected = TRUE;
          }
          if (IioUdsStackLimits->Mmio64Limit != StackLimits->HighMmio.Limit && StackLimits->HighMmio.Limit != 0) {
            Rejected = TRUE;
          }
          PCIDEBUG ("[%d.%d] Current MMIOH: 0x%012llX..0x%012llX\n", Socket, Stack,
                    IioUdsStackLimits->Mmio64Base, IioUdsStackLimits->Mmio64Limit);
          PCIDEBUG ("[%d.%d]   Saved MMIOH: 0x%012llX..0x%012llX %a\n", Socket, Stack,
                    StackLimits->HighMmio.Base, StackLimits->HighMmio.Limit, Rejected ? "rejected" : "");
        }
      }
      //
      // Per socket
      //
      if (IioUdsSocketLimits->PciResourceIoBase != SocketLimits->Io.Base && SocketLimits->Io.Base != 0) {
        Rejected = TRUE;
      }
      if (IioUdsSocketLimits->PciResourceIoLimit != SocketLimits->Io.Limit && SocketLimits->Io.Limit != 0) {
        Rejected = TRUE;
      }
      PCIDEBUG("[%d] Current I/O: 0x%04X..0x%04X\n", Socket,
               IioUdsSocketLimits->PciResourceIoBase, IioUdsSocketLimits->PciResourceIoLimit);
      PCIDEBUG("[%d]   Saved I/O: 0x%04X..0x%04X %a\n", Socket,
               SocketLimits->Io.Base, SocketLimits->Io.Limit, Rejected ? "rejected" : "");

      if (IioUdsSocketLimits->Mmio32Base != SocketLimits->LowMmio.Base && SocketLimits->LowMmio.Base != 0) {
        Rejected = TRUE;
      }
      if (IioUdsSocketLimits->Mmio32Limit != SocketLimits->LowMmio.Limit && SocketLimits->LowMmio.Limit != 0) {
        Rejected = TRUE;
      }
      PCIDEBUG ("[%d] Current MMIOL: 0x%08X..0x%08X\n", Socket,
               IioUdsSocketLimits->Mmio32Base, IioUdsSocketLimits->Mmio32Limit);
      PCIDEBUG ("[%d]   Saved MMIOL: 0x%08X..0x%08X %a\n", Socket,
                SocketLimits->LowMmio.Base, SocketLimits->LowMmio.Limit, Rejected ? "rejected" : "");

      if (IioUdsSocketLimits->Mmio64Base != SocketLimits->HighMmio.Base && SocketLimits->HighMmio.Base != 0) {
        Rejected = TRUE;
      }
      if (IioUdsSocketLimits->Mmio64Limit != SocketLimits->HighMmio.Limit && SocketLimits->HighMmio.Limit != 0) {
        Rejected = TRUE;
      }
      PCIDEBUG ("[%d] Current MMIOH: 0x%012llX..0x%012llX\n", Socket,
               IioUdsSocketLimits->Mmio64Base, IioUdsSocketLimits->Mmio64Limit);
      PCIDEBUG ("[%d]   Saved MMIOH: 0x%012llX..0x%012llX %a\n", Socket,
                SocketLimits->HighMmio.Base, SocketLimits->HighMmio.Limit, Rejected ? "rejected" : "");

      if (IioUdsUboxStackLimits->Mmio64Base != UboxStackLimits->HighMmio.Base && UboxStackLimits->HighMmio.Base != 0) {
        Rejected = TRUE;
      }
      if (IioUdsUboxStackLimits->Mmio64Limit != UboxStackLimits->HighMmio.Limit && UboxStackLimits->HighMmio.Limit != 0) {
        Rejected = TRUE;
      }
      PCIDEBUG ("[%d] Current UBOX: 0x%08X..0x%08X\n", Socket,
                IioUdsUboxStackLimits->Mmio64Base, IioUdsUboxStackLimits->Mmio64Limit);
      PCIDEBUG ("[%d]   Saved UBOX: 0x%08X..0x%08X %a\n", Socket,
                UboxStackLimits->HighMmio.Base, UboxStackLimits->HighMmio.Limit, Rejected ? "rejected" : "");
    }
  }
  DEBUG ((DEBUG_INFO, "[PCI] Resource rebalance rejected ? %a\n", Rejected ? "TRUE" : "FALSE"));
  return Rejected;
}


/**
 Read SYSTEM_PCI_RESOURCE_CONFIGURATION_DATA_NAME variable from flash and verify its content.

 If the variable does not exist, or is not valid for current system configuration
 the buffer at *PciResConfigPtr is just cleared.

 @param[out] PciResConfigPtr - Buffer for the resource configuration variable.

 @retval EFI_SUCCESS            The function completed successfully.
 @retval EFI_NOT_FOUND          The variable was not found.
 @retval EFI_DEVICE_ERROR       The variable could not be retrieved due to a hardware error.
 @retval EFI_SECURITY_VIOLATION The variable could not be retrieved due to an authentication failure.
**/
EFI_STATUS
PciHostReadResourceConfig (
  OUT SYSTEM_PCI_BASE_LIMITS *PciResConfigPtr
  )
{
  UINTN                       VarSize;
  EFI_STATUS                  Status;
  UINT8                       Socket;

  VarSize = sizeof(*PciResConfigPtr);
  Status = gRT->GetVariable (SYSTEM_PCI_RESOURCE_CONFIGURATION_DATA_NAME, &gEfiSocketPciResourceDataGuid,
                             NULL, &VarSize, PciResConfigPtr);
  if (EFI_ERROR (Status) && Status != EFI_BUFFER_TOO_SMALL) {
    goto ErrExit;
  }
  if (Status == EFI_BUFFER_TOO_SMALL || VarSize != sizeof(*PciResConfigPtr)) {

    PCIDEBUG ("Got variable '%s' of unexpected size %d (expect %d) - overwrite\n",
              SYSTEM_PCI_RESOURCE_CONFIGURATION_DATA_NAME, VarSize, sizeof(*PciResConfigPtr));
    Status = EFI_NOT_FOUND;
    goto ErrExit;
  }
  //
  // If any of the below checks fails clear the buffer and return EFI_NOT_FOUND.
  //
  Status = EFI_NOT_FOUND;
  if (PciResConfigPtr->MmioHBase != mIioUds->IioUdsPtr->PlatformData.PlatGlobalMmio64Base ||
      PciResConfigPtr->MmioHLimit != mIioUds->IioUdsPtr->PlatformData.PlatGlobalMmio64Limit) {

    PCIDEBUG ("%s: Memory map changed (MMIOH %012llX..%012llX != %012llX..%012llX) - overwrite\n",
              SYSTEM_PCI_RESOURCE_CONFIGURATION_DATA_NAME,
              PciResConfigPtr->MmioHBase, PciResConfigPtr->MmioHLimit,
              mIioUds->IioUdsPtr->PlatformData.PlatGlobalMmio64Base,
              mIioUds->IioUdsPtr->PlatformData.PlatGlobalMmio64Limit);
    goto ErrExit;
  }
  if (PciResConfigPtr->MmioLBase != mIioUds->IioUdsPtr->PlatformData.PlatGlobalMmio32Base ||
      PciResConfigPtr->MmioLLimit != mIioUds->IioUdsPtr->PlatformData.PlatGlobalMmio32Limit) {

    PCIDEBUG ("%s: Memory map changed (MMIOL %08X..%08X != %08X..%08X) - overwrite\n",
              SYSTEM_PCI_RESOURCE_CONFIGURATION_DATA_NAME,
              PciResConfigPtr->MmioLBase, PciResConfigPtr->MmioLLimit,
              mIioUds->IioUdsPtr->PlatformData.PlatGlobalMmio32Base,
              mIioUds->IioUdsPtr->PlatformData.PlatGlobalMmio32Limit);
    goto ErrExit;
  }
  if (PciResConfigPtr->IoBase != mIioUds->IioUdsPtr->PlatformData.PlatGlobalIoBase ||
      PciResConfigPtr->IoLimit != mIioUds->IioUdsPtr->PlatformData.PlatGlobalIoLimit) {

    PCIDEBUG ("%s: Memory map changed (I/O %04X..%04X != %04X..%04X) - overwrite\n",
              SYSTEM_PCI_RESOURCE_CONFIGURATION_DATA_NAME,
              PciResConfigPtr->IoBase, PciResConfigPtr->IoLimit,
              mIioUds->IioUdsPtr->PlatformData.PlatGlobalIoBase,
              mIioUds->IioUdsPtr->PlatformData.PlatGlobalIoLimit);
    goto ErrExit;
  }
  for (Socket = 0; Socket < NELEMENTS (PciResConfigPtr->Socket); Socket++) {

    if (PciResConfigPtr->StackPresentBitmap[Socket] !=
                                              mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap) {

      PCIDEBUG ("%s: Stack bitmap mismach (%04X != %04X) in socket %d - overwrite\n",
                SYSTEM_PCI_RESOURCE_CONFIGURATION_DATA_NAME, PciResConfigPtr->StackPresentBitmap[Socket],
                mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap, Socket);
      goto ErrExit;
    }
  }
  return EFI_SUCCESS;

 ErrExit:
  ZeroMem (PciResConfigPtr, sizeof(*PciResConfigPtr));
  return Status;
} // PciHostReadResourceConfig()


/**
  Adjust resource ratio assignment among CPU sockets to fit the resource needs from PCI devices.
  Update Setup variable if there are changes from the existing ratio requests for this boot.

  @param[in]  HostBridgeInstance    -  The Host Bridge Instance where the resource adjustment happens.
  @param[out] Result                -  Output parameter. Indicates whether changes have been made.
**/
VOID
AdjustResourceAmongRootBridges (
  IN  PCI_HOST_BRIDGE_INSTANCE          *HostBridgeInstance,
  OUT SOCKET_RESOURCE_ADJUSTMENT_RESULT *Result
  )
{
  PCI_ROOT_BRIDGE_INSTANCE               *RootBridgeInstance;
  LIST_ENTRY                             *List;
  CPU_RESOURCE                           SocketResources[MAX_SOCKET];
  UINT64                                 SocketIoLength;
  UINT64                                 SocketMem32Length;
  UINT64                                 SocketMem64Length;
  UINT64                                 SocketIoBase;
  UINT64                                 SocketMem32Base;
  UINT64                                 SocketMem64Base;
  UINT64                                 RsvLenAtBegin;
  UINT64                                 RsvLenAtEnd;
  UINT64                                 StackLength;
  UINT64                                 NewLength;
  UINT64                                 Alignment;
  UINT64                                 Remainder;
  UINT8                                  Socket;
  UINT8                                  ValidSockets;
  BOOLEAN                                ChangedType[TypeMax];
  BOOLEAN                                ChangedTypeOOR[TypeMax]; // Change type for out of resources
  UINT8                                  TypeIndex;
  UINT8                                  ChangedBitMap;
  EFI_STATUS                             Status;
  SYSTEM_PCI_BASE_LIMITS                 SocketPciResourceData;
  UINT8                                  Stack;
  UINT8                                  LastStack;
  UINT16                                 IoGranularity;
  UINT32                                 MmiolGranularity;
  UINT64                                 MmiohGranularity;
  BOOLEAN                                OutOfResources;
  UINT32                                 UboxMmioSize;
  BOOLEAN                                IsVirtualRootBridge;
  PCI_BASE_LIMITS                        *CurStackLimits;
  PCI_BASE_LIMITS                        *UboxStackLimits;
  PCI_BASE_LIMITS                        *CurSocketLimits;
  UINT32                                 PlatGlobalMmiolBase;
  UINT32                                 VtdBarSize;

  *Result = SocketResourceRatioNotChanged;
  SetMem (ChangedType, TypeMax, FALSE);
  SetMem (ChangedTypeOOR, TypeMax, FALSE);
  ChangedBitMap = 0;
  OutOfResources = FALSE;
  IsVirtualRootBridge = FALSE;

  IoGranularity = mIioUds->IioUdsPtr->PlatformData.IoGranularity;
  MmiolGranularity = mIioUds->IioUdsPtr->PlatformData.MmiolGranularity;
  MmiohGranularity = (UINT64) mIioUds->IioUdsPtr->PlatformData.MmiohGranularity.lo;
  MmiohGranularity |= ((UINT64)mIioUds->IioUdsPtr->PlatformData.MmiohGranularity.hi) << 32;
  ZeroMem (&SocketResources[0], sizeof(SocketResources));
  //
  // Read the system resource cfg from NVRAM. If the variable does not exist, or is
  // not valid for current system configuration the buffer SocketPciResourceData
  // is just cleared.
  //
  Status = PciHostReadResourceConfig (&SocketPciResourceData);
  if (EFI_ERROR (Status)) {

    if (Status != EFI_NOT_FOUND) {

      ASSERT_EFI_ERROR (Status);
      return;
    }
    //
    // Variable is not initialized yet, go with empty structure.
    //
  } else if (IsResourceMapRejected (&SocketPciResourceData)) {
    //
    // If variable is already initialized, but rejected by KTI do not reboot to avoid loop.
    //
    return;
  }

  UboxMmioSize = mIioUds->IioUdsPtr->PlatformData.UboxMmioSize;
  PlatGlobalMmiolBase = mIioUds->IioUdsPtr->PlatformData.PlatGlobalMmio32Base;
  ValidSockets = 0;
  for (List = HostBridgeInstance->RootBridges.ForwardLink, Socket = 0; Socket < MAX_SOCKET; Socket ++) {

    if (!mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].Valid) {
      continue;
    }
    ValidSockets++;
    //
    // Calculate the length of resources available per socket
    //
    if (mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].PciResourceIoBase >=
        mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].PciResourceIoLimit) {
      SocketIoBase = 0;
      SocketIoLength = 0;
    } else {
      SocketIoBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].PciResourceIoBase;
      SocketIoLength = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].PciResourceIoLimit -
                       mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].PciResourceIoBase;
    }
    if (mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].Mmio32Base >=
                                                   mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].Mmio32Limit) {
      SocketMem32Base = 0;
      SocketMem32Length = 0;
    } else {
      SocketMem32Base = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].Mmio32Base;
      SocketMem32Length = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].Mmio32Limit  -
                                                      mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].Mmio32Base;
      // Reserve 8M for ubox mmio
      SocketMem32Length = SocketMem32Length - UboxMmioSize;
    }

    if (mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].Mmio64Base >=
                                                   mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].Mmio64Limit) {
      SocketMem64Base = 0;
      SocketMem64Length = 0;
    } else{
      SocketMem64Base = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].Mmio64Base;
      SocketMem64Length = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].Mmio64Limit -
                                                      mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].Mmio64Base;
    }

    // Get all the resources that are in this socket
    SocketResources[Socket].IoResourcesLeft = (UINT16)SocketIoLength;
    SocketResources[Socket].MmiolResourcesLeft = (UINT32)SocketMem32Length;
    SocketResources[Socket].MmiohResourcesLeft = (UINT64)SocketMem64Length;

    LastStack = LastStackOfSocket (Socket);

    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
      if (!(mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1 << Stack))) {
        continue;
      }
      RootBridgeInstance = ROOT_BRIDGE_FROM_LINK (List);
      //
      // Check IO Resource
      //
      Alignment = RootBridgeInstance->ResAllocNode[TypeIo].Alignment + 1;
      NewLength = RootBridgeInstance->ResAllocNode[TypeIo].Length;

      if (IsVirtualRootBridge) {
        NewLength += NewLength;
      }
      // IoTrap allocates 256 byte range from GCD for common pool usage
      // For device to fit move to the next available alignment
      if ((Socket == 0) && (Stack == 0)) {
        NewLength += Alignment;
      }

      if (NewLength != 0) {
        //
        // At least 2KB align per KTI requirement. Add the length requested with given alignment.
        // If the sum is not 2KB aligned add on the remainder that would make it align.
        // Bump up to 4KB for root bridge requirements
        // Have to make sure Alignment is handled for direct address allocation
        //
        Remainder = SocketIoBase & (Alignment - 1);
        if (Remainder != 0) {
          NewLength += Alignment - Remainder;
        }
        if (NewLength % (IoGranularity * 2)) {
          Remainder = (IoGranularity * 2) - (NewLength % (IoGranularity * 2));
          NewLength += Remainder;
        }
        //
        // Store length as length - 1 for handling
        //
        NewLength -= 1;

        // Zero StackLength if its disable or negative
        if (mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceIoBase >=
            mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceIoLimit) {
          StackLength = 0;
        } else {
          StackLength = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceIoLimit -
                        mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceIoBase;
        }
        SocketResources[Socket].StackRes[Stack].NumIoPortsDesired = (UINT16)NewLength;
        // Check if new length can fit in the socket or stack
        if (SocketResources[Socket].IoResourcesLeft > (UINT16)NewLength) {
            SocketResources[Socket].IoResourcesLeft -=  (UINT16)NewLength + 1;
        } else if (SocketResources[Socket].IoResourcesLeft == (UINT16)NewLength) {
            SocketResources[Socket].IoResourcesLeft -=  (UINT16)NewLength;
        } else {
          // If there are resources left consume them
          if (SocketResources[Socket].IoResourcesLeft != 0) {
            NewLength = NewLength - SocketResources[Socket].IoResourcesLeft - 1;
            SocketResources[Socket].IoResourcesLeft =  0;
          }

          SocketResources[Socket].IoResourceNeeds += (UINT16)NewLength + 1;
          OutOfResources = TRUE;
          ChangedTypeOOR[TypeIo] = TRUE;
        }
        SocketResources[Socket].StackRes[Stack].IoAlignment = Alignment;
        if (NewLength > StackLength) {
          SocketResources[Socket].StackRes[Stack].NeedIoUpdate = TRUE;

          //IoResourcesLeft is UINT16 type, not 2's-complement value.
          if (SocketResources[Socket].IoResourcesLeft > SocketIoLength) {
            DEBUG ((DEBUG_ERROR, "[PCI] Out of Resources for Socket = %x  Stack = %x Type = %x\n",
                   Socket, Stack, TypeIo));
            SocketResources[Socket].IoResourcesLeft = 0;
          }
          ChangedType[TypeIo] = TRUE;
        }
        SocketIoBase += SocketResources[Socket].StackRes[Stack].NumIoPortsDesired + 1;
        DEBUG ((DEBUG_INFO, "SocketResources[%x].IoResourceLeft = %x\n",
               Socket, SocketResources[Socket].IoResourcesLeft));
        DEBUG ((DEBUG_INFO, "SocketResources[%x].StackRes[%x].IoAlignment = %x\n",
               Socket, Stack, SocketResources[Socket].StackRes[Stack].IoAlignment));
      } else {
        SocketResources[Socket].StackRes[Stack].NumIoPortsDesired = 0;
      }
      //
      // Check Mmem32 resource. This Host bridge does not support separated MEM / PMEM requests,
      // so only count MEM requests here.
      //
      Alignment = RootBridgeInstance->ResAllocNode[TypeMem32].Alignment + 1;
      NewLength = RootBridgeInstance->ResAllocNode[TypeMem32].Length;
      //
      // Account for reserved regions at begin and end of the stack MMIO32 region.
      //
      RsvLenAtBegin = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem32Base -
                                      mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio32Base;

      RsvLenAtEnd = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio32Limit -
                           mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem32Limit;
      NewLength += RsvLenAtBegin + RsvLenAtEnd;
      if (Alignment < RsvLenAtBegin) {
        Alignment = RsvLenAtBegin;
      }
      if (Alignment < RsvLenAtEnd) {
        Alignment = RsvLenAtEnd;
      }
      //
      // Always account for VT-d reserved resource ranges.
      // TODO: Remove when VTd BAR is included in RsvLenAtEnd.
      //
      if (mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].VtdBarAddress != 0) {

        VtdBarSize = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem32Limit -
                     mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].VtdBarAddress + 1;
        NewLength += VtdBarSize;
        if (Alignment < VtdBarSize) {
          Alignment = VtdBarSize;
        }
      }

      if (IsVirtualRootBridge) {
        NewLength += NewLength;
      }
      // PCH Allocates reserved MMIO for Sx SMI handler use
      // For device to fit move to the next available alignment
      if ((Socket == 0) && (Stack == 0)) {
        NewLength += Alignment;
      }

      if (NewLength != 0) {
        //
        // At least 4MB align per KTI requirement. Add the length requested with given alignment.
        // If the sum is not 4MB aligned add on the remainder that would make it align.
        // Have to make sure Alignment is handled for direct address allocation
        //
        Remainder = SocketMem32Base & (Alignment - 1);
        if (Remainder != 0) {
          NewLength += Alignment - Remainder;
        }
        if (NewLength % MmiolGranularity) {

          Remainder = MmiolGranularity - (NewLength % MmiolGranularity);
          NewLength += Remainder;
        }

        if (Stack == LastStack) {
          //
          // Ubox address must be 8MB aligned for the base address on most processors; skip check
          // if uboxMmioSize is 0 (avoid divide by zero exception).
          // At this point the requested resource has already been calculated to be satisfied.
          // Add granularity padding if necessary to satisfy Ubox requirement.
          //
          if (UboxMmioSize != 0 && (SocketMem32Base + NewLength) % UboxMmioSize) {
            Remainder = UboxMmioSize - (NewLength % UboxMmioSize);
            NewLength += Remainder;
          }
        }
        //
        // Store length as length - 1 for handling
        //
        NewLength -= 1;

        // Zero StackLength if its disable or negative
        if (mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio32Base >=
                                   mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio32Limit) {
          StackLength = 0;
        } else {
          StackLength = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio32Limit -
                                      mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio32Base;
        }
        SocketResources[Socket].StackRes[Stack].MmiolLength = (UINT32)NewLength;

        // Check if new length can fit in the socket or stack
        if (SocketResources[Socket].MmiolResourcesLeft > (UINT32)NewLength) {
            SocketResources[Socket].MmiolResourcesLeft -=  (UINT32)NewLength + 1;
        } else if (SocketResources[Socket].MmiolResourcesLeft == (UINT32)NewLength) {
            SocketResources[Socket].MmiolResourcesLeft -=  (UINT32)NewLength;
        } else {
            // If there are resources left consume them
            if (SocketResources[Socket].MmiolResourcesLeft) {
                NewLength = NewLength - SocketResources[Socket].MmiolResourcesLeft - 1;
                SocketResources[Socket].MmiolResourcesLeft =  0;
            }

            SocketResources[Socket].MmiolResourceNeeds += (UINT32)NewLength + 1;
            OutOfResources = TRUE;
            ChangedTypeOOR[TypeMem32] = TRUE;
        }
        SocketResources[Socket].StackRes[Stack].MmiolAlignment = Alignment;

        if (NewLength > StackLength) {
          SocketResources[Socket].StackRes[Stack].MmiolUpdate = 1;

          //MmiolResourcesLeft is UINT32 type, not 2's-complement value.
          if (SocketResources[Socket].MmiolResourcesLeft > SocketMem32Length) {
            DEBUG ((DEBUG_ERROR, "Out of Resources for Socket = %x  Stack = %x Type = %x\n",
                   Socket, Stack, TypeMem32));
            SocketResources[Socket].MmiolResourcesLeft = 0;
          }
          ChangedType[TypeMem32] = TRUE;
        }
        SocketMem32Base += SocketResources[Socket].StackRes[Stack].MmiolLength + 1;
        DEBUG ((DEBUG_INFO, "SocketResources[%x].MmiolResourceLeft = %x\n",
               Socket, SocketResources[Socket].MmiolResourcesLeft));
        DEBUG ((DEBUG_INFO, "SocketResources[%x].StackRes[%x].MmiolAlignment = %x\n",
               Socket, Stack, SocketResources[Socket].StackRes[Stack].MmiolAlignment));
      } else {
        SocketResources[Socket].StackRes[Stack].MmiolLength = 0;
      }
      //
      // Check Mem64 resource. This Host bridge does not support separated MEM / PMEM requests, so only count MEM requests here.
      //
      Alignment = RootBridgeInstance->ResAllocNode[TypeMem64].Alignment + 1;
      NewLength = RootBridgeInstance->ResAllocNode[TypeMem64].Length;
      //
      // Account for reserved regions at begin and end of the stack MMIO32 region.
      //
      RsvLenAtBegin = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem64Base -
                                      mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio64Base;

      RsvLenAtEnd = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio64Limit -
                           mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].PciResourceMem64Limit;
      NewLength += RsvLenAtBegin + RsvLenAtEnd;
      if (Alignment < RsvLenAtBegin) {
        Alignment = RsvLenAtBegin;
      }
      if (Alignment < RsvLenAtEnd) {
        Alignment = RsvLenAtEnd;
      }
      if (IsVirtualRootBridge) {
        NewLength += NewLength;
      }

      if (NewLength != 0) {
        //
        // At least 1GB align per KTI requirement. Add the length requested with given alignment.
        // If the sum is not 1GB aligned add on the remainder that would make it align.
        // Have to make sure Alignment is handled for direct address allocation
        //
        Remainder = SocketMem64Base & (Alignment - 1);
        if (Remainder != 0) {
          NewLength += Alignment - Remainder;
        }
        if (NewLength % MmiohGranularity) {
          Remainder = MmiohGranularity - (NewLength % MmiohGranularity);
          NewLength += Remainder;
        }
        //
        // Store length as length - 1 for handling
        //
        NewLength -= 1;

        // Zero StackLength if it's disable or negative
        if (mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio64Base >=
                                   mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio64Limit) {
          StackLength = 0;
        } else {
          StackLength = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio64Limit -
                                      mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio64Base;
        }
        SocketResources[Socket].StackRes[Stack].MmiohLength = NewLength;

        // Check if new length can fit in the socket or stack
        if (SocketResources[Socket].MmiohResourcesLeft > NewLength) {
          SocketResources[Socket].MmiohResourcesLeft -=  NewLength + 1;
        } else if (SocketResources[Socket].MmiohResourcesLeft == NewLength) {
          SocketResources[Socket].MmiohResourcesLeft -=  NewLength;
        } else {
          // If there are resources left consume them
          if (SocketResources[Socket].MmiohResourcesLeft != 0) {
            NewLength = NewLength - SocketResources[Socket].MmiohResourcesLeft - 1;
            SocketResources[Socket].MmiohResourcesLeft =  0;
          }

          SocketResources[Socket].MmiohResourceNeeds += NewLength + 1;
          OutOfResources = TRUE;
          ChangedTypeOOR[TypeMem64] = TRUE;
        }
        SocketResources[Socket].StackRes[Stack].MmiohAlignment = Alignment;

        if (NewLength > StackLength) {
          SocketResources[Socket].StackRes[Stack].MmiohUpdate = 1;

          //MmiohResourcesLeft is UINT64 type, not 2's-complement value.
          if (SocketResources[Socket].MmiohResourcesLeft > SocketMem64Length) {
            DEBUG ((DEBUG_ERROR, "Out of Resources for Socket = %x  Stack = %x Type = %x\n",
                   Socket, Stack, TypeMem64));
            SocketResources[Socket].MmiohResourcesLeft = 0;
          }
          ChangedType[TypeMem64] = TRUE;
        }
        SocketMem64Base += SocketResources[Socket].StackRes[Stack].MmiohLength + 1;
        DEBUG ((DEBUG_INFO, "SocketResources[%x].MmiohResourceLeft = %lx\n",
               Socket, SocketResources[Socket].MmiohResourcesLeft));
        DEBUG ((DEBUG_INFO, "SocketResources[%x].StackRes[%x].MmiohAlignment = %lx\n",
               Socket, Stack, SocketResources[Socket].StackRes[Stack].MmiohAlignment));
      } else {
        SocketResources[Socket].StackRes[Stack].MmiohLength = 0;
      }

      List = List->ForwardLink;

    } // for Stack

    // Check and update all resource types in socket that needs adjustment
    for (TypeIndex = 0; TypeIndex < TypeMax; TypeIndex++) {

      if (ChangedType[TypeIndex]) {

        DEBUG ((DEBUG_INFO, "[%d] Adjust stack %s resources...\n", Socket, mPciResourceTypeStr[TypeIndex]));
        Status = AdjustResources (&SocketResources[Socket], Socket, TypeIndex);
        ChangedType[TypeIndex] = FALSE;
        if (Status == EFI_SUCCESS) {
          ChangedBitMap |= (1 << TypeIndex);
        } else {
          ChangedBitMap &= ~(1 << TypeIndex);
        }
      }
    }
    //
    // Account for Ubox resources to accurately calculate new alignments for the next socket
    //
    SocketMem32Base += UboxMmioSize;
  } // for Socket ..

  ASSERT (List == &HostBridgeInstance->RootBridges);

  //
  // If a socket is out of resources, try to adjusting sockets for more room.
  //
  if (OutOfResources && (MAX_SOCKET > 1) && (ValidSockets > 1)) {

    for (TypeIndex = 0; TypeIndex < TypeMax; TypeIndex++) {

      if (ChangedTypeOOR[TypeIndex]) {

        DEBUG ((DEBUG_INFO, "Adjust socket %s resources...\n", mPciResourceTypeStr[TypeIndex]));
        Status = AdjustSocketResources (SocketResources, TypeIndex, ValidSockets);
        if (Status == EFI_SUCCESS) {
          ChangedBitMap |= (1 << TypeIndex);
        } else {
          ChangedBitMap &= ~(1 << TypeIndex);
        }
      }
    }
  } else if (OutOfResources && ChangedTypeOOR[TypeMem64]){
    //
    // Allow mmioh to be adjusted to access max available physical address range.
    //
    Status = AdjustSocketResources (SocketResources, TypeMem64, ValidSockets);
    if (Status == EFI_SUCCESS) {
      ChangedBitMap |= (1 << TypeIndex);
    } else {
      ChangedBitMap &= ~(1 << TypeIndex);
    }
  }

  // Update changed resource type.
  // OemGetResourceMapUpdate() will only update changed resource type so it is alright if data is zero.
  if (ChangedBitMap != 0) {

    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

      SocketPciResourceData.StackPresentBitmap[Socket] = mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap;
      for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
        if (!(mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1 << Stack))) {
          continue;
        }
        CurStackLimits = &SocketPciResourceData.Socket[Socket].StackLimits[Stack];

        //
        // Disable stacks that have no resources and are assigned none.
        // Reaching this far means the stack is valid and should be disabled if base equals limit and
        // length is zero.
        // Assigned address will be none zero at this point because CSTACK takes the first 4K in legacy
        // IO space.
        //
        if ((SocketResources[Socket].StackRes[Stack].NeedIoUpdate) &&
            (SocketResources[Socket].StackRes[Stack].IoLimit -
             SocketResources[Socket].StackRes[Stack].IoBase == 0)) {
          SocketResources[Socket].StackRes[Stack].IoBase = (UINT16)(-1);
          SocketResources[Socket].StackRes[Stack].IoLimit = 0;
        }

        if ((SocketResources[Socket].StackRes[Stack].MmiolUpdate) &&
            (SocketResources[Socket].StackRes[Stack].MmiolLimit -
             SocketResources[Socket].StackRes[Stack].MmiolBase == 0)) {
          SocketResources[Socket].StackRes[Stack].MmiolBase = (UINT32)(-1);
          SocketResources[Socket].StackRes[Stack].MmiolLimit = 0;
        }

        if ((SocketResources[Socket].StackRes[Stack].MmiohUpdate) &&
            (SocketResources[Socket].StackRes[Stack].MmiohLimit -
             SocketResources[Socket].StackRes[Stack].MmiohBase == 0)) {
          SocketResources[Socket].StackRes[Stack].MmiohBase = (UINT64)(-1);
          SocketResources[Socket].StackRes[Stack].MmiohLimit = 0;
        }

        // Zero base if 4K because mIioUds struct reserves 4K of Io for legacy purposes
        // Remove if mIioUds first base starts at zero
        if (SocketResources[Socket].StackRes[Stack].IoBase == 0x1000){
          SocketResources[Socket].StackRes[Stack].IoBase = 0;
        }

        if (SocketResources[Socket].StackRes[Stack].NeedIoUpdate) {
          CurStackLimits->Io.Base = SocketResources[Socket].StackRes[Stack].IoBase;
          CurStackLimits->Io.Limit = SocketResources[Socket].StackRes[Stack].IoLimit;
        }

        if (SocketResources[Socket].StackRes[Stack].MmiolUpdate) {
          if ((Socket == 0) && (Stack == 0)) {
            CurStackLimits->LowMmio.Base = PlatGlobalMmiolBase;
          } else {
            CurStackLimits->LowMmio.Base = SocketResources[Socket].StackRes[Stack].MmiolBase;
          }
          CurStackLimits->LowMmio.Limit = SocketResources[Socket].StackRes[Stack].MmiolLimit;
        }

        if (SocketResources[Socket].StackRes[Stack].MmiohUpdate) {
          CurStackLimits->HighMmio.Base = SocketResources[Socket].StackRes[Stack].MmiohBase;
          CurStackLimits->HighMmio.Limit = SocketResources[Socket].StackRes[Stack].MmiohLimit;
        }

        DEBUG((DEBUG_INFO, "\nSocketResources[%x].StackRes[%x].IoBase =%x\n",Socket,Stack,SocketResources[Socket].StackRes[Stack].IoBase));
        DEBUG((DEBUG_INFO, "SocketResources[%x].StackRes[%x].IoLimit =%x\n",Socket,Stack,SocketResources[Socket].StackRes[Stack].IoLimit));
        DEBUG((DEBUG_INFO, "SocketResources[%x].StackRes[%x].MmiolBase =%x\n",Socket,Stack,SocketResources[Socket].StackRes[Stack].MmiolBase));
        DEBUG((DEBUG_INFO, "SocketResources[%x].StackRes[%x].MmiolLimit =%x\n",Socket,Stack,SocketResources[Socket].StackRes[Stack].MmiolLimit));
        DEBUG((DEBUG_INFO, "SocketResources[%x].StackRes[%x].MmiohBase =%lx\n",Socket,Stack,SocketResources[Socket].StackRes[Stack].MmiohBase));
        DEBUG((DEBUG_INFO, "SocketResources[%x].StackRes[%x].MmiohLimit =%lx\n",Socket,Stack,SocketResources[Socket].StackRes[Stack].MmiohLimit));
      } // for Stack

      // Initialize to disabled
      SocketResources[Socket].IoBase = (UINT16)(-1);
      SocketResources[Socket].IoLimit = 0;
      SocketResources[Socket].MmiolBase = (UINT32)(-1);
      SocketResources[Socket].MmiolLimit = 0;
      SocketResources[Socket].MmiohBase = (UINT64)(-1);
      SocketResources[Socket].MmiohLimit = 0;

      // Search backwards to find the beginning valid stack
      for (Stack = MAX_IIO_STACK - 1; Stack < MAX_IIO_STACK ; Stack--) {
        CurSocketLimits = &SocketPciResourceData.Socket[Socket].SocketLimits;

        if (!(mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1 << Stack))) {
          continue;
        }

        if (SocketResources[Socket].StackRes[Stack].IoBase != (UINT16)(-1)) {
          SocketResources[Socket].IoBase = SocketResources[Socket].StackRes[Stack].IoBase;
        }

        if (SocketResources[Socket].StackRes[Stack].MmiolBase != (UINT32)(-1)) {
          SocketResources[Socket].MmiolBase = SocketResources[Socket].StackRes[Stack].MmiolBase;
        }

        if (SocketResources[Socket].StackRes[Stack].MmiohBase != (UINT64)(-1)) {
          SocketResources[Socket].MmiohBase = SocketResources[Socket].StackRes[Stack].MmiohBase;
        }
      } // for Stack

      // Search to find the last valid limit
      for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
        if (!(mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1 << Stack))) {
          continue;
        }

        if (SocketResources[Socket].StackRes[Stack].IoLimit != 0) {
          SocketResources[Socket].IoLimit = SocketResources[Socket].StackRes[Stack].IoLimit;
        }

        if (SocketResources[Socket].StackRes[Stack].MmiolLimit) {
          SocketResources[Socket].MmiolLimit = SocketResources[Socket].StackRes[Stack].MmiolLimit;
        }

        if (SocketResources[Socket].StackRes[Stack].MmiohLimit) {
          SocketResources[Socket].MmiohLimit = SocketResources[Socket].StackRes[Stack].MmiohLimit;
        }
      } // for Stack

      // Update socket level resource range
      if (SocketResources[Socket].StackRes[0].NeedIoUpdate) {
        CurSocketLimits->Io.Base = SocketResources[Socket].IoBase;
        CurSocketLimits->Io.Limit = SocketResources[Socket].IoLimit;
      }

      if (SocketResources[Socket].StackRes[0].MmiolUpdate) {
        //
        // Apply stolen 8M for ubox mmio per socket
        //
        if (UboxMmioSize != 0) {
          UboxStackLimits = &SocketPciResourceData.Socket[Socket].StackLimits[UBOX_STACK];

          UboxStackLimits->LowMmio.Base = SocketResources[Socket].MmiolLimit + 1;
          SocketResources[Socket].MmiolLimit = (UINT32)UboxStackLimits->LowMmio.Base + UboxMmioSize - 1;
          UboxStackLimits->LowMmio.Limit = SocketResources[Socket].MmiolLimit;
        }
        CurSocketLimits->LowMmio.Base = SocketResources[Socket].MmiolBase;
        CurSocketLimits->LowMmio.Limit = SocketResources[Socket].MmiolLimit;
      }

      if (SocketResources[Socket].StackRes[0].MmiohUpdate) {
        CurSocketLimits->HighMmio.Base  = SocketResources[Socket].MmiohBase;
        CurSocketLimits->HighMmio.Limit = SocketResources[Socket].MmiohLimit;
      }

      DEBUG((DEBUG_INFO, "\nSocketResources[%x].UboxBase =%x\n",Socket,UboxStackLimits->LowMmio.Base));
      DEBUG((DEBUG_INFO, "SocketResources[%x].UboxLimit =%x\n",Socket,UboxStackLimits->LowMmio.Limit));
      DEBUG((DEBUG_INFO, "\nSocketResources[%x].IoBase =%x\n",Socket,SocketResources[Socket].IoBase));
      DEBUG((DEBUG_INFO, "SocketResources[%x].IoLimit =%x\n",Socket,SocketResources[Socket].IoLimit));
      DEBUG((DEBUG_INFO, "SocketResources[%x].MmiolBase =%x\n",Socket,SocketResources[Socket].MmiolBase));
      DEBUG((DEBUG_INFO, "SocketResources[%x].MmiolLimit =%x\n",Socket,SocketResources[Socket].MmiolLimit));
      DEBUG((DEBUG_INFO, "SocketResources[%x].MmiohBase =%lx\n",Socket,SocketResources[Socket].MmiohBase));
      DEBUG((DEBUG_INFO, "SocketResources[%x].MmiohLimit =%lx\n",Socket,SocketResources[Socket].MmiohLimit));
    } // for Socket
    SocketPciResourceData.MmioHBase = mIioUds->IioUdsPtr->PlatformData.PlatGlobalMmio64Base;
    SocketPciResourceData.MmioHLimit = mIioUds->IioUdsPtr->PlatformData.PlatGlobalMmio64Limit;
    SocketPciResourceData.MmioLBase = mIioUds->IioUdsPtr->PlatformData.PlatGlobalMmio32Base;
    SocketPciResourceData.MmioLLimit = mIioUds->IioUdsPtr->PlatformData.PlatGlobalMmio32Limit;
    SocketPciResourceData.IoBase = mIioUds->IioUdsPtr->PlatformData.PlatGlobalIoBase;
    SocketPciResourceData.IoLimit = mIioUds->IioUdsPtr->PlatformData.PlatGlobalIoLimit;

    PCIDEBUG("Writing resource rebalance request '%s':\n", SYSTEM_PCI_RESOURCE_CONFIGURATION_DATA_NAME);
    PCIDEBUG("System I/O  : %04X..%04X\n", SocketPciResourceData.IoBase, SocketPciResourceData.IoLimit);
    PCIDEBUG("System MMIOL: %08X..%08X\n", SocketPciResourceData.MmioLBase, SocketPciResourceData.MmioLLimit);
    PCIDEBUG("System MMIOH: %012llX..%012llX\n", SocketPciResourceData.MmioHBase, SocketPciResourceData.MmioHLimit);
    for (Socket = 0; Socket < NELEMENTS (SocketPciResourceData.Socket); Socket++) {

      PCIDEBUG("[%d] StackPresent: 0x%04X\n", Socket, SocketPciResourceData.StackPresentBitmap[Socket]);
      PCIDEBUG("[%d] I/O  : %04X..%04X\n", Socket,
               SocketPciResourceData.Socket[Socket].SocketLimits.Io.Base,
               SocketPciResourceData.Socket[Socket].SocketLimits.Io.Limit);
      PCIDEBUG("[%d] MMIOL: %08X..%08X\n", Socket,
               SocketPciResourceData.Socket[Socket].SocketLimits.LowMmio.Base,
               SocketPciResourceData.Socket[Socket].SocketLimits.LowMmio.Limit);
      PCIDEBUG("[%d] MMIOH: %012llX..%012llX\n", Socket,
               SocketPciResourceData.Socket[Socket].SocketLimits.HighMmio.Base,
               SocketPciResourceData.Socket[Socket].SocketLimits.HighMmio.Limit);
      for (Stack = 0; Stack <  NELEMENTS (SocketPciResourceData.Socket[Socket].StackLimits); Stack++) {

        PCIDEBUG("[%d.%d] I/O  : %04X..%04X\n", Socket, Stack,
                 SocketPciResourceData.Socket[Socket].StackLimits[Stack].Io.Base,
                 SocketPciResourceData.Socket[Socket].StackLimits[Stack].Io.Limit);
        PCIDEBUG("[%d.%d] MMIOL: %08X..%08X\n", Socket, Stack,
                 SocketPciResourceData.Socket[Socket].StackLimits[Stack].LowMmio.Base,
                 SocketPciResourceData.Socket[Socket].StackLimits[Stack].LowMmio.Limit);
        PCIDEBUG("[%d.%d] MMIOH: %012llX..%012llX\n", Socket, Stack,
                 SocketPciResourceData.Socket[Socket].StackLimits[Stack].HighMmio.Base,
                 SocketPciResourceData.Socket[Socket].StackLimits[Stack].HighMmio.Limit);
      }
    }

    *Result = SocketResourceRatioChanged;
    Status = gRT->SetVariable(
              SYSTEM_PCI_RESOURCE_CONFIGURATION_DATA_NAME,
              &gEfiSocketPciResourceDataGuid,
              EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
              sizeof(SocketPciResourceData),
              &SocketPciResourceData
            );
    ASSERT_EFI_ERROR(Status);
  }

  return;
}

