/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/


#ifndef _OEM_MISC_LIB_H_
#define _OEM_MISC_LIB_H_

#include <Uefi.h>

#include <PlatformArch.h>
#include <Library/I2CLib.h>

#define HCCS_PLL_VALUE_2600  0x52240681
#define HCCS_PLL_VALUE_2800  0x52240701
#define HCCS_PLL_VALUE_3000  0x52240781

typedef enum {
  EmHilink0Hccs1X8 = 0,
  EmHilink0Pcie1X8 = 2,
  EmHilink0Pcie1X4Pcie2X4 = 3,
  EmHilink0Sas2X8 = 4,
  EmHilink0Hccs1X8Width16,
  EmHilink0Hccs1X8Width32,
  EmHilink0Hccs1X8Speed5G,
} HILINK0_MODE_TYPE;

typedef enum {
  EmHilink1Sas2X1 = 0,
  EmHilink1Hccs0X8 = 1,
  EmHilink1Pcie0X8 = 2,
  EmHilink1Hccs0X8Width16,
  EmHilink1Hccs0X8Width32,
  EmHilink1Hccs0X8Speed5G,
} HILINK1_MODE_TYPE;

typedef enum {
  EmHilink2Pcie2X8 = 0,
  EmHilink2Hccs2X8 = 1,
  EmHilink2Sas0X8 = 2,
  EmHilink2Hccs2X8Width16,
  EmHilink2Hccs2X8Width32,
  EmHilink2Hccs2X8Speed5G,
} HILINK2_MODE_TYPE;

typedef enum {
  EmHilink5Pcie3X4 = 0,
  EmHilink5Pcie2X2Pcie3X2 = 1,
  EmHilink5Sas1X4 = 2,
} HILINK5_MODE_TYPE;


typedef struct {
  HILINK0_MODE_TYPE Hilink0Mode;
  HILINK1_MODE_TYPE Hilink1Mode;
  HILINK2_MODE_TYPE Hilink2Mode;
  UINT32 Hilink3Mode;
  UINT32 Hilink4Mode;
  HILINK5_MODE_TYPE Hilink5Mode;
  UINT32 Hilink6Mode;
  UINT32 UseSsc;
} SERDES_PARAM;

#define SERDES_INVALID_MACRO_ID  0xFFFFFFFF
#define SERDES_INVALID_LANE_NUM  0xFFFFFFFF
#define SERDES_INVALID_RATE_MODE  0xFFFFFFFF

typedef struct {
  UINT32 MacroId;
  UINT32 DsNum;
  UINT32 DsCfg;
} SERDES_POLARITY_INVERT;


#define PCIEDEVICE_REPORT_MAX      8
#define MAX_PROCESSOR_SOCKETS      MAX_SOCKET
#define MAX_MEMORY_CHANNELS        MAX_CHANNEL
#define MAX_DIMM_PER_CHANNEL       MAX_DIMM

typedef struct _REPORT_PCIEDIDVID2BMC{
    UINTN   Bus;
    UINTN   Device;
    UINTN   Function;
    UINTN   Slot;
}REPORT_PCIEDIDVID2BMC;
extern REPORT_PCIEDIDVID2BMC PcieDeviceToReport[PCIEDEVICE_REPORT_MAX];
extern VOID GetPciDidVid (REPORT_PCIEDIDVID2BMC *Report);

BOOLEAN OemIsSocketPresent (UINTN Socket);
VOID CoreSelectBoot(VOID);
VOID OemPcieResetAndOffReset(void);
extern I2C_DEVICE gRtcDevice;

UINTN OemGetSocketNumber(VOID);
UINTN OemGetDdrChannel (VOID);
UINTN OemGetDimmSlot(UINTN Socket, UINTN Channel);

BOOLEAN OemIsMpBoot();
UINT32 OemIsWarmBoot();

VOID OemBiosSwitch(UINT32 Master);
BOOLEAN OemIsNeedDisableExpanderBuffer(VOID);

extern EFI_STRING_ID gDimmToDevLocator[MAX_SOCKET][MAX_CHANNEL][MAX_DIMM];
EFI_HII_HANDLE EFIAPI OemGetPackages ();
UINTN OemGetCpuFreq (UINT8 Socket);

UINTN
OemGetHccsFreq (
  VOID
  );

EFI_STATUS
OemGetSerdesParam (
  SERDES_PARAM *ParamA,
  SERDES_PARAM *ParamB,
  UINT32       SocketId
  );

#endif
