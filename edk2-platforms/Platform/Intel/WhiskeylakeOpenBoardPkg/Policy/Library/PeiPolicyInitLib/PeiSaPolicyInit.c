/** @file
  This file is SampleCode for Intel SA PEI Policy initialization.


  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiSaPolicyInit.h"
#include <Library/ConfigBlockLib.h>

/**
  PcieCardResetWorkAround performs PCIe Card reset on root port

  @param[in out] SiPreMemPolicyPpi SI_PREMEM_POLICY_PPI

  @retval EFI_SUCCESS              The policy is installed and initialized.
**/
EFI_STATUS
  PcieCardResetWorkAround (
  IN OUT   SI_PREMEM_POLICY_PPI         *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                      Status;
  SA_MISC_PEI_PREMEM_CONFIG       *MiscPeiPreMemConfig;
  SWITCHABLE_GRAPHICS_CONFIG      *SgGpioData;

  Status = GetConfigBlock((VOID *)SiPreMemPolicyPpi, &gSaMiscPeiPreMemConfigGuid, (VOID *)&MiscPeiPreMemConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock((VOID *)SiPreMemPolicyPpi, &gSwitchableGraphicsConfigGuid, (VOID *)&SgGpioData);
  ASSERT_EFI_ERROR(Status);

  if (SgGpioData->SaRtd3Pcie0Gpio.GpioSupport != NotSupported) {
    ///
    /// dGPU is present.
    ///      If PCIe Mode or SG Muxless
    ///              Power on MXM
    ///              Configure GPIOs to drive MXM in PCIe mode or SG Muxless
    ///      else
    ///              Do Nothing
    ///
    if ((MiscPeiPreMemConfig->SgMode == SgModeMuxless) ||
        (MiscPeiPreMemConfig->SgMode == SgModeDgpu)) {
      DEBUG((DEBUG_INFO, "Configure GPIOs for driving the dGPU.\n"));
      ///
      ///  Drive DGPU HOLD RST Enable to make sure we hold reset
      ///
      PcieGpioWrite (
        SgGpioData->SaRtd3Pcie0Gpio.HoldRst.GpioNo,
        SgGpioData->SaRtd3Pcie0Gpio.HoldRst.Active,
        GP_ENABLE
        );
      ///
      /// wait 100ms
      ///
      MicroSecondDelay((MiscPeiPreMemConfig->SgDelayAfterHoldReset) * STALL_ONE_MILLI_SECOND);

      ///
      /// Drive DGPU PWR EN to Power On MXM
      ///
      PcieGpioWrite (
        SgGpioData->SaRtd3Pcie0Gpio.PwrEnable.GpioNo,
        SgGpioData->SaRtd3Pcie0Gpio.PwrEnable.Active,
        GP_ENABLE
        );
      ///
      /// wait 300ms
      ///
      MicroSecondDelay((MiscPeiPreMemConfig->SgDelayAfterPwrEn) * STALL_ONE_MILLI_SECOND);

      ///
      /// Drive DGPU HOLD RST Disabled to remove reset
      ///
      PcieGpioWrite (
        SgGpioData->SaRtd3Pcie0Gpio.HoldRst.GpioNo,
        SgGpioData->SaRtd3Pcie0Gpio.HoldRst.Active,
        GP_DISABLE
        );
      ///
      /// wait 100ms
      ///
      MicroSecondDelay((MiscPeiPreMemConfig->SgDelayAfterHoldReset) * STALL_ONE_MILLI_SECOND);
    }
  }
  return EFI_SUCCESS;
}

/**
  PCIe GPIO Write

  @param[in] Gpio        - GPIO Number
  @param[in] Active      - GPIO Active Information; High/Low
  @param[in] Level       - Write GPIO value (0/1)

**/
VOID
PcieGpioWrite (
  IN  UINT32                Gpio,
  IN  BOOLEAN               Active,
  IN  BOOLEAN               Level
  )
{
  EFI_STATUS  Status;

  if (Active == 0) {
    Level = (~Level) & 0x1;
  }
  Status = GpioSetOutputValue(Gpio, (UINT32)Level);
  if (Status != EFI_SUCCESS) {
    return;
  }
}

