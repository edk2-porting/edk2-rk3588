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
  Adjust resource assignments among sockets to fit the low
  MMIO resources (32-bit addresses) from the PCI(e) devices in the system

  @param[in,out] SocketResources - CPU_RESOURCE structure pointer that stores all resources need per socket
  @param[in]     ResourceType    - Type of resource that requires alignment
  @param[in]     ValidSockets    - Number of Valid Sockets, need it to calculate how resources need to be splitted

  @retval EFI_SUCCESS            -  Succeed.
  @retval EFI_OUT_OF_RESOURCES   -  Not enough resources to be adjusted within the socket.
 */
EFI_STATUS
AdjustSocketMmioL (
  IN OUT CPU_RESOURCE *SocketResources,
  IN     UINT8         ResourceType,
  IN     UINT8         ValidSockets
  )
{
  CONST UINT8 LastSocket = ValidSockets - 1;
  UINT8       Socket;
  UINT8       Stack;
  UINT8       LastStack;
  UINT64      Take;
  UINT32      UboxMmioSize;
  UINT64      ResourceSize;
  UINT64      TotalResourceSize;
  UINT32      TempMmioBase;
  UINT32      TempMmioLimit;

  Take = 0;
  UboxMmioSize = mIioUds->IioUdsPtr->PlatformData.UboxMmioSize;
  //
  // Get first and last MMIOL address
  //
  TempMmioBase  = mIioUds->IioUdsPtr->PlatformData.PlatGlobalMmio32Base;
  TempMmioLimit = mIioUds->IioUdsPtr->PlatformData.PlatGlobalMmio32Limit;
  //
  // Find all the extra space left
  //
  for (Socket = 0; Socket < ValidSockets; Socket++) {
    if ((SocketResources[Socket].MmiolResourceNeeds == 0) && (SocketResources[Socket].MmiolResourcesLeft != 0)) {

      Take += SocketResources[Socket].MmiolResourcesLeft + 1;
      SocketResources[Socket].MmiolResourcesLeft = 0;
    }
  }
  //
  // Give space to sockets that needs more space favoring first come first served
  //
  for (Socket = 0; Socket < ValidSockets; Socket++) {
    if ((SocketResources[Socket].MmiolResourceNeeds != 0) && (Take >= SocketResources[Socket].MmiolResourceNeeds)) {

      Take -= SocketResources[Socket].MmiolResourceNeeds;
      DEBUG((DEBUG_ERROR, "SocketResources[%x].MmiolResourceNeeds = %x\n", Socket, SocketResources[Socket].MmiolResourceNeeds));
      SocketResources[Socket].MmiolResourceNeeds = 0;
    }
  }
  //
  // Give away leftover resources
  //
  LastStack = LastStackOfSocket (LastSocket);
  if (Take != 0) {
    if (SocketResources[LastSocket].StackRes[LastStack].MmiolLength != 0) {
      SocketResources[LastSocket].StackRes[LastStack].MmiolLength += (UINT32)Take;
    } else{
      SocketResources[LastSocket].StackRes[LastStack].MmiolLength += ((UINT32)Take - 1);
    }
  }
  //
  // Verify all resource requested can fit into the systems address range.
  //
  TotalResourceSize = 0;
  for (Socket = 0; Socket < ValidSockets; Socket++) {
    LastStackWithResources (&SocketResources[Socket], Socket, ResourceType, &LastStack, &ResourceSize);
    TotalResourceSize += ResourceSize + UboxMmioSize;
  }
  DEBUG ((DEBUG_INFO, "Total Request MMIOL Range = %08Xh\n", TotalResourceSize));
  DEBUG ((DEBUG_INFO, "Total System MMIOL Range  = %08Xh\n", (TempMmioLimit - TempMmioBase + 1)));
  if (TotalResourceSize > (TempMmioLimit - TempMmioBase + 1)) {
    //
    // Not enough system resources to support the request.
    // Remove all request to update NVRAM variable for this resource type.
    //
    for (Socket = 0; Socket < ValidSockets; Socket ++) {
      for (Stack = 0; Stack < MAX_IIO_STACK; Stack ++) {
        if (!(mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1 << Stack))) {
          continue;
        }
        SocketResources[Socket].StackRes[Stack].MmiolUpdate = 0;
      }
    }
    DEBUG ((DEBUG_ERROR, "[PCI] ERROR: Out of adjustable MMIOL resources. Can't adjust across sockets\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_ERROR, "Assigning new socket MMIOL range...\n"));
  for (Socket = 0, TempMmioLimit = TempMmioBase - 1; Socket < ValidSockets; Socket ++) {

    SocketResources[Socket].MmiolBase = TempMmioLimit + 1;
    //
    // Update the stacks base and limit values.
    //
    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {

      if (!(mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1 << Stack))) {

        SocketResources[Socket].StackRes[Stack].MmiolBase = 0;
        SocketResources[Socket].StackRes[Stack].MmiolLimit = 0;

      } else {

        SocketResources[Socket].StackRes[Stack].MmiolBase = TempMmioLimit + 1;
        if (SocketResources[Socket].StackRes[Stack].MmiolLength != 0) {
          //
          // MmiolLength is actually length-1, so we should move TempMmioLimit by MmiolLength+1,
          // but only when it is >0, i.e. only for stack that has resources.
          //
          TempMmioLimit += SocketResources[Socket].StackRes[Stack].MmiolLength + 1;
          SocketResources[Socket].StackRes[Stack].MmiolLimit = TempMmioLimit;

        } else {

          SocketResources[Socket].StackRes[Stack].MmiolLimit = SocketResources[Socket].StackRes[Stack].MmiolBase;
        }
        SocketResources[Socket].StackRes[Stack].MmiolUpdate = 1;
      }
      DEBUG ((DEBUG_ERROR, "SocketResources[%x].StackRes[%x].MmiolBase  = %X  newLength = %x\n", Socket, Stack,
              SocketResources[Socket].StackRes[Stack].MmiolBase, SocketResources[Socket].StackRes[Stack].MmiolLength));
      DEBUG ((DEBUG_ERROR, "SocketResources[%x].StackRes[%x].MmiolLimit = %X\n", Socket, Stack,
              SocketResources[Socket].StackRes[Stack].MmiolLimit));
      DEBUG ((DEBUG_ERROR, "SocketResources[%x].StackRes[%x].MmiolUpdate= %d\n", Socket, Stack,
              SocketResources[Socket].StackRes[Stack].MmiolUpdate));
    } // for (Stack...)
    //
    // In the socket resources there can be UBOX, unfortunatelly not exposed in stackPresentBitmap
    // so it has to be handled with such uguly hacks.
    //
    TempMmioLimit += UboxMmioSize;
    SocketResources[Socket].MmiolLimit = TempMmioLimit;
  } // for (Socket...)

  return EFI_SUCCESS;
}

