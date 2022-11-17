#
#  Copyright (c) 2012-2014, ARM Limited. All rights reserved.
#  
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#

  #
  # Networking stack
  #
!include NetworkPkg/Network.fdf.inc

!if $(INCLUDE_TFTP_COMMAND) == TRUE
  #
  # TFTP Shell command
  #
  INF ShellPkg/DynamicCommand/TftpDynamicCommand/TftpDynamicCommand.inf
!endif
