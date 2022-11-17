/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __SOCKET_CONFIGURATION_H__
#define __SOCKET_CONFIGURATION_H__

#define SOCKET_CONFIG_CLASS_ID          44
#define SOCKET_CONFIG_SUBCLASS_ID       0x00

#define VFR_BLANK_LINE subtitle text = STRING_TOKEN(STR_NULL_STRING);
#define VFR_END_FORM endform;
#define VFR_END_FORMSET endformset;
#define VFR_HORIZ_LINE subtitle text = STRING_TOKEN(STR_HORIZONTAL_LINE);



#define SOCKET_IIO_CONFIG_KEY             0xFAFB
#define SOCKET_COMMONRC_CONFIG_KEY        0xFAFA
#define SOCKET_MP_LINK_CONFIG_KEY         0xFAF9
#define SOCKET_MEMORY_CONFIG_KEY          0xFAF8
#define SOCKET_MISC_CONFIG_KEY            0xFAF7
#define SOCKET_CONFIG_KEY                 0xF00A
#define SOCKET_POWERMANAGEMENT_CONFIGURATION_KEY 0xF00B
#define SOCKET_PROCESSORCORE_CONFIGURATION_KEY   0xF00C



#define KEY_XMP_PROFILE                 0x1DC0
#define KEY_XMP_PROFILE1                0x1DC1
#define KEY_CPU_ONLINE                  0x10B8

// {26A69232-ABF8-4597-8876-A7DC0A7CA602}
#define SOCKET_CONFIG_SETUP_GUID {0x26a69232, 0xabf8, 0x4597, 0x88, 0x76, 0xa7, 0xdc, 0xa, 0x7c, 0xa6, 0x2}



#define VFR_FORMID_SOCKET                   0x510
#define VFR_FORMID_COMMONRC                 0x511
#define VFR_FORMID_KTI                      0x512
#define VFR_FORMID_PROCESSOR                0x513
#define VFR_FORMID_KTI_STATUS               0x514

#define VFR_FORMID_SOCKET_IIO_CONFIGURATION 0x517

#define VFR_FORMID_IIO                  0x518
#define VFR_FORMID_IIO2                 0x519
#define VFR_FORMID_IIO3                 0x51A
#define VFR_FORMID_IIO4                 0x51B
#define VFR_FORMID_PWRMGT               0x51C

//
// KTI Form for SV
//

#define VFR_FORMID_KTI_GENERAL          0x521
#define VFR_FORMID_KTISYSTEMWIDE        0x522
#define VFR_FORMID_KTISYSTEM_PHY_LINK   0x523
#define VFR_FORMID_KTISYSTEM_OSB        0x524
#define VFR_FORMID_KTIPERSOCKET         0x525
#define VFR_FID_KTI_CPU0                0x526
#define VFR_FID_KTI_CPU1                0x527
#define VFR_FID_KTI_CPU2                0x528
#define VFR_FID_KTI_CPU3                0x529
#define VFR_FID_KTI_CPU4                0x52A
#define VFR_FID_KTI_CPU5                0x52B
#define VFR_FID_KTI_CPU6                0x52C
#define VFR_FID_KTI_CPU7                0x52D
//
// KTI Form IDs
//
#define VFR_FORMID_CPU_KTII             0x530
#define VFR_FID_KTI_STATUS              0x531
#define VFR_FID_KTI_TOPOLOGY            0x532
#define VFR_FID_KTI_TOPOLOGY_MATRIX     0x533
#define VFR_FID_IIO_DEV_HIDE            0x534
#define VFR_FID_KTI_SOCKET_RES          0x535
#define VFR_FID_KTI_SOCKET_RES_REQUEST  0x536
#define VFR_FID_KTI_SOCKET_RES_STATUS   0x537
#define VFR_FORMID_MEMORY               0x540
#define VFR_FORMID_MEMORY_RAS           0x541
#define VFR_FID_KTI_CPU0_LINK0          0x542
#define VFR_FID_KTI_CPU0_LINK1          0x543
#define VFR_FID_KTI_CPU0_LINK2          0x544
#define VFR_FID_KTI_CPU1_LINK0          0x545
#define VFR_FID_KTI_CPU1_LINK1          0x546
#define VFR_FID_KTI_CPU1_LINK2          0x547
#define VFR_FID_KTI_CPU2_LINK0          0x548
#define VFR_FID_KTI_CPU2_LINK1          0x549
#define VFR_FID_KTI_CPU2_LINK2          0x54A
#define VFR_FID_KTI_CPU3_LINK0          0x54B
#define VFR_FID_KTI_CPU3_LINK1          0x54C
#define VFR_FID_KTI_CPU3_LINK2          0x54D
#define VFR_FID_KTI_CPU4_LINK0          0x54E
#define VFR_FID_KTI_CPU4_LINK1          0x54F
#define VFR_FORMID_BRANCH_RANK_CONFIG       0x571
#define VFR_FORMID_VALHOOKS_CONFIG          0x572
#define VFR_FORMID_THERMTHRT_CONFIG         0x573
#define VFR_FORMID_MEMTOPOLOGY_DISPLAY      0x574
#define VFR_FORMID_PAGE_POLICY_DISPLAY      0x57D
#define VFR_FORMID_MEMORY_TRAINING_DISPLAY  0x57E
#define VFR_FORMID_MEM_PWR_SAVE_ADV_ID      0x57F
#define VFR_FORMID_CKE_DISPLAY              0x59F
#define VFR_FORMID_SREF_DISPLAY             0x5A0
#define VFR_FORMID_MEM_THERMAL_ID           0x580
#define VFR_FORMID_XMP_DISPLAY              0x581
#define VFR_FORMID_MEM_PM_CFG_ID            0x582
#define VFR_FORMID_MEM_MAP                  0x58A
#define VFR_FORMID_RAPL                     0x58B
#define VFR_FORMID_SECURE_ERASE         0x58E

#define VFR_FID_KTI_CPU4_LINK2          0x590
#define VFR_FID_KTI_CPU5_LINK0          0x591
#define VFR_FID_KTI_CPU5_LINK1          0x592
#define VFR_FID_KTI_CPU5_LINK2          0x593
#define VFR_FID_KTI_CPU6_LINK0          0x594
#define VFR_FID_KTI_CPU6_LINK1          0x595
#define VFR_FID_KTI_CPU6_LINK2          0x596
#define VFR_FID_KTI_CPU7_LINK0          0x597
#define VFR_FID_KTI_CPU7_LINK1          0x598
#define VFR_FID_KTI_CPU7_LINK2          0x599

#define VFR_FID_KTI_WARNING_LOG         0x59E


//
// MEMORY Form IDs
//




//
// IIO Form IDs
//
#define VFR_FORMID_IIO_CONFIG           0x450
#define VFR_FORMID_VTD                  0x451
#define VFR_FORMID_PCIE                 0x452
#define VFR_FORMID_PCIE_IIO0P0          0x453
#define VFR_FORMID_PCIE_IIO0P1          0x454
#define VFR_FORMID_PCIE_IIO0P2          0x455
#define VFR_FORMID_PCIE_IIO0P3          0x456
#define VFR_FORMID_PCIE_IIO0P4          0x457
#define VFR_FORMID_PCIE_IIO0P5          0x458
#define VFR_FORMID_PCIE_IIO0P6          0x459
#define VFR_FORMID_PCIE_IIO0P7          0x45A
#define VFR_FORMID_PCIE_IIO0P8          0x45B
#define VFR_FORMID_PCIE_IIO0P9          0x45C
#define VFR_FORMID_PCIE_IIO0P10         0x45D
#define VFR_FORMID_PCIE_IIO0P11         0x45E
#define VFR_FORMID_PCIE_IIO0P12         0x45F
#define VFR_FORMID_PCIE_IIO0P13         0x460
#define VFR_FORMID_PCIE_IIO0P14         0x461
#define VFR_FORMID_PCIE_IIO0P15         0x488
#define VFR_FORMID_PCIE_IIO0P16         0x489
#define VFR_FORMID_PCIE_IIO0P17         0x48A
#define VFR_FORMID_PCIE_IIO0P18         0x48B
#define VFR_FORMID_PCIE_IIO0P19         0x48C
#define VFR_FORMID_PCIE_IIO0P20         0x48D

#define VFR_FORMID_PCIE_IIO1P0          0x462
#define VFR_FORMID_PCIE_IIO1P1          0x463
#define VFR_FORMID_PCIE_IIO1P2          0x469
#define VFR_FORMID_PCIE_IIO1P3          0x46A
#define VFR_FORMID_PCIE_IIO1P4          0x46B
#define VFR_FORMID_PCIE_IIO1P5          0x46C
#define VFR_FORMID_PCIE_IIO1P6          0x46D
#define VFR_FORMID_PCIE_IIO1P7          0x46E
#define VFR_FORMID_PCIE_IIO1P8          0x46F
#define VFR_FORMID_PCIE_IIO1P9          0x470
#define VFR_FORMID_PCIE_IIO1P10         0x475
#define VFR_FORMID_PCIE_IIO1P11         0x476
#define VFR_FORMID_PCIE_IIO1P12         0x477
#define VFR_FORMID_PCIE_IIO1P13         0x478
#define VFR_FORMID_PCIE_IIO1P14         0x479
#define VFR_FORMID_PCIE_IIO1P15         0x48E
#define VFR_FORMID_PCIE_IIO1P16         0x48F
#define VFR_FORMID_PCIE_IIO1P17         0x490
#define VFR_FORMID_PCIE_IIO1P18         0x491
#define VFR_FORMID_PCIE_IIO1P19         0x492
#define VFR_FORMID_PCIE_IIO1P20         0x493

#define VFR_FORMID_IIO0                 0x47A
#define VFR_FORMID_IIO1                 0x47B
#define VFR_FORMID_IOAT_CONFIG          0x47C
#define VFR_FORMID_IIO0IOAT             0x47D
#define VFR_FORMID_IIO1IOAT             0x47E
#define VFR_FORMID_IIO2IOAT             0x47F
#define VFR_FORMID_IIO3IOAT             0x480
#define VFR_FORMID_IIO_PCIE_SLOT        0x487
//
// extended IIO form IDs for 4S
//
#define VFR_FORMID_PCIE_IIO2P0          0x0690
#define VFR_FORMID_PCIE_IIO2P1          0x0691
#define VFR_FORMID_PCIE_IIO2P2          0x0692
#define VFR_FORMID_PCIE_IIO2P3          0x0693
#define VFR_FORMID_PCIE_IIO2P4          0x0694
#define VFR_FORMID_PCIE_IIO2P5          0x0695
#define VFR_FORMID_PCIE_IIO2P6          0x0696
#define VFR_FORMID_PCIE_IIO2P7          0x0697
#define VFR_FORMID_PCIE_IIO2P8          0x0698
#define VFR_FORMID_PCIE_IIO2P9          0x0699
#define VFR_FORMID_PCIE_IIO2P10         0x069A
#define VFR_FORMID_PCIE_IIO2P11         0x069B
#define VFR_FORMID_PCIE_IIO2P12         0x069C
#define VFR_FORMID_PCIE_IIO2P13         0x069D
#define VFR_FORMID_PCIE_IIO2P14         0x069E
#define VFR_FORMID_PCIE_IIO2P15         0x06AA
#define VFR_FORMID_PCIE_IIO2P16         0x06AB
#define VFR_FORMID_PCIE_IIO2P17         0x06AC
#define VFR_FORMID_PCIE_IIO2P18         0x06AD
#define VFR_FORMID_PCIE_IIO2P19         0x06AE
#define VFR_FORMID_PCIE_IIO2P20         0x06AF

#define VFR_FORMID_PCIE_IIO3P0          0x069F
#define VFR_FORMID_PCIE_IIO3P1          0x0670
#define VFR_FORMID_PCIE_IIO3P2          0x0671
#define VFR_FORMID_PCIE_IIO3P3          0x0672
#define VFR_FORMID_PCIE_IIO3P4          0x0673
#define VFR_FORMID_PCIE_IIO3P5          0x06A0
#define VFR_FORMID_PCIE_IIO3P6          0x06A1
#define VFR_FORMID_PCIE_IIO3P7          0x06A2
#define VFR_FORMID_PCIE_IIO3P8          0x06A3
#define VFR_FORMID_PCIE_IIO3P9          0x06A4
#define VFR_FORMID_PCIE_IIO3P10         0x06A5
#define VFR_FORMID_PCIE_IIO3P11         0x06A6
#define VFR_FORMID_PCIE_IIO3P12         0x06A7
#define VFR_FORMID_PCIE_IIO3P13         0x06A8
#define VFR_FORMID_PCIE_IIO3P14         0x06A9
#define VFR_FORMID_PCIE_IIO3P15         0x06B0
#define VFR_FORMID_PCIE_IIO3P16         0x06B1
#define VFR_FORMID_PCIE_IIO3P17         0x06B2
#define VFR_FORMID_PCIE_IIO3P18         0x06B3
#define VFR_FORMID_PCIE_IIO3P19         0x06B4
#define VFR_FORMID_PCIE_IIO3P20         0x06B5

#define VFR_FORMID_VMD                  0x06C0
#define VFR_FORMID_VMD_IIO0             0x06C1
#define VFR_FORMID_VMD_IIO1             0x06C2
#define VFR_FORMID_VMD_IIO2             0x06C3
#define VFR_FORMID_VMD_IIO3             0x06C4
#define VFR_FORMID_IIO_RTO_CONFIG       0x06C5
#define VFR_FORMID_IIO_RTO_SKT0SVDEVHIDE        0x06C6
#define VFR_FORMID_IIO_RTO_SKT1SVDEVHIDE        0x06C7
#define VFR_FORMID_IIO_RTO_SKT2SVDEVHIDE        0x06C8
#define VFR_FORMID_IIO_RTO_SKT3SVDEVHIDE        0x06C9

#define VFR_FORMID_RTO_PCIE_IIO0P0          0x06CA
#define VFR_FORMID_RTO_PCIE_IIO0P1          0x06CB
#define VFR_FORMID_RTO_PCIE_IIO0P2          0x06CD
#define VFR_FORMID_RTO_PCIE_IIO0P3          0x06CE
#define VFR_FORMID_RTO_PCIE_IIO0P4          0x06CF
#define VFR_FORMID_RTO_PCIE_IIO0P5          0x06D0
#define VFR_FORMID_RTO_PCIE_IIO0P6          0x06D1
#define VFR_FORMID_RTO_PCIE_IIO0P7          0x06D2
#define VFR_FORMID_RTO_PCIE_IIO0P8          0x06D3
#define VFR_FORMID_RTO_PCIE_IIO0P9          0x06D4
#define VFR_FORMID_RTO_PCIE_IIO0P10         0x06D5
#define VFR_FORMID_RTO_PCIE_IIO0P11         0x06D6
#define VFR_FORMID_RTO_PCIE_IIO0P12         0x06D7
#define VFR_FORMID_RTO_PCIE_IIO0P13         0x06D8
#define VFR_FORMID_RTO_PCIE_IIO0P14         0x06D9
#define VFR_FORMID_RTO_PCIE_IIO0P15         0x06DA
#define VFR_FORMID_RTO_PCIE_IIO0P16         0x06DB
#define VFR_FORMID_RTO_PCIE_IIO0P17         0x06DC
#define VFR_FORMID_RTO_PCIE_IIO0P18         0x06DD
#define VFR_FORMID_RTO_PCIE_IIO0P19         0x06DE
#define VFR_FORMID_RTO_PCIE_IIO0P20         0x06DF

#define VFR_FORMID_RTO_PCIE_IIO1P0          0x06E0
#define VFR_FORMID_RTO_PCIE_IIO1P1          0x06E1
#define VFR_FORMID_RTO_PCIE_IIO1P2          0x06E2
#define VFR_FORMID_RTO_PCIE_IIO1P3          0x06E3
#define VFR_FORMID_RTO_PCIE_IIO1P4          0x06E4
#define VFR_FORMID_RTO_PCIE_IIO1P5          0x06E5
#define VFR_FORMID_RTO_PCIE_IIO1P6          0x06E6
#define VFR_FORMID_RTO_PCIE_IIO1P7          0x06E7
#define VFR_FORMID_RTO_PCIE_IIO1P8          0x06E8
#define VFR_FORMID_RTO_PCIE_IIO1P9          0x06E9
#define VFR_FORMID_RTO_PCIE_IIO1P10         0x06EA
#define VFR_FORMID_RTO_PCIE_IIO1P11         0x06EB
#define VFR_FORMID_RTO_PCIE_IIO1P12         0x06EC
#define VFR_FORMID_RTO_PCIE_IIO1P13         0x06ED
#define VFR_FORMID_RTO_PCIE_IIO1P14         0x06EE
#define VFR_FORMID_RTO_PCIE_IIO1P15         0x06EF
#define VFR_FORMID_RTO_PCIE_IIO1P16         0x06F0
#define VFR_FORMID_RTO_PCIE_IIO1P17         0x06F1
#define VFR_FORMID_RTO_PCIE_IIO1P18         0x06F2
#define VFR_FORMID_RTO_PCIE_IIO1P19         0x06F3
#define VFR_FORMID_RTO_PCIE_IIO1P20         0x06F4
#define VFR_FORMID_RTO_PCIE_IIO2P0          0x06F5
#define VFR_FORMID_RTO_PCIE_IIO2P1          0x06F6
#define VFR_FORMID_RTO_PCIE_IIO2P2          0x06F7
#define VFR_FORMID_RTO_PCIE_IIO2P3          0x06F8
#define VFR_FORMID_RTO_PCIE_IIO2P4          0x06F9
#define VFR_FORMID_RTO_PCIE_IIO2P5          0x06FA
#define VFR_FORMID_RTO_PCIE_IIO2P6          0x06FB
#define VFR_FORMID_RTO_PCIE_IIO2P7          0x06FC
#define VFR_FORMID_RTO_PCIE_IIO2P8          0x06FD
#define VFR_FORMID_RTO_PCIE_IIO2P9          0x06FE
#define VFR_FORMID_RTO_PCIE_IIO2P10         0x06FF
#define VFR_FORMID_RTO_PCIE_IIO2P11         0x0700
#define VFR_FORMID_RTO_PCIE_IIO2P12         0x0701
#define VFR_FORMID_RTO_PCIE_IIO2P13         0x0702
#define VFR_FORMID_RTO_PCIE_IIO2P14         0x0703
#define VFR_FORMID_RTO_PCIE_IIO2P15         0x0704
#define VFR_FORMID_RTO_PCIE_IIO2P16         0x0705
#define VFR_FORMID_RTO_PCIE_IIO2P17         0x0706
#define VFR_FORMID_RTO_PCIE_IIO2P18         0x0707
#define VFR_FORMID_RTO_PCIE_IIO2P19         0x0708
#define VFR_FORMID_RTO_PCIE_IIO2P20         0x0709

#define VFR_FORMID_RTO_PCIE_IIO3P0          0x070A
#define VFR_FORMID_RTO_PCIE_IIO3P1          0x070B
#define VFR_FORMID_RTO_PCIE_IIO3P2          0x070C
#define VFR_FORMID_RTO_PCIE_IIO3P3          0x070D
#define VFR_FORMID_RTO_PCIE_IIO3P4          0x070E
#define VFR_FORMID_RTO_PCIE_IIO3P5          0x070F
#define VFR_FORMID_RTO_PCIE_IIO3P6          0x0710
#define VFR_FORMID_RTO_PCIE_IIO3P7          0x0711
#define VFR_FORMID_RTO_PCIE_IIO3P8          0x0712
#define VFR_FORMID_RTO_PCIE_IIO3P9          0x0713
#define VFR_FORMID_RTO_PCIE_IIO3P10         0x0714
#define VFR_FORMID_RTO_PCIE_IIO3P11         0x0715
#define VFR_FORMID_RTO_PCIE_IIO3P12         0x0716
#define VFR_FORMID_RTO_PCIE_IIO3P13         0x0717
#define VFR_FORMID_RTO_PCIE_IIO3P14         0x0718
#define VFR_FORMID_RTO_PCIE_IIO3P15         0x0719
#define VFR_FORMID_RTO_PCIE_IIO3P16         0x071A
#define VFR_FORMID_RTO_PCIE_IIO3P17         0x071B
#define VFR_FORMID_RTO_PCIE_IIO3P18         0x071C
#define VFR_FORMID_RTO_PCIE_IIO3P19         0x071D
#define VFR_FORMID_RTO_PCIE_IIO3P20         0x071E

#define VFR_FORMID_RTO_IIO0                 0x071F
#define VFR_FORMID_RTO_IIO1                 0x0720
#define VFR_FORMID_RTO_IIO2                 0x0721
#define VFR_FORMID_RTO_IIO3                 0x0722

#define VFR_FORMID_PCIEAIC                  0x0723
#define VFR_FORMID_PCIEAIC_IIO0             0x0724
#define VFR_FORMID_PCIEAIC_IIO1             0x0725
#define VFR_FORMID_PCIEAIC_IIO2             0x0726
#define VFR_FORMID_PCIEAIC_IIO3             0x0727

#define VFR_FORMLABLE_SOCKET_TOP        0x4062
#define VFR_FORMLABLE_SOCKET_BOTTOM     0x4063

//
// Defines used for variables to be range checked before consumption.
//
#define MAX_CAS_LATENCY                        32
#define MAX_TRP_LATENCY                        32
#define MAX_TRCD_LATENCY                       32
#define MAX_TRRD_LATENCY                       255
#define MAX_TWTR_LATENCY                       255
#define MAX_TRAS_LATENCY                       63
#define MAX_TRTP_LATENCY                       255
#define MAX_TWR_LATENCY                        50
#define MAX_TFAW_LATENCY                       63
#define MAX_TCWL_LATENCY                       31
#define MAX_TRC_LATENCY                        255
#define MAX_REFRESH_RATE                       32767
#define MAX_TRFC_LATENCY                       1023
#define MAX_MC_BGF_THRESHOLD                   15


//Per Socket forms for active core count and IOT options
#define VFR_FORMID_PER_SOCKET          0x300
#define VFR_FORMID_CPU_SOCKET0         0x301
#define VFR_FORMID_CPU_SOCKET1         0x302
#define VFR_FORMID_CPU_SOCKET2         0x303
#define VFR_FORMID_CPU_SOCKET3         0x304
#define VFR_FORMID_CPU_SOCKET4         0x305
#define VFR_FORMID_CPU_SOCKET5         0x306
#define VFR_FORMID_CPU_SOCKET6         0x307
#define VFR_FORMID_CPU_SOCKET7         0x308
#define VFR_FORMID_IIO_ERR     	       0x309

#define SOCKET0_CPUPWRADVPMCFG_FORMID  0x310
#define SOCKET1_CPUPWRADVPMCFG_FORMID  0x311
#define SOCKET2_CPUPWRADVPMCFG_FORMID  0x312
#define SOCKET3_CPUPWRADVPMCFG_FORMID  0x313
#define SOCKET4_CPUPWRADVPMCFG_FORMID  0x314
#define SOCKET5_CPUPWRADVPMCFG_FORMID  0x315
#define SOCKET6_CPUPWRADVPMCFG_FORMID  0x316
#define SOCKET7_CPUPWRADVPMCFG_FORMID  0x317

//P State Control Form
#define P_STATE_CONTROL_FORMID          0x380
#define XE_RATIO_LIMIT_FORMID           0x381
#define VID_OPTIONS_FORM_ID             0x382

//HWPM control Form
#define HWPM_STATE_CONTROL_FORMID       0x385

//C State Control Form
#define CPU0_CSTATE_CONTROL_FORM_ID     0x390
#define HLV_SASV_FORM_ID                0x391

//T State Control Form
#define CPU_TSTATE_CONTROL_FORM_ID      0x392

//CPU Theraml Management
#define CPU_THERMMAL_MANAGE_FORM_ID     0x393

//Package C State Control
#define PACKAGE_CSTATE_CONTROL_FORM_ID     0x394

//DST 2.0
#define CPU_THERMAL_DTS_2_0_FORM_ID     0x395

//Advacned PM Tuning Form
#define CPU_POWER_ADVANCED_CONFIG_FORM_ID   0x3A0
#define ENERGY_PERF_BIAS_FORM_ID            0x3A1
//#define PROG_POWERCTL_MSR_FORM_ID           0x3A2
#define PROG_MSR_PP_CURT_CFG_CTRL_FORM_ID   0x3A3
#define PROG_MSR_PP_CURT_PSI_CONFIG_FORM_ID 0x3A4
#define PROG_ENTRY_CRITERIA_FORM_ID         0x3A5
#define PROG_CSR_SWLTROVRD_FORM_ID          0x3A6
#define PROG_CSR_DYNAMIC_PERF_POWER_CTL_FORM_ID   0x3A7
#define PROG_CSR_SAPMCTL_FORM_ID            0x3A8
#define PROG_CSR_PERF_P_LIMIT_FORM_ID       0x3A9

#define DRAM_RAPL_FORMID                    0x3B0
#define SOCKET_RAPL_FORMID                  0x3C0

#define ACPI_S_STATE_FORMID                 0x3D0

#define PROG_CSR_PMAX_CONFIG_FORM_ID        0x3E0

#define SOCKET0_PKGCENTRY_FORMID        0x3F0
#define SOCKET1_PKGCENTRY_FORMID        0x3F1
#define SOCKET2_PKGCENTRY_FORMID        0x3F2
#define SOCKET3_PKGCENTRY_FORMID        0x3F3
#define SOCKET4_PKGCENTRY_FORMID        0x3F4
#define SOCKET5_PKGCENTRY_FORMID        0x3F5
#define SOCKET6_PKGCENTRY_FORMID        0x3F6
#define SOCKET7_PKGCENTRY_FORMID        0x3F7

#define SOCKET0_PKGCSAPM_FORMID         0x3F8
#define SOCKET1_PKGCSAPM_FORMID         0x3F9
#define SOCKET2_PKGCSAPM_FORMID         0x3FA
#define SOCKET3_PKGCSAPM_FORMID         0x3FB
#define SOCKET4_PKGCSAPM_FORMID         0x3FC
#define SOCKET5_PKGCSAPM_FORMID         0x3FD
#define SOCKET6_PKGCSAPM_FORMID         0x3FE
#define SOCKET7_PKGCSAPM_FORMID         0x3FF

#define SOCKET0_PMAX_CONFIG_FORMID        0x400
#define SOCKET1_PMAX_CONFIG_FORMID        0x401
#define SOCKET2_PMAX_CONFIG_FORMID        0x402
#define SOCKET3_PMAX_CONFIG_FORMID        0x403
#define SOCKET4_PMAX_CONFIG_FORMID        0x404
#define SOCKET5_PMAX_CONFIG_FORMID        0x405
#define SOCKET6_PMAX_CONFIG_FORMID        0x406
#define SOCKET7_PMAX_CONFIG_FORMID        0x407

// {516D5A04-C0D5-4657-B908-E4FB1D935EF0}
#define SOCKET_FORMSET_GUID \
  { \
  0x516d5a04, 0xc0d5, 0x4657, 0xb9, 0x8, 0xe4, 0xfb, 0x1d, 0x93, 0x5e, 0xf0 \
  }

// {DD84017E-7F52-48F9-B16E-50ED9E0DBE27}
#define SOCKET_IIO_CONFIG_GUID \
  { \
    0xdd84017e, 0x7f52, 0x48f9, 0xb1, 0x6e, 0x50, 0xed, 0x9e, 0xd, 0xbe, 0x27 \
  }

// {4402CA38-808F-4279-BCEC-5BAF8D59092F}
#define SOCKET_COMMONRC_CONFIG_GUID \
  { \
    0x4402ca38, 0x808f, 0x4279, 0xbc, 0xec, 0x5b, 0xaf, 0x8d, 0x59, 0x09, 0x2f \
  }

// {2B9B22DE-2AD4-4ABC-957D-5F18C504A05C}
#define SOCKET_MP_LINK_CONFIG_GUID \
  { \
    0x2b9b22de, 0x2ad4, 0x4abc, 0x95, 0x7d, 0x5f, 0x18, 0xc5, 0x04, 0xa0, 0x5c \
  }

// {CA3FF937-D646-4936-90E8-1B950649B389}
#define SOCKET_PCI_RESOURCE_CONFIG_DATA_GUID \
  { \
    0xca3ff937, 0xd646, 0x4936, 0x90, 0xe8, 0x1b, 0x95, 0x06, 0x49, 0xb3, 0x89 \
  }

// {98CF19ED-4109-4681-B79D-9196757C7824}
#define SOCKET_MEMORY_CONFIG_GUID \
  { \
    0x98cf19ed, 0x4109, 0x4681, 0xb7, 0x9d, 0x91, 0x96, 0x75, 0x7c, 0x78, 0x24 \
  }

// {6BE64B20-C679-4ECD-ACE8-87AB4B70EC06}
#define SOCKET_MISC_CONFIG_GUID \
  { \
    0x6be64b20, 0xc679, 0x4ecd, 0xac, 0xe8, 0x87, 0xab, 0x4b, 0x70, 0xec, 0x6 \
  }
// {A1047342-BDBA-4DAE-A67A-40979B65C7F8}
#define SOCKET_POWERMANAGEMENT_CONFIG_GUID \
  { \
    0xA1047342, 0xBDBA, 0x4DAE, 0xA6, 0x7A, 0x40, 0x97, 0x9B, 0x65, 0xC7, 0xF8 \
  }
// {07013588-C789-4E12-A7C3-88FAFAE79F7C}
#define SOCKET_PROCESSORCORE_CONFIG_GUID \
  { \
    0x07013588, 0xC789, 0x4E12, 0xA7, 0xC3, 0x88, 0xFA, 0xFA, 0xE7, 0x9F, 0x7C \
  }
#endif
