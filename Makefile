# Makefile for securOS

# Tools
CC = gcc
LD = ld
NASM = nasm
QEMU = qemu-system-i386

# Flags
CFLAGS = -m32 -ffreestanding -c -g -Wall -Wextra -fno-pic -fno-stack-protector -I $(SRC_DIR)/kernel
LDFLAGS = -m elf_i386 -T linker.ld --oformat binary
NASMFLAGS = -f elf32

# Directories
SRC_DIR = src
BUILD_DIR = build
KERNEL_DIR = $(SRC_DIR)/kernel
BOOT_DIR = $(SRC_DIR)/boot

# Files
C_SOURCES = $(wildcard $(KERNEL_DIR)/*.c) \
            $(wildcard $(KERNEL_DIR)/*/*.c) \
            $(wildcard $(KERNEL_DIR)/*/*/*.c)
HEADERS = $(wildcard $(KERNEL_DIR)/*.h) \
          $(wildcard $(KERNEL_DIR)/*/*.h) \
          $(wildcard $(KERNEL_DIR)/*/*/*.h)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))

# Targets
all: $(BUILD_DIR)/securOS.bin

# Create build directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/kernel/drivers
	mkdir -p $(BUILD_DIR)/kernel/utils
	mkdir -p $(BUILD_DIR)/kernel/shell
	mkdir -p $(BUILD_DIR)/kernel/ui
	mkdir -p $(BUILD_DIR)/kernel/crypto
	mkdir -p $(BUILD_DIR)/kernel/fs
	mkdir -p $(BUILD_DIR)/kernel/security
	mkdir -p $(BUILD_DIR)/kernel/commands

# Bootloader
$(BUILD_DIR)/bootloader.bin: $(BOOT_DIR)/bootloader.asm | $(BUILD_DIR)
	$(NASM) -f bin $< -o $@

# Kernel Entry
$(BUILD_DIR)/kernel_entry.o: $(KERNEL_DIR)/kernel_entry.asm | $(BUILD_DIR)
	$(NASM) $(NASMFLAGS) $< -o $@

# Kernel C Objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

# Kernel Binary
$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel_entry.o $(OBJ_FILES)
	$(LD) $(LDFLAGS) -o $@ $^

# OS Image
$(BUILD_DIR)/securOS.bin: $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/kernel.bin
	cat $^ > $@
	truncate -s 1440k $@

# Run
run: $(BUILD_DIR)/securOS.bin
	$(QEMU) -drive format=raw,file=$<

# Clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean
