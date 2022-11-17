/** @file
 *
 *  Copyright (c) 2017, Andrei Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DmaLib.h>
#include <Library/TimerLib.h>

#include <Protocol/EmbeddedExternalDevice.h>
#include <Protocol/BlockIo.h>
#include <Protocol/DevicePath.h>
#include <Protocol/RpiMmcHost.h>
#include <Protocol/RpiFirmware.h>

#include <IndustryStandard/Bcm2836.h>
#include <IndustryStandard/RpiMbox.h>
#include <IndustryStandard/Bcm2836SdHost.h>

#define SDHOST_BLOCK_BYTE_LENGTH            512

// Driver Timing Parameters
#define CMD_STALL_AFTER_POLL_US             1
#define CMD_MIN_POLL_TOTAL_TIME_US          100000 // 100ms
#define CMD_MAX_POLL_COUNT                  (CMD_MIN_POLL_TOTAL_TIME_US / CMD_STALL_AFTER_POLL_US)
#define CMD_MAX_RETRY_COUNT                 3
#define CMD_STALL_AFTER_RETRY_US            20 // 20us
#define FIFO_MAX_POLL_COUNT                 1000000
#define STALL_TO_STABILIZE_US               10000 // 10ms

#define IDENT_MODE_SD_CLOCK_FREQ_HZ         400000 // 400KHz

// Macros adopted from MmcDxe internal header
#define SDHOST_R0_READY_FOR_DATA            BIT8
#define SDHOST_R0_CURRENTSTATE(Response)    ((Response >> 9) & 0xF)

#define DEBUG_MMCHOST_SD       DEBUG_VERBOSE
#define DEBUG_MMCHOST_SD_INFO  DEBUG_INFO
#define DEBUG_MMCHOST_SD_ERROR DEBUG_ERROR

STATIC RASPBERRY_PI_FIRMWARE_PROTOCOL   *mFwProtocol;

// Per Physical Layer Simplified Specs
#ifndef NDEBUG
STATIC CONST CHAR8* mStrSdState[] = { "idle", "ready", "ident", "stby",
                                      "tran", "data", "rcv", "prg", "dis",
                                      "ina" };
STATIC CONST CHAR8 *mFsmState[] = { "identmode", "datamode", "readdata",
                                    "writedata", "readwait", "readcrc",
                                    "writecrc", "writewait1", "powerdown",
                                    "powerup", "writestart1", "writestart2",
                                    "genpulses", "writewait2", "?",
                                    "startpowdown" };
#endif /* NDEBUG */
STATIC BOOLEAN mCardIsPresent = FALSE;
STATIC CARD_DETECT_STATE mCardDetectState = CardDetectRequired;
STATIC UINT32 mLastGoodCmd = MMC_GET_INDX (MMC_CMD0);

STATIC inline BOOLEAN
IsAppCmd (
  VOID
  )
{
  return mLastGoodCmd == MMC_CMD55;
}

STATIC BOOLEAN
IsBusyCmd (
  IN  UINT32 MmcCmd
  )
{
  if (IsAppCmd ()) {
    return FALSE;
  }

  return MmcCmd == MMC_CMD7 || MmcCmd == MMC_CMD12;
}

STATIC BOOLEAN
IsWriteCmd (
  IN  UINT32 MmcCmd
  )
{
  if (IsAppCmd ()) {
    return FALSE;
  }

  return MmcCmd == MMC_CMD24 || MmcCmd == MMC_CMD25;
}

STATIC BOOLEAN
IsReadCmd (
  IN  UINT32 MmcCmd,
  IN  UINT32 Argument
  )
{
  if (MmcCmd == MMC_CMD8 && !IsAppCmd ()) {
    if (Argument == CMD8_MMC_ARG) {
      DEBUG ((DEBUG_MMCHOST_SD, "Sending MMC CMD8 variant\n"));
      return TRUE;
    } else {
      ASSERT (Argument == CMD8_SD_ARG);
      DEBUG ((DEBUG_MMCHOST_SD, "Sending SD CMD8 variant\n"));
      return FALSE;
    }
  }

  return
    (MmcCmd == MMC_CMD6 && !IsAppCmd ()) ||
    (MmcCmd == MMC_CMD17 && !IsAppCmd ()) ||
    (MmcCmd == MMC_CMD18 && !IsAppCmd ()) ||
    (MmcCmd == MMC_CMD13 && IsAppCmd ()) ||
    (MmcCmd == MMC_ACMD22 && IsAppCmd ()) ||
    (MmcCmd == MMC_ACMD51 && IsAppCmd ());
}

STATIC VOID
SdHostDumpRegisters (
  VOID
  )
{
  DEBUG ((DEBUG_MMCHOST_SD, "SdHost: Registers Dump:\n"));
  DEBUG ((DEBUG_MMCHOST_SD, "  CMD:  0x%8.8X\n", MmioRead32 (SDHOST_CMD)));
  DEBUG ((DEBUG_MMCHOST_SD, "  ARG:  0x%8.8X\n", MmioRead32 (SDHOST_ARG)));
  DEBUG ((DEBUG_MMCHOST_SD, "  TOUT: 0x%8.8X\n", MmioRead32 (SDHOST_TOUT)));
  DEBUG ((DEBUG_MMCHOST_SD, "  CDIV: 0x%8.8X\n", MmioRead32 (SDHOST_CDIV)));
  DEBUG ((DEBUG_MMCHOST_SD, "  RSP0: 0x%8.8X\n", MmioRead32 (SDHOST_RSP0)));
  DEBUG ((DEBUG_MMCHOST_SD, "  RSP1: 0x%8.8X\n", MmioRead32 (SDHOST_RSP1)));
  DEBUG ((DEBUG_MMCHOST_SD, "  RSP2: 0x%8.8X\n", MmioRead32 (SDHOST_RSP2)));
  DEBUG ((DEBUG_MMCHOST_SD, "  RSP3: 0x%8.8X\n", MmioRead32 (SDHOST_RSP3)));
  DEBUG ((DEBUG_MMCHOST_SD, "  HSTS: 0x%8.8X\n", MmioRead32 (SDHOST_HSTS)));
  DEBUG ((DEBUG_MMCHOST_SD, "  VDD:  0x%8.8X\n", MmioRead32 (SDHOST_VDD)));
  DEBUG ((DEBUG_MMCHOST_SD, "  EDM:  0x%8.8X\n", MmioRead32 (SDHOST_EDM)));
  DEBUG ((DEBUG_MMCHOST_SD, "  HCFG: 0x%8.8X\n", MmioRead32 (SDHOST_HCFG)));
  DEBUG ((DEBUG_MMCHOST_SD, "  HBCT: 0x%8.8X\n", MmioRead32 (SDHOST_HBCT)));
  DEBUG ((DEBUG_MMCHOST_SD, "  HBLC: 0x%8.8X\n\n", MmioRead32 (SDHOST_HBLC)));
}

#ifndef NDEBUG
STATIC EFI_STATUS
SdHostGetSdStatus (
  UINT32* SdStatus
  )
{
  ASSERT (SdStatus != NULL);

  // On command completion with R1 or R1b response type
  // the SDCard status will be in RSP0
  UINT32 Rsp0 = MmioRead32 (SDHOST_RSP0);
  if (Rsp0 != 0xFFFFFFFF) {
    *SdStatus = Rsp0;
    return EFI_SUCCESS;
  }

  return EFI_NO_RESPONSE;
}
#endif /* NDEBUG */

STATIC VOID
SdHostDumpSdCardStatus (
  VOID
  )
{
#ifndef NDEBUG
  UINT32 SdCardStatus;
  EFI_STATUS Status = SdHostGetSdStatus (&SdCardStatus);
  if (!EFI_ERROR (Status)) {
    UINT32 CurrState = SDHOST_R0_CURRENTSTATE (SdCardStatus);
    DEBUG ((DEBUG_MMCHOST_SD,
      "SdHost: SdCardStatus 0x%8.8X: ReadyForData?%d, State[%d]: %a\n",
      SdCardStatus,
      ((SdCardStatus & SDHOST_R0_READY_FOR_DATA) ? 1 : 0),
      CurrState,
      ((CurrState < (sizeof (mStrSdState) / sizeof (*mStrSdState))) ?
        mStrSdState[CurrState] : "UNDEF")));
  }
#endif /* NDEBUG */
}

STATIC VOID
SdHostDumpStatus (
  VOID
  )
{
  SdHostDumpRegisters ();

#ifndef NDEBUG
  UINT32 Hsts = MmioRead32 (SDHOST_HSTS);

  if (Hsts & SDHOST_HSTS_ERROR) {
    DEBUG ((DEBUG_MMCHOST_SD_ERROR, "SdHost: Diagnose HSTS: 0x%8.8X\n", Hsts));

    DEBUG ((DEBUG_MMCHOST_SD_ERROR, "SdHost: Last Good CMD = %u\n", MMC_GET_INDX (mLastGoodCmd)));
    if (Hsts & SDHOST_HSTS_FIFO_ERROR)
      DEBUG ((DEBUG_MMCHOST_SD_ERROR, "  - Fifo Error\n"));
    if (Hsts & SDHOST_HSTS_CRC7_ERROR)
      DEBUG ((DEBUG_MMCHOST_SD_ERROR, "  - CRC7 Error\n"));
    if (Hsts & SDHOST_HSTS_CRC16_ERROR)
      DEBUG ((DEBUG_MMCHOST_SD_ERROR, "  - CRC16 Error\n"));
    if (Hsts & SDHOST_HSTS_CMD_TIME_OUT)
      DEBUG ((DEBUG_MMCHOST_SD_ERROR, "  - CMD Timeout (TOUT %x)\n", MmioRead32 (SDHOST_TOUT)));
    if (Hsts & SDHOST_HSTS_REW_TIME_OUT)
      DEBUG ((DEBUG_MMCHOST_SD_ERROR, "  - Read/Erase/Write Transfer Timeout\n"));
  }

  UINT32 Edm = MmioRead32 (SDHOST_EDM);
  DEBUG (((Hsts & SDHOST_HSTS_ERROR) ? DEBUG_MMCHOST_SD_ERROR : DEBUG_MMCHOST_SD,
    "SdHost: Diagnose EDM: 0x%8.8X\n", Edm));
  DEBUG (((Hsts & SDHOST_HSTS_ERROR) ? DEBUG_MMCHOST_SD_ERROR : DEBUG_MMCHOST_SD,
    "  - FSM: 0x%x (%a)\n", (Edm & 0xF), mFsmState[Edm & 0xF]));
  DEBUG (((Hsts & SDHOST_HSTS_ERROR) ? DEBUG_MMCHOST_SD_ERROR : DEBUG_MMCHOST_SD,
    "  - Fifo Count: %d\n", ((Edm >> 4) & 0x1F)));
  DEBUG (((Hsts & SDHOST_HSTS_ERROR) ? DEBUG_MMCHOST_SD_ERROR : DEBUG_MMCHOST_SD,
    "  - Fifo Write Threshold: %d\n",
    ((Edm >> SDHOST_EDM_WRITE_THRESHOLD_SHIFT) & SDHOST_EDM_THRESHOLD_MASK)));
  DEBUG (((Hsts & SDHOST_HSTS_ERROR) ? DEBUG_MMCHOST_SD_ERROR : DEBUG_MMCHOST_SD,
     "  - Fifo Read Threshold: %d\n",
    ((Edm >> SDHOST_EDM_READ_THRESHOLD_SHIFT) & SDHOST_EDM_THRESHOLD_MASK)));
#endif

  SdHostDumpSdCardStatus ();
}

STATIC EFI_STATUS
SdHostSetClockFrequency (
  IN UINTN TargetSdFreqHz
  )
{
  EFI_STATUS Status;
  UINT32 CoreClockFreqHz = 0;

  // First figure out the core clock
  Status = mFwProtocol->GetClockRate (RPI_MBOX_CLOCK_RATE_CORE, &CoreClockFreqHz);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ASSERT (CoreClockFreqHz != 0);

  // fSDCLK = fcore_pclk/(ClockDiv+2)
  UINT32 ClockDiv = (CoreClockFreqHz - (2 * TargetSdFreqHz)) / TargetSdFreqHz;
  UINT32 ActualSdFreqHz = CoreClockFreqHz / (ClockDiv + 2);

  DEBUG ((DEBUG_MMCHOST_SD_INFO,
    "SdHost: CoreClock=%dHz, CDIV=%d, Requested SdClock=%dHz, Actual SdClock=%dHz\n",
    CoreClockFreqHz, ClockDiv, TargetSdFreqHz, ActualSdFreqHz));

  MmioWrite32 (SDHOST_CDIV, ClockDiv);
  // Set timeout after 1 second, i.e ActualSdFreqHz SD clock cycles
  MmioWrite32 (SDHOST_TOUT, ActualSdFreqHz);

  gBS->Stall (STALL_TO_STABILIZE_US);

  return Status;
}

STATIC BOOLEAN
SdIsReadOnly (
  IN EFI_MMC_HOST_PROTOCOL *This
  )
{
  return FALSE;
}

STATIC EFI_STATUS
SdBuildDevicePath (
  IN EFI_MMC_HOST_PROTOCOL       *This,
  IN EFI_DEVICE_PATH_PROTOCOL    **DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL *NewDevicePathNode;
  EFI_GUID DevicePathGuid = EFI_CALLER_ID_GUID;

  DEBUG ((DEBUG_MMCHOST_SD, "SdHost: SdBuildDevicePath()\n"));

  NewDevicePathNode = CreateDeviceNode (HARDWARE_DEVICE_PATH, HW_VENDOR_DP, sizeof (VENDOR_DEVICE_PATH));
  CopyGuid (&((VENDOR_DEVICE_PATH*)NewDevicePathNode)->Guid, &DevicePathGuid);
  *DevicePath = NewDevicePathNode;

  return EFI_SUCCESS;
}

STATIC EFI_STATUS
SdSendCommand (
  IN EFI_MMC_HOST_PROTOCOL    *This,
  IN MMC_CMD                  MmcCmd,
  IN UINT32                   Argument
  )
{
  UINT32 Hsts;

  //
  // Fail fast, CMD5 (CMD_IO_SEND_OP_COND)
  // is only valid for SDIO cards and thus
  // expected to always fail.
  //
  if (MmcCmd == MMC_CMD5) {
    DEBUG ((DEBUG_MMCHOST_SD, "SdHost: SdSendCommand(CMD%d, Argument: %08x) ignored\n",
      MMC_GET_INDX (MmcCmd), Argument));
    return EFI_UNSUPPORTED;
  }

  if (MmioRead32 (SDHOST_CMD) & SDHOST_CMD_NEW_FLAG) {
    DEBUG ((DEBUG_MMCHOST_SD_ERROR,
      "SdHost: SdSendCommand(): Failed to execute CMD%d, a CMD is already being executed.\n",
      MMC_GET_INDX (MmcCmd)));
    SdHostDumpStatus ();
    return EFI_DEVICE_ERROR;
  }

  // Write command argument
  MmioWrite32 (SDHOST_ARG, Argument);

  UINT32 SdCmd = 0;
  {
    // Set response type
    if (MmcCmd & MMC_CMD_WAIT_RESPONSE) {
      if (MmcCmd & MMC_CMD_LONG_RESPONSE) {
        SdCmd |= SDHOST_CMD_RESPONSE_CMD_LONG_RESP;
      }
    } else {
      SdCmd |= SDHOST_CMD_RESPONSE_CMD_NO_RESP;
    }

    if (IsBusyCmd (MmcCmd)) {
      SdCmd |= SDHOST_CMD_BUSY_CMD;
    }

    if (IsReadCmd (MmcCmd, Argument)) {
      SdCmd |= SDHOST_CMD_READ_CMD;
    }

    if (IsWriteCmd (MmcCmd)) {
      SdCmd |= SDHOST_CMD_WRITE_CMD;
    }

    SdCmd |= MMC_GET_INDX (MmcCmd);
  }

  if (IsReadCmd (MmcCmd, Argument) || IsWriteCmd (MmcCmd)) {
    if (IsAppCmd () && MmcCmd == MMC_ACMD22) {
      MmioWrite32 (SDHOST_HBCT, 0x4);
    } else if (IsAppCmd () && MmcCmd == MMC_ACMD51) {
      MmioWrite32 (SDHOST_HBCT, 0x8);
    } else if (!IsAppCmd () && MmcCmd == MMC_CMD6) {
      MmioWrite32 (SDHOST_HBCT, 0x40);
    } else {
      MmioWrite32 (SDHOST_HBCT, SDHOST_BLOCK_BYTE_LENGTH);
    }
  }

  DEBUG ((DEBUG_MMCHOST_SD,
    "SdHost: SdSendCommand(CMD%d, Argument: %08x): BUSY=%d, RESP=%d, WRITE=%d, READ=%d\n",
    MMC_GET_INDX (MmcCmd), Argument, ((SdCmd & SDHOST_CMD_BUSY_CMD) ? 1 : 0),
    ((SdCmd & (SDHOST_CMD_RESPONSE_CMD_LONG_RESP | SDHOST_CMD_RESPONSE_CMD_NO_RESP)) >> 9),
    ((SdCmd & SDHOST_CMD_WRITE_CMD) ? 1 : 0), ((SdCmd & SDHOST_CMD_READ_CMD) ? 1 : 0)));

  UINT32 PollCount = 0;
  UINT32 RetryCount = 0;
  BOOLEAN IsCmdExecuted = FALSE;
  EFI_STATUS Status = EFI_SUCCESS;

  // Keep retrying the command until it succeeds.
  while ((RetryCount < CMD_MAX_RETRY_COUNT) && !IsCmdExecuted) {
    Status = EFI_SUCCESS;

    // Clear prev cmd status
    MmioWrite32 (SDHOST_HSTS, SDHOST_HSTS_CLEAR);

    if (IsReadCmd (MmcCmd, Argument) || IsWriteCmd (MmcCmd)) {
      // Flush Fifo if this cmd will start a new transfer in case
      // there is stale bytes in the Fifo
      MmioOr32 (SDHOST_EDM, SDHOST_EDM_FIFO_CLEAR);
    }

    if (MmioRead32 (SDHOST_CMD) & SDHOST_CMD_NEW_FLAG) {
      DEBUG ((DEBUG_MMCHOST_SD_ERROR,
        "%a(%u): CMD%d is still being executed after %d trial(s)\n",
        __FUNCTION__, __LINE__, MMC_GET_INDX (MmcCmd), RetryCount));
    }

    // Write command and set it to start execution
    MmioWrite32 (SDHOST_CMD, SDHOST_CMD_NEW_FLAG | SdCmd);

    // Poll for the command status until it finishes execution
    while (PollCount < CMD_MAX_POLL_COUNT) {
      UINT32 CmdReg = MmioRead32 (SDHOST_CMD);

      // Read status of command response
      if (CmdReg & SDHOST_CMD_FAIL_FLAG) {
        Status = EFI_DEVICE_ERROR;
        /*
         * Must fall-through and wait for the command completion!
         */
      }

      // Check if command is completed.
      if (!(CmdReg & SDHOST_CMD_NEW_FLAG)) {
        IsCmdExecuted = TRUE;
        break;
      }

      ++PollCount;
      gBS->Stall (CMD_STALL_AFTER_POLL_US);
    }

    if (!IsCmdExecuted) {
      ++RetryCount;
      gBS->Stall (CMD_STALL_AFTER_RETRY_US);
    }
  }

  if (RetryCount == CMD_MAX_RETRY_COUNT) {
    Status = EFI_TIMEOUT;
  }

  Hsts = MmioRead32 (SDHOST_HSTS);
  if (EFI_ERROR (Status) ||
    (Hsts & SDHOST_HSTS_ERROR) != 0) {
    if (MmcCmd == MMC_CMD1 && (Hsts & SDHOST_HSTS_CRC7_ERROR) != 0) {
      /*
       * SdHost seems to have no way to specify
       * R3 as a transfer type.
       */
      IsCmdExecuted = TRUE;
      Status = EFI_SUCCESS;
      MmioWrite32 (SDHOST_HSTS, SDHOST_HSTS_CLEAR);
    } else if (MmcCmd == MMC_CMD7 && Argument == 0) {
      /*
       * Deselecting the SDCard with CMD7 and RCA=0x0
       * always timeout on SDHost.
       */
      Status = EFI_SUCCESS;
    } else {
      DEBUG ((DEBUG_MMCHOST_SD_ERROR, "%a(%u): CMD%d execution failed after %d trial(s)\n",
        __FUNCTION__, __LINE__, MMC_GET_INDX (MmcCmd), RetryCount));
      SdHostDumpStatus ();
    }

    MmioWrite32 (SDHOST_HSTS, SDHOST_HSTS_CLEAR);
  }

  if (IsCmdExecuted && !EFI_ERROR (Status)) {
    ASSERT (!(MmioRead32 (SDHOST_HSTS) & SDHOST_HSTS_ERROR));
    mLastGoodCmd = MmcCmd;
  }

  return Status;
}

STATIC EFI_STATUS
SdReceiveResponse (
  IN EFI_MMC_HOST_PROTOCOL    *This,
  IN MMC_RESPONSE_TYPE        Type,
  IN UINT32*                  Buffer
  )
{
  if (Buffer == NULL) {
    DEBUG ((DEBUG_MMCHOST_SD_ERROR, "SdHost: SdReceiveResponse(): Input Buffer is NULL\n"));
    return EFI_INVALID_PARAMETER;
  }

  if ((Type == MMC_RESPONSE_TYPE_R1) ||
    (Type == MMC_RESPONSE_TYPE_R1b) ||
    (Type == MMC_RESPONSE_TYPE_R3) ||
    (Type == MMC_RESPONSE_TYPE_R6) ||
    (Type == MMC_RESPONSE_TYPE_R7)) {
    Buffer[0] = MmioRead32 (SDHOST_RSP0);
    DEBUG ((DEBUG_MMCHOST_SD, "SdHost: SdReceiveResponse(Type: %x), Buffer[0]: %08x\n",
      Type, Buffer[0]));

  } else if (Type == MMC_RESPONSE_TYPE_R2) {
    Buffer[0] = MmioRead32 (SDHOST_RSP0);
    Buffer[1] = MmioRead32 (SDHOST_RSP1);
    Buffer[2] = MmioRead32 (SDHOST_RSP2);
    Buffer[3] = MmioRead32 (SDHOST_RSP3);

    DEBUG ((DEBUG_MMCHOST_SD,
      "SdHost: SdReceiveResponse(Type: %x), Buffer[0-3]: %08x, %08x, %08x, %08x\n",
      Type, Buffer[0], Buffer[1], Buffer[2], Buffer[3]));
  }

  return EFI_SUCCESS;
}

STATIC EFI_STATUS
SdReadBlockData (
  IN EFI_MMC_HOST_PROTOCOL    *This,
  IN EFI_LBA                  Lba,
  IN UINTN                    Length,
  IN UINT32*                  Buffer
  )
{
  DEBUG ((DEBUG_MMCHOST_SD, "SdHost: SdReadBlockData(LBA: 0x%x, Length: 0x%x, Buffer: 0x%x)\n",
    (UINT32)Lba, Length, Buffer));

  ASSERT (Buffer != NULL);
  ASSERT (Length % 4 == 0);

  EFI_STATUS Status = EFI_SUCCESS;

  mFwProtocol->SetLed (TRUE);
  {
    UINT32 NumWords = Length / 4;
    UINT32 WordIdx;

    for (WordIdx = 0; WordIdx < NumWords; ++WordIdx) {
      UINT32 PollCount = 0;
      while (PollCount < FIFO_MAX_POLL_COUNT) {
        UINT32 Hsts = MmioRead32 (SDHOST_HSTS);
        if ((Hsts & SDHOST_HSTS_DATA_FLAG) != 0) {
          MmioWrite32 (SDHOST_HSTS, SDHOST_HSTS_DATA_FLAG);
          Buffer[WordIdx] = MmioRead32 (SDHOST_DATA);
          break;
        }

        ++PollCount;
        gBS->Stall (CMD_STALL_AFTER_RETRY_US);
      }

      if (PollCount == FIFO_MAX_POLL_COUNT) {
        DEBUG ((DEBUG_MMCHOST_SD_ERROR,
            "SdHost: SdReadBlockData(): Block Word%d read poll timed-out\n", WordIdx));
        SdHostDumpStatus ();
        MmioWrite32 (SDHOST_HSTS, SDHOST_HSTS_CLEAR);
        Status = EFI_TIMEOUT;
        break;
      }
    }
  }
  mFwProtocol->SetLed (FALSE);

  return Status;
}

STATIC EFI_STATUS
SdWriteBlockData (
  IN EFI_MMC_HOST_PROTOCOL    *This,
  IN EFI_LBA                  Lba,
  IN UINTN                    Length,
  IN UINT32*                  Buffer
  )
{
  DEBUG ((DEBUG_MMCHOST_SD,
    "SdHost: SdWriteBlockData(LBA: 0x%x, Length: 0x%x, Buffer: 0x%x)\n",
    (UINT32)Lba, Length, Buffer));

  ASSERT (Buffer != NULL);
  ASSERT (Length % SDHOST_BLOCK_BYTE_LENGTH == 0);

  EFI_STATUS Status = EFI_SUCCESS;

  mFwProtocol->SetLed (TRUE);
  {
    UINT32 NumWords = Length / 4;
    UINT32 WordIdx;

    for (WordIdx = 0; WordIdx < NumWords; ++WordIdx) {
      UINT32 PollCount = 0;
      while (PollCount < FIFO_MAX_POLL_COUNT) {
        if (MmioRead32 (SDHOST_HSTS) & SDHOST_HSTS_DATA_FLAG) {
          MmioWrite32 (SDHOST_HSTS, SDHOST_HSTS_DATA_FLAG);
          MmioWrite32 (SDHOST_DATA, Buffer[WordIdx]);
          break;
        }

        ++PollCount;
        gBS->Stall (CMD_STALL_AFTER_RETRY_US);
      }

      if (PollCount == FIFO_MAX_POLL_COUNT) {
        DEBUG ((DEBUG_MMCHOST_SD_ERROR,
          "SdHost: SdWriteBlockData(): Block Word%d write poll timed-out\n", WordIdx));
        SdHostDumpStatus ();
        MmioWrite32 (SDHOST_HSTS, SDHOST_HSTS_CLEAR);
        Status = EFI_TIMEOUT;
        break;
      }
    }
  }
  mFwProtocol->SetLed (FALSE);

  return Status;
}

STATIC EFI_STATUS
SdSetIos (
  IN EFI_MMC_HOST_PROTOCOL      *This,
  IN  UINT32                    BusClockFreq,
  IN  UINT32                    BusWidth,
  IN  UINT32                    TimingMode
  )
{
  if (BusWidth != 0) {
    UINT32 Hcfg = MmioRead32 (SDHOST_HCFG);

    DEBUG ((DEBUG_MMCHOST_SD_INFO, "Setting BusWidth %u\n", BusWidth));
    if (BusWidth == 4) {
      Hcfg |= SDHOST_HCFG_WIDE_EXT_BUS;
    } else {
      Hcfg &= ~SDHOST_HCFG_WIDE_EXT_BUS;
    }

    Hcfg |= SDHOST_HCFG_WIDE_INT_BUS | SDHOST_HCFG_SLOW_CARD;
    MmioWrite32 (SDHOST_HCFG, Hcfg);
  }

  if (BusClockFreq != 0) {
    DEBUG ((DEBUG_MMCHOST_SD_INFO, "Setting Freq %u Hz\n", BusClockFreq));
    SdHostSetClockFrequency (BusClockFreq);
  }

  return EFI_SUCCESS;
}

STATIC EFI_STATUS
SdNotifyState (
  IN EFI_MMC_HOST_PROTOCOL    *This,
  IN MMC_STATE                State
  )
{
  DEBUG ((DEBUG_MMCHOST_SD, "SdHost: SdNotifyState(State: %d) ", State));

  // Stall all operations except init until card detection has occurred.
  if (State != MmcHwInitializationState && mCardDetectState != CardDetectCompleted) {
    return EFI_NOT_READY;
  }

  switch (State) {
  case MmcHwInitializationState:
    DEBUG ((DEBUG_MMCHOST_SD, "MmcHwInitializationState\n", State));

    // Turn-off SD Card power
    MmioWrite32 (SDHOST_VDD, 0);

    // Reset command and arg
    MmioWrite32 (SDHOST_CMD, 0);
    MmioWrite32 (SDHOST_ARG, 0);
    // Reset clock divider
    MmioWrite32 (SDHOST_CDIV, 0);
    // Default timeout
    MmioWrite32 (SDHOST_TOUT, 0xffffffff);
    // Clear status flags
    MmioWrite32 (SDHOST_HSTS, SDHOST_HSTS_CLEAR);;
    // Reset controller configs
    MmioWrite32 (SDHOST_HCFG, 0);
    MmioWrite32 (SDHOST_HBCT, 0);
    MmioWrite32 (SDHOST_HBLC, 0);

    gBS->Stall (STALL_TO_STABILIZE_US);

    // Turn-on SD Card power
    MmioWrite32 (SDHOST_VDD, 1);

    gBS->Stall (STALL_TO_STABILIZE_US);

    // Write controller configs
    UINT32 Hcfg = 0;
    Hcfg |= SDHOST_HCFG_WIDE_INT_BUS;
    Hcfg |= SDHOST_HCFG_SLOW_CARD; // Use all bits of CDIV in DataMode
    MmioWrite32 (SDHOST_HCFG, Hcfg);

    // Set default clock frequency
    EFI_STATUS Status = SdHostSetClockFrequency (IDENT_MODE_SD_CLOCK_FREQ_HZ);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_MMCHOST_SD_ERROR,
        "SdHost: SdNotifyState(): Fail to initialize SD clock to %dHz\n",
        IDENT_MODE_SD_CLOCK_FREQ_HZ));
      SdHostDumpStatus ();
      return Status;
    }
    break;
  case MmcIdleState:
    DEBUG ((DEBUG_MMCHOST_SD, "MmcIdleState\n", State));
    break;
  case MmcReadyState:
    DEBUG ((DEBUG_MMCHOST_SD, "MmcReadyState\n", State));
    break;
  case MmcIdentificationState:
    DEBUG ((DEBUG_MMCHOST_SD, "MmcIdentificationState\n", State));
    break;
  case MmcStandByState:
    DEBUG ((DEBUG_MMCHOST_SD, "MmcStandByState\n", State));
    break;
  case MmcTransferState:
    DEBUG ((DEBUG_MMCHOST_SD, "MmcTransferState\n", State));
    break;
    break;
  case MmcSendingDataState:
    DEBUG ((DEBUG_MMCHOST_SD, "MmcSendingDataState\n", State));
    break;
  case MmcReceiveDataState:
    DEBUG ((DEBUG_MMCHOST_SD, "MmcReceiveDataState\n", State));
    break;
  case MmcProgrammingState:
    DEBUG ((DEBUG_MMCHOST_SD, "MmcProgrammingState\n", State));
    break;
  case MmcDisconnectState:
  case MmcInvalidState:
  default:
    DEBUG ((DEBUG_MMCHOST_SD_ERROR, "SdHost: SdNotifyState(): Invalid State: %d\n", State));
    ASSERT (0);
  }

  return EFI_SUCCESS;
}

STATIC BOOLEAN
SdIsCardPresent (
  IN EFI_MMC_HOST_PROTOCOL *This
  )
{
  EFI_STATUS Status;

  //
  // If we are already in progress (we may get concurrent calls)
  // or completed the detection, just return the current value.
  //
  if (mCardDetectState != CardDetectRequired) {
    return mCardIsPresent;
  }

  mCardDetectState = CardDetectInProgress;
  mCardIsPresent = FALSE;

  //
  // The two following commands should succeed even if no card is present.
  //
  Status = SdNotifyState (This, MmcHwInitializationState);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SdIsCardPresent: Error MmcHwInitializationState, Status=%r.\n", Status));
    // If we failed init, go back to requiring card detection
    mCardDetectState = CardDetectRequired;
    return FALSE;
  }

  Status = SdSendCommand (This, MMC_CMD0, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SdIsCardPresent: CMD0 Error, Status=%r.\n", Status));
    goto out;
  }

  //
  // CMD8 should tell us if an SD card is present.
  //
  Status = SdSendCommand (This, MMC_CMD8, CMD8_SD_ARG);
  if (!EFI_ERROR (Status)) {
     DEBUG ((DEBUG_INFO, "SdIsCardPresent: Maybe SD card detected.\n"));
     mCardIsPresent = TRUE;
     goto out;
  }

  //
  // MMC/eMMC won't accept CMD8, but we can try CMD1.
  //
  Status = SdSendCommand (This, MMC_CMD1, EMMC_CMD1_CAPACITY_GREATER_THAN_2GB);
  if (!EFI_ERROR (Status)) {
     DEBUG ((DEBUG_INFO, "SdIsCardPresent: Maybe MMC card detected.\n"));
     mCardIsPresent = TRUE;
     goto out;
  }

  DEBUG ((DEBUG_INFO, "SdIsCardPresent: Not detected, Status=%r.\n", Status));

out:
  mCardDetectState = CardDetectCompleted;
  return mCardIsPresent;
}

BOOLEAN
SdIsMultiBlock (
  IN EFI_MMC_HOST_PROTOCOL *This
  )
{
  return TRUE;
}

EFI_MMC_HOST_PROTOCOL gMmcHost =
  {
    MMC_HOST_PROTOCOL_REVISION,
    SdIsCardPresent,
    SdIsReadOnly,
    SdBuildDevicePath,
    SdNotifyState,
    SdSendCommand,
    SdReceiveResponse,
    SdReadBlockData,
    SdWriteBlockData,
    SdSetIos,
    SdIsMultiBlock
  };

EFI_STATUS
SdHostInitialize (
  IN EFI_HANDLE          ImageHandle,
  IN EFI_SYSTEM_TABLE    *SystemTable
  )
{
  EFI_STATUS Status;
  EFI_HANDLE Handle = NULL;

  if (PcdGet32 (PcdSdIsArasan)) {
    DEBUG ((DEBUG_INFO, "SD is not routed to SdHost\n"));
    return EFI_REQUEST_UNLOAD_IMAGE;
  }

  Status = gBS->LocateProtocol (&gRaspberryPiFirmwareProtocolGuid, NULL, (VOID**)&mFwProtocol);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_MMCHOST_SD, "SdHost: Initialize\n"));
  DEBUG ((DEBUG_MMCHOST_SD, "Config:\n"));
  DEBUG ((DEBUG_MMCHOST_SD, " - FIFO_MAX_POLL_COUNT=%d\n", FIFO_MAX_POLL_COUNT));
  DEBUG ((DEBUG_MMCHOST_SD, " - CMD_STALL_AFTER_POLL_US=%dus\n", CMD_STALL_AFTER_POLL_US));
  DEBUG ((DEBUG_MMCHOST_SD, " - CMD_MIN_POLL_TOTAL_TIME_US=%dms\n", CMD_MIN_POLL_TOTAL_TIME_US / 1000));
  DEBUG ((DEBUG_MMCHOST_SD, " - CMD_MAX_POLL_COUNT=%d\n", CMD_MAX_POLL_COUNT));
  DEBUG ((DEBUG_MMCHOST_SD, " - CMD_MAX_RETRY_COUNT=%d\n", CMD_MAX_RETRY_COUNT));
  DEBUG ((DEBUG_MMCHOST_SD, " - CMD_STALL_AFTER_RETRY_US=%dus\n", CMD_STALL_AFTER_RETRY_US));

  Status = gBS->InstallMultipleProtocolInterfaces (
    &Handle,
    &gRaspberryPiMmcHostProtocolGuid,
    &gMmcHost,
    NULL
  );
  ASSERT_EFI_ERROR (Status);
  return Status;
}
