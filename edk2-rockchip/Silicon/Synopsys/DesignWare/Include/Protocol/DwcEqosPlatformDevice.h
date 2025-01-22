/** @file
  Synopsys DesignWare Ethernet Quality-of-Service (EQoS) Platform Protocol

  To register an EQOS controller with the SNP driver, configure and install both
  an instance of this protocol and a device path ending with MAC_ADDR_DEVICE_PATH.

  Note: Both the MAC and the PHY must be initialized prior (i.e. clocks, resets,
  PHY auto-negotiation), as the necessary abstractions for that are not currently
  implemented in EDK2.

  Copyright (c) 2025, Mario Bălănică <mariobalanica02@gmail.com>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef DWC_EQOS_PLATFORM_DEVICE_PROTOCOL_H_
#define DWC_EQOS_PLATFORM_DEVICE_PROTOCOL_H_

#define DWC_EQOS_PLATFORM_DEVICE_PROTOCOL_GUID \
  { 0x60975136, 0x4601, 0x41b3, { 0xbf, 0x9a, 0x90, 0xe9, 0x59, 0xfc, 0xb0, 0x02 } }

typedef struct _DWC_EQOS_PLATFORM_DEVICE_PROTOCOL DWC_EQOS_PLATFORM_DEVICE_PROTOCOL;

typedef enum {
  EqosAxiBusWidth128 = 16,
  EqosAxiBusWidth64  = 8,
  EqosAxiBusWidth32  = 4,
} DWC_EQOS_AXI_BUS_WIDTH;

typedef enum {
  EqosAxiBlen256 = BIT7,
  EqosAxiBlen128 = BIT6,
  EqosAxiBlen64  = BIT5,
  EqosAxiBlen32  = BIT4,
  EqosAxiBlen16  = BIT3,
  EqosAxiBlen8   = BIT2,
  EqosAxiBlen4   = BIT1,
} DWC_EQOS_AXI_BLEN;

typedef struct {
  UINT32                    CsrClockRate;
  DWC_EQOS_AXI_BUS_WIDTH    AxiBusWidth;
  BOOLEAN                   AxiFixedBurst;
  BOOLEAN                   AxiMixedBurst;
  UINT8                     AxiWrOsrLmt;
  UINT8                     AxiRdOsrLmt;
  DWC_EQOS_AXI_BLEN         AxiBlen;
} DWC_EQOS_CONFIG;

/**
  Overrides the default device configuration.

  @param[in] This           The protocol instance pointer.
  @param[in] Config         A pointer to the current configuration.

**/
typedef
VOID
(EFIAPI *DWC_EQOS_GET_CONFIG)(
  IN DWC_EQOS_PLATFORM_DEVICE_PROTOCOL    *This,
  IN DWC_EQOS_CONFIG                      *Config
  );

/**
  Adjusts platform clocks depending on the negotiated interface speed.

  @param[in] This           The protocol instance pointer.
  @param[in] Speed          The interface speed in Mbps (10/100/1000).

  @retval EFI_SUCCESS             The operation completed successfully.
  @retval EFI_UNSUPPORTED         The operation is unsupported.
  @retval EFI_DEVICE_ERROR        The device reported an error while attempting
                                  to perform the operation.

**/
typedef
EFI_STATUS
(EFIAPI *DWC_EQOS_SET_INTERFACE_SPEED)(
  IN DWC_EQOS_PLATFORM_DEVICE_PROTOCOL    *This,
  IN UINT32                               Speed
  );

struct _DWC_EQOS_PLATFORM_DEVICE_PROTOCOL {
  EFI_PHYSICAL_ADDRESS            BaseAddress;
  EFI_MAC_ADDRESS                 MacAddress;

  DWC_EQOS_GET_CONFIG             GetConfig;
  DWC_EQOS_SET_INTERFACE_SPEED    SetInterfaceSpeed;
};

extern EFI_GUID  gDwcEqosPlatformDeviceProtocolGuid;

#endif
