.DEFAULT_GOAL:=all

ARM_LD:=arm-none-eabi-ld
ARM_AS:=arm-none-eabi-as
ARM_GCC:=arm-none-eabi-gcc
ARM_OBJDUMP:=arm-none-eabi-objdump
ARM_OBJCOPY:=arm-none-eabi-objcopy

MAKE_ANY = $(patsubst %/Makefile.inc,%,$(shell find -name Makefile.inc))

BUILD_DIR = ./build

.PHONY:all
all:output_dir $(MAKE_ANY)

.PHONY:output_dir
output_dir:
	if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR);fi;

.PHONY:$(MAKE_ANY)
$(MAKE_ANY):%:%/Makefile.inc
	make CC=$(ARM_GCC) AS=$(ARM_AS) -C ./$@ -f Makefile.inc

.PHONY:nuke
nuke:
	rm -rf $(BUILD_DIR)

.PHONY:archive
archive:all $(BUILD_DIR)/mx280.elf

$(BUILD_DIR)/mx280.elf:
	$(ARM_LD) -T ./config/stm32f103c8.ld $$(find -name *.o) -o $(BUILD_DIR)/mx280.elf
	$(ARM_OBJDUMP) $(BUILD_DIR)/mx280.elf -dxf > $(BUILD_DIR)/mx280.lst
	$(ARM_OBJCOPY) $(BUILD_DIR)/mx280.elf $(BUILD_DIR)/mx280.bin -O binary

	