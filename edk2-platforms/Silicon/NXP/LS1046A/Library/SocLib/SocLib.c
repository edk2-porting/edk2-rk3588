/** @Soc.c
  SoC specific Library containg functions to initialize various SoC components

  Copyright 2017-2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Library/ChassisLib.h>
#include <Library/DebugLib.h>
#include <Library/SocLib.h>
#include <Soc.h>

/**
  Return the input clock frequency to an IP Module.
  This function reads the RCW bits and calculates the  PLL multiplier/divider
  values to be applied to various IP modules.
  If a module is disabled or doesn't exist on platform, then return zero.

  @param[in]  BaseClock  Base clock to which PLL multiplier/divider values is
                         to be applied.
  @param[in]  ClockType  Variable of Type NXP_IP_CLOCK. Indicates which IP clock
                         is to be retrieved.
  @param[in]  Args       Variable argument list which is parsed based on
                         ClockType. e.g. if the ClockType is NXP_I2C_CLOCK, then
                         the second argument will be interpreted as controller
                         number. e.g. if there are four i2c controllers in SOC,
                         then this value can be 0, 1, 2, 3
                         e.g. if ClockType is NXP_CORE_CLOCK, then second
                         argument is interpreted as cluster number and third
                         argument is interpreted as core number (within the
                         cluster)

  @return                Actual Clock Frequency. Return value 0 should be
                         interpreted as clock not being provided to IP.
**/
UINT64
SocGetClock (
  IN  UINT64        BaseClock,
  IN  NXP_IP_CLOCK  ClockType,
  IN  VA_LIST       Args
  )
{
  LS1046A_DEVICE_CONFIG  *Dcfg;
  UINT32                 RcwSr;
  UINT64                 ReturnValue;

  ReturnValue = 0;
  Dcfg = (LS1046A_DEVICE_CONFIG  *)LS1046A_DCFG_ADDRESS;

  switch (ClockType) {
  case NXP_UART_CLOCK:
  case NXP_I2C_CLOCK:
    RcwSr = DcfgRead32 ((UINTN)&Dcfg->RcwSr[0]);
    ReturnValue = BaseClock * SYS_PLL_RAT (RcwSr);
    ReturnValue >>= 1; // 1/2 Platform Clock
    break;
  default:
    break;
  }

  return ReturnValue;
}

/**
   Function to select pins depending upon pcd using supplemental
   configuration unit(SCFG) extended RCW controlled pinmux control
   register which contains the bits to provide pin multiplexing control.
   This register is reset on HRESET.
 **/
STATIC
VOID
ConfigScfgMux (VOID)
{
  LS1046A_SUPPLEMENTAL_CONFIG  *Scfg;
  UINT32 UsbPwrFault;

  Scfg = (LS1046A_SUPPLEMENTAL_CONFIG *)LS1046A_SCFG_ADDRESS;
  // Configures functionality of the IIC3_SCL to USB2_DRVVBUS
  // Configures functionality of the IIC3_SDA to USB2_PWRFAULT
  // USB3 is not used, configure mux to IIC4_SCL/IIC4_SDA
  ScfgWrite32 ((UINTN)&Scfg->RcwPMuxCr0, SCFG_RCWPMUXCRO_NOT_SELCR_USB);

  ScfgWrite32 ((UINTN)&Scfg->UsbDrvVBusSelCr, SCFG_USBDRVVBUS_SELCR_USB1);
  UsbPwrFault = (SCFG_USBPWRFAULT_DEDICATED << SCFG_USBPWRFAULT_USB3_SHIFT) |
                (SCFG_USBPWRFAULT_DEDICATED << SCFG_USBPWRFAULT_USB2_SHIFT) |
                (SCFG_USBPWRFAULT_SHARED << SCFG_USBPWRFAULT_USB1_SHIFT);
  ScfgWrite32 ((UINTN)&Scfg->UsbPwrFaultSelCr, UsbPwrFault);
  ScfgWrite32 ((UINTN)&Scfg->UsbPwrFaultSelCr, UsbPwrFault);
}

STATIC
VOID
ApplyErrata (
  VOID
  )
{
  ErratumA008997 ();
  ErratumA009007 ();
  ErratumA009008 ();
  ErratumA009798 ();
}



/**
  Function to initialize SoC specific constructs
 **/
VOID
SocInit (
  VOID
  )
{
  LS1046A_SUPPLEMENTAL_CONFIG  *Scfg;

  Scfg = (LS1046A_SUPPLEMENTAL_CONFIG *)LS1046A_SCFG_ADDRESS;

  /* Make SEC, SATA and USB reads and writes snoopable */
  ScfgOr32((UINTN)&Scfg->SnpCnfgCr, SCFG_SNPCNFGCR_SECRDSNP |
    SCFG_SNPCNFGCR_SECWRSNP | SCFG_SNPCNFGCR_USB1RDSNP |
    SCFG_SNPCNFGCR_USB1WRSNP | SCFG_SNPCNFGCR_USB2RDSNP |
    SCFG_SNPCNFGCR_USB2WRSNP | SCFG_SNPCNFGCR_USB3RDSNP |
    SCFG_SNPCNFGCR_USB3WRSNP | SCFG_SNPCNFGCR_SATARDSNP |
    SCFG_SNPCNFGCR_SATAWRSNP);

  ApplyErrata ();
  ChassisInit ();

  //
  // Due to the extensive functionality present on the chip and the limited number of external
  // signals available, several functional blocks share signal resources through multiplexing.
  // In this case when there is alternate functionality between multiple functional blocks,
  // the signal's function is determined at the chip level (rather than at the block level)
  // typically by a reset configuration word (RCW) option. Some of the signals' function are
  // determined externel to RCW at Power-on Reset Sequence.
  //
  ConfigScfgMux ();

  return;
}
