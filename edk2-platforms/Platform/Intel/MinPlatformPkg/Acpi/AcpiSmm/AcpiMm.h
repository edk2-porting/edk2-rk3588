/** @file
  Internal header file for the ACPI MM driver.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
Copyright (c) Microsoft Corporation.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _ACPI_MM_H_
#define _ACPI_MM_H_

/**
  ACPI initialization logic shared between the Traditional MM and
  Standalone MM driver instances.

**/
VOID
InitializeAcpiMm (
  VOID
  );

#endif
