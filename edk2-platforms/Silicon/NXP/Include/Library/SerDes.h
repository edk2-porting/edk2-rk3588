/** SerDes.h
  Header file for SoC specific SerDes routines

  Copyright 2017-2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef SERDES_H
#define SERDES_H

VOID
GetSerDesProtocolMap (
  OUT UINT64               *SerDesProtocolMap
  );

typedef VOID
(*SERDES_PROBE_LANES_CALLBACK) (
  IN UINT32 LaneProtocol,
  IN VOID *Arg
  );

VOID
SerDesProbeLanes (
  IN SERDES_PROBE_LANES_CALLBACK SerDesLaneProbeCallback,
  IN VOID                        *Arg
  );
#endif
