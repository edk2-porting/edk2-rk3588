/** @file
  Initializes PCH CIO2 device ACPI data.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PchInit.h>

/**
  Update ASL definitions for CIO2 device.

  @retval EFI_SUCCESS         The function completed successfully
**/
EFI_STATUS
UpdateCio2AcpiData (
  VOID
  )
{
  UINT32 Index;
  PCH_STEPPING    PchStep;

  DEBUG ((DEBUG_INFO, "UpdateCio2AcpiData() Start\n"));
  PchStep   = PchStepping ();

  //if CIO2 is enabled as ACPI device then update its ACPI data
  if (PchStep >= PchLpC0) {
    mPchNvsAreaProtocol.Area->Cio2EnabledAsAcpiDevice = 0;
    DEBUG ((DEBUG_INFO, "UpdateCio2AcpiData() Cio2 has not been enabled as ACPI device\n"));
  } else {
    mPchNvsAreaProtocol.Area->Cio2EnabledAsAcpiDevice = 0;
    if (mPchConfigHob->Cio2.DeviceEnable == 1) {
      mPchNvsAreaProtocol.Area->Cio2EnabledAsAcpiDevice = 1;
      for (Index = 0; Index < mPchConfigHob->Interrupt.NumOfDevIntConfig; Index++) {
        if ((mPchConfigHob->Interrupt.DevIntConfig[Index].Device == PCI_DEVICE_NUMBER_PCH_CIO2) &&
            (mPchConfigHob->Interrupt.DevIntConfig[Index].Function == PCI_FUNCTION_NUMBER_PCH_CIO2)) {
          mPchNvsAreaProtocol.Area->Cio2IrqNumber = mPchConfigHob->Interrupt.DevIntConfig[Index].Irq;
          DEBUG ((DEBUG_INFO, "UpdateCio2AcpiData() Cio2 has been enabled as ACPI device. Irq number = 0x%x\n", mPchConfigHob->Interrupt.DevIntConfig[Index].Irq));
          break;
        }
      }
    }
  }

  DEBUG ((DEBUG_INFO, "UpdateCio2AcpiData() End\n"));

  return EFI_SUCCESS;
}


