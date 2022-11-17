/** @file

  Copyright (c) 2017, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __SYNQUACER_I2C_DXE_H__
#define __SYNQUACER_I2C_DXE_H__

#include <PiDxe.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeLib.h>

#include <Protocol/I2cMaster.h>
#include <Protocol/NonDiscoverableDevice.h>

extern EFI_COMPONENT_NAME2_PROTOCOL gSynQuacerI2cDriverComponentName2;

#define SYNQUACER_I2C_SIGNATURE         SIGNATURE_32 ('S', 'I', '2', 'C')
#define SYNQUACER_I2C_FROM_THIS(a)      CR ((a), SYNQUACER_I2C_MASTER, \
                                            I2cMaster, SYNQUACER_I2C_SIGNATURE)

#pragma pack(1)
typedef struct {
  VENDOR_DEVICE_PATH              Vendor;
  UINT64                          MmioBase;
  EFI_DEVICE_PATH_PROTOCOL        End;
} SYNQUACER_I2C_DEVICE_PATH;
#pragma pack()

typedef struct {
  UINT32                          Signature;
  EFI_I2C_MASTER_PROTOCOL         I2cMaster;
  EFI_PHYSICAL_ADDRESS            MmioBase;
  SYNQUACER_I2C_DEVICE_PATH       DevicePath;
  NON_DISCOVERABLE_DEVICE         *Dev;
  EFI_EVENT                       VirtualAddressChangeEvent;
  BOOLEAN                         Runtime;
} SYNQUACER_I2C_MASTER;

EFI_STATUS
SynQuacerI2cInit (
  IN      EFI_HANDLE              DriverBindingHandle,
  IN      EFI_HANDLE              ControllerHandle
  );

EFI_STATUS
SynQuacerI2cRelease (
  IN      EFI_HANDLE              DriverBindingHandle,
  IN      EFI_HANDLE              ControllerHandle
  );

#define REFCLK_RATE         FixedPcdGet32 (PcdI2cReferenceClock)

#define DIV_ROUND_UP(n, d)  (((n) + (d) - 1) / (d))

#define F_I2C_SPEED_SM      100000
#define F_I2C_SPEED_FM      400000

// I2C register adress definitions
#define F_I2C_REG_BSR       (0x00 << 2) // Bus Status Regster
#define F_I2C_REG_BCR       (0x01 << 2) // Bus Control Register
#define F_I2C_REG_CCR       (0x02 << 2) // Clock Control Register
#define F_I2C_REG_ADR       (0x03 << 2) // Address Register
#define F_I2C_REG_DAR       (0x04 << 2) // Data Register
#define F_I2C_REG_CSR       (0x05 << 2) // Expansion CS Register
#define F_I2C_REG_FSR       (0x06 << 2) // Bus Clock Frequency Register
#define F_I2C_REG_BC2R      (0x07 << 2) // Bus Control 2 Register

// I2C register bit definitions
#define F_I2C_BSR_FBT       BIT0        // First Byte Transfer
#define F_I2C_BSR_GCA       BIT1        // General Call Address
#define F_I2C_BSR_AAS       BIT2        // Address as Slave
#define F_I2C_BSR_TRX       BIT3        // Transfer/Receive
#define F_I2C_BSR_LRB       BIT4        // Last Received Bit
#define F_I2C_BSR_AL        BIT5        // Arbitration Lost
#define F_I2C_BSR_RSC       BIT6        // Repeated Start Condition
#define F_I2C_BSR_BB        BIT7        // Bus Busy

#define F_I2C_BCR_INT       BIT0        // Interrupt
#define F_I2C_BCR_INTE      BIT1        // Interrupt Enable
#define F_I2C_BCR_GCAA      BIT2        // General Call Access Acknowledge
#define F_I2C_BCR_ACK       BIT3        // Acknowledge
#define F_I2C_BCR_MSS       BIT4        // Master Slave Select
#define F_I2C_BCR_SCC       BIT5        // Start Condition Continue
#define F_I2C_BCR_BEIE      BIT6        // Bus Error Interrupt Enable
#define F_I2C_BCR_BER       BIT7        // Bus Error

#define F_I2C_CCR_CS_MASK   0x1f        // CCR Clock Period Select
#define F_I2C_CCR_EN        BIT5        // Enable
#define F_I2C_CCR_FM        BIT6        // Speed Mode Select

#define F_I2C_CSR_CS_MASK   0x3f        // CSR Clock Period Select

#define F_I2C_BC2R_SCLL     BIT0        // SCL Low Drive
#define F_I2C_BC2R_SDAL     BIT1        // SDA Low Drive
#define F_I2C_BC2R_SCLS     BIT4        // SCL Status
#define F_I2C_BC2R_SDAS     BIT5        // SDA Status

// PCLK frequency
#define F_I2C_BUS_CLK_FR(rate)          ((rate) / 20000000 + 1)

#define F_I2C_MIN_CLK_RATE              (14 * 1000000)  // min frequency 14 MHz
#define F_I2C_MAX_CLK_RATE              (200 * 1000000) // max frequency 200 MHz
#define F_I2C_CLK_RATE_18M              (18 * 1000000)  // threshold freq 18 MHz

// STANDARD MODE frequency
#define F_I2C_CLK_MASTER_STANDARD(rate) \
            DIV_ROUND_UP (DIV_ROUND_UP ((rate), 100000) - 2, 2)

// FAST MODE frequency
#define F_I2C_CLK_MASTER_FAST(rate) \
            DIV_ROUND_UP ((DIV_ROUND_UP ((rate), 400000) - 2) * 2, 3)

// (clkrate <= 18000000)
// calculate the value of CS bits in CCR register in standard mode
#define F_I2C_CCR_CS_STANDARD_MAX_18M(rate) \
            ((F_I2C_CLK_MASTER_STANDARD (rate) - 65) & F_I2C_CCR_CS_MASK)

// calculate the value of CS bits in CSR register in standard mode
#define F_I2C_CSR_CS_STANDARD_MAX_18M(rate) 0x00

// calculate the value of CS bits in CCR register in fast mode
#define F_I2C_CCR_CS_FAST_MAX_18M(rate) \
            ((F_I2C_CLK_MASTER_FAST (rate) - 1)  & F_I2C_CCR_CS_MASK)

// calculate the value of CS bits in CSR register in fast mode
#define F_I2C_CSR_CS_FAST_MAX_18M(rate) 0x00

// (clkrate > 18000000)
// calculate the value of CS bits in CCR register in standard mode */
#define F_I2C_CCR_CS_STANDARD_MIN_18M(rate) \
            ((F_I2C_CLK_MASTER_STANDARD (rate) - 1) & F_I2C_CCR_CS_MASK)

// calculate the value of CS bits in CSR register in standard mode
#define F_I2C_CSR_CS_STANDARD_MIN_18M(rate) \
            (((F_I2C_CLK_MASTER_STANDARD (rate) - 1) >> 5) & F_I2C_CSR_CS_MASK)

// calculate the value of CS bits in CCR register in fast mode
#define F_I2C_CCR_CS_FAST_MIN_18M(rate) \
            ((F_I2C_CLK_MASTER_FAST (rate) - 1) & F_I2C_CCR_CS_MASK)

/* calculate the value of CS bits in CSR register in fast mode */
#define F_I2C_CSR_CS_FAST_MIN_18M(rate) \
            (((F_I2C_CLK_MASTER_FAST (rate) - 1) >> 5) & F_I2C_CSR_CS_MASK)

#endif
