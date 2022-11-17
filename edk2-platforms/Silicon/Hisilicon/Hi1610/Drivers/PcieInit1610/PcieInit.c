/** @file
*
*  Copyright (c) 2016, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2016, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include "PcieInit.h"
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/OemMiscLib.h>
#include <Library/PlatformPciLib.h>


extern VOID PcieRegWrite(UINT32 Port, UINTN Offset, UINT32 Value);
extern EFI_STATUS PciePortReset(UINT32 HostBridgeNum, UINT32 Port);
extern EFI_STATUS PciePortInit (UINT32 soctype, UINT32 HostBridgeNum, PCIE_DRIVER_CFG *PcieCfg);

PCIE_DRIVER_CFG gastr_pcie_driver_cfg[PCIE_MAX_ROOTBRIDGE] =
{
    //Port 0
    {
        0x0,                        //Portindex

        {
            PCIE_ROOT_COMPLEX,      //PortType
            PCIE_WITDH_X8,          //PortWidth
            PCIE_GEN3_0,            //PortGen
        }, //PortInfo

    },

    //Port 1
    {
        0x1,                        //Portindex
        {
            PCIE_ROOT_COMPLEX,      //PortType
            PCIE_WITDH_X8,          //PortWidth
            PCIE_GEN3_0,            //PortGen
        },

    },

    //Port 2
    {
        0x2,                        //Portindex
        {
            PCIE_ROOT_COMPLEX,      //PortType
            PCIE_WITDH_X8,          //PortWidth
            PCIE_GEN3_0,            //PortGen
        },

    },

    //Port 3
    {
        0x3,                        //Portindex
        {
            PCIE_ROOT_COMPLEX,      //PortType
            PCIE_WITDH_X8,          //PortWidth
            PCIE_GEN3_0,            //PortGen
        },

    },
    //Port 4
    {
        0x4,                        //Portindex
        {
            PCIE_ROOT_COMPLEX,      //PortType
            PCIE_WITDH_X8,          //PortWidth
            PCIE_GEN3_0,            //PortGen
        },

    },
    //Port 5
    {
        0x5,                        //Portindex
        {
            PCIE_ROOT_COMPLEX,      //PortType
            PCIE_WITDH_X8,          //PortWidth
            PCIE_GEN3_0,            //PortGen
        },

    },
    //Port 6
    {
        0x6,                        //Portindex
        {
            PCIE_ROOT_COMPLEX,      //PortType
            PCIE_WITDH_X8,          //PortWidth
            PCIE_GEN3_0,            //PortGen
        },

    },
    //Port 7
    {
        0x7,                        //Portindex
        {
            PCIE_ROOT_COMPLEX,      //PortType
            PCIE_WITDH_X8,          //PortWidth
            PCIE_GEN3_0,            //PortGen
        },

    },
};

EFI_STATUS
PcieInitEntry (
  IN EFI_HANDLE                 ImageHandle,
  IN EFI_SYSTEM_TABLE           *SystemTable
  )

{
    UINT32             Port;
    EFI_STATUS         Status = EFI_SUCCESS;
    UINT32             HostBridgeNum = 0;
    UINT32             soctype = 0;
    UINT32       PcieRootBridgeMask;


    if (!OemIsMpBoot())
    {
        PcieRootBridgeMask = PcdGet32(PcdPcieRootBridgeMask);
    }
    else
    {
        PcieRootBridgeMask = PcdGet32(PcdPcieRootBridgeMask2P);
    }

    soctype = PcdGet32(Pcdsoctype);
    for (HostBridgeNum = 0; HostBridgeNum < PCIE_MAX_HOSTBRIDGE; HostBridgeNum++) {
        for (Port = 0; Port < PCIE_MAX_ROOTBRIDGE; Port++) {
            /*
               Host Bridge may contain lots of root bridges.
               Each Host bridge have PCIE_MAX_ROOTBRIDGE root bridges
               PcieRootBridgeMask have PCIE_MAX_ROOTBRIDGE*HostBridgeNum bits,
               and each bit stands for this PCIe Port is enable or not
            */
            if (!(((( PcieRootBridgeMask >> (PCIE_MAX_ROOTBRIDGE * HostBridgeNum))) >> Port) & 0x1)) {
                continue;
            }

            Status = PciePortInit(soctype, HostBridgeNum, &gastr_pcie_driver_cfg[Port]);
            if(EFI_ERROR(Status))
            {
                DEBUG((EFI_D_ERROR, "HostBridge %d, Pcie Port %d Init Failed! \n", HostBridgeNum, Port));
            }

        }
    }


    return EFI_SUCCESS;

}


