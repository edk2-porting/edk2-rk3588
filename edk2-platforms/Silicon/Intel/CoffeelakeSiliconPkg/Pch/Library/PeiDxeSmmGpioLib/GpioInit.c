/** @file
  This file contains routines for GPIO initialization

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "GpioLibrary.h"
#include <Register/PchRegsPcr.h>

//
// GPIO_GROUP_DW_DATA structure is used by GpioConfigurePch function
// to cache values which will be programmed into respective GPIO registers
// after all GpioPads are processed. This way MMIO accesses are decreased
// and instead of doing one programming for one GpioPad there is only
// one access for whole register.
//
typedef struct {
  UINT32             HostSoftOwnReg;
  UINT32             HostSoftOwnRegMask;
  UINT32             GpiGpeEnReg;
  UINT32             GpiGpeEnRegMask;
  UINT32             GpiNmiEnReg;
  UINT32             GpiNmiEnRegMask;
  UINT32             GpiSmiEnReg;
  UINT32             GpiSmiEnRegMask;
  UINT32             ConfigUnlockMask;
  UINT32             OutputUnlockMask;
} GPIO_GROUP_DW_DATA;

//
// GPIO_GROUP_DW_NUMBER contains number of DWords required to
// store Pad data for all groups. Each pad uses one bit.
//
// For Cannonlake only vGPIO group has >32 pads but those pads
// will not be accessed by this function so GPIO_GROUP_DW_NUMBER can be 1
//
#define GPIO_GROUP_DW_NUMBER  1

/**
  Get GPIO DW Register values (HOSTSW_OWN, GPE_EN, NMI_EN, Lock).

  @param[in]     PadNumber      GPIO pad number
  @param[in]     GpioConfig     GPIO Config data
  @param[in out] DwRegsValues   Values for GPIO DW Registers

  @retval None
**/
STATIC
VOID
GpioDwRegValueFromGpioConfig (
  IN UINT32                 PadNumber,
  IN CONST GPIO_CONFIG      *GpioConfig,
  IN OUT GPIO_GROUP_DW_DATA *GroupDwData
  )
{
  UINT32  PadBitPosition;
  UINT32  DwNum;

  PadBitPosition = GPIO_GET_PAD_POSITION (PadNumber);
  DwNum = GPIO_GET_DW_NUM (PadNumber);

  if (DwNum >= GPIO_GROUP_DW_NUMBER) {
    ASSERT (FALSE);
    return;
  }
  //
  // Update value to be programmed in HOSTSW_OWN register
  //
  GroupDwData[DwNum].HostSoftOwnRegMask |= (GpioConfig->HostSoftPadOwn & 0x1) << PadBitPosition;
  GroupDwData[DwNum].HostSoftOwnReg |= (GpioConfig->HostSoftPadOwn >> 0x1) << PadBitPosition;

  //
  // Update value to be programmed in GPI_GPE_EN register
  //
  GroupDwData[DwNum].GpiGpeEnRegMask |= (GpioConfig->InterruptConfig & 0x1) << PadBitPosition;
  GroupDwData[DwNum].GpiGpeEnReg |= ((GpioConfig->InterruptConfig & GpioIntSci) >> 3) << PadBitPosition;

  //
  // Update value to be programmed in GPI_NMI_EN register
  //
  GroupDwData[DwNum].GpiNmiEnRegMask |= (GpioConfig->InterruptConfig & 0x1) << PadBitPosition;
  GroupDwData[DwNum].GpiNmiEnReg |= ((GpioConfig->InterruptConfig & GpioIntNmi) >> 1) << PadBitPosition;

  //
  // Update value to be programmed in GPI_SMI_EN register
  GroupDwData[DwNum].GpiSmiEnRegMask |= (GpioConfig->InterruptConfig & 0x1) << PadBitPosition;
  GroupDwData[DwNum].GpiSmiEnReg |= ((GpioConfig->InterruptConfig & GpioIntSmi) >> 2) << PadBitPosition;
  if ((GpioConfig->InterruptConfig & GpioIntSmi) == GpioIntSmi) {
    GroupDwData[DwNum].HostSoftOwnRegMask |= 1 << PadBitPosition;
    GroupDwData[DwNum].HostSoftOwnReg |= 1 << PadBitPosition;
  }

  //
  // Update information on Pad Configuration Lock
  //
  GroupDwData[DwNum].ConfigUnlockMask |= ((GpioConfig->LockConfig >> 1) & 0x1) << PadBitPosition;

  //
  // Update information on Pad Configuration Lock Tx
  //
  GroupDwData[DwNum].OutputUnlockMask |= ((GpioConfig->LockConfig >> 3) & 0x1) << PadBitPosition;

  //
  // if pad in GpioMode is an output default action should be to leave output unlocked
  //
  if ((GpioConfig->PadMode == GpioPadModeGpio) &&
      (GpioConfig->Direction == GpioDirOut) &&
      ((GpioConfig->LockConfig & B_GPIO_LOCK_CONFIG_OUTPUT_LOCK_MASK) == GpioLockDefault)) {
    GroupDwData[DwNum].OutputUnlockMask |= 0x1 << PadBitPosition;
  }
}

/**
  This internal procedure will scan GPIO initialization table and unlock
  all pads present in it

  @param[in] NumberOfItem               Number of GPIO pad records in table
  @param[in] GpioInitTableAddress       GPIO initialization table
  @param[in] Index                      Index of GPIO Initialization table record

  @retval EFI_SUCCESS                   The function completed successfully
  @retval EFI_INVALID_PARAMETER         Invalid group or pad number
**/
STATIC
EFI_STATUS
GpioUnlockPadsForAGroup (
  IN UINT32                    NumberOfItems,
  IN GPIO_INIT_CONFIG          *GpioInitTableAddress,
  IN UINT32                    Index
  )
{
  UINT32                 PadsToUnlock[GPIO_GROUP_DW_NUMBER];
  UINT32                 DwNum;
  UINT32                 PadBitPosition;
  CONST GPIO_GROUP_INFO  *GpioGroupInfo;
  UINT32                 GpioGroupInfoLength;
  CONST GPIO_INIT_CONFIG *GpioData;
  GPIO_GROUP             Group;
  UINT32                 GroupIndex;
  UINT32                 PadNumber;

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  GpioData   = &GpioInitTableAddress[Index];
  Group      = GpioGetGroupFromGpioPad (GpioData->GpioPad);
  GroupIndex = GpioGetGroupIndexFromGpioPad (GpioData->GpioPad);

  ZeroMem (PadsToUnlock, sizeof (PadsToUnlock));
  //
  // Loop through pads for one group. If pad belongs to a different group then
  // break and move to register programming.
  //
  while (Index < NumberOfItems) {

    GpioData   = &GpioInitTableAddress[Index];
    if (GroupIndex != GpioGetGroupIndexFromGpioPad (GpioData->GpioPad)) {
      //if next pad is from different group then break loop
      break;
    }

    PadNumber  = GpioGetPadNumberFromGpioPad (GpioData->GpioPad);
    //
    // Check if legal pin number
    //
    if (PadNumber >= GpioGroupInfo[GroupIndex].PadPerGroup) {
      DEBUG ((DEBUG_ERROR, "GPIO ERROR: Pin number (%d) exceeds possible range for group %d\n", PadNumber, GroupIndex));
      return EFI_INVALID_PARAMETER;
    }

    PadBitPosition = GPIO_GET_PAD_POSITION (PadNumber);
    DwNum = GPIO_GET_DW_NUM (PadNumber);

    if (DwNum >= GPIO_GROUP_DW_NUMBER) {
      ASSERT (FALSE);
      return EFI_UNSUPPORTED;
    }
    //
    // Update pads which need to be unlocked
    //
    PadsToUnlock[DwNum] |= 0x1 << PadBitPosition;

    //Move to next item
    Index++;
  }

  for (DwNum = 0; DwNum <= GPIO_GET_DW_NUM (GpioGroupInfo[GroupIndex].PadPerGroup); DwNum++) {
    //
    // Unlock pads
    //
    if (PadsToUnlock[DwNum] != 0) {
      GpioUnlockPadCfgForGroupDw (Group, DwNum, PadsToUnlock[DwNum]);
      GpioUnlockPadCfgTxForGroupDw (Group, DwNum, PadsToUnlock[DwNum]);
    }
  }

  return EFI_SUCCESS;
}

/**
  This procedure will initialize multiple PCH GPIO pins

  @param[in] NumberofItem               Number of GPIO pads to be updated
  @param[in] GpioInitTableAddress       GPIO initialization table

  @retval EFI_SUCCESS                   The function completed successfully
  @retval EFI_INVALID_PARAMETER         Invalid group or pad number
**/
STATIC
EFI_STATUS
GpioConfigurePch (
  IN UINT32                    NumberOfItems,
  IN GPIO_INIT_CONFIG          *GpioInitTableAddress
  )
{
  UINT32                 Index;
  UINT32                 PadCfgDwReg[GPIO_PADCFG_DW_REG_NUMBER];
  UINT32                 PadCfgDwRegMask[GPIO_PADCFG_DW_REG_NUMBER];
  UINT32                 PadCfgReg;
  GPIO_GROUP_DW_DATA     GroupDwData[GPIO_GROUP_DW_NUMBER];
  UINT32                 DwNum;
  CONST GPIO_GROUP_INFO  *GpioGroupInfo;
  UINT32                 GpioGroupInfoLength;
  GPIO_PAD_OWN           PadOwnVal;
  CONST GPIO_INIT_CONFIG *GpioData;
  UINT32                 GroupIndex;
  UINT32                 PadNumber;
  PCH_SBI_PID            GpioCom;

  PadOwnVal = GpioPadOwnHost;

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  Index = 0;
  while (Index < NumberOfItems) {

    GpioData   = &GpioInitTableAddress[Index];
    GroupIndex = GpioGetGroupIndexFromGpioPad (GpioData->GpioPad);
    GpioCom    = GpioGroupInfo[GroupIndex].Community;

    DEBUG_CODE_BEGIN();
    if (!GpioIsCorrectPadForThisChipset (GpioData->GpioPad)) {
      DEBUG ((DEBUG_ERROR, "GPIO ERROR: Incorrect GpioPad (0x%08x) used on this chipset!\n", GpioData->GpioPad));
      ASSERT (FALSE);
      return EFI_UNSUPPORTED;
    }
    DEBUG_CODE_END ();

    //
    // Unlock pads for a given group which are going to be reconfigured
    //
    //
    // Because PADCFGLOCK/LOCKTX register reset domain is Powergood, lock settings
    // will get back to default only after G3 or DeepSx transition. On the other hand GpioPads
    // configuration is controlled by a configurable type of reset - PadRstCfg. This means that if
    // PadRstCfg != Powergood GpioPad will have its configuration locked despite it being not the
    // one desired by BIOS. Before reconfiguring all pads they will get unlocked.
    //
    GpioUnlockPadsForAGroup (NumberOfItems, GpioInitTableAddress, Index);

    ZeroMem (GroupDwData, sizeof (GroupDwData));
    //
    // Loop through pads for one group. If pad belongs to a different group then
    // break and move to register programming.
    //
    while (Index < NumberOfItems) {

      GpioData   = &GpioInitTableAddress[Index];
      if (GroupIndex != GpioGetGroupIndexFromGpioPad (GpioData->GpioPad)) {
        //if next pad is from different group then break loop
        break;
      }

      PadNumber  = GpioGetPadNumberFromGpioPad (GpioData->GpioPad);

      DEBUG_CODE_BEGIN ();
      //
      // Check if legal pin number
      //
      if (PadNumber >= GpioGroupInfo[GroupIndex].PadPerGroup) {
        DEBUG ((DEBUG_ERROR, "GPIO ERROR: Pin number (%d) exceeds possible range for group %d\n", PadNumber, GroupIndex));
        return EFI_INVALID_PARAMETER;
      }

      //
      // Check if selected GPIO Pad is not owned by CSME/ISH
      //
      GpioGetPadOwnership (GpioData->GpioPad, &PadOwnVal);

      if (PadOwnVal != GpioPadOwnHost) {
        DEBUG ((DEBUG_ERROR, "GPIO ERROR: Accessing pad not owned by host (Group=%d, Pad=%d)!\n", GroupIndex, PadNumber));
        DEBUG ((DEBUG_ERROR, "** Please make sure the GPIO usage in sync between CSME and BIOS configuration. \n"));
        DEBUG ((DEBUG_ERROR, "** All the GPIO occupied by CSME should not do any configuration by BIOS.\n"));
        //Move to next item
        Index++;
        continue;
      }

      //
      // Check if Pad enabled for SCI is to be in unlocked state
      //
      if (((GpioData->GpioConfig.InterruptConfig & GpioIntSci) == GpioIntSci) &&
          ((GpioData->GpioConfig.LockConfig & B_GPIO_LOCK_CONFIG_PAD_CONF_LOCK_MASK) != GpioPadConfigUnlock)){
        DEBUG ((DEBUG_ERROR, "GPIO ERROR: %a used for SCI is not unlocked!\n", GpioName (GpioData->GpioPad)));
        ASSERT (FALSE);
        return EFI_INVALID_PARAMETER;
      }
      DEBUG_CODE_END ();

      ZeroMem (PadCfgDwReg, sizeof (PadCfgDwReg));
      ZeroMem (PadCfgDwRegMask, sizeof (PadCfgDwRegMask));
      //
      // Get GPIO PADCFG register value from GPIO config data
      //
      GpioPadCfgRegValueFromGpioConfig (
        GpioData->GpioPad,
        &GpioData->GpioConfig,
        PadCfgDwReg,
        PadCfgDwRegMask
        );

      //
      // Create PADCFG register offset using group and pad number
      //
      PadCfgReg = S_GPIO_PCR_PADCFG * PadNumber + GpioGroupInfo[GroupIndex].PadCfgOffset;

      //
      // Write PADCFG DW0 register
      //
      MmioAndThenOr32 (
        PCH_PCR_ADDRESS (GpioCom, PadCfgReg),
        ~PadCfgDwRegMask[0],
        PadCfgDwReg[0]
        );

      //
      // Write PADCFG DW1 register
      //
      MmioAndThenOr32 (
        PCH_PCR_ADDRESS (GpioCom, PadCfgReg + 0x4),
        ~PadCfgDwRegMask[1],
        PadCfgDwReg[1]
        );

      //
      // Write PADCFG DW2 register
      //
      MmioAndThenOr32 (
        PCH_PCR_ADDRESS (GpioCom, PadCfgReg + 0x8),
        ~PadCfgDwRegMask[2],
        PadCfgDwReg[2]
        );

      //
      // Get GPIO DW register values from GPIO config data
      //
      GpioDwRegValueFromGpioConfig (
        PadNumber,
        &GpioData->GpioConfig,
        GroupDwData
        );

      //Move to next item
      Index++;
    }

    for (DwNum = 0; DwNum <= GPIO_GET_DW_NUM (GpioGroupInfo[GroupIndex].PadPerGroup); DwNum++) {
      //
      // Write HOSTSW_OWN registers
      //
      if (GpioGroupInfo[GroupIndex].HostOwnOffset != NO_REGISTER_FOR_PROPERTY) {
        MmioAndThenOr32 (
          PCH_PCR_ADDRESS (GpioCom, GpioGroupInfo[GroupIndex].HostOwnOffset + DwNum * 0x4),
          ~GroupDwData[DwNum].HostSoftOwnRegMask,
          GroupDwData[DwNum].HostSoftOwnReg
          );
      }

      //
      // Write GPI_GPE_EN registers
      //
      if (GpioGroupInfo[GroupIndex].GpiGpeEnOffset != NO_REGISTER_FOR_PROPERTY) {
        MmioAndThenOr32 (
          PCH_PCR_ADDRESS (GpioCom, GpioGroupInfo[GroupIndex].GpiGpeEnOffset + DwNum * 0x4),
          ~GroupDwData[DwNum].GpiGpeEnRegMask,
          GroupDwData[DwNum].GpiGpeEnReg
          );
      }

      //
      // Write GPI_NMI_EN registers
      //
      if (GpioGroupInfo[GroupIndex].NmiEnOffset != NO_REGISTER_FOR_PROPERTY) {
        MmioAndThenOr32 (
          PCH_PCR_ADDRESS (GpioCom, GpioGroupInfo[GroupIndex].NmiEnOffset + DwNum * 0x4),
          ~GroupDwData[DwNum].GpiNmiEnRegMask,
          GroupDwData[DwNum].GpiNmiEnReg
          );
      } else if (GroupDwData[DwNum].GpiNmiEnReg != 0x0) {
        DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group %d has no pads supporting NMI\n", GroupIndex));
        ASSERT_EFI_ERROR (EFI_UNSUPPORTED);
      }

      //
      // Write GPI_SMI_EN registers
      //
      if (GpioGroupInfo[GroupIndex].SmiEnOffset != NO_REGISTER_FOR_PROPERTY) {
        MmioAndThenOr32 (
          PCH_PCR_ADDRESS (GpioCom, GpioGroupInfo[GroupIndex].SmiEnOffset + DwNum * 0x4),
          ~GroupDwData[DwNum].GpiSmiEnRegMask,
          GroupDwData[DwNum].GpiSmiEnReg
          );
      } else if (GroupDwData[DwNum].GpiSmiEnReg != 0x0) {
        DEBUG ((DEBUG_ERROR, "GPIO ERROR: Group %d has no pads supporting SMI\n", GroupIndex));
        ASSERT_EFI_ERROR (EFI_UNSUPPORTED);
      }

      //
      // Update Pad Configuration unlock data
      //
      if (GroupDwData[DwNum].ConfigUnlockMask) {
        GpioStoreGroupDwUnlockPadConfigData (GroupIndex, DwNum, GroupDwData[DwNum].ConfigUnlockMask);
      }

      //
      // Update Pad Output unlock data
      //
      if (GroupDwData[DwNum].OutputUnlockMask) {
        GpioStoreGroupDwUnlockOutputData (GroupIndex, DwNum, GroupDwData[DwNum].OutputUnlockMask);
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  This procedure will clear all status bits of any GPIO interrupts.
**/
STATIC
VOID
GpioClearAllGpioInterrupts (
  VOID
  )
{
  GPIO_GROUP             Group;
  CONST GPIO_GROUP_INFO  *GpioGroupInfo;
  GPIO_GROUP             GpioGroupLowest;
  GPIO_GROUP             GpioGroupHighest;
  UINT32                 GroupIndex;
  UINT32                 GpioGroupInfoLength;
  UINT32                 DwNum;

  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);

  GpioGroupLowest = GpioGetLowestGroup ();
  GpioGroupHighest = GpioGetHighestGroup ();

  for (Group = GpioGroupLowest; Group <= GpioGroupHighest; Group++) {
    GroupIndex = GpioGetGroupIndexFromGroup (Group);
    //
    // Check if group has GPI IS register
    //
    if (GpioGroupInfo[GroupIndex].GpiIsOffset != NO_REGISTER_FOR_PROPERTY) {
      //
      // Clear all GPI_IS Status bits by writing '1'
      //
      for (DwNum = 0; DwNum <= GPIO_GET_DW_NUM (GpioGroupInfo[GroupIndex].PadPerGroup); DwNum++) {
        MmioWrite32 (
          PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].GpiIsOffset + DwNum * 0x4),
          0xFFFFFFFF
          );
      }
    }

    //
    // Check if group has GPI_GPE_STS register
    //
    if (GpioGroupInfo[GroupIndex].GpiGpeStsOffset != NO_REGISTER_FOR_PROPERTY) {
      //
      // Clear all GPI_GPE_STS Status bits by writing '1'
      //
      for (DwNum = 0; DwNum <= GPIO_GET_DW_NUM (GpioGroupInfo[GroupIndex].PadPerGroup); DwNum++) {
        MmioWrite32 (
          PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].GpiGpeStsOffset + DwNum * 0x4),
          0xFFFFFFFF
          );
      }
    }

    //
    // Check if group has SMI_STS register
    //
    if (GpioGroupInfo[GroupIndex].SmiStsOffset != NO_REGISTER_FOR_PROPERTY) {
      //
      // Clear all SMI_STS Status bits by writing '1'
      //
      for (DwNum = 0; DwNum <= GPIO_GET_DW_NUM (GpioGroupInfo[GroupIndex].PadPerGroup); DwNum++) {
        MmioWrite32 (
          PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].SmiStsOffset + DwNum * 4),
          0xFFFFFFFF
          );
      }
    }

    //
    // Check if group has NMI_STS register
    //
    if (GpioGroupInfo[GroupIndex].NmiStsOffset != NO_REGISTER_FOR_PROPERTY) {
      //
      // Clear all NMI_STS Status bits by writing '1'
      //
      for (DwNum = 0; DwNum <= GPIO_GET_DW_NUM (GpioGroupInfo[GroupIndex].PadPerGroup); DwNum++) {
        MmioWrite32 (
          PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, GpioGroupInfo[GroupIndex].NmiStsOffset + DwNum * 4),
          0xFFFFFFFF
          );
      }
    }

  }
}

/**
  This procedure will initialize multiple GPIO pins. Use GPIO_INIT_CONFIG structure.
  Structure contains fields that can be used to configure each pad.
  Pad not configured using GPIO_INIT_CONFIG will be left with hardware default values.
  Separate fields could be set to hardware default if it does not matter, except
  GpioPad and PadMode.
  Function will work in most efficient way if pads which belong to the same group are
  placed in adjacent records of the table.
  Although function can enable pads for Native mode, such programming is done
  by reference code when enabling related silicon feature.

  @param[in] NumberofItem               Number of GPIO pads to be updated
  @param[in] GpioInitTableAddress       GPIO initialization table

  @retval EFI_SUCCESS                   The function completed successfully
  @retval EFI_INVALID_PARAMETER         Invalid group or pad number
**/
EFI_STATUS
GpioConfigurePads (
  IN UINT32                    NumberOfItems,
  IN GPIO_INIT_CONFIG          *GpioInitTableAddress
  )
{
  EFI_STATUS   Status;
  Status =  GpioConfigurePch (NumberOfItems, GpioInitTableAddress);
  GpioClearAllGpioInterrupts ();
  return Status;
}

