/** @file

Copyright (c) 2017 - 2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <SaPolicyCommon.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PciLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>

#include <Library/PeiSaPolicyLib.h>
#include <Library/BoardInitLib.h>
#include <PchAccess.h>
#include <Library/GpioNativeLib.h>
#include <Library/GpioLib.h>
#include <GpioPinsSklLp.h>
#include <GpioPinsSklH.h>
#include <Library/GpioExpanderLib.h>
#include <SioRegs.h>
#include <Library/PchPcrLib.h>
#include <Library/SiliconInitLib.h>
#include <Library/PchResetLib.h>

#include "PeiKabylakeRvp3InitLib.h"

#include <ConfigBlock.h>
#include <ConfigBlock/MemoryConfig.h>

//
// Reference RCOMP resistors on motherboard - for SKL RVP1
//
GLOBAL_REMOVE_IF_UNREFERENCED const UINT16 RcompResistorSklRvp1[SA_MRC_MAX_RCOMP] = { 200, 81, 162 };
//
// RCOMP target values for RdOdt, WrDS, WrDSCmd, WrDSCtl, WrDSClk - for SKL RVP1
//
GLOBAL_REMOVE_IF_UNREFERENCED const UINT16 RcompTargetSklRvp1[SA_MRC_MAX_RCOMP_TARGETS] = { 100, 40, 40, 23, 40 };

/**
  SkylaeA0Rvp3 board configuration init function for PEI pre-memory phase.

  PEI_BOARD_CONFIG_PCD_INIT

  @param  Content  pointer to the buffer contain init information for board init.

  @retval EFI_SUCCESS             The function completed successfully.
  @retval EFI_INVALID_PARAMETER   The parameter is NULL.
**/
EFI_STATUS
EFIAPI
KabylakeRvp3InitPreMem (
  VOID
  )
{
  PcdSet32S (PcdPcie0WakeGpioNo, 0);
  PcdSet8S  (PcdPcie0HoldRstExpanderNo, 0);
  PcdSet32S (PcdPcie0HoldRstGpioNo, 8);
  PcdSetBoolS (PcdPcie0HoldRstActive, TRUE);
  PcdSet8S  (PcdPcie0PwrEnableExpanderNo, 0);
  PcdSet32S (PcdPcie0PwrEnableGpioNo, 16);
  PcdSetBoolS (PcdPcie0PwrEnableActive, FALSE);

  //
  // HSIO PTSS Table
  //
  PcdSet32S (PcdSpecificLpHsioPtssTable1,     (UINTN) PchLpHsioPtss_Bx_KabylakeRvp3);
  PcdSet16S (PcdSpecificLpHsioPtssTable1Size, (UINTN) PchLpHsioPtss_Bx_KabylakeRvp3_Size);
  PcdSet32S (PcdSpecificLpHsioPtssTable2,     (UINTN) PchLpHsioPtss_Cx_KabylakeRvp3);
  PcdSet16S (PcdSpecificLpHsioPtssTable2Size, (UINTN) PchLpHsioPtss_Cx_KabylakeRvp3_Size);

  //
  // DRAM related definition
  //
  PcdSet8S (PcdSaMiscUserBd, 5);

  PcdSet32S (PcdMrcDqByteMap, (UINTN) mDqByteMapSklRvp3);
  PcdSet16S (PcdMrcDqByteMapSize, sizeof (mDqByteMapSklRvp3));
  PcdSet32S (PcdMrcDqsMapCpu2Dram, (UINTN) mDqsMapCpu2DramSklRvp3);
  PcdSet16S (PcdMrcDqsMapCpu2DramSize, sizeof (mDqsMapCpu2DramSklRvp3));
  PcdSet32S (PcdMrcRcompResistor, (UINTN) RcompResistorSklRvp1);
  PcdSet32S (PcdMrcRcompTarget, (UINTN) RcompTargetSklRvp1);
  //
  // Example policy for DIMM slots implementation boards:
  // 1. Assign Smbus address of DIMMs and SpdData will be updated later
  //    by reading from DIMM SPD.
  // 2. No need to apply hardcoded SpdData buffers here for such board.
  //   Example:
  //   PcdMrcSpdAddressTable0 = 0xA0
  //   PcdMrcSpdAddressTable1 = 0xA2
  //   PcdMrcSpdAddressTable2 = 0xA4
  //   PcdMrcSpdAddressTable3 = 0xA6
  //   PcdMrcSpdData = 0
  //   PcdMrcSpdDataSize = 0
  //
  // Kabylake RVP3 has 8GB Memory down implementation withouit SPD,
  // So assign all SpdAddress to 0 and apply static SpdData buffers:
  //   PcdMrcSpdAddressTable0 = 0
  //   PcdMrcSpdAddressTable1 = 0
  //   PcdMrcSpdAddressTable2 = 0
  //   PcdMrcSpdAddressTable3 = 0
  //   PcdMrcSpdData = static data buffer
  //   PcdMrcSpdDataSize = sizeof (static data buffer)
  //
  PcdSet8S (PcdMrcSpdAddressTable0, 0);
  PcdSet8S (PcdMrcSpdAddressTable1, 0);
  PcdSet8S (PcdMrcSpdAddressTable2, 0);
  PcdSet8S (PcdMrcSpdAddressTable3, 0);
  PcdSet32S (PcdMrcSpdData, (UINTN) mSkylakeRvp3Spd110);
  PcdSet16S (PcdMrcSpdDataSize, mSkylakeRvp3Spd110Size);

  PcdSetBoolS (PcdIoExpanderPresent, TRUE);

  return EFI_SUCCESS;
}

/**
  SkylaeA0Rvp3 board configuration init function for PEI pre-memory phase.

  PEI_BOARD_CONFIG_PCD_INIT

  @param  Content  pointer to the buffer contain init information for board init.

  @retval EFI_SUCCESS             The function completed successfully.
  @retval EFI_INVALID_PARAMETER   The parameter is NULL.
**/
EFI_STATUS
EFIAPI
SkylakeRvp3InitPreMem (
  VOID
  )
{
  PcdSet32S (PcdPcie0WakeGpioNo, 0);
  PcdSet8S  (PcdPcie0HoldRstExpanderNo, 0);
  PcdSet32S (PcdPcie0HoldRstGpioNo, 8);
  PcdSetBoolS (PcdPcie0HoldRstActive, TRUE);
  PcdSet8S  (PcdPcie0PwrEnableExpanderNo, 0);
  PcdSet32S (PcdPcie0PwrEnableGpioNo, 16);
  PcdSetBoolS (PcdPcie0PwrEnableActive, FALSE);

  //
  // HSIO PTSS Table
  //
  PcdSet32S (PcdSpecificLpHsioPtssTable1,     (UINTN) PchLpHsioPtss_Bx_KabylakeRvp3);
  PcdSet16S (PcdSpecificLpHsioPtssTable1Size, (UINTN) PchLpHsioPtss_Bx_KabylakeRvp3_Size);
  PcdSet32S (PcdSpecificLpHsioPtssTable2,     (UINTN) PchLpHsioPtss_Cx_KabylakeRvp3);
  PcdSet16S (PcdSpecificLpHsioPtssTable2Size, (UINTN) PchLpHsioPtss_Cx_KabylakeRvp3_Size);

  //
  // DRAM related definition
  //
  PcdSet8S (PcdSaMiscUserBd, 5);

  PcdSet32S (PcdMrcDqByteMap, (UINTN) mDqByteMapSklRvp3);
  PcdSet16S (PcdMrcDqByteMapSize, sizeof (mDqByteMapSklRvp3));
  PcdSet32S (PcdMrcDqsMapCpu2Dram, (UINTN) mDqsMapCpu2DramSklRvp3);
  PcdSet16S (PcdMrcDqsMapCpu2DramSize, sizeof (mDqsMapCpu2DramSklRvp3));
  PcdSet32S (PcdMrcRcompResistor, (UINTN) RcompResistorSklRvp1);
  PcdSet32S (PcdMrcRcompTarget, (UINTN) RcompTargetSklRvp1);
  //
  // Example policy for DIMM slots implementation boards:
  // 1. Assign Smbus address of DIMMs and SpdData will be updated later
  //    by reading from DIMM SPD.
  // 2. No need to apply hardcoded SpdData buffers here for such board.
  //   Example:
  //   PcdMrcSpdAddressTable0 = 0xA0
  //   PcdMrcSpdAddressTable1 = 0xA2
  //   PcdMrcSpdAddressTable2 = 0xA4
  //   PcdMrcSpdAddressTable3 = 0xA6
  //   PcdMrcSpdData = 0
  //   PcdMrcSpdDataSize = 0
  //
  // Skylake RVP3 has 4GB Memory down implementation withouit SPD,
  // So assign all SpdAddress to 0 and apply static SpdData buffers:
  //   PcdMrcSpdAddressTable0 = 0
  //   PcdMrcSpdAddressTable1 = 0
  //   PcdMrcSpdAddressTable2 = 0
  //   PcdMrcSpdAddressTable3 = 0
  //   PcdMrcSpdData = static data buffer
  //   PcdMrcSpdDataSize = sizeof (static data buffer)
  //
  PcdSet8S (PcdMrcSpdAddressTable0, 0);
  PcdSet8S (PcdMrcSpdAddressTable1, 0);
  PcdSet8S (PcdMrcSpdAddressTable2, 0);
  PcdSet8S (PcdMrcSpdAddressTable3, 0);
  PcdSet32S (PcdMrcSpdData, (UINTN) mSkylakeRvp3Spd);
  PcdSet16S (PcdMrcSpdDataSize, mSkylakeRvp3SpdSize);

  PcdSetBoolS (PcdIoExpanderPresent, TRUE);

  return EFI_SUCCESS;
}

#define SIO_RUNTIME_REG_BASE_ADDRESS                          0x0680

/**
  Configures GPIO.

  @param[in]  GpioTable       Point to Platform Gpio table
  @param[in]  GpioTableCount  Number of Gpio table entries

**/
VOID
ConfigureGpio (
  IN GPIO_INIT_CONFIG                 *GpioDefinition,
  IN UINT16                           GpioTableCount
  )
{
  EFI_STATUS          Status;

  DEBUG ((DEBUG_INFO, "ConfigureGpio() Start\n"));

  Status = GpioConfigurePads (GpioTableCount, GpioDefinition);

  DEBUG ((DEBUG_INFO, "ConfigureGpio() End\n"));
}

/**
  Configure GPIO Before Memory is not ready.

**/
VOID
GpioInitPreMem (
  VOID
  )
{
  // ConfigureGpio ();
}

/**
  Configure Super IO.

**/
VOID
SioInit (
  VOID
  )
{
  //
  // Program and Enable Default Super IO Configuration Port Addresses and range
  //
  PchLpcGenIoRangeSet (PcdGet16 (PcdLpcSioConfigDefaultPort) & (~0xF), 0x10);

  //
  // 128 Byte Boundary and SIO Runtime Register Range is 0x0 to 0xF;
  //
  PchLpcGenIoRangeSet (SIO_RUNTIME_REG_BASE_ADDRESS  & (~0x7F), 0x10);

  return;
}

/**
  Configues the IC2 Controller on which GPIO Expander Communicates.
  This Function is to enable the I2CGPIOExapanderLib to programm the Gpios
  Complete intilization will be done in later Stage

**/
VOID
EFIAPI
I2CGpioExpanderInitPreMem(
  VOID
  )
{
  ConfigureSerialIoController (PchSerialIoIndexI2C4, PchSerialIoAcpiHidden);
  SerialIoI2cGpioInit (PchSerialIoIndexI2C4, PchSerialIoAcpiHidden, PchSerialIoIs33V);
}

/**
  Configure GPIO and SIO before memory ready.

  @retval  EFI_SUCCESS   Operation success.
**/
EFI_STATUS
EFIAPI
KabylakeRvp3BoardInitBeforeMemoryInit (
  VOID
  )
{
  EFI_STATUS    Status;

  if (LibPcdGetSku () == BoardIdKabyLakeYLpddr3Rvp3) {
    KabylakeRvp3InitPreMem ();
  } else if (LibPcdGetSku () == BoardIdSkylakeRvp3) {
    SkylakeRvp3InitPreMem ();
  }

  //
  // Configures the I2CGpioExpander
  //
  if (PcdGetBool (PcdIoExpanderPresent)) {
    I2CGpioExpanderInitPreMem();
  }

  GpioInitPreMem ();
  SioInit ();

  ///
  /// Do basic PCH init
  ///
  SiliconInit ();

  //
  // Install PCH RESET PPI and EFI RESET2 PeiService
  //
  Status = PchInitializeReset ();
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
KabylakeRvp3BoardDebugInit (
  VOID
  )
{
  ///
  /// Do Early PCH init
  ///
  EarlySiliconInit ();
  return EFI_SUCCESS;
}

EFI_BOOT_MODE
EFIAPI
KabylakeRvp3BoardBootModeDetect (
  VOID
  )
{
  return BOOT_WITH_FULL_CONFIGURATION;
}

