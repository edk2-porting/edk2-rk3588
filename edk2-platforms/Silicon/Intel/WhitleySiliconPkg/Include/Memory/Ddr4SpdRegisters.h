/** @file

  @copyright
  Copyright 2019 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DDR4_SPD_REGS_H_
#define _DDR4_SPD_REGS_H_

//
// DDR4 SPD Spec 4.0 Register Definitions
//

/*  Byte 132 (0x084) (Registered): RDIMM Thermal Heat Spreader Solution  */

#define SPD_RDIMM_RDIMM_THERMAL_HEAT_SPREADER_SOLUTION_REG       0x0084

typedef union {
  struct {
    UINT8 heat_spreader_thermal_characteristics : 7;
                            /* Bits[0:6]
                               Heat Spreader Thermal Characteristics
                               0 = Undefined
                               All other settings to be defined
                            */
    UINT8 heat_spreader_solution : 1;
                            /* Bits[7]
                               Heat Spreader Solution
                               0 = Heat spreader solution is not incorporated onto this assembly
                               1 = Heat spreader solution is incorporated onto this assembly
                            */
  } Bits;
  UINT8 Data;
} RDIMM_RDIMM_THERMAL_HEAT_SPREADER_SOLUTION_STRUCT;

#endif  // #ifndef _DDR4_SPD_REGS_H_