/** @file
 *
 *  Copyright (c) 2020, Pete Batard <pete@akeo.ie>
 *  Copyright (c) 2019, ARM Limited. All rights reserved.
 *  Copyright (c) 2017-2020, Andrei Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) 2016, Linaro, Ltd. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <PiDxe.h>

#include <Library/ArmLib.h>
#include <Library/DmaLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include <IndustryStandard/Bcm2836.h>
#include <IndustryStandard/RpiMbox.h>

#include <Protocol/RpiFirmware.h>

//
// The number of statically allocated buffer pages
//
#define NUM_PAGES   1

STATIC VOID  *mDmaBuffer;
STATIC VOID  *mDmaBufferMapping;
STATIC UINTN mDmaBufferBusAddress;

STATIC SPIN_LOCK mMailboxLock;

STATIC
BOOLEAN
DrainMailbox (
  VOID
  )
{
  INTN    Tries;
  UINT32  Val;

  //
  // Get rid of stale response data in the mailbox
  //
  Tries = 0;
  do {
    Val = MmioRead32 (BCM2836_MBOX_BASE_ADDRESS + BCM2836_MBOX_STATUS_OFFSET);
    if (Val & (1U << BCM2836_MBOX_STATUS_EMPTY)) {
      return TRUE;
    }
    ArmDataSynchronizationBarrier ();
    MmioRead32 (BCM2836_MBOX_BASE_ADDRESS + BCM2836_MBOX_READ_OFFSET);
  } while (++Tries < RPI_MBOX_MAX_TRIES);

  return FALSE;
}

STATIC
BOOLEAN
MailboxWaitForStatusCleared (
  IN  UINTN   StatusMask
  )
{
  INTN    Tries;
  UINT32  Val;

  //
  // Get rid of stale response data in the mailbox
  //
  Tries = 0;
  do {
    Val = MmioRead32 (BCM2836_MBOX_BASE_ADDRESS + BCM2836_MBOX_STATUS_OFFSET);
    if ((Val & StatusMask) == 0) {
      return TRUE;
    }
    ArmDataSynchronizationBarrier ();
  } while (++Tries < RPI_MBOX_MAX_TRIES);

  return FALSE;
}

STATIC
EFI_STATUS
MailboxTransaction (
  IN    UINTN   Length,
  IN    UINTN   Channel,
  OUT   UINT32  *Result
  )
{
  if (Channel >= BCM2836_MBOX_NUM_CHANNELS) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get rid of stale response data in the mailbox
  //
  if (!DrainMailbox ()) {
    DEBUG ((DEBUG_ERROR, "%a: timeout waiting for mailbox to drain\n",
      __FUNCTION__));
    return EFI_TIMEOUT;
  }

  //
  // Wait for the 'output register full' bit to become clear
  //
  if (!MailboxWaitForStatusCleared (1U << BCM2836_MBOX_STATUS_FULL)) {
    DEBUG ((DEBUG_ERROR, "%a: timeout waiting for outbox to become empty\n",
      __FUNCTION__));
    return EFI_TIMEOUT;
  }

  ArmDataSynchronizationBarrier ();

  //
  // Start the mailbox transaction
  //
  MmioWrite32 (BCM2836_MBOX_BASE_ADDRESS + BCM2836_MBOX_WRITE_OFFSET,
    (UINT32)((UINTN)mDmaBufferBusAddress | Channel));

  ArmDataSynchronizationBarrier ();

  //
  // Wait for the 'input register empty' bit to clear
  //
  if (!MailboxWaitForStatusCleared (1U << BCM2836_MBOX_STATUS_EMPTY)) {
    DEBUG ((DEBUG_ERROR, "%a: timeout waiting for inbox to become full\n",
      __FUNCTION__));
    return EFI_TIMEOUT;
  }

  //
  // Read back the result
  //
  ArmDataSynchronizationBarrier ();
  *Result = MmioRead32 (BCM2836_MBOX_BASE_ADDRESS + BCM2836_MBOX_READ_OFFSET);
  ArmDataSynchronizationBarrier ();

  return EFI_SUCCESS;
}

#pragma pack(1)
typedef struct {
  UINT32    BufferSize;
  UINT32    Response;
} RPI_FW_BUFFER_HEAD;

typedef struct {
  UINT32    TagId;
  UINT32    TagSize;
  UINT32    TagValueSize;
} RPI_FW_TAG_HEAD;

typedef struct {
  UINT32                    DeviceId;
  UINT32                    PowerState;
} RPI_FW_POWER_STATE_TAG;

typedef struct {
  RPI_FW_BUFFER_HEAD        BufferHead;
  RPI_FW_TAG_HEAD           TagHead;
  RPI_FW_POWER_STATE_TAG    TagBody;
  UINT32                    EndTag;
} RPI_FW_SET_POWER_STATE_CMD;
#pragma pack()

STATIC
EFI_STATUS
EFIAPI
RpiFirmwareSetPowerState (
  IN  UINT32    DeviceId,
  IN  BOOLEAN   PowerState,
  IN  BOOLEAN   Wait
  )
{
  RPI_FW_SET_POWER_STATE_CMD  *Cmd;
  EFI_STATUS                  Status;
  UINT32                      Result;

  if (!AcquireSpinLockOrFail (&mMailboxLock)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to acquire spinlock\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  Cmd = mDmaBuffer;
  ZeroMem (Cmd, sizeof (*Cmd));

  Cmd->BufferHead.BufferSize  = sizeof (*Cmd);
  Cmd->BufferHead.Response    = 0;
  Cmd->TagHead.TagId          = RPI_MBOX_SET_POWER_STATE;
  Cmd->TagHead.TagSize        = sizeof (Cmd->TagBody);
  Cmd->TagHead.TagValueSize   = 0;
  Cmd->TagBody.DeviceId       = DeviceId;
  Cmd->TagBody.PowerState     = (PowerState ? RPI_MBOX_POWER_STATE_ENABLE : 0) |
                                (Wait ? RPI_MBOX_POWER_STATE_WAIT : 0);
  Cmd->EndTag                 = 0;

  Status = MailboxTransaction (Cmd->BufferHead.BufferSize, RPI_MBOX_VC_CHANNEL, &Result);

  ReleaseSpinLock (&mMailboxLock);

  if (EFI_ERROR (Status) ||
      Cmd->BufferHead.Response != RPI_MBOX_RESP_SUCCESS) {
    DEBUG ((DEBUG_ERROR,
      "%a: mailbox transaction error: Status == %r, Response == 0x%x\n",
      __FUNCTION__, Status, Cmd->BufferHead.Response));
    Status = EFI_DEVICE_ERROR;
  }

  if (!EFI_ERROR (Status) &&
      PowerState ^ (Cmd->TagBody.PowerState & RPI_MBOX_POWER_STATE_ENABLE)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to %sable power for device %d\n",
      __FUNCTION__, PowerState ? "en" : "dis", DeviceId));
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

#pragma pack()
typedef struct {
  UINT32                    Base;
  UINT32                    Size;
} RPI_FW_ARM_MEMORY_TAG;

typedef struct {
  RPI_FW_BUFFER_HEAD        BufferHead;
  RPI_FW_TAG_HEAD           TagHead;
  RPI_FW_ARM_MEMORY_TAG     TagBody;
  UINT32                    EndTag;
} RPI_FW_GET_ARM_MEMORY_CMD;
#pragma pack()

STATIC
EFI_STATUS
EFIAPI
RpiFirmwareGetArmMemory (
  OUT   UINT32 *Base,
  OUT   UINT32 *Size
  )
{
  RPI_FW_GET_ARM_MEMORY_CMD   *Cmd;
  EFI_STATUS                  Status;
  UINT32                      Result;

  if (!AcquireSpinLockOrFail (&mMailboxLock)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to acquire spinlock\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  Cmd = mDmaBuffer;
  ZeroMem (Cmd, sizeof (*Cmd));

  Cmd->BufferHead.BufferSize  = sizeof (*Cmd);
  Cmd->BufferHead.Response    = 0;
  Cmd->TagHead.TagId          = RPI_MBOX_GET_ARM_MEMSIZE;
  Cmd->TagHead.TagSize        = sizeof (Cmd->TagBody);
  Cmd->TagHead.TagValueSize   = 0;
  Cmd->EndTag                 = 0;

  Status = MailboxTransaction (Cmd->BufferHead.BufferSize, RPI_MBOX_VC_CHANNEL, &Result);

  ReleaseSpinLock (&mMailboxLock);

  if (EFI_ERROR (Status) ||
      Cmd->BufferHead.Response != RPI_MBOX_RESP_SUCCESS) {
    DEBUG ((DEBUG_ERROR,
      "%a: mailbox transaction error: Status == %r, Response == 0x%x\n",
      __FUNCTION__, Status, Cmd->BufferHead.Response));
    return EFI_DEVICE_ERROR;
  }

  *Base = Cmd->TagBody.Base;
  *Size = Cmd->TagBody.Size;
  return EFI_SUCCESS;
}

#pragma pack()
typedef struct {
  UINT8                     MacAddress[6];
  UINT32                    Padding;
} RPI_FW_MAC_ADDR_TAG;

typedef struct {
  RPI_FW_BUFFER_HEAD        BufferHead;
  RPI_FW_TAG_HEAD           TagHead;
  RPI_FW_MAC_ADDR_TAG       TagBody;
  UINT32                    EndTag;
} RPI_FW_GET_MAC_ADDR_CMD;
#pragma pack()

STATIC
EFI_STATUS
EFIAPI
RpiFirmwareGetMacAddress (
  OUT   UINT8   MacAddress[6]
  )
{
  RPI_FW_GET_MAC_ADDR_CMD     *Cmd;
  EFI_STATUS                  Status;
  UINT32                      Result;

  if (!AcquireSpinLockOrFail (&mMailboxLock)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to acquire spinlock\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  Cmd = mDmaBuffer;
  ZeroMem (Cmd, sizeof (*Cmd));

  Cmd->BufferHead.BufferSize  = sizeof (*Cmd);
  Cmd->BufferHead.Response    = 0;
  Cmd->TagHead.TagId          = RPI_MBOX_GET_MAC_ADDRESS;
  Cmd->TagHead.TagSize        = sizeof (Cmd->TagBody);
  Cmd->TagHead.TagValueSize   = 0;
  Cmd->EndTag                 = 0;

  Status = MailboxTransaction (Cmd->BufferHead.BufferSize, RPI_MBOX_VC_CHANNEL, &Result);

  ReleaseSpinLock (&mMailboxLock);

  if (EFI_ERROR (Status) ||
      Cmd->BufferHead.Response != RPI_MBOX_RESP_SUCCESS) {
    DEBUG ((DEBUG_ERROR,
      "%a: mailbox transaction error: Status == %r, Response == 0x%x\n",
      __FUNCTION__, Status, Cmd->BufferHead.Response));
    return EFI_DEVICE_ERROR;
  }

  CopyMem (MacAddress, Cmd->TagBody.MacAddress, sizeof (Cmd->TagBody.MacAddress));
  return EFI_SUCCESS;
}

#pragma pack()
typedef struct {
  UINT64                    Serial;
} RPI_FW_SERIAL_TAG;

typedef struct {
  RPI_FW_BUFFER_HEAD        BufferHead;
  RPI_FW_TAG_HEAD           TagHead;
  RPI_FW_SERIAL_TAG         TagBody;
  UINT32                    EndTag;
} RPI_FW_GET_SERIAL_CMD;
#pragma pack()

STATIC
EFI_STATUS
EFIAPI
RpiFirmwareGetSerial (
  OUT   UINT64 *Serial
  )
{
  RPI_FW_GET_SERIAL_CMD       *Cmd;
  EFI_STATUS                  Status;
  UINT32                      Result;

  if (!AcquireSpinLockOrFail (&mMailboxLock)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to acquire spinlock\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  Cmd = mDmaBuffer;
  ZeroMem (Cmd, sizeof (*Cmd));

  Cmd->BufferHead.BufferSize  = sizeof (*Cmd);
  Cmd->BufferHead.Response    = 0;
  Cmd->TagHead.TagId          = RPI_MBOX_GET_BOARD_SERIAL;
  Cmd->TagHead.TagSize        = sizeof (Cmd->TagBody);
  Cmd->TagHead.TagValueSize   = 0;
  Cmd->EndTag                 = 0;

  Status = MailboxTransaction (Cmd->BufferHead.BufferSize, RPI_MBOX_VC_CHANNEL, &Result);

  ReleaseSpinLock (&mMailboxLock);

  if (EFI_ERROR (Status) ||
      Cmd->BufferHead.Response != RPI_MBOX_RESP_SUCCESS) {
    DEBUG ((DEBUG_ERROR,
      "%a: mailbox transaction error: Status == %r, Response == 0x%x\n",
      __FUNCTION__, Status, Cmd->BufferHead.Response));
    return EFI_DEVICE_ERROR;
  }

  *Serial = Cmd->TagBody.Serial;
  // Some platforms return 0 or 0x0000000010000000 for serial.
  // For those, try to use the MAC address.
  if ((*Serial == 0) || ((*Serial & 0xFFFFFFFF0FFFFFFFULL) == 0)) {
    Status = RpiFirmwareGetMacAddress ((UINT8*) Serial);
    // Convert to a more user-friendly value
    *Serial = SwapBytes64 (*Serial << 16);
  }

  return Status;
}

#pragma pack()
typedef struct {
  UINT32                    Model;
} RPI_FW_MODEL_TAG;

typedef struct {
  RPI_FW_BUFFER_HEAD        BufferHead;
  RPI_FW_TAG_HEAD           TagHead;
  RPI_FW_MODEL_TAG          TagBody;
  UINT32                    EndTag;
} RPI_FW_GET_MODEL_CMD;
#pragma pack()

STATIC
EFI_STATUS
EFIAPI
RpiFirmwareGetModel (
  OUT   UINT32 *Model
  )
{
  RPI_FW_GET_MODEL_CMD       *Cmd;
  EFI_STATUS                  Status;
  UINT32                      Result;

  if (!AcquireSpinLockOrFail (&mMailboxLock)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to acquire spinlock\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  Cmd = mDmaBuffer;
  ZeroMem (Cmd, sizeof (*Cmd));

  Cmd->BufferHead.BufferSize  = sizeof (*Cmd);
  Cmd->BufferHead.Response    = 0;
  Cmd->TagHead.TagId          = RPI_MBOX_GET_BOARD_MODEL;
  Cmd->TagHead.TagSize        = sizeof (Cmd->TagBody);
  Cmd->TagHead.TagValueSize   = 0;
  Cmd->EndTag                 = 0;

  Status = MailboxTransaction (Cmd->BufferHead.BufferSize, RPI_MBOX_VC_CHANNEL, &Result);

  ReleaseSpinLock (&mMailboxLock);

  if (EFI_ERROR (Status) ||
      Cmd->BufferHead.Response != RPI_MBOX_RESP_SUCCESS) {
    DEBUG ((DEBUG_ERROR,
      "%a: mailbox transaction error: Status == %r, Response == 0x%x\n",
      __FUNCTION__, Status, Cmd->BufferHead.Response));
    return EFI_DEVICE_ERROR;
  }

  *Model = Cmd->TagBody.Model;
  return EFI_SUCCESS;
}

#pragma pack()
typedef struct {
  UINT32                    Revision;
} RPI_FW_MODEL_REVISION_TAG;

typedef struct {
  RPI_FW_BUFFER_HEAD        BufferHead;
  RPI_FW_TAG_HEAD           TagHead;
  RPI_FW_MODEL_REVISION_TAG TagBody;
  UINT32                    EndTag;
} RPI_FW_GET_REVISION_CMD;
#pragma pack()

STATIC
EFI_STATUS
EFIAPI
RpiFirmwareGetModelRevision (
  OUT   UINT32 *Revision
  )
{
  RPI_FW_GET_REVISION_CMD       *Cmd;
  EFI_STATUS                    Status;
  UINT32                        Result;

  if (!AcquireSpinLockOrFail (&mMailboxLock)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to acquire spinlock\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  Cmd = mDmaBuffer;
  ZeroMem (Cmd, sizeof (*Cmd));

  Cmd->BufferHead.BufferSize  = sizeof (*Cmd);
  Cmd->BufferHead.Response    = 0;
  Cmd->TagHead.TagId          = RPI_MBOX_GET_BOARD_REVISION;
  Cmd->TagHead.TagSize        = sizeof (Cmd->TagBody);
  Cmd->TagHead.TagValueSize   = 0;
  Cmd->EndTag                 = 0;

  Status = MailboxTransaction (Cmd->BufferHead.BufferSize, RPI_MBOX_VC_CHANNEL, &Result);

  ReleaseSpinLock (&mMailboxLock);

  if (EFI_ERROR (Status) ||
      Cmd->BufferHead.Response != RPI_MBOX_RESP_SUCCESS) {
    DEBUG ((DEBUG_ERROR,
      "%a: mailbox transaction error: Status == %r, Response == 0x%x\n",
      __FUNCTION__, Status, Cmd->BufferHead.Response));
    return EFI_DEVICE_ERROR;
  }

  *Revision = Cmd->TagBody.Revision;
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
RpiFirmwareGetFirmwareRevision (
  OUT   UINT32 *Revision
  )
{
  RPI_FW_GET_REVISION_CMD       *Cmd;
  EFI_STATUS                    Status;
  UINT32                        Result;

  if (!AcquireSpinLockOrFail (&mMailboxLock)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to acquire spinlock\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  Cmd = mDmaBuffer;
  ZeroMem (Cmd, sizeof (*Cmd));

  Cmd->BufferHead.BufferSize  = sizeof (*Cmd);
  Cmd->BufferHead.Response    = 0;
  Cmd->TagHead.TagId          = RPI_MBOX_GET_REVISION;
  Cmd->TagHead.TagSize        = sizeof (Cmd->TagBody);
  Cmd->TagHead.TagValueSize   = 0;
  Cmd->EndTag                 = 0;

  Status = MailboxTransaction (Cmd->BufferHead.BufferSize, RPI_MBOX_VC_CHANNEL, &Result);

  ReleaseSpinLock (&mMailboxLock);

  if (EFI_ERROR (Status) ||
      Cmd->BufferHead.Response != RPI_MBOX_RESP_SUCCESS) {
    DEBUG ((DEBUG_ERROR,
      "%a: mailbox transaction error: Status == %r, Response == 0x%x\n",
      __FUNCTION__, Status, Cmd->BufferHead.Response));
    return EFI_DEVICE_ERROR;
  }

  *Revision = Cmd->TagBody.Revision;
  return EFI_SUCCESS;
}

STATIC
CHAR8*
EFIAPI
RpiFirmwareGetModelName (
  IN INTN ModelId
  )
{
  UINT32  Revision;

  // If a negative ModelId is passed, detect it.
  if ((ModelId < 0) && (RpiFirmwareGetModelRevision (&Revision) == EFI_SUCCESS)) {
    ModelId = (Revision >> 4) & 0xFF;
  }

  switch (ModelId) {
  // www.raspberrypi.org/documentation/hardware/raspberrypi/revision-codes/README.md
  case 0x00:
    return "Raspberry Pi Model A";
  case 0x01:
    return "Raspberry Pi Model B";
  case 0x02:
    return "Raspberry Pi Model A+";
  case 0x03:
    return "Raspberry Pi Model B+";
  case 0x04:
    return "Raspberry Pi 2 Model B";
  case 0x06:
    return "Raspberry Pi Compute Module 1";
  case 0x08:
    return "Raspberry Pi 3 Model B";
  case 0x09:
    return "Raspberry Pi Zero";
  case 0x0A:
    return "Raspberry Pi Compute Module 3";
  case 0x0C:
    return "Raspberry Pi Zero W";
  case 0x0D:
    return "Raspberry Pi 3 Model B+";
  case 0x0E:
    return "Raspberry Pi 3 Model A+";
  case 0x10:
    return "Raspberry Pi Compute Module 3+";
  case 0x11:
    return "Raspberry Pi 4 Model B";
  case 0x13:
    return "Raspberry Pi 400";
  case 0x14:
    return "Raspberry Pi Compute Module 4";
  default:
    return "Unknown Raspberry Pi Model";
  }
}

STATIC
EFI_STATUS
EFIAPI
RPiFirmwareGetModelInstalledMB (
  OUT   UINT32 *InstalledMB
  )
{
  EFI_STATUS Status;
  UINT32     Revision;

  Status = RpiFirmwareGetModelRevision(&Revision);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Could not get the board revision: Status == %r\n",
      __FUNCTION__, Status));
    return EFI_DEVICE_ERROR;
  }

  //
  // www.raspberrypi.org/documentation/hardware/raspberrypi/revision-codes/README.md
  // Bits [20-22] indicate the amount of memory starting with 256MB (000b)
  // and doubling in size for each value (001b = 512 MB, 010b = 1GB, etc.)
  //
  *InstalledMB = 256 << ((Revision >> 20) & 0x07);
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
RPiFirmwareGetModelFamily (
  OUT   UINT32 *ModelFamily
  )
{
  EFI_STATUS                  Status;
  UINT32                      Revision;
  UINT32                      ModelId;

  Status = RpiFirmwareGetModelRevision(&Revision);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Could not get the board revision: Status == %r\n",
      __FUNCTION__, Status));
    return EFI_DEVICE_ERROR;
  } else {
    ModelId = (Revision >> 4) & 0xFF;
  }

  switch (ModelId) {
  // www.raspberrypi.org/documentation/hardware/raspberrypi/revision-codes/README.md
  case 0x00:          // Raspberry Pi Model A
  case 0x01:          // Raspberry Pi Model B
  case 0x02:          // Raspberry Pi Model A+
  case 0x03:          // Raspberry Pi Model B+
  case 0x06:          // Raspberry Pi Compute Module 1
  case 0x09:          // Raspberry Pi Zero
  case 0x0C:          // Raspberry Pi Zero W
      *ModelFamily = 1;
      break;
  case 0x04:          // Raspberry Pi 2 Model B
      *ModelFamily = 2;
      break;
  case 0x08:          // Raspberry Pi 3 Model B
  case 0x0A:          // Raspberry Pi Compute Module 3
  case 0x0D:          // Raspberry Pi 3 Model B+
  case 0x0E:          // Raspberry Pi 3 Model A+
  case 0x10:          // Raspberry Pi Compute Module 3+
      *ModelFamily = 3;
      break;
  case 0x11:          // Raspberry Pi 4 Model B
  case 0x13:          // Raspberry Pi 400
  case 0x14:          // Raspberry Pi Computer Module 4
      *ModelFamily = 4;
      break;
  default:
      *ModelFamily = 0;
      break;
  }

  if (*ModelFamily == 0) {
    DEBUG ((DEBUG_ERROR,
      "%a: Unknown Raspberry Pi model family : ModelId == 0x%x\n",
      __FUNCTION__, ModelId));
    return EFI_UNSUPPORTED;
    }

  return EFI_SUCCESS;
}

STATIC
CHAR8*
EFIAPI
RpiFirmwareGetManufacturerName (
  IN INTN ManufacturerId
  )
{
  UINT32  Revision;

  // If a negative ModelId is passed, detect it.
  if ((ManufacturerId < 0) && (RpiFirmwareGetModelRevision (&Revision) == EFI_SUCCESS)) {
    ManufacturerId = (Revision >> 16) & 0x0F;
  }

  switch (ManufacturerId) {
  // www.raspberrypi.org/documentation/hardware/raspberrypi/revision-codes/README.md
  case 0x00:
    return "Sony UK";
  case 0x01:
    return "Egoman";
  case 0x02:
  case 0x04:
    return "Embest";
  case 0x03:
    return "Sony Japan";
  case 0x05:
    return "Stadium";
  default:
    return "Unknown Manufacturer";
  }
}

STATIC
CHAR8*
EFIAPI
RpiFirmwareGetCpuName (
  IN INTN CpuId
  )
{
  UINT32  Revision;

  // If a negative CpuId is passed, detect it.
  if ((CpuId < 0) && (RpiFirmwareGetModelRevision (&Revision) == EFI_SUCCESS)) {
    CpuId = (Revision >> 12) & 0x0F;
  }

  switch (CpuId) {
  // www.raspberrypi.org/documentation/hardware/raspberrypi/revision-codes/README.md
  case 0x00:
    return "BCM2835 (ARM11)";
  case 0x01:
    return "BCM2836 (ARM Cortex-A7)";
  case 0x02:
    return "BCM2837 (ARM Cortex-A53)";
  case 0x03:
    return "BCM2711 (ARM Cortex-A72)";
  default:
    return "Unknown CPU Model";
  }
}

#pragma pack()
typedef struct {
  UINT32 Width;
  UINT32 Height;
} RPI_FW_FB_SIZE_TAG;

typedef struct {
  RPI_FW_BUFFER_HEAD        BufferHead;
  RPI_FW_TAG_HEAD           TagHead;
  RPI_FW_FB_SIZE_TAG        TagBody;
  UINT32                    EndTag;
} RPI_FW_GET_FB_SIZE_CMD;

typedef struct {
  UINT32 Depth;
} RPI_FW_FB_DEPTH_TAG;

typedef struct {
  UINT32 Pitch;
} RPI_FW_FB_PITCH_TAG;

typedef struct {
  UINT32 AlignmentBase;
  UINT32 Size;
} RPI_FW_FB_ALLOC_TAG;

typedef struct {
  RPI_FW_BUFFER_HEAD        BufferHead;
  RPI_FW_TAG_HEAD           FreeFbTag;
  UINT32                    EndTag;
} RPI_FW_FREE_FB_CMD;

typedef struct {
  RPI_FW_BUFFER_HEAD        BufferHead;
  RPI_FW_TAG_HEAD           PhysSizeTag;
  RPI_FW_FB_SIZE_TAG        PhysSize;
  RPI_FW_TAG_HEAD           VirtSizeTag;
  RPI_FW_FB_SIZE_TAG        VirtSize;
  RPI_FW_TAG_HEAD           DepthTag;
  RPI_FW_FB_DEPTH_TAG       Depth;
  RPI_FW_TAG_HEAD           AllocFbTag;
  RPI_FW_FB_ALLOC_TAG       AllocFb;
  RPI_FW_TAG_HEAD           PitchTag;
  RPI_FW_FB_PITCH_TAG       Pitch;
  UINT32                    EndTag;
} RPI_FW_INIT_FB_CMD;
#pragma pack()

STATIC
EFI_STATUS
EFIAPI
RpiFirmwareGetFbSize (
  OUT   UINT32 *Width,
  OUT   UINT32 *Height
  )
{
  RPI_FW_GET_FB_SIZE_CMD     *Cmd;
  EFI_STATUS                  Status;
  UINT32                      Result;

  if (!AcquireSpinLockOrFail (&mMailboxLock)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to acquire spinlock\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  Cmd = mDmaBuffer;
  ZeroMem (Cmd, sizeof (*Cmd));

  Cmd->BufferHead.BufferSize  = sizeof (*Cmd);
  Cmd->BufferHead.Response    = 0;
  Cmd->TagHead.TagId          = RPI_MBOX_GET_FB_GEOMETRY;
  Cmd->TagHead.TagSize        = sizeof (Cmd->TagBody);
  Cmd->TagHead.TagValueSize   = 0;
  Cmd->EndTag                 = 0;

  Status = MailboxTransaction (Cmd->BufferHead.BufferSize, RPI_MBOX_VC_CHANNEL, &Result);

  ReleaseSpinLock (&mMailboxLock);

  if (EFI_ERROR (Status) ||
      Cmd->BufferHead.Response != RPI_MBOX_RESP_SUCCESS) {
    DEBUG ((DEBUG_ERROR,
      "%a: mailbox  transaction error: Status == %r, Response == 0x%x\n",
      __FUNCTION__, Status, Cmd->BufferHead.Response));
    return EFI_DEVICE_ERROR;
  }

  *Width = Cmd->TagBody.Width;
  *Height = Cmd->TagBody.Height;
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
RpiFirmwareFreeFb (VOID)
{
  RPI_FW_FREE_FB_CMD *Cmd;
  EFI_STATUS         Status;
  UINT32             Result;

  if (!AcquireSpinLockOrFail (&mMailboxLock)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to acquire spinlock\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  Cmd = mDmaBuffer;
  ZeroMem (Cmd, sizeof (*Cmd));

  Cmd->BufferHead.BufferSize   = sizeof (*Cmd);
  Cmd->BufferHead.Response     = 0;

  Cmd->FreeFbTag.TagId         = RPI_MBOX_FREE_FB;
  Cmd->FreeFbTag.TagSize       = 0;
  Cmd->FreeFbTag.TagValueSize  = 0;
  Cmd->EndTag                  = 0;

  Status = MailboxTransaction (Cmd->BufferHead.BufferSize, RPI_MBOX_VC_CHANNEL, &Result);
  ReleaseSpinLock (&mMailboxLock);

  if (EFI_ERROR (Status) ||
      Cmd->BufferHead.Response != RPI_MBOX_RESP_SUCCESS) {
    DEBUG ((DEBUG_ERROR,
      "%a: mailbox transaction error: Status == %r, Response == 0x%x\n",
      __FUNCTION__, Status, Cmd->BufferHead.Response));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
RpiFirmwareAllocFb (
  IN  UINT32 Width,
  IN  UINT32 Height,
  IN  UINT32 Depth,
  OUT EFI_PHYSICAL_ADDRESS *FbBase,
  OUT UINTN *FbSize,
  OUT UINTN *Pitch
  )
{
  RPI_FW_INIT_FB_CMD *Cmd;
  EFI_STATUS         Status;
  UINT32             Result;

  ASSERT (FbSize != NULL);
  ASSERT (FbBase != NULL);

  if (!AcquireSpinLockOrFail (&mMailboxLock)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to acquire spinlock\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  Cmd = mDmaBuffer;
  ZeroMem (Cmd, sizeof (*Cmd));

  Cmd->BufferHead.BufferSize  = sizeof (*Cmd);
  Cmd->BufferHead.Response    = 0;

  Cmd->PhysSizeTag.TagId      = RPI_MBOX_SET_FB_PGEOM;
  Cmd->PhysSizeTag.TagSize    = sizeof (Cmd->PhysSize);
  Cmd->PhysSize.Width         = Width;
  Cmd->PhysSize.Height        = Height;
  Cmd->VirtSizeTag.TagId      = RPI_MBOX_SET_FB_VGEOM;
  Cmd->VirtSizeTag.TagSize    = sizeof (Cmd->VirtSize);
  Cmd->VirtSize.Width         = Width;
  Cmd->VirtSize.Height        = Height;
  Cmd->DepthTag.TagId         = RPI_MBOX_SET_FB_DEPTH;
  Cmd->DepthTag.TagSize       = sizeof (Cmd->Depth);
  Cmd->Depth.Depth            = Depth;
  Cmd->AllocFbTag.TagId       = RPI_MBOX_ALLOC_FB;
  Cmd->AllocFbTag.TagSize     = sizeof (Cmd->AllocFb);
  Cmd->AllocFb.AlignmentBase  = 32;
  Cmd->PitchTag.TagId         = RPI_MBOX_GET_FB_LINELENGTH;
  Cmd->PitchTag.TagSize       = sizeof (Cmd->Pitch);
  Cmd->EndTag                 = 0;

  Status = MailboxTransaction (Cmd->BufferHead.BufferSize, RPI_MBOX_VC_CHANNEL, &Result);

  ReleaseSpinLock (&mMailboxLock);

  if (EFI_ERROR (Status) ||
      Cmd->BufferHead.Response != RPI_MBOX_RESP_SUCCESS) {
    DEBUG ((DEBUG_ERROR,
      "%a: mailbox transaction error: Status == %r, Response == 0x%x\n",
      __FUNCTION__, Status, Cmd->BufferHead.Response));
    return EFI_DEVICE_ERROR;
  }

  *Pitch = Cmd->Pitch.Pitch;
  *FbBase = Cmd->AllocFb.AlignmentBase - BCM2836_DMA_DEVICE_OFFSET;
  *FbSize = Cmd->AllocFb.Size;
  return EFI_SUCCESS;
}

#pragma pack()
typedef struct {
  RPI_FW_BUFFER_HEAD        BufferHead;
  RPI_FW_TAG_HEAD           TagHead;
  UINT8                     CommandLine[0];
} RPI_FW_GET_COMMAND_LINE_CMD;
#pragma pack()

STATIC
EFI_STATUS
EFIAPI
RpiFirmwareGetCommmandLine (
  IN  UINTN               BufferSize,
  OUT CHAR8               CommandLine[]
  )
{
  RPI_FW_GET_COMMAND_LINE_CMD  *Cmd;
  EFI_STATUS                    Status;
  UINT32                        Result;

  if ((BufferSize % sizeof (UINT32)) != 0) {
    DEBUG ((DEBUG_ERROR, "%a: BufferSize must be a multiple of 4\n",
      __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  if (sizeof (*Cmd) + BufferSize > EFI_PAGES_TO_SIZE (NUM_PAGES)) {
    DEBUG ((DEBUG_ERROR, "%a: BufferSize exceeds size of DMA buffer\n",
      __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  if (!AcquireSpinLockOrFail (&mMailboxLock)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to acquire spinlock\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  Cmd = mDmaBuffer;
  ZeroMem (Cmd, sizeof (*Cmd) + BufferSize + sizeof (UINT32));

  Cmd->BufferHead.BufferSize  = sizeof (*Cmd) + BufferSize + sizeof (UINT32);
  Cmd->BufferHead.Response    = 0;
  Cmd->TagHead.TagId          = RPI_MBOX_GET_COMMAND_LINE;
  Cmd->TagHead.TagSize        = BufferSize;
  Cmd->TagHead.TagValueSize   = 0;

  Status = MailboxTransaction (Cmd->BufferHead.BufferSize, RPI_MBOX_VC_CHANNEL, &Result);

  ReleaseSpinLock (&mMailboxLock);

  if (EFI_ERROR (Status) ||
      Cmd->BufferHead.Response != RPI_MBOX_RESP_SUCCESS) {
    DEBUG ((DEBUG_ERROR,
      "%a: mailbox transaction error: Status == %r, Response == 0x%x\n",
      __FUNCTION__, Status, Cmd->BufferHead.Response));
    return EFI_DEVICE_ERROR;
  }

  Cmd->TagHead.TagValueSize &= ~RPI_MBOX_VALUE_SIZE_RESPONSE_MASK;
  if (Cmd->TagHead.TagValueSize >= BufferSize &&
      Cmd->CommandLine[Cmd->TagHead.TagValueSize - 1] != '\0') {
    DEBUG ((DEBUG_ERROR, "%a: insufficient buffer size\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (CommandLine, Cmd->CommandLine, Cmd->TagHead.TagValueSize);

  if (Cmd->TagHead.TagValueSize == 0 ||
      CommandLine[Cmd->TagHead.TagValueSize - 1] != '\0') {
    //
    // Add a NUL terminator if required.
    //
    CommandLine[Cmd->TagHead.TagValueSize] = '\0';
  }

  return EFI_SUCCESS;
}

#pragma pack()
typedef struct {
  UINT32                    ClockId;
  UINT32                    ClockRate;
  UINT32                    SkipTurbo;
} RPI_FW_SET_CLOCK_RATE_TAG;

typedef struct {
  RPI_FW_BUFFER_HEAD        BufferHead;
  RPI_FW_TAG_HEAD           TagHead;
  RPI_FW_SET_CLOCK_RATE_TAG TagBody;
  UINT32                    EndTag;
} RPI_FW_SET_CLOCK_RATE_CMD;
#pragma pack()

STATIC
EFI_STATUS
EFIAPI
RpiFirmwareSetClockRate (
  IN  UINT32  ClockId,
  IN  UINT32  ClockRate,
  IN  BOOLEAN SkipTurbo
  )
{
  RPI_FW_SET_CLOCK_RATE_CMD   *Cmd;
  EFI_STATUS                  Status;
  UINT32                      Result;

  if (!AcquireSpinLockOrFail (&mMailboxLock)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to acquire spinlock\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  Cmd = mDmaBuffer;
  ZeroMem (Cmd, sizeof (*Cmd));

  Cmd->BufferHead.BufferSize  = sizeof (*Cmd);
  Cmd->BufferHead.Response    = 0;
  Cmd->TagHead.TagId          = RPI_MBOX_SET_CLOCK_RATE;
  Cmd->TagHead.TagSize        = sizeof (Cmd->TagBody);
  Cmd->TagHead.TagValueSize   = 0;
  Cmd->TagBody.ClockId        = ClockId;
  Cmd->TagBody.ClockRate      = ClockRate;
  Cmd->TagBody.SkipTurbo      = SkipTurbo ? 1 : 0;
  Cmd->EndTag                 = 0;

  Status = MailboxTransaction (Cmd->BufferHead.BufferSize, RPI_MBOX_VC_CHANNEL, &Result);

  ReleaseSpinLock (&mMailboxLock);

  if (EFI_ERROR (Status) ||
      Cmd->BufferHead.Response != RPI_MBOX_RESP_SUCCESS) {
    DEBUG ((DEBUG_ERROR,
      "%a: mailbox transaction error: Status == %r, Response == 0x%x\n",
      __FUNCTION__, Status, Cmd->BufferHead.Response));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

#pragma pack()
typedef struct {
  UINT32                    ClockId;
  UINT32                    ClockRate;
} RPI_FW_CLOCK_RATE_TAG;

typedef struct {
  RPI_FW_BUFFER_HEAD        BufferHead;
  RPI_FW_TAG_HEAD           TagHead;
  RPI_FW_CLOCK_RATE_TAG     TagBody;
  UINT32                    EndTag;
} RPI_FW_GET_CLOCK_RATE_CMD;
#pragma pack()

STATIC
EFI_STATUS
RpiFirmwareGetClockRate (
  IN  UINT32 ClockId,
  IN  UINT32 ClockKind,
  OUT UINT32 *ClockRate
  )
{
  RPI_FW_GET_CLOCK_RATE_CMD   *Cmd;
  EFI_STATUS                  Status;
  UINT32                      Result;

  if (!AcquireSpinLockOrFail (&mMailboxLock)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to acquire spinlock\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  Cmd = mDmaBuffer;
  ZeroMem (Cmd, sizeof (*Cmd));

  Cmd->BufferHead.BufferSize  = sizeof (*Cmd);
  Cmd->BufferHead.Response    = 0;
  Cmd->TagHead.TagId          = ClockKind;
  Cmd->TagHead.TagSize        = sizeof (Cmd->TagBody);
  Cmd->TagHead.TagValueSize   = 0;
  Cmd->TagBody.ClockId        = ClockId;
  Cmd->EndTag                 = 0;

  Status = MailboxTransaction (Cmd->BufferHead.BufferSize, RPI_MBOX_VC_CHANNEL, &Result);

  ReleaseSpinLock (&mMailboxLock);

  if (EFI_ERROR (Status) ||
      Cmd->BufferHead.Response != RPI_MBOX_RESP_SUCCESS) {
    DEBUG ((DEBUG_ERROR,
      "%a: mailbox transaction error: Status == %r, Response == 0x%x\n",
      __FUNCTION__, Status, Cmd->BufferHead.Response));
    return EFI_DEVICE_ERROR;
  }

  *ClockRate = Cmd->TagBody.ClockRate;
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
RpiFirmwareGetCurrentClockState (
  IN  UINT32    ClockId,
  OUT UINT32    *ClockState
  )
{
  return RpiFirmwareGetClockRate (ClockId, RPI_MBOX_GET_CLOCK_STATE, ClockState);
}

STATIC
EFI_STATUS
EFIAPI
RpiFirmwareGetCurrentClockRate (
  IN  UINT32    ClockId,
  OUT UINT32    *ClockRate
  )
{
  return RpiFirmwareGetClockRate (ClockId, RPI_MBOX_GET_CLOCK_RATE, ClockRate);
}

STATIC
EFI_STATUS
EFIAPI
RpiFirmwareGetMaxClockRate (
  IN  UINT32    ClockId,
  OUT UINT32    *ClockRate
  )
{
  return RpiFirmwareGetClockRate (ClockId, RPI_MBOX_GET_MAX_CLOCK_RATE, ClockRate);
}

STATIC
EFI_STATUS
EFIAPI
RpiFirmwareGetMinClockRate (
  IN  UINT32    ClockId,
  OUT UINT32    *ClockRate
  )
{
  return RpiFirmwareGetClockRate (ClockId, RPI_MBOX_GET_MIN_CLOCK_RATE, ClockRate);
}

#pragma pack()
typedef struct {
  UINT32                    ClockId;
  UINT32                    ClockState;
} RPI_FW_GET_CLOCK_STATE_TAG;

typedef struct {
  RPI_FW_BUFFER_HEAD         BufferHead;
  RPI_FW_TAG_HEAD            TagHead;
  RPI_FW_GET_CLOCK_STATE_TAG TagBody;
  UINT32                     EndTag;
} RPI_FW_SET_CLOCK_STATE_CMD;
#pragma pack()

STATIC
EFI_STATUS
RpiFirmwareSetClockState (
  IN  UINT32 ClockId,
  IN  UINT32 ClockState
  )
{
  RPI_FW_SET_CLOCK_STATE_CMD  *Cmd;
  EFI_STATUS                  Status;
  UINT32                      Result;

  if (!AcquireSpinLockOrFail (&mMailboxLock)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to acquire spinlock\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  Cmd = mDmaBuffer;
  ZeroMem (Cmd, sizeof (*Cmd));

  Cmd->BufferHead.BufferSize  = sizeof (*Cmd);
  Cmd->BufferHead.Response    = 0;
  Cmd->TagHead.TagId          = RPI_MBOX_SET_CLOCK_STATE;
  Cmd->TagHead.TagSize        = sizeof (Cmd->TagBody);
  Cmd->TagHead.TagValueSize   = 0;
  Cmd->TagBody.ClockId        = ClockId;
  Cmd->TagBody.ClockState     = ClockState;
  Cmd->EndTag                 = 0;

  Status = MailboxTransaction (Cmd->BufferHead.BufferSize, RPI_MBOX_VC_CHANNEL, &Result);

  ReleaseSpinLock (&mMailboxLock);

  if (EFI_ERROR (Status) ||
      Cmd->BufferHead.Response != RPI_MBOX_RESP_SUCCESS) {
    DEBUG ((DEBUG_ERROR,
      "%a: mailbox transaction error: Status == %r, Response == 0x%x\n",
      __FUNCTION__, Status, Cmd->BufferHead.Response));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

#pragma pack()
typedef struct {
  UINT32 Pin;
  UINT32 State;
} RPI_FW_SET_GPIO_TAG;

typedef struct {
  RPI_FW_BUFFER_HEAD        BufferHead;
  RPI_FW_TAG_HEAD           TagHead;
  RPI_FW_SET_GPIO_TAG       TagBody;
  UINT32                    EndTag;
} RPI_FW_SET_GPIO_CMD;
#pragma pack()

STATIC
VOID
EFIAPI
RpiFirmwareSetGpio (
  IN  UINT32  Gpio,
  IN  BOOLEAN State
  )
{
  RPI_FW_SET_GPIO_CMD *Cmd;
  EFI_STATUS          Status;
  UINT32              Result;

  if (!AcquireSpinLockOrFail (&mMailboxLock)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to acquire spinlock\n", __FUNCTION__));
    return;
  }

  Cmd = mDmaBuffer;
  ZeroMem (Cmd, sizeof (*Cmd));

  Cmd->BufferHead.BufferSize  = sizeof (*Cmd);
  Cmd->BufferHead.Response    = 0;
  Cmd->TagHead.TagId          = RPI_MBOX_SET_GPIO;
  Cmd->TagHead.TagSize        = sizeof (Cmd->TagBody);
  /*
   * There's also a 128 pin offset.
   */
  Cmd->TagBody.Pin = 128 + Gpio;
  Cmd->TagBody.State = State;
  Cmd->TagHead.TagValueSize   = 0;
  Cmd->EndTag                 = 0;

  Status = MailboxTransaction (Cmd->BufferHead.BufferSize, RPI_MBOX_VC_CHANNEL, &Result);

  ReleaseSpinLock (&mMailboxLock);

  if (EFI_ERROR (Status) ||
      Cmd->BufferHead.Response != RPI_MBOX_RESP_SUCCESS) {
    DEBUG ((DEBUG_ERROR,
      "%a: mailbox  transaction error: Status == %r, Response == 0x%x\n",
      __FUNCTION__, Status, Cmd->BufferHead.Response));
  }
}

STATIC
VOID
EFIAPI
RpiFirmwareSetLed (
  IN  BOOLEAN On
  )
{
  RpiFirmwareSetGpio (RPI_EXP_GPIO_LED, On);
}

#pragma pack()
typedef struct {
  UINT32                       DeviceAddress;
} RPI_FW_NOTIFY_XHCI_RESET_TAG;

typedef struct {
  RPI_FW_BUFFER_HEAD           BufferHead;
  RPI_FW_TAG_HEAD              TagHead;
  RPI_FW_NOTIFY_XHCI_RESET_TAG TagBody;
  UINT32                       EndTag;
} RPI_FW_NOTIFY_XHCI_RESET_CMD;
#pragma pack()

STATIC
EFI_STATUS
EFIAPI
RpiFirmwareNotifyXhciReset (
  IN UINTN BusNumber,
  IN UINTN DeviceNumber,
  IN UINTN FunctionNumber
  )
{
  RPI_FW_NOTIFY_XHCI_RESET_CMD *Cmd;
  EFI_STATUS                   Status;
  UINT32                       Result;

  if (!AcquireSpinLockOrFail (&mMailboxLock)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to acquire spinlock\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  Cmd = mDmaBuffer;
  ZeroMem (Cmd, sizeof (*Cmd));

  Cmd->BufferHead.BufferSize  = sizeof (*Cmd);
  Cmd->BufferHead.Response    = 0;
  Cmd->TagHead.TagId          = RPI_MBOX_NOTIFY_XHCI_RESET;
  Cmd->TagHead.TagSize        = sizeof (Cmd->TagBody);
  Cmd->TagHead.TagValueSize   = 0;
  Cmd->TagBody.DeviceAddress  = BusNumber << 20 | DeviceNumber << 15 | FunctionNumber << 12;
  Cmd->EndTag                 = 0;

  Status = MailboxTransaction (Cmd->BufferHead.BufferSize, RPI_MBOX_VC_CHANNEL, &Result);

  ReleaseSpinLock (&mMailboxLock);

  if (EFI_ERROR (Status) ||
      Cmd->BufferHead.Response != RPI_MBOX_RESP_SUCCESS) {
    DEBUG ((DEBUG_ERROR,
      "%a: mailbox  transaction error: Status == %r, Response == 0x%x\n",
      __FUNCTION__, Status, Cmd->BufferHead.Response));
  }

  return Status;
}

#pragma pack()
typedef struct {
  UINT32                       Gpio;
  UINT32                       Direction;
  UINT32                       Polarity;
  UINT32                       TermEn;
  UINT32                       TermPullUp;
} RPI_FW_GPIO_GET_CFG_TAG;

typedef struct {
  RPI_FW_BUFFER_HEAD           BufferHead;
  RPI_FW_TAG_HEAD              TagHead;
  RPI_FW_GPIO_GET_CFG_TAG      TagBody;
  UINT32                       EndTag;
} RPI_FW_NOTIFY_GPIO_GET_CFG_CMD;
#pragma pack()


STATIC
EFI_STATUS
EFIAPI
RpiFirmwareNotifyGpioGetCfg (
  IN UINTN  Gpio,
  IN UINT32 *Polarity
  )
{
  RPI_FW_NOTIFY_GPIO_GET_CFG_CMD *Cmd;
  EFI_STATUS                   Status;
  UINT32                       Result;

  if (!AcquireSpinLockOrFail (&mMailboxLock)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to acquire spinlock\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  Cmd = mDmaBuffer;
  ZeroMem (Cmd, sizeof (*Cmd));

  Cmd->BufferHead.BufferSize  = sizeof (*Cmd);
  Cmd->BufferHead.Response    = 0;
  Cmd->TagHead.TagId          = RPI_MBOX_GET_GPIO_CONFIG;
  Cmd->TagHead.TagSize        = sizeof (Cmd->TagBody);
  Cmd->TagBody.Gpio = 128 + Gpio;

  Cmd->TagHead.TagValueSize   = 0;
  Cmd->EndTag                 = 0;

  Status = MailboxTransaction (Cmd->BufferHead.BufferSize, RPI_MBOX_VC_CHANNEL, &Result);

  *Polarity = Cmd->TagBody.Polarity;

  ReleaseSpinLock (&mMailboxLock);

  if (EFI_ERROR (Status) ||
      Cmd->BufferHead.Response != RPI_MBOX_RESP_SUCCESS) {
    DEBUG ((DEBUG_ERROR,
      "%a: mailbox  transaction error: Status == %r, Response == 0x%x\n",
      __FUNCTION__, Status, Cmd->BufferHead.Response));
  }

  return Status;
}


#pragma pack()
typedef struct {
  UINT32                       Gpio;
  UINT32                       Direction;
  UINT32                       Polarity;
  UINT32                       TermEn;
  UINT32                       TermPullUp;
  UINT32                       State;
} RPI_FW_GPIO_SET_CFG_TAG;

typedef struct {
  RPI_FW_BUFFER_HEAD           BufferHead;
  RPI_FW_TAG_HEAD              TagHead;
  RPI_FW_GPIO_SET_CFG_TAG      TagBody;
  UINT32                       EndTag;
} RPI_FW_NOTIFY_GPIO_SET_CFG_CMD;
#pragma pack()


STATIC
EFI_STATUS
EFIAPI
RpiFirmwareNotifyGpioSetCfg (
  IN UINTN Gpio,
  IN UINTN Direction,
  IN UINTN State
  )
{
  RPI_FW_NOTIFY_GPIO_SET_CFG_CMD *Cmd;
  EFI_STATUS                   Status;
  UINT32                       Result;

  Status = RpiFirmwareNotifyGpioGetCfg (Gpio, &Result);
  if (EFI_ERROR (Status)) {
	  DEBUG ((DEBUG_ERROR, "%a: Failed to get GPIO polarity\n", __FUNCTION__));
	  Result = 0; //default polarity
  }


  if (!AcquireSpinLockOrFail (&mMailboxLock)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to acquire spinlock\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  Cmd = mDmaBuffer;
  ZeroMem (Cmd, sizeof (*Cmd));

  Cmd->BufferHead.BufferSize  = sizeof (*Cmd);
  Cmd->BufferHead.Response    = 0;
  Cmd->TagHead.TagId          = RPI_MBOX_SET_GPIO_CONFIG;
  Cmd->TagHead.TagSize        = sizeof (Cmd->TagBody);

  Cmd->TagBody.Gpio = 128 + Gpio;
  Cmd->TagBody.Direction = Direction;
  Cmd->TagBody.Polarity = Result;
  Cmd->TagBody.TermEn = 0;
  Cmd->TagBody.TermPullUp = 0;
  Cmd->TagBody.State = State;

  Cmd->TagHead.TagValueSize   = 0;
  Cmd->EndTag                 = 0;

  Status = MailboxTransaction (Cmd->BufferHead.BufferSize, RPI_MBOX_VC_CHANNEL, &Result);

  ReleaseSpinLock (&mMailboxLock);

  if (EFI_ERROR (Status) ||
      Cmd->BufferHead.Response != RPI_MBOX_RESP_SUCCESS) {
    DEBUG ((DEBUG_ERROR,
      "%a: mailbox  transaction error: Status == %r, Response == 0x%x\n",
      __FUNCTION__, Status, Cmd->BufferHead.Response));
  }

  RpiFirmwareSetGpio (Gpio,!State);


  return Status;
}

STATIC RASPBERRY_PI_FIRMWARE_PROTOCOL mRpiFirmwareProtocol = {
  RpiFirmwareSetPowerState,
  RpiFirmwareGetMacAddress,
  RpiFirmwareGetCommmandLine,
  RpiFirmwareGetCurrentClockRate,
  RpiFirmwareGetMaxClockRate,
  RpiFirmwareGetMinClockRate,
  RpiFirmwareSetClockRate,
  RpiFirmwareAllocFb,
  RpiFirmwareFreeFb,
  RpiFirmwareGetFbSize,
  RpiFirmwareSetLed,
  RpiFirmwareGetSerial,
  RpiFirmwareGetModel,
  RpiFirmwareGetModelRevision,
  RpiFirmwareGetModelName,
  RPiFirmwareGetModelFamily,
  RpiFirmwareGetFirmwareRevision,
  RpiFirmwareGetManufacturerName,
  RpiFirmwareGetCpuName,
  RpiFirmwareGetArmMemory,
  RPiFirmwareGetModelInstalledMB,
  RpiFirmwareNotifyXhciReset,
  RpiFirmwareGetCurrentClockState,
  RpiFirmwareSetClockState,
  RpiFirmwareNotifyGpioSetCfg
};

/**
  Initialize the state information for the CPU Architectural Protocol

  @param  ImageHandle   of the loaded driver
  @param  SystemTable   Pointer to the System Table

  @retval EFI_SUCCESS           Protocol registered
  @retval EFI_OUT_OF_RESOURCES  Cannot allocate protocol data structure
  @retval EFI_DEVICE_ERROR      Hardware problems

**/
EFI_STATUS
RpiFirmwareDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS      Status;
  UINTN           BufferSize;

  //
  // We only need one of these
  //
  ASSERT_PROTOCOL_ALREADY_INSTALLED (NULL, &gRaspberryPiFirmwareProtocolGuid);

  InitializeSpinLock (&mMailboxLock);

  Status = DmaAllocateBuffer (EfiBootServicesData, NUM_PAGES, &mDmaBuffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to allocate DMA buffer (Status == %r)\n", __FUNCTION__));
    return Status;
  }

  BufferSize = EFI_PAGES_TO_SIZE (NUM_PAGES);
  Status = DmaMap (MapOperationBusMasterCommonBuffer, mDmaBuffer, &BufferSize,
             &mDmaBufferBusAddress, &mDmaBufferMapping);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to map DMA buffer (Status == %r)\n", __FUNCTION__));
    goto FreeBuffer;
  }

  //
  // The channel index is encoded in the low bits of the bus address,
  // so make sure these are cleared.
  //
  ASSERT (!(mDmaBufferBusAddress & (BCM2836_MBOX_NUM_CHANNELS - 1)));

  Status = gBS->InstallProtocolInterface (&ImageHandle,
                  &gRaspberryPiFirmwareProtocolGuid, EFI_NATIVE_INTERFACE,
                  &mRpiFirmwareProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: failed to install RPI firmware protocol (Status == %r)\n",
      __FUNCTION__, Status));
    goto UnmapBuffer;
  }

  return EFI_SUCCESS;

UnmapBuffer:
  DmaUnmap (mDmaBufferMapping);
FreeBuffer:
  DmaFreeBuffer (NUM_PAGES, mDmaBuffer);

  return Status;
}
