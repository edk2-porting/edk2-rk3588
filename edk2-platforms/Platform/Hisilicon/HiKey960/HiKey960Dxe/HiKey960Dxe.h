/** @file
*
*  Copyright (c) 2018, Linaro Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __HIKEY960DXE_H__
#define __HIKEY960DXE_H__

#define ADB_REBOOT_ADDRESS               0x32100000
#define ADB_REBOOT_BOOTLOADER            0x77665500
#define ADB_REBOOT_NONE                  0x77665501

#define DETECT_SW_FASTBOOT               68        // GPIO8_4

#define HIKEY960_BOOT_OPTION_NUM         4

enum {
  BOOT_MODE_RECOVERY  = 0,
  BOOT_MODE_MASK = 1,
};

#endif /* __HIKEY960DXE_H__ */
