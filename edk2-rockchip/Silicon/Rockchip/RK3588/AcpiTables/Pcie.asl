/** @file
 *
 *  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include "AcpiTables.h"

#define PCIE_ROOT_COMPLEX(Segment, LegacyIrq)                                  \
  Device (PCI ## Segment) {                                                    \
    Name (_HID, "PNP0A08")                                                     \
    Name (_CID, "PNP0A03")                                                     \
    Name (_CCA, 0)                                                             \
    Name (_UID, Segment)                                                       \
    Name (_SEG, Segment)                                                       \
    Method (_BBN) {                                                            \
      If (PBOF) {                                                              \
        Return (PCIE_BUS_BASE (Segment) + PBMI)                                \
      } Else {                                                                 \
        Return (PBMI)                                                          \
      }                                                                        \
    }                                                                          \
    Name (_STA, 0xF)                                                           \
                                                                               \
    Name (_PRT, Package() {                                                    \
      Package (4) { 0x0FFFF, 0, 0, LegacyIrq },                                \
      Package (4) { 0x0FFFF, 1, 0, LegacyIrq },                                \
      Package (4) { 0x0FFFF, 2, 0, LegacyIrq },                                \
      Package (4) { 0x0FFFF, 3, 0, LegacyIrq }                                 \
    })                                                                         \
                                                                               \
    Method (_CRS, 0, Serialized) {                                             \
      Name (RBUF, ResourceTemplate () {                                        \
        WORDBUSNUMBER_BUF (00, ResourceProducer)                               \
        QWORDMEMORY_BUF (01, ResourceProducer)                                 \
        QWORDMEMORY_BUF (02, ResourceProducer)                                 \
        QWORDIO_BUF (03, ResourceProducer)                                     \
      })                                                                       \
      WORD_SET (                                                               \
        00,                                                                    \
        _BBN,                                                                  \
        PBMA - PBMI + 1,                                                       \
        0                                                                      \
      )                                                                        \
      QWORD_SET (                                                              \
        01,                                                                    \
        PCIE_MEM32_BUS_BASE,                                                   \
        PCIE_MEM32_SIZE,                                                       \
        PCIE_MEM32_TRANSLATION (Segment)                                       \
      )                                                                        \
      QWORD_SET (                                                              \
        02,                                                                    \
        PCIE_MEM64_BASE (Segment),                                             \
        PCIE_MEM64_SIZE,                                                       \
        0                                                                      \
      )                                                                        \
      QWORD_SET (                                                              \
        03,                                                                    \
        PCIE_IO_BUS_BASE,                                                      \
        PCIE_IO_SIZE,                                                          \
        PCIE_IO_TRANSLATION (Segment)                                          \
      )                                                                        \
      Return (RBUF)                                                            \
    }                                                                          \
                                                                               \
    Device (RES0) {                                                            \
      Name (_HID, "AMZN0001")                                                  \
      Name (_CID, "PNP0C02")                                                   \
      Name (_UID, Segment)                                                     \
      Method (_CRS, 0, Serialized) {                                           \
        Name (RBUF, ResourceTemplate () {                                      \
          QWORDMEMORY_BUF (00, ResourceProducer)                               \
        })                                                                     \
        QWORD_SET (00, PCIE_DBI_BASE (Segment), PCIE_DBI_SIZE, 0)              \
        Return (RBUF)                                                          \
      }                                                                        \
    }                                                                          \
                                                                               \
    Device (RES1) {                                                            \
      Name (_HID, "PNP0C02")                                                   \
      Name (_UID, Segment + 1)                                                 \
      Method (_CRS, 0, Serialized) {                                           \
        Name (RBUF, ResourceTemplate () {                                      \
          QWORDMEMORY_BUF (00, ResourceProducer)                               \
        })                                                                     \
        QWORD_SET (00, PCIE_CFG_BASE (Segment), PCIE_CFG_SIZE , 0)             \
        Return (RBUF)                                                          \
      }                                                                        \
    }                                                                          \
                                                                               \
    Name (SUPP, Zero) /* PCI _OSC Support Field value */                       \
    Name (CTRL, Zero) /* PCI _OSC Control Field value */                       \
                                                                               \
    Method (_OSC, 4) {                                                         \
      If (Arg0 == ToUUID ("33DB4D5B-1FF7-401C-9657-7441C03DD766")) {           \
        /* Create DWord-adressable fields from the Capabilities Buffer */      \
        CreateDWordField (Arg3, 0, CDW1)                                       \
        CreateDWordField (Arg3, 4, CDW2)                                       \
        CreateDWordField (Arg3, 8, CDW3)                                       \
                                                                               \
        /* Save Capabilities DWord2 & 3 */                                     \
        SUPP = CDW2                                                            \
        CTRL = CDW3                                                            \
                                                                               \
        /* Mask out native hot plug control */                                 \
        CTRL &= 0x1E                                                           \
                                                                               \
        /* Always allow native PME, AER and Capability Structure control */    \
        /* Never allow SHPC and LTR control */                                 \
        CTRL &= 0x1D                                                           \
                                                                               \
        /* Unknown revision */                                                 \
        If (Arg1 != 1) {                                                       \
          Cdw1 |= 0x08                                                         \
        }                                                                      \
                                                                               \
        /* Capabilities bits were masked */                                    \
        If (CDW3 != CTRL) {                                                    \
          CDW1 |= 0x10                                                         \
        }                                                                      \
                                                                               \
        /* Update DWORD3 in the buffer */                                      \
        CDW3 = CTRL                                                            \
        Return (Arg3)                                                          \
      } Else {                                                                 \
        /* Unrecognized UUID */                                                \
        CDW1 |= 4                                                              \
        Return (Arg3)                                                          \
      }                                                                        \
    }                                                                          \
  }

Scope (\_SB_) {
  Name (PBMI, 0xABCD)   // PCI Bus Minimum
  Name (PBMA, 0xABCD)   // PCI Bus Maximum
  Name (PBOF, 1)        // PCI Bus Offset

  PCIE_ROOT_COMPLEX (0, 292)
  PCIE_ROOT_COMPLEX (1, 287)
  PCIE_ROOT_COMPLEX (2, 272)
  PCIE_ROOT_COMPLEX (3, 277)
  PCIE_ROOT_COMPLEX (4, 282)
}
