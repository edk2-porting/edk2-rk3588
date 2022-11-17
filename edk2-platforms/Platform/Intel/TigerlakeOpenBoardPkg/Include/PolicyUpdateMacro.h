
/** @file
  Macros for platform to update different types of policy.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _POLICY_UPDATE_MACRO_H_
#define _POLICY_UPDATE_MACRO_H_

#ifdef UPDATE_POLICY
#undef UPDATE_POLICY
#endif

#ifdef COPY_POLICY
#undef COPY_POLICY
#endif

#ifdef GET_POLICY
#undef GET_POLICY
#endif

#ifdef AND_POLICY
#undef AND_POLICY
#endif

#ifdef OR_POLICY
#undef OR_POLICY
#endif

#if FixedPcdGetBool(PcdFspModeSelection) == 1
//
// MACROS for platform code use
//
#define UPDATE_POLICY(UpdField, ConfigField, Value)  UpdField = Value;
#define COPY_POLICY(UpdField, ConfigField, Value, Size)  CopyMem (UpdField, Value, Size);
#define GET_POLICY(UpdField, ConfigField, Value)  Value = UpdField;
#define AND_POLICY(UpdField, ConfigField, Value)  UpdField &= Value;
#define OR_POLICY(UpdField, ConfigField, Value)  UpdField |= Value;
#else
#define UPDATE_POLICY(UpdField, ConfigField, Value)  ConfigField = Value;
#define COPY_POLICY(UpdField, ConfigField, Value, Size)  CopyMem (ConfigField, Value, Size);
#define GET_POLICY(UpdField, ConfigField, Value)  Value = ConfigField;
#define AND_POLICY(UpdField, ConfigField, Value)  ConfigField &= Value;
#define OR_POLICY(UpdField, ConfigField, Value)  ConfigField |= Value;
#endif

#endif //_POLICY_UPDATE_MACRO_H_
