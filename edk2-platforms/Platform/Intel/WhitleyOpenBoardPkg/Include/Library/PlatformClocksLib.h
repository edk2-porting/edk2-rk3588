/** @file

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __PLATFORM_CLOCKS_LIB__
#define __PLATFORM_CLOCKS_LIB__

//
// Known clock generator types
//
typedef enum {
  ClockGeneratorCk410,
  ClockGeneratorCk420,
  ClockGeneratorCk440,
  ClockGeneratorCk505,
  ClockGeneratorMax
} CLOCK_GENERATOR_TYPE;

#define CLOCK_GENERATOR_ADDRESS 0xD2
//
// Clock generator details table
//
typedef struct {
  CLOCK_GENERATOR_TYPE      ClockType;
  UINT8                     ClockId;
  UINT8                     SpreadSpectrumByteOffset;
  UINT8                     SpreadSpectrumBitOffset;
} CLOCK_GENERATOR_DETAILS;

//
// An arbitrary maximum length for clock generator buffers
//
#define MAX_CLOCK_GENERATOR_BUFFER_LENGTH           0x20

//
// CK410 Definitions
//
#define CK410_GENERATOR_ID                          0x65
#define CK410_GENERATOR_SPREAD_SPECTRUM_BYTE        1
#define CK410_GENERATOR_SPREAD_SPECTRUM_BIT         BIT0
#define CK410_GENERATOR_CLOCK_FREERUN_BYTE          4
#define CK410_GENERATOR_CLOCK_FREERUN_BIT           (BIT0 | BIT1 | BIT2)

//
// CK420 Definitions
//
#define CK420_GENERATOR_ID                          0x11 // IDT ICS932SQ420B
#define CK420_GENERATOR_SPREAD_SPECTRUM_BYTE        1
#define CK420_GENERATOR_SPREAD_SPECTRUM_BIT         BIT0

//
// CK440 Definitions
//
#define CK440_GENERATOR_ID                          0x12
#define CK440_GENERATOR_SPREAD_SPECTRUM_BYTE        6
#define CK440_GENERATOR_SPREAD_SPECTRUM_BIT         BIT2

//
// CK505 Definitions
//
#define CK505_GENERATOR_ID                          0x26 // Silego SLG505YC264B
#define CK505_GENERATOR_SPREAD_SPECTRUM_BYTE        4
#define CK505_GENERATOR_SPREAD_SPECTRUM_BIT         (BIT0 | BIT1)

#define CLOCK_GENERATOR_SETTINGS_PLATFORMSRP {0xFF, 0x9E, 0x3F, 0x00, 0x00, 0x0F, 0x08, 0x31, 0x0A, 0x17, 0xFF, 0xFE}
#define CLOCK_GENERATOR_SETTINGS_PLATFORMDVP {0xFF, 0x9E, 0x3F, 0x00, 0x00, 0x0F, 0x08, 0x31, 0x0A, 0x17}

EFI_STATUS
ConfigureClockGenerator (
  IN     EFI_PEI_SERVICES         **PeiServices,
  IN     CLOCK_GENERATOR_TYPE     ClockType,
  IN     UINT8                    ClockAddress,
  IN     UINTN                    ConfigurationTableLength,
  IN OUT UINT8                    *ConfigurationTable,
  IN     BOOLEAN                  EnableSpreadSpectrum,
  IN     CLOCK_GENERATOR_DETAILS  *mSupportedClockGeneratorT,
  IN     BOOLEAN                  SecondarySmbus
  );

#define PEI_STALL_RESOLUTION  1

#endif

