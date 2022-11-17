/** @file
*
*  Copyright (c) 2018, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/


#include <Uefi.h>
#include <Library/ArmLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/OemAddressMapLib.h>
#include <Library/OemMiscLib.h>
#include <Library/PcdLib.h>
#include <Library/PlatformSysCtrlLib.h>
#include <PiPei.h>

#define PERI_SUBCTRL_BASE                               (0x40000000)
#define MDIO_SUBCTRL_BASE                               (0x60000000)
#define PCIE2_SUBCTRL_BASE                              (0xA0000000)
#define PCIE0_SUBCTRL_BASE                              (0xB0000000)
#define ALG_BASE                                        (0xD0000000)

#define SC_BROADCAST_EN_REG                             (0x16220)
#define SC_BROADCAST_SCL1_ADDR0_REG                     (0x16230)
#define SC_BROADCAST_SCL1_ADDR1_REG                     (0x16234)
#define SC_BROADCAST_SCL2_ADDR0_REG                     (0x16238)
#define SC_BROADCAST_SCL2_ADDR1_REG                     (0x1623C)
#define SC_BROADCAST_SCL3_ADDR0_REG                     (0x16240)
#define SC_BROADCAST_SCL3_ADDR1_REG                     (0x16244)
#define PCIE_SUBCTRL_SC_DISP_DAW_EN_REG                 (0x1000)
#define PCIE_SUBCTRL_SC_DISPATCH_DAW_ARRAY3_REG         (0x1010)
#define PCIE_SUBCTRL_SC_DISPATCH_DAW_ARRAY4_REG         (0x1014)
#define PCIE_SUBCTRL_SC_DISPATCH_DAW_ARRAY5_REG         (0x1018)
#define PCIE_SUBCTRL_SC_DISPATCH_DAW_ARRAY6_REG         (0x101C)
#define PCIE_SUBCTRL_SC_REMAP_CTRL_REG                  (0x1200)
#define SC_ITS_M3_INT_MUX_SEL_REG                       (0x21F0)
#define SC_TM_CLKEN0_REG                                (0x2050)

#define SC_TM_CLKEN0_REG_VALUE                          (0x3)
#define SC_BROADCAST_EN_REG_VALUE                       (0x7)
#define SC_BROADCAST_SCLx_ADDRx_REG_VALUE0              (0x0)
#define SC_BROADCAST_SCLx_ADDRx_REG_VALUE1              (0x40016260)
#define SC_BROADCAST_SCLx_ADDRx_REG_VALUE2              (0x60016260)
#define SC_BROADCAST_SCLx_ADDRx_REG_VALUE3              (0x400)
#define SC_ITS_M3_INT_MUX_SEL_REG_VALUE                 (0x7)
#define PCIE_SUBCTRL_SC_REMAP_CTRL_REG_VALUE0           (0x0)
#define PCIE_SUBCTRL_SC_DISP_DAW_EN_REG_VALUE0          (0x27)
#define PCIE_SUBCTRL_SC_DISP_DAW_EN_REG_VALUE1          (0x2F)
#define PCIE_SUBCTRL_SC_DISP_DAW_EN_REG_VALUE2          (0x77)
#define PCIE_SUBCTRL_SC_DISPATCH_DAW_ARRAY3_REG_VALUE0  (0x178033)
#define PCIE_SUBCTRL_SC_DISPATCH_DAW_ARRAY4_REG_VALUE0  (0x17003c)
#define PCIE_SUBCTRL_SC_DISPATCH_DAW_ARRAY5_REG_VALUE0  (0x15003d)
#define PCIE_SUBCTRL_SC_DISPATCH_DAW_ARRAY5_REG_VALUE1  (0x170035)
#define PCIE_SUBCTRL_SC_DISPATCH_DAW_ARRAY6_REG_VALUE0  (0x16003e)

STATIC
VOID
QResetAp (
  VOID
  )
{
  MmioWrite64 (FixedPcdGet64 (PcdMailBoxAddress), 0x0);
  (VOID)WriteBackInvalidateDataCacheRange (
          (VOID *)FixedPcdGet64 (PcdMailBoxAddress),
          sizeof (UINT64)
          );

  //SCCL A
  if (!PcdGet64 (PcdTrustedFirmwareEnable)) {
    StartUpBSP ();
  }
}


EFI_STATUS
EFIAPI
EarlyConfigEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  DEBUG ((DEBUG_INFO,"SMMU CONFIG........."));
  (VOID)SmmuConfigForBios ();
  DEBUG ((DEBUG_INFO,"Done\n"));

  DEBUG ((DEBUG_INFO,"AP CONFIG........."));
  (VOID)QResetAp ();
  DEBUG ((DEBUG_INFO,"Done\n"));

  DEBUG ((DEBUG_INFO,"MN CONFIG........."));
  (VOID)MN_CONFIG ();
  DEBUG ((DEBUG_INFO,"Done\n"));

  return EFI_SUCCESS;
}

