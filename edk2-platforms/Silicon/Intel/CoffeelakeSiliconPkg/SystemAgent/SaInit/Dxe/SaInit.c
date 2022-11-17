/** @file
  This is the Common driver that initializes the Intel System Agent.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "SaInit.h"
#include <Library/PciSegmentLib.h>
#include <Private/SaConfigHob.h>
#include <Protocol/PciEnumerationComplete.h>

//
// Declare I/O Ports used to perform PCI Confguration Cycles
//
#define PCI_CONFIGURATION_ADDRESS_PORT  0xCF8
#define PCI_CONFIGURATION_DATA_PORT     0xCFC

/**
  Convert a PCI Library address to PCI CF8 formatted address.

  Declare macro to convert PCI Library address to PCI CF8 formatted address.
  Bit fields of PCI Library and CF8 formatted address is as follows:
  PCI Library formatted address    CF8 Formatted Address
 =============================    ======================
    Bits 00..11  Register           Bits 00..07  Register
    Bits 12..14  Function           Bits 08..10  Function
    Bits 15..19  Device             Bits 11..15  Device
    Bits 20..27  Bus                Bits 16..23  Bus
    Bits 28..31  Reserved(MBZ)      Bits 24..30  Reserved(MBZ)
                                    Bits 31..31  Must be 1

  @param  A The address to convert.

  @retval The coverted address.

**/
#define PCI_TO_CF8_ADDRESS(A) \
  ((UINT32) ((((A) >> 4) & 0x00ffff00) | ((A) & 0xfc) | 0x80000000))

///
/// Global Variables
///
GLOBAL_REMOVE_IF_UNREFERENCED SA_CONFIG_HOB                          *mSaConfigHob;
BOOLEAN                                                              mSkipPamLock = FALSE;

/*
  Intel(R) Core Processor Skylake BWG version 0.4.0

  18.6 System Agent Configuration Locking
   For reliable operation and security, System BIOS must set the following bits:
   1. For all modern Intel processors, Intel strongly recommends that BIOS should set
       the D_LCK bit. Set B0:D0:F0.R088h [4] = 1b to lock down SMRAM space.
  BaseAddr values for mSaSecurityRegisters that uses PciExpressBaseAddress will be initialized at
  Runtime inside function SaPcieInitPolicy().
*/
GLOBAL_REMOVE_IF_UNREFERENCED BOOT_SCRIPT_REGISTER_SETTING mSaSecurityRegisters[] = {
  {0,  R_SA_SMRAMC,  0xFFFFFFFF,  BIT4}
};

/**
  SystemAgent Initialization Common Function.

  @retval EFI_SUCCESS   - Always.
**/

VOID
SaInitEntryPoint (
  VOID
  )
{
  ///
  /// Get SaConfigHob HOB
  ///
  mSaConfigHob              = NULL;
  mSaConfigHob              = (SA_CONFIG_HOB *) GetFirstGuidHob (&gSaConfigHobGuid);
  if (mSaConfigHob != NULL) {
    mSkipPamLock = mSaConfigHob->SkipPamLock;
  }

  return;
}



/**
  Common function locks the PAM register as part of the SA Security requirements.

  @retval EFI_SUCCESS   - Always.
**/

VOID
SaPamLock (
  VOID
  )
{
  UINT64 BaseAddress;
  UINT32 Data32Or;

  if (mSkipPamLock == FALSE) {
    //
    // Lock PAM by PAM Lock Bit
    //
    BaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, 0, 0, 0, 0);
    Data32Or    = BIT0;
    DEBUG ((DEBUG_INFO, "PAM_LOCK!!\n"));
    PciSegmentOr32 (BaseAddress + R_SA_PAM0, Data32Or);
  }
}

/**
  This function does SA security lock
**/
VOID
SaSecurityLock (
  VOID
  )
{
  UINT8           Index;
  UINT32          RegOffset;
  UINT32          Data32Or;
  UINT32          Data32;
  UINT8           Data8;

  ///
  /// 17.2 System Agent Security Lock configuration
  ///
  DEBUG ((DEBUG_INFO, "DXE SaSecurityLock\n"));
  for (Index = 0; Index < (sizeof (mSaSecurityRegisters) / sizeof (BOOT_SCRIPT_REGISTER_SETTING)); Index++) {
    RegOffset   = mSaSecurityRegisters[Index].Offset;
    Data32Or    = mSaSecurityRegisters[Index].OrMask;

    if (RegOffset == R_SA_SMRAMC) {
      ///
      /// SMRAMC LOCK must use CF8/CFC access
      ///
      PciCf8Or8 (PCI_CF8_LIB_ADDRESS (SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, R_SA_SMRAMC), (UINT8) Data32Or);
      Data8 = PciCf8Read8 (PCI_CF8_LIB_ADDRESS (SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, R_SA_SMRAMC));
      Data32 = PCI_TO_CF8_ADDRESS (PCI_CF8_LIB_ADDRESS (SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, R_SA_SMRAMC));
      S3BootScriptSaveIoWrite (
        S3BootScriptWidthUint32,
        (UINTN) (PCI_CONFIGURATION_ADDRESS_PORT),
        1,
        &Data32
        );
      S3BootScriptSaveIoWrite (
        S3BootScriptWidthUint8,
        (UINTN) (PCI_CONFIGURATION_DATA_PORT),
        1,
        &Data8
        );
    }
  }
}

/**
  This function performs SA Security locking in EndOfDxe callback

  @retval EFI_SUCCESS     - Security lock has done
  @retval EFI_UNSUPPORTED - Security lock not done successfully
**/
EFI_STATUS
SaSecurityInit (
  VOID
  )
{

  UINT8                     Index;

  for (Index = 0; Index < (sizeof (mSaSecurityRegisters) / sizeof (BOOT_SCRIPT_REGISTER_SETTING)); Index++) {
    if (mSaSecurityRegisters[Index].BaseAddr != PcdGet64 (PcdMchBaseAddress)) {
      mSaSecurityRegisters[Index].BaseAddr = PcdGet64 (PcdPciExpressBaseAddress);
    }
  }
  SaSecurityLock ();

  return EFI_SUCCESS;
}
