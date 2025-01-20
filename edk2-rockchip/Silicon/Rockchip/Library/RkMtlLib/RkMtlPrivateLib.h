/** @file

  Copyright (c) 2017-2018, Arm Limited. All rights reserved.
  Copyright (c) 2021, Jared McNeill <jmcneill@invisible.ca>

  SPDX-License-Identifier: BSD-2-Clause-Patent

  System Control and Management Interface V1.0
    http://infocenter.arm.com/help/topic/com.arm.doc.den0056a/
    DEN0056A_System_Control_and_Management_Interface.pdf
**/

#ifndef RK_MTL_PRIVATE_LIB_H_
#define RK_MTL_PRIVATE_LIB_H_

// Mailbox transport layer.
#define MTL_MAILBOX_BASE  (FixedPcdGet64 (PcdRkMtlMailBoxBase))
#define MTL_MAILBOX_SIZE  (FixedPcdGet32 (PcdRkMtlMailBoxSize))

#define MTL_POLL  0

#define MTL_CHANNEL_BUSY  0
#define MTL_CHANNEL_FREE  1

// Response time out value on a channel 1s.
#define  RESPONSE_TIMEOUT  1000000
#define  NUM_CHANNELS      1

// Arbitarary poll time.
#define MTL_POLL_WAIT_TIME  100000

#endif /* RK_MTL_PRIVATE_LIB_H_ */
