/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#ifndef _PLATFORM_SYS_CTRL_LIB_H_
#define _PLATFORM_SYS_CTRL_LIB_H_

#define PACKAGE_16CORE      0
#define PACKAGE_32CORE      1
#define PACKAGE_RESERVED    2
#define PACKAGE_TYPE_NUM    3

UINT32 PlatformGetPackageType (VOID);

VOID DisplayCpuInfo (VOID);
UINT32 CheckChipIsEc(VOID);

UINTN PlatformGetPll (UINT32 NodeId, UINTN Pll);

#define DJTAG_READ_INVALID_VALUE 0xFFFFFFFF
#define DJTAG_CHAIN_ID_AA        1
#define DJTAG_CHAIN_ID_LLC       4


#define SC_DJTAG_MSTR_EN_OFFSET             0x6800
#define SC_DJTAG_MSTR_START_EN_OFFSET       0x6804
#define SC_DJTAG_SEC_ACC_EN_OFFSET          0x6808
#define SC_DJTAG_DEBUG_MODULE_SEL_OFFSET    0x680C
#define SC_DJTAG_MSTR_WR_OFFSET             0x6810
#define SC_DJTAG_CHAIN_UNIT_CFG_EN_OFFSET   0x6814
#define SC_DJTAG_MSTR_ADDR_OFFSET           0x6818
#define SC_DJTAG_MSTR_DATA_OFFSET           0x681C
#define SC_DJTAG_TMOUT_OFFSET               0x6820
#define SC_TDRE_OP_ADDR_OFFSET              0x6824
#define SC_TDRE_WDATA_OFFSET                0x6828
#define SC_TDRE_REPAIR_EN_OFFSET            0x682C
#define SC_DJTAG_RD_DATA0_OFFSET            0xE800
#define SC_TDRE_RDATA0_OFFSET               0xE830


UINTN PlatformGetI2cBase(UINT32 Socket,UINT8 Port);

VOID PlatformAddressMapCleanUp (VOID);
VOID PlatformDisableDdrWindow (VOID);

VOID PlatformEnableArchTimer (VOID);

EFI_STATUS
DawFindFreeWindow (UINTN Socket, UINTN *DawIndex);

VOID DawSetWindow (UINTN Socket, UINTN WindowIndex, UINT32 Value);

VOID DJTAG_TDRE_WRITE(UINT32 Offset, UINT32 Value, UINT32 ChainID, UINT32 NodeId, BOOLEAN Repair);

UINT32 DJTAG_TDRE_READ(UINT32 Offset, UINT32 ChainID, UINT32 NodeId, BOOLEAN Repair);

VOID RemoveRoceReset(VOID);

UINTN PlatformGetDdrChannel (VOID);

VOID ITSCONFIG (VOID);

VOID MN_CONFIG (VOID);

VOID SmmuConfigForOS (VOID);
VOID SmmuConfigForBios (VOID);

VOID StartUpBSP (VOID);

VOID LlcCleanInvalidate (VOID);

UINTN PlatformGetCpuFreq (UINT8 Socket);
VOID ClearInterruptStatus(VOID);

UINTN PlatformGetCoreCount (VOID);
VOID DAWConfigEn(UINT32 socket);

VOID DResetUsb ();
UINT32 PlatformGetEhciBase ();
UINT32 PlatformGetOhciBase ();
VOID PlatformPllInit();
// PLL initialization for super IO clusters.
VOID SiclPllInit(UINT32 SclId);
VOID PlatformDeviceDReset();
VOID PlatformGicdInit();
VOID PlatformLpcInit();
// Synchronize architecture timer counter between different super computing
// clusters.
VOID PlatformArchTimerSynchronize(VOID);
VOID PlatformEventBroadcastConfig(VOID);
UINTN GetDjtagRegBase(UINT32 NodeId);
VOID LlcCleanInvalidateAsm(VOID);
VOID PlatformMdioInit(VOID);
VOID DisableClusterClock(UINTN CpuClusterBase);
VOID EnableClusterClock(UINTN CpuClusterBase);
VOID DisableSocketClock (UINT8 Skt);

EFI_STATUS EFIAPI HandleI2CException (UINT32 Socket, UINT32 Port);
EFI_STATUS EFIAPI HandleI2CExceptionBySocket (UINT32 Socket);

#endif
