## @file
#  makefile for building C utilities.
#
#  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

!INCLUDE $(EDK_TOOLS_PATH)\Source\C\Makefiles\ms.common

APPLICATIONS = \
  FitGen \

all: $(APPLICATIONS)
    @echo.
    @echo ######################
    @echo # Build executables
    @echo ######################
    @if not exist $(BIN_PATH) mkdir $(BIN_PATH)
    @$(EDK_TOOLS_PATH)\Source\C\Makefiles\NmakeSubdirs.bat all $**

.PHONY: clean
clean: $(APPLICATIONS)
  @$(EDK_TOOLS_PATH)\Source\C\Makefiles\NmakeSubdirs.bat clean $**

.PHONY: cleanall
cleanall: $(APPLICATIONS)
  @$(EDK_TOOLS_PATH)\Source\C\Makefiles\NmakeSubdirs.bat cleanall $**

!INCLUDE $(EDK_TOOLS_PATH)\Source\C\Makefiles\ms.rule

