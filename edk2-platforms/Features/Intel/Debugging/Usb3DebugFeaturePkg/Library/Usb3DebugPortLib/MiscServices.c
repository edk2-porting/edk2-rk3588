/** @file
  Miscellaneous services internal to USB debug port implementation.

  Copyright (c) 2011 - 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "Usb3DebugPortLibInternal.h"

/**
  Verifies if the bit positions specified by a mask are set in a register.

  @param[in, out] Register    UNITN register
  @param[in]      BitMask     32-bit mask

  @return  BOOLEAN  - TRUE  if all bits specified by the mask are enabled.
                    - FALSE even if one of the bits specified by the mask
                            is not enabled.
**/
BOOLEAN
XhcIsBitSet(
  IN OUT  UINTN  Register,
  IN      UINT32 BitMask
  )
{
  if ((MmioRead32 (Register) & (BitMask)) != 0) {
    return TRUE;
  }
  return FALSE;
}

/**
  Sets bits as per the enabled bit positions in the mask.

  @param[in, out] Register    UINTN register
  @param[in]      BitMask     32-bit mask
**/
VOID
XhcSetR32Bit(
  IN OUT  UINTN  Register,
  IN      UINT32 BitMask
  )
{
  UINT32    RegisterValue;

  RegisterValue = MmioRead32 (Register);
  RegisterValue |= (UINT32)(BitMask);
  MmioWrite32 (Register, RegisterValue);
}

/**
  Clears bits as per the enabled bit positions in the mask.

  @param[in, out] Register    UINTN register
  @param[in]      BitMask     32-bit mask
**/
VOID
XhcClrR32Bit(
  IN OUT  UINTN  Register,
  IN      UINT32 BitMask
  )
{
  UINT32    RegisterValue;

  RegisterValue = MmioRead32 (Register);
  RegisterValue &= (UINT32)(~(BitMask));
  MmioWrite32 (Register, RegisterValue);
}

/**
  Initialize the USB debug port hardware.

  If no initialization is required, then return RETURN_SUCCESS.
  If the serial device was successfully initialized, then return RETURN_SUCCESS.
  If the serial device could not be initialized, then return RETURN_DEVICE_ERROR.

  @retval RETURN_SUCCESS        The serial device was initialized.
  @retval RETURN_DEVICE_ERROR   The serial device could not be initialized.
**/
RETURN_STATUS
EFIAPI
Usb3DebugPortInitialize (
  VOID
  )
{
  USB3Initialize ();
  return RETURN_SUCCESS;
}

/**
  Write data from buffer to USB debug port.

  Writes NumberOfBytes data bytes from Buffer to the serial device.
  The number of bytes actually written to the serial device is returned.
  If the return value is less than NumberOfBytes, then the write operation failed.
  If Buffer is NULL, then ASSERT().
  If NumberOfBytes is zero, then return 0.

  @param  Buffer           Pointer to the data buffer to be written.
  @param  NumberOfBytes    Number of bytes to written to the serial device.

  @retval 0                NumberOfBytes is 0.
  @retval >0               The number of bytes written to the serial device.
                           If this value is less than NumberOfBytes, then the read operation failed.
**/
UINTN
EFIAPI
Usb3DebugPortWrite (
  IN UINT8     *Buffer,
  IN UINTN     NumberOfBytes
  )
{
  Usb3DbgOut (Buffer, &NumberOfBytes);
  return NumberOfBytes;
}

/**
  Read data from USB debug port and save the datas in buffer.

  Reads NumberOfBytes data bytes from a serial device into the buffer
  specified by Buffer. The number of bytes actually read is returned.
  If the return value is less than NumberOfBytes, then the rest operation failed.
  If Buffer is NULL, then ASSERT().
  If NumberOfBytes is zero, then return 0.

  @param  Buffer           Pointer to the data buffer to store the data read from the serial device.
  @param  NumberOfBytes    Number of bytes which will be read.

  @retval 0                Read data failed, no data is to be read.
  @retval >0               Actual number of bytes read from serial device.
**/
UINTN
EFIAPI
Usb3DebugPortRead (
  OUT UINT8   *Buffer,
  IN  UINTN   NumberOfBytes
  )
{
  Usb3DbgIn (Buffer, &NumberOfBytes);
  return NumberOfBytes;
}

/**
  Polls a USB debug port to see if there is any data waiting to be read.

  Polls a serial device to see if there is any data waiting to be read.
  If there is data waiting to be read from the serial device, then TRUE is returned.
  If there is no data waiting to be read from the serial device, then FALSE is returned.

  @retval TRUE             Data is waiting to be read from the serial device.
  @retval FALSE            There is no data waiting to be read from the serial device.
**/
BOOLEAN
EFIAPI
Usb3DebugPortPoll (
  VOID
  )
{
  return FALSE;
}

/**
  Write the data to the XHCI debug register.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the runtime register.
  @param  Data         The data to write.

**/
VOID
XhcWriteDebugReg (
  IN USB3_DEBUG_PORT_INSTANCE *Xhc,
  IN UINT32                   Offset,
  IN UINT32                   Data
  )
{
  EFI_PHYSICAL_ADDRESS  DebugCapabilityBase;

  DebugCapabilityBase = Xhc->DebugCapabilityBase;

  MmioWrite32 ((UINTN)(DebugCapabilityBase + Offset), Data);

  return;
}

/**
  Read XHCI debug register.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the debug register.

  @return The register content read

**/
UINT32
XhcReadDebugReg (
  IN  USB3_DEBUG_PORT_INSTANCE *Xhc,
  IN  UINT32                   Offset
  )
{
  UINT32                  Data;
  EFI_PHYSICAL_ADDRESS    DebugCapabilityBase;

  DebugCapabilityBase = Xhc->DebugCapabilityBase;

  Data = MmioRead32 ((UINTN)(DebugCapabilityBase + Offset));

  return Data;
}

/**
  Set one bit of the debug register while keeping other bits.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the debug register.
  @param  Bit          The bit mask of the register to set.

**/
VOID
XhcSetDebugRegBit (
  IN USB3_DEBUG_PORT_INSTANCE *Xhc,
  IN UINT32                   Offset,
  IN UINT32                   Bit
  )
{
  UINT32                  Data;

  Data  = XhcReadDebugReg (Xhc, Offset);
  Data |= Bit;
  XhcWriteDebugReg (Xhc, Offset, Data);
}

/**
  Clear one bit of the debug register while keeping other bits.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the debug register.
  @param  Bit          The bit mask of the register to set.

**/
VOID
XhcClearDebugRegBit (
  IN USB3_DEBUG_PORT_INSTANCE *Xhc,
  IN UINT32                   Offset,
  IN UINT32                   Bit
  )
{
  UINT32                  Data;

  Data  = XhcReadDebugReg (Xhc, Offset);
  Data  &= ~Bit;
  XhcWriteDebugReg (Xhc, Offset, Data);
}

/**
  Write the data to the XHCI MMIO register.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the runtime register.
  @param  Data         The data to write.

**/
VOID
XhcWriteMmioReg (
  IN USB3_DEBUG_PORT_INSTANCE *Xhc,
  IN UINT32                   Offset,
  IN UINT32                   Data
  )
{
  EFI_PHYSICAL_ADDRESS    MmioBase;

  MmioBase = Xhc->XhcMmioBase;
  MmioWrite32 ((UINTN)(MmioBase + Offset), Data);

  return;
}

/**
  Read XHCI MMIO register.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the runtime register.

  @return The register content read

**/
UINT32
XhcReadMmioReg (
  IN  USB3_DEBUG_PORT_INSTANCE *Xhc,
  IN  UINT32                   Offset
  )
{
  UINT32                  Data;
  EFI_PHYSICAL_ADDRESS    MmioBase;

  MmioBase = Xhc->XhcMmioBase;

  Data = MmioRead32 ((UINTN)(MmioBase + Offset));

  return Data;
}

/**
  Set one bit of the operational register while keeping other bits.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the runtime register.
  @param  Bit          The bit mask of the register to set.

**/
VOID
XhcSetMmioRegBit (
  IN USB3_DEBUG_PORT_INSTANCE   *Xhc,
  IN UINT32                     Offset,
  IN UINT32                     Bit
  )
{
  UINT32                  Data;

  Data  = XhcReadMmioReg (Xhc, Offset);
  Data |= Bit;
  XhcWriteMmioReg (Xhc, Offset, Data);
}

/**
  Clear one bit of the operational register while keeping other bits.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the runtime register.
  @param  Bit          The bit mask of the register to set.

**/
VOID
XhcClearMmioRegBit (
  IN USB3_DEBUG_PORT_INSTANCE   *Xhc,
  IN UINT32                     Offset,
  IN UINT32                     Bit
  )
{
  UINT32                  Data;

  Data  = XhcReadMmioReg (Xhc, Offset);
  Data  &= ~Bit;
  XhcWriteMmioReg (Xhc, Offset, Data);
}

/**
  Wait the operation register's bit as specified by Bit
  to be set (or clear).

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the operational register.
  @param  Bit          The bit of the register to wait for.
  @param  WaitToSet    Wait the bit to set or clear.
  @param  Timeout      The time to wait before abort (in millisecond, ms).

  @retval EFI_SUCCESS  The bit successfully changed by host controller.
  @retval EFI_TIMEOUT  The time out occurred.

**/
EFI_STATUS
XhcWaitMmioRegBit (
  IN USB3_DEBUG_PORT_INSTANCE *Xhc,
  IN UINT32                   Offset,
  IN UINT32                   Bit,
  IN BOOLEAN                  WaitToSet,
  IN UINT32                   Timeout
  )
{
  UINT32                  Index;
  UINTN                   Loop;

  Loop   = (Timeout * XHC_1_MILLISECOND / XHC_POLL_DELAY) + 1;

  for (Index = 0; Index < Loop; Index++) {
    if (XHC_REG_BIT_IS_SET (Xhc, Offset, Bit) == WaitToSet) {
      return EFI_SUCCESS;
    }

    MicroSecondDelay (XHC_POLL_DELAY);
  }

  return EFI_TIMEOUT;
}
