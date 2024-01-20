/** @file
 *
 *  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
 *  Copyright (c) 2024, CoolStar. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/
#include "AcpiTables.h"

#define RK3588_PLL_AUPLL_RATE   786432000
#define RK3588_PLL_CPLL_RATE    1500000000

#define I2S_INITIAL_CLOCK  (256 * 48000)

#define I2S_DSM_UUID  ToUUID ("7056bfa1-af0b-48e5-b67f-139f2004a26a")

#if FixedPcdGetBool(PcdI2S0Supported)
  Device (I2S0) {
    Name (_HID, "RKCP3003")
    Name (_UID, 0)
    Name (_CCA, 0)

    Method (_CRS, 0x0, Serialized) {
      Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xfe470000, 0x1000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 212 }
      })
      Return (RBUF)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) { "rockchip,dma", "DMA0" },
        Package (2) { "rockchip,tx", 0 },
        Package (2) { "rockchip,rx", 1 },
        Package (2) { "rockchip,tplg", BOARD_I2S0_TPLG },
      }
    })
    Name (_DEP, Package () { \_SB.DMA0 })

    Method (_INI, 0, Serialized) {
      _DSM (I2S_DSM_UUID, 0, 1, Package () { I2S_INITIAL_CLOCK })
    }

    Method (_DSM, 4, Serialized) {
      // CRU_CLKSEL_CON24
      OperationRegion (CRU, SystemMemory, 0xFD7C0360, 0x8)
      Field (CRU, DWordAcc, NoLock, Preserve) {
        C24, 32,
        C25, 32
      }

      // Check the UUID
      If (Arg0 == I2S_DSM_UUID) {
        // Check the revision
        If (Arg1 >= 0) {
          // Check the function index
          Switch (ToInteger (Arg2)) {
            //
            // Supported functions:
            // Bit 0 - Indicates support for functions other than 0
            // Bit 1 - Indicates support for setting TX master clock
            // Bit 2 - Indicates support for setting RX master clock
            //
            Case (0) {
              Return (Buffer () { 0x03 })
            }

            //
            // Function Index 1: Set TX master clock
            //   Takes the target clock rate in Hz, returns the actual set rate.
            //
            Case (1) {
              // Source divided by clk_i2s0_8ch_tx_src_div + 1
              Local0 = RK3588_PLL_AUPLL_RATE / (((C24 >> 4) & 0xf) + 1)

              // Compute numerator/denumerator
              Local1 = FRBA (DerefOf (Arg3[0]), Local0, 0xFFFF, 0xFFFF)

              // Set fractional register
              C25 = (DerefOf (Local1[0])) << 16 | DerefOf (Local1[1])

              Return ((Local0 * DerefOf (Local1[0])) / DerefOf (Local1[1]))
            }
          } // Function index check
        } // Revision check
      } // UUID check
      Return (Buffer () { 0x0 })
    } // _DSM
  }
#endif

#if FixedPcdGetBool(PcdI2S1Supported)
  Device (I2S1) {
    Name (_HID, "RKCP3003")
    Name (_UID, 1)
    Name (_CCA, 0)

    Method (_CRS, 0x0, Serialized) {
      Name (RBUF, ResourceTemplate() {
        Memory32Fixed (ReadWrite, 0xfe480000, 0x1000)
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 213 }
      })
      Return (RBUF)
    }
    Name (_DSD, Package () {
      ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) { "rockchip,dma", "DMA0" },
        Package (2) { "rockchip,tx", 2 },
        Package (2) { "rockchip,rx", 3 },
        Package (2) { "rockchip,tplg", BOARD_I2S1_TPLG },
        // XXX: Currently I2S1 is only used by Orange Pi 5.
        // The following properties should be board specific
        Package (2) { "rockchip,i2s-tx-route", 0x3210 },,
        Package (2) { "rockchip,i2s-rx-route", 0x1320 },
      }
    })
    Name (_DEP, Package () { \_SB.DMA0 })

    Method (_INI, 0, Serialized) {
      _DSM (I2S_DSM_UUID, 0, 1, Package () { I2S_INITIAL_CLOCK })
    }

    Method (_DSM, 4, Serialized) {
      // PMU1CRU_CLKSEL_CON05
      OperationRegion (CRU, SystemMemory, 0xFD7F0314, 0x8)
      Field (CRU, DWordAcc, NoLock, Preserve) {
        C05, 32,
        C06, 32
      }

      // Check the UUID
      If (Arg0 == I2S_DSM_UUID) {
        // Check the revision
        If (Arg1 >= 0) {
          // Check the function index
          Switch (ToInteger (Arg2)) {
            //
            // Supported functions:
            // Bit 0 - Indicates support for functions other than 0
            // Bit 1 - Indicates support for setting TX master clock
            // Bit 2 - Indicates support for setting RX master clock
            //
            Case (0) {
              Return (Buffer () { 0x03 })
            }

            //
            // Function Index 1: Set TX master clock
            //   Takes the target clock rate in Hz, returns the actual set rate.
            //
            Case (1) {
              // Source divided by clk_i2s1_8ch_tx_src_div + 1
              Local0 = RK3588_PLL_CPLL_RATE / (((C05 >> 2) & 0xf) + 1)

              // Compute numerator/denumerator
              Local1 = FRBA (DerefOf (Arg3[0]), Local0, 0xFFFF, 0xFFFF)

              // Set fractional register
              C06 = (DerefOf (Local1[0])) << 16 | DerefOf (Local1[1])

              Return ((Local0 * DerefOf (Local1[0])) / DerefOf (Local1[1]))
            }
          } // Function index check
        } // Revision check
      } // UUID check
      Return (Buffer () { 0x0 })
    } // _DSM
  }
#endif

  //
  // FRBA - Find Rational Best Approximation
  //
  // Arguments: (4)
  //   Arg0 - Given numerator
  //   Arg1 - Given denominator
  //   Arg2 - Maximum numerator
  //   Arg3 - Maximum denominator
  //
  // Return:
  //   Package of two integers: best numerator, best denominator
  //
  Function (FRBA, PkgObj, { IntObj, IntObj, IntObj, IntObj }) {
    //
    // Variables used throughout this function:
    //   Local0, Local1 - Remaining numerator/denominator for Euclidean algorithm
    //   Local2, Local4 - Previous numerator/denominator convergent
    //   Local3, Local5 - Current numerator/denominator convergent
    //   Local6 - Temporary variable for swapping values around
    //   Local7 - Next term in the continued fraction
    //
    Local0 = Arg0
    Local1 = Arg1
    Local2 = 0
    Local3 = 1
    Local4 = 1
    Local5 = 0

    While (1) {
      // Reached limits? Fall back to previous convergent.
      If (Local3 > Arg2 || Local5 > Arg3) {
        Local3 = Local2
        Local5 = Local4
        Break
      }

      // No remainder? We're done.
      If (Local1 == 0) {
        Break
      }

      // Find next term for convergent
      Local7 = Local0 / Local1

      // Set remainder
      Local6 = Local1
      Local1 = Local0 % Local1
      Local0 = Local6

      // Calculate numerator and save previous one
      Local6 = Local2 + Local7 * Local3
      Local2 = Local3
      Local3 = Local6

      // Caculate denominator and save previous one
      Local6 = Local4 + Local7 * Local5
      Local4 = Local5
      Local5 = Local6
    }

    // Prepare return object
    Local0 = Package (2) { 0, 0 }
    Local0[0] = Local3
    Local0[1] = Local5

    Return (Local0)
  }
