/** @file
  IIO PCI Root Bridge Io Protocol code. Generic enough to work for all IIOs.
  Does not support configuration accesses to the extended PCI Express registers yet.

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PciHostBridge.h"
#include "PciRootBridge.h"

#include <Protocol/IioUds.h>
#include <Protocol/DynamicSiLibraryProtocol.h>

#include "PciRebalance.h"

extern EFI_IIO_UDS_PROTOCOL *mIioUds;


//
// Pci Root Bridge Io Module Variables
//
EFI_CPU_IO2_PROTOCOL               *mCpuIo;
STATIC DYNAMIC_SI_LIBARY_PROTOCOL  *mDynamicSiLibraryProtocol;

/**
  Construct the Pci Root Bridge Io protocol.

  @param[out] Protocol         - Protocol to initialize.
  @param[in]  HostBridgeHandle - Handle to the HostBridge.
  @param[in]  ResAperture      - Resource apperture of the root bridge.
  @param[in]  SegmentNumber    - PCI segment of this root bridge
  @param[in]  AllocAttributes  - Attribute of resouce allocated.

  @retval EFI_SUCCESS  -  Success.
  @retval Others       -  Fail.
**/
EFI_STATUS
SimpleIioRootBridgeConstructor (
     OUT EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *Protocol,
  IN     EFI_HANDLE                         HostBridgeHandle,
  IN     PCI_ROOT_BRIDGE_RESOURCE_APERTURE *ResAperture,
  IN     UINT16                             SegmentNumber,
  IN     UINT64                             AllocAttributes
  )
{
  EFI_STATUS                Status;
  PCI_ROOT_BRIDGE_INSTANCE  *RootBridge;
  PCI_RESOURCE_TYPE         Index;
  UINT32                    HecBase;
  UINT32                    HecSize;

  RootBridge = ROOT_BRIDGE_FROM_THIS (Protocol);

  //
  // Initialize the apertures with default values
  //
  CopyMem (
    &RootBridge->Aperture,
    ResAperture,
    sizeof (PCI_ROOT_BRIDGE_RESOURCE_APERTURE)
    );

  for (Index = TypeIo; Index < TypeMax; Index++) {
    RootBridge->ResAllocNode[Index].Type   = Index;
    RootBridge->ResAllocNode[Index].Base   = 0;
    RootBridge->ResAllocNode[Index].Length = 0;
    RootBridge->ResAllocNode[Index].Status = ResNone;
  }

  EfiInitializeLock (&RootBridge->PciLock, TPL_HIGH_LEVEL);
  RootBridge->PciAddress             = 0xCF8;
  RootBridge->PciData                = 0xCFC;

  RootBridge->AllocationAttributes   = AllocAttributes;
  RootBridge->Attributes             = 0;
  RootBridge->Supports = EFI_PCI_ATTRIBUTE_IDE_PRIMARY_IO |
    EFI_PCI_ATTRIBUTE_IDE_SECONDARY_IO |
    EFI_PCI_ATTRIBUTE_ISA_IO_16         |
    EFI_PCI_ATTRIBUTE_VGA_PALETTE_IO_16 |
    EFI_PCI_ATTRIBUTE_VGA_MEMORY |
    EFI_PCI_ATTRIBUTE_VGA_IO_16;

  //
  // Don't support BASE above 4GB currently
  // Position to bit 39:28
  //
  HecBase = (UINT32) mIioUds->IioUdsPtr->PlatformData.PciExpressBase;
  HecSize = (UINT32) mIioUds->IioUdsPtr->PlatformData.PciExpressSize;
  ASSERT (HecBase != 0);

  RootBridge->HecBase            = HecBase;
  RootBridge->HecLen             = HecSize;

  RootBridge->BusNumberAssigned  = FALSE;
  RootBridge->BusScanCount       = 0;

  Protocol->ParentHandle          = HostBridgeHandle;

  Protocol->PollMem               = RootBridgeIoPollMem;
  Protocol->PollIo                = RootBridgeIoPollIo;

  Protocol->Mem.Read              = RootBridgeIoMemRead;
  Protocol->Mem.Write             = RootBridgeIoMemWrite;

  Protocol->Io.Read               = RootBridgeIoIoRead;
  Protocol->Io.Write              = RootBridgeIoIoWrite;

  Protocol->CopyMem               = RootBridgeIoCopyMem;

  Protocol->Pci.Read              = RootBridgeIoPciRead;
  Protocol->Pci.Write             = RootBridgeIoPciWrite;

  Protocol->Map                   = RootBridgeIoMap;
  Protocol->Unmap                 = RootBridgeIoUnmap;

  Protocol->AllocateBuffer        = RootBridgeIoAllocateBuffer;
  Protocol->FreeBuffer            = RootBridgeIoFreeBuffer;

  Protocol->Flush                 = RootBridgeIoFlush;

  Protocol->GetAttributes         = RootBridgeIoGetAttributes;
  Protocol->SetAttributes         = RootBridgeIoSetAttributes;

  Protocol->Configuration         = RootBridgeIoConfiguration;

  Protocol->SegmentNumber         = SegmentNumber;

  Status = gBS->LocateProtocol (
                  &gEfiCpuIo2ProtocolGuid,
                  NULL,
                  &mCpuIo
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gDynamicSiLibraryProtocolGuid, NULL, &mDynamicSiLibraryProtocol);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

/**
  Return the result of (Multiplicand * Multiplier / Divisor).

  @param Multiplicand A 64-bit unsigned value.
  @param Multiplier   A 64-bit unsigned value.
  @param Divisor      A 32-bit unsigned value.
  @param Remainder    A pointer to a 32-bit unsigned value. This parameter is
                      optional and may be NULL.

  @return Multiplicand * Multiplier / Divisor.
**/
UINT64
MultThenDivU64x64x32 (
  IN      UINT64                    Multiplicand,
  IN      UINT64                    Multiplier,
  IN      UINT32                    Divisor,
  OUT     UINT32                    *Remainder  OPTIONAL
  )
{
  UINT64                            Uint64;
  UINT32                            LocalRemainder;
  UINT32                            Uint32;
  if (Multiplicand > DivU64x64Remainder (MAX_UINT64, Multiplier, NULL)) {
    //
    // Make sure Multiplicand is the bigger one.
    //
    if (Multiplicand < Multiplier) {
      Uint64       = Multiplicand;
      Multiplicand = Multiplier;
      Multiplier   = Uint64;
    }
    //
    // Because Multiplicand * Multiplier overflows,
    //   Multiplicand * Multiplier / Divisor
    // = (2 * Multiplicand' + 1) * Multiplier / Divisor
    // = 2 * (Multiplicand' * Multiplier / Divisor) + Multiplier / Divisor
    //
    Uint64 = MultThenDivU64x64x32 (RShiftU64 (Multiplicand, 1), Multiplier, Divisor, &LocalRemainder);
    Uint64 = LShiftU64 (Uint64, 1);
    Uint32 = 0;
    if ((Multiplicand & 0x1) == 1) {
      Uint64 += DivU64x32Remainder (Multiplier, Divisor, &Uint32);
    }
    return Uint64 + DivU64x32Remainder (Uint32 + LShiftU64 (LocalRemainder, 1), Divisor, Remainder);
  } else {
    return DivU64x32Remainder (MultU64x64 (Multiplicand, Multiplier), Divisor, Remainder);
  }
}

/**
  Return the elapsed tick count from CurrentTick.

  @param  CurrentTick  On input, the previous tick count.
                       On output, the current tick count.
  @param  StartTick    The value the performance counter starts with when it
                       rolls over.
  @param  EndTick      The value that the performance counter ends with before
                       it rolls over.

  @return  The elapsed tick count from CurrentTick.
**/
UINT64
GetElapsedTick (
  UINT64  *CurrentTick,
  UINT64  StartTick,
  UINT64  EndTick
  )
{
  UINT64  PreviousTick;

  PreviousTick = *CurrentTick;
  *CurrentTick = GetPerformanceCounter();
  if (StartTick < EndTick) {
    return *CurrentTick - PreviousTick;
  } else {
    return PreviousTick - *CurrentTick;
  }
}

/**
  Polls an address in memory mapped I/O space until an exit condition is met,
  or a timeout occurs.

  This function provides a standard way to poll a PCI memory location. A PCI
  memory read operation is performed at the PCI memory address specified by
  Address for the width specified by Width. The result of this PCI memory read
  operation is stored in Result. This PCI memory read operation is repeated
  until either a timeout of Delay 100 ns units has expired, or (Result & Mask)
  is equal to Value.

  @param[in]   This      A pointer to the EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL.
  @param[in]   Width     Signifies the width of the memory operations.
  @param[in]   Address   The base address of the memory operations. The caller
                         is responsible for aligning Address if required.
  @param[in]   Mask      Mask used for the polling criteria. Bytes above Width
                         in Mask are ignored. The bits in the bytes below Width
                         which are zero in Mask are ignored when polling the
                         memory address.
  @param[in]   Value     The comparison value used for the polling exit
                         criteria.
  @param[in]   Delay     The number of 100 ns units to poll. Note that timer
                         available may be of poorer granularity.
  @param[out]  Result    Pointer to the last value read from the memory
                         location.

  @retval EFI_SUCCESS            The last data returned from the access matched
                                 the poll exit criteria.
  @retval EFI_INVALID_PARAMETER  Width is invalid.
  @retval EFI_INVALID_PARAMETER  Result is NULL.
  @retval EFI_TIMEOUT            Delay expired before a match occurred.
  @retval EFI_OUT_OF_RESOURCES   The request could not be completed due to a
                                 lack of resources.
**/
EFI_STATUS
EFIAPI
RootBridgeIoPollMem (
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN  UINT64                                 Address,
  IN  UINT64                                 Mask,
  IN  UINT64                                 Value,
  IN  UINT64                                 Delay,
  OUT UINT64                                 *Result
  )
{
  EFI_STATUS  Status;
  UINT64      NumberOfTicks;
  UINT32      Remainder;
  UINT64      StartTick;
  UINT64      EndTick;
  UINT64      CurrentTick;
  UINT64      ElapsedTick;
  UINT64      Frequency;

  if (Result == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if ((UINT32)Width > EfiPciWidthUint64) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // No matter what, always do a single poll.
  //
  Status = This->Mem.Read (This, Width, Address, 1, Result);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if ((*Result & Mask) == Value) {
    return EFI_SUCCESS;
  }

  if (Delay == 0) {
    return EFI_SUCCESS;

  } else {
    //
    // NumberOfTicks = Frenquency * Delay / EFI_TIMER_PERIOD_SECONDS(1)
    //
    Frequency     = GetPerformanceCounterProperties (&StartTick, &EndTick);
    NumberOfTicks = MultThenDivU64x64x32 (Frequency, Delay, (UINT32)EFI_TIMER_PERIOD_SECONDS(1), &Remainder);
    if (Remainder >= (UINTN)EFI_TIMER_PERIOD_SECONDS(1) / 2) {
      NumberOfTicks++;
    }
    for ( ElapsedTick = 0, CurrentTick = GetPerformanceCounter()
        ; ElapsedTick <= NumberOfTicks
        ; ElapsedTick += GetElapsedTick (&CurrentTick, StartTick, EndTick)
        ) {
      Status = This->Mem.Read (This, Width, Address, 1, Result);
      if (EFI_ERROR (Status)) {
        return Status;
      }

      if ((*Result & Mask) == Value) {
        return EFI_SUCCESS;
      }
    }
  }
  return EFI_TIMEOUT;
}

/**
  Reads from the I/O space of a PCI Root Bridge. Returns when either the
  polling exit criteria is satisfied or after a defined duration.

  This function provides a standard way to poll a PCI I/O location. A PCI I/O
  read operation is performed at the PCI I/O address specified by Address for
  the width specified by Width.
  The result of this PCI I/O read operation is stored in Result. This PCI I/O
  read operation is repeated until either a timeout of Delay 100 ns units has
  expired, or (Result & Mask) is equal to Value.

  @param[in] This      A pointer to the EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL.
  @param[in] Width     Signifies the width of the I/O operations.
  @param[in] Address   The base address of the I/O operations. The caller is
                       responsible for aligning Address if required.
  @param[in] Mask      Mask used for the polling criteria. Bytes above Width in
                       Mask are ignored. The bits in the bytes below Width
                       which are zero in Mask are ignored when polling the I/O
                       address.
  @param[in] Value     The comparison value used for the polling exit criteria.
  @param[in] Delay     The number of 100 ns units to poll. Note that timer
                       available may be of poorer granularity.
  @param[out] Result   Pointer to the last value read from the memory location.

  @retval EFI_SUCCESS            The last data returned from the access matched
                                 the poll exit criteria.
  @retval EFI_INVALID_PARAMETER  Width is invalid.
  @retval EFI_INVALID_PARAMETER  Result is NULL.
  @retval EFI_TIMEOUT            Delay expired before a match occurred.
  @retval EFI_OUT_OF_RESOURCES   The request could not be completed due to a
                                 lack of resources.
**/
EFI_STATUS
EFIAPI
RootBridgeIoPollIo (
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN  UINT64                                 Address,
  IN  UINT64                                 Mask,
  IN  UINT64                                 Value,
  IN  UINT64                                 Delay,
  OUT UINT64                                 *Result
  )
{
  EFI_STATUS  Status;
  UINT64      NumberOfTicks;
  UINT32      Remainder;
  UINT64      StartTick;
  UINT64      EndTick;
  UINT64      CurrentTick;
  UINT64      ElapsedTick;
  UINT64      Frequency;

  //
  // No matter what, always do a single poll.
  //

  if (Result == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if ((UINT32)Width > EfiPciWidthUint64) {
    return EFI_INVALID_PARAMETER;
  }

  Status = This->Io.Read (This, Width, Address, 1, Result);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  if ((*Result & Mask) == Value) {
    return EFI_SUCCESS;
  }

  if (Delay == 0) {
    return EFI_SUCCESS;

  } else {
    //
    // NumberOfTicks = Frenquency * Delay / EFI_TIMER_PERIOD_SECONDS(1)
    //
    Frequency     = GetPerformanceCounterProperties (&StartTick, &EndTick);
    NumberOfTicks = MultThenDivU64x64x32 (Frequency, Delay, (UINT32)EFI_TIMER_PERIOD_SECONDS(1), &Remainder);
    if (Remainder >= (UINTN)EFI_TIMER_PERIOD_SECONDS(1) / 2) {
      NumberOfTicks++;
    }
    for ( ElapsedTick = 0, CurrentTick = GetPerformanceCounter()
        ; ElapsedTick <= NumberOfTicks
        ; ElapsedTick += GetElapsedTick (&CurrentTick, StartTick, EndTick)
        ) {
      Status = This->Io.Read (This, Width, Address, 1, Result);
      if (EFI_ERROR (Status)) {
        return Status;
      }

      if ((*Result & Mask) == Value) {
        return EFI_SUCCESS;
      }
    }
  }
  return EFI_TIMEOUT;
}

/**
  Enables a PCI driver to access PCI controller registers in the PCI root
  bridge memory space.

  The Mem.Read(), and Mem.Write() functions enable a driver to access PCI
  controller registers in the PCI root bridge memory space.
  The memory operations are carried out exactly as requested. The caller is
  responsible for satisfying any alignment and memory width restrictions that a
  PCI Root Bridge on a platform might require.

  @param[in]   This      A pointer to the EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL.
  @param[in]   Width     Signifies the width of the memory operation.
  @param[in]   Address   The base address of the memory operation. The caller
                         is responsible for aligning the Address if required.
  @param[in]   Count     The number of memory operations to perform. Bytes
                         moved is Width size * Count, starting at Address.
  @param[out]  Buffer    For read operations, the destination buffer to store
                         the results. For write operations, the source buffer
                         to write data from.

  @retval EFI_SUCCESS            The data was read from or written to the PCI
                                 root bridge.
  @retval EFI_INVALID_PARAMETER  Width is invalid for this PCI root bridge.
  @retval EFI_INVALID_PARAMETER  Buffer is NULL.
  @retval EFI_OUT_OF_RESOURCES   The request could not be completed due to a
                                 lack of resources.
**/
EFI_STATUS
EFIAPI
RootBridgeIoMemRead (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN     UINT64                                 Address,
  IN     UINTN                                  Count,
  OUT    VOID                                   *Buffer
  )
{
  PCI_ROOT_BRIDGE_INSTANCE  *RootBridge;
  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Width < 0 ||Width >= EfiPciWidthMaximum  ) {
    return EFI_INVALID_PARAMETER;
  }

  RootBridge = ROOT_BRIDGE_FROM_THIS (This);
  //
  // Check memory access limit
  //
  if (RootBridge->Aperture.Mem64Limit > RootBridge->Aperture.Mem64Base) {
      if (Address > RootBridge->Aperture.Mem64Limit) {
        return EFI_INVALID_PARAMETER;
      }
  } else {
      if (Address > RootBridge->Aperture.Mem32Limit) {
        return EFI_INVALID_PARAMETER;
      }
  }

  return mCpuIo->Mem.Read (
                      mCpuIo,
                      (EFI_CPU_IO_PROTOCOL_WIDTH) Width,
                      Address,
                      Count,
                      Buffer
                      );
}

/**
  Enables a PCI driver to access PCI controller registers in the PCI root
  bridge memory space.

  The Mem.Read(), and Mem.Write() functions enable a driver to access PCI
  controller registers in the PCI root bridge memory space.
  The memory operations are carried out exactly as requested. The caller is
  responsible for satisfying any alignment and memory width restrictions that a
  PCI Root Bridge on a platform might require.

  @param[in]   This      A pointer to the EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL.
  @param[in]   Width     Signifies the width of the memory operation.
  @param[in]   Address   The base address of the memory operation. The caller
                         is responsible for aligning the Address if required.
  @param[in]   Count     The number of memory operations to perform. Bytes
                         moved is Width size * Count, starting at Address.
  @param[in]   Buffer    For read operations, the destination buffer to store
                         the results. For write operations, the source buffer
                         to write data from.

  @retval EFI_SUCCESS            The data was read from or written to the PCI
                                 root bridge.
  @retval EFI_INVALID_PARAMETER  Width is invalid for this PCI root bridge.
  @retval EFI_INVALID_PARAMETER  Buffer is NULL.
  @retval EFI_OUT_OF_RESOURCES   The request could not be completed due to a
                                 lack of resources.
**/
EFI_STATUS
EFIAPI
RootBridgeIoMemWrite (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN     UINT64                                 Address,
  IN     UINTN                                  Count,
  IN     VOID                                   *Buffer
  )
{
  PCI_ROOT_BRIDGE_INSTANCE  *RootBridge;

  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Width < 0 || Width >= EfiPciWidthMaximum ) {
    return EFI_INVALID_PARAMETER;
  }

  RootBridge = ROOT_BRIDGE_FROM_THIS (This);

  //
  // Check memory access limit
  //
  if (RootBridge->Aperture.Mem64Limit > RootBridge->Aperture.Mem64Base) {
      if (Address > RootBridge->Aperture.Mem64Limit) {
        return EFI_INVALID_PARAMETER;
      }
  } else {
      if (Address > RootBridge->Aperture.Mem32Limit) {
        return EFI_INVALID_PARAMETER;
      }
  }
  return mCpuIo->Mem.Write (
                      mCpuIo,
                      (EFI_CPU_IO_PROTOCOL_WIDTH) Width,
                      Address,
                      Count,
                      Buffer
                      );
}

/**
  Enables a PCI driver to access PCI controller registers in the PCI root
  bridge I/O space.

  @param[in]   This        A pointer to the EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL.
  @param[in]   Width       Signifies the width of the memory operations.
  @param[in]   Address     The base address of the I/O operation. The caller is
                           responsible for aligning the Address if required.
  @param[in]   Count       The number of I/O operations to perform. Bytes moved
                           is Width size * Count, starting at Address.
  @param[out]  Buffer      For read operations, the destination buffer to store
                           the results. For write operations, the source buffer
                           to write data from.

  @retval EFI_SUCCESS              The data was read from or written to the PCI
                                   root bridge.
  @retval EFI_INVALID_PARAMETER    Width is invalid for this PCI root bridge.
  @retval EFI_INVALID_PARAMETER    Buffer is NULL.
  @retval EFI_OUT_OF_RESOURCES     The request could not be completed due to a
                                   lack of resources.
**/
EFI_STATUS
EFIAPI
RootBridgeIoIoRead (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN     UINT64                                 Address,
  IN     UINTN                                  Count,
  OUT    VOID                                   *Buffer
  )
{
  UINTN                     AlignMask;
  PCI_ROOT_BRIDGE_INSTANCE  *RootBridge;

  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Width < 0 || Width >= EfiPciWidthMaximum ) {
    return EFI_INVALID_PARAMETER;
  }

  RootBridge = ROOT_BRIDGE_FROM_THIS (This);

  //
  // AlignMask = (1 << Width) - 1;
  //
  AlignMask = (1 << (Width & 0x03)) - 1;

  //
  // check Io access limit
  //
  if (Address > RootBridge->Aperture.IoLimit) {
    return EFI_INVALID_PARAMETER;
  }

  if (Address & AlignMask) {
    return EFI_INVALID_PARAMETER;
  }

  return mCpuIo->Io.Read (
                      mCpuIo,
                      (EFI_CPU_IO_PROTOCOL_WIDTH) Width,
                      Address,
                      Count,
                      Buffer
                      );

}

/**
  Enables a PCI driver to access PCI controller registers in the PCI root
  bridge I/O space.

  @param[in]   This        A pointer to the EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL.
  @param[in]   Width       Signifies the width of the memory operations.
  @param[in]   Address     The base address of the I/O operation. The caller is
                           responsible for aligning the Address if required.
  @param[in]   Count       The number of I/O operations to perform. Bytes moved
                           is Width size * Count, starting at Address.
  @param[in]   Buffer      For read operations, the destination buffer to store
                           the results. For write operations, the source buffer
                           to write data from.

  @retval EFI_SUCCESS              The data was read from or written to the PCI
                                   root bridge.
  @retval EFI_INVALID_PARAMETER    Width is invalid for this PCI root bridge.
  @retval EFI_INVALID_PARAMETER    Buffer is NULL.
  @retval EFI_OUT_OF_RESOURCES     The request could not be completed due to a
                                   lack of resources.
**/
EFI_STATUS
EFIAPI
RootBridgeIoIoWrite (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL         *This,
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH   Width,
  IN       UINT64                                  Address,
  IN       UINTN                                   Count,
  IN       VOID                                    *Buffer
  )
{
  UINTN                     AlignMask;
  PCI_ROOT_BRIDGE_INSTANCE  *RootBridge;

  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Width < 0 || Width >= EfiPciWidthMaximum ) {
    return EFI_INVALID_PARAMETER;
  }

  RootBridge = ROOT_BRIDGE_FROM_THIS (This);

  //
  // AlignMask = (1 << Width) - 1;
  //
  AlignMask = (1 << (Width & 0x03)) - 1;

  //
  // Check Io access limit
  //
  if (Address > RootBridge->Aperture.IoLimit) {
    return EFI_INVALID_PARAMETER;
  }

  if (Address & AlignMask) {
    return EFI_INVALID_PARAMETER;
  }

  return mCpuIo->Io.Write (
                      mCpuIo,
                      (EFI_CPU_IO_PROTOCOL_WIDTH) Width,
                      Address,
                      Count,
                      Buffer
                      );

}

/**
  Enables a PCI driver to copy one region of PCI root bridge memory space to
  another region of PCI root bridge memory space.

  The CopyMem() function enables a PCI driver to copy one region of PCI root
  bridge memory space to another region of PCI root bridge memory space. This
  is especially useful for video scroll operation on a memory mapped video
  buffer.
  The memory operations are carried out exactly as requested. The caller is
  responsible for satisfying any alignment and memory width restrictions that a
  PCI root bridge on a platform might require.

  @param[in] This        A pointer to the EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL
                         instance.
  @param[in] Width       Signifies the width of the memory operations.
  @param[in] DestAddress The destination address of the memory operation. The
                         caller is responsible for aligning the DestAddress if
                         required.
  @param[in] SrcAddress  The source address of the memory operation. The caller
                         is responsible for aligning the SrcAddress if
                         required.
  @param[in] Count       The number of memory operations to perform. Bytes
                         moved is Width size * Count, starting at DestAddress
                         and SrcAddress.

  @retval  EFI_SUCCESS             The data was copied from one memory region
                                   to another memory region.
  @retval  EFI_INVALID_PARAMETER   Width is invalid for this PCI root bridge.
  @retval  EFI_OUT_OF_RESOURCES    The request could not be completed due to a
                                   lack of resources.
**/
EFI_STATUS
EFIAPI
RootBridgeIoCopyMem (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL              *This,
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH        Width,
  IN UINT64                                       DestAddress,
  IN UINT64                                       SrcAddress,
  IN UINTN                                        Count
  )
{
  EFI_STATUS  Status;
  BOOLEAN     Forward;
  UINTN       Stride;
  UINTN       Index;
  UINT64      Result;

  if ((UINT32) Width > EfiPciWidthUint64) {
    return EFI_INVALID_PARAMETER;
  }

  if (DestAddress == SrcAddress) {
    return EFI_SUCCESS;
  }

  Stride = (UINTN)1 << Width;

  Forward = TRUE;
  if ((DestAddress > SrcAddress) &&
      (DestAddress < (SrcAddress + Count * Stride))) {
    Forward = FALSE;
    SrcAddress = SrcAddress + (Count - 1) * Stride;
    DestAddress = DestAddress + (Count - 1) * Stride;
  }

  for (Index = 0; Index < Count; Index++) {
    Status = RootBridgeIoMemRead (
               This,
               Width,
               SrcAddress,
               1,
               &Result
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    Status = RootBridgeIoMemWrite (
               This,
               Width,
               DestAddress,
               1,
               &Result
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    if (Forward) {
      SrcAddress += Stride;
      DestAddress += Stride;
    } else {
      SrcAddress -= Stride;
      DestAddress -= Stride;
    }
  }
  return EFI_SUCCESS;
}


/**

Arguments:

**/
STATIC
EFI_STATUS
RootBridgeIoPciRW (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *This,
  IN BOOLEAN                                Write,
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH  Width,
  IN UINT64                                 UserAddress,
  IN UINTN                                  Count,
  IN OUT VOID                              *UserBuffer
  )
{
  PCI_CONFIG_ACCESS_CF8             Pci;
  PCI_CONFIG_ACCESS_CF8             PciAligned;
  UINT32                            Stride;
  UINTN                             PciData;
  UINTN                             PciDataStride;
  PCI_ROOT_BRIDGE_INSTANCE         *RootBridge;

  if (Width >= EfiPciWidthMaximum) {
    return EFI_INVALID_PARAMETER;
  }

  RootBridge = ROOT_BRIDGE_FROM_THIS (This);

  ASSERT (((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS*)&UserAddress)->ExtendedRegister == 0x00);

  Stride = 1 << Width;

  Pci.Bits.Reg = ((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS*) &UserAddress)->Register;
  Pci.Bits.Func = ((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS*) &UserAddress)->Function;
  Pci.Bits.Dev = ((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS*) &UserAddress)->Device;
  Pci.Bits.Bus = ((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS*) &UserAddress)->Bus;
  Pci.Bits.Reserved = 0;
  Pci.Bits.Enable = 1;

  //
  // PCI Configure access are all 32-bit aligned, but by accessing the
  //  CONFIG_DATA_REGISTER (0xcfc) with different widths more cycle types
  //  are possible on PCI.
  //
  // To read a byte of PCI configuration space you load 0xcf8 and
  //  read 0xcfc, 0xcfd, 0xcfe, 0xcff
  //
  PciDataStride = ((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS*) &UserAddress)->Register & 0x03;

  while (Count) {
    PciAligned = Pci;
    PciAligned.Bits.Reg &= 0xfc;
    PciData = RootBridge->PciData + PciDataStride;
    EfiAcquireLock(&RootBridge->PciLock);
    This->Io.Write (This, EfiPciWidthUint32, \
                    RootBridge->PciAddress, 1, &PciAligned);
    if (Write) {
      This->Io.Write (This, Width, PciData, 1, UserBuffer);
    } else {
      This->Io.Read (This, Width, PciData, 1, UserBuffer);
    }
    EfiReleaseLock(&RootBridge->PciLock);
    UserBuffer = ((UINT8 *)UserBuffer) + Stride;
    PciDataStride = (PciDataStride + Stride) % 4;
    Count -= 1;

    //
    // Only increment the PCI address if Width is not a FIFO.
    //
    if (Width >= EfiPciWidthUint8 && Width <= EfiPciWidthUint64) {
      Pci.Bits.Reg += Stride;
    }
  }
  return EFI_SUCCESS;
}

/**
  Allows read from PCI configuration space.

  @param This     A pointer to EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL
  @param Width    Signifies the width of the memory operation.
  @param Address  The address within the PCI configuration space
                  for the PCI controller.
  @param Count    The number of PCI configuration operations
                  to perform.
  @param Buffer   The destination buffer to store the results.

  @retval EFI_SUCCESS           The data was read from the PCI root bridge.
  @retval EFI_INVALID_PARAMETER Invalid parameters found.
**/
EFI_STATUS
EFIAPI
RootBridgeIoPciRead (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL       *This,
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH Width,
  IN     UINT64                                Address,
  IN     UINTN                                 Count,
  IN OUT VOID                                  *Buffer
  )
{
  PCI_ROOT_BRIDGE_INSTANCE  *RootBridge;
  UINT32                    PciBus;
  UINT32                    PciDev;
  UINT32                    PciFn;
  UINT32                    PciExtReg;
  USRA_ADDRESS              EndPointPciAddress;
  UINT8                     *pData8 = Buffer;
  UINT8                     Size;

  Size = 1 << (Width & 0x3);

  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Width < 0 || Width >= EfiPciWidthMaximum ) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Read Pci configuration space
  //
  RootBridge = ROOT_BRIDGE_FROM_THIS (This);

  if (RootBridge->HecBase == 0) {
    return RootBridgeIoPciRW (This, FALSE, Width, Address, Count, Buffer);
  }

  if (!((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS *) &Address)->ExtendedRegister) {
    PciExtReg = ((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS *) &Address)->Register;
  } else {
    PciExtReg = ((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS *) &Address)->ExtendedRegister & 0x0FFF;
  }

  PciBus        = ((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS *) &Address)->Bus;
  PciDev        = ((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS *) &Address)->Device;
  PciFn         = ((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS *) &Address)->Function;

  USRA_BLOCK_PCIE_ADDRESS (EndPointPciAddress, Width, Count, This->SegmentNumber, PciBus, PciDev, PciFn, PciExtReg);
  mDynamicSiLibraryProtocol->RegisterRead (&EndPointPciAddress, pData8);
  return EFI_SUCCESS;
}

/**
  Allows write to PCI configuration space.

  @param This     A pointer to EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL
  @param Width    Signifies the width of the memory operation.
  @param Address  The address within the PCI configuration space
                  for the PCI controller.
  @param Count    The number of PCI configuration operations
                  to perform.
  @param Buffer   The source buffer to get the results.

  @retval EFI_SUCCESS            The data was written to the PCI root bridge.
  @retval EFI_INVALID_PARAMETER  Invalid parameters found.
**/
EFI_STATUS
EFIAPI
RootBridgeIoPciWrite (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL       *This,
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH Width,
  IN     UINT64                                Address,
  IN     UINTN                                 Count,
  IN OUT VOID                                  *Buffer
  )
{
  PCI_ROOT_BRIDGE_INSTANCE  *RootBridge;
  UINT32                    PciBus;
  UINT32                    PciDev;
  UINT32                    PciFn;
  UINT32                    PciExtReg;
  USRA_ADDRESS              EndPointPciAddress;
  UINT8                     *pData8 = Buffer;
  UINT8                     Size;

  Size = 1 << (Width & 0x3);

  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Width < 0 || Width >= EfiPciWidthMaximum) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Write Pci configuration space
  //
  RootBridge = ROOT_BRIDGE_FROM_THIS (This);

  if (RootBridge->HecBase == 0) {
    return RootBridgeIoPciRW (This, TRUE, Width, Address, Count, Buffer);
  }

  if (!((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS *) &Address)->ExtendedRegister) {
    PciExtReg = ((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS *) &Address)->Register;
  } else {
    PciExtReg = ((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS *) &Address)->ExtendedRegister & 0x0FFF;
  }

  PciBus        = ((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS *) &Address)->Bus;
  PciDev        = ((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS *) &Address)->Device;
  PciFn         = ((EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS *) &Address)->Function;

  USRA_BLOCK_PCIE_ADDRESS (EndPointPciAddress, Width, Count, This->SegmentNumber, PciBus, PciDev, PciFn, PciExtReg);
  mDynamicSiLibraryProtocol->RegisterWrite (&EndPointPciAddress, pData8);

  return EFI_SUCCESS;
}

/**
  Provides the PCI controller-specific address needed to access
  system memory for DMA.

  @param This           A pointer to the EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL.
  @param Operation      Indicate if the bus master is going to read or write
                        to system memory.
  @param HostAddress    The system memory address to map on the PCI controller.
  @param NumberOfBytes  On input the number of bytes to map.
                        On output the number of bytes that were mapped.
  @param DeviceAddress  The resulting map address for the bus master PCI
                        controller to use to access the system memory's HostAddress.
  @param Mapping        The value to pass to Unmap() when the bus master DMA
                        operation is complete.

  @retval EFI_SUCCESS            Success.
  @retval EFI_INVALID_PARAMETER  Invalid parameters found.
  @retval EFI_UNSUPPORTED        The HostAddress cannot be mapped as a common buffer.
  @retval EFI_DEVICE_ERROR       The System hardware could not map the requested address.
  @retval EFI_OUT_OF_RESOURCES   The request could not be completed due to lack of resources.
**/
EFI_STATUS
EFIAPI
RootBridgeIoMap (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL            *This,
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_OPERATION  Operation,
  IN     VOID                                       *HostAddress,
  IN OUT UINTN                                      *NumberOfBytes,
  OUT    EFI_PHYSICAL_ADDRESS                       *DeviceAddress,
  OUT    VOID                                       **Mapping
  )
{
  EFI_STATUS                                         Status;
  PCI_ROOT_BRIDGE_INSTANCE                          *RootBridge;
  EFI_PHYSICAL_ADDRESS                               PhysicalAddress;
  MAP_INFO                                          *MapInfo;

  if (NumberOfBytes == NULL || Mapping == NULL || DeviceAddress == NULL || HostAddress == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Initialize the return values to their defaults
  //
  *Mapping = NULL;

  //
  // Make sure that Operation is valid
  //
  if ((UINT32) Operation >= EfiPciOperationMaximum) {
    return EFI_INVALID_PARAMETER;
  }

  RootBridge = ROOT_BRIDGE_FROM_THIS (This);

  if (mIoMmu != NULL) {
    if (!RootBridge->DmaAbove4G) {
      //
      // Clear 64bit support
      //
      if (Operation > EfiPciOperationBusMasterCommonBuffer) {
        Operation = (EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_OPERATION) (Operation - EfiPciOperationBusMasterRead64);
      }
    }
    Status = mIoMmu->Map (
                       mIoMmu,
                       (EDKII_IOMMU_OPERATION) Operation,
                       HostAddress,
                       NumberOfBytes,
                       DeviceAddress,
                       Mapping
                       );
    return Status;
  }
  //
  // Most PCAT like chipsets can not handle performing DMA above 4GB.
  // If any part of the DMA transfer being mapped is above 4GB, then
  // map the DMA transfer to a buffer below 4GB.
  //
  PhysicalAddress = (EFI_PHYSICAL_ADDRESS) (UINTN) HostAddress;
  if ((PhysicalAddress + *NumberOfBytes) > SIZE_4GB) {
    //
    // Common Buffer operations can not be remapped.  If the common buffer
    // if above 4GB, then it is not possible to generate a mapping, so return
    // an error.
    //
    if (Operation == EfiPciOperationBusMasterCommonBuffer || Operation == EfiPciOperationBusMasterCommonBuffer64) {
      return EFI_INVALID_PARAMETER;
    }
    //
    // Allocate a MAP_INFO structure to remember the mapping when Unmap() is
    // called later.
    //
    MapInfo = AllocatePool (sizeof (MAP_INFO));
    if (MapInfo == NULL) {
      *NumberOfBytes = 0;
      return EFI_OUT_OF_RESOURCES;
    }
    //
    // Return a pointer to the MAP_INFO structure in Mapping
    //
    *Mapping = MapInfo;

    //
    // Initialize the MAP_INFO structure
    //
    MapInfo->Operation = Operation;
    MapInfo->NumberOfBytes = *NumberOfBytes;
    MapInfo->NumberOfPages = EFI_SIZE_TO_PAGES (*NumberOfBytes);
    MapInfo->HostAddress = PhysicalAddress;
    MapInfo->MappedHostAddress = SIZE_4GB - 1;

    //
    // Allocate a buffer below 4GB to map the transfer to.
    //
    Status = gBS->AllocatePages (
                    AllocateMaxAddress,
                    EfiBootServicesData,
                    MapInfo->NumberOfPages,
                    &MapInfo->MappedHostAddress
                    );
    if (EFI_ERROR (Status)) {
      FreePool (MapInfo);
      *NumberOfBytes = 0;
      return Status;
    }
    //
    // If this is a read operation from the Bus Master's point of view,
    // then copy the contents of the real buffer into the mapped buffer
    // so the Bus Master can read the contents of the real buffer.
    //
    if (Operation == EfiPciOperationBusMasterRead || Operation == EfiPciOperationBusMasterRead64) {
      CopyMem (
        (VOID *) (UINTN) MapInfo->MappedHostAddress,
        (VOID *) (UINTN) MapInfo->HostAddress,
        MapInfo->NumberOfBytes
        );
    }
    //
    // The DeviceAddress is the address of the maped buffer below 4GB
    //
    *DeviceAddress = MapInfo->MappedHostAddress;
  } else {
    //
    // The transfer is below 4GB, so the DeviceAddress is simply the HostAddress
    //
    *DeviceAddress = PhysicalAddress;
  }

  return EFI_SUCCESS;
}

/**
  Completes the Map() operation and releases any corresponding resources.

  The Unmap() function completes the Map() operation and releases any
  corresponding resources.
  If the operation was an EfiPciOperationBusMasterWrite or
  EfiPciOperationBusMasterWrite64, the data is committed to the target system
  memory.
  Any resources used for the mapping are freed.

  @param[in] This      A pointer to the EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL.
  @param[in] Mapping   The mapping value returned from Map().

  @retval EFI_SUCCESS            The range was unmapped.
  @retval EFI_INVALID_PARAMETER  Mapping is not a value that was returned by Map().
  @retval EFI_DEVICE_ERROR       The data was not committed to the target system memory.
**/
EFI_STATUS
EFIAPI
RootBridgeIoUnmap (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This,
  IN VOID                             *Mapping
  )
{
  EFI_STATUS                           Status;
  MAP_INFO                            *MapInfo;

  if (mIoMmu != NULL) {
    Status = mIoMmu->Unmap (
                       mIoMmu,
                       Mapping
                       );
    return Status;
  }
  //
  // See if the Map() operation associated with this Unmap() required a mapping buffer.
  // If a mapping buffer was not required, then this function simply returns EFI_SUCCESS.
  //
  if (Mapping != NULL) {
    //
    // Get the MAP_INFO structure from Mapping
    //
    MapInfo = (MAP_INFO *) Mapping;

    //
    // If this is a write operation from the Bus Master's point of view,
    // then copy the contents of the mapped buffer into the real buffer
    // so the processor can read the contents of the real buffer.
    //
    if ((MapInfo->Operation == EfiPciOperationBusMasterWrite) ||
        (MapInfo->Operation == EfiPciOperationBusMasterWrite64)
        ) {
      CopyMem (
        (VOID *) (UINTN) MapInfo->HostAddress,
        (VOID *) (UINTN) MapInfo->MappedHostAddress,
        MapInfo->NumberOfBytes
        );
    }
    //
    // Free the mapped buffer and the MAP_INFO structure.
    //
    gBS->FreePages (MapInfo->MappedHostAddress, MapInfo->NumberOfPages);
    FreePool (Mapping);
  }

  return EFI_SUCCESS;
}

/**
  Allocates pages that are suitable for an EfiPciOperationBusMasterCommonBuffer
  or EfiPciOperationBusMasterCommonBuffer64 mapping.

  @param This        A pointer to the EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL.
  @param Type        This parameter is not used and must be ignored.
  @param MemoryType  The type of memory to allocate, EfiBootServicesData or
                     EfiRuntimeServicesData.
  @param Pages       The number of pages to allocate.
  @param HostAddress A pointer to store the base system memory address of the
                     allocated range.
  @param Attributes  The requested bit mask of attributes for the allocated
                     range. Only the attributes
                     EFI_PCI_ATTRIBUTE_MEMORY_WRITE_COMBINE,
                     EFI_PCI_ATTRIBUTE_MEMORY_CACHED, and
                     EFI_PCI_ATTRIBUTE_DUAL_ADDRESS_CYCLE may be used with this
                     function.

  @retval EFI_SUCCESS            The requested memory pages were allocated.
  @retval EFI_INVALID_PARAMETER  MemoryType is invalid.
  @retval EFI_INVALID_PARAMETER  HostAddress is NULL.
  @retval EFI_UNSUPPORTED        Attributes is unsupported. The only legal
                                 attribute bits are MEMORY_WRITE_COMBINE,
                                 MEMORY_CACHED, and DUAL_ADDRESS_CYCLE.
  @retval EFI_OUT_OF_RESOURCES   The memory pages could not be allocated.
**/
EFI_STATUS
EFIAPI
RootBridgeIoAllocateBuffer (
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This,
  IN  EFI_ALLOCATE_TYPE                Type,
  IN  EFI_MEMORY_TYPE                  MemoryType,
  IN  UINTN                            Pages,
  OUT VOID                             **HostAddress,
  IN  UINT64                           Attributes
  )
{
  EFI_STATUS                           Status;
  EFI_PHYSICAL_ADDRESS                 PhysicalAddress;
  PCI_ROOT_BRIDGE_INSTANCE            *RootBridge;

  //
  // Validate Attributes
  //
  if ((Attributes & EFI_PCI_ATTRIBUTE_INVALID_FOR_ALLOCATE_BUFFER) != 0) {
    return EFI_UNSUPPORTED;
  }

  //
  // Check for invalid inputs
  //
  if (HostAddress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // The only valid memory types are EfiBootServicesData and
  // EfiRuntimeServicesData
  //
  if (MemoryType != EfiBootServicesData &&
      MemoryType != EfiRuntimeServicesData) {
    return EFI_INVALID_PARAMETER;
  }

  RootBridge = ROOT_BRIDGE_FROM_THIS (This);

  if (mIoMmu != NULL) {

    if (!RootBridge->DmaAbove4G) {
      //
      // Clear DUAL_ADDRESS_CYCLE
      //
      Attributes &= ~((UINT64) EFI_PCI_ATTRIBUTE_DUAL_ADDRESS_CYCLE);
    }
    Status = mIoMmu->AllocateBuffer (
                       mIoMmu,
                       Type,
                       MemoryType,
                       Pages,
                       HostAddress,
                       Attributes
                       );
    return Status;
  }

  //
  // Limit allocations to memory below 4GB
  //
  PhysicalAddress = SIZE_4GB - 1;

  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  MemoryType,
                  Pages,
                  &PhysicalAddress
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *HostAddress = (VOID *) (UINTN) PhysicalAddress;

  return EFI_SUCCESS;
}

/**
  Frees memory that was allocated with AllocateBuffer().

  The FreeBuffer() function frees memory that was allocated with
  AllocateBuffer().

  @param This        A pointer to the EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL.
  @param Pages       The number of pages to free.
  @param HostAddress The base system memory address of the allocated range.

  @retval EFI_SUCCESS            The requested memory pages were freed.
  @retval EFI_INVALID_PARAMETER  The memory range specified by HostAddress and
                                 Pages was not allocated with AllocateBuffer().
**/
EFI_STATUS
EFIAPI
RootBridgeIoFreeBuffer (
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This,
  IN  UINTN                            Pages,
  OUT VOID                             *HostAddress
  )
{
  EFI_STATUS                Status;

  if (mIoMmu != NULL) {
    Status = mIoMmu->FreeBuffer (
                       mIoMmu,
                       Pages,
                       HostAddress
                       );
    return Status;
  }

  return gBS->FreePages ((EFI_PHYSICAL_ADDRESS) (UINTN) HostAddress, Pages);
}

/**
  Flushes all PCI posted write transactions from a PCI host bridge to system
  memory.

  The Flush() function flushes any PCI posted write transactions from a PCI
  host bridge to system memory. Posted write transactions are generated by PCI
  bus masters when they perform write transactions to target addresses in
  system memory.
  This function does not flush posted write transactions from any PCI bridges.
  A PCI controller specific action must be taken to guarantee that the posted
  write transactions have been flushed from the PCI controller and from all the
  PCI bridges into the PCI host bridge. This is typically done with a PCI read
  transaction from the PCI controller prior to calling Flush().

  @param This        A pointer to the EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL.

  @retval EFI_SUCCESS        The PCI posted write transactions were flushed
                             from the PCI host bridge to system memory.
  @retval EFI_DEVICE_ERROR   The PCI posted write transactions were not flushed
                             from the PCI host bridge due to a hardware error.
**/
EFI_STATUS
EFIAPI
RootBridgeIoFlush (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL           *This
  )
{
  return EFI_SUCCESS;
}

/**
  Gets the attributes that a PCI root bridge supports setting with
  SetAttributes(), and the attributes that a PCI root bridge is currently
  using.

  The GetAttributes() function returns the mask of attributes that this PCI
  root bridge supports and the mask of attributes that the PCI root bridge is
  currently using.

  @param This        A pointer to the EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL.
  @param Supported   A pointer to the mask of attributes that this PCI root
                     bridge supports setting with SetAttributes().
  @param Attributes  A pointer to the mask of attributes that this PCI root
                     bridge is currently using.

  @retval  EFI_SUCCESS           If Supports is not NULL, then the attributes
                                 that the PCI root bridge supports is returned
                                 in Supports. If Attributes is not NULL, then
                                 the attributes that the PCI root bridge is
                                 currently using is returned in Attributes.
  @retval  EFI_INVALID_PARAMETER Both Supports and Attributes are NULL.
**/
EFI_STATUS
EFIAPI
RootBridgeIoGetAttributes (
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This,
  OUT UINT64                           *Supported,
  OUT UINT64                           *Attributes
  )
{
  PCI_ROOT_BRIDGE_INSTANCE *RootBridge;

  if (Attributes == NULL && Supported == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  RootBridge = ROOT_BRIDGE_FROM_THIS (This);
  //
  // Set the return value for Supported and Attributes
  //
  if (Supported != NULL) {
    *Supported  = RootBridge->Supports;
  }

  if (Attributes != NULL) {
    *Attributes = RootBridge->Attributes;
  }

  return EFI_SUCCESS;
}

/**
  Sets attributes for a resource range on a PCI root bridge.

  The SetAttributes() function sets the attributes specified in Attributes for
  the PCI root bridge on the resource range specified by ResourceBase and
  ResourceLength. Since the granularity of setting these attributes may vary
  from resource type to resource type, and from platform to platform, the
  actual resource range and the one passed in by the caller may differ. As a
  result, this function may set the attributes specified by Attributes on a
  larger resource range than the caller requested. The actual range is returned
  in ResourceBase and ResourceLength. The caller is responsible for verifying
  that the actual range for which the attributes were set is acceptable.

  @param This            A pointer to the
                         EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL.
  @param Attributes      The mask of attributes to set. If the
                         attribute bit MEMORY_WRITE_COMBINE,
                         MEMORY_CACHED, or MEMORY_DISABLE is set,
                         then the resource range is specified by
                         ResourceBase and ResourceLength. If
                         MEMORY_WRITE_COMBINE, MEMORY_CACHED, and
                         MEMORY_DISABLE are not set, then
                         ResourceBase and ResourceLength are ignored,
                         and may be NULL.
  @param ResourceBase    A pointer to the base address of the
                         resource range to be modified by the
                         attributes specified by Attributes.
  @param ResourceLength  A pointer to the length of the resource
                                   range to be modified by the attributes
                                   specified by Attributes.

  @retval  EFI_SUCCESS           The current configuration of this PCI root bridge
                                 was returned in Resources.
  @retval  EFI_UNSUPPORTED       The current configuration of this PCI root bridge
                                 could not be retrieved.
**/
EFI_STATUS
EFIAPI
RootBridgeIoSetAttributes (
  IN     EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *This,
  IN     UINT64                           Attributes,
  IN OUT UINT64                           *ResourceBase,
  IN OUT UINT64                           *ResourceLength
  )
{
  PCI_ROOT_BRIDGE_INSTANCE            *RootBridge;

  RootBridge = ROOT_BRIDGE_FROM_THIS (This);

  if ((Attributes & (~RootBridge->Supports)) != 0) {
    return EFI_UNSUPPORTED;
  }

  RootBridge->Attributes = Attributes;
  return EFI_SUCCESS;
}

/**
  Retrieves the current resource settings of this PCI root bridge in the form
  of a set of ACPI resource descriptors.

  There are only two resource descriptor types from the ACPI Specification that
  may be used to describe the current resources allocated to a PCI root bridge.
  These are the QWORD Address Space Descriptor, and the End Tag. The QWORD
  Address Space Descriptor can describe memory, I/O, and bus number ranges for
  dynamic or fixed resources. The configuration of a PCI root bridge is described
  with one or more QWORD Address Space Descriptors followed by an End Tag.

  @param[in]   This        A pointer to the EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL.
  @param[out]  Resources   A pointer to the resource descriptors that
                           describe the current configuration of this PCI root
                           bridge. The storage for the resource
                           descriptors is allocated by this function. The
                           caller must treat the return buffer as read-only
                           data, and the buffer must not be freed by the
                           caller.

  @retval  EFI_SUCCESS     The current configuration of this PCI root bridge
                           was returned in Resources.
  @retval  EFI_UNSUPPORTED The current configuration of this PCI root bridge
                           could not be retrieved.
**/
EFI_STATUS
EFIAPI
RootBridgeIoConfiguration (
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL     *This,
  OUT VOID                                **Resources
  )
{
  EFI_STATUS                        Status;
  UINTN                             Idx;

  PCI_ROOT_BRIDGE_INSTANCE          *RootBridge;
  PCI_RES_NODE                      *ResAllocNode;
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *Config;

  //
  // Get this instance of the Root Bridge.
  //
  RootBridge = ROOT_BRIDGE_FROM_THIS (This);

  //
  // If the pointer is not NULL, it points to a buffer already allocated.
  //
  if (RootBridge->ConfigBuffer == NULL) {
    Status = gBS->AllocatePool (
                    EfiBootServicesData,
                    TypeMax * sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) + sizeof (EFI_ACPI_END_TAG_DESCRIPTOR),
                    &RootBridge->ConfigBuffer
                    );
    if (EFI_ERROR (Status)) {
      return EFI_OUT_OF_RESOURCES;
    }
  }

  Config = RootBridge->ConfigBuffer;

  ZeroMem (Config, TypeMax * sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) + sizeof (EFI_ACPI_END_TAG_DESCRIPTOR));

  for (Idx = 0; Idx < TypeMax; Idx++) {

    ResAllocNode = &RootBridge->ResAllocNode[Idx];

    if (ResAllocNode->Status != ResAllocated) {
      continue;
    }

    switch (ResAllocNode->Type) {

    case TypeIo:
      Config->Desc          = ACPI_ADDRESS_SPACE_DESCRIPTOR;
      Config->Len           = sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) - 3;
      Config->ResType       = ACPI_ADDRESS_SPACE_TYPE_IO;
      Config->AddrRangeMin  = ResAllocNode->Base;
      Config->AddrRangeMax  = ResAllocNode->Base + ResAllocNode->Length - 1;
      Config->AddrLen       = ResAllocNode->Length;
      break;

    case TypeMem32:
      Config->Desc                  = ACPI_ADDRESS_SPACE_DESCRIPTOR;
      Config->Len                   = sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) - 3;
      Config->ResType               = ACPI_ADDRESS_SPACE_TYPE_MEM;
      Config->AddrSpaceGranularity  = 32;
      Config->AddrRangeMin          = ResAllocNode->Base;
      Config->AddrRangeMax          = ResAllocNode->Base + ResAllocNode->Length - 1;
      Config->AddrLen               = ResAllocNode->Length;
      break;

    case TypePMem32:
      Config->Desc                  = ACPI_ADDRESS_SPACE_DESCRIPTOR;
      Config->Len                   = sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) - 3;
      Config->ResType               = ACPI_ADDRESS_SPACE_TYPE_MEM;
      Config->SpecificFlag          = 6;
      Config->AddrSpaceGranularity  = 32;
      Config->AddrRangeMin          = ResAllocNode->Base;
      Config->AddrRangeMax          = ResAllocNode->Base + ResAllocNode->Length - 1;
      Config->AddrLen               = ResAllocNode->Length;
      break;

    case TypeMem64:
      Config->Desc                  = ACPI_ADDRESS_SPACE_DESCRIPTOR;
      Config->Len                   = sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) - 3;
      Config->ResType               = ACPI_ADDRESS_SPACE_TYPE_MEM;
      Config->SpecificFlag          = 6;
      Config->AddrSpaceGranularity  = 64;
      Config->AddrRangeMin          = ResAllocNode->Base;
      Config->AddrRangeMax          = ResAllocNode->Base + ResAllocNode->Length - 1;
      Config->AddrLen               = ResAllocNode->Length;
      break;

    case TypePMem64:
      Config->Desc                  = ACPI_ADDRESS_SPACE_DESCRIPTOR;
      Config->Len                   = sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) - 3;
      Config->ResType               = ACPI_ADDRESS_SPACE_TYPE_MEM;
      Config->SpecificFlag          = 6;
      Config->AddrSpaceGranularity  = 64;
      Config->AddrRangeMin          = ResAllocNode->Base;
      Config->AddrRangeMax          = ResAllocNode->Base + ResAllocNode->Length - 1;
      Config->AddrLen               = ResAllocNode->Length;
      break;

    case TypeBus:
      Config->Desc          = ACPI_ADDRESS_SPACE_DESCRIPTOR;
      Config->Len           = sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) - 3;
      Config->ResType       = ACPI_ADDRESS_SPACE_TYPE_BUS;
      Config->AddrRangeMin  = ResAllocNode->Base;
      Config->AddrRangeMax  = ResAllocNode->Base + ResAllocNode->Length - 1;
      Config->AddrLen       = ResAllocNode->Length;
      break;

    default:
      break;
    }

    Config++;
  }
  //
  // Terminate the entries.
  //
  ((EFI_ACPI_END_TAG_DESCRIPTOR *) Config)->Desc      = ACPI_END_TAG_DESCRIPTOR;
  ((EFI_ACPI_END_TAG_DESCRIPTOR *) Config)->Checksum  = 0x0;

  *Resources = RootBridge->ConfigBuffer;
  return EFI_SUCCESS;
}

