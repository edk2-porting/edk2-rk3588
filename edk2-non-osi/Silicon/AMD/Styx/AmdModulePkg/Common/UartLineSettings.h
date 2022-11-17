/**
 * @file
 *
 * Generic UART line setting values. These are shared between UEFI and the SCP.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      STYX
 * @e sub-project:  (TBD)
 * @e \$Revision$        @e  \$Date$
 *
 **/
/*****************************************************************************
 *
 *  Copyright 2015 - 2016 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
 *
 *  This program and the accompanying materials are licensed and made available
 *  under the terms and conditions of the BSD License which accompanies this
 *  distribution. The full text of the license may be found at
 *  http://opensource.org/licenses/bsd-license.php
 *
 *  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR
 *  IMPLIED.
 *
 * ***************************************************************************
 **/

#ifndef _UART_LINE_SETTINGS_H_
#define _UART_LINE_SETTINGS_H_

//#########################################################################
//#########################################################################
//#########################################################################
//        NOTE: This file shared between SCP and UEFI, make sure all     //
//              changes are reflected in both copies.                    //
//#########################################################################
//#########################################################################
//#########################################################################


/// UART Baudrate enum
typedef enum {
  /// This subset is defined/used in UEFI
  UART_BAUDRATE_9600    = 9600,       ///< 9600 Baudrate
  UART_BAUDRATE_19200   = 19200,      ///< 19200 Baudrate
  UART_BAUDRATE_38400   = 38400,      ///< 38400 Baudrate
  UART_BAUDRATE_57600   = 57600,      ///< 57600 Baudrate
  UART_BAUDRATE_115200  = 115200,     ///< 115200 Baudrate

  /// These could be used within the SCP.
  UART_BAUDRATE_110     = 110,        ///< 110 Baudrate
  UART_BAUDRATE_300     = 300,        ///< 300 Baudrate
  UART_BAUDRATE_600     = 600,        ///< 600 Baudrate
  UART_BAUDRATE_1200    = 1200,       ///< 1200 Baudrate
  UART_BAUDRATE_2400    = 2400,       ///< 2400 Baudrate
  UART_BAUDRATE_4800    = 4800,       ///< 4800 Baudrate
  UART_BAUDRATE_14400   = 14400,      ///< 14400 Baudrate
  UART_BAUDRATE_230400  = 230400,     ///< 230400 Baudrate
  UART_BAUDRATE_460800  = 460800,     ///< 460800 Baudrate
  UART_BAUDRATE_921600  = 921600,     ///< 921600 Baudrate
} UART_BAUDRATE;

/// UART Parity enum
typedef enum {
  DEFAULT_PARITY = 0,                 ///< Default Parity
  NO_PARITY,                          ///< No Parity
  EVEN_PARITY,                        ///< Even Parity
  ODD_PARITY,                         ///< Odd Parity
  MARK_PARITY,                        ///< Mark Parity
  SPACE_PARITY                        ///< Space Parity
} UART_PARITY;

/// UART Stop Bit enum
typedef enum {
  UART_STOP_BIT_0 = 0,                 ///< No Stop Bits
  UART_STOP_BIT_1,                     ///< One Stop Bit
  UART_STOP_BIT_1_5,                   ///< One and One Half Stop bits
  UART_STOP_BIT_2                      ///< Two Stop Bits
} UART_STOP_BITS;

/// UART Data Length enum
typedef enum {
  UART_DATA_BITS_5 = 5,                ///< Five Data Bits
  UART_DATA_BITS_6,                    ///< Six Data Bits
  UART_DATA_BITS_7,                    ///< Seven Data Bits
  UART_DATA_BITS_8,                    ///< Eight Data Bits
} UART_DATA_BITS;

/// UART Line Settings structure
typedef struct _UART_LINE_SETTINGS {
  UART_BAUDRATE   BaudRate;            ///< UART Baudrate
  UART_DATA_BITS  DataBits;            ///< UART Data Bits
  UART_PARITY     Parity;              ///< UART Parity
  UART_STOP_BITS  StopBits;            ///< UART Stop Bits
} UART_LINE_SETTINGS;

#endif /* _UART_LINE_SETTINGS_H_ */
