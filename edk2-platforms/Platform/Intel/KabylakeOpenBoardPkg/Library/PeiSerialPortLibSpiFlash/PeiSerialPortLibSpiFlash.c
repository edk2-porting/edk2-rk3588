/** @file
  Serial I/O Port library implementation for output to SPI flash

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
Copyright (c) Microsoft Corporation.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Ppi/Spi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/SerialPortLib.h>
#include <Library/SpiLib.h>

typedef struct {
  UINT32                CurrentWriteOffset;
} SPI_FLASH_DEBUG_CONTEXT;

/**
  Returns a pointer to the PCH SPI PPI.

  @return Pointer to PCH_SPI_PPI    If an instance of the PCH SPI PPI is found
  @return NULL                      If an instance of the PCH SPI PPI is not found

**/
PCH_SPI_PPI *
GetSpiPpi (
  VOID
  )
{
  EFI_STATUS    Status;
  PCH_SPI_PPI   *PchSpiPpi;

  Status =  PeiServicesLocatePpi (
              &gPchSpiPpiGuid,
              0,
              NULL,
              (VOID **) &PchSpiPpi
              );
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  return PchSpiPpi;
}

/**
  Common function to write trace data to a chosen debug interface like
  UART Serial device, USB Serial device or Trace Hub device

  @param  Buffer           Point of data buffer which needs to be written.
  @param  NumberOfBytes    Number of output bytes which are cached in Buffer.

**/
UINTN
EFIAPI
SerialPortWrite (
  IN UINT8     *Buffer,
  IN UINTN     NumberOfBytes
  )
{
  EFI_STATUS                Status;
  EFI_HOB_GUID_TYPE         *GuidHob;
  SPI_FLASH_DEBUG_CONTEXT   *Context;
  PCH_SPI_PPI               *PchSpiPpi;
  UINT32                    BytesWritten;
  UINT32                    SourceBufferOffset;
  UINT32                    NvMessageAreaSize;
  UINT32                    LinearOffset;

  BytesWritten       = NumberOfBytes;
  SourceBufferOffset = 0;

  NvMessageAreaSize = (UINT32) FixedPcdGet32 (PcdFlashNvDebugMessageSize);

  if (NumberOfBytes == 0 || NvMessageAreaSize == 0) {
    return 0;
  }
  GuidHob = GetFirstGuidHob (&gSpiFlashDebugHobGuid);
  if (GuidHob == NULL) {
    return 0;
  }
  Context = GET_GUID_HOB_DATA (GuidHob);
  if (Context == NULL || Context->CurrentWriteOffset >= NvMessageAreaSize) {
    return 0;
  }
  PchSpiPpi = GetSpiPpi ();
  if (PchSpiPpi == NULL) {
    return 0;
  }

  if ((Context->CurrentWriteOffset + NumberOfBytes) / NvMessageAreaSize > 0) {
    LinearOffset = (UINT32) (FixedPcdGet32 (PcdFlashNvDebugMessageBase) - FixedPcdGet32 (PcdFlashAreaBaseAddress));
    Status = PchSpiPpi->FlashErase (
                          PchSpiPpi,
                          FlashRegionBios,
                          LinearOffset,
                          NvMessageAreaSize
                          );
    if (!EFI_ERROR (Status)) {
      Context->CurrentWriteOffset = 0;
    } else {
      return 0;
    }
  }

  if (NumberOfBytes > NvMessageAreaSize) {
    BytesWritten = NvMessageAreaSize;
    SourceBufferOffset = NumberOfBytes - NvMessageAreaSize;
  }

  LinearOffset = (FixedPcdGet32 (PcdFlashNvDebugMessageBase) + Context->CurrentWriteOffset) - FixedPcdGet32 (PcdFlashAreaBaseAddress);

  Status = PchSpiPpi->FlashWrite (
                        PchSpiPpi,
                        FlashRegionBios,
                        LinearOffset,
                        BytesWritten,
                        (UINT8 *) &Buffer[SourceBufferOffset]
                        );
  if (!EFI_ERROR (Status)) {
    Context->CurrentWriteOffset += BytesWritten;
    return BytesWritten;
  }

  return 0;
}

/**
  Common function to Read data from UART serial device, USB serial device and save the datas in buffer.

  @param  Buffer           Point of data buffer which need to be writed.
  @param  NumberOfBytes    Number of output bytes which are cached in Buffer.

  @retval 0                Read data failed, no data is to be read.
  @retval >0               Actual number of bytes read from debug device.

**/
UINTN
EFIAPI
SerialPortRead (
  OUT UINT8     *Buffer,
  IN  UINTN     NumberOfBytes
)
{
  return 0;
}

/**
  Polls a serial device to see if there is any data waiting to be read.

  Polls a serial device to see if there is any data waiting to be read.
  If there is data waiting to be read from the serial device, then TRUE is returned.
  If there is no data waiting to be read from the serial device, then FALSE is returned.

  @retval TRUE             Data is waiting to be read from the serial device.
  @retval FALSE            There is no data waiting to be read from the serial device.

**/
BOOLEAN
EFIAPI
SerialPortPoll (
  VOID
  )
{
  return FALSE;
}

/**
  Sets the control bits on a serial device.

  @param Control                Sets the bits of Control that are settable.

  @retval RETURN_SUCCESS        The new control bits were set on the serial device.
  @retval RETURN_UNSUPPORTED    The serial device does not support this operation.
  @retval RETURN_DEVICE_ERROR   The serial device is not functioning correctly.

**/
RETURN_STATUS
EFIAPI
SerialPortSetControl (
  IN UINT32 Control
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Retrieve the status of the control bits on a serial device.

  @param Control                A pointer to return the current control signals from the serial device.

  @retval RETURN_SUCCESS        The control bits were read from the serial device.
  @retval RETURN_UNSUPPORTED    The serial device does not support this operation.
  @retval RETURN_DEVICE_ERROR   The serial device is not functioning correctly.

**/
RETURN_STATUS
EFIAPI
SerialPortGetControl (
  OUT UINT32 *Control
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Sets the baud rate, receive FIFO depth, transmit/receice time out, parity,
  data bits, and stop bits on a serial device.

  @param BaudRate           The requested baud rate. A BaudRate value of 0 will use the
                            device's default interface speed.
                            On output, the value actually set.
  @param ReveiveFifoDepth   The requested depth of the FIFO on the receive side of the
                            serial interface. A ReceiveFifoDepth value of 0 will use
                            the device's default FIFO depth.
                            On output, the value actually set.
  @param Timeout            The requested time out for a single character in microseconds.
                            This timeout applies to both the transmit and receive side of the
                            interface. A Timeout value of 0 will use the device's default time
                            out value.
                            On output, the value actually set.
  @param Parity             The type of parity to use on this serial device. A Parity value of
                            DefaultParity will use the device's default parity value.
                            On output, the value actually set.
  @param DataBits           The number of data bits to use on the serial device. A DataBits
                            vaule of 0 will use the device's default data bit setting.
                            On output, the value actually set.
  @param StopBits           The number of stop bits to use on this serial device. A StopBits
                            value of DefaultStopBits will use the device's default number of
                            stop bits.
                            On output, the value actually set.

  @retval RETURN_SUCCESS            The new attributes were set on the serial device.
  @retval RETURN_UNSUPPORTED        The serial device does not support this operation.
  @retval RETURN_INVALID_PARAMETER  One or more of the attributes has an unsupported value.
  @retval RETURN_DEVICE_ERROR       The serial device is not functioning correctly.

**/
RETURN_STATUS
EFIAPI
SerialPortSetAttributes (
  IN OUT UINT64             *BaudRate,
  IN OUT UINT32             *ReceiveFifoDepth,
  IN OUT UINT32             *Timeout,
  IN OUT EFI_PARITY_TYPE    *Parity,
  IN OUT UINT8              *DataBits,
  IN OUT EFI_STOP_BITS_TYPE *StopBits
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Initialize the serial device hardware.

  If no initialization is required, then return RETURN_SUCCESS.
  If the serial device was successfully initialized, then return RETURN_SUCCESS.
  If the serial device could not be initialized, then return RETURN_DEVICE_ERROR.

  @retval RETURN_SUCCESS        The serial device was initialized.
  @retval RETURN_DEVICE_ERROR   The serial device could not be initialized.

**/
RETURN_STATUS
EFIAPI
SerialPortInitialize (
  VOID
  )
{
  EFI_STATUS                Status;
  EFI_HOB_GUID_TYPE         *GuidHob;
  SPI_FLASH_DEBUG_CONTEXT   *Context;

  GuidHob = GetFirstGuidHob (&gSpiFlashDebugHobGuid);
  if (GuidHob != NULL) {
    // Initialization only needs to occur once
    return EFI_SUCCESS;
  }

  //
  // Perform silicon specific initialization required to enable write to SPI flash.
  //
  Status = SpiServiceInit ();
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Context = (SPI_FLASH_DEBUG_CONTEXT *) BuildGuidHob (&gSpiFlashDebugHobGuid, sizeof (SPI_FLASH_DEBUG_CONTEXT));
  if (Context == NULL) {
    return EFI_DEVICE_ERROR;
  }

  ZeroMem ((VOID *) Context, sizeof (SPI_FLASH_DEBUG_CONTEXT));

  return EFI_SUCCESS;
}
