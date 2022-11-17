/** @file
  HDAUDIO policy

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _HDAUDIO_CONFIG_H_
#define _HDAUDIO_CONFIG_H_

#include <ConfigBlock.h>
#include <PchHda.h>
#include <Uefi.h>

#define HDAUDIO_PREMEM_CONFIG_REVISION 1
#define HDAUDIO_CONFIG_REVISION 2
#define HDAUDIO_DXE_CONFIG_REVISION 2

extern EFI_GUID gHdAudioPreMemConfigGuid;
extern EFI_GUID gHdAudioConfigGuid;
extern EFI_GUID gHdAudioDxeConfigGuid;

#pragma pack (push,1)

///
/// The PCH_HDAUDIO_CONFIG block describes the expected configuration of the Intel HD Audio feature.
///

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

/**
  This structure contains the policies which are related to HD Audio device (cAVS).

  <b>Revision 1:</b>
  - Inital version.
  <b>Revision 2:</b>
  - Move DspEndpointDmic, DspEndpointBluetooth, DspEndpointI2s and DspFeatureMask to PCH_HDAUDIO_DXE_CONFIG
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  UINT32  DspEnable             :  1;    ///< DSP enablement: 0: Disable; <b>1: Enable</b>
  UINT32  Pme                   :  1;    ///< Azalia wake-on-ring, <b>0: Disable</b>; 1: Enable
  UINT32  VcType                :  1;    ///< Virtual Channel Type Select: <b>0: VC0</b>, 1: VC1
  UINT32  HdAudioLinkFrequency  :  4;    ///< HDA-Link frequency (PCH_HDAUDIO_LINK_FREQUENCY enum): <b>2: 24MHz</b>, 1: 12MHz, 0: 6MHz
  UINT32  IDispLinkFrequency    :  4;    ///< iDisp-Link frequency (PCH_HDAUDIO_LINK_FREQUENCY enum): <b>4: 96MHz</b>, 3: 48MHz
  UINT32  IDispLinkTmode        :  3;    ///< iDisp-Link T-Mode (PCH_HDAUDIO_IDISP_TMODE enum): <b>0: 2T</b>, 1: 1T, 2: 4T, 3: 8T, 4: 16T
  /**
    Universal Audio Architecture compliance for DSP enabled system:
    <b>0: Not-UAA Compliant (Intel SST driver supported only)</b>,
       1: UAA Compliant (HDA Inbox driver or SST driver supported)
  **/
  UINT32  DspUaaCompliance      :  1;
  UINT32  IDispCodecDisconnect  :  1;    ///< iDisplay Audio Codec disconnection, <b>0: Not disconnected, enumerable</b>; 1: Disconnected SDI, not enumerable
  UINT32  CodecSxWakeCapability :  1;    ///< Capability to detect wake initiated by a codec in Sx (eg by modem codec), <b>0: Disable</b>; 1: Enable
  /**
    Audio Link Mode configuration bitmask.
    Allows to configure enablement of the following interfaces: HDA-Link, DMIC, SSP, SoundWire.
  **/
  UINT32  AudioLinkHda          :  1;    ///< HDA-Link enablement: 0: Disable; <b>1: Enable</b>. Muxed with SSP0/SSP1/SNDW1
  UINT32  AudioLinkDmic0        :  1;    ///< DMIC0 link enablement: 0: Disable; <b>1: Enable</b>. Muxed with SNDW4
  UINT32  AudioLinkDmic1        :  1;    ///< DMIC1 link enablement: 0: Disable; <b>1: Enable</b>. Muxed with SNDW3
  UINT32  AudioLinkSsp0         :  1;    ///< I2S/SSP0 link enablement: <b>0: Disable</b>; 1: Enable. Muxed with HDA SDI0
  UINT32  AudioLinkSsp1         :  1;    ///< I2S/SSP1 link enablement: <b>0: Disable</b>; 1: Enable. Muxed with HDA SDI1/SNDW2
  /**
    I2S/SSP2 link enablement: <b>0: Disable</b>; 1: Enable.
    @note Since the I2S/SSP2 pin set contains pads which are also used for CNVi purpose, enabling AudioLinkSsp2
    is exclusive with CNVi is present.
  **/
  UINT32  AudioLinkSsp2         :  1;
  UINT32  AudioLinkSndw1        :  1;    ///< SoundWire1 link enablement: <b>0: Disable</b>; 1: Enable. Muxed with HDA
  UINT32  AudioLinkSndw2        :  1;    ///< SoundWire2 link enablement: <b>0: Disable</b>; 1: Enable. Muxed with SSP1
  UINT32  AudioLinkSndw3        :  1;    ///< SoundWire3 link enablement: <b>0: Disable</b>; 1: Enable. Muxed with DMIC1
  UINT32  AudioLinkSndw4        :  1;    ///< SoundWire4 link enablement: <b>0: Disable</b>; 1: Enable. Muxed with DMIC0
  /**
    Soundwire Clock Buffer GPIO RCOMP adjustments based on bus topology:
    <b>0: non-ACT</b> - 50 Ohm driver impedance when bus topology does not have the external AC termination;
       1: ACT - 8 Ohm driver impedance when bus topology has the external AC termination.
  **/
  UINT32  SndwBufferRcomp       :  1;
  UINT32  RsvdBits0             :  4;    ///< Reserved bits 0
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

/**
  This structure contains the premem policies which are related to HD Audio device (cAVS).

  <b>Revision 1:</b>
  - Inital version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;          ///< Config Block Header
  UINT32  Enable                : 1;     ///< Intel HD Audio (Azalia) enablement: 0: Disable, <b>1: Enable</b>
  UINT32  RsvdBits              : 31;    ///< Reserved bits 0
} PCH_HDAUDIO_PREMEM_CONFIG;

/**
  This structure contains the DXE policies which are related to HD Audio device (cAVS).

  <b>Revision 1:</b>
  - Inital version.
  <b>Revision 2:</b>
  - Add NhltDefaultFlow option for disabling NHLT flow from Si code.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;          ///< Config Block Header
  /**
    AudioDSP/iSST endpoints configuration exposed via NHLT ACPI table:
  **/
  UINT32  DspEndpointDmic       :  2;    ///< DMIC Select (PCH_HDAUDIO_DMIC_TYPE enum): 0: Disable; 1: 2ch array; <b>2: 4ch array</b>; 3: 1ch array
  UINT32  DspEndpointBluetooth  :  1;    ///< Bluetooth enablement: <b>0: Disable</b>; 1: Enable
  UINT32  DspEndpointI2s        :  1;    ///< I2S enablement: <b>0: Disable</b>; 1: Enable
  UINT32  NhltDefaultFlow       :  1;    ///< Default Nhlt flow: 0: Disable, <b>1: Enable</b>
  UINT32  RsvdBits1             : 27;    ///< Reserved bits 1
  /**
    Bitmask of supported DSP features:
    [BIT0] - WoV; [BIT1] - BT Sideband; [BIT2] - Codec VAD; [BIT5] - BT Intel HFP; [BIT6] - BT Intel A2DP
    [BIT7] - DSP based speech pre-processing disabled; [BIT8] - 0: Intel WoV, 1: Windows Voice Activation
    Default is <b>zero</b>.
  **/
  UINT32  DspFeatureMask;
} PCH_HDAUDIO_DXE_CONFIG;

#pragma pack (pop)

#endif // _HDAUDIO_CONFIG_H_
