/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Uefi.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/SerialPortLib.h>
#include <Library/OemMiscLib.h>
#include <PlatformArch.h>
#include <Library/PlatformSysCtrlLib.h>
#include <Library/OemAddressMapLib.h>
#include <Library/LpcLib.h>
REPORT_PCIEDIDVID2BMC  PcieDeviceToReport[PCIEDEVICE_REPORT_MAX] = {
      {67,0,0,0},
      {225,0,0,3},
      {0xFFFF,0xFFFF,0xFFFF,0xFFFF},
      {0xFFFF,0xFFFF,0xFFFF,0xFFFF}
};

REPORT_PCIEDIDVID2BMC PcieDeviceToReport2P[PCIEDEVICE_REPORT_MAX] = {
  {0x79,0,0,0},
  {0xFF,0xFF,0xFF,1},
  {0xC1,0,0,2},
  {0xF9,0,0,3},
  {0xFF,0xFF,0xFF,4},
  {0x11,0,0,5},
  {0x31,0,0,6},
  {0x21,0,0,7}
};

VOID
GetPciDidVid (
  REPORT_PCIEDIDVID2BMC *Report
  )
{
  if (OemIsMpBoot ()) {
    (VOID)CopyMem (
            (VOID *)Report,
            (VOID *)PcieDeviceToReport2P,
            sizeof (PcieDeviceToReport2P)
            );
  } else {
    (VOID)CopyMem (
            (VOID *)Report,
            (VOID *)PcieDeviceToReport,
            sizeof (PcieDeviceToReport)
            );
  }
}

// Right now we only support 1P
BOOLEAN OemIsSocketPresent (UINTN Socket)
{
  if (0 == Socket)
  {
    return TRUE;
  }

  if(1 == Socket)
  {
    return TRUE;
  }

  return FALSE;
}


UINTN OemGetSocketNumber (VOID)
{

    if(!OemIsMpBoot())
    {
        return 1;
    }

    return 2;

}


UINTN OemGetDdrChannel (VOID)
{
    return 4;
}


UINTN OemGetDimmSlot(UINTN Socket, UINTN Channel)
{
    return 2;
}


// Nothing to do for EVB
VOID OemPostEndIndicator (VOID)
{

     DEBUG((EFI_D_ERROR,"M3 release reset CONFIG........."));

     MmioWrite32(0xd0002180, 0x3);
     MmioWrite32(0xd0002194, 0xa4);
     MmioWrite32(0xd0000a54, 0x1);

     MicroSecondDelay(10000);

     MmioWrite32(0xd0002108, 0x1);
     MmioWrite32(0xd0002114, 0x1);
     MmioWrite32(0xd0002120, 0x1);
     MmioWrite32(0xd0003108, 0x1);

     MicroSecondDelay(500000);
     DEBUG((EFI_D_ERROR,"Done\n"));

}



VOID CoreSelectBoot(VOID)
{
    if (!PcdGet64 (PcdTrustedFirmwareEnable))
    {
        StartUpBSP ();
    }

    return;
}

BOOLEAN OemIsMpBoot()
{
    UINT32 Tmp;

    Tmp = MmioRead32(0x602E0050);
    if ( ((Tmp >> 10) & 0xF) == 0x3)
        return TRUE;
    else
        return FALSE;
}

VOID OemLpcInit(VOID)
{
    LpcInit();
    return;
}

UINT32 OemIsWarmBoot(VOID)
{
    return 0;
}

VOID OemBiosSwitch(UINT32 Master)
{
    (VOID)Master;
    return;
}

BOOLEAN OemIsNeedDisableExpanderBuffer(VOID)
{
    return TRUE;
}
