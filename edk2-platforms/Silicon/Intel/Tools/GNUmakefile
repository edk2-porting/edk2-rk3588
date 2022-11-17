## @file
#  GNUmakefile for building C utilities.
#
#  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

MAKEROOT = $(EDK_TOOLS_PATH)/Source/C

APPLICATIONS = \
  FitGen \

SUBDIRS := $(APPLICATIONS)

$(APPLICATIONS): $(MAKEROOT)/bin

.PHONY: subdirs $(SUBDIRS)
subdirs: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: $(patsubst %,%-clean,$(sort $(SUBDIRS)))
$(patsubst %,%-clean,$(sort $(SUBDIRS))):
	-$(MAKE) -C $(@:-clean=) clean

clean: $(patsubst %,%-clean,$(sort $(SUBDIRS)))

