/** @file
  Chipset definition for ME Devices.

  Conventions:

  - Prefixes:
    - Definitions beginning with "R_" are registers
    - Definitions beginning with "B_" are bits within registers
    - Definitions beginning with "V_" are meaningful values of bits within the registers
    - Definitions beginning with "S_" are register sizes
    - Definitions beginning with "N_" are the bit position
  - Registers / bits that are different between PCH generations are denoted by
    "_ME_[generation_name]_" in register/bit names.
  - Registers / bits that are specific to PCH-H denoted by "PCH_H_" in register/bit names.
    Registers / bits that are specific to PCH-LP denoted by "PCH_LP_" in register/bit names.
    e.g., "_ME_PCH_H_", "_ME_PCH_LP_"
    Registers / bits names without _PCH_H_ or _PCH_LP_ apply for both H and LP.
  - Registers / bits that are different between SKUs are denoted by "_[SKU_name]"
    at the end of the register/bit names
  - Registers / bits of new devices introduced in a PCH generation will be just named
    as "_ME_" without [generation_name] inserted.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _ME_CHIPSET_H_
#define _ME_CHIPSET_H_

#define ME_SEGMENT            0
#define ME_BUS                0
#define ME_DEVICE_NUMBER      22
#define HECI_MIN_FUNC         0
#define HECI_MAX_FUNC         5

#define HECI_FUNCTION_NUMBER  0x00
#define HECI2_FUNCTION_NUMBER 0x01
#define IDER_FUNCTION_NUMBER  0x02
#define SOL_FUNCTION_NUMBER   0x03
#define HECI3_FUNCTION_NUMBER 0x04
#define HECI4_FUNCTION_NUMBER 0x05

#define IDER_BUS_NUMBER       ME_BUS
#define IDER_DEVICE_NUMBER    ME_DEVICE_NUMBER
#define SOL_BUS_NUMBER        ME_BUS
#define SOL_DEVICE_NUMBER     ME_DEVICE_NUMBER

 ///
/// Convert to HECI# defined in BWG from Fun#
///
#define HECI_NAME_MAP(a) ((a < 2) ? (a + 1) : (a - 1))

///
/// ME-related Chipset Definition
///
#define HeciEnable()    MeDeviceControl (HECI1, Enabled);
#define Heci2Enable()   MeDeviceControl (HECI2, Enabled);
#define Heci3Enable()   MeDeviceControl (HECI3, Enabled);
#define Heci4Enable()   MeDeviceControl (HECI4, Enabled);
#define IderEnable()    MeDeviceControl (IDER, Enabled);
#define SolEnable()     MeDeviceControl (SOL, Enabled);

#define HeciDisable()   MeDeviceControl (HECI1, Disabled);
#define Heci2Disable()  MeDeviceControl (HECI2, Disabled);
#define Heci3Disable()  MeDeviceControl (HECI3, Disabled);
#define IderDisable()   MeDeviceControl (IDER, Disabled);
#define SolDisable()    MeDeviceControl (SOL, Disabled);

#define DisableAllMeDevices() \
  HeciDisable (); \
  Heci2Disable (); \
  Heci3Disable (); \
  IderDisable (); \
  SolDisable ();

///
/// HECI Device Id Definitions
///
 #define IS_PCH_H_HECI_DEVICE_ID(DeviceId) \
     (  \
       (DeviceId == V_ME_PCH_H_HECI_DEVICE_ID) \
     )

 #define IS_PCH_LP_HECI_DEVICE_ID(DeviceId) \
     (  \
       (DeviceId == V_ME_PCH_LP_HECI_DEVICE_ID) \
     )

 #define IS_HECI_DEVICE_ID(DeviceId) \
     ( \
       IS_PCH_H_HECI_DEVICE_ID(DeviceId) || \
       IS_PCH_LP_HECI_DEVICE_ID(DeviceId) \
     )

///
/// HECI2 Device Id Definitions
///
#define IS_PCH_H_HECI2_DEVICE_ID(DeviceId) \
     (  \
       (DeviceId == V_ME_PCH_H_HECI2_DEVICE_ID) \
     )

#define IS_PCH_LP_HECI2_DEVICE_ID(DeviceId) \
     (  \
       (DeviceId == V_ME_PCH_LP_HECI2_DEVICE_ID) \
     )

#define IS_HECI2_DEVICE_ID(DeviceId) \
     ( \
       IS_PCH_H_HECI2_DEVICE_ID(DeviceId) || \
       IS_PCH_LP_HECI2_DEVICE_ID(DeviceId) \
     )

///
/// HECI3 Device Id Definitions
///
#define IS_PCH_H_HECI3_DEVICE_ID(DeviceId) \
    (  \
      (DeviceId == V_ME_PCH_H_HECI3_DEVICE_ID) \
    )

#define IS_PCH_LP_HECI3_DEVICE_ID(DeviceId) \
    (  \
      (DeviceId == V_ME_PCH_LP_HECI3_DEVICE_ID) \
    )

#define IS_HECI3_DEVICE_ID(DeviceId) \
    ( \
      IS_PCH_H_HECI3_DEVICE_ID(DeviceId) || \
      IS_PCH_LP_HECI3_DEVICE_ID(DeviceId) \
    )

///
/// HECI4 Device Id Definitions
///
#define IS_PCH_H_HECI4_DEVICE_ID(DeviceId) \
    (  \
      (DeviceId == V_ME_PCH_H_HECI4_DEVICE_ID) \
    )

#define IS_PCH_LP_HECI4_DEVICE_ID(DeviceId) \
    (  \
      (DeviceId == V_ME_PCH_LP_HECI4_DEVICE_ID) \
    )

#define IS_HECI4_DEVICE_ID(DeviceId) \
    ( \
      IS_PCH_H_HECI4_DEVICE_ID(DeviceId) || \
      IS_PCH_LP_HECI4_DEVICE_ID(DeviceId) \
    )

///
/// SoL Device Id Definitions
///
#define IS_PCH_H_SOL_DEVICE_ID(DeviceId) \
    (  \
      (DeviceId == V_ME_PCH_H_SOL_DEVICE_ID) \
    )

#define IS_PCH_LP_SOL_DEVICE_ID(DeviceId) \
    (  \
      (DeviceId == V_ME_PCH_LP_SOL_DEVICE_ID) \
    )

#define IS_PCH_SOL_DEVICE_ID(DeviceId) \
    ( \
      IS_PCH_H_SOL_DEVICE_ID(DeviceId) || \
      IS_PCH_LP_SOL_DEVICE_ID(DeviceId) \
    )

#endif
