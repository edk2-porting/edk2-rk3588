/** @file
  PCI Segment Library for NXP SoCs with multiple RCs

  Copyright 2018-2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <PiDxe.h>
#include <Base.h>
#include <IndustryStandard/Pci22.h>
#include <Library/PciSegmentLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Pcie.h>

typedef enum {
  PciCfgWidthUint8      = 0,
  PciCfgWidthUint16,
  PciCfgWidthUint32,
  PciCfgWidthMax
} PCI_CFG_WIDTH;

/**
  Assert the validity of a PCI Segment address.
  A valid PCI Segment address should not contain 1's in bits 28..31 and 48..63

  @param  A The address to validate.
  @param  M Additional bits to assert to be zero.

**/
#define ASSERT_INVALID_PCI_SEGMENT_ADDRESS(A,M) \
  ASSERT (((A) & (0xffff0000f0000000ULL | (M))) == 0)

STATIC BOOLEAN CfgShiftEnable;
STATIC BOOLEAN PciLsGen4Ctrl;

STATIC
VOID
PciLsGen4SetBusMaster (
  IN EFI_PHYSICAL_ADDRESS   Dbi
  )
{
  PciLsGen4SetPg (Dbi, 0);
  MmioOr32 (Dbi + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_BUS_MASTER);
}

STATIC
VOID
PcieCfgSetTarget (
  IN EFI_PHYSICAL_ADDRESS   Dbi,
  IN UINT32                 Target
  )
{
  STATIC_ASSERT (
    PAB_AXI_AMAP_PEX_WIN_L(0) <= INDIRECT_ADDR_BNDRY,
    "PcieCfgSetTarget() boundary check error");

  PciLsGen4SetPg (Dbi, 0);
  MmioWrite32 (Dbi + PAB_AXI_AMAP_PEX_WIN_L(0), Target);
  MmioWrite32 (Dbi + PAB_AXI_AMAP_PEX_WIN_H(0), 0);
}

/**
  Function to return PCIe Physical Address(PCIe view) or Controller
  Address(CPU view) for NXP Layerscape Gen4 SoC

  @param  Address Address passed from bus layer.
  @param  Segment Segment number for Root Complex.
  @param  Offset  Config space register offset.
  @param  Bus     PCIe Bus number.

  @return Return PCIe CPU or Controller address.

**/
STATIC
UINT64
PciLsGen4GetConfigBase (
  IN  UINT64      Address,
  IN  UINT16      Segment,
  IN  UINT16      Offset,
  IN  UINT8       Bus
  )
{
  UINT32 Target;

  if (Bus > 0) {
    PciLsGen4SetBusMaster (PCI_SEG0_DBI_BASE + PCI_DBI_SIZE_DIFF * Segment);

    Target = (((Address >> 20) & 0xFF) << 24) |
             (((Address >> 15) & 0x1F) << 19) |
             (((Address >> 12) & 0x7) << 16);

    PcieCfgSetTarget ((PCI_SEG0_DBI_BASE + PCI_DBI_SIZE_DIFF * Segment),
      Target);
    return PCI_SEG0_MMIO_MEMBASE + Offset + PCI_BASE_DIFF * Segment;
  } else {
      if (Offset < INDIRECT_ADDR_BNDRY) {
        PciLsGen4SetPg (PCI_SEG0_DBI_BASE + PCI_DBI_SIZE_DIFF * Segment, 0);
        return (PCI_SEG0_DBI_BASE + PCI_DBI_SIZE_DIFF * Segment + Offset);
      }
      PciLsGen4SetPg (PCI_SEG0_DBI_BASE + PCI_DBI_SIZE_DIFF * Segment,
        OFFSET_TO_PAGE_IDX (Offset));
      Offset = OFFSET_TO_PAGE_ADDR (Offset);
      return (PCI_SEG0_DBI_BASE + PCI_DBI_SIZE_DIFF * Segment + Offset);
  }
}

STATIC
UINT64
PciLsCfgTarget (
  IN  EFI_PHYSICAL_ADDRESS Dbi,
  IN  UINT64               Address,
  IN  UINT16               Segment,
  IN  UINT8                Bus,
  IN  UINT16               Offset
  )
{
  UINT32 Target;

  Target = (((Address >> 20) & 0xFF) << 24) |
           (((Address >> 15) & 0x1F) << 19) |
           (((Address >> 12) & 0x7) << 16);

  if (Bus > 1) {
    MmioWrite32 ((UINTN)Dbi + IATU_VIEWPORT_OFF, IATU_VIEWPORT_OUTBOUND | IATU_REGION_INDEX1);
  } else {
    MmioWrite32 ((UINTN)Dbi + IATU_VIEWPORT_OFF, IATU_VIEWPORT_OUTBOUND | IATU_REGION_INDEX0);
  }

  MmioWrite32 ((UINTN)Dbi + IATU_LWR_TARGET_ADDR_OFF_OUTBOUND_0, Target);

  if (Bus > 1) {
    return PCI_SEG0_MMIO_MEMBASE + PCI_BASE_DIFF * Segment + SEG_CFG_SIZE + Offset;
  } else {
    return PCI_SEG0_MMIO_MEMBASE + PCI_BASE_DIFF * Segment + Offset;
  }
}

/**
  Function to return PCIe Physical Address(PCIe view) or Controller
  Address(CPU view) for different RCs

  @param  Address Address passed from bus layer.
  @param  Segment Segment number for Root Complex.
  @param  Offset  Config space register offset.
  @param  Bus     PCIe Bus number.

  @return Return PCIe CPU or Controller address.

**/
STATIC
UINT64
PciLsGetConfigBase (
  IN  UINT64      Address,
  IN  UINT16      Segment,
  IN  UINT16      Offset,
  IN  UINT8       Bus
  )
{
  UINT32 CfgAddr;

  if (CfgShiftEnable) {
    CfgAddr = (UINT32)Address;
    if (Bus > 0) {
      return PCI_SEG0_MMIO_MEMBASE + PCI_BASE_DIFF * Segment + CfgAddr;
    } else {
      return PCI_SEG0_DBI_BASE + PCI_DBI_SIZE_DIFF * Segment + CfgAddr;
    }
  } else {
    CfgAddr = (UINT16)Offset;
    if (Bus > 0) {
      return PciLsCfgTarget (PCI_SEG0_DBI_BASE + PCI_DBI_SIZE_DIFF * Segment,
               Address, Segment, Bus, Offset);
    } else {
      return PCI_SEG0_DBI_BASE + PCI_DBI_SIZE_DIFF * Segment + CfgAddr;
    }
  }
}

/**
  Function to return PCIe Physical Address(PCIe view) or Controller
  Address(CPU view) for different RCs

  @param  Address Address passed from bus layer.
  @param  Segment Segment number for Root Complex.
  @param  Offset  Config space register offset.

  @return Return PCIe CPU or Controller address.

**/
STATIC
UINT64
PciSegmentLibGetConfigBase (
  IN  UINT64      Address,
  IN  UINT16      Segment,
  IN  UINT16      Offset
  )
{
  UINT8  Bus;

  Bus = ((UINT32)Address >> 20) & 0xff;

  if (PciLsGen4Ctrl) {
    return PciLsGen4GetConfigBase (Address, Segment, Offset, Bus);
  } else {
    return PciLsGetConfigBase (Address, Segment, Offset, Bus);
  }
}

/**
  Internal worker function to read a PCI configuration register.

  @param  Address The address that encodes the Segment, PCI Bus, Device,
                  Function and Register.
  @param  Width   The width of data to read

  @return The value read from the PCI configuration register.

**/
STATIC
UINT32
PciSegmentLibReadWorker (
  IN  UINT64                      Address,
  IN  PCI_CFG_WIDTH               Width
  )
{
  UINT64    Base;
  UINT16    Offset;
  UINT16    Segment;

  Segment = (Address >> 32);
  Offset = (Address & 0xfff );

  Base = PciSegmentLibGetConfigBase (Address, Segment, Offset);

  // ignore devices > 0 on bus 0
  if ((Address & 0xff00000) == 0 && (Address & 0xf8000) != 0) {
    return MAX_UINT32;
  }

  // ignore device > 0 on bus 1
  if ((Address & 0xfe00000) == 0 && (Address & 0xf8000) != 0) {
    return MAX_UINT32;
  }

  switch (Width) {
  case PciCfgWidthUint8:
    return MmioRead8 (Base);
  case PciCfgWidthUint16:
    return MmioRead16 (Base);
  case PciCfgWidthUint32:
    return MmioRead32 (Base);
  default:
    ASSERT (FALSE);
  }

  return CHAR_NULL;
}

/**
  Internal worker function to writes a PCI configuration register.

  @param  Address The address that encodes the Segment, PCI Bus, Device,
                  Function and Register.
  @param  Width   The width of data to write
  @param  Data    The value to write.

  @return The value written to the PCI configuration register.

**/
STATIC
UINT32
PciSegmentLibWriteWorker (
  IN  UINT64                      Address,
  IN  PCI_CFG_WIDTH               Width,
  IN  UINT32                      Data
  )
{
  UINT64    Base;
  UINT32    Offset;
  UINT16    Segment;

  Segment = (Address >> 32);
  Offset = (Address & 0xfff );

  Base = PciSegmentLibGetConfigBase (Address, Segment, Offset);

  // ignore devices > 0 on bus 0
  if ((Address & 0xff00000) == 0 && (Address & 0xf8000) != 0) {
    return Data;
  }

  // ignore device > 0 on bus 1
  if ((Address & 0xfe00000) == 0 && (Address & 0xf8000) != 0) {
    return MAX_UINT32;
  }

  switch (Width) {
  case PciCfgWidthUint8:
    MmioWrite8 (Base , Data);
    break;
  case PciCfgWidthUint16:
    MmioWrite16 (Base , Data);
    break;
  case PciCfgWidthUint32:
    MmioWrite32 (Base , Data);
    break;
  default:
    ASSERT (FALSE);
  }

  return Data;
}

/**
  Register a PCI device so PCI configuration registers may be accessed after
  SetVirtualAddressMap().

  If any reserved bits in Address are set, then ASSERT().

  @param  Address                  The address that encodes the PCI Bus, Device,
                                   Function and Register.

  @retval RETURN_SUCCESS           The PCI device was registered for runtime access.
  @retval RETURN_UNSUPPORTED       An attempt was made to call this function
                                   after ExitBootServices().
  @retval RETURN_UNSUPPORTED       The resources required to access the PCI device
                                   at runtime could not be mapped.
  @retval RETURN_OUT_OF_RESOURCES  There are not enough resources available to
                                   complete the registration.

**/
RETURN_STATUS
EFIAPI
PciSegmentRegisterForRuntimeAccess (
  IN UINTN  Address
  )
{
  ASSERT_INVALID_PCI_SEGMENT_ADDRESS (Address, 0);
  return RETURN_UNSUPPORTED;
}

/**
  Reads an 8-bit PCI configuration register.

  Reads and returns the 8-bit PCI configuration register specified by Address.

  If any reserved bits in Address are set, then ASSERT().

  @param  Address   The address that encodes the PCI Segment, Bus, Device, Function,
                    and Register.

  @return The 8-bit PCI configuration register specified by Address.

**/
UINT8
EFIAPI
PciSegmentRead8 (
  IN UINT64                    Address
  )
{
  ASSERT_INVALID_PCI_SEGMENT_ADDRESS (Address, 0);

  return (UINT8) PciSegmentLibReadWorker (Address, PciCfgWidthUint8);
}

/**
  Writes an 8-bit PCI configuration register.

  Writes the 8-bit PCI configuration register specified by Address with the value specified by Value.
  Value is returned.  This function must guarantee that all PCI read and write operations are serialized.

  If any reserved bits in Address are set, then ASSERT().

  @param  Address     The address that encodes the PCI Segment, Bus, Device, Function, and Register.
  @param  Value       The value to write.

  @return The value written to the PCI configuration register.

**/
UINT8
EFIAPI
PciSegmentWrite8 (
  IN UINT64                    Address,
  IN UINT8                     Value
  )
{
  ASSERT_INVALID_PCI_SEGMENT_ADDRESS (Address, 0);

  return (UINT8) PciSegmentLibWriteWorker (Address, PciCfgWidthUint8, Value);
}

/**
  Reads a 16-bit PCI configuration register.

  Reads and returns the 16-bit PCI configuration register specified by Address.

  If any reserved bits in Address are set, then ASSERT().
  If Address is not aligned on a 16-bit boundary, then ASSERT().

  @param  Address   The address that encodes the PCI Segment, Bus, Device, Function, and Register.

  @return The 16-bit PCI configuration register specified by Address.

**/
UINT16
EFIAPI
PciSegmentRead16 (
  IN UINT64                    Address
  )
{
  ASSERT_INVALID_PCI_SEGMENT_ADDRESS (Address, 1);

  return (UINT16) PciSegmentLibReadWorker (Address, PciCfgWidthUint16);
}

/**
  Writes a 16-bit PCI configuration register.

  Writes the 16-bit PCI configuration register specified by Address with the
  value specified by Value.

  Value is returned.

  If any reserved bits in Address are set, then ASSERT().
  If Address is not aligned on a 16-bit boundary, then ASSERT().

  @param  Address     The address that encodes the PCI Segment, Bus, Device, Function, and Register.
  @param  Value       The value to write.

  @return The parameter of Value.

**/
UINT16
EFIAPI
PciSegmentWrite16 (
  IN UINT64                    Address,
  IN UINT16                    Value
  )
{
  ASSERT_INVALID_PCI_SEGMENT_ADDRESS (Address, 1);

  return (UINT16) PciSegmentLibWriteWorker (Address, PciCfgWidthUint16, Value);
}

/**
  Reads a 32-bit PCI configuration register.

  Reads and returns the 32-bit PCI configuration register specified by Address.

  If any reserved bits in Address are set, then ASSERT().
  If Address is not aligned on a 32-bit boundary, then ASSERT().

  @param  Address   The address that encodes the PCI Segment, Bus, Device, Function,
                    and Register.

  @return The 32-bit PCI configuration register specified by Address.

**/
UINT32
EFIAPI
PciSegmentRead32 (
  IN UINT64                    Address
  )
{
  ASSERT_INVALID_PCI_SEGMENT_ADDRESS (Address, 3);

  return PciSegmentLibReadWorker (Address, PciCfgWidthUint32);
}

/**
  Writes a 32-bit PCI configuration register.

  Writes the 32-bit PCI configuration register specified by Address with the
  value specified by Value.

  Value is returned.

  If any reserved bits in Address are set, then ASSERT().
  If Address is not aligned on a 32-bit boundary, then ASSERT().

  @param  Address     The address that encodes the PCI Segment, Bus, Device,
                      Function, and Register.
  @param  Value       The value to write.

  @return The parameter of Value.

**/
UINT32
EFIAPI
PciSegmentWrite32 (
  IN UINT64                    Address,
  IN UINT32                    Value
  )
{
  ASSERT_INVALID_PCI_SEGMENT_ADDRESS (Address, 3);

  return PciSegmentLibWriteWorker (Address, PciCfgWidthUint32, Value);
}

/**
  Reads a range of PCI configuration registers into a caller supplied buffer.

  Reads the range of PCI configuration registers specified by StartAddress and
  Size into the buffer specified by Buffer. This function only allows the PCI
  configuration registers from a single PCI function to be read. Size is
  returned.

  If any reserved bits in StartAddress are set, then ASSERT().
  If ((StartAddress & 0xFFF) + Size) > 0x1000, then ASSERT().
  If Size > 0 and Buffer is NULL, then ASSERT().

  @param  StartAddress  The starting address that encodes the PCI Segment, Bus,
                        Device, Function and Register.
  @param  Size          The size in bytes of the transfer.
  @param  Buffer        The pointer to a buffer receiving the data read.

  @return Size

**/
UINTN
EFIAPI
PciSegmentReadBuffer (
  IN  UINT64                   StartAddress,
  IN  UINTN                    Size,
  OUT VOID                     *Buffer
  )
{
  UINTN                             ReturnValue;

  ASSERT_INVALID_PCI_SEGMENT_ADDRESS (StartAddress, 0);
  ASSERT (((StartAddress & 0xFFF) + Size) <= SIZE_4KB);

  if (Size == 0) {
    return Size;
  }

  ASSERT (Buffer != NULL);

  //
  // Save Size for return
  //
  ReturnValue = Size;

  if ((StartAddress & BIT0) != 0) {
    //
    // Read a byte if StartAddress is byte aligned
    //
    *(volatile UINT8 *)Buffer = PciSegmentRead8 (StartAddress);
    StartAddress += sizeof (UINT8);
    Size -= sizeof (UINT8);
    Buffer = (UINT8*)Buffer + BIT0;
  }

  if (Size >= sizeof (UINT16) && (StartAddress & BIT1) != 0) {
    //
    // Read a word if StartAddress is word aligned
    //
    WriteUnaligned16 (Buffer, PciSegmentRead16 (StartAddress));
    StartAddress += sizeof (UINT16);
    Size -= sizeof (UINT16);
    Buffer = (UINT16*)Buffer + BIT0;
  }

  while (Size >= sizeof (UINT32)) {
    //
    // Read as many double words as possible
    //
    WriteUnaligned32 (Buffer, PciSegmentRead32 (StartAddress));
    StartAddress += sizeof (UINT32);
    Size -= sizeof (UINT32);
    Buffer = (UINT32*)Buffer + BIT0;
  }

  if (Size >= sizeof (UINT16)) {
    //
    // Read the last remaining word if exist
    //
    WriteUnaligned16 (Buffer, PciSegmentRead16 (StartAddress));
    StartAddress += sizeof (UINT16);
    Size -= sizeof (UINT16);
    Buffer = (UINT16*)Buffer + BIT0;
  }

  if (Size >= sizeof (UINT8)) {
    //
    // Read the last remaining byte if exist
    //
    *(volatile UINT8 *)Buffer = PciSegmentRead8 (StartAddress);
  }

  return ReturnValue;
}


/**
  Copies the data in a caller supplied buffer to a specified range of PCI
  configuration space.

  Writes the range of PCI configuration registers specified by StartAddress and
  Size from the buffer specified by Buffer. This function only allows the PCI
  configuration registers from a single PCI function to be written. Size is
  returned.

  If any reserved bits in StartAddress are set, then ASSERT().
  If ((StartAddress & 0xFFF) + Size) > 0x1000, then ASSERT().
  If Size > 0 and Buffer is NULL, then ASSERT().

  @param  StartAddress  The starting address that encodes the PCI Segment, Bus,
                        Device, Function and Register.
  @param  Size          The size in bytes of the transfer.
  @param  Buffer        The pointer to a buffer containing the data to write.

  @return The parameter of Size.

**/
UINTN
EFIAPI
PciSegmentWriteBuffer (
  IN UINT64                    StartAddress,
  IN UINTN                     Size,
  IN VOID                      *Buffer
  )
{
  UINTN                             ReturnValue;

  ASSERT_INVALID_PCI_SEGMENT_ADDRESS (StartAddress, 0);
  ASSERT (((StartAddress & 0xFFF) + Size) <= SIZE_4KB);

  if (Size == 0) {
    return Size;
  }

  ASSERT (Buffer != NULL);

  //
  // Save Size for return
  //
  ReturnValue = Size;

  if ((StartAddress & BIT0) != 0) {
    //
    // Write a byte if StartAddress is byte aligned
    //
    PciSegmentWrite8 (StartAddress, *(UINT8*)Buffer);
    StartAddress += sizeof (UINT8);
    Size -= sizeof (UINT8);
    Buffer = (UINT8*)Buffer + BIT0;
  }

  if (Size >= sizeof (UINT16) && (StartAddress & BIT1) != 0) {
    //
    // Write a word if StartAddress is word aligned
    //
    PciSegmentWrite16 (StartAddress, ReadUnaligned16 (Buffer));
    StartAddress += sizeof (UINT16);
    Size -= sizeof (UINT16);
    Buffer = (UINT16*)Buffer + BIT0;
  }

  while (Size >= sizeof (UINT32)) {
    //
    // Write as many double words as possible
    //
    PciSegmentWrite32 (StartAddress, ReadUnaligned32 (Buffer));
    StartAddress += sizeof (UINT32);
    Size -= sizeof (UINT32);
    Buffer = (UINT32*)Buffer + BIT0;
  }

  if (Size >= sizeof (UINT16)) {
    //
    // Write the last remaining word if exist
    //
    PciSegmentWrite16 (StartAddress, ReadUnaligned16 (Buffer));
    StartAddress += sizeof (UINT16);
    Size -= sizeof (UINT16);
    Buffer = (UINT16*)Buffer + BIT0;
  }

  if (Size >= sizeof (UINT8)) {
    //
    // Write the last remaining byte if exist
    //
    PciSegmentWrite8 (StartAddress, *(UINT8*)Buffer);
  }

  return ReturnValue;
}

EFI_STATUS
PciSegLibInit (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  CfgShiftEnable = CFG_SHIFT_ENABLE;
  PciLsGen4Ctrl = PCI_LS_GEN4_CTRL;
  return EFI_SUCCESS;
}
