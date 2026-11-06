PLACE ?= build/isos/Dkernel.iso
CC=i686-elf-gcc
AS = i686-elf-as
CFLAGS= -ffreestanding -Iinclude -Wall -Wextra -fno-stack-protector -fno-builtin -nostdinc -fno-pic -fno-pie -fno-common
LDFLAGS= -ffreestanding -nostdlib -lgcc -T kernel.ld
LD_SCRIPT = kernel.ld

MODE ?= debug

TARGET = build/bin/Dkernel.elf
# Saves all the files ending with .c
# and .s to change them from .c/.s to .o
SRCS_C := $(shell find src -type f -name "*.c")
SRCS_S := $(shell find src -type f -name "*.s")

ifeq ($(MODE), release)
    CFLAGS+= -O2
    TARGET = build/bin/kernel.elf
    OBJ_DIR = build/obj/release/
    ISO_DIR = build/release
    KERNEL_ISO = kernel.iso
    PLACE = build/isos/kernel.iso
else
    CFLAGS += -g -O0
    OBJ_DIR = build/obj/debug/
    ISO_DIR = build/debug
    KERNEL_ISO = Dkernel.iso
endif

OBJS := $(patsubst src/%.c, $(OBJ_DIR)%.o, $(SRCS_C))
OBJS += $(patsubst src/%.s, $(OBJ_DIR)%.o, $(SRCS_S))

.PHONY: clean all iso

all: $(TARGET) iso

#Build section
$(TARGET): $(OBJS) $(LD_SCRIPT)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# Make's rule to create the C and Assembly 
# object files and where to place them
$(OBJ_DIR)%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)%.o: src/%.s
	mkdir -p $(dir $@)
	$(AS) $< -o $@

#End build section

iso: $(TARGET)
	mkdir -p $(ISO_DIR)/boot
	cp $< $(ISO_DIR)/boot
	grub-mkrescue $(ISO_DIR) -o build/isos/$(KERNEL_ISO)

run:
	qemu-system-i386 -cdrom $(PLACE)

clean:
	rm -rf build/bin/* build/debug/boot/Dkernel.elf build/release/boot/kernel.elf build/isos/* build/obj/*
