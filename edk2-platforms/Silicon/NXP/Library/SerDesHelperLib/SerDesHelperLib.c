/** SerDes.c
  Provides SoC specific SerDes interface

  Copyright 2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/SerDesHelperLib.h>

/**
  Function to get SerDes Lane protocol corresponding to
  SerDes protocol.

  @param  SerDes              SerDes number.
  @param  SerDesProtocol      SerDes protocol number.
  @param  Lane                SerDes Lane number.
  @param  SerDesMaxProtocol   Max SerDes protocol number.
  @param  Config              SerDes Configuration.

  @return SerDes Lane protocol.

**/
UINT32
GetSerDesProtocol (
  IN  INTN            SerDes,
  IN  INTN            SerDesProtocol,
  IN  INTN            Lane,
  IN  UINT32          SerDesMaxProtocol,
  IN  SERDES_CONFIG   *Config
  )
{
  while (Config->Protocol) {
    if (Config->Protocol == SerDesProtocol) {
      return Config->SerDesLane[Lane];
    }
    Config++;
  }

  return SerDesMaxProtocol;
}

/**
  Function to validate input SerDes protocol.

  @param  SerDes              SerDes number.
  @param  SerDesProtocol      SerDes protocol number.
  @param  SerDesNumLanes      Number of SerDes Lanes.
  @param  Config              SerDes Configuration.

  @return EFI_NOT_FOUND     SerDes Protocol not a valid protocol.
  @return EFI_SUCCESS       SerDes Protocol is a valid protocol.

**/
EFI_STATUS
IsSerDesProtocolValid (
  IN  INTN           SerDes,
  IN  UINT32         SerDesProtocol,
  IN  UINT8          SerDesNumLanes,
  IN  SERDES_CONFIG  *Config
  )
{
  UINT8 Count;

  while (Config->Protocol) {
    if (Config->Protocol == SerDesProtocol) {
      DEBUG ((DEBUG_INFO, "Protocol: 0x%x Matched with the one in Table\n", SerDesProtocol));
      break;
    }
    Config++;
  }

  if (!Config->Protocol) {
    return EFI_NOT_FOUND;
  }

  for (Count = 0; Count < SerDesNumLanes; Count++) {
    if (Config->SerDesLane[Count] != 0) {
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/**
  Get Lane protocol on provided SerDes Lane and execute callback function.

  @param  SerDes                  SerDes number.
  @param  SerDesProtocol          SerDes protocol number.
  @param  SerDesNumLanes          Number of SerDes Lanes.
  @param  SerDesMaxProtocol       Max SerDes protocol number.
  @param  Config                  SerDes Configuration.
  @param  SerDesLaneProbeCallback Pointer Callback function to be called for Lane protocol
  @param  Arg                     Pointer to Arguments to be passed to callback function.
**/
VOID
SerDesInstanceProbeLanes (
  IN  UINT32                      SerDes,
  IN  UINT32                      SerDesProtocol,
  IN  UINT8                       SerDesNumLanes,
  IN  UINT32                      SerDesMaxProtocol,
  IN  SERDES_CONFIG               *Config,
  IN  SERDES_PROBE_LANES_CALLBACK SerDesLaneProbeCallback,
  IN  VOID                        *Arg
  )
{
  INT8    Lane;
  UINT32  LaneProtocol;

  // Invoke callback for all lanes in the SerDes instance:
  for (Lane = 0; Lane < SerDesNumLanes; Lane++) {
    LaneProtocol = GetSerDesProtocol (SerDes, SerDesProtocol, Lane, SerDesMaxProtocol, Config);
    ASSERT (LaneProtocol < SerDesMaxProtocol);
    if (LaneProtocol != 0x0) {
      SerDesLaneProbeCallback (LaneProtocol, Arg);
    }
  }
}

/**
  Function to fill SerDes map information.

  @param  SerDes              SerDes number.
  @param  SerDesProtocol      SerDes protocol number.
  @param  SerDesNumLanes      Number of SerDes Lanes.
  @param  SerDesMaxProtocol   Max SerDes protocol number.
  @param  Config              SerDes Configuration.
  @param  SerDesProtocolMap   Output SerDes protocol map of enabled devices.

**/
EFI_STATUS
GetSerDesMap (
  IN  UINT32                    SerDes,
  IN  UINT32                    SerDesProtocol,
  IN  UINT8                     SerDesNumLanes,
  IN  UINT32                    SerDesMaxProtocol,
  IN  SERDES_CONFIG             *Config,
  OUT UINT64                    *SerDesProtocolMap
  )
{
  INTN                   Lane;
  EFI_STATUS             Status;
  UINT32                 LaneProtocol;

  Status = IsSerDesProtocolValid (SerDes, SerDesProtocol, SerDesNumLanes, Config);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: SERDES%d[PRTCL] = 0x%x is not valid, Status = %r \n",
            __FUNCTION__, SerDes + 1, SerDesProtocol, Status));
    return Status;
  }

  for (Lane = 0; Lane < SerDesNumLanes; Lane++) {
    LaneProtocol = GetSerDesProtocol (SerDes, SerDesProtocol, Lane, SerDesMaxProtocol, Config);
    if (LaneProtocol >= SerDesMaxProtocol) {
      DEBUG ((DEBUG_ERROR, "Unknown SerDes lane protocol %d\n", LaneProtocol));
      return EFI_NO_MAPPING;
    }
    *SerDesProtocolMap |= (BIT0 << (LaneProtocol));
  }

  return EFI_SUCCESS;
}
