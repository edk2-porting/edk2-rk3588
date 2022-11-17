/** @file
  PCH configuration based on PCH policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_POLICY_COMMON_H_
#define _PCH_POLICY_COMMON_H_

#include <ConfigBlock.h>
#include <UsbConfig.h>
#include <Usb2PhyConfig.h>
#include <Usb3HsioConfig.h>

#include "PchLimits.h"
#include "ConfigBlock/PchGeneralConfig.h"
#include <PchPcieRpConfig.h>
#include <IoApicConfig.h>
#include <SataConfig.h>
#include <RstConfig.h>
#include <PchDmiConfig.h>
#include "ConfigBlock/FlashProtectionConfig.h"
#include <InterruptConfig.h>
#include <HdAudioConfig.h>
#include <IshConfig.h>
#include <GbeConfig.h>
#include "ConfigBlock/LockDownConfig.h"
#include "P2sbConfig.h"
#include <PmConfig.h>
#include <ScsConfig.h>
#include <SerialIoConfig.h>
#include <ThcConfig.h>
#include <ThermalConfig.h>
#include <EspiConfig.h>
#include <CnviConfig.h>
#include <IehConfig.h>
#include <PsfConfig.h>
#include <FivrConfig.h>
#include <AdrConfig.h>
#include <RtcConfig.h>
#include <HybridStorageConfig.h>
#include <SpiConfig.h>

#ifndef FORCE_ENABLE
#define FORCE_ENABLE  1
#endif
#ifndef FORCE_DISABLE
#define FORCE_DISABLE 2
#endif
#ifndef PLATFORM_POR
#define PLATFORM_POR  0
#endif


#endif // _PCH_POLICY_COMMON_H_
