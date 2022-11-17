/** @file

  Copyright (c) 2018-2019, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>

#ifndef _SECURE96_H_
#define _SECURE96_H_

#define ATSHA204A_SLAVE_ADDRESS         0x60
#define ATSHA204A_DT_NODENAME           atsha204a@60

#define ATECC508A_SLAVE_ADDRESS         0x51
#define ATECC508A_DT_NODENAME           atecc508a@51

#define INFINEON_SLB9670_DT_NODENAME    __CONCATENATE(tpm@,SECURE96_SPI0_CS)

#ifndef SECURE96_SPI0_CS
#define SECURE96_SPI0_CS                0
#endif

#ifndef SECURE96_ACPI_GPIO
#define SECURE96_ACPI_GPIO              "\\_SB.GPIO"
#endif

#ifndef SECURE96_ACPI_I2C0
#define SECURE96_ACPI_I2C0              "\\_SB.I2C0"
#endif

#ifndef SECURE96_ACPI_SPI0
#define SECURE96_ACPI_SPI0              "\\_SB.SPI0"
#endif

#endif // _SECURE96_H_
