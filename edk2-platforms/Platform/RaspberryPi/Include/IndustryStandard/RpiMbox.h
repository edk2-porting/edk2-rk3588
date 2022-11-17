/** @file
 *
 * Copyright (c) 2019-2020, Pete Batard <pete@akeo.ie>
 * Copyright (c) 2017-2020, Andrei Warkentin <andrey.warkentin@gmail.com>
 * Copyright (c) 2016, Linaro Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __RASPBERRY_PI_MAILBOX_H__
#define __RASPBERRY_PI_MAILBOX_H__

/*
 * Number of iterations to perform when waiting for the mailbox.
 *
 * This number was arrived at empirically, following discussion
 * at https://github.com/raspberrypi/firmware/issues/1376, to
 * avoid mailbox time-outs on some commands.
 */
#define RPI_MBOX_MAX_TRIES                                    0x8000000

/* Mailbox channels */
#define RPI_MBOX_PM_CHANNEL                                   0
#define RPI_MBOX_FB_CHANNEL                                   1
#define RPI_MBOX_VUART_CHANNEL                                2
#define RPI_MBOX_VCHIQ_CHANNEL                                3
#define RPI_MBOX_LED_CHANNEL                                  4
#define RPI_MBOX_BUTTON_CHANNEL                               5
#define RPI_MBOX_TOUCHSCREEN_CHANNEL                          6
/* Request from ARM for response by VideoCore */
#define RPI_MBOX_VC_CHANNEL                                   8
/* Request from VideoCore for response by ARM */
#define RPI_MBOX_ARM_CHANNEL                                  9

#define RPI_MBOX_RESP_SUCCESS                                 0x80000000
#define RPI_MBOX_RESP_FAILURE                                 0x80000001

#define RPI_MBOX_VALUE_SIZE_RESPONSE_MASK                     BIT31

#define RPI_MBOX_GET_REVISION                                 0x00000001
#define RPI_MBOX_GET_BOARD_MODEL                              0x00010001
#define RPI_MBOX_GET_BOARD_REVISION                           0x00010002
#define RPI_MBOX_GET_MAC_ADDRESS                              0x00010003
#define RPI_MBOX_GET_BOARD_SERIAL                             0x00010004
#define RPI_MBOX_GET_ARM_MEMSIZE                              0x00010005
#define RPI_MBOX_GET_VC_MEMSIZE                               0x00010006
#define RPI_MBOX_GET_CLOCKS                                   0x00010007

#define RPI_MBOX_GET_POWER_STATE                              0x00020001
#define RPI_MBOX_GET_TIMING                                   0x00020002

#define RPI_MBOX_SET_POWER_STATE                              0x00028001

#define RPI_MBOX_POWER_STATE_SDHCI                            0x00000000
#define RPI_MBOX_POWER_STATE_UART0                            0x00000001
#define RPI_MBOX_POWER_STATE_UART1                            0x00000002
#define RPI_MBOX_POWER_STATE_USB_HCD                          0x00000003
#define RPI_MBOX_POWER_STATE_I2C0                             0x00000004
#define RPI_MBOX_POWER_STATE_I2C1                             0x00000005
#define RPI_MBOX_POWER_STATE_I2C2                             0x00000006
#define RPI_MBOX_POWER_STATE_SPI                              0x00000007
#define RPI_MBOX_POWER_STATE_CCP2TX                           0x00000008

#define RPI_MBOX_GET_CLOCK_STATE                              0x00030001
#define RPI_MBOX_GET_CLOCK_RATE                               0x00030002
#define RPI_MBOX_GET_VOLTAGE                                  0x00030003
#define RPI_MBOX_GET_MAX_CLOCK_RATE                           0x00030004
#define RPI_MBOX_GET_MAX_VOLTAGE                              0x00030005
#define RPI_MBOX_GET_TEMPERATURE                              0x00030006
#define RPI_MBOX_GET_MIN_CLOCK_RATE                           0x00030007
#define RPI_MBOX_GET_MIN_VOLTAGE                              0x00030008
#define RPI_MBOX_GET_TURBO                                    0x00030009
#define RPI_MBOX_GET_MAX_TEMPERATURE                          0x0003000a
#define RPI_MBOX_GET_STC                                      0x0003000b
#define RPI_MBOX_ALLOCATE_MEMORY                              0x0003000c
#define RPI_MBOX_LOCK_MEMORY                                  0x0003000d
#define RPI_MBOX_UNLOCK_MEMORY                                0x0003000e
#define RPI_MBOX_RELEASE_MEMORY                               0x0003000f
#define RPI_MBOX_EXECUTE_CODE                                 0x00030010
#define RPI_MBOX_EXECUTE_QPU                                  0x00030011
#define RPI_MBOX_SET_ENABLE_QPU                               0x00030012
#define RPI_MBOX_GET_DISPMANX_RESOURCE_MEM_HANDLE             0x00030014
#define RPI_MBOX_GET_EDID_BLOCK                               0x00030020
#define RPI_MBOX_GET_CUSTOMER_OTP                             0x00030021
#define RPI_MBOX_GET_DOMAIN_STATE                             0x00030030
#define RPI_MBOX_GET_GPIO_STATE                               0x00030041
#define RPI_MBOX_GET_GPIO_CONFIG                              0x00030043
#define RPI_MBOX_GET_PERIPH_REG                               0x00030045
#define RPI_MBOX_GET_THROTTLED                                0x00030046
#define RPI_MBOX_GET_CLOCK_MEASURED                           0x00030047
#define RPI_MBOX_NOTIFY_REBOOT                                0x00030048
#define RPI_MBOX_GET_POE_HAT_VAL                              0x00030049
#define RPI_MBOX_SET_POE_HAT_VAL                              0x00030050
#define RPI_MBOX_NOTIFY_XHCI_RESET                            0x00030058

#define RPI_MBOX_SET_CLOCK_STATE                              0x00038001
#define RPI_MBOX_SET_CLOCK_RATE                               0x00038002
#define RPI_MBOX_SET_VOLTAGE                                  0x00038003
#define RPI_MBOX_SET_TURBO                                    0x00038009
#define RPI_MBOX_SET_CUSTOMER_OTP                             0x00038021
#define RPI_MBOX_SET_DOMAIN_STATE                             0x00038030
#define RPI_MBOX_SET_GPIO                                     0x00038041
#define RPI_MBOX_SET_SDHOST_CLOCK                             0x00038042
#define RPI_MBOX_SET_GPIO_CONFIG                              0x00038043
#define RPI_MBOX_SET_PERIPH_REG                               0x00038045

#define RPI_MBOX_ALLOC_FB                                     0x00040001
#define RPI_MBOX_FB_BLANK                                     0x00040002
#define RPI_MBOX_GET_FB_GEOMETRY                              0x00040003
#define RPI_MBOX_GET_FB_VIRTUAL_WIDTH_HEIGHT                  0x00040004
#define RPI_MBOX_GET_FB_COLOR_DEPTH                           0x00040005
#define RPI_MBOX_GET_FB_PIXEL_ORDER                           0x00040006
#define RPI_MBOX_GET_FB_ALPHA_MODE                            0x00040007
#define RPI_MBOX_GET_FB_LINELENGTH                            0x00040008
#define RPI_MBOX_GET_FB_VIRTUAL_OFFSET                        0x00040009
#define RPI_MBOX_GET_FB_OVERSCAN                              0x0004000a
#define RPI_MBOX_GET_FB_PALETTE                               0x0004000b
#define RPI_MBOX_GET_FB_TOUCHBUF                              0x0004000f
#define RPI_MBOX_GET_FB_GPIOVIRTBUF                           0x00040010

#define RPI_MBOX_TEST_FB_PHYSICAL_WIDTH_HEIGHT                0x00044003
#define RPI_MBOX_TEST_FB_VIRTUAL_WIDTH_HEIGHT                 0x00044004
#define RPI_MBOX_TEST_FB_DEPTH                                0x00044005
#define RPI_MBOX_TEST_FB_PIXEL_ORDER                          0x00044006
#define RPI_MBOX_TEST_FB_ALPHA_MODE                           0x00044007
#define RPI_MBOX_TEST_FB_VIRTUAL_OFFSET                       0x00044009
#define RPI_MBOX_TEST_FB_OVERSCAN                             0x0004400a
#define RPI_MBOX_TEST_FB_PALETTE                              0x0004400b
#define RPI_MBOX_TEST_FB_VSYNC                                0x0004400e

#define RPI_MBOX_FREE_FB                                      0x00048001
#define RPI_MBOX_SET_FB_PGEOM                                 0x00048003
#define RPI_MBOX_SET_FB_VGEOM                                 0x00048004
#define RPI_MBOX_SET_FB_DEPTH                                 0x00048005
#define RPI_MBOX_SET_FB_PIXEL_ORDER                           0x00048006
#define RPI_MBOX_SET_FB_ALPHA_MODE                            0x00048007
#define RPI_MBOX_SET_FB_VIRTUAL_OFFSET                        0x00048009
#define RPI_MBOX_SET_FB_OVERSCAN                              0x0004800a
#define RPI_MBOX_SET_FB_PALET TE                              0x0004800b
#define RPI_MBOX_VCHIQ_INIT                                   0x00048010
#define RPI_MBOX_SET_FB_TOUCHBUF                              0x0004801f
#define RPI_MBOX_SET_FB_GPIOVIRTBUF                           0x00048020
#define RPI_MBOX_SET_FB_VSYNC                                 0x0004800e
#define RPI_MBOX_SET_FB_BACKLIGHT                             0x0004800f

#define RPI_MBOX_GET_COMMAND_LINE                             0x00050001

#define RPI_MBOX_GET_DMA_CHANNELS                             0x00060001

#define RPI_MBOX_POWER_STATE_ENABLE                           BIT0
#define RPI_MBOX_POWER_STATE_WAIT                             BIT1

#define RPI_MBOX_CLOCK_RATE_EMMC                              0x000000001
#define RPI_MBOX_CLOCK_RATE_UART                              0x000000002
#define RPI_MBOX_CLOCK_RATE_ARM                               0x000000003
#define RPI_MBOX_CLOCK_RATE_CORE                              0x000000004
#define RPI_MBOX_CLOCK_RATE_V3D                               0x000000005
#define RPI_MBOX_CLOCK_RATE_H264                              0x000000006
#define RPI_MBOX_CLOCK_RATE_ISP                               0x000000007
#define RPI_MBOX_CLOCK_RATE_SDRAM                             0x000000008
#define RPI_MBOX_CLOCK_RATE_PIXEL                             0x000000009
#define RPI_MBOX_CLOCK_RATE_PWM                               0x00000000a
#define RPI_MBOX_CLOCK_RATE_HEVC                              0x00000000b
#define RPI_MBOX_CLOCK_RATE_EMMC2                             0x00000000c
#define RPI_MBOX_CLOCK_RATE_M2MC                              0x00000000d
#define RPI_MBOX_CLOCK_RATE_PIXEL_BVB                         0x00000000d

#define RPI_EXP_GPIO_DIR_IN                                   0
#define RPI_EXP_GPIO_DIR_OUT                                  1

#define RPI_EXP_GPIO_BT                                       0
#define RPI_EXP_GPIO_WIFI                                     1
#define RPI_EXP_GPIO_LED                                      2
#define RPI_EXP_GPIO_RESET                                    3
#define RPI_EXP_GPIO_SD_VOLT                                  4
#define RPI_EXP_GPIO_CAMERA                                   5
#define RPI_EXP_GPIO_SD_POWER                                 6
#define RPI_EXP_GPIO_POWER_LED                                7

#endif /* __RASPBERRY_PI_MAILBOX_H__ */
