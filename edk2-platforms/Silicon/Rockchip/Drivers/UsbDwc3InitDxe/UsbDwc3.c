/** @file

  Copyright 2017, 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "UsbDwc3.h"

STATIC
VOID
XhciSetBeatBurstLength (
  IN  UINTN  UsbReg
  )
{
  DWC3       *Dwc3Reg;

  Dwc3Reg = (VOID *)(UsbReg + DWC3_REG_OFFSET);

  MmioAndThenOr32 ((UINTN)&Dwc3Reg->GSBusCfg0, ~USB3_ENABLE_BEAT_BURST_MASK,
    USB3_ENABLE_BEAT_BURST);

  MmioOr32 ((UINTN)&Dwc3Reg->GSBusCfg1, USB3_SET_BEAT_BURST_LIMIT);
}

STATIC
VOID
Dwc3SetFladj (
  IN  DWC3   *Dwc3Reg,
  IN  UINT32 Val
  )
{
  MmioOr32 ((UINTN)&Dwc3Reg->GFLAdj, GFLADJ_30MHZ_REG_SEL |
    GFLADJ_30MHZ (Val));
}

STATIC
VOID
Dwc3SetMode (
  IN  DWC3   *Dwc3Reg,
  IN  UINT32 Mode
  )
{
  MmioAndThenOr32 ((UINTN)&Dwc3Reg->GCtl,
    ~(DWC3_GCTL_PRTCAPDIR (DWC3_GCTL_PRTCAP_OTG)),
    DWC3_GCTL_PRTCAPDIR (Mode));
}

STATIC
VOID
Dwc3SetUsb2PhyCfg (
  IN  DWC3   *Dwc3Reg
  )
{
  UINT32     Reg;

  Reg = MmioRead32 ((UINTN)&Dwc3Reg->GUsb2PhyCfg);
  Reg &= ~(1 << 3);
  Reg |= 1 << 3;
  Reg &= ~(1 << 6);
  Reg |= 1 << 6;
  Reg &= ~(1 << 8);
  Reg |= 0 << 8;
  Reg &= ~(0xf << 10);
  Reg |= 0x5<<10;
  Reg &= ~(1 << 30);
  Reg |= 0 << 30;
  MmioWrite32 ((UINTN)&Dwc3Reg->GUsb2PhyCfg, Reg);
}

/**
  This function issues phy reset and core soft reset

  @param  Dwc3Reg      Pointer to DWC3 register.

**/
STATIC
VOID
Dwc3CoreSoftReset (
  IN  DWC3   *Dwc3Reg
  )
{
  //
  // Put core in reset before resetting PHY
  //
  MmioOr32 ((UINTN)&Dwc3Reg->GCtl, DWC3_GCTL_CORESOFTRESET);

  //
  // Assert USB2 PHY reset
  //
  MmioOr32 ((UINTN)&Dwc3Reg->GUsb3PipeCtl[0], DWC3_GUSB3PIPECTL_PHYSOFTRST);

  //
  // Assert USB3 PHY reset
  //
  MmioOr32 ((UINTN)&Dwc3Reg->GUsb2PhyCfg, DWC3_GUSB2PHYCFG_PHYSOFTRST);

  MemoryFence ();

  //
  // Clear USB3 PHY reset
  //
  MmioAnd32 ((UINTN)&Dwc3Reg->GUsb3PipeCtl[0], ~DWC3_GUSB3PIPECTL_PHYSOFTRST);

  //
  // Clear USB2 PHY reset
  //
  MmioAnd32 ((UINTN)&Dwc3Reg->GUsb2PhyCfg, ~DWC3_GUSB2PHYCFG_PHYSOFTRST);

  MemoryFence ();

  //
  // Take core out of reset, PHYs are stable now
  //
  MmioAnd32 ((UINTN)&Dwc3Reg->GCtl, ~DWC3_GCTL_CORESOFTRESET);
}

/**
  This function performs low-level initialization of DWC3 Core

  @param  Dwc3Reg      Pointer to DWC3 register.

**/
STATIC
EFI_STATUS
Dwc3CoreInit (
  IN  DWC3   *Dwc3Reg
  )
{
  UINT32     Revision;
  UINT32     Reg;
  UINTN      Dwc3Hwparams1;

  Revision = MmioRead32 ((UINTN)&Dwc3Reg->GSnpsId);
  //
  // This should read as 0x5533, ascii of U3(DWC_usb3) followed by revision num
  //
  if ((Revision & DWC3_GSNPSID_MASK) != DWC3_SYNOPSYS_ID) {
    DEBUG ((DEBUG_ERROR,"This is not a DesignWare USB3 DRD Core.\n"));
    return EFI_NOT_FOUND;
  }

  Dwc3CoreSoftReset (Dwc3Reg);

  Reg = MmioRead32 ((UINTN)&Dwc3Reg->GCtl);
  Reg &= ~DWC3_GCTL_SCALEDOWN_MASK;
  Reg &= ~DWC3_GCTL_DISSCRAMBLE;

  Dwc3Hwparams1 = MmioRead32 ((UINTN)&Dwc3Reg->GHwParams1);

  if (DWC3_GHWPARAMS1_EN_PWROPT (Dwc3Hwparams1) ==
      DWC3_GHWPARAMS1_EN_PWROPT_CLK) {
    Reg &= ~DWC3_GCTL_DSBLCLKGTNG;
  } else {
    DEBUG ((DEBUG_WARN,"No power optimization available.\n"));
  }

  if ((Revision & DWC3_RELEASE_MASK) < DWC3_RELEASE_190a) {
    Reg |= DWC3_GCTL_U2RSTECN;
  }

  MmioWrite32 ((UINTN)&Dwc3Reg->GCtl, Reg);

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
XhciCoreInit (
  IN  UINTN  UsbReg
  )
{
  EFI_STATUS Status;
  DWC3       *Dwc3Reg;

  Dwc3Reg = (VOID *)(UsbReg + DWC3_REG_OFFSET);

  Status = Dwc3CoreInit (Dwc3Reg);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Dwc3CoreInit Failed for controller 0x%x (0x%r) \n",
      UsbReg, Status));

    return Status;
  }

  Dwc3SetMode (Dwc3Reg, DWC3_GCTL_PRTCAP_HOST);

  Dwc3SetFladj (Dwc3Reg, GFLADJ_30MHZ_DEFAULT);

  Dwc3SetUsb2PhyCfg(Dwc3Reg);

  Dwc3Force20ClkFor30Clk((UINTN)&Dwc3Reg->Res2, TRUE);

  return Status;
}


EFI_STATUS
EFIAPI
InitializeDwc3Controller (
  IN  UINTN  UsbReg
  )
{
  EFI_STATUS Status;

  Status = XhciCoreInit (UsbReg);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "USB Controller init Failed for %d (0x%r)\n",
      UsbReg, Status));
    return EFI_DEVICE_ERROR;
  }

  //
  // Change beat burst and outstanding pipelined transfers requests
  //
  XhciSetBeatBurstLength (UsbReg);

  return EFI_SUCCESS;
}

/**
  The Entry Point of module. It follows the standard UEFI driver model.

  @param[in] ImageHandle   The firmware allocated handle for the EFI image.
  @param[in] SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS      The entry point is executed successfully.
  @retval other            Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
InitializeDwc3 (
  IN EFI_HANDLE            ImageHandle,
  IN EFI_SYSTEM_TABLE      *SystemTable
  )
{
  EFI_STATUS    Status;
  UINT32        NumUsbController;
  UINT32        ControllerAddr;
  UINT32        Index;

  NumUsbController = PcdGet32 (PcdNumDwc3Controller);

  for (Index = 0; Index < NumUsbController; Index++) {
    ControllerAddr = PcdGet32 (PcdDwc3BaseAddress) +
                      (Index * PcdGet32 (PcdDwc3Size));
    Status = InitializeDwc3Controller (ControllerAddr);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to init dwc3 controller 0x%x, error 0x%r \n",
        ControllerAddr, Status));
    }
  }

  UdPhyU3PortDisable();

  return Status;
}
