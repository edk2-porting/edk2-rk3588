# Intel&reg; PCH Ignition Firmware
Copyright &copy; 2021, Intel Corporation. All rights reserved.

## Overview
Intel&reg; PCH Ignition Firmware is a variant of Intel&reg; Management Engine (ME) Firmware that is intended to provide lightweight chipset initialization. It does not contain all the features of the Intel&reg; Server Platform Services (SPS) ME Firmware. Intel&reg; PCH Ignition Firmware is consequently much smaller than Intel&reg; SPS Firmware (~0.5 MB vs. ~3 MB). Both Intel&reg; SPS Firmware and Intel&reg; PCH Ignition Firmware are specifically designed for server platforms with Intel&reg; Xeon&reg; Processors and are different than the Intel&reg; Management Engine Firmware found on client platforms. This Intel&reg; PCH Ignition Firmware may only be used with 3rd generation Intel&reg; Xeon&reg; Processors.

Note that Intel&reg; PCH Ignition Firmware is mutually exclusive with Intel&reg; SPS Firmware. After flashing and booting Intel&reg; PCH Ignition Firmware, the PCH will be locked to Intel&reg; PCH Ignition Firmware and will not be able to run Intel&reg; SPS Firmware, even if the SPI is re-flashed. Conversely, a system that has Intel&reg; SPS Firmware and has gone through End of Manufacturing (EOM) will not be able to run Intel&reg; PCH Ignition Firmware.

## Package Contents

Intel&reg; PCH Ignition Firmware is provided in the file:
 - MeRegion.bin - Intel&reg; PCH Ignition Firmware Image

## License Information
This release package contains the following license files:
 - License.txt - The primary license is provided in the main directory.
 - Other_Licenses - Third-party licenses are provided in the Other_Licenses directory.
