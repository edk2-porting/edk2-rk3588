/** @file
 *
 *  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef SARADC_LIB_H__
#define SARADC_LIB_H__

RETURN_STATUS
SaradcReadChannel (
  IN  UINT32  Channel,
  OUT UINT32  *Data
  );

#endif /* SARADC_LIB_H__ */
