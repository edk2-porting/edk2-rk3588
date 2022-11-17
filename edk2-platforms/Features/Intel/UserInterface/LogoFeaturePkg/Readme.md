# Overview
* **Feature Name:** Logo
* **PI Phase(s) Supported:** DXE
* **SMM Required?** No

## Purpose
This feature provides the Logo image and driver that produces EDKII_PLATFORM_LOGO_PROTOCOL:

1. LogoDxe.inf includes a BMP logo in the EFI file, the driver provides the image via EDKII_PLATFORM_LOGO_PROTOCOL.
2. JpegLogoDxe.inf includes a JPEG logo in the EFI file, the driver uses EFI_HII_IMAGE_DECODER_PROTOCOL to decode the JPEG file and provide the image via EDKII_PLATFORM_LOGO_PROTOCOL.

# High-Level Theory of Operation

The driver produces a EDKII_PLATFORM_LOGO_PROTOCOL to provide a logo picture shown on setup screen.

The Logo file is built in HII package list as EFI resource, the driver uses EFI_HII_DATABASE_PROTOCOL
to the publish HII package list to HII Database and get images via EFI_HII_IMAGE_EX_PROTOCOL.

## Firmware Volumes

* PostMemory

## Modules

A bulleted list of the modules that make up the feature.

* LogoDxe
* JpegLogoDxe

## LogoDxe

This driver provides a bitmap image via EDKII_PLATFORM_LOGO_PROTOCOL.

## JpegLogoDxe

This driver uses EFI_HII_IMAGE_DECODER_PROTOCOL to decode the jpeg data and provide a bitmap image via
EDKII_PLATFORM_LOGO_PROTOCOL.

## Key Functions

* This feature produces a EDKII_PLATFORM_LOGO_PROTOCOL which could be used by consumer such as Edk2\MdeModulePkg\Library\BootLogoLib.

## Configuration

* gLogoFeaturePkgTokenSpaceGuid.PcdLogoFeatureEnable
  TRUE: Enable logo feature
  FALSE: Disable logo feature
* gLogoFeaturePkgTokenSpaceGuid.PcdJpgEnable
  TRUE: Use jpeg logo
  FALSE: Use bitmap logo

## Data Flows

Architecturally defined data structures and flows for the feature.

### Data flows of LogoDxe

```txt
| Consumer |    | LogoFeatruePkg |    |HiiDatabaseDxe|
------------    ------------------    ----------------
      |                 |                     |
      |                 |--bitmap image data->|
      |<---bitmap data--|<-return bitmap data-|
```

### Data flows of JpegLogoDxe

```txt
| Consumer |    | LogoFeatruePkg |    |HiiDatabaseDxe|    | EFI_HII_IMAGE_DECODER_PROTOCOL |
------------    ------------------    ----------------    ----------------------------------
      |                 |                     |                            |
      |                 |---Jpeg Image data-->|---------decode Jpeg------->|
      |<---bitmap data--|<-return bitmap data-|<-----return bitmap data----|
```

## Control Flows

Consumer gets image from the logo driver via EDKII_PLATFORM_LOGO_PROTOCOL.

## Build Flows

No any special build flows is needed.

## Test Point Results

Not available.

## Functional Exit Criteria

* Check wether EDKII_PLATFORM_LOGO_PROTOCOL->GetImage() return a default Intel logo.
* If using jpeg logo, a dex driver which produces EFI_HII_IMAGE_DECODER_PROTOCOL(name is
gEfiHiiImageDecoderNameJpegGuid) is necessary, it is used to decode Jpeg data.

## Feature Enabling Checklist

### Option 1

* Include AdvancedFeatures.dsc in platform dsc file, and use pcd to enable the feature.

```dsc
  gLogoFeaturePkgTokenSpaceGuid.PcdLogoFeatureEnable                      |TRUE
  gLogoFeaturePkgTokenSpaceGuid.PcdJpgEnable                              |FALSE
  !include Include/AdvancedFeatures.dsc
```

* Include PreMemory.fdf and inPostMemory.fdf in platform fdf file

```fdf
!include Include/PreMemory.fdf
!include Include/PostMemory.fdf
```

### Option 2

* To use bitmap logo, include "LogoFeaturePkg/LogoDxe/LogoDxe.inf" in platform dsc file, and include
"INF LogoFeaturePkg/LogoDxe/LogoDxe.inf" in platform fdf file.

* To use jpeg logo, include "LogoFeaturePkg/LogoDxe/JpegLogoDxe.inf" in platform dsc file, and include
"INF LogoFeaturePkg/LogoDxe/JpegLogoDxe.inf" in platform fdf file.

## Performance Impact

* LogoDxe
  * EFI size: 180 KB
  * Time spent in EDKII_PLATFORM_LOGO_PROTOCOL->GetImage(): about 18 milliseconds

* JpegLogoDxe
  * EFI size: 13.6 KB
  * Time spent in EDKII_PLATFORM_LOGO_PROTOCOL->GetImage(): about 80 milliseconds
