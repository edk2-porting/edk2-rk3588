/** @file
 *
 *  Copyright (c) 2019, Pete Batard <pete@akeo.ie>.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef BCM2835_RNG_H__
#define BCM2835_RNG_H__

#define BCM2835_RNG_OFFSET                  0x00104000
#define RNG_BASE_ADDRESS                    ((FixedPcdGet64 (PcdBcm283xRegistersAddress)) \
                                            + BCM2835_RNG_OFFSET)

#define RNG_CTRL                            (RNG_BASE_ADDRESS + 0x0)
#define RNG_STATUS                          (RNG_BASE_ADDRESS + 0x4)
#define RNG_DATA                            (RNG_BASE_ADDRESS + 0x8)

#define RNG_CTRL_ENABLE                     0x1

#endif /* BCM2835_RNG_H__ */
