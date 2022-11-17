/** @file
  This file configures Aspire VN7-572G board-specific FSP UPDs.

  Copyright (c) 2021, Baruch Binyamin Doron
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "PeiPchPolicyUpdate.h"
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <PchPolicyCommon.h>

/* TODO:
 * - Validate PCH Sample policies: only SA one used by default.
 * - Remove likely fuse-disabled devices when reset handling is committed?
 * - Remove duplicate policy
 *   - Consider updating some policies, rather than overriding. This could be factored into
 *     BoardInitLib for deduplication
 * - Copy initialised array, where sane
 * - Set IgdDvmt50PreAlloc? */

#define SA_VR           0
#define IA_VR           1
#define GT_UNSLICED_VR  2
#define GT_SLICED_VR    3

// Numbering is one-based indexing. The code uses zero-based indexing,
// where these are the first unpopulated ports
#define NUM_POPULATED_USB2_PORTS  9
#define NUM_POPULATED_USB3_PORTS  4
// The first three USB2 ports are all also USB3 ports. More specifically,
// the routing for these is copied from the vendor's SetupUtility
#define POPULATED_USB3_AS_USB2_PORTS  3

/**
  Performs the remainder of board-specific FSP Policy initialization.

  @param[in][out]  FspmUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspBoardPolicyUpdatePreMem (
  IN OUT FSPM_UPD    *FspmUpd
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  // BUGBUG: Preserve FSP defaults - PeiSiliconPolicyInitLibFsp ultimately overrides to 0.
  // Drop when https://edk2.groups.io/g/devel/message/79391 is merged
  FspmUpd->FspmConfig.PeciC10Reset = 1;
  FspmUpd->FspmConfig.RefClk = 1;  // Maybe "auto" is safe, but that isn't the FSP default

  // TODO: Why should this be here?
  FspmUpd->FspmConfig.TsegSize = PcdGet32 (PcdTsegSize);

  /* System Agent config */
  FspmUpd->FspmConfig.UserBd = PcdGet8 (PcdSaMiscUserBd);
  FspmUpd->FspmConfig.DqPinsInterleaved = (UINT8) PcdGetBool (PcdMrcDqPinsInterleaved);
  FspmUpd->FspmConfig.CaVrefConfig = PcdGet8 (PcdMrcCaVrefConfig);
  FspmUpd->FspmConfig.SaGv = 3;  // Enabled

  /* iGFX config */
  FspmUpd->FspmConfig.PrimaryDisplay = 4;  // Switchable Graphics

  /* PCIe config */
  FspmUpd->FspmConfig.PcieRpEnableMask = 0x341;  // Ports 1, 7, 9 and 10

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  Performs the remainder of board-specific FSP Policy initialization.

  @param[in][out]  FspsUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspBoardPolicyUpdate (
  IN OUT FSPS_UPD    *FspsUpd
  )
{
  INTN  Index;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  // FIXME/NB: This is insecure and not production-ready!
  // TODO: Configure SPI lockdown by variable on FrontPage?
  // - Later, also configure stronger protection: PRRs
  FspsUpd->FspsConfig.PchLockDownBiosLock = 0;  // Default. Will enable, not remove
  FspsUpd->FspsConfig.PchLockDownSpiEiss = 0;
  // This may be PWRM+0x18[BIT22], causing HSTI "PCH Security Configuration -  Reserved Check failure"
  // I think the intel_pmc_core kernel module requires this to populate debugfs?
  FspsUpd->FspsTestConfig.PchPmPmcReadDisable = 0;

  // BUGBUG: Preserve FSP defaults - Pei*PolicyLib ultimately overrides
  // Requires HW support?
  FspsUpd->FspsConfig.PchPmSlpS0VmEnable = 0;
  // Do not clear UART2, may be set by PeiPchPolicyUpdate.c:PeiFspPchPolicyUpdate()
  // - Presently always set to PCI by policy
  ZeroMem (&FspsUpd->FspsConfig.SerialIoDevMode, sizeof(FspsUpd->FspsConfig.SerialIoDevMode)-1);
  // I2C controllers are in PCI mode
  // - Board has no GPIO expander on I2C4 (despite SetupUtility claim that it does - this would be static text)
  FspsUpd->FspsConfig.SerialIoDevMode[0] = 2;
  FspsUpd->FspsConfig.SerialIoDevMode[1] = 2;

  // Acer IDs (TODO: "Newgate" IDs)
  FspsUpd->FspsConfig.DefaultSvid = 0x1025;
  FspsUpd->FspsConfig.DefaultSid = 0x1037;
  FspsUpd->FspsConfig.PchSubSystemVendorId = 0x1025;
  FspsUpd->FspsConfig.PchSubSystemId = 0x1037;

  /* System Agent config */
  // Set the Thermal Control Circuit (TCC) activation value to 97C
  // even though FSP integration guide says to set it to 100C for SKL-U
  // (offset at 0), because when the TCC activates at 100C, the CPU
  // will have already shut itself down from overheating protection.
  FspsUpd->FspsTestConfig.TccActivationOffset = 3;

  // VR Slew rate setting for improving audible noise
  FspsUpd->FspsConfig.AcousticNoiseMitigation = 1;
  FspsUpd->FspsConfig.SlowSlewRateForIa = 3;  // Fast/16
  FspsUpd->FspsConfig.SlowSlewRateForGt = 3;  // Fast/16
  FspsUpd->FspsConfig.SlowSlewRateForSa = 0;  // Fast/2
  FspsUpd->FspsConfig.FastPkgCRampDisableIa = 0;
  FspsUpd->FspsConfig.FastPkgCRampDisableGt = 0;
  FspsUpd->FspsConfig.FastPkgCRampDisableSa = 0;

  // VR domain configuration (copied from board port, before VR config moved
  // to SoC. Should match SKL-U (GT2, 15W) in the SKL-U datasheet, vol. 1)
  FspsUpd->FspsConfig.AcLoadline[SA_VR] = 1030;   // 10.3mOhm (in 1/100 increments)
  FspsUpd->FspsConfig.DcLoadline[SA_VR] = 1030;   // 10.3mOhm (in 1/100 increments)
  FspsUpd->FspsConfig.Psi1Threshold[SA_VR] = 80;  // 20A (in 1/4 increments)
  FspsUpd->FspsConfig.Psi2Threshold[SA_VR] = 16;  // 4A (in 1/4 increments)
  FspsUpd->FspsConfig.Psi3Threshold[SA_VR] = 4;   // 1A (in 1/4 increments)
  FspsUpd->FspsConfig.IccMax[SA_VR] = 18;         // 4.5A (in 1/4 increments)
  FspsUpd->FspsConfig.VrVoltageLimit[SA_VR] = 1520;  // 1520mV

  FspsUpd->FspsConfig.AcLoadline[IA_VR] = 240;    // 2.4mOhm (in 1/100 increments)
  FspsUpd->FspsConfig.DcLoadline[IA_VR] = 240;    // 2.4mOhm (in 1/100 increments)
  FspsUpd->FspsConfig.Psi1Threshold[IA_VR] = 80;  // 20A (in 1/4 increments)
  FspsUpd->FspsConfig.Psi2Threshold[IA_VR] = 20;  // 5A (in 1/4 increments)
  FspsUpd->FspsConfig.Psi3Threshold[IA_VR] = 4;   // 1A (in 1/4 increments)
  FspsUpd->FspsConfig.IccMax[IA_VR] = 116;        // 29A (in 1/4 increments)
  FspsUpd->FspsConfig.VrVoltageLimit[IA_VR] = 1520;  // 1520mV

  FspsUpd->FspsConfig.AcLoadline[GT_UNSLICED_VR] = 310;    // 3.1mOhm (in 1/100 increments)
  FspsUpd->FspsConfig.DcLoadline[GT_UNSLICED_VR] = 310;    // 3.1mOhm (in 1/100 increments)
  FspsUpd->FspsConfig.Psi1Threshold[GT_UNSLICED_VR] = 80;  // 20A (in 1/4 increments)
  FspsUpd->FspsConfig.Psi2Threshold[GT_UNSLICED_VR] = 20;  // 5A (in 1/4 increments)
  FspsUpd->FspsConfig.Psi3Threshold[GT_UNSLICED_VR] = 4;   // 1A (in 1/4 increments)
  FspsUpd->FspsConfig.IccMax[GT_UNSLICED_VR] = 124;        // 31A (in 1/4 increments)
  FspsUpd->FspsConfig.VrVoltageLimit[GT_UNSLICED_VR] = 1520;  // 1520mV

  FspsUpd->FspsConfig.AcLoadline[GT_SLICED_VR] = 310;    // 3.1mOhm (in 1/100 increments)
  FspsUpd->FspsConfig.DcLoadline[GT_SLICED_VR] = 310;    // 3.1mOhm (in 1/100 increments)
  FspsUpd->FspsConfig.Psi1Threshold[GT_SLICED_VR] = 80;  // 20A (in 1/4 increments)
  FspsUpd->FspsConfig.Psi2Threshold[GT_SLICED_VR] = 20;  // 5A (in 1/4 increments)
  FspsUpd->FspsConfig.Psi3Threshold[GT_SLICED_VR] = 4;   // 1A (in 1/4 increments)
  FspsUpd->FspsConfig.IccMax[GT_SLICED_VR] = 124;        // 31A (in 1/4 increments)
  FspsUpd->FspsConfig.VrVoltageLimit[GT_SLICED_VR] = 1520;  // 1520mV

  // PL1, PL2 override 35W, PL4 override 43W (converted to processor units, then 125 mW increments)
  // BUGBUG: PL1 and PL2 not being configured in MSR 0x610. Requires additional UPD?
  FspsUpd->FspsTestConfig.PowerLimit1 = 35000;
  FspsUpd->FspsTestConfig.PowerLimit2Power = 35000;
  FspsUpd->FspsTestConfig.PowerLimit4 = 43000;

  // ISL95857 VR
  // Send VR specific command for PS4 exit issue
  FspsUpd->FspsConfig.SendVrMbxCmd1 = 2;
  // Send VR mailbox command for IA/GT/SA rails
  FspsUpd->FspsConfig.IslVrCmd = 2;

  /* Skycam config */
  FspsUpd->FspsConfig.SaImguEnable = 0;
  FspsUpd->FspsConfig.PchCio2Enable = 0;

  /* Sensor hub config */
  FspsUpd->FspsConfig.PchIshEnable = 0;

  /* xHCI config */
  FspsUpd->FspsConfig.SsicPortEnable = 0;
  // Configure USB2 ports in two sets
  for (Index = 0; Index < POPULATED_USB3_AS_USB2_PORTS; Index++) {
    FspsUpd->FspsConfig.Usb2AfeTxiset[Index] = 0x2;    // 16.9mV
    FspsUpd->FspsConfig.Usb2AfePredeemp[Index] = 1;    // De-emphasis on
    FspsUpd->FspsConfig.Usb2AfePetxiset[Index] = 0x3;  // 28.15mV
    FspsUpd->FspsConfig.Usb2AfePehalfbit[Index] = 1;   // Half-bit
    FspsUpd->FspsConfig.Usb2OverCurrentPin[Index] = PchUsbOverCurrentPinSkip;
  }
  for (Index = POPULATED_USB3_AS_USB2_PORTS; Index < NUM_POPULATED_USB2_PORTS; Index++) {
    FspsUpd->FspsConfig.Usb2AfeTxiset[Index] = 0;      // 0mV
    FspsUpd->FspsConfig.Usb2AfePredeemp[Index] = 0x2;  // Pre-emphasis and de-emphasis on
    FspsUpd->FspsConfig.Usb2AfePetxiset[Index] = 0x7;  // 56.3mV
    FspsUpd->FspsConfig.Usb2AfePehalfbit[Index] = 1;   // Half-bit
    FspsUpd->FspsConfig.Usb2OverCurrentPin[Index] = PchUsbOverCurrentPinSkip;
  }
  // Configure all USB3 ports
  for (Index = 0; Index < NUM_POPULATED_USB3_PORTS; Index++) {
    FspsUpd->FspsConfig.Usb3HsioTxDeEmphEnable[Index] = 1;
    FspsUpd->FspsConfig.Usb3HsioTxDeEmph[Index] = 0x29;  // Default (approximately -3.5dB de-emphasis)
    FspsUpd->FspsConfig.Usb3OverCurrentPin[Index] = PchUsbOverCurrentPinSkip;
  }
  // Disable supported, but not present, ports
  for (Index = NUM_POPULATED_USB2_PORTS; Index < PCH_LP_XHCI_MAX_USB2_PORTS; Index++) {
    FspsUpd->FspsConfig.PortUsb20Enable[Index] = 0;
  }
  for (Index = NUM_POPULATED_USB3_PORTS; Index < PCH_LP_XHCI_MAX_USB3_PORTS; Index++) {
    FspsUpd->FspsConfig.PortUsb30Enable[Index] = 0;
  }

  /* xDCI config */
  FspsUpd->FspsConfig.XdciEnable = 0;

  /* SATA config */
  // This is a hard silicon requirement, discovered several times by coreboot boards
  FspsUpd->FspsConfig.SataPwrOptEnable = 1;
  // Disable supported, but not present, ports
  FspsUpd->FspsConfig.SataPortsEnable[0] = 0;

  /* PCIe config */
  // Port 1 (dGPU; x4)
  FspsUpd->FspsConfig.PcieRpAdvancedErrorReporting[0] = 1;
  FspsUpd->FspsConfig.PcieRpLtrEnable[0] = 1;
  FspsUpd->FspsConfig.PcieRpClkReqSupport[0] = 1;
  FspsUpd->FspsConfig.PcieRpClkReqNumber[0] = 0;
  FspsUpd->FspsConfig.PcieRpMaxPayload[0] = PchPcieMaxPayload256;
  FspsUpd->FspsConfig.PcieRpClkSrcNumber[0] = 0x1F;  // CLKSRC pin invalid
  // Port 7 (NGFF; x2)
  FspsUpd->FspsConfig.PcieRpAdvancedErrorReporting[6] = 1;
  FspsUpd->FspsConfig.PcieRpLtrEnable[6] = 1;
  FspsUpd->FspsConfig.PcieRpClkReqSupport[6] = 1;
  FspsUpd->FspsConfig.PcieRpClkReqNumber[6] = 3;
  FspsUpd->FspsConfig.PcieRpMaxPayload[6] = PchPcieMaxPayload256;
  FspsUpd->FspsConfig.PcieRpClkSrcNumber[6] = 0x1F;  // CLKSRC pin invalid
  // Port 9 (LAN)
  FspsUpd->FspsConfig.PcieRpAdvancedErrorReporting[8] = 1;
  FspsUpd->FspsConfig.PcieRpLtrEnable[8] = 1;
  FspsUpd->FspsConfig.PcieRpClkReqSupport[8] = 1;
  FspsUpd->FspsConfig.PcieRpClkReqNumber[8] = 1;
  FspsUpd->FspsConfig.PcieRpMaxPayload[8] = PchPcieMaxPayload256;
  FspsUpd->FspsConfig.PcieRpClkSrcNumber[8] = 0x1F;  // CLKSRC pin invalid
  // Port 10 (WLAN)
  FspsUpd->FspsConfig.PcieRpAdvancedErrorReporting[9] = 1;
  FspsUpd->FspsConfig.PcieRpLtrEnable[9] = 1;
  FspsUpd->FspsConfig.PcieRpClkReqSupport[9] = 1;
  FspsUpd->FspsConfig.PcieRpClkReqNumber[9] = 2;
  FspsUpd->FspsConfig.PcieRpMaxPayload[9] = PchPcieMaxPayload256;
  FspsUpd->FspsConfig.PcieRpClkSrcNumber[9] = 0x1F;  // CLKSRC pin invalid
  // ASPM L0s is broken/unsupported on Qualcomm Atheros QCA6174 (AER: corrected errors)
  // BUGBUG: PcieDeviceTable.c entries aren't setting PCI config space
  FspsUpd->FspsConfig.PcieRpAspm[9] = PchPcieAspmL1;

  /* SCS config */
  // Although platform NVS area shows this enabled, the SD card reader is connected over USB, not SCS
  FspsUpd->FspsConfig.ScsEmmcEnabled = 0;
  FspsUpd->FspsConfig.ScsSdCardEnabled = 0;

  /* LPC config */
  // EC/KBC requires continuous mode
  FspsUpd->FspsConfig.PchPmLpcClockRun = 1;
  FspsUpd->FspsConfig.PchSirqMode = PchContinuousMode;

  /* HDA config */
  // FIXME: DspEnable is set, per PeiPchPolicyLib, however it is disabled in the HOB produced by FSP
  // Returned to DXE as HOB, used to select blob for NHLT
  // - FIXME: 1ch array DMIC may not be supported by the Linux driver
  FspsUpd->FspsConfig.PchHdaDspEndpointDmic = PchHdaDmic1chArray;

  /* GbE config */
  FspsUpd->FspsConfig.PchLanEnable = 0;

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
  return EFI_SUCCESS;
}
