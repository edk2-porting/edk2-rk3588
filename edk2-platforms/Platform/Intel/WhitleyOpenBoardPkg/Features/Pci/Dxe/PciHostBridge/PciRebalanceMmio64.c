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
  Adjust resource assignments among sockets to fit the high
  MMIO resources (64-bit addresses, typically above 4GB) from
  the PCI(e) devices in the system

  @param[in,out] SocketResources - CPU_RESOURCE structure pointer that stores all resources need per socket
  @param[in]     ResourceType    - Type of resource that requires alignment
  @param[in]     ValidSockets    - Number of Valid Sockets, need it to calculate how resources need to be splitted

  @retval EFI_SUCCESS            -  Succeed.
  @retval EFI_OUT_OF_RESOURCES   -  Not enough resources to be adjusted within the socket.
 */
EFI_STATUS
AdjustSocketMmioH (
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
  UINT64      UnAllocatedMmioh;
  UINT64      MaxMmioh;
  UINT64      ResourceSize;
  UINT64      TotalResourceSize;
  UINT64      TempMmioBase;
  UINT64      TempMmioLimit;

  Take = 0;
  UboxMmioSize = mIioUds->IioUdsPtr->PlatformData.UboxMmioSize;
  UnAllocatedMmioh = 0;
  //
  // Get first and last IO base address
  //
  TempMmioBase  = mIioUds->IioUdsPtr->PlatformData.PlatGlobalMmio64Base;
  TempMmioLimit = mIioUds->IioUdsPtr->PlatformData.PlatGlobalMmio64Limit;

  // Find all the extra space left
  for (Socket = 0; Socket < ValidSockets; Socket ++) {
    if ((SocketResources[Socket].MmiohResourceNeeds == 0) && (SocketResources[Socket].MmiohResourcesLeft != 0)) {

      Take += SocketResources[Socket].MmiohResourcesLeft + 1;
      SocketResources[Socket].MmiohResourcesLeft = 0;
    }
  }

  MaxMmioh = (UINT64) mIioUds->IioUdsPtr->PlatformData.MmiohGranularity.lo;
  MaxMmioh |= ((UINT64) mIioUds->IioUdsPtr->PlatformData.MmiohGranularity.hi) << 32;
  //
  // Maximum chunk accessible in the system based on the given granularity
  //
  if (UboxMmioSize == 0) {
    MaxMmioh = MaxMmioh * 32; //for 14nm
  } else {
    MaxMmioh = ((UINT64) 1 << mIioUds->IioUdsPtr->PlatformData.MaxAddressBits);
  }

  //
  // Find out how much MMIOH has not been allocated
  //
  if (MaxMmioh > (TempMmioLimit - TempMmioBase)) {
    UnAllocatedMmioh = MaxMmioh - (TempMmioLimit - TempMmioBase) - 1;
  } else {
    //
    // Extra MMIOH is not enough to close the gap for a successful adjustment.
    // Use all extra MMIOH in case if only a small amount is needed for adjustment or
    // the granularity was reduced due to MMIOH base.
    // (14nm only) or remove if map is rejected for this case to start over with correct values.
    // (10nm only) does not have this problem and doesn't need the code below because the limit is removed and we can use max address lines.
    //
    Take += MaxMmioh;
  }

  // Give space to sockets that needs more space favoring first come first served
  for (Socket = 0; Socket < ValidSockets; Socket ++) {
    if ((SocketResources[Socket].MmiohResourceNeeds != 0) && (Take >= SocketResources[Socket].MmiohResourceNeeds)) {
      //
      // The socket requesting additional resources can be granted by using the already
      // allocated range given to the whole system originally.
      //
      Take -= SocketResources[Socket].MmiohResourceNeeds;
      DEBUG ((DEBUG_ERROR, "SocketResources[%x].MmiohResourceNeeds = %llX\n",Socket, SocketResources[Socket].MmiohResourceNeeds));
      SocketResources[Socket].MmiohResourceNeeds = 0;
    } else if ((SocketResources[Socket].MmiohResourceNeeds != 0) &&
               (Take < SocketResources[Socket].MmiohResourceNeeds) &&
               ((UnAllocatedMmioh + Take) >= SocketResources[Socket].MmiohResourceNeeds)) {
      //
      // Apply unallocated Mmioh to the socket that requires more to satisfy its request
      // that's outside the allocated range given to the whole system originally.
      //
      SocketResources[Socket].MmiohResourceNeeds -= Take;
      UnAllocatedMmioh -= SocketResources[Socket].MmiohResourceNeeds;
      DEBUG ((DEBUG_INFO, "SocketResources[%x].MmiohResourceNeeds = %llX\n", Socket, SocketResources[Socket].MmiohResourceNeeds));
      DEBUG ((DEBUG_INFO, "Unallocated MMIOH left = %llX\n", UnAllocatedMmioh));
      SocketResources[Socket].MmiohResourceNeeds = 0;
    }
  }
  //
  // Give away leftover resources
  //
  LastStack = LastStackOfSocket (LastSocket);
  if (Take != 0) {
    if (SocketResources[LastSocket].StackRes[LastStack].MmiohLength != 0) {
      SocketResources[LastSocket].StackRes[LastStack].MmiohLength += Take;
    } else{
      SocketResources[LastSocket].StackRes[LastStack].MmiohLength += (Take - 1);
    }
  }
  //
  // Verify all resource requested can fit into the systems address range.
  //
  TotalResourceSize = 0;
  for (Socket = 0; Socket < ValidSockets; Socket++) {
    LastStackWithResources (&SocketResources[Socket], Socket, ResourceType, &LastStack, &ResourceSize);
    TotalResourceSize += ResourceSize;
  }
  DEBUG ((DEBUG_INFO, "MaxMmioh                 = %016llXh\n", MaxMmioh));
  DEBUG ((DEBUG_INFO, "Total Request MMIOH Range= %016llXh\n", TotalResourceSize));
  DEBUG ((DEBUG_INFO, "Total System MMIOH Range = %016llXh\n", (MaxMmioh - TempMmioBase)));
  if (TotalResourceSize > MaxMmioh) {
    //
    // Not enough system resources to support the request.
    // Remove all request to update NVRAM variable for this resource type.
    //
    for (Socket = 0; Socket < ValidSockets; Socket ++) {
      for (Stack = 0; Stack < MAX_IIO_STACK; Stack ++) {
        if (!(mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1 << Stack))) {
          continue;
        }
        SocketResources[Socket].StackRes[Stack].MmiohUpdate = 0;
      }
    }
    DEBUG ((DEBUG_ERROR, "[PCI] ERROR: Out of adjustable MMIOH resources. Can't adjust across sockets\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_ERROR, "Assigning new socket MMIOH range...\n"));
  for (Socket = 0, TempMmioLimit = TempMmioBase - 1; Socket < ValidSockets; Socket++) {

    SocketResources[Socket].MmiohBase = TempMmioLimit + 1;
    //
    // Update the stacks base and limit values.
    //
    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {

      if (!(mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].stackPresentBitmap & (1 << Stack))) {

       SocketResources[Socket].StackRes[Stack].MmiohBase = 0;
       SocketResources[Socket].StackRes[Stack].MmiohLimit = 0;

      } else {

        SocketResources[Socket].StackRes[Stack].MmiohBase = TempMmioLimit + 1;
        if (SocketResources[Socket].StackRes[Stack].MmiohLength != 0) {
          //
          // MmiohLength is actually length-1, so we should move TempMmioLimit by MmiohLength+1,
          // but only when it is >0, i.e. only for stack that has resources.
          //
          TempMmioLimit += SocketResources[Socket].StackRes[Stack].MmiohLength + 1;
          SocketResources[Socket].StackRes[Stack].MmiohLimit = TempMmioLimit;

        } else {

          SocketResources[Socket].StackRes[Stack].MmiohLimit = SocketResources[Socket].StackRes[Stack].MmiohBase;
        }
        SocketResources[Socket].StackRes[Stack].MmiohUpdate = 1;
      }
      DEBUG ((DEBUG_ERROR, "SocketResources[%x].StackRes[%x].MmiohBase  = %llX   newLength = %llX\n", Socket, Stack,
              SocketResources[Socket].StackRes[Stack].MmiohBase, SocketResources[Socket].StackRes[Stack].MmiohLength));
      DEBUG ((DEBUG_ERROR, "SocketResources[%x].StackRes[%x].MmiohLimit = %llX\n", Socket, Stack,
              SocketResources[Socket].StackRes[Stack].MmiohLimit));
      DEBUG ((DEBUG_ERROR, "SocketResources[%x].StackRes[%x].MmiohUpdate= %d\n", Socket, Stack,
              SocketResources[Socket].StackRes[Stack].MmiohUpdate));
    } // for (Stack...)

    SocketResources[Socket].MmiohLimit = TempMmioLimit;
  } // for (Socket...)
  return EFI_SUCCESS;
}

