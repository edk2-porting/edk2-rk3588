/**@file
  Thunderbolt(TM) Pei Library

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/GpioLib.h>
#include <GpioPinsSklLp.h>
#include <GpioPinsSklH.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>
#include <Library/MmPciLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/GpioExpanderLib.h>
#include <Ppi/ReadOnlyVariable2.h>

#include <Base.h>
#include <Library/TbtCommonLib.h>
#include <TbtBoardInfo.h>
#include <IndustryStandard/Pci22.h>
#include <Library/PchCycleDecodingLib.h>
#include <Ppi/PeiTbtPolicy.h>
#include <Library/PciSegmentLib.h>
#include <Library/PeiTbtPolicyLib.h>
#include <Library/PchPmcLib.h>
#include <Private/Library/PeiDTbtInitLib.h>

/**
Is host router (For dTBT) or End Point (For iTBT) present before sleep

@param[in] ControllerType - DTBT_CONTROLLER or ITBT_CONTROLLER
@param[in] Controller     - Controller begin offset of CMOS

@Retval     TRUE      There is a TBT HostRouter presented before sleep
@Retval     FALSE     There is no TBT HostRouter presented before sleep

BOOLEAN
IsHostRouterPresentBeforeSleep(
IN  UINT8        ControllerType,
IN  UINT8        Controller
)
{
  UINT8 SavedState;

  SavedState = (UINT8)GetTbtHostRouterStatus();
  if (ControllerType == DTBT_CONTROLLER){
    return ((SavedState & (DTBT_SAVE_STATE_OFFSET << Controller)) == (DTBT_SAVE_STATE_OFFSET << Controller));
  } else {
    if (ControllerType == ITBT_CONTROLLER) {
      return ((SavedState & (ITBT_SAVE_STATE_OFFSET << Controller)) == (ITBT_SAVE_STATE_OFFSET << Controller));
    }
  }
  return 0;
}
**/

/**
Execute TBT PCIE2TBT_SX_EXIT_TBT_CONNECTED Mail Box Command for S4 mode with PreBootAclEnable

@param[in]  Bus       Bus number for Host Router (DTBT)
@param[in]  Device    Device number for Host Router (DTBT)
@param[in]  Function  Function number for Host Router (DTBT)
@param[in]  Timeout   Time out with 100 ms garnularity
@Retval     true      if command executes succesfully
**/
BOOLEAN
TbtSetPcie2TbtSxExitCommandWithPreBootAclEnable(
   IN    UINT8                   Bus,
   IN    UINT8                   Device,
   IN    UINT8                   Function,
   IN    UINT32                  Timeout
)
{
  UINT64      Pcie2Tbt;
  UINT64      Tbt2Pcie;
  UINT32      RegisterValue;
  BOOLEAN     ReturnFlag;
  UINT32      Command;

  GET_TBT2PCIE_REGISTER_ADDRESS(0, Bus, Device, Function, Tbt2Pcie)
  GET_PCIE2TBT_REGISTER_ADDRESS(0, Bus, Device, Function, Pcie2Tbt)

// If PreBootAcl is Enable, we need to enable DATA bit while sending SX EXIT MAIL BOX Command
  Command = (1 << 8) | PCIE2TBT_SX_EXIT_TBT_CONNECTED;
  PciSegmentWrite32 (Pcie2Tbt, Command | PCIE2TBT_VLD_B);

  ReturnFlag = InternalWaitforCommandCompletion(Tbt2Pcie, Timeout, TRUE, &RegisterValue);

  PciSegmentWrite32(Pcie2Tbt, 0);

  return ReturnFlag;
}

/**
Set the Sleep Mode if the HR is up.
@param[in]  Bus       Bus number for Host Router (DTBT)
@param[in]  Device    Device number for Host Router (DTBT)
@param[in]  Function  Function number for Host Router (DTBT)
**/
VOID
TbtSetSxMode(
IN    UINT8                   Bus,
IN    UINT8                   Device,
IN    UINT8                   Function,
IN    UINT8                   TbtBootOn
)
{
  UINT64                          TbtUsDevId;
  UINT64                          Tbt2Pcie;
  UINT32                          RegVal;
  UINT32                          MaxLoopCount;
  UINTN                           Delay;
  UINT8                           RetCode;
  EFI_BOOT_MODE                   BootMode;
  EFI_STATUS                      Status;

  TbtUsDevId = PCI_SEGMENT_LIB_ADDRESS(0, Bus, Device, Function, 0);
  GET_TBT2PCIE_REGISTER_ADDRESS(0, Bus, Device, Function, Tbt2Pcie)

  MaxLoopCount = TBT_5S_TIMEOUT;  // Wait 5 sec
  Delay = 100 * 1000;
  RetCode = 0x62;

  Status = PeiServicesGetBootMode(&BootMode);
  ASSERT_EFI_ERROR(Status);

  if ((BootMode == BOOT_ON_S4_RESUME) && (TbtBootOn == 2)) {
    MaxLoopCount = TBT_3S_TIMEOUT;
    if (!TbtSetPcie2TbtSxExitCommandWithPreBootAclEnable(Bus, Device, Function, MaxLoopCount)) {
      //
      // Nothing to wait, HR is not responsive
      //
      return;
    }
  }
  else {
    if (!TbtSetPcie2TbtCommand(PCIE2TBT_SX_EXIT_TBT_CONNECTED, Bus, Device, Function, MaxLoopCount)) {
      //
      // Nothing to wait, HR is not responsive
      //
      return;
    }
  }

  DEBUG((DEBUG_INFO, "Wait for Dev ID != 0xFF\n"));

  while (MaxLoopCount-- > 0) {
    //
    // Check what HR still here
    //
    RegVal = PciSegmentRead32(Tbt2Pcie);
    if (0xFFFFFFFF == RegVal) {
      RetCode = 0x6F;
      break;
    }
    //
    // Check completion of TBT link
    //
    RegVal = PciSegmentRead32(TbtUsDevId);
    if (0xFFFFFFFF != RegVal) {
      RetCode = 0x61;
      break;
    }

    MicroSecondDelay(Delay);
  }

  DEBUG((DEBUG_INFO, "Return code = 0x%x\n", RetCode));
}
/**
  set tPCH25 Timing to 10 ms for DTBT.

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtSetTPch25Timing (
  IN  PEI_TBT_POLICY  *PeiTbtConfig
)
{
  DEBUG ((DEBUG_INFO, "DTbtSetTPch25Timing call Inside\n"));
  UINT32                PchPwrmBase;

  //
  //During boot, reboot and wake  tPCH25 Timing should be set to 10 ms
  //
  PchPwrmBaseGet (&PchPwrmBase);
  MmioOr32 (
    (UINTN) (PchPwrmBase + R_PCH_PWRM_CFG),
    (BIT0 | BIT1)
    );

  DEBUG((DEBUG_INFO, "DTbtSetTPch25Timing call Return\n"));
  return EFI_SUCCESS;
}

/**
  Do ForcePower for DTBT Controller

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtForcePower (
  IN  PEI_TBT_POLICY  *PeiTbtConfig
)
{

  DEBUG ((DEBUG_INFO, "DTbtForcePower call Inside\n"));

      if (PeiTbtConfig->DTbtCommonConfig.Gpio3ForcePwr) {
        DEBUG((DEBUG_INFO, "ForcePwrGpio.GpioPad = %x \n", PeiTbtConfig-> DTbtControllerConfig.ForcePwrGpio.GpioPad));
        ForceDtbtPower(PeiTbtConfig-> DTbtControllerConfig.ForcePwrGpio.GpioAccessType,PeiTbtConfig-> DTbtControllerConfig.ForcePwrGpio.Expander, PeiTbtConfig-> DTbtControllerConfig.ForcePwrGpio.GpioPad, PeiTbtConfig-> DTbtControllerConfig.ForcePwrGpio.GpioLevel);
        DEBUG((DEBUG_INFO, "ForceDtbtPower asserted \n"));
        MicroSecondDelay(PeiTbtConfig->DTbtCommonConfig.Gpio3ForcePwrDly * 1000);
        DEBUG((DEBUG_INFO, "Delay after ForceDtbtPower = 0x%x ms \n", PeiTbtConfig->DTbtCommonConfig.Gpio3ForcePwrDly));
      }

  DEBUG ((DEBUG_INFO, "DTbtForcePower call Return\n"));
  return EFI_SUCCESS;
}

/**
  Clear VGA Registers for DTBT.

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtClearVgaRegisters (
  IN  PEI_TBT_POLICY  *PeiTbtConfig
)
{
  UINTN      RpDev;
  UINTN      RpFunc;
  EFI_STATUS Status;
  UINT64     BridngeBaseAddress;
  UINT16     Data16;

  DEBUG ((DEBUG_INFO, "DTbtClearVgaRegisters call Inside\n"));

  Status = EFI_SUCCESS;

  Status = GetDTbtRpDevFun(PeiTbtConfig-> DTbtControllerConfig.Type, PeiTbtConfig-> DTbtControllerConfig.PcieRpNumber - 1, &RpDev, &RpFunc);
  ASSERT_EFI_ERROR(Status);
  //
  // VGA Enable and VGA 16-bit decode registers of Bridge control register of Root port where
  // Host router resides should be cleaned
  //
 
  BridngeBaseAddress = PCI_SEGMENT_LIB_ADDRESS(0, 0, (UINT32)RpDev, (UINT32)RpFunc, 0);
  Data16 = PciSegmentRead16(BridngeBaseAddress + PCI_BRIDGE_CONTROL_REGISTER_OFFSET);
  Data16 &= (~(EFI_PCI_BRIDGE_CONTROL_VGA | EFI_PCI_BRIDGE_CONTROL_VGA_16));
  PciSegmentWrite16(BridngeBaseAddress + PCI_BRIDGE_CONTROL_REGISTER_OFFSET, Data16);

  DEBUG ((DEBUG_INFO, "DTbtClearVgaRegisters call Return\n"));
  return Status;
}

/**
  Exectue Mail box command "Boot On".

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtBootOn(
  IN  PEI_TBT_POLICY  *PeiTbtConfig
)
{
  EFI_STATUS Status;
  UINT32     OrgBusNumberConfiguration;
  UINTN      RpDev;
  UINTN      RpFunc;

  DEBUG((DEBUG_INFO, "DTbtBootOn call Inside\n"));

  Status = EFI_SUCCESS;

      Status = GetDTbtRpDevFun(PeiTbtConfig-> DTbtControllerConfig.Type, PeiTbtConfig-> DTbtControllerConfig.PcieRpNumber - 1, &RpDev, &RpFunc);
      ASSERT_EFI_ERROR(Status);
      OrgBusNumberConfiguration = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET));
      //
      // Set Sec/Sub buses to 0xF0
      //
      PciSegmentWrite32(PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET), 0x00F0F000);
      //
      //When Thunderbolt(TM) boot [TbtBootOn] is enabled in bios setup we need to do the below:
      //Bios should send "Boot On" message through PCIE2TBT register
      //The Boot On command as described above would include the command and acknowledge from FW (with the default timeout in BIOS),
      //once the Boot On command is completed it is guaranteed that the AlpineRidge(AR) device is there and the PCI tunneling was done by FW,
      //next step from BIOS is enumeration using SMI
      //

      if (PeiTbtConfig->DTbtCommonConfig.TbtBootOn > 0) {
        //
        // Exectue Mail box command "Boot On / Pre-Boot ACL"
        //
        //Command may be executed only during boot/reboot and not during Sx exit flow
        if (PeiTbtConfig->DTbtCommonConfig.TbtBootOn == 1) {
          if (!TbtSetPcie2TbtCommand(PCIE2TBT_BOOT_ON, 0xF0, 0, 0, TBT_5S_TIMEOUT)) {
            //
            // Nothing to wait, HR is not responsive
            //
            DEBUG((DEBUG_INFO, "<TbtPei> DTbtBootOn - Boot On message sent failed \n"));
          }
        }
        if (PeiTbtConfig->DTbtCommonConfig.TbtBootOn == 2) {
          if (!TbtSetPcie2TbtCommand(PCIE2TBT_PREBOOTACL, 0xF0, 0, 0, TBT_3S_TIMEOUT)) {
            //
            // Nothing to wait, HR is not responsive
            //
            DEBUG((DEBUG_INFO, "<TbtPei> DTbtBootOn - Pre-Boot ACL message sent failed \n"));
          }
        }
      }
      //
      // Reset Sec/Sub buses to original value
      //
      PciSegmentWrite32(PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET), OrgBusNumberConfiguration);

  DEBUG((DEBUG_INFO, "DTbtBootOn call Return\n"));
  return Status;
}

/**
  Exectue Mail box command "USB On".

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtUsbOn(
  IN  PEI_TBT_POLICY  *PeiTbtConfig
)
{
  EFI_STATUS                      Status;
  UINTN                           RpDev;
  UINTN                           RpFunc;
  UINT32                          OrgBusNumberConfiguration;
  UINT64                          TbtBaseAddress;
  UINT32                          MaxWaitIter;
  UINT32                          RegVal;
  EFI_BOOT_MODE                   BootMode;

  DEBUG((DEBUG_INFO, "DTbtUsbOn call Inside\n"));

  Status = EFI_SUCCESS;

      Status = GetDTbtRpDevFun(PeiTbtConfig-> DTbtControllerConfig.Type, PeiTbtConfig-> DTbtControllerConfig.PcieRpNumber - 1, &RpDev, &RpFunc);
      ASSERT_EFI_ERROR(Status);
      OrgBusNumberConfiguration = PciSegmentRead32(PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET));
      //
      // Set Sec/Sub buses to 0xF0
      //
      PciSegmentWrite32(PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET), 0x00F0F000);

      //
      //When Thunderbolt(TM) Usb boot [TbtUsbOn] is enabled in bios setup we need to do the below:
      //Bios should send "Usb On" message through PCIE2TBT register
      //The Usb On command as described above would include the command and acknowledge from FW (with the default timeout in BIOS),
      //once the Usb On command is completed it is guaranteed that the AlpineRidge(AR) device is there and the PCI tunneling was done by FW,
      //next step from BIOS is enumeration using SMI
      //
      if (PeiTbtConfig->DTbtCommonConfig.TbtUsbOn) {
        if (PeiTbtConfig->DTbtCommonConfig.TbtBootOn > 0) {
          MaxWaitIter = 50;   // Wait 5 sec
          TbtBaseAddress = PCI_SEGMENT_LIB_ADDRESS(0, 0xF0, 0, 0, 0);
          //
          // Driver clears the PCIe2TBT Valid bit to support two consicutive mailbox commands
          //
          PciSegmentWrite32(TbtBaseAddress + PCIE2TBT_DTBT_R, 0);
          DEBUG((DEBUG_INFO, "TbtBaseAddress + PCIE2TBT_DTBT_R = 0x%lx \n", TbtBaseAddress + PCIE2TBT_DTBT_R));
          while (MaxWaitIter-- > 0) {
            RegVal = PciSegmentRead32(TbtBaseAddress + TBT2PCIE_DTBT_R);
            if (0xFFFFFFFF == RegVal) {
              //
              // Device is not here return now
              //
              DEBUG((DEBUG_INFO, "TBT device is not present \n"));
              break;
            }

            if (!(RegVal & TBT2PCIE_DON_R)) {
              break;
            }
            MicroSecondDelay(100 * 1000);
          }
        }

        Status = PeiServicesGetBootMode(&BootMode);
        ASSERT_EFI_ERROR(Status);

        //
        // Exectue Mail box command "Usb On"
        //
        //Command may be executed only during boot/reboot and not during S3 exit flow
        //In case of S4 Exit send USB ON cmd only if Host Router was inactive/not present during S4 entry
        if ((BootMode == BOOT_ON_S4_RESUME) ) {
          // USB_ON cmd not required
        } else {
          if (!TbtSetPcie2TbtCommand(PCIE2TBT_USB_ON, 0xF0, 0, 0, TBT_5S_TIMEOUT)) {
            //
            // Nothing to wait, HR is not responsive
            //
            DEBUG((DEBUG_INFO, "<TbtPei> TbtBootSupport - Usb On message sent failed \n"));
          }
        }
      }
      //
      // Reset Sec/Sub buses to original value
      //
      PciSegmentWrite32(PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET), OrgBusNumberConfiguration);

  DEBUG((DEBUG_INFO, "DTbtUsbOn call return\n"));
  return Status;
}

/**
  Exectue Mail box command "Sx Exit".

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtSxExitFlow(
  IN  PEI_TBT_POLICY  *PeiTbtConfig
)
{
  EFI_STATUS                      Status;
  UINT32                          OrgBusNumberConfiguration;
  UINTN                           RpDev;
  UINTN                           RpFunc;
  UINT32                          Count;

  DEBUG((DEBUG_INFO, "DTbtSxExitFlow call Inside\n"));

  Status = EFI_SUCCESS;
  Count = 0;

      Status = GetDTbtRpDevFun(PeiTbtConfig-> DTbtControllerConfig.Type, PeiTbtConfig-> DTbtControllerConfig.PcieRpNumber - 1, &RpDev, &RpFunc);
      ASSERT_EFI_ERROR(Status);
      OrgBusNumberConfiguration = PciSegmentRead32(PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET));
      //
      // Set Sec/Sub buses to 0xF0
      //
      PciSegmentWrite32(PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET), 0x00F0F000);

      if ( (PeiTbtConfig->DTbtCommonConfig.TbtBootOn == 2)) {
        //
        // WA: When system with TBT 3.1 device, resume SX system need to wait device ready. In document that maximum time out should be 500ms.
        //
        while (PciSegmentRead32(PCI_SEGMENT_LIB_ADDRESS(0, 0xf0, 0x0, 0x0, 0x08)) == 0xffffffff) { //End Device will be with Device Number 0x0, Function Number 0x0.
          MicroSecondDelay(STALL_ONE_MICRO_SECOND * 1000); // 1000usec
          Count++;
          if (Count > 10000) { //Allowing Max Delay of 10 sec for CFL-S board.
          break;
          }
        }

        //
        // Upon wake, if BIOS saved pre-Sx Host Router state as active (system went to sleep with
        // attached devices), BIOS should:
        // 1. Execute "Sx_Exit_TBT_Connected" mailbox command.
        // 2. If procedure above returns true, BIOS should perform "wait for fast link bring-up" loop
        // 3. Continue regular wake flow.
        //
        //
        // Exectue Mail box command and perform "wait for fast link bring-up" loop
        //
        TbtSetSxMode(0xF0, 0, 0, PeiTbtConfig->DTbtCommonConfig.TbtBootOn);
      }
      //
      // Reset Sec/Sub buses to original value
      //
      PciSegmentWrite32(PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET), OrgBusNumberConfiguration);

  DEBUG((DEBUG_INFO, "DTbtSxExitFlow call Return\n"));
  return Status;
}


/**
  Initialize Thunderbolt(TM)

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/
EFI_STATUS
EFIAPI
TbtInit (
  VOID
  )
{
  EFI_STATUS            Status;
  PEI_TBT_POLICY             *PeiTbtConfig;

  //
  // Get the TBT Policy
  //
  Status = PeiServicesLocatePpi (
             &gPeiTbtPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &PeiTbtConfig
             );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, " gPeiTbtPolicyPpiGuid Not installed!!!\n"));
  }
  ASSERT_EFI_ERROR (Status);
  //
  // Exectue Mail box command "Boot On"
  //
  Status = DTbtBootOn (PeiTbtConfig);
  //
  // Exectue Mail box command "Usb On"
  //
  Status = DTbtUsbOn (PeiTbtConfig);
  //
  //During boot, reboot and wake  (bits [1:0]) of PCH PM_CFG register should be
  //set to 11b - 10 ms (default value is 0b - 10 us)
  //
  Status = DTbtSetTPch25Timing (PeiTbtConfig);
  //
  // Configure Tbt Force Power 
  //
  Status = DTbtForcePower (PeiTbtConfig);
  //
  // VGA Enable and VGA 16-bit decode registers of Bridge control register of Root port where
  // Host router resides should be cleaned
  //
  Status = DTbtClearVgaRegisters (PeiTbtConfig);
  //
  // Upon wake, if BIOS saved pre-Sx Host Router state as active (system went to sleep with
  // attached devices), BIOS should:
  // 1. Execute "Sx_Exit_TBT_Connected" mailbox command.
  // 2. If procedure above returns true, BIOS should perform "wait for fast link bring-up" loop
  // 3. Continue regular wake flow.
  //
  Status = DTbtSxExitFlow (PeiTbtConfig);
  return EFI_SUCCESS;
}
