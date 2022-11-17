/** @file
  PeiTbtInit library implementition with empty functions.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/PchPcieRpLib.h>
#include <Library/TbtCommonLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseLib.h>
#include <Library/GpioLib.h>


/**
  Selects the proper TBT Root port to assign resources
  based on the user input value

  @param[in]  SetupData          Pointer to Setup data

  @retval     TbtSelectorChosen  Rootport number.
**/
VOID
GetRootporttoSetResourcesforTbt (
  IN UINTN                              RpIndex,
  OUT UINT8                             *RsvdExtraBusNum,
  OUT UINT16                            *RsvdPcieMegaMem,
  OUT UINT8                             *PcieMemAddrRngMax,
  OUT UINT16                            *RsvdPciePMegaMem,
  OUT UINT8                             *PciePMemAddrRngMax,
  OUT BOOLEAN                           *SetResourceforTbt
  )
{
  UINTN TbtRpNumber;
  TbtRpNumber = (UINTN) PcdGet8 (PcdDTbtPcieRpNumber);

    if (RpIndex == (TbtRpNumber - 1)) {
        *RsvdExtraBusNum = PcdGet8 (PcdDTbtPcieExtraBusRsvd);
        *RsvdPcieMegaMem = PcdGet16 (PcdDTbtPcieMemRsvd);
        *PcieMemAddrRngMax = PcdGet8 (PcdDTbtPcieMemAddrRngMax);
        *RsvdPciePMegaMem = PcdGet16 (PcdDTbtPciePMemRsvd);
        *PciePMemAddrRngMax = PcdGet8 (PcdDTbtPciePMemAddrRngMax);
        *SetResourceforTbt = TRUE;
      }
      else {
        *SetResourceforTbt = FALSE;
      }
  }

/**
  Internal function to Wait for Tbt2PcieDone Bit.to Set or clear
  @param[in]  CommandOffsetAddress      Tbt2Pcie Register Address
  @param[in]  TimeOut                   Time out with 100 ms garnularity
  @param[in]  Tbt2PcieDone              Wait condition (wait for Bit to Clear/Set)
  @param[out] *Tbt2PcieValue Function   Register value
**/
BOOLEAN
InternalWaitforCommandCompletion(
  IN  UINT64   CommandOffsetAddress,
  IN  UINT32   TimeOut,
  IN  BOOLEAN  Tbt2PcieDone,
  OUT UINT32   *Tbt2PcieValue
  )
{
  BOOLEAN ReturnFlag;
  UINT32  Tbt2PcieCheck;

  ReturnFlag = FALSE;
  while (TimeOut-- > 0) {
    *Tbt2PcieValue = PciSegmentRead32 (CommandOffsetAddress);

    if (0xFFFFFFFF == *Tbt2PcieValue ) {
      //
      // Device is not here return now
      //
      ReturnFlag     = FALSE;
      break;
    }

    if(Tbt2PcieDone) {
      Tbt2PcieCheck  =  *Tbt2PcieValue & TBT2PCIE_DON_R;
    } else {
      Tbt2PcieCheck  = !(*Tbt2PcieValue & TBT2PCIE_DON_R);
    }

    if (Tbt2PcieCheck) {
      ReturnFlag     = TRUE;
      break;
    }

    MicroSecondDelay(TBT_MAIL_BOX_DELAY);
  }
  return ReturnFlag;
}
/**
  Get Security Level.
  @param[in]  Bus       Bus number Host Router (DTBT)
  @param[in]  Device    Device number for Host Router (DTBT)
  @param[in]  Function  Function number for  Host Router (DTBT)
  @param[in]  Command   Command for  Host Router (DTBT)
  @param[in]  Timeout   Time out with 100 ms garnularity
**/
UINT8
GetSecLevel (
  IN    UINT8                   Bus,
  IN    UINT8                   Device,
  IN    UINT8                   Function,
  IN    UINT8                   Command,
  IN    UINT32                  Timeout
  )
{
  UINT64       Pcie2Tbt;
  UINT64       Tbt2Pcie;
  UINT32       RegisterValue;
  UINT8        ReturnFlag;

  ReturnFlag           = 0xFF;

  DEBUG ((DEBUG_INFO, "GetSecLevel() \n"));

  GET_TBT2PCIE_REGISTER_ADDRESS(0, Bus, Device, Function, Tbt2Pcie)
  GET_PCIE2TBT_REGISTER_ADDRESS(0, Bus, Device, Function, Pcie2Tbt)

  PciSegmentWrite32 (Pcie2Tbt, Command | PCIE2TBT_VLD_B);

  if(InternalWaitforCommandCompletion(Tbt2Pcie, Timeout, TRUE, &RegisterValue)) {
    ReturnFlag     = (UINT8) (0xFF & (RegisterValue >> 8));
  }

  PciSegmentWrite32 (Pcie2Tbt, 0);

  InternalWaitforCommandCompletion(Tbt2Pcie, Timeout, FALSE, &RegisterValue);
  DEBUG ((DEBUG_INFO, "Security Level configured to %x \n", ReturnFlag));

  return ReturnFlag;
}

/**
  Set Security Level.
  @param[in]  Data      Security State
  @param[in]  Bus       Bus number for Host Router (DTBT)
  @param[in]  Device    Device number for Host Router (DTBT)
  @param[in]  Function  Function number for Host Router (DTBT)
  @param[in]  Command   Command for  Host Router (DTBT)
  @param[in]  Timeout   Time out with 100 ms garnularity
**/
BOOLEAN
SetSecLevel (
  IN    UINT8                   Data,
  IN    UINT8                   Bus,
  IN    UINT8                   Device,
  IN    UINT8                   Function,
  IN    UINT8                   Command,
  IN    UINT32                  Timeout
  )
{
  UINT64       Pcie2Tbt;
  UINT64       Tbt2Pcie;
  UINT32       RegisterValue;
  BOOLEAN      ReturnFlag;

  ReturnFlag   = FALSE;

  DEBUG ((DEBUG_INFO, "SetSecLevel() \n"));

  GET_TBT2PCIE_REGISTER_ADDRESS(0, Bus, Device, Function, Tbt2Pcie)
  GET_PCIE2TBT_REGISTER_ADDRESS(0, Bus, Device, Function, Pcie2Tbt)

  PciSegmentWrite32 (Pcie2Tbt, (Data << 8) | Command | PCIE2TBT_VLD_B);

  ReturnFlag = InternalWaitforCommandCompletion(Tbt2Pcie, Timeout, TRUE, &RegisterValue);
  DEBUG ((DEBUG_INFO, "RegisterValue %x \n", RegisterValue));
  PciSegmentWrite32 (Pcie2Tbt, 0);

  InternalWaitforCommandCompletion(Tbt2Pcie, Timeout, FALSE, &RegisterValue);
  DEBUG ((DEBUG_INFO, "Return value %x \n", ReturnFlag));
  return ReturnFlag;
}

/**
Based on the Security Mode Selection, BIOS drives FORCE_PWR.

@param[in]  GpioNumber
@param[in]  Value
**/
VOID
ForceDtbtPower(
  IN  UINT8          GpioAccessType,
  IN  UINT8          Expander,
  IN  UINT32         GpioNumber,
  IN  BOOLEAN        Value
)
{
  if (GpioAccessType == 0x01) {
    // PCH
    GpioSetOutputValue (GpioNumber, (UINT32)Value);
  } else if (GpioAccessType == 0x02) {
    // IoExpander {TCA6424A}
    GpioExpSetOutput (Expander, (UINT8)GpioNumber, (UINT8)Value);
  }
}

/**
Execute TBT Mail Box Command

@param[in]  Command   TBT Command
@param[in]  Bus       Bus number for  Host Router (DTBT)
@param[in]  Device    Device number for  Host Router (DTBT)
@param[in]  Function  Function number for  Host Router (DTBT)
@param[in]  Timeout   Time out with 100 ms garnularity
@Retval     true      if command executes succesfully
**/
BOOLEAN
TbtSetPcie2TbtCommand(
   IN    UINT8                   Command,
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

   GET_TBT2PCIE_REGISTER_ADDRESS(0, Bus, Device, Function, Tbt2Pcie)
   GET_PCIE2TBT_REGISTER_ADDRESS(0, Bus, Device, Function, Pcie2Tbt)

   PciSegmentWrite32 (Pcie2Tbt, Command | PCIE2TBT_VLD_B);

   ReturnFlag = InternalWaitforCommandCompletion(Tbt2Pcie, Timeout, TRUE, &RegisterValue);

   PciSegmentWrite32(Pcie2Tbt, 0);

   return ReturnFlag;
}
/**
  Get Pch/Peg Pcie Root Port Device and Function Number for TBT by Root Port physical Number

  @param[in]  RpNumber              Root port physical number. (0-based)
  @param[out] RpDev                 Return corresponding root port device number.
  @param[out] RpFun                 Return corresponding root port function number.

  @retval     EFI_SUCCESS           Root port device and function is retrieved
  @retval     EFI_INVALID_PARAMETER If Invalid Root Port Number or TYPE is Passed
**/
EFI_STATUS
EFIAPI
GetDTbtRpDevFun (
  IN  BOOLEAN Type,
  IN  UINTN   RpNumber,
  OUT UINTN   *RpDev,
  OUT UINTN   *RpFunc
  )
{
  EFI_STATUS            Status;
  UINTN                 TbtRpDev;
  UINTN                 TbtRpFunc;

  Status = EFI_INVALID_PARAMETER; // Update the Status to EFI_SUCCESS if valid input found.
  //
  // PCH-H can support up to 24 root ports. PEG0,PEG1 and PEG2 will be
  // with device number 0x1 and Function number 0,1 and 2 respectively.
  //
  if (Type == DTBT_TYPE_PEG)
  {
    //
    //  PEG Rootport
    //
    if (RpNumber <= 2) {
      *RpDev  =   0x01;
      *RpFunc =   RpNumber;
      Status  =   EFI_SUCCESS;
    }
  }
  if (Type == DTBT_TYPE_PCH)
  {
    //
    //  PCH Rootport
    //
    if (RpNumber <= 23) {
      Status  = GetPchPcieRpDevFun (RpNumber, &TbtRpDev, &TbtRpFunc);
      *RpDev  = TbtRpDev;
      *RpFunc = TbtRpFunc;
    }
  }

  ASSERT_EFI_ERROR (Status);
  return Status;
}

BOOLEAN
IsTbtHostRouter (
  IN    UINT16  DeviceID
  )
{
  switch (DeviceID) {
  case AR_HR_2C:
  case AR_HR_4C:
  case AR_HR_LP:
  case AR_HR_C0_2C:
  case AR_HR_C0_4C:
  case TR_HR_2C:
  case TR_HR_4C:
    return TRUE;
  }

  return FALSE;
} // IsTbtHostRouter


