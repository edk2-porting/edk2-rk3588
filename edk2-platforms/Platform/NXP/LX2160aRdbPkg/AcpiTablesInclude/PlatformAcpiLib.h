/** @file
 *  Acpi lib headers
 *
 *  Copyright 2020 NXP
 *  Copyright 2020 Puresoftware Ltd
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
**/


#ifndef LX2160ARDB_PLATFORM_ACPI_LIB_H
#define LX2160ARDB_PLATFORM_ACPI_LIB_H

#include <PlatformAcpiTableGenerator.h>

/** C array containing the compiled AML template.
    These symbols are defined in the auto generated C file
    containing the AML bytecode array.
*/
extern CHAR8  dsdt_aml_code[];

#endif
