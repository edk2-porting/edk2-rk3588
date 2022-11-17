# Ice Lake-SP and Cooper Lake Intel&reg; IFWI Ingredients
The IFWI (Integrated Firmware Image) is the full firmware image for Intel based platforms. The IFWI is typically written to a NOR flash chip and read by the PCH over the SPI bus when the platform is powered on. The bill of materials for an IFWI varies with each chipset, here are some examples of what it can contain:

- Flash Descriptor
- ME Firmware
- EC Firmware
- Integrated Ethernet Firmware
- Thunderbolt Firmware (for SoCs with integrated (non-discrete) Thunderbolt)
- System Firmware (aka BIOS)

For the Ice Lake-SP and Cooper Lake chipsets, the IFWI contains the following:
- Flash Descriptor
- ME Firmware
- System Firmware (aka BIOS)


This directory provides IFWI ingredients for Ice Lake-SP and Cooper Lake boards. This allows the MinPlatform build to produce a full IFWI (Integrated Firmware Image) instead of just the BIOS region.

## Current IFWI Ingredient Versions

### ME Firmware

Board Name  | ME Firmware Type | Version Number
:---------- | :--------------- | :-------------
Wilson City | SPS | 4.4.4.58
Cooper City | SPS | 4.4.4.33
Delta Lake  | Ignition | 4.2.4.14
