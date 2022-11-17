/** @file

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Guid/SocketPciResourceData.h>
#include <Guid/SocketIioVariable.h>
#include <Protocol/IioUds.h>

#include "PciHostBridge.h"
#include "PciRootBridge.h"
#include "PciRebalance.h"


extern EFI_IIO_UDS_PROTOCOL *mIioUds;

/**
  Return TRUE if the specified socket/stack combination exists,
  otherwise return FALSE

  @param Socket - the socket to be checked
  @param Stack - the stack of the socket to be checked

  @retval TRUE - the socket/stack combination exists
  @retval FALSE - the socket/stack combination does not exist
*/
STATIC BOOLEAN
IsStackPresent (
  UINT8 Socket,
  UINT8 Stack
  )
{
  BOOLEAN Result;
  UINT64 Mask;

  ASSERT (Socket < ARRAY_SIZE (mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo));  // simple overrun check
  if (Socket >= ARRAY_SIZE (mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo)) {
    Result = FALSE;
    goto err_exit;
  }

  //
  // if the StackPresentBitmap is a single byte, then we can track 8 stacks,
  // the sizeof will tell us how many bytes we have, we scale by 8 to
  // determine the maximum number of stacks we can track.  Stacks larger
  // than this are not present essentially by definition, but could also
  // be a sign that we need a wider type to store the information; hence we
  // assert
  //
  ASSERT (Stack < 8 * sizeof(mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[0].stackPresentBitmap));
  if (Stack >= 8 * sizeof(mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[0].stackPresentBitmap)) {
    Result = FALSE;
    goto err_exit;
  }

  Mask = 1;
  Mask <<= Stack;
  Result = (Mask & mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap) != 0;

err_exit:
  return Result;
}


/**
  Adjust resource assignment among sockets to fit the IO
  resources from the PCI() devices in the system

  @param SocketResources   -  CPU_RESOURCE structure pointer that stores all resources need per socket
  @param ResourceType      -  type of resource that requires alignment
  @param ValidSockets       -  Number of Valid Sockets, need it
                            to calculate how resources need to
                            be split

  @retval EFI_SUCCESS            -  Succeed.
  @retval EFI_OUT_OF_RESOURCES   -  Not enough resources to be adjusted within the socket.
**/
EFI_STATUS
AdjustSocketIo (
  CPU_RESOURCE *SocketResources,
  UINT8         ResourceType,
  UINT8         ValidSockets
)
{
  UINT64 Base;                                    ///< Next base I/O port number to use
  UINT64 Limit;                                   ///< Most recent limit for I/O port numbers
  CONST UINT64 MaxLimit = ((UINT64)1 << 16) - 1;  ///< Maximum value for limit; used to ensure we don't overflow
  CPU_RESOURCE *CurSocketResources;               ///< Pointer to the CPU_RESOURE structure for the CPU we
                                                  ///< are examining
  STACK_RESOURCE *CurStackResources;              ///< Pointer to the STACK_RESOURCE structure for the CPU/Stack
                                                  ///< we are examining
  UINT16 NumFreePorts;                            ///< Number of i/o ports allocated to sockets that are not used
  UINT8 LastStack;                                ///< Last enabled stack of the last enabled socket
  CONST UINT8 LastSocketIndex = ValidSockets - 1; ///< Index of the last socket
  UINT64 TotalResourceSize;
  UINT64 ResourceSize;
  UINT8 Socket;                                   ///< Loop variable used to iterate over the sockets
  UINT8 Stack;                                    ///< Loop variable used to iterate over the stacks of a given socket

  NumFreePorts = 0;
  for (Socket = 0; Socket < ValidSockets; Socket++)  {
    CurSocketResources = &SocketResources[Socket];
    if (CurSocketResources->IoResourceNeeds == 0 && CurSocketResources->IoResourcesLeft != 0) {
      ASSERT (NumFreePorts < NumFreePorts + CurSocketResources->IoResourcesLeft + 1); // check for overflow
      NumFreePorts += CurSocketResources->IoResourcesLeft + 1;
      CurSocketResources->IoResourcesLeft = 0;
    }
  }

  for (Socket = 0; Socket < ValidSockets; Socket++) {
    CurSocketResources = &SocketResources[Socket];
    if (CurSocketResources->IoResourceNeeds != 0 && NumFreePorts >= CurSocketResources->IoResourceNeeds) {
      ASSERT (NumFreePorts > NumFreePorts - CurSocketResources->IoResourceNeeds); // check for underflow
      NumFreePorts -= CurSocketResources->IoResourceNeeds;
      CurSocketResources->IoResourceNeeds = 0;
    }
  }

  LastStack = LastStackOfSocket (LastSocketIndex);

  if (NumFreePorts > 0) {
    CurStackResources = &SocketResources[LastSocketIndex].StackRes[LastStack];
    if (CurStackResources->NumIoPortsDesired != 0) {
      CurStackResources->NumIoPortsDesired += NumFreePorts;
    } else {
      CurStackResources->NumIoPortsDesired += NumFreePorts - 1;
    }
  }

  //
  // Verify all resource requested can fit into the systems address range.
  //
  TotalResourceSize = 0;
  for (Socket = 0; Socket < ValidSockets; Socket ++) {
    LastStackWithResources (&SocketResources[Socket], Socket, ResourceType, &LastStack, &ResourceSize);
    TotalResourceSize += ResourceSize;
  }
  DEBUG ((DEBUG_INFO, "Total Request IO Range = %xh\n", TotalResourceSize));
  DEBUG ((DEBUG_INFO, "Total System IO Range  = %xh\n", MaxLimit));
  if (TotalResourceSize > MaxLimit) {
    //
    // Not enough system resources to support the request.
    // Remove all request to update NVRAM variable for this resource type.
    //
    for (Socket = 0; Socket < ValidSockets; Socket ++) {
      for (Stack = 0; Stack < MAX_IIO_STACK; Stack ++) {
        if (!(mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1 << Stack))) {
          continue;
        }
        SocketResources[Socket].StackRes[Stack].NeedIoUpdate = 0;
      }
    }
    DEBUG ((DEBUG_ERROR, "ERROR: Out of adjustable IO resources. Can't adjust across sockets\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG((DEBUG_ERROR, "Assigning new socket i/o range...\n"));

  Base = mIioUds->IioUdsPtr->PlatformData.IIO_resource[0].PciResourceIoBase;
  Limit = Base;   // assume no resources are allocated
  for (Socket = 0, CurSocketResources = SocketResources; Socket < ValidSockets; Socket++, CurSocketResources++) {
    DEBUG ((DEBUG_INFO, "socket = %d, Base = %x, Limit =%x, MaxLimit = %x\n", Socket, Base, Limit, MaxLimit));
    ASSERT (Base < MaxLimit);
    CurSocketResources->IoBase = (UINT16)Base;
    DEBUG ((DEBUG_INFO, "set socket io base to %x\n", Base));

    for (Stack = 0, CurStackResources = CurSocketResources->StackRes;
         Stack < MAX_IIO_STACK;
         Stack++, CurStackResources++) {
      if (!IsStackPresent (Socket, Stack)) {
        DEBUG ((DEBUG_INFO, "  Stack %d not present, setting base/limit to 0xffff/0\n", Stack));
        CurStackResources->IoBase = 0xffff;
        CurStackResources->IoLimit = 0;
        continue;
      }

      if (CurStackResources->NumIoPortsDesired == 0) {
        DEBUG ((DEBUG_INFO, "  Stack %d doesn't need i/o resources, setting base/limit to 0xffff/0\n", Stack));
        CurStackResources->IoBase = 0xffff;
        CurStackResources->IoLimit = 0;
        CurStackResources->NeedIoUpdate = TRUE;
        continue;
      }

      DEBUG((DEBUG_INFO, "  Stack %d setting i/o base to %x, ports desired was %x\n",
        Stack, Base, CurStackResources->NumIoPortsDesired));
      ASSERT (Base < MaxLimit);
      CurStackResources->IoBase = (UINT16)Base;
      Limit = Base + CurStackResources->NumIoPortsDesired;
      DEBUG ((DEBUG_INFO, "    limit set to %x (var and stack)\n", Limit));
      ASSERT (Base <= Limit);
      ASSERT (Limit <= MaxLimit);
      CurStackResources->IoLimit = (UINT16)Limit;
      CurStackResources->NeedIoUpdate = TRUE;
      Base = Limit + 1;
      DEBUG ((DEBUG_INFO, "    Base variable updated to %x\n", Base));
    }
    ASSERT (Limit <= MaxLimit);
    DEBUG ((DEBUG_INFO, "  Socket %d limit set to %x\n", Socket, Limit));
    CurSocketResources->IoLimit = (UINT16)Limit;
  }

  DEBUG ((DEBUG_INFO, "Dumping new I/O requests\n"));
  for (Socket = 0, CurSocketResources = SocketResources; Socket < ValidSockets; Socket++, CurSocketResources++) {
    DEBUG((DEBUG_INFO, "socket %d %x/%x\n", Socket, CurSocketResources->IoBase, CurSocketResources->IoLimit));
    for (Stack = 0, CurStackResources = CurSocketResources->StackRes;
         Stack < MAX_IIO_STACK;
         Stack++, CurStackResources++) {
      DEBUG ((DEBUG_INFO, "%d/%d: %x/%x\n", Socket, Stack, CurStackResources->IoBase, CurStackResources->IoLimit));
    }
  }

  return EFI_SUCCESS;
}

