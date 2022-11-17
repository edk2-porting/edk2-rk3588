/** @file
*
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Uefi.h>
#include <PlatformArch.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CpldD06.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/LpcLib.h>
#include <Library/OemAddressMapLib.h>
#include <Library/OemMiscLib.h>
#include <Library/PcdLib.h>
#include <Library/PlatformSysCtrlLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TimerLib.h>

REPORT_PCIEDIDVID2BMC PcieDeviceToReport[PCIEDEVICE_REPORT_MAX] = {
  {67,0,0,0},
  {225,0,0,3},
  {0xFFFF,0xFFFF,0xFFFF,0xFFFF},
  {0xFFFF,0xFFFF,0xFFFF,0xFFFF}
};

//Cpu0 Riser type is (X16 + X8) & Cpu1 Riser type is (X16 + X8)
REPORT_PCIEDIDVID2BMC PcieDeviceToReport_2P_Type1 [PCIEDEVICE_REPORT_MAX] = {
  {0x01,0,0,0},
  {0x03,0,0,1},
  {0xFF,0xFF,0xFF,2},
  {0x81,0,0,3},
  {0x84,0,0,4},
  {0xFF,0xFF,0xFF,5}
};

//Cpu0 Riser type is (X16 + X8) & Cpu1 Riser type is (3 * X8)
REPORT_PCIEDIDVID2BMC PcieDeviceToReport_2P_Type2 [PCIEDEVICE_REPORT_MAX] = {
  {0x01,0,0,0},
  {0x03,0,0,1},
  {0xFF,0xFF,0xFF,2},
  {0xFF,0xFF,0xFF,3},
  {0x81,0,0,4},
  {0x85,0,0,5}
};

//Cpu0 Riser type is (3 * X8) & Cpu1 Riser type is (X16 + X8)
REPORT_PCIEDIDVID2BMC PcieDeviceToReport_2P_Type3 [PCIEDEVICE_REPORT_MAX] = {
  {0xFF,0xFF,0xFF,0},
  {0x01,0,0,1},
  {0x04,0,0,2},
  {0x81,0,0,3},
  {0x84,0,0,4},
  {0xFF,0xFF,0xFF,5}
};

//Cpu0 Riser type is (3 * X8) & Cpu1 Riser type is (3 * X8)
REPORT_PCIEDIDVID2BMC PcieDeviceToReport_2P_Type4 [PCIEDEVICE_REPORT_MAX] = {
  {0xFF,0xFF,0xFF,0},
  {0x01,0,0,1},
  {0x04,0,0,2},
  {0xFF,0xFF,0xFF,3},
  {0x81,0,0,4},
  {0x85,0,0,5}
};

VOID
GetPciDidVid (
  REPORT_PCIEDIDVID2BMC *Report
  )
{
  UINT32                             PresentStatus;
  UINT32                             CardType;
  UINT8                              Cpu0CardType = 0;
  UINT8                              Cpu1CardType = 0;

  PresentStatus = MmioRead32 (CPLD_BASE_ADDRESS + CPLD_RISER_PRSNT_FLAG);
  CardType = MmioRead32 (CPLD_BASE_ADDRESS + CPLD_RISER2_BOARD_ID);

  // Offset 0x40: Bit7 = 1 CPU0 Riser present
  if ((PresentStatus & CPU0_RISER_PRESENT) != 0) {
    Cpu0CardType = (UINT8) (PresentStatus >> 8);
  }

  // Offset 0x40: Bit6 = 1 CPU1 Riser present
  if ((PresentStatus & CPU1_RISER_PRESENT) != 0) {
    Cpu1CardType = (UINT8)CardType;
  }

  if (OemIsMpBoot ()) {
    if (Cpu0CardType == CPLD_X16_X8_BOARD_ID) {
      if (Cpu1CardType == CPLD_X16_X8_BOARD_ID) {
        (VOID)CopyMem ((VOID *)Report, (VOID *)PcieDeviceToReport_2P_Type1,
                        sizeof (PcieDeviceToReport_2P_Type1));
      } else {
        (VOID)CopyMem ((VOID *)Report, (VOID *)PcieDeviceToReport_2P_Type2,
                        sizeof (PcieDeviceToReport_2P_Type2));
      }
    } else {
      if (Cpu1CardType == CPLD_X16_X8_BOARD_ID) {
        (VOID)CopyMem ((VOID *)Report, (VOID *)PcieDeviceToReport_2P_Type3,
                        sizeof (PcieDeviceToReport_2P_Type3));
      } else {
        (VOID)CopyMem ((VOID *)Report, (VOID *)PcieDeviceToReport_2P_Type4,
                        sizeof (PcieDeviceToReport_2P_Type4));
      }
    }
  } else {
    (VOID)CopyMem ((VOID *)Report, (VOID *)PcieDeviceToReport,
                    sizeof (PcieDeviceToReport));
  }
}


// Right now we only support 1P
BOOLEAN
OemIsSocketPresent (
  UINTN Socket
  )
{
  UINT32 SocketMask = PcdGet32 (PcdSocketMask);
  return (BOOLEAN)((SocketMask & (1 << Socket)) ? TRUE : FALSE);
}


UINTN
OemGetSocketNumber (
  VOID
  )
{
  if(!OemIsMpBoot ()) {
    return 1;
  }

  return MAX_PROCESSOR_SOCKETS;
}


UINTN
OemGetDdrChannel (
  VOID
  )
{
  return MAX_MEMORY_CHANNELS;
}


UINTN
OemGetDimmSlot (
  UINTN Socket,
  UINTN Channel
  )
{
  return MAX_DIMM_PER_CHANNEL;
}


BOOLEAN
OemIsMpBoot (
  VOID
  )
{
  return PcdGet32 (PcdIsMPBoot);
}

VOID
OemLpcInit (
  VOID
  )
{
  LpcInit ();
  return;
}

UINT32
OemIsWarmBoot (
  VOID
  )
{
  return 0;
}

VOID
OemBiosSwitch (
  UINT32 Master
  )
{
  (VOID)Master;
  return;
}

BOOLEAN
OemIsNeedDisableExpanderBuffer (
  VOID
  )
{
  return TRUE;
}

UINTN OemGetCpuFreq (UINT8 Socket)
{
  UINT8 BoardRevision;

  BoardRevision = MmioRead8 (CPLD_BASE_ADDRESS + CPLD_BOM_VER_FLAG);

  // Board revision 4 and higher run at 2.5GHz
  // Earlier revisions run at 2GHz
  if (BoardRevision >= CPLD_BOARD_REVISION_4TH) {
    return 2500000000;
  } else {
    return 2000000000;
  }
}

UINTN
OemGetHccsFreq (
  VOID
  )
{
  return HCCS_PLL_VALUE_2600;
}

