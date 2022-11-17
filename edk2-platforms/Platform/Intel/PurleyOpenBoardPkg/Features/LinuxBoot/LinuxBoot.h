/** @file
Copyright (c) 2021, American Megatrends International LLC. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __LINUX_BOOT_PRIVATE__H__
#define __LINUX_BOOT_PRIVATE__H__
#ifdef __cplusplus
extern "C" {
#endif  //  #ifdef __cplusplus

//---------------------------------------------------------------------------

//#include <Token.h>

#define BOOTSIG			0x1FE
#define SETUP_HDR		0x53726448	/* 0x53726448 == "HdrS" */

#define E820_RAM		1
#define E820_RESERVED		2
#define E820_ACPI		3
#define E820_NVS		4
#define E820_UNUSABLE		5

#pragma pack(1)

struct SetupHeader {
  UINT8  SetupSecs;   // Sectors for setup code
  UINT16 Rootflags;
  UINT32 SysSize;
  UINT16 RamSize;
  UINT16 VideoMode;
  UINT16 RootDev;
  UINT16 Signature;   // Boot signature
  UINT16 Jump;
  UINT32 Header;
  UINT16 Version;
  UINT16 SuSwitch;
  UINT16 SetupSeg;
  UINT16 StartSys;
  UINT16 KernelVer;
  UINT8  LoaderId;
  UINT8  LoadFlags;
  UINT16 MoveSize;
  UINT32 Code32Start;    // Start of code loaded high
  UINT32 RamDiskStart;   // Start of initial ramdisk
  UINT32 RamDiskLen;     // Length of initial ramdisk
  UINT32 BootSectkludge;
  UINT16 HeapEnd;
  UINT8  ExtLoaderVer;  // Extended boot loader version
  UINT8  ExtLoaderType; // Extended boot loader ID
  UINT32 CmdLinePtr;   // 32-bit pointer to the kernel command line
  UINT32 RamDiskMax;    // Highest legal initrd address
  UINT32 KernelAlignment; // Physical addr alignment required for kernel
  UINT8  RelocatableKernel; // Whether kernel is relocatable or not
  UINT8  MinAlignment;
  UINT16 XloadFlags;
  UINT32 CmdlineSize;
  UINT32 HardwareSubarch;
  UINT64 HardwareSubarchData;
  UINT32 PayloadOffset;
  UINT32 PayloadLength;
  UINT64 SetupData;
  UINT64 PrefAddress;
  UINT32 InitSize;
  UINT32 HandoverOffset;
};

struct EfiInfo {
  UINT32 EfiLoaderSignature;
  UINT32 EfiSystab;
  UINT32 EfiMemdescSize;
  UINT32 EfiMemdescVersion;
  UINT32 EfiMemMap;
  UINT32 EfiMemMapSize;
  UINT32 EfiSystabHi;
  UINT32 EfiMemMapHi;
};

struct E820Entry {
  UINT64 Addr;        // start of memory segment
  UINT64 Size;        // size of memory segment
  UINT32 Type;        // type of memory segment
};

struct ScreenInfo {
  UINT8  OrigX;           // 0x00
  UINT8  OrigY;           // 0x01
  UINT16 ExtMemK;        // 0x02
  UINT16 OrigVideoPage;  // 0x04
  UINT8  OrigVideoMode;  // 0x06
  UINT8  OrigVideoCols;  // 0x07
  UINT8  Flags;            // 0x08
  UINT8  Unused2;          // 0x09
  UINT16 OrigVideoEgaBx;// 0x0a
  UINT16 Unused3;          // 0x0c
  UINT8  OrigVideoLines; // 0x0e
  UINT8  OrigVideoIsVGA; // 0x0f
  UINT16 OrigVideoPoints;// 0x10

  // VESA graphic mode -- linear frame buffer
  UINT16 LfbWidth;        // 0x12
  UINT16 LfbHeight;       // 0x14
  UINT16 LfbDepth;        // 0x16
  UINT32 LfbBase;         // 0x18
  UINT32 LfbSize;         // 0x1c
  UINT16 ClMagic, ClOffset; // 0x20
  UINT16 LfbLineLength;       // 0x24
  UINT8  RedSize;         // 0x26
  UINT8  RedPos;          // 0x27
  UINT8  GreenSize;       // 0x28
  UINT8  GreenPos;        // 0x29
  UINT8  BlueSize;        // 0x2a
  UINT8  BluePos;         // 0x2b
  UINT8  RsvdSize;        // 0x2c
  UINT8  RsvdPos;         // 0x2d
  UINT16 VesaPmSeg;       // 0x2e
  UINT16 VesaPmOff;       // 0x30
  UINT16 Pages;            // 0x32
  UINT16 VesaAttributes;  // 0x34
  UINT32 Capabilities;     // 0x36
  UINT8  Reserved[6];     // 0x3a
};

struct BootParams {
  struct ScreenInfo ScreenInfo;
  UINT8  ApmBiosInfo[0x14];
  UINT8  Pad2[4];
  UINT64 TbootAddr;
  UINT8  IstInfo[0x10];
  UINT8  Pad3[16];
  UINT8  Hd0Info[16];
  UINT8  Hd1Info[16];
  UINT8  SysDescTable[0x10];
  UINT8  OlpcOfwHeader[0x10];
  UINT8  Pad4[128];
  UINT8  EdidInfo[0x80];
  struct EfiInfo EfiInfo;
  UINT32 AltMemK;
  UINT32 Scratch;
  UINT8 E820Entries;
  UINT8 EddBufEntries;
  UINT8 EddMbrSigBufEntries;
  UINT8 Pad6[6];
  struct SetupHeader Hdr;
  UINT8 Pad7[0x290-0x1f1-sizeof(struct SetupHeader)];
  UINT32 EddMbrSigBuffer[16];
  struct E820Entry E820Map[128];
  UINT8  Pad8[48];
  UINT8  EddBuf[0x1ec];
  UINT8  Pad9[276];
};
#pragma pack ()

//---------------------------------------------------------------------------

#ifndef MIN
#define MIN(x,y)                ((x) < (y) ? (x) : (y))
#endif  //  #ifndef MIN

#define KERNEL_SETUP_SIZE       16384

//---------------------------------------------------------------------------

///
/// Function prototypes from Bds module
///
VOID ConnectEverything();
VOID RecoverTheMemoryAbove4Gb();
VOID SignalAllDriversConnectedEvent();
VOID SignalProtocolEvent(IN EFI_GUID *ProtocolGuid);

#if LINUXBOOT_SIGNAL_EXITPMAUTH == 1
VOID SignalExitPmAuthProtocolEvent(VOID);
#endif  //  #if LINUXBOOT_SIGNAL_EXITPMAUTH == 1

typedef VOID (BDS_CONTROL_FLOW_FUNCTION)();

//---------------------------------------------------------------------------

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif  //  #ifdef __cplusplus
#endif  //  #ifndef __LINUX_BOOT_PRIVATE__H__
