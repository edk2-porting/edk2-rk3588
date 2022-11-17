/** SerDesHelperLib.h
  The Header file for SerDesHelperLib

  Copyright 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef SERDES_HELPER_LIB_H
#define SERDES_HELPER_LIB_H

#include <Uefi.h>
#include <Library/SerDes.h>

typedef struct {
  UINT16 Protocol;
  UINT8  SerDesLane[FixedPcdGet8 (PcdSerDesLanes)];
} SERDES_CONFIG;

typedef enum {
  SERDES_1  = 0,
  SERDES_2,
  SERDES_3,
  SERDES_MAX
} SERDES_NUMBER;

UINT32
GetSerDesProtocol (
  IN  INTN            SerDes,
  IN  INTN            SerDesProtocol,
  IN  INTN            Lane,
  IN  UINT32          SerDesMaxProtocol,
  IN  SERDES_CONFIG   *Config
  );

EFI_STATUS
IsSerDesProtocolValid (
  IN  INTN           SerDes,
  IN  UINT32         SerDesProtocol,
  IN  UINT8          SerDesNumLanes,
  IN  SERDES_CONFIG  *Config
  );

EFI_STATUS
GetSerDesMap (
  IN  UINT32                    SerDes,
  IN  UINT32                    SerDesProtocol,
  IN  UINT8                     SerDesNumLanes,
  IN  UINT32                    SerDesMaxProtocol,
  IN  SERDES_CONFIG             *Config,
  OUT UINT64                    *SerDesProtocolMap
  );

VOID
SerDesInstanceProbeLanes (
  IN  UINT32                      SerDes,
  IN  UINT32                      SerDesProtocol,
  IN  UINT8                       SerDesNumLanes,
  IN  UINT32                      SerDesMaxProtocol,
  IN  SERDES_CONFIG               *Config,
  IN  SERDES_PROBE_LANES_CALLBACK SerDesLaneProbeCallback,
  IN  VOID                        *Arg
  );
#endif
