/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Platform.h>
#include <Register/PchRegsUsb.h>
#include <PchLimits.h>
#include <PchPolicyCommon.h>
#include <IioBifurcationSlotTable.h>

VOID
SetBifurcations(
    IN OUT IIO_GLOBALS *IioGlobalData,
    IN     IIO_BIFURCATION_ENTRY *BifurcationTable,
    IN     UINT8                  BifurcationEntries
)
{
  UINT8 Socket;
  UINT8 Iou;
  UINT8 Index;

  for (Index = 0; Index < BifurcationEntries ; Index++) {
    Socket = BifurcationTable[Index].Socket;
    Iou = BifurcationTable[Index].IouNumber;
    switch (Iou) {
     case Iio_Iou0:
       if (IioGlobalData->SetupData.ConfigIOU0[Socket]==IIO_BIFURCATE_AUTO) {
         IioGlobalData->SetupData.ConfigIOU0[Socket] = BifurcationTable[Index].Bifurcation;
       }
       break;
     case Iio_Iou1:
       if (IioGlobalData->SetupData.ConfigIOU1[Socket] == IIO_BIFURCATE_AUTO) {
         IioGlobalData->SetupData.ConfigIOU1[Socket] = BifurcationTable[Index].Bifurcation;
       }
       break;
     case Iio_Iou2:
       if (IioGlobalData->SetupData.ConfigIOU2[Socket]==IIO_BIFURCATE_AUTO) {
         IioGlobalData->SetupData.ConfigIOU2[Socket] = BifurcationTable[Index].Bifurcation;
       }
       break;
     case Iio_Mcp0:
       if (IioGlobalData->SetupData.ConfigMCP0[Socket] == IIO_BIFURCATE_AUTO) {
           IioGlobalData->SetupData.ConfigMCP0[Socket] = BifurcationTable[Index].Bifurcation;
       }
       break;
     case Iio_Mcp1:
       if (IioGlobalData->SetupData.ConfigMCP1[Socket] == IIO_BIFURCATE_AUTO) {
           IioGlobalData->SetupData.ConfigMCP1[Socket] = BifurcationTable[Index].Bifurcation;
       }
       break;
     default:
       DEBUG((EFI_D_ERROR, "Invalid bifurcation table: Bad Iou (%d)", Iou));
       ASSERT(Iou);
       break;
    }
  }
}

VOID
EnableHotPlug (
    IN OUT IIO_GLOBALS *IioGlobalData,
    IN UINT8 Port,
    IN UINT8 VppPort,
    IN UINT8 VppAddress,
    IN UINT8 PortOwnership
  )
{
  IioGlobalData->SetupData.SLOTHPCAP[Port]= ENABLE;
  IioGlobalData->SetupData.SLOTAIP[Port]  = ENABLE;  // Attention Indicator Present
  IioGlobalData->SetupData.SLOTPIP[Port]  = ENABLE;  // Power Indicator Present
  IioGlobalData->SetupData.SLOTMRLSP[Port]= ENABLE; // MRL Sensor Present
  IioGlobalData->SetupData.SLOTABP[Port]  = ENABLE; // Attention Button Present
  IioGlobalData->SetupData.SLOTPCP[Port]  = ENABLE; // Power Controlled Present

 if (PortOwnership ==  PCIEAIC_OCL_OWNERSHIP){
    IioGlobalData->SetupData.SLOTAIP[Port]  = DISABLE;  // Attention Indicator Present
    IioGlobalData->SetupData.SLOTPIP[Port]  = DISABLE;  // Power Indicator Present
  }
  if (PortOwnership ==  VMD_OWNERSHIP){
    IioGlobalData->SetupData.SLOTABP[Port]  = DISABLE;
    IioGlobalData->SetupData.SLOTPCP[Port]  = DISABLE;
    IioGlobalData->SetupData.SLOTMRLSP[Port]= DISABLE;
  }
  //
  // Set SLTCAP settings based on VMD/PCIe SSD Ownership
  //
  if ((PortOwnership == PCIEAIC_OCL_OWNERSHIP) ||
      (PortOwnership == VMD_OWNERSHIP)){
    IioGlobalData->SetupData.SLOTHPSUP[Port]= ENABLE;   // HotPlug Surprise
  } 

  if (VppPort!= VPP_PORT_MAX) {
    IioGlobalData->SetupData.VppEnable[Port]= ENABLE;
    IioGlobalData->SetupData.VppPort[Port]= VppPort;
    IioGlobalData->SetupData.VppAddress[Port] = VppAddress;
  } else {
      DEBUG((EFI_D_ERROR, "PCIE HOT Plug. Missing VPP values on slot table\n"));
  }
}

VOID
ConfigSlots (
    IN OUT IIO_GLOBALS        *IioGlobalData,
    IN IIO_SLOT_CONFIG_ENTRY  *Slot,
    IN UINT8                  SlotEntries
  )
{
  UINT8 Index;
  UINT8 Port;

  for (Index =0; Index < SlotEntries; Index ++) {
    Port=Slot[Index].PortIndex;
    if (Slot[Index].Hidden != NOT_HIDE) {
      IioGlobalData->SetupData.HidePEXPMenu[Port] = HIDE;
      IioGlobalData->SetupData.PEXPHIDE[Port]= HIDE;
    }
    /// Check if slot is assigned.
    if (Slot[Index].SlotNumber!= NO_SLT_IMP){
       IioGlobalData->SetupData.SLOTIMP[Port]= SLT_IMP;
       IioGlobalData->SetupData.SLOTPSP[Port]=Slot[Index].SlotNumber;
       IioGlobalData->SetupData.SLOTEIP[Port]=Slot[Index].InterLockPresent;
       if (Slot[Index].SlotPowerLimitScale!= PWR_SCL_MAX) {
         IioGlobalData->SetupData.SLOTSPLS[Port] = Slot[Index].SlotPowerLimitScale;
         IioGlobalData->SetupData.SLOTSPLV[Port] = Slot[Index].SlotPowerLimitValue;
       }
       if (Slot[Index].HotPlugCapable != DISABLE) {
         EnableHotPlug(IioGlobalData, Port, Slot[Index].VppPort, Slot[Index].VppAddress, REGULAR_PCIE_OWNERSHIP);
       }
    }
  }
}

/**
    Verify if and Slot should be implemented based on IOUX bifurcation settings.

    @param IioGlobalData           Pointer to Iio Globals.
    @param Port                  - Port Index

    @retval TRUE/FALSE         to determine if an slot shoudl be implemented or not 
                               based on the IOUX bifurcation settings in case user want to do an 
                               override and VMD is enabled.

**/
BOOLEAN
SlotImplemented(
    IN OUT IIO_GLOBALS        *IioGlobalData,
    IN UINT8                  Port
  ){
  UINT8 IioIndex;
  UINT8 PortIndex;
  UINT8 Stack;
  BOOLEAN  SlotImp = FALSE;

  IioIndex = Port/NUMBER_PORTS_PER_SOCKET;
  PortIndex = (Port - (NUMBER_PORTS_PER_SOCKET * IioIndex));
 // Stack = (((PortIndex + 3)/4) - 1) + (IioIndex*VMD_STACK_PER_SOCKET);
  Stack = IioGlobalData->IioVar.IioVData.StackPerPort[IioIndex][PortIndex];
  DEBUG((DEBUG_INFO, "SlotImplemented:IioIndex = %x, Stack = %x, Port = %x, PortIndex =%x\n", IioIndex, Stack, Port, PortIndex));

  switch(Stack){
    case IIO_PSTACK0:
      if (IioGlobalData->SetupData.ConfigIOU0[IioIndex] == IIO_BIFURCATE_x4x4x4x4){
         SlotImp = TRUE;
      } else if (IioGlobalData->SetupData.ConfigIOU0[IioIndex] == IIO_BIFURCATE_x4x4xxx8){
        if ((PortIndex == PORT_1D_INDEX) || (PortIndex == PORT_1C_INDEX) || (PortIndex == PORT_1A_INDEX)){
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU0[IioIndex] == IIO_BIFURCATE_xxx8x4x4){
        if ((PortIndex == PORT_1C_INDEX) || (PortIndex == PORT_1B_INDEX) || (PortIndex == PORT_1A_INDEX)){
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU0[IioIndex] == IIO_BIFURCATE_xxx8xxx8){
        if ((PortIndex == PORT_1C_INDEX) || (PortIndex == PORT_1A_INDEX)){
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU0[IioIndex] == IIO_BIFURCATE_xxxxxx16){
        if (PortIndex == PORT_1A_INDEX){
          SlotImp = TRUE;
        }
      }
      break;
    case IIO_PSTACK1:
      if (IioGlobalData->SetupData.ConfigIOU1[IioIndex] == IIO_BIFURCATE_x4x4x4x4){
         SlotImp = TRUE;
      } else if (IioGlobalData->SetupData.ConfigIOU1[IioIndex] == IIO_BIFURCATE_x4x4xxx8){
        if ((PortIndex == PORT_2D_INDEX) || (PortIndex == PORT_2C_INDEX) || (PortIndex == PORT_2A_INDEX)){
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU1[IioIndex] == IIO_BIFURCATE_xxx8x4x4){
        if ((PortIndex == PORT_2C_INDEX) || (PortIndex == PORT_2B_INDEX) || (PortIndex == PORT_2A_INDEX)){
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU1[IioIndex] == IIO_BIFURCATE_xxx8xxx8){
        if ((PortIndex == PORT_2C_INDEX) || (PortIndex == PORT_2A_INDEX)){
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU1[IioIndex] == IIO_BIFURCATE_xxxxxx16){
        if (PortIndex == PORT_2A_INDEX){
          SlotImp = TRUE;
        }
      }
      break;
    case IIO_PSTACK2:
      if (IioGlobalData->SetupData.ConfigIOU2[IioIndex] == IIO_BIFURCATE_x4x4x4x4){
         SlotImp = TRUE;
      } else if (IioGlobalData->SetupData.ConfigIOU2[IioIndex] == IIO_BIFURCATE_x4x4xxx8){
        if ((PortIndex == PORT_3D_INDEX) || (PortIndex == PORT_3C_INDEX) || (PortIndex == PORT_3A_INDEX)){
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU2[IioIndex] == IIO_BIFURCATE_xxx8x4x4){
        if ((PortIndex == PORT_3C_INDEX) || (PortIndex == PORT_3B_INDEX) || (PortIndex == PORT_3A_INDEX)){
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU2[IioIndex] == IIO_BIFURCATE_xxx8xxx8){
        if ((PortIndex == PORT_3C_INDEX) || (PortIndex == PORT_3A_INDEX)){
          SlotImp = TRUE;
        }
      } else if (IioGlobalData->SetupData.ConfigIOU2[IioIndex] == IIO_BIFURCATE_xxxxxx16){
        if (PortIndex == PORT_3A_INDEX){
          SlotImp = TRUE;
        }
      }
      break;
  }
  DEBUG((DEBUG_INFO, "SlotImplemented:  = %x\n", SlotImp));
  return SlotImp;
}

/**
    Verify if VMD is enabled and override Slot conofgiration
    based on the VMD settings

    @param IioGlobalData           Pointer to Iio Globals.
    @param Slot                  - Slot configuarion settings 
    @param SlotEntries           - Number of slot entries

    @retval None

**/
VOID
OverrideConfigSlots (
    IN OUT IIO_GLOBALS        *IioGlobalData,
    IN IIO_SLOT_CONFIG_ENTRY  *Slot,
    IN UINT8                  SlotEntries
  )
{
  UINT8 Index;
  UINT8 Port;
  UINT8 IioIndex;
  UINT8 VmdPort;
  UINT8 Stack;

  for (Index =0; Index < SlotEntries; Index ++) {
    Port = Slot[Index].PortIndex;
    //
    // Check if Slot is capable of PcieSSD Solution and override the SLOT Config values
    //
    if (Slot[Index].PcieSSDCapable){
      IioIndex = Port/NUMBER_PORTS_PER_SOCKET;
      Stack = ((((Port - (NUMBER_PORTS_PER_SOCKET * IioIndex))+ 3)/4) - 1) + (IioIndex*VMD_STACK_PER_SOCKET);
      DEBUG((DEBUG_INFO, "Stack = %x, Port = %x\n", Stack, Port));

      //
      // check if VMD will own Pcie Root Port
      //
      if(IioGlobalData->SetupData.VMDEnabled[Stack]){
        VmdPort = ((IioIndex * VMD_PORTS_PER_SOCKET) + (Port - (NUMBER_PORTS_PER_SOCKET * IioIndex))) - 1;
        if (IioGlobalData->SetupData.VMDPortEnable[VmdPort]){
          IioGlobalData->IioVar.IioOutData.PciePortOwnership[Port] = VMD_OWNERSHIP;
        }
      } else {

        DEBUG((DEBUG_INFO, "IioGlobalData->SetupData.PcieAICEnabled[%x] = %x\n",Stack, IioGlobalData->SetupData.PcieAICEnabled[Stack]));
        //
        // Check if Pcie AIC Card will be present on Pcie Root Port
        //
        if(IioGlobalData->SetupData.PcieAICEnabled[Stack]){
          //
          // Force to have this port enabled by default for hot-plug.
          //
          IioGlobalData->SetupData.PciePortDisable[(IioIndex * NUMBER_PORTS_PER_SOCKET) + Port] = ENABLE;
          IioGlobalData->IioVar.IioOutData.PciePortOwnership[Port] = PCIEAIC_OCL_OWNERSHIP;
          DEBUG((DEBUG_ERROR, "Port = %x, PciePortDisable = %x\n",Port,IioGlobalData->SetupData.PciePortDisable[(IioIndex * NUMBER_PORTS_PER_SOCKET) + Port]));
        }
      } // No _VMD Ownership

      DEBUG((DEBUG_INFO, "PciePortOwnerShip[%x] = %x\n",Port, IioGlobalData->IioVar.IioOutData.PciePortOwnership[Port]));

      // if PcieSSDSupport required do slot override settings accordingly
      if((IioGlobalData->IioVar.IioOutData.PciePortOwnership[Port] != REGULAR_PCIE_OWNERSHIP) && 
          (SlotImplemented(IioGlobalData, Port) == TRUE)){
          IioGlobalData->SetupData.SLOTIMP[Port]= SLT_IMP;
          IioGlobalData->SetupData.SLOTPSP[Port]= 0x50  + Port; // Just program a value for PCIEACI_OCL/VMD
          IioGlobalData->SetupData.SLOTEIP[Port]= DISABLE;

          if (Slot[Index].SlotPowerLimitScale!= PWR_SCL_MAX) {
            IioGlobalData->SetupData.SLOTSPLS[Port] = Slot[Index].SlotPowerLimitScale;
            IioGlobalData->SetupData.SLOTSPLV[Port] = Slot[Index].SlotPowerLimitValue;
          }
          DEBUG((DEBUG_INFO,"Slot[Index].PcieSSDVppPort = %x\n", Slot[Index].PcieSSDVppPort));
          // Enable hot-plug if slot/port supports it
          if (Slot[Index].PcieSSDVppPort != VPP_PORT_MAX) {
          DEBUG((DEBUG_INFO, "IioGlobalData->SetupData.VMDHotPlugEnable[%x] = %x\n",Stack,IioGlobalData->SetupData.VMDHotPlugEnable[Stack]));
          DEBUG((DEBUG_INFO, "IioGlobalData->SetupData.PcieAICHotPlugEnable[%x] = %x\n",Stack,IioGlobalData->SetupData.PcieAICHotPlugEnable[Stack]));
          // Check if hot-plug is enabled for VMD or PCIeAIC case.
            if (((IioGlobalData->IioVar.IioOutData.PciePortOwnership[Port] == VMD_OWNERSHIP) && (IioGlobalData->SetupData.VMDHotPlugEnable[Stack])) ||
              ((IioGlobalData->IioVar.IioOutData.PciePortOwnership[Port] == PCIEAIC_OCL_OWNERSHIP) && (IioGlobalData->SetupData.PcieAICHotPlugEnable[Stack]))) {
              EnableHotPlug(IioGlobalData, Port, Slot[Index].PcieSSDVppPort, Slot[Index].PcieSSDVppAddress, IioGlobalData->IioVar.IioOutData.PciePortOwnership[Port]);
              DEBUG((DEBUG_INFO,"Enable HotPlug Done\n"));
            }
          }
          //
          // Unhide the port in order to get configured and it will be hide later for VMDLateSetup if VMD own the Pcie Root Port
          //
          IioGlobalData->SetupData.PEXPHIDE[Port]= NOT_HIDE;
      }// PcieSSDSupport
    }// PcieSSDCapable
  }// Per Slot
}


/**
    Auto determine which PCIe Root port to be hidden if its
    lanes are assigned to its preceding root port...use the
    Setup option variable of ConfigIOU to determine which ports
    are to be hidden on each IOU for corresponding IIO

    @param IOUx                  - IOUx Index
    @param IioIndex              - Index to Iio
    @param IioGlobalData           Pointer to Iio Globals.

    @retval None

**/
VOID
CalculatePEXPHideFromIouBif (
    IN UINT8 Iou,
    IN UINT8 IioIndex,
    IN OUT IIO_GLOBALS *IioGlobalData
)
{

  UINT8 *PXPHide, *HidePEXPMenu;
  UINT8 CurrentIOUConfigValue;
  UINT8 PXPOffset;
  PXPHide = IioGlobalData->SetupData.PEXPHIDE;
  HidePEXPMenu = IioGlobalData->SetupData.HidePEXPMenu;
  CurrentIOUConfigValue =0;

  PXPOffset=IioIndex * NUMBER_PORTS_PER_SOCKET;

  switch (Iou) {
  case Iio_Iou0:
    CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU0[IioIndex];
    PXPOffset+= PORT_1A_INDEX;
    break;
  case Iio_Iou1:
    CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU1[IioIndex];
    PXPOffset+= PORT_2A_INDEX;
    break;
  case Iio_Iou2:
    CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU2[IioIndex];
    PXPOffset+= PORT_3A_INDEX;
    break;
  case Iio_Mcp0:
    CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigMCP0[IioIndex];
    PXPOffset+= PORT_4A_INDEX;
    break;
  case Iio_Mcp1:
    CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigMCP1[IioIndex];
    PXPOffset += PORT_5A_INDEX;
    break;
  }

  switch(CurrentIOUConfigValue){
    case IIO_BIFURCATE_xxxxxxxx:
      PXPHide[PXPOffset + Iio_PortA] = HIDE;            // hide A
      PXPHide[PXPOffset + Iio_PortB] = HIDE;            // hide B
      PXPHide[PXPOffset + Iio_PortC] = HIDE;            // hide C
      PXPHide[PXPOffset + Iio_PortD] = HIDE;            // hide D
      HidePEXPMenu[PXPOffset + Iio_PortA] = HIDE;       // hide the Setup menu for A
      HidePEXPMenu[PXPOffset + Iio_PortB] = HIDE;       // hide the Setup menu for B
      HidePEXPMenu[PXPOffset + Iio_PortC] = HIDE;       // hide the Setup menu for C
      HidePEXPMenu[PXPOffset + Iio_PortD] = HIDE;       // hide the Setup menu for D
      break;
    case IIO_BIFURCATE_x4x4xxx8:
      PXPHide[PXPOffset + Iio_PortA] = NOT_HIDE;        // show A
      PXPHide[PXPOffset + Iio_PortB] = HIDE;            // hide B
      PXPHide[PXPOffset + Iio_PortC] = NOT_HIDE;        // show C
      PXPHide[PXPOffset + Iio_PortD] = NOT_HIDE;        // show D
      HidePEXPMenu[PXPOffset + Iio_PortA] = NOT_HIDE;   // show the Setup menu for B
      HidePEXPMenu[PXPOffset + Iio_PortB] = HIDE;       // hide the Setup menu for B
      HidePEXPMenu[PXPOffset + Iio_PortC] = NOT_HIDE;   // show the Setup menu for D
      HidePEXPMenu[PXPOffset + Iio_PortD] = NOT_HIDE;   // show the Setup menu for B
      break;
    case IIO_BIFURCATE_xxx8x4x4:
      PXPHide[PXPOffset + Iio_PortA] = NOT_HIDE;        // show A
      PXPHide[PXPOffset + Iio_PortB] = NOT_HIDE;        // show B
      PXPHide[PXPOffset + Iio_PortC] = NOT_HIDE;        // show C
      PXPHide[PXPOffset + Iio_PortD] = HIDE;            // hide port D
      HidePEXPMenu[PXPOffset + Iio_PortA] = NOT_HIDE;   // show the Setup menu for A
      HidePEXPMenu[PXPOffset + Iio_PortB] = NOT_HIDE;   // show the Setup menu for B
      HidePEXPMenu[PXPOffset + Iio_PortC] = NOT_HIDE;   // show the Setup menu for C
      HidePEXPMenu[PXPOffset + Iio_PortD] = HIDE;       // hide the Setup menu for D
      break;
    case IIO_BIFURCATE_xxx8xxx8:
      PXPHide[PXPOffset + Iio_PortA] = NOT_HIDE;        // show A
      PXPHide[PXPOffset + Iio_PortB] = HIDE;            // hide B
      PXPHide[PXPOffset + Iio_PortC] = NOT_HIDE;        // show C
      PXPHide[PXPOffset + Iio_PortD] = HIDE;            // hide D
      HidePEXPMenu[PXPOffset + Iio_PortA] = NOT_HIDE;   // show the Setup menu for A
      HidePEXPMenu[PXPOffset + Iio_PortB] = HIDE;       // hide the Setup menu for B
      HidePEXPMenu[PXPOffset + Iio_PortC] = NOT_HIDE;   // show the Setup menu for C
      HidePEXPMenu[PXPOffset + Iio_PortD] = HIDE;       // hide the Setup menu for D
      break;
    case IIO_BIFURCATE_xxxxxx16:
      PXPHide[PXPOffset + Iio_PortA] = NOT_HIDE;        // show A
      PXPHide[PXPOffset + Iio_PortB] = HIDE;            // hide B
      PXPHide[PXPOffset + Iio_PortC] = HIDE;            // hide C
      PXPHide[PXPOffset + Iio_PortD] = HIDE;            // hide D
      HidePEXPMenu[PXPOffset + Iio_PortA] = NOT_HIDE;   // show the Setup menu for A
      HidePEXPMenu[PXPOffset + Iio_PortB] = HIDE;       // hide the Setup menu for B
      HidePEXPMenu[PXPOffset + Iio_PortC] = HIDE;       // hide the Setup menu for C
      HidePEXPMenu[PXPOffset + Iio_PortD] = HIDE;       // hide the Setup menu for D
      break;
    default:
      PXPHide[PXPOffset + Iio_PortA] = NOT_HIDE;        // show A
      PXPHide[PXPOffset + Iio_PortB] = NOT_HIDE;        // show B
      PXPHide[PXPOffset + Iio_PortC] = NOT_HIDE;        // show C
      PXPHide[PXPOffset + Iio_PortD] = NOT_HIDE;        // show port D
      HidePEXPMenu[PXPOffset + Iio_PortA] = NOT_HIDE;   // show the Setup menu for A
      HidePEXPMenu[PXPOffset + Iio_PortB] = NOT_HIDE;   // show the Setup menu for B
      HidePEXPMenu[PXPOffset + Iio_PortC] = NOT_HIDE;   // show the Setup menu for C
      HidePEXPMenu[PXPOffset + Iio_PortD] = NOT_HIDE;   // show the Setup menu for D
     break;
  }

  //
  // Change PEXPHIDE setting to hide all PCIe port of a IOU if IIO_BIFURCATE_xxxxxxxx is set.
  // And set ConfigIOUx/ConfigMCPx to default bifucation control value
  // Bifurcation_Control[2:0] in IOU Bifurcation Control (PCIE_IOU_BIF_CTRL) register should be 000b ~ 100b.
  //
  if (CurrentIOUConfigValue == IIO_BIFURCATE_xxxxxxxx) {
    switch (Iou) {
    case Iio_Iou0:      
      IioGlobalData->SetupData.ConfigIOU0[IioIndex] = IIO_BIFURCATE_x4x4x4x4;
      break;
    case Iio_Iou1:
      IioGlobalData->SetupData.ConfigIOU1[IioIndex] = IIO_BIFURCATE_x4x4x4x4;
      break;
    case Iio_Iou2:
      IioGlobalData->SetupData.ConfigIOU2[IioIndex] = IIO_BIFURCATE_x4x4x4x4;
      break;
    case Iio_Mcp0:
      IioGlobalData->SetupData.ConfigMCP0[IioIndex] = IIO_BIFURCATE_x4x4x4x4;
      break;
    case Iio_Mcp1:
      IioGlobalData->SetupData.ConfigMCP1[IioIndex] = IIO_BIFURCATE_x4x4x4x4;
      break;
    default:
      break;
    }
  }
}


VOID
DumpPort(
    IIO_GLOBALS *IioGlobalData,
    UINT8        Port,
    UINT8        NumberOfPorts
)
{
  UINT8 Index;
  DEBUG((EFI_D_INFO, "IDX, Port Hide, Slot Impl, Slot Number, HotPlug, PcieSSD, VppPort, VppAddress, Interlock\n"));
  for (Index = Port; Index < (Port + NumberOfPorts); Index++ ) {
  DEBUG((EFI_D_INFO, "%3d|   %2d    |    %2d    |   %3d      |   %3d  |  %3d  |  0x%02x  |  0x%02x     |  %2d      \n", \
                       Index, \
                       IioGlobalData->SetupData.PEXPHIDE[Index],  \
                       IioGlobalData->SetupData.SLOTIMP[Index],   \
                       IioGlobalData->SetupData.SLOTPSP[Index],   \
                       IioGlobalData->SetupData.SLOTHPCAP[Index], \
                       IioGlobalData->IioVar.IioOutData.PciePortOwnership[Index],   \
                       IioGlobalData->SetupData.VppPort[Index],   \
                       IioGlobalData->SetupData.VppAddress[Index],\
                       IioGlobalData->SetupData.SLOTEIP[Index]));
            }
          }
/// Dump iio configuration. Dump the current IIO configuration to the serial
/// log.
VOID
DumpIioConfiguration(
    IN UINT8 iio,
    IN IIO_GLOBALS *IioGlobalData
)
{
  UINT8 Iou;
  UINT8 PortIndex;
  UINT8 Bifurcation;
  UINT8 IouPorts;
  PortIndex = iio * NUMBER_PORTS_PER_SOCKET;
  /// First dump the socket number;
  DEBUG((EFI_D_INFO, "Socket number: %d \n", iio));

  /// Dump DMI configuration:
  if ((iio == 0) && (PortIndex == 0)){
      DEBUG((EFI_D_INFO, "PORT 0: DMI Port\n"));
  } else {
      DEBUG((EFI_D_INFO, "PORT 0: DMI Port working as PCIE\n"));
      DumpPort(IioGlobalData, PortIndex, 1);
  }
  IouPorts=4;
  /// Dump IOU bifurcations:
  for (Iou = Iio_Iou0; Iou< Iio_IouMax; Iou ++) {
      /// Reset port index.
      PortIndex = iio * NUMBER_PORTS_PER_SOCKET;
      // Get the bifurcation
      switch (Iou) {
        case Iio_Iou0:
          Bifurcation = IioGlobalData->SetupData.ConfigIOU0[iio];
          PortIndex += PORT_1A_INDEX;
          DEBUG((EFI_D_INFO, "IUO0: Root Port 1, Bifurcation: %d\n", Bifurcation));
          break;
        case Iio_Iou1:
          Bifurcation = IioGlobalData->SetupData.ConfigIOU1[iio];
          PortIndex += PORT_2A_INDEX;
          DEBUG((EFI_D_INFO, "IUO1: Root Port 2, Bifurcation: %d\n", Bifurcation));
          break;
        case Iio_Iou2:
          Bifurcation = IioGlobalData->SetupData.ConfigIOU2[iio];
          PortIndex += PORT_3A_INDEX;
          DEBUG((EFI_D_INFO, "IUO2: Root Port 3, Bifurcation: %d\n", Bifurcation));
          break;
        case Iio_Mcp0:
          Bifurcation = IioGlobalData->SetupData.ConfigMCP0[iio];
          PortIndex += PORT_4A_INDEX;
          DEBUG((EFI_D_INFO, "MCP0, Bifurcation: %d\n", Bifurcation));
          break;
        case Iio_Mcp1:
          Bifurcation = IioGlobalData->SetupData.ConfigMCP1[iio];
          PortIndex += PORT_5A_INDEX;
          DEBUG((EFI_D_INFO, "MCP1, Bifurcation: %d\n", Bifurcation));
          break;
        default:
          DEBUG((EFI_D_INFO, "Iou no detected = %d",Iou));
          break;
        }
      DumpPort(IioGlobalData, PortIndex, IouPorts);
  }

}

UINT8
GetUplinkPortInformationCommon (
    IN UINT8 IioIndex
)
{
  UINT8 UplinkPortIndex = 0xFF;

  if (IioIndex == 0) {
    UplinkPortIndex = PcdGet8(PcdOemSkuUplinkPortIndex);
  }

  return UplinkPortIndex;
}
/**

  SystemIioPortBifurcationInit - Program the UDS data structure with OEM IIO init values
  for SLOTs and Bifurcation.

  @param mSB - pointer to this protocol
  @param IioUds - Pointer to the IIO UDS datastructure.

  @retval EFI_SUCCESS

**/
VOID
SystemIioPortBifurcationInitCommon (
    IIO_GLOBALS *IioGlobalData,
    IIO_BIFURCATION_ENTRY         **BifurcationTable,
    UINT8                         *BifurcationEntries,
    IIO_SLOT_CONFIG_ENTRY         **SlotTable,
    UINT8                         *SlotEntries
)
{

  UINT8                         PortIndex;//, iio;

  /// This function outline:
  //// 1 Based on platform apply the default bifurcation and slot configuration.
  //// 2 Apply dynamic overrides based on GPIO and other configurations.
  //// 3 Hide unused ports due bifurcation.

  for (PortIndex = 0; PortIndex < MAX_SOCKET*NUMBER_PORTS_PER_SOCKET; PortIndex++) {
    IioGlobalData->SetupData.PEXPHIDE[PortIndex] = 0;
    IioGlobalData->SetupData.HidePEXPMenu[PortIndex] = 0;
  }

  *BifurcationEntries = 0;
  *SlotEntries = 0;

  *BifurcationTable   = (IIO_BIFURCATION_ENTRY *)(UINTN)PcdGet64 (PcdIioBifurcationTable);
  *BifurcationEntries = PcdGet8 (PcdIioBifurcationTableEntries);
  *SlotTable          = (IIO_SLOT_CONFIG_ENTRY *)(UINTN)PcdGet64 (PcdIioSlotTable);
  *SlotEntries        = PcdGet8 (PcdIioSlotTableEntries);
}

VOID
SystemHideIioPortsCommon(
    IIO_GLOBALS *IioGlobalData,
    UINT8       IioIndex
)
{
    CalculatePEXPHideFromIouBif(Iio_Iou0, IioIndex, IioGlobalData);
    CalculatePEXPHideFromIouBif(Iio_Iou1, IioIndex, IioGlobalData);
    CalculatePEXPHideFromIouBif(Iio_Iou2, IioIndex, IioGlobalData);
    CalculatePEXPHideFromIouBif(Iio_Mcp0, IioIndex, IioGlobalData);
    CalculatePEXPHideFromIouBif(Iio_Mcp1, IioIndex, IioGlobalData);
    DumpIioConfiguration(IioIndex, IioGlobalData);
}
