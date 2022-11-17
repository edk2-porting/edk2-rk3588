/* $NoKeywords */
/**
 * @file
 *
 * MemIscp.h
 *
 * Contains common Memory Training ISCP-related structures and defines.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: FDK
 * @e sub-project: UEFI
 * @e version: $Revision: 317558 $ @e date: $Date: 2015-04-24 17:20:55 -0700 (Fri, 24 Apr 2015) $
 *
 */
/*****************************************************************************
*
*  Copyright 2013 - 2016 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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
***************************************************************************/

//#########################################################################
//#########################################################################
//#########################################################################
//        NOTE: This file shared between SCP and UEFI, make sure all     //
//              changes are reflected in both copies.                    //
//#########################################################################
//#########################################################################
//#########################################################################

#ifndef CPUISCP_H_
#define CPUISCP_H_

#ifdef __cplusplus
  extern "C" {
  #endif

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
  #include <ProcessorBind.h>  // Included just so this file can be built into both the RTOS
                              // and UEFI without needing separate copies for both build
                              // environments.


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

  /// Processor ID
  typedef struct {
    UINT32                   ProcIDMsd;            ///< Processor ID Msd
    UINT32                   ProcIDLsd;            ///< Processor ID Lsd
  } ISCP_PROC_ID;

  /// Processor Type
  typedef enum {
    ISCP_CPU_TYPE_OTHER = 1,                       ///< Other
    ISCP_CPU_TYPE_UNKNOWN,                         ///< Unknown
    ISCP_CPU_TYPE_CENTRAL_PROCESSOR,               ///< Central Processor
    ISCP_CPU_TYPE_MATH_COPROCESSOR,                ///< Math Coprocessor
    ISCP_CPU_TYPE_DSP_PROCESSOR,                   ///< DSP Processor
    ISCP_CPU_TYPE_VIDEO_PROCESSOR                  ///< Video Processor
  } ISCP_PROCESSOR_TYPE;

  /// Processor Information - Processor Family.
  typedef enum {
    ISCP_ProcessorFamilyOther                  = 0x01,  ///< Processor Family - Other
    ISCP_ProcessorFamilyUnknown                = 0x02,  ///< Processor Family - Unknown
    ISCP_ProcessorFamily8086                   = 0x03,  ///< Processor Family - 8086
    ISCP_ProcessorFamily80286                  = 0x04,  ///< Processor Family - 80286
    ISCP_ProcessorFamilyIntel386               = 0x05,  ///< Processor Family - Intel 386
    ISCP_ProcessorFamilyIntel486               = 0x06,  ///< Processor Family - Intel 486
    ISCP_ProcessorFamily8087                   = 0x07,  ///< Processor Family - 8087
    ISCP_ProcessorFamily80287                  = 0x08,  ///< Processor Family - 80287
    ISCP_ProcessorFamily80387                  = 0x09,  ///< Processor Family - 80387
    ISCP_ProcessorFamily80487                  = 0x0A,  ///< Processor Family - 80487
    ISCP_ProcessorFamilyPentium                = 0x0B,  ///< Processor Family - Pentium
    ISCP_ProcessorFamilyPentiumPro             = 0x0C,  ///< Processor Family - Pentium Pro
    ISCP_ProcessorFamilyPentiumII              = 0x0D,  ///< Processor Family - Pentium II
    ISCP_ProcessorFamilyPentiumMMX             = 0x0E,  ///< Processor Family - Pentium MMX
    ISCP_ProcessorFamilyCeleron                = 0x0F,  ///< Processor Family - Celeron
    ISCP_ProcessorFamilyPentiumIIXeon          = 0x10,  ///< Processor Family - Pentium II Xeon
    ISCP_ProcessorFamilyPentiumIII             = 0x11,  ///< Processor Family - Pentium III
    ISCP_ProcessorFamilyM1                     = 0x12,  ///< Processor Family - M1
    ISCP_ProcessorFamilyM2                     = 0x13,  ///< Processor Family - M2
    ISCP_ProcessorFamilyIntelCeleronM          = 0x14,  ///< Processor Family - Intel Celeron
    ISCP_ProcessorFamilyIntelPentium4Ht        = 0x15,  ///< Processor Family - Intel Pentium 4Ht
    ISCP_ProcessorFamilyAmdDuron               = 0x18,  ///< Processor Family - AMD Duron
    ISCP_ProcessorFamilyK5                     = 0x19,  ///< Processor Family - K5
    ISCP_ProcessorFamilyK6                     = 0x1A,  ///< Processor Family - K6
    ISCP_ProcessorFamilyK6_2                   = 0x1B,  ///< Processor Family - K6-2
    ISCP_ProcessorFamilyK6_3                   = 0x1C,  ///< Processor Family - K6-3
    ISCP_ProcessorFamilyAmdAthlon              = 0x1D,  ///< Processor Family - AMD Athlon
    ISCP_ProcessorFamilyAmd29000               = 0x1E,  ///< Processor Family - AMD 29000
    ISCP_ProcessorFamilyK6_2Plus               = 0x1F,  ///< Processor Family - K6-2 Plus
    ISCP_ProcessorFamilyPowerPC                = 0x20,  ///< Processor Family - Power PC
    ISCP_ProcessorFamilyPowerPC601             = 0x21,  ///< Processor Family - Power PC 601
    ISCP_ProcessorFamilyPowerPC603             = 0x22,  ///< Processor Family - Power PC 603
    ISCP_ProcessorFamilyPowerPC603Plus         = 0x23,  ///< Processor Family - Power PC 603 Plus
    ISCP_ProcessorFamilyPowerPC604             = 0x24,  ///< Processor Family - Power PC 604
    ISCP_ProcessorFamilyPowerPC620             = 0x25,  ///< Processor Family - Power PC 620
    ISCP_ProcessorFamilyPowerPCx704            = 0x26,  ///< Processor Family - Power PC x704
    ISCP_ProcessorFamilyPowerPC750             = 0x27,  ///< Processor Family - Power PC 750
    ISCP_ProcessorFamilyIntelCoreDuo           = 0x28,  ///< Processor Family - Intel Core Duo
    ISCP_ProcessorFamilyIntelCoreDuoMobile     = 0x29,  ///< Processor Family - Intel core Duo Mobile
    ISCP_ProcessorFamilyIntelCoreSoloMobile    = 0x2A,  ///< Processor Family - Intel Core Solo Mobile
    ISCP_ProcessorFamilyIntelAtom              = 0x2B,  ///< Processor Family - Intel Atom
    ISCP_ProcessorFamilyAlpha                  = 0x30,  ///< Processor Family - Alpha
    ISCP_ProcessorFamilyAlpha21064             = 0x31,  ///< Processor Family - Alpha 21064
    ISCP_ProcessorFamilyAlpha21066             = 0x32,  ///< Processor Family - Alpha 21166
    ISCP_ProcessorFamilyAlpha21164             = 0x33,  ///< Processor Family - Alpha 21164
    ISCP_ProcessorFamilyAlpha21164PC           = 0x34,  ///< Processor Family - Alpha 21164PC
    ISCP_ProcessorFamilyAlpha21164a            = 0x35,  ///< Processor Family - Alpha 21164a
    ISCP_ProcessorFamilyAlpha21264             = 0x36,  ///< Processor Family - Alpha 21264
    ISCP_ProcessorFamilyAlpha21364             = 0x37,  ///< Processor Family - Alpha 21364
    ISCP_ProcessorFamilyAmdTurionIIUltraDualCoreMobileM    = 0x38,  ///< Processor Family - AMD Turion II Ultra Dual Core Mobile M
    ISCP_ProcessorFamilyAmdTurionIIDualCoreMobileM         = 0x39,  ///< Processor Family - AMD Turion II Dual Core Mobile M
    ISCP_ProcessorFamilyAmdAthlonIIDualCoreM   = 0x3A,  ///< Processor Family - AMD Athlon II Dual Core M
    ISCP_ProcessorFamilyAmdOpteron6100Series   = 0x3B,  ///< Processor Family - AMD Opteron 6100 Series
    ISCP_ProcessorFamilyAmdOpteron4100Series   = 0x3C,  ///< Processor Family - AMD Opteron 4100 Series
    ISCP_ProcessorFamilyAmdOpteron6200Series   = 0x3D,  ///< Processor Family - AMD Opteron 6200 Series
    ISCP_ProcessorFamilyAmdOpteron4200Series   = 0x3E,  ///< Processor Family - AMD Opteron 4200 Series
    ISCP_ProcessorFamilyAmdFxSeries            = 0x3F,  ///< Processor Family - AMD FX Series
    ISCP_ProcessorFamilyMips                   = 0x40,  ///< Processor Family - MIPs
    ISCP_ProcessorFamilyMIPSR4000              = 0x41,  ///< Processor Family - MIPs R4000
    ISCP_ProcessorFamilyMIPSR4200              = 0x42,  ///< Processor Family - MIPs R4200
    ISCP_ProcessorFamilyMIPSR4400              = 0x43,  ///< Processor Family - MIPs R4400
    ISCP_ProcessorFamilyMIPSR4600              = 0x44,  ///< Processor Family - MIPs R4600
    ISCP_ProcessorFamilyMIPSR10000             = 0x45,  ///< Processor Family - MIPs R10000
    ISCP_ProcessorFamilyAmdCSeries             = 0x46,  ///< Processor Family - AMD C Series
    ISCP_ProcessorFamilyAmdESeries             = 0x47,  ///< Processor Family - AMD E Series
    ISCP_ProcessorFamilyAmdASeries             = 0x48,  ///< Processor Family - AMD A Series
    ISCP_ProcessorFamilyAmdGSeries             = 0x49,  ///< Processor Family - AMD G Series
    ISCP_ProcessorFamilyAmdZSeries             = 0x4A,  ///< Processor Family - AMD Z Series
    ISCP_ProcessorFamilyAmdRSeries             = 0x4B,  ///< Processor Family - AMD R Series
    ISCP_ProcessorFamilyAmdOpteron4300         = 0x4C,  ///< Processor Family - AMD Opteron 4300
    ISCP_ProcessorFamilyAmdOpteron6300         = 0x4D,  ///< Processor Family - AMD Opteron 6300
    ISCP_ProcessorFamilyAmdOpteron3300         = 0x4E,  ///< Processor Family - AMD Opteron 3300
    ISCP_ProcessorFamilyAmdFireProSeries       = 0x4F,  ///< Processor Family - AMD Fire Pro Series
    ISCP_ProcessorFamilySparc                  = 0x50,  ///< Processor Family - Sparc
    ISCP_ProcessorFamilySuperSparc             = 0x51,  ///< Processor Family - Super Sparc
    ISCP_ProcessorFamilymicroSparcII           = 0x52,  ///< Processor Family - Sparc II
    ISCP_ProcessorFamilymicroSparcIIep         = 0x53,  ///< Processor Family - Sparc IIep
    ISCP_ProcessorFamilyUltraSparc             = 0x54,  ///< Processor Family - Ultra Sparc
    ISCP_ProcessorFamilyUltraSparcII           = 0x55,  ///< Processor Family - Ultra Sparc II
    ISCP_ProcessorFamilyUltraSparcIii          = 0x56,  ///< Processor Family - Ultra Sparc Iii
    ISCP_ProcessorFamilyUltraSparcIII          = 0x57,  ///< Processor Family - Ultra Sparc III
    ISCP_ProcessorFamilyUltraSparcIIIi         = 0x58,  ///< Processor Family - Ultra Sparc IIIi
    ISCP_ProcessorFamily68040                  = 0x60,  ///< Processor Family - 68040
    ISCP_ProcessorFamily68xxx                  = 0x61,  ///< Processor Family - 68xxx
    ISCP_ProcessorFamily68000                  = 0x62,  ///< Processor Family - 68000
    ISCP_ProcessorFamily68010                  = 0x63,  ///< Processor Family - 68010
    ISCP_ProcessorFamily68020                  = 0x64,  ///< Processor Family - 68020
    ISCP_ProcessorFamily68030                  = 0x65,  ///< Processor Family - 68030
    ISCP_ProcessorFamilyAmdOpteronASeries      = 0x69,  ///< Processor Family - AMD Opteron A Series
    ISCP_ProcessorFamilyHobbit                 = 0x70,  ///< Processor Family - Hobbit
    ISCP_ProcessorFamilyCrusoeTM5000           = 0x78,  ///< Processor Family - Crusoe TM5000
    ISCP_ProcessorFamilyCrusoeTM3000           = 0x79,  ///< Processor Family - Crusoe TM3000
    ISCP_ProcessorFamilyEfficeonTM8000         = 0x7A,  ///< Processor Family - Efficeon TM8000
    ISCP_ProcessorFamilyWeitek                 = 0x80,  ///< Processor Family - Weitek
    ISCP_ProcessorFamilyItanium                = 0x82,  ///< Processor Family - Itanium
    ISCP_ProcessorFamilyAmdAthlon64            = 0x83,  ///< Processor Family - AMD Athlon64
    ISCP_ProcessorFamilyAmdOpteron             = 0x84,  ///< Processor Family - AMD Opeteron
    ISCP_ProcessorFamilyAmdSempron             = 0x85,  ///< Processor Family - AMD Sempron
    ISCP_ProcessorFamilyAmdTurion64Mobile      = 0x86,  ///< Processor Family - AMD Turion 64 Modbile
    ISCP_ProcessorFamilyDualCoreAmdOpteron     = 0x87,  ///< Processor Family - AMD Dual Core Opteron
    ISCP_ProcessorFamilyAmdAthlon64X2DualCore  = 0x88,  ///< Processor Family - AMD Athlon 64 X2 Dual Core
    ISCP_ProcessorFamilyAmdTurion64X2Mobile    = 0x89,  ///< Processor Family - AMD Turion 64 X2 Mobile
    ISCP_ProcessorFamilyQuadCoreAmdOpteron     = 0x8A,  ///< Processor Family - AMD Quad Core Opteron
    ISCP_ProcessorFamilyThirdGenerationAmdOpteron = 0x8B,  ///< Processor Family - AMD 3rd Generation Opteron
    ISCP_ProcessorFamilyAmdPhenomFxQuadCore    = 0x8C,  ///< Processor Family - AMD Phenom FX Quad Core
    ISCP_ProcessorFamilyAmdPhenomX4QuadCore    = 0x8D,  ///< Processor Family - AMD Phenom X4 Quad Core
    ISCP_ProcessorFamilyAmdPhenomX2DualCore    = 0x8E,  ///< Processor Family - AMD Phenom X2 Quad Core
    ISCP_ProcessorFamilyAmdAthlonX2DualCore    = 0x8F,  ///< Processor Family - AMD Athlon X2 Dual Core
    ISCP_ProcessorFamilyPARISC                 = 0x90,  ///< Processor Family - PARISC
    ISCP_ProcessorFamilyPaRisc8500             = 0x91,  ///< Processor Family - PARISC 8500
    ISCP_ProcessorFamilyPaRisc8000             = 0x92,  ///< Processor Family - PARISC 8000
    ISCP_ProcessorFamilyPaRisc7300LC           = 0x93,  ///< Processor Family - PARISC 7300LC
    ISCP_ProcessorFamilyPaRisc7200             = 0x94,  ///< Processor Family - PARISC 7200
    ISCP_ProcessorFamilyPaRisc7100LC           = 0x95,  ///< Processor Family - PARISC 7100LC
    ISCP_ProcessorFamilyPaRisc7100             = 0x96,  ///< Processor Family - PARISC 7100
    ISCP_ProcessorFamilyV30                    = 0xA0,  ///< Processor Family - V30
    ISCP_ProcessorFamilyQuadCoreIntelXeon3200Series  = 0xA1,  ///< Processor Family - Intel Quad Core Xeon 3200 Series
    ISCP_ProcessorFamilyDualCoreIntelXeon3000Series  = 0xA2,  ///< Processor Family - Intel Dual Core Xeon 3000 Series
    ISCP_ProcessorFamilyQuadCoreIntelXeon5300Series  = 0xA3,  ///< Processor Family - Intel Quad Core Xeon 5300 Series
    ISCP_ProcessorFamilyDualCoreIntelXeon5100Series  = 0xA4,  ///< Processor Family - Intel Dual Core Xeon 5100 Series
    ISCP_ProcessorFamilyDualCoreIntelXeon5000Series  = 0xA5,  ///< Processor Family - Intel Dual Core Xeon 5000 Series
    ISCP_ProcessorFamilyDualCoreIntelXeonLV          = 0xA6,  ///< Processor Family - Intel Dual Core Xeon LV
    ISCP_ProcessorFamilyDualCoreIntelXeonULV         = 0xA7,  ///< Processor Family - Intel Dual Core Xeon ULV
    ISCP_ProcessorFamilyDualCoreIntelXeon7100Series  = 0xA8,  ///< Processor Family - Intel Quad Core Xeon 7100 Series
    ISCP_ProcessorFamilyQuadCoreIntelXeon5400Series  = 0xA9,  ///< Processor Family - Intel Quad Core Xeon 5400 Series
    ISCP_ProcessorFamilyQuadCoreIntelXeon            = 0xAA,  ///< Processor Family - Intel Quad Core Xeon
    ISCP_ProcessorFamilyDualCoreIntelXeon5200Series  = 0xAB,  ///< Processor Family - Intel Dual Core Xeon 5200 Series
    ISCP_ProcessorFamilyDualCoreIntelXeon7200Series  = 0xAC,  ///< Processor Family - Intel Dual Core Xeon 7200 Series
    ISCP_ProcessorFamilyQuadCoreIntelXeon7300Series  = 0xAD,  ///< Processor Family - Intel Quad Core Xeon 7300 Series
    ISCP_ProcessorFamilyQuadCoreIntelXeon7400Series  = 0xAE,  ///< Processor Family - Intel Quad Core Xeon 7400 Series
    ISCP_ProcessorFamilyMultiCoreIntelXeon7400Series = 0xAF,  ///< Processor Family - Intel Multi-Core Xeon 7400 Series
    ISCP_ProcessorFamilyPentiumIIIXeon               = 0xB0,  ///< Processor Family - Intel Pentium III Xeon
    ISCP_ProcessorFamilyPentiumIIISpeedStep          = 0xB1,  ///< Processor Family - Intel Pentium III Speed Step
    ISCP_ProcessorFamilyPentium4                     = 0xB2,  ///< Processor Family - Pentium 4
    ISCP_ProcessorFamilyIntelXeon                    = 0xB3,  ///< Processor Family - Intel Xeon
    ISCP_ProcessorFamilyAS400                        = 0xB4,  ///< Processor Family - AS400
    ISCP_ProcessorFamilyIntelXeonMP                  = 0xB5,  ///< Processor Family - Intel Xeon MP
    ISCP_ProcessorFamilyAMDAthlonXP                  = 0xB6,  ///< Processor Family - AMD Athlon XP
    ISCP_ProcessorFamilyAMDAthlonMP                  = 0xB7,  ///< Processor Family - AMD Athlon MP
    ISCP_ProcessorFamilyIntelItanium2                = 0xB8,  ///< Processor Family - Intel Itanum2
    ISCP_ProcessorFamilyIntelPentiumM                = 0xB9,  ///< Processor Family - Intel Pentium M
    ISCP_ProcessorFamilyIntelCeleronD                = 0xBA,  ///< Processor Family - Intel Celeron D
    ISCP_ProcessorFamilyIntelPentiumD                = 0xBB,  ///< Processor Family - Intel Pentium D
    ISCP_ProcessorFamilyIntelPentiumEx               = 0xBC,  ///< Processor Family - Intel pentium Ex
    ISCP_ProcessorFamilyIntelCoreSolo                = 0xBD,  ///< Processor Family - Intel Core Solo
    ISCP_ProcessorFamilyReserved                     = 0xBE,  ///< Processor Family - Reserved
    ISCP_ProcessorFamilyIntelCore2                   = 0xBF,  ///< Processor Family - Intel Core 2
    ISCP_ProcessorFamilyIntelCore2Solo               = 0xC0,  ///< Processor Family - Intel Core 2 Solo
    ISCP_ProcessorFamilyIntelCore2Extreme            = 0xC1,  ///< Processor Family - Intel Core 2 Extreme
    ISCP_ProcessorFamilyIntelCore2Quad               = 0xC2,  ///< Processor Family - Intel Core 2 Quad
    ISCP_ProcessorFamilyIntelCore2ExtremeMobile      = 0xC3,  ///< Processor Family - Intel Core 2 Extremem Mobile
    ISCP_ProcessorFamilyIntelCore2DuoMobile          = 0xC4,  ///< Processor Family - Intel core 2 Duo Mobile
    ISCP_ProcessorFamilyIntelCore2SoloMobile         = 0xC5,  ///< Processor Family - Intel Core 2 Solo Mobile
    ISCP_ProcessorFamilyIntelCoreI7                  = 0xC6,  ///< Processor Family - Intel Core I7
    ISCP_ProcessorFamilyDualCoreIntelCeleron         = 0xC7,  ///< Processor Family - Intel Dual Core Celeron
    ISCP_ProcessorFamilyIBM390                       = 0xC8,  ///< Processor Family - IBM 390
    ISCP_ProcessorFamilyG4                           = 0xC9,  ///< Processor Family - G4
    ISCP_ProcessorFamilyG5                           = 0xCA,  ///< Processor Family - G5
    ISCP_ProcessorFamilyG6                           = 0xCB,  ///< Processor Family - G6
    ISCP_ProcessorFamilyzArchitecture                = 0xCC,  ///< Processor Family - zArchitecture
    ISCP_ProcessorFamilyIntelCoreI5                  = 0xCD,  ///< Processor Family - Intel Core I5
    ISCP_ProcessorFamilyIntelCoreI3                  = 0xCE,  ///< Processor Family - Intel Core I3
    ISCP_ProcessorFamilyViaC7M                       = 0xD2,  ///< Processor Family - Via C7M
    ISCP_ProcessorFamilyViaC7D                       = 0xD3,  ///< Processor Family - Via C7D
    ISCP_ProcessorFamilyViaC7                        = 0xD4,  ///< Processor Family - Via C7
    ISCP_ProcessorFamilyViaEden                      = 0xD5,  ///< Processor Family - Via Eden
    ISCP_ProcessorFamilyMultiCoreIntelXeon           = 0xD6,  ///< Processor Family - Intel Multi-core Xeon
    ISCP_ProcessorFamilyDualCoreIntelXeon3Series     = 0xD7,  ///< Processor Family - Intel Dual-core Xeon 3-Series
    ISCP_ProcessorFamilyQuadCoreIntelXeon3Series     = 0xD8,  ///< Processor Family - Intel Quad-core Xeon 3-Series
    ISCP_ProcessorFamilyViaNano                      = 0xD9,  ///< Processor Family - Via Nano
    ISCP_ProcessorFamilyDualCoreIntelXeon5Series     = 0xDA,  ///< Processor Family - Intel Dual-core Xeon 5-Series
    ISCP_ProcessorFamilyQuadCoreIntelXeon5Series     = 0xDB,  ///< Processor Family - Intel Quad-core Xeon 5-Series
    ISCP_ProcessorFamilyDualCoreIntelXeon7Series     = 0xDD,  ///< Processor Family - Intel Dual-core Xeon 7-Series
    ISCP_ProcessorFamilyQuadCoreIntelXeon7Series     = 0xDE,  ///< Processor Family - Intel Quad-core Xeon 7-Series
    ISCP_ProcessorFamilyMultiCoreIntelXeon7Series    = 0xDF,  ///< Processor Family - Intel Multi-core Xeon 7-Series
    ISCP_ProcessorFamilyMultiCoreIntelXeon3400Series = 0xE0,  ///< Processor Family - Intel Multi-core Xeon 3400-Series
    ISCP_ProcessorFamilyAmdOpteron3000Series         = 0xE4,  ///< Processor Family - AMD Opteron 3000 Series
    ISCP_ProcessorFamilyAmdSempronII                 = 0xE5,  ///< Processor Family - AMD Sempron II
    ISCP_ProcessorFamilyEmbeddedAmdOpteronQuadCore   = 0xE6,  ///< Processor Family - AMD Embedded Opteron Quad Core
    ISCP_ProcessorFamilyAmdPhenomTripleCore          = 0xE7,  ///< Processor Family - AMD Phonon Triple Core
    ISCP_ProcessorFamilyAmdTurionUltraDualCoreMobile = 0xE8,  ///< Processor Family - AMD Turion Ultra Dual Core Mobile
    ISCP_ProcessorFamilyAmdTurionDualCoreMobile      = 0xE9,  ///< Processor Family - AMD Turion Dual Core Mobile
    ISCP_ProcessorFamilyAmdAthlonDualCore            = 0xEA,  ///< Processor Family - AMD Turion Dual Core Mobile
    ISCP_ProcessorFamilyAmdSempronSI                 = 0xEB,  ///< Processor Family - AMD Sempron SI
    ISCP_ProcessorFamilyAmdPhenomII                  = 0xEC,  ///< Processor Family - AMD Phenon II
    ISCP_ProcessorFamilyAmdAthlonII                  = 0xED,  ///< Processor Family - AMD Athlon II
    ISCP_ProcessorFamilySixCoreAmdOpteron            = 0xEE,  ///< Processor Family - AMD 6-Core Opteron
    ISCP_ProcessorFamilyAmdSempronM                  = 0xEF,  ///< Processor Family - AMD Sempon M
    ISCP_ProcessorFamilyi860                         = 0xFA,  ///< Processor Family - i860
    ISCP_ProcessorFamilyi960                         = 0xFB,  ///< Processor Family - i960
    ISCP_ProcessorFamilyIndicatorFamily2             = 0xFE,  ///< Processor Family - Indicator Family 2
    ISCP_ProcessorFamilyReserved1                    = 0xFF   ///< Processor Family - Reserved
  } ISCP_PROCESSOR_FAMILY_DATA;

  /// Processor Information2 - Processor Family2.
  typedef enum {
    ISCP_ProcessorFamilySH3                   = 0x0104,  ///< ProcessorFamily - SH3
    ISCP_ProcessorFamilySH4                   = 0x0105,  ///< ProcessorFamily - SH4
    ISCP_ProcessorFamilyARM                   = 0x0118,  ///< ProcessorFamily - ARM
    ISCP_ProcessorFamilyStrongARM             = 0x0119,  ///< ProcessorFamily - Strong ARM
    ISCP_ProcessorFamily6x86                  = 0x012C,  ///< ProcessorFamily - x86
    ISCP_ProcessorFamilyMediaGX               = 0x012D,  ///< ProcessorFamily - Media GX
    ISCP_ProcessorFamilyMII                   = 0x012E,  ///< ProcessorFamily - MII
    ISCP_ProcessorFamilyWinChip               = 0x0140,  ///< ProcessorFamily - WinChip
    ISCP_ProcessorFamilyDSP                   = 0x015E,  ///< ProcessorFamily - DSP
    ISCP_ProcessorFamilyVideoProcessor        = 0x01F4   ///< ProcessorFamily - Video Processor
  } ISCP_PROCESSOR_FAMILY2_DATA;

  /// Processor Information - Processor Upgrade.
  typedef enum {
    ISCP_ProcessorUpgradeOther         = 0x01,          ///< Processor Upgrade - Other
    ISCP_ProcessorUpgradeUnknown       = 0x02,          ///< Processor Upgrade - Unknown
    ISCP_ProcessorUpgradeDaughterBoard = 0x03,          ///< Processor Upgrade - Daughter Board
    ISCP_ProcessorUpgradeZIFSocket     = 0x04,          ///< Processor Upgrade - ZIF Socket
    ISCP_ProcessorUpgradePiggyBack     = 0x05,          ///< Processor Upgrade - Piggyback
    ISCP_ProcessorUpgradeNone          = 0x06,          ///< Processor Upgrade - None
    ISCP_ProcessorUpgradeLIFSocket     = 0x07,          ///< Processor Upgrade - LIF Socket
    ISCP_ProcessorUpgradeSlot1         = 0x08,          ///< Processor Upgrade - Slot 1
    ISCP_ProcessorUpgradeSlot2         = 0x09,          ///< Processor Upgrade - Slot 2
    ISCP_ProcessorUpgrade370PinSocket  = 0x0A,          ///< Processor Upgrade - 370 Pin Socket
    ISCP_ProcessorUpgradeSlotA         = 0x0B,          ///< Processor Upgrade - Slot A
    ISCP_ProcessorUpgradeSlotM         = 0x0C,          ///< Processor Upgrade - Slot M
    ISCP_ProcessorUpgradeSocket423     = 0x0D,          ///< Processor Upgrade - Socket 423
    ISCP_ProcessorUpgradeSocketA       = 0x0E,          ///< Processor Upgrade - Socket A
    ISCP_ProcessorUpgradeSocket478     = 0x0F,          ///< Processor Upgrade - Socket 478
    ISCP_ProcessorUpgradeSocket754     = 0x10,          ///< Processor Upgrade - Socket 754
    ISCP_ProcessorUpgradeSocket940     = 0x11,          ///< Processor Upgrade - Socket 940
    ISCP_ProcessorUpgradeSocket939     = 0x12,          ///< Processor Upgrade - Socket 939
    ISCP_ProcessorUpgradeSocketmPGA604 = 0x13,          ///< Processor Upgrade - PGA 604
    ISCP_ProcessorUpgradeSocketLGA771  = 0x14,          ///< Processor Upgrade - LGA 771
    ISCP_ProcessorUpgradeSocketLGA775  = 0x15,          ///< Processor Upgrade - LGA 775
    ISCP_ProcessorUpgradeSocketS1      = 0x16,          ///< Processor Upgrade - S1
    ISCP_ProcessorUpgradeAM2           = 0x17,          ///< Processor Upgrade - AM2
    ISCP_ProcessorUpgradeF1207         = 0x18,          ///< Processor Upgrade - F1207
    ISCP_ProcessorSocketLGA1366        = 0x19,          ///< Processor Upgrade - LGA 1366
    ISCP_ProcessorUpgradeSocketG34     = 0x1A,          ///< Processor Upgrade - G34
    ISCP_ProcessorUpgradeSocketAM3     = 0x1B,          ///< Processor Upgrade - AM3
    ISCP_ProcessorUpgradeSocketC32     = 0x1C,          ///< Processor Upgrade - C32
    ISCP_ProcessorUpgradeSocketLGA1156 = 0x1D,          ///< Processor Upgrade - LGA 1156
    ISCP_ProcessorUpgradeSocketLGA1567 = 0x1E,          ///< Processor Upgrade - LGA 1567
    ISCP_ProcessorUpgradeSocketPGA988A = 0x1F,          ///< Processor Upgrade - PGA 988A
    ISCP_ProcessorUpgradeSocketBGA1288 = 0x20,          ///< Processor Upgrade - PGA 1288
    ISCP_ProcessorUpgradeSocketrPGA988B = 0x21,         ///< Processor Upgrade - PGA 988B
    ISCP_ProcessorUpgradeSocketBGA1023 = 0x22,          ///< Processor Upgrade - BGA 1023
    ISCP_ProcessorUpgradeSocketBGA1224 = 0x23,          ///< Processor Upgrade - BGA 1224
    ISCP_ProcessorUpgradeSocketLGA1155 = 0x24,          ///< Processor Upgrade - LGA 1155
    ISCP_ProcessorUpgradeSocketLGA1356 = 0x25,          ///< Processor Upgrade - LGA 1356
    ISCP_ProcessorUpgradeSocketLGA2011 = 0x26,          ///< Processor Upgrade - LGA 2011
    ISCP_ProcessorUpgradeSocketFS1     = 0x27,          ///< Processor Upgrade - FS1
    ISCP_ProcessorUpgradeSocketFS2     = 0x28,          ///< Processor Upgrade - FS2
    ISCP_ProcessorUpgradeSocketFM1     = 0x29,          ///< Processor Upgrade - FM1
    ISCP_ProcessorUpgradeSocketFM2     = 0x2A,          ///< Processor Upgrade - FM2
    ISCP_ProcessorUpgradeSocketLGA2011_3 = 0x2B,        ///< Processor Upgrade - LGA 2011-3
    ISCP_ProcessorUpgradeSocketLGA1356_3 = 0x2C         ///< Processor Upgrade - LGA 1356-3
  } ISCP_PROCESSOR_UPGRADE;

  /// CPU Information - Characteristics.
  typedef struct {
    UINT16  Reserved0                 :1;          ///< CPU Information - Reserved
    UINT16  Unknown                   :1;          ///< CPU Information - Unknown
    UINT16  Capable64Bit              :1;          ///< CPU Information - Capable 64-Bit
    UINT16  MultiCore                 :1;          ///< CPU Information - Multi-core
    UINT16  HardwareThread            :1;          ///< CPU Information - Hardware Thread
    UINT16  ExecuteProtection         :1;          ///< CPU Information - Execute Protection
    UINT16  EnhancedVirtualization    :1;          ///< CPU Information - Enhanced Virtualization
    UINT16  PowerPerformanceControl   :1;          ///< CPU Information - Power Performance Control
    UINT16  Reserved8_15              :8;          ///< CPU Information - Reserved
  } ISCP_PROCESSOR_CHARACTERISTICS;

  /// CPU Information - CPU Status.
  typedef enum {
    ISCP_CPU_STATUS_UNKNOWN = 0,                     ///< CPU Status - Unknown
    ISCP_CPU_STATUS_ENABLED,                         ///< CPU Status - Enabled
    ISCP_CPU_STATUS_DISABLED_BY_USER,                ///< CPU Status - Disabled by user
    ISCP_CPU_STATUS_DISABLED_BY_BIOS,                ///< CPU Status - Disabled by BIOS
    ISCP_CPU_STATUS_IDLE,                            ///< CPU Status - Idle
    ISCP_CPU_STATUS_RESERVED_5,                      ///< CPU Status - Reserved
    ISCP_CPU_STATUS_RESERVED_6,                      ///< CPU Status - Reserved
    ISCP_CPU_STATUS_OTHER                            ///< CPU Status - Other
  } ISCP_CPU_STATUS;


  /// CPU Information - Status.
  typedef struct {
    UINT16  CpuStatus                 :3;            ///< CPU Status
    UINT16  Reserved3_5               :3;            ///< Reserved Bits[5:3]
    UINT16  CpuSocketPopulated        :1;            ///< CPU Socket Populated
    UINT16  Reserved7_15              :9;            ///< Reserved Bits[15:9]
  } PROCESSOR_STATUS;

  /// Cache Information - Operation Mode.
  typedef enum {
    ISCP_CACHE_OPERATION_MODE_WRITE_THROUGH = 0,          ///< Cache Operation Mode Write Through
    ISCP_CACHE_OPERATION_MODE_WRITE_BACK,                 ///< Cache Operation Mode Write Back
    ISCP_CACHE_OPERATION_MODE_VARIES_WITH_MEMORY_ADDRESS, ///< Cache Operation Mode Varies with Memory Address
    ISCP_CACHE_OPERATION_MODE_UNKNOWN,                    ///< Cache Operation Mode Unknown
  } ISCP_CACHE_OPERATION_MODE;

  /// Cache Information - Location.
  typedef enum {
    ISCP_CACHE_LOCATION_INTERNAL = 0,               ///< Cache Location Internal
    ISCP_CACHE_LOCATION_EXTERNAL,                   ///< Cache Location External
    ISCP_CACHE_LOCATION_RESERVED,                   ///< Cache Location Reserved
    ISCP_CACHE_LOCATION_UNKNOWN,                    ///< Cache Location Unknown
  } ISCP_CACHE_LOCATION;

  /// Cache Information - Level.
  typedef enum {
    ISCP_CACHE_LEVEL_1 = 0,                         ///< Cache Level 1
    ISCP_CACHE_LEVEL_2,                             ///< Cache Level 2
    ISCP_CACHE_LEVEL_3,                             ///< Cache Level 3
    ISCP_CACHE_LEVEL_4,                             ///< Cache Level 4
  } ISCP_CACHE_LEVEL;

  /// Cache Information - Configuration.
  typedef struct {
    UINT16  CacheLevel                :3;            ///< Cache Level
    UINT16  CacheSocketd              :1;            ///< Cache Socket ID
    UINT16  Reserved_4                :1;            ///< Cache Reserved
    UINT16  Location                  :2;            ///< Cache Location
    UINT16  EnabledDisabled           :1;            ///< Cache Enabled / Disabled
    UINT16  OperationMode             :2;            ///< Operation Mode
    UINT16  Reserved10_15             :6;            ///< Cache Reserved
  } ISCP_CACHE_CONFIGURATION;

  /// Cache Information - SRAM Type.
  typedef struct {
    UINT16  Other                :1;                 ///< SRAM Type - Other
    UINT16  Unknown              :1;                 ///< SRAM Type - Unknown
    UINT16  NonBurst             :1;                 ///< SRAM Type - NonBurst
    UINT16  Burst                :1;                 ///< SRAM Type - Burst
    UINT16  PipelineBurst        :1;                 ///< SRAM Type - Pipeline Burst
    UINT16  Synchronous          :1;                 ///< SRAM Type - Synchronous
    UINT16  Asynchronous         :1;                 ///< SRAM Type - Asynchronous
    UINT16  Reserved7_15         :9;                 ///< SRAM Type - Reserved
  } ISCP_CACHE_SRAM_TYPE;

  /// Cache Information - Error Correction Type.
  typedef enum {
    ISCP_ECC_TYPE_OTHER = 1,                         ///< ECC Type - Other
    ISCP_ECC_TYPE_UNKNOWN,                           ///< ECC Type - Unknown
    ISCP_ECC_TYPE_NONE,                              ///< ECC Type - None
    ISCP_ECC_TYPE_PARITY,                            ///< ECC Type - Parity
    ISCP_ECC_TYPE_SINGLE_BIT,                        ///< ECC Type - Single-Bit
    ISCP_ECC_TYPE_MULTI_BIT                          ///< ECC Type - Multi-Bit
  } ISCP_CACHE_ECC_TYPE;

  /// Cache Information - System Cache Type.
  typedef enum {
    ISCP_SYSTEM_CACHE_TYPE_OTHER  = 1,               ///< System Cache Type - Other
    ISCP_SYSTEM_CACHE_TYPE_UNKNOWN,                  ///< System Cache Type - Unknown
    ISCP_SYSTEM_CACHE_TYPE_INSTRUCTION,              ///< System Cache Type - Instruction
    ISCP_SYSTEM_CACHE_TYPE_DATA,                     ///< System Cache Type - Data
    ISCP_SYSTEM_CACHE_TYPE_UNIFIED                   ///< System Cache Type - Unified
  } ISCP_SYSTEM_CACHE_TYPE;

  /// Cache Information - Associativity.
  typedef enum {
    ISCP_CACHE_ASSOCIATIVITY_OTHER = 1,                 ///< Cache Associativity - Other
    ISCP_CACHE_ASSOCIATIVITY_UNKNOWN,                   ///< Cache Associativity - Unknown
    ISCP_CACHE_ASSOCIATIVITY_DIRECT_MAPPED,             ///< Cache Associativity - Direct Mapped
    ISCP_CACHE_ASSOCIATIVITY_2_WAY_SET_ASSOCIATIVE,     ///< Cache Associativity - 2-way Set Assciative
    ISCP_CACHE_ASSOCIATIVITY_4_WAY_SET_ASSOCIATIVE,     ///< Cache Associativity - 4-way Set Assciative
    ISCP_CACHE_ASSOCIATIVITY_FULLY_ASSOCIATIVE,         ///< Cache Associativity - Fully Assciative
    ISCP_CACHE_ASSOCIATIVITY_8_WAY_SET_ASSOCIATIVE,     ///< Cache Associativity - 8-way Set Assciative
    ISCP_CACHE_ASSOCIATIVITY_16_WAY_SET_ASSOCIATIVE,    ///< Cache Associativity - 16-way Set Assciative
    ISCP_CACHE_ASSOCIATIVITY_12_WAY_SET_ASSOCIATIVE,    ///< Cache Associativity - 12-way Set Assciative
    ISCP_CACHE_ASSOCIATIVITY_24_WAY_SET_ASSOCIATIVE,    ///< Cache Associativity - 24-way Set Assciative
    ISCP_CACHE_ASSOCIATIVITY_32_WAY_SET_ASSOCIATIVE,    ///< Cache Associativity - 32-way Set Assciative
    ISCP_CACHE_ASSOCIATIVITY_48_WAY_SET_ASSOCIATIVE,    ///< Cache Associativity - 48-way Set Assciative
    ISCP_CACHE_ASSOCIATIVITY_64_WAY_SET_ASSOCIATIVE,    ///< Cache Associativity - 64-way Set Assciative
    ISCP_CACHE_ASSOCIATIVITY_20_WAY_SET_ASSOCIATIVE     ///< Cache Associativity - 20-way Set Assciative
  } ISCP_CACHE_ASSOCIATIVITY;

  /// DMI TYPE 4 - CPU Information
  typedef struct {
    UINT16                    T4ProcType;               ///< Processor Type
    UINT16                    T4ProcFamily;             ///< Processor Family
    ISCP_PROC_ID              T4ProcId;                 ///< Processor Id
    UINT16                    T4Voltage;                ///< Processor Voltage
    UINT16                    T4ExternalClock;          ///< Processor External Clock
    UINT16                    T4MaxSpeed;               ///< Processor Maximum Speed
    UINT16                    T4CurrentSpeed;           ///< Processor Current Speed
    UINT16                    T4Status;                 ///< Processor Status
    UINT16                    T4ProcUpgrade;            ///< Processor Upgrade
    UINT16                    T4CoreCount;              ///< Processor Core Count
    UINT16                    T4CoreEnabled;            ///< Processor Core Enabled
    UINT16                    T4ThreadCount;            ///< Processor Thread Count
    UINT16                    T4ProcCharacteristics;    ///< Processor Characteristics
    UINT16                    T4ProcFamily2;            ///< Processor Family 2
    UINT16                    T4CoreCount2;             ///< Processor Core Count 2
    UINT16                    T4CoreEnabled2;           ///< Processor Core Enabled 2
    UINT16                    T4ThreadCount2;           ///< Processor Thread Count 2
    UINT8                     T4SerialNumber[8];        ///< Processor Serial Number
  } ISCP_TYPE4_SMBIOS_INFO;

  /// DMI Type 7 - Cache Information
  typedef struct {
    UINT16                    T7CacheCfg;               ///< Cache Configuration
    UINT16                    T7MaxCacheSize;           ///< Maximum Cache Size
    UINT16                    T7InstallSize;            ///< Cache Install Size
    UINT16                    T7SupportedSramType;      ///< Supported SRAM Type
    UINT16                    T7CurrentSramType;        ///< Current SRAM Type
    UINT16                    T7CacheSpeed;             ///< Cache Speed in nanoseconds
    UINT16                    T7ErrorCorrectionType;    ///< Cache Error Correction Type
    UINT16                    T7SystemCacheType;        ///< System Cache Type
    UINT16                    T7Associativity;          ///< Cache Associativity
  } ISCP_TYPE7_SMBIOS_INFO;

  #ifdef __cplusplus
  }
#endif


#endif /* CPUISCP_H_ */
