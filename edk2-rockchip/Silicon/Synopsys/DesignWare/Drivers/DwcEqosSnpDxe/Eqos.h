/** @file

  Copyright (c) 2025, Mario Bălănică <mariobalanica02@gmail.com>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef DWC_EQOS_H_
#define DWC_EQOS_H_

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DmaLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/NetLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include <Protocol/AdapterInformation.h>
#include <Protocol/ComponentName.h>
#include <Protocol/ComponentName2.h>
#include <Protocol/DwcEqosPlatformDevice.h>
#include <Protocol/SimpleNetwork.h>

#include "EqosHw.h"

//
// XXX: Having more RX descriptors (e.g. 256) affects performance
// and seems to lead to more TCP segments getting dropped.
//
#define EQOS_TX_DESC_COUNT  32
#define EQOS_RX_DESC_COUNT  2

#define EQOS_DESC_ALIGN  sizeof (struct EQOS_DMA_DESCRIPTOR)

#define EQOS_TX_DESC_RING_SIZE  (EQOS_TX_DESC_COUNT * EQOS_DESC_ALIGN)
#define EQOS_RX_DESC_RING_SIZE  (EQOS_RX_DESC_COUNT * EQOS_DESC_ALIGN)

#define EQOS_DESC_OFF(n)  ((n) * EQOS_DESC_ALIGN)
#define EQOS_TX_NEXT(n)   (((n) + 1) % EQOS_TX_DESC_COUNT)
#define EQOS_RX_NEXT(n)   (((n) + 1) % EQOS_RX_DESC_COUNT)

#define MAX_ETHERNET_PACKET_SIZE  1500

#define MAX_ETHERNET_FRAME_SIZE  (18 + MAX_ETHERNET_PACKET_SIZE + 4) // Header + Data + CRC

//
// According to the TRM:
// The buffer size must be a multiple of 4, 8, or 16 depending
// on the data bus widths (32-bit, 64-bit, or 128-bit respectively).
// This is required even if the value of buffer address pointer is not
// aligned to data bus width.
//
#define EQOS_RX_BUFFER_SIZE  ALIGN_VALUE (MAX_ETHERNET_FRAME_SIZE, EqosAxiBusWidth128)

typedef struct {
  UINT32                               Signature;
  EFI_HANDLE                           ControllerHandle;

  EFI_LOCK                             Lock;
  EFI_EVENT                            ExitBootServicesEvent;

  EFI_SIMPLE_NETWORK_PROTOCOL          Snp;
  EFI_SIMPLE_NETWORK_MODE              SnpMode;
  EFI_ADAPTER_INFORMATION_PROTOCOL     Aip;

  DWC_EQOS_PLATFORM_DEVICE_PROTOCOL    *Platform;
  DWC_EQOS_CONFIG                      Config;

  VOID                                 *TxDescs;
  VOID                                 *TxDescsMap;
  EFI_PHYSICAL_ADDRESS                 TxDescsPhysAddr;
  UINT8                                *TxBuffers[EQOS_TX_DESC_COUNT];
  VOID                                 *TxBuffersMap[EQOS_TX_DESC_COUNT];
  UINT32                               TxQueued;
  UINT32                               TxNext;
  UINT32                               TxCurrent;

  VOID                                 *RxDescs;
  VOID                                 *RxDescsMap;
  EFI_PHYSICAL_ADDRESS                 RxDescsPhysAddr;
  EFI_PHYSICAL_ADDRESS                 RxBuffersAddr;
  VOID                                 *RxBuffersMap[EQOS_RX_DESC_COUNT];
  UINT32                               RxCurrent;

  UINT32                               HwFeatures[4];

  EFI_PHYSICAL_ADDRESS                 Base;
} EQOS_PRIVATE_DATA;

extern EFI_COMPONENT_NAME_PROTOCOL   gEqosComponentName;
extern EFI_COMPONENT_NAME2_PROTOCOL  gEqosComponentName2;
extern EFI_DRIVER_BINDING_PROTOCOL   gEqosDriverBinding;

extern CONST EFI_SIMPLE_NETWORK_PROTOCOL       gEqosSnpTemplate;
extern CONST EFI_ADAPTER_INFORMATION_PROTOCOL  gEqosAipTemplate;

#define EQOS_DRIVER_SIGNATURE  SIGNATURE_32 ('E', 'Q', 'o', 'S')
#define EQOS_PRIVATE_DATA_FROM_SNP_THIS(a)  CR (a, EQOS_PRIVATE_DATA, Snp, EQOS_DRIVER_SIGNATURE)
#define EQOS_PRIVATE_DATA_FROM_AIP_THIS(a)  CR (a, EQOS_PRIVATE_DATA, Aip, EQOS_DRIVER_SIGNATURE)

#define EQOS_RX_BUFFER(p, idx)  ((UINT8 *)(UINTN)(p)->RxBuffersAddr + EQOS_RX_BUFFER_SIZE * (idx))
#define EQOS_DESC(buf, idx)     ((EQOS_DMA_DESCRIPTOR *)((UINTN)(buf) + EQOS_DESC_OFF ((idx))))

EFI_STATUS
EqosIdentify (
  IN EQOS_PRIVATE_DATA  *Eqos
  );

VOID
EqosInitializeConfig (
  IN EQOS_PRIVATE_DATA  *Eqos
  );

EFI_STATUS
EqosReset (
  IN EQOS_PRIVATE_DATA  *Eqos
  );

EFI_STATUS
EqosInitialize (
  IN EQOS_PRIVATE_DATA  *Eqos
  );

VOID
EqosEnableTxRx (
  IN EQOS_PRIVATE_DATA  *Eqos
  );

VOID
EqosDisableTxRx (
  IN EQOS_PRIVATE_DATA  *Eqos
  );

VOID
EqosDmaInitDescriptorRings (
  IN EQOS_PRIVATE_DATA  *Eqos
  );

EFI_STATUS
EqosStart (
  IN EQOS_PRIVATE_DATA  *Eqos
  );

EFI_STATUS
EqosStop (
  IN EQOS_PRIVATE_DATA  *Eqos
  );

EFI_STATUS
EqosDmaAllocate (
  IN EQOS_PRIVATE_DATA  *Eqos
  );

VOID
EqosDmaFree (
  IN EQOS_PRIVATE_DATA  *Eqos
  );

EFI_STATUS
EqosDmaMapTxDescriptor (
  IN EQOS_PRIVATE_DATA  *Eqos,
  IN UINT32             DescIndex,
  IN UINTN              NumberOfBytes
  );

EFI_STATUS
EqosDmaMapRxDescriptor (
  IN EQOS_PRIVATE_DATA  *Eqos,
  IN UINT32             DescIndex
  );

VOID
EqosDmaUnmapTxDescriptor (
  IN  EQOS_PRIVATE_DATA  *Eqos,
  IN UINT32              DescIndex
  );

VOID
EqosDmaUnmapRxDescriptor (
  IN EQOS_PRIVATE_DATA  *Eqos,
  IN UINT32             DescIndex
  );

EFI_STATUS
EqosDmaMapAllRxDescriptors (
  IN EQOS_PRIVATE_DATA  *Eqos
  );

VOID
EqosDmaUnmapAllTxDescriptors (
  IN EQOS_PRIVATE_DATA  *Eqos
  );

VOID
EqosDmaUnmapAllRxDescriptors (
  IN EQOS_PRIVATE_DATA  *Eqos
  );

EFI_STATUS
EqosCheckTxDescriptor (
  IN EQOS_PRIVATE_DATA  *Eqos,
  IN UINT32             DescIndex
  );

EFI_STATUS
EqosCheckRxDescriptor (
  IN  EQOS_PRIVATE_DATA  *Eqos,
  IN  UINT32             DescIndex,
  OUT UINTN              *FrameLength
  );

VOID
EqosGetDmaInterruptStatus (
  IN  EQOS_PRIVATE_DATA  *Eqos,
  OUT UINT32             *InterruptStatus  OPTIONAL
  );

VOID
EqosGetPhyIfStatus (
  IN  EQOS_PRIVATE_DATA  *Eqos,
  OUT BOOLEAN            *LinkUp      OPTIONAL,
  OUT UINT32             *Speed       OPTIONAL,
  OUT BOOLEAN            *FullDuplex  OPTIONAL
  );

VOID
EqosSetLinkConfig (
  IN EQOS_PRIVATE_DATA  *Eqos,
  IN UINT32             Speed,
  IN BOOLEAN            FullDuplex
  );

EFI_STATUS
EqosUpdateLink (
  IN EQOS_PRIVATE_DATA  *Eqos
  );

EFI_STATUS
EqosSetRxFilters (
  IN EQOS_PRIVATE_DATA  *Eqos,
  IN UINT32             ReceiveFilterSetting,
  IN BOOLEAN            ResetMCastFilter,
  IN UINTN              MCastFilterCnt        OPTIONAL,
  IN EFI_MAC_ADDRESS    *MCastFilter          OPTIONAL
  );

VOID
EqosGetMacAddress (
  IN  EFI_PHYSICAL_ADDRESS  Base,
  OUT EFI_MAC_ADDRESS       *MacAddress
  );

VOID
EqosSetMacAddress (
  IN EFI_PHYSICAL_ADDRESS  Base,
  IN EFI_MAC_ADDRESS       *MacAddress
  );

UINT32
EqosEtherCrc32Le (
  IN UINT8  *Buffer,
  IN UINTN  Length
  );

UINT32
EqosBitReverse32 (
  IN UINT32  Value
  );

#endif
