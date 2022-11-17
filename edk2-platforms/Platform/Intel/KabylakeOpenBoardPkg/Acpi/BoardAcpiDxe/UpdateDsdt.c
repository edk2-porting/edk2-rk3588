/** @file
  Acpi Gnvs Init Library.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/GlobalNvsArea.h>
extern GLOBAL_REMOVE_IF_UNREFERENCED EFI_GLOBAL_NVS_AREA_PROTOCOL              mGlobalNvsArea;

VOID
UpdateDsdt (
  IN VOID   *Table
  )
{
  UINT8   *CurrPtr;
  UINT8   *TmpDsdtPointer;
  UINT8   *DsdtPointer;
  UINT32  *Signature;
  UINT8   *Operation;
  UINT32  *Address;
  UINT8   *Value;
  UINT16  *Size;
  BOOLEAN EnterDock = FALSE;

  UINT8   MaximumDsdtPointLength;

  MaximumDsdtPointLength = 20;

    //
    // Fix up the AML code in the DSDT affected by end user options.
    // Fix up the following ASL Code:
    // (1)  ACPI Global NVS Memory Base and Size.
    // (2)  ACPI Graphics NVS Memory Base and Size.
    // (3)  SMBus I/O Base.
    // (4)  Thermal Management Methods.
    //
      //
      // Loop through the ASL looking for values that we must fix up.
      //
      CurrPtr = (UINT8 *) Table;
      for (DsdtPointer = CurrPtr;
           DsdtPointer <= (CurrPtr + ((EFI_ACPI_COMMON_HEADER *) CurrPtr)->Length);
           DsdtPointer++
          ) {
        Signature = (UINT32 *) DsdtPointer;
        switch (*Signature) {
        //
        // GNVS operation region
        //
        case (SIGNATURE_32 ('G', 'N', 'V', 'S')):
          //
          // Conditional match.  For Region Objects, the Operator will always be the
          // byte immediately before the specific name.  Therefore, subtract 1 to check
          // the Operator.
          //
          Operation = DsdtPointer - 1;
          if (*Operation == AML_EXT_REGION_OP) {
            Address   = (UINT32 *) (DsdtPointer + 6);
            *Address  = (UINT32) (UINTN) mGlobalNvsArea.Area;
            Size      = (UINT16 *) (DsdtPointer + 11);
            *Size     = sizeof (EFI_GLOBAL_NVS_AREA);
          }
          break;

        //
        // _AC0 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '0')):
          //
          // Conditional match.  _AC0 is >63 and <4095 bytes, so the package length is 2 bytes.
          // Therefore, subtract 3 to check the Operator.
          //
          Operation = DsdtPointer - 3;
          if (*Operation == AML_METHOD_OP) {
            //
            // Check if we want _AC0 enabled
            //
            if (PcdGet8 (PcdDisableActiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '0');
            }
          }
          break;

        //
        // _AL0 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '0')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {

            //
            // Check if we want _AL0 enabled
            //
            if (PcdGet8 (PcdDisableActiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '0');
            }
          }
          break;

        //
        // _AC1 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '1')):
          //
          // Conditional match.  _AC1 is < 63 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _AC1 enabled
            //
            if (PcdGet8 (PcdDisableActiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '1');
            }
          }
          break;

        //
        // _AL1 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '1')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {

            //
            // Check if we want _AL1 enabled
            //
            if (PcdGet8 (PcdDisableActiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '1');
            }
          }
          break;

        //
        // _AC2 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '2')):
          //
          // Conditional match.  _AC2 is < 63 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _AC2 enabled
            //
            if (PcdGet8 (PcdDisableActiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '2');
            }
          }
          break;

        //
        // _AL2 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '2')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {

            //
            // Check if we want _AL2 enabled
            //
            if (PcdGet8 (PcdDisableActiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '2');
            }
          }
          break;

        //
        // _AC3 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '3')):
          //
          // Conditional match.  _AC3 is < 63 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _AC3 enabled
            //
            if (PcdGet8 (PcdDisableActiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '3');
            }
          }
          break;

        //
        // _AL3 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '3')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {

            //
            // Check if we want _AL3 enabled
            //
            if (PcdGet8 (PcdDisableActiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '3');
            }
          }
          break;

        //
        // _AC4 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '4')):
          //
          // Conditional match.  _AC4 is < 63 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _AC4 enabled
            //
            if (PcdGet8 (PcdDisableActiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '4');
            }
          }
          break;

        //
        // _AL4 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '4')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {

            //
            // Check if we want _AL4 enabled
            //
            if (PcdGet8 (PcdDisableActiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '4');
            }
          }
          break;

        //
        // _AC5 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '5')):
          //
          // Conditional match.  _AC5 is < 63 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _AC5 enabled
            //
            if (PcdGet8 (PcdDisableActiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '5');
            }
          }
          break;

        //
        // _AL5 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '5')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {

            //
            // Check if we want _AL5 enabled
            //
            if (PcdGet8 (PcdDisableActiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '5');
            }
          }
          break;

        //
        // _AC6 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '6')):
          //
          // Conditional match.  _AC6 is < 63 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _AC6 enabled
            //
            if (PcdGet8 (PcdDisableActiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '6');
            }
          }
          break;

        //
        // _AL6 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '6')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {

            //
            // Check if we want _AL6 enabled
            //
            if (PcdGet8 (PcdDisableActiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '6');
            }
          }
          break;

        //
        // _AC7 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '7')):
          //
          // Conditional match.  _AC7 is < 63 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _AC7 enabled
            //
            if (PcdGet8 (PcdDisableActiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '7');
            }
          }
          break;

        //
        // _AL7 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '7')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {

            //
            // Check if we want _AL7 enabled
            //
            if (PcdGet8 (PcdDisableActiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '7');
            }
          }
          break;

        //
        // _AC8 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '8')):
          //
          // Conditional match.  _AC8 is < 63 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _AC8 enabled
            //
            if (PcdGet8 (PcdDisableActiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '8');
            }
          }
          break;

        //
        // _AL8 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '8')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {

            //
            // Check if we want _AL8 enabled
            //
            if (PcdGet8 (PcdDisableActiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '8');
            }
          }
          break;

        //
        // _AC9 method
        //
        case (SIGNATURE_32 ('_', 'A', 'C', '9')):
          //
          // Conditional match.  _AC9 is < 63 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _AC9 enabled
            //
            if (PcdGet8 (PcdDisableActiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'C', '9');
            }
          }
          break;

        //
        // _AL9 method
        //
        case (SIGNATURE_32 ('_', 'A', 'L', '9')):
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {

            //
            // Check if we want _AL9 enabled
            //
            if (PcdGet8 (PcdDisableActiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'A', 'L', '9');
            }
          }
          break;

        //
        // _PSL method
        //
        case (SIGNATURE_32 ('_', 'P', 'S', 'L')):
          //
          // Conditional match.  _PSL is < 256 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 3;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _PSL enabled
            //
            if (PcdGet8 (PcdDisablePassiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'P', 'S', 'L');
            }
          }
          break;

        //
        // _PSV method
        //
        case (SIGNATURE_32 ('_', 'P', 'S', 'V')):
          //
          // Conditional match.  _PSV is < 256 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 3;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _PSV enabled
            //
            if (PcdGet8 (PcdDisablePassiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'P', 'S', 'V');
            }
          }
          break;

        //
        // _CRT method
        //
        case (SIGNATURE_32 ('_', 'C', 'R', 'T')):
          //
          // Conditional match.  _CRT is < 256 bytes, so the package length is 1 byte.
          // Subtract 3 to check the Operator for CRB, subract 2 for Harris Beach.
          //
          Operation = DsdtPointer - 3;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _CRT enabled
            //
            if (PcdGet8 (PcdDisableCriticalTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'C', 'R', 'T');
            }
          }
          break;

        //
        // _TC1 method
        //
        case (SIGNATURE_32 ('_', 'T', 'C', '1')):
          //
          // Conditional match.  _TC1 is < 256 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _TC1 enabled
            //
            if (PcdGet8 (PcdDisablePassiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'T', 'C', '1');
            }
          }
          break;

        //
        // _TC2 method
        //
        case (SIGNATURE_32 ('_', 'T', 'C', '2')):
          //
          // Conditional match.  _TC2 is < 256 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _TC2 enabled
            //
            if (PcdGet8 (PcdDisablePassiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'T', 'C', '2');
            }
          }
          break;

        //
        // _TSP method
        //
        case (SIGNATURE_32 ('_', 'T', 'S', 'P')):
          //
          // Conditional match.  _TSP is < 256 bytes, so the package length is 1 byte.
          // Therefore, subtract 2 to check the Operator.
          //
          Operation = DsdtPointer - 2;
          if (*Operation == AML_METHOD_OP) {

            //
            // Check if we want _TSP enabled
            //
            if (PcdGet8 (PcdDisablePassiveTripPoints) == 0) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'T', 'S', 'P');
            }
          }
          break;

        //
        // Update SS3 Name with Setup value
        //
        case (SIGNATURE_32 ('S', 'S', '3', '_')):
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {
            Value   = (UINT8 *) DsdtPointer + 4;
            *Value  = PcdGet8 (PcdAcpiSleepState);
          }
          break;
        //
        // Update SS4 Name with Setup value
        //
        case (SIGNATURE_32 ('S', 'S', '4', '_')):
          Operation = DsdtPointer - 1;
          if (*Operation == AML_NAME_OP) {
            Value   = (UINT8 *) DsdtPointer + 4;
            *Value  = PcdGet8 (PcdAcpiHibernate);
          }
          break;
        //
        // _EJ0 method
        //
        case (SIGNATURE_32 ('_', 'E', 'J', '0')):
          if (PcdGet8 (PcdLowPowerS0Idle)) {
            //
            // Remove _EJ0 for SOC
            //
            if (*(DsdtPointer-3) == AML_METHOD_OP) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'E', 'J', '0');
              EnterDock = TRUE;
            }
          }
          break;
        //
        // _STA method for Device (\_SB.PCI0.DOCK)
        //
        case (SIGNATURE_32 ('_', 'S', 'T', 'A')):
          if (PcdGet8 (PcdLowPowerS0Idle)) {
            //
            // Remove _STA in (\_SB.PCI0.DOCK) for SOC
            //
            if ((*(DsdtPointer-3) == AML_METHOD_OP) && (EnterDock)) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'S', 'T', 'A');
              EnterDock = FALSE;
            }
          }
          break;
        //
        // _UPC method for Device (\_SB.PCI0.XHC.RHUB)
        //
        case (SIGNATURE_32('H', 'S', '1', '3')):
          for (TmpDsdtPointer = DsdtPointer;
               TmpDsdtPointer <= DsdtPointer + MaximumDsdtPointLength;
               TmpDsdtPointer++){
            Signature = (UINT32 *) TmpDsdtPointer;
            switch (*Signature) {
              case(SIGNATURE_32('U', 'P', 'C', 'P')):
                Value   = (UINT8 *)((UINT32 *)TmpDsdtPointer + 2);
                break;
              default:
                //
                // Do nothing.
                //
                break;
            }
          }
        break;


        //
        // _DCK method
        //
        case (SIGNATURE_32 ('_', 'D', 'C', 'K')):
          if (PcdGet8 (PcdLowPowerS0Idle)) {
            //
            // Remove _DCK for SOC
            //
            if (*(DsdtPointer-3) == AML_METHOD_OP) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'D', 'C', 'K');
            }
          }
          break;

        //
        // mask _DEP from CPU's scope if CS disabled.
        //
        case (SIGNATURE_32 ('P', 'R', '0', '0')):
        case (SIGNATURE_32 ('P', 'R', '0', '1')):
        case (SIGNATURE_32 ('P', 'R', '0', '2')):
        case (SIGNATURE_32 ('P', 'R', '0', '3')):
        case (SIGNATURE_32 ('P', 'R', '0', '4')):
        case (SIGNATURE_32 ('P', 'R', '0', '5')):
        case (SIGNATURE_32 ('P', 'R', '0', '6')):
        case (SIGNATURE_32 ('P', 'R', '0', '7')):
        case (SIGNATURE_32 ('P', 'R', '0', '8')):
        case (SIGNATURE_32 ('P', 'R', '0', '9')):
        case (SIGNATURE_32 ('P', 'R', '1', '0')):
        case (SIGNATURE_32 ('P', 'R', '1', '1')):
        case (SIGNATURE_32 ('P', 'R', '1', '2')):
        case (SIGNATURE_32 ('P', 'R', '1', '3')):
        case (SIGNATURE_32 ('P', 'R', '1', '4')):
        case (SIGNATURE_32 ('P', 'R', '1', '5')):

          if (PcdGet8 (PcdLowPowerS0Idle) == 0) {
            for (TmpDsdtPointer = DsdtPointer; TmpDsdtPointer <= DsdtPointer + MaximumDsdtPointLength; TmpDsdtPointer++){
              Signature = (UINT32 *) TmpDsdtPointer;
              switch (*Signature) {
                case(SIGNATURE_32('_', 'D', 'E', 'P')):
                  *(UINT8 *) TmpDsdtPointer = 'X';
                  break;
                default:
                  //
                  // Do nothing.
                  //
                  break;
              }
            }
          }
          break;

        //
        // _EDL name
        //
        case (SIGNATURE_32 ('_', 'E', 'D', 'L')):
          if (PcdGet8 (PcdLowPowerS0Idle)) {
            //
            // Remove _EDL for SOC
            //
            if (*(DsdtPointer-1) == AML_NAME_OP) {
              Signature   = (UINT32 *) DsdtPointer;
              *Signature  = SIGNATURE_32 ('X', 'E', 'D', 'L');
            }
          }
          break;

        default:
          //
          // Do nothing.
          //
          break;
        }
      }
}

