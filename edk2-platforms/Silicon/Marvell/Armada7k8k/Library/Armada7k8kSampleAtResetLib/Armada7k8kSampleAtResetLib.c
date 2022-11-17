/********************************************************************************
Copyright (C) 2018 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

Glossary - abbreviations used in Marvell SampleAtReset library implementation:
AP - Application Processor hardware block (Armada 7k8k incorporates AP806)
CP - South Bridge hardware blocks (Armada 7k8k incorporates CP110)
SAR - Sample At Reset

*******************************************************************************/

#include <Uefi.h>

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/SampleAtResetLib.h>

#include "Armada7k8kSampleAtResetLib.h"

UINT32
EFIAPI
SampleAtResetGetCpuFrequency (
  VOID
  )
{
  CONST PLL_FREQUENCY_DESCRIPTION *PllFrequencies;
  UINT32 ClockValue;
  UINT32 Index;

  ClockValue = MmioAnd32 (AP806_SAR_BASE, SAR_CLOCK_FREQUENCY_MODE_MASK);

  PllFrequencies = PllFrequencyTable;

  for (Index = 0; Index < SAR_MAX_OPTIONS; Index++, PllFrequencies++) {
    if (PllFrequencies->ClockingOption == ClockValue) {
      break;
    }
  }

  return PllFrequencies->CpuFrequency;
}

UINT32
EFIAPI
SampleAtResetGetDramFrequency (
  VOID
  )
{
  CONST PLL_FREQUENCY_DESCRIPTION *PllFrequencies;
  UINT32 ClockValue;
  UINT32 Index;

  ClockValue = MmioAnd32 (AP806_SAR_BASE, SAR_CLOCK_FREQUENCY_MODE_MASK);

  PllFrequencies = PllFrequencyTable;

  for (Index = 0; Index < SAR_MAX_OPTIONS; Index++, PllFrequencies++) {
    if (PllFrequencies->ClockingOption == ClockValue) {
      break;
    }
  }

  return PllFrequencies->DdrFrequency;
}
