## @file
#
#  Copyright (c) 2018, Linaro, Ltd. All rights reserved.<BR>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

.PHONY: all
all: $(OUTPUT_DIR)/Stage2Tables.bin

$(OUTPUT_DIR)/Stage2Tables.bin: $(OUTPUT_DIR)/Stage2Tables.elf
	$(OBJCOPY) $(OBJCOPY_FLAGS) $(^) $(@)

$(OUTPUT_DIR)/Stage2Tables.elf: $(MODULE_DIR)/Stage2Tables.S
	$(ASM) $(ASM_FLAGS) -o $(@) $(^)
