/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _printf_h
#define _printf_h

#include "DataTypes.h"

#define PRINTF_CONTROL_OUTOF_SYNC_ERR_MAJOR   0xCF
#define PRINTF_CONTROL_OUTOF_SYNC_ERR_MINOR   0x01

#define TAB_STOP            4
#define LEFT_JUSTIFY        0x01
#define PREFIX_SIGN         0x02
#define PREFIX_BLANK        0x04
#define COMMON_PREFIX_ZERO  0x08
#define LONG_TYPE           0x10

#define INT_SIGNED          0x20
#define COMA_TYPE           0x40
#define LONG_LONG_TYPE      0x80
#define TO_UPPER            0x100

#define CHAR_CR             0x0d
#define CHAR_LF             0x0a

//
// ANSI Escape sequences for color
//
#define ANSI_FOREGROUND_BLACK      30
#define ANSI_FOREGROUND_RED        31
#define ANSI_FOREGROUND_GREEN      32
#define ANSI_FOREGROUND_YELLOW     33
#define ANSI_FOREGROUND_BLUE       34
#define ANSI_FOREGROUND_MAGENTA    35
#define ANSI_FOREGROUND_CYAN       36
#define ANSI_FOREGROUND_WHITE      37

#define ANSI_BACKGROUND_BLACK      40
#define ANSI_BACKGROUND_RED        41
#define ANSI_BACKGROUND_GREEN      42
#define ANSI_BACKGROUND_YELLOW     43
#define ANSI_BACKGROUND_BLUE       44
#define ANSI_BACKGROUND_MAGENTA    45
#define ANSI_BACKGROUND_CYAN       46
#define ANSI_BACKGROUND_WHITE      47

#define ANSI_ATTRIBUTE_OFF         0
#define ANSI_ATTRIBUTE_BOLD        1
#define ANSI_ATTRIBUTE_UNDERSCORE  4
#define ANSI_ATTRIBUTE_BLINK       5
#define ANSI_ATTRIBUTE_REVERSE     7
#define ANSI_ATTRIBUTE_CONCEAL     8

#ifndef INT32_MAX
#define INT32_MAX             0x7fffffffU
#endif

#ifndef va_start
typedef INT8 *  va_list;
#define _INTSIZEOF(n)   ((sizeof (n) + sizeof (UINT32) - 1) &~(sizeof (UINT32) - 1))
#define va_start(ap, v) (ap = (va_list) & v + _INTSIZEOF (v))
#define va_arg(ap, t)   (*(t *) ((ap += _INTSIZEOF (t)) - _INTSIZEOF (t)))
#define va_end(ap)      (ap = (va_list) 0)
#endif

#define ISDIGIT(_c)     (((_c) >= '0') && ((_c) <= '9'))
#define ISHEXDIGIT(_c)  (((_c) >= 'a') && ((_c) <= 'f'))

#endif // _printf_h
