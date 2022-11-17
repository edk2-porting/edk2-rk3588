/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PEI_PCH_POLICY_LIBRARY_H_
#define _PEI_PCH_POLICY_LIBRARY_H_

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PchInfoLib.h>
#include <Ppi/PchPolicy.h>
#include <PchAccess.h>
#include <Library/PchSerialIoLib.h>
#include <Library/PchPolicyLib.h>

#define PCH_HPET_BASE_ADDRESS             0xFED00000


#endif // _PEI_PCH_POLICY_LIBRARY_H_
