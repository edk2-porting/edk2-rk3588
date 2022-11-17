## @file
#  Component description file for the Simics Ich10 SiPkg DXE libraries.
#
# Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

[LibraryClasses.common]
  ResetSystemLib|$(PCH_PKG)/Library/ResetSystemLib/ResetSystemLib.inf
  PchSpiCommonLib|$(PCH_PKG)/LibraryPrivate/BasePchSpiCommonLib/BasePchSpiCommonLib.inf
