/** @file
  HDAUDIO policy

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _HDAUDIO_CONFIG_H_
#define _HDAUDIO_CONFIG_H_

#define HDAUDIO_CONFIG_REVISION 2
extern EFI_GUID gHdAudioConfigGuid;

#pragma pack (push,1)

///
/// The PCH_HDAUDIO_CONFIG block describes the expected configuration of the Intel HD Audio feature.
///
#define PCH_HDAUDIO_AUTO  2

#define HDAUDIO_VERB_TABLE_VIDDID(Vid,Did)                      (UINT32)((UINT16)Vid | ((UINT16)Did << 16))
#define HDAUDIO_VERB_TABLE_RID_SDI_SIZE(Rid,Sdi,VerbTableSize)  (UINT32)((UINT8)Rid | ((UINT8)Sdi << 8) | ((UINT16)VerbTableSize << 16))
#define HDAUDIO_VERB_TABLE_CMD_SIZE(VerbTable)                  ((sizeof (VerbTable) - sizeof (PCH_HDA_VERB_TABLE_HEADER)) / (sizeof (UINT32)))

///
/// Use this macro to create HDAUDIO_VERB_TABLE and populate size automatically
///
#define HDAUDIO_VERB_TABLE_INIT(Vid,Did,Rid,Sdi,...) \
{ \
  { Vid, Did, Rid, Sdi, (sizeof((UINT32[]){__VA_ARGS__})/sizeof(UINT32)) }, \
  { __VA_ARGS__ } \
}

/**
  Azalia verb table header
  Every verb table should contain this defined header and followed by azalia verb commands.
**/
typedef struct {
  UINT16  VendorId;             ///< Codec Vendor ID
  UINT16  DeviceId;             ///< Codec Device ID
  UINT8   RevisionId;           ///< Revision ID of the codec. 0xFF matches any revision.
  UINT8   SdiNum;               ///< SDI number, 0xFF matches any SDI.
  UINT16  DataDwords;           ///< Number of data DWORDs following the header.
} PCH_HDA_VERB_TABLE_HEADER;

#ifdef _MSC_VER
//
// Disable "zero-sized array in struct/union" extension warning.
// Used for neater verb table definitions.
//
#pragma warning (push)
#pragma warning (disable: 4200)
#endif
typedef struct  {
  PCH_HDA_VERB_TABLE_HEADER  Header;
  UINT32 Data[];
} HDAUDIO_VERB_TABLE;
#ifdef _MSC_VER
#pragma warning (pop)
#endif

enum PCH_HDAUDIO_IO_BUFFER_OWNERSHIP {
  PchHdaIoBufOwnerHdaLink        = 0,   ///< HD-Audio link owns all the I/O buffers.
  PchHdaIoBufOwnerHdaLinkI2sPort = 1,   ///< HD-Audio link owns 4 and I2S port owns 4 of the I/O buffers.
  PchHdaIoBufOwnerI2sPort        = 3    ///< I2S0 and I2S1 ports own all the I/O buffers.
};

enum PCH_HDAUDIO_IO_BUFFER_VOLTAGE {
  PchHdaIoBuf33V = 0,
  PchHdaIoBuf18V = 1
};

enum PCH_HDAUDIO_VC_TYPE {
  PchHdaVc0 = 0,
  PchHdaVc1 = 1
};

enum PCH_HDAUDIO_DMIC_TYPE {
  PchHdaDmicDisabled = 0,
  PchHdaDmic2chArray = 1,
  PchHdaDmic4chArray = 2,
  PchHdaDmic1chArray = 3
};

typedef enum {
  PchHdaLinkFreq6MHz  = 0,
  PchHdaLinkFreq12MHz = 1,
  PchHdaLinkFreq24MHz = 2,
  PchHdaLinkFreq48MHz = 3,
  PchHdaLinkFreq96MHz = 4,
  PchHdaLinkFreqInvalid
} PCH_HDAUDIO_LINK_FREQUENCY;

typedef enum  {
  PchHdaIDispMode2T = 0,
  PchHdaIDispMode1T = 1
} PCH_HDAUDIO_IDISP_TMODE;

/**
  This structure contains the policies which are related to HD Audio device (cAVS).

  <b>Revision 1:</b>
  - Inital version.
  <b>Revision 2:</b>
  - Deprecate DspPpModuleMask: Pre/Post-processing modules (3rd Party IP) support moved to Platform Package.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  /**
    This member describes whether or not Intel HD Audio (Azalia) should be enabled.
    If enabled (in Auto mode) and no codec exists the reference code will automatically disable
    the HD Audio device.
    0: Disable, 1: Enable, <b>2: Auto (enabled if codec detected and initialized, disabled otherwise)</b>
  **/
  UINT32  Enable               :  2;
  UINT32  DspEnable            :  1;    ///< DSP enablement: 0: Disable; <b>1: Enable</b>
  UINT32  Pme                  :  1;    ///< Azalia wake-on-ring, <b>0: Disable</b>; 1: Enable
  UINT32  IoBufferOwnership    :  2;    ///< I/O Buffer Ownership Select: <b>0: HD-A Link</b>; 1: Shared, HD-A Link and I2S Port; 3: I2S Ports
  UINT32  IoBufferVoltage      :  1;    ///< I/O Buffer Voltage Mode Select: <b>0: 3.3V</b>; 1: 1.8V
  UINT32  VcType               :  1;    ///< Virtual Channel Type Select: <b>0: VC0</b>, 1: VC1
  UINT32  HdAudioLinkFrequency :  4;    ///< HDA-Link frequency (PCH_HDAUDIO_LINK_FREQUENCY enum): <b>2: 24MHz</b>, 1: 12MHz, 0: 6MHz
  UINT32  IDispLinkFrequency   :  4;    ///< iDisp-Link frequency (PCH_HDAUDIO_LINK_FREQUENCY enum): <b>4: 96MHz</b>, 3: 48MHz
  UINT32  IDispLinkTmode       :  1;    ///< iDisp-Link T-Mode (PCH_HDAUDIO_IDISP_TMODE enum): <b>0: 2T</b>, 1: 1T
  /**
    Universal Audio Architecture compliance for DSP enabled system:
    <b>0: Not-UAA Compliant (Intel SST driver supported only)</b>,
       1: UAA Compliant (HDA Inbox driver or SST driver supported)
  **/
  UINT32  DspUaaCompliance     :  1;
  UINT32  IDispCodecDisconnect :  1;    ///< iDisplay Audio Codec disconnection, <b>0: Not disconnected, enumerable</b>; 1: Disconnected SDI, not enumerable
  UINT32  RsvdBits0            : 13;    ///< Reserved bits 1
  /**
    Bitmask of supported DSP endpoint configuration exposed via NHLT ACPI table:
  **/
  UINT32  DspEndpointDmic      :  2;    ///< DMIC Select (PCH_HDAUDIO_DMIC_TYPE enum): 0: Disable; 1: 2ch array; <b>2: 4ch array</b>; 3: 1ch array
  UINT32  DspEndpointBluetooth :  1;    ///< Bluetooth enablement: <b>0: Disable</b>; 1: Enable
  UINT32  DspEndpointI2s       :  1;    ///< I2S enablement: <b>0: Disable</b>; 1: Enable
  UINT32  RsvdBits1            : 28;    ///< Reserved bits 2
  /**
    Bitmask of supported DSP features:
    [BIT0] - WoV; [BIT1] - BT Sideband; [BIT2] - Codec VAD; [BIT5] - BT Intel HFP; [BIT6] - BT Intel A2DP
    [BIT7] - DSP based speech pre-processing disabled; [BIT8] - 0: Intel WoV, 1: Windows Voice Activation
    Default is <b>zero</b>.
  **/
  UINT32  DspFeatureMask;
  /**
    @deprecated from revision 2.
    Bitmask of supported DSP Pre/Post-Processing Modules.
    Specific pre/post-processing module bit position must be coherent with the ACPI implementation:
    \_SB.PCI0.HDAS._DSM Function 3: Query Pre/Post Processing Module Support.
    DspPpModuleMask is passed to ACPI as 'ADPM' NVS variable
    Default is <b>zero</b>.
  **/
  UINT32  DspPpModuleMask;
  UINT16  ResetWaitTimer;               ///< <b>(Test)</b> The delay timer after Azalia reset, the value is number of microseconds. Default is <b>600</b>.
  UINT8   Rsvd0;                        ///< Reserved bytes, align to multiple 4
  /**
    Number of the verb table entry defined in VerbTablePtr.
    Each entry points to a verb table which contains HDAUDIO_VERB_TABLE structure and verb command blocks.
  **/
  UINT8   VerbTableEntryNum;
  /**
    Pointer to a verb table array.
    This pointer points to 32bits address, and is only eligible and consumed in post mem phase.
    Each entry points to a verb table which contains HDAUDIO_VERB_TABLE structure and verb command blocks.
    The prototype of this is:
    HDAUDIO_VERB_TABLE **VerbTablePtr;
  **/
  UINT32  VerbTablePtr;
} PCH_HDAUDIO_CONFIG;

#pragma pack (pop)

#endif // _HDAUDIO_CONFIG_H_
