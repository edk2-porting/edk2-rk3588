/** @file

  Copyright (c) 2017-2018, Arm Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

  System Control and Management Interface V1.0
    http://infocenter.arm.com/help/topic/com.arm.doc.den0056a/
    DEN0056A_System_Control_and_Management_Interface.pdf

  Juno ARM Development Platform SoC
    https://www.arm.com/files/pdf/
    DDI0515D1a_juno_arm_development_platform_soc_trm.pdf
**/

#ifndef ARM_JUNO_MTL_PRIVATE_LIB_H_
#define ARM_JUNO_MTL_PRIVATE_LIB_H_

// Mailbox transport layer.
#define MTL_DOORBELL_MODIFY_MASK   (0x00000001U)
#define MTL_DOORBELL_PRESERVE_MASK (~MTL_DOORBELL_MODIFY_MASK)

#define MTL_DOORBELL_BASE    (FixedPcdGet64 (PcdArmMtlDoorBell))
#define MTL_MAILBOX_BASE     (FixedPcdGet64 (PcdArmMtlMailBoxBase))
#define MTL_MAILBOX_SIZE     (FixedPcdGet32 (PcdArmMtlMailBoxSize))

#define MTL_POLL         0
#define MTL_INTR         1

/* For Juno, the mailbox for high priority is non-trusted SRAM + 256.

   NOTE: Below is not documented anywhere (yet)

   The payload sizes are 128 bytes.

   There are two channels:

   Channel 0
    - Agent (OS) to Platform (SCP) memory base: non-trusted SRAM + 0
    - Platform (SCP) to Agent (OS) memory base: non-trusted SRAM + 128
    - Doorbell (both directions): MHU, bit 0

   Channel 1
    - Agent (OS) to Platform (SCP) memory base: non-trusted SRAM + 256
    - Platform (SCP) to Agent (OS) memory base: non-trusted SRAM + 384
    - Doorbell (both directions): MHU, bit 0
*/
#define MTL_MAILBOX_HIGH_PRIORITY_OFFSET (MTL_MAILBOX_SIZE * 2)

// ARM MHU interrupt registers.
#define CPU_INTR_L_SET  0x108
#define CPU_INTR_H_SET  0x128

// MTL uses MHU interrupt registers for communication with the SCP.
#define MTL_DOORBELL_REGISTER_LOW   (MTL_DOORBELL_BASE + CPU_INTR_L_SET)
#define MTL_DOORBELL_REGISTER_HIGH  (MTL_DOORBELL_BASE + CPU_INTR_H_SET)

#define MTL_CHANNEL_BUSY    0
#define MTL_CHANNEL_FREE    1

// Response time out value on a MHU channel 20ms.
#define  RESPONSE_TIMEOUT  20000

/* As per SCMI spec. as a agent UEFI(or OS) can access only two channels
   (low or high priority) secure channel is only accessible
   to ARM Trusted firmware. */
#define  NUM_CHANNELS      2

/* Each channel must use a doorbell register to interrupt the SCP firmware.
   on Juno these are MHU interrupt registers for low and high priority
   channels. */
#define  DOORBELL_LOW   {                                \
                          MTL_DOORBELL_REGISTER_LOW,     \
                          MTL_DOORBELL_MODIFY_MASK,      \
                          MTL_DOORBELL_PRESERVE_MASK     \
                        }

#define  DOORBELL_HIGH  {                                \
                          MTL_DOORBELL_REGISTER_HIGH,    \
                          MTL_DOORBELL_MODIFY_MASK,      \
                          MTL_DOORBELL_PRESERVE_MASK     \
                        }

// Arbitarary poll time.
#define MTL_POLL_WAIT_TIME 100

#endif /* ARM_JUNO_MTL_PRIVATE_LIB_H_ */

