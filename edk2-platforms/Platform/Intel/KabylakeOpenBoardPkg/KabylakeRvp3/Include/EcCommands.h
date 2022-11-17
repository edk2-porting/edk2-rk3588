/** @file
  Definition for supported EC commands.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef EC_COMMANDS_H_
#define EC_COMMANDS_H_

//
// Timeout if EC command/data fails
//
#define EC_TIME_OUT        0x20000

//
// The EC implements an embedded controller interface at ports 0x60/0x64 and a ACPI compliant
// system management controller at ports 0x62/0x66. Port 0x66 is the command and status port,
// port 0x62 is the data port.
//
#define EC_D_PORT        0x62
#define EC_C_PORT        0x66

//
// Status Port 0x62
//
#define EC_S_OVR_TMP     0x80    // Current CPU temperature exceeds the threshold
#define EC_S_SMI_EVT     0x40    // SMI event is pending
#define EC_S_SCI_EVT     0x20    // SCI event is pending
#define EC_S_BURST       0x10    // EC is in burst mode or normal mode
#define EC_S_CMD         0x08    // Byte in data register is command/data
#define EC_S_IGN         0x04    // Ignored
#define EC_S_IBF         0x02    // Input buffer is full/empty
#define EC_S_OBF         0x01    // Output buffer is full/empty

//
// EC commands that are issued to the EC through the command port (0x66).
// New commands and command parameters should only be written by the host when IBF=0.
// Data read from the EC data port is valid only when OBF=1.
//
#define EC_C_FAB_ID                    0x0D    // Get the board fab ID in the lower 3 bits
#define EC_C_ACPI_READ                 0x80    // Read a byte of EC RAM
#define EC_C_ACPI_WRITE                0x81    // Write a byte of EC RAM

#endif // EC_COMMANDS_H_
