PLACE?="build/isos/kernel.iso"
CC=i686-elf-gcc
AS = i686-elf-as
CFLAGS= -ffreestanding -Iinclude -Wall -Wextra -fno-stack-protector -fno-builtin -fno-pic -fno-pie
LDFLAGS= -ffreestanding -nostdlib -lgcc

MODE?=debug

QEMUFLAGS?=
# Saves all the files ending with .c to
# change them from .c to .o
SRCS_C := $(shell find src -type f -name "*.c")
SRCS_S := $(shell find src -type f -name "*.s")
OBJS := $(patsubst src/%.c, build/obj/%.o, $(SRCS_C))
OBJS += $(patsubst src/%.s, build/obj/%.o, $(SRCS_S))
DOBJS := $(patsubst src/%.c, build/debugBin/%.o, $(SRCS_C))
DOBJS += $(patsubst src/%.s, build/debugBin/%.o, $(SRCS_S))

# Checks if the folders exists to create them if not exists and don't create if it exists
$(shell mkdir -p build/bin build/debug/boot/grub build/debugBin build/iso/boot/grub build/isos build/obj)

.PHONY: debug clean all build iso

all: build iso

iso: build/bin/kernel.elf
	mkdir -p $(dir $@)
	cp build/bin/kernel.elf build/iso/boot/kernel.elf
	grub-mkrescue build/iso -o $(PLACE)

#Build section
build: $(OBJS)
	$(CC) $(LDFLAGS) -T kernel.ld $(OBJS) -o build/bin/kernel.elf

# Tells make how to create the C and Assembly 
# object files and where to place them
build/obj/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/obj/%.o: src/%.s
	mkdir -p $(dir $@)
	$(AS) $< -o $@

#End build section

#Debug section
debug: $(DOBJS)
	$(CC) $(LDFLAGS) -T kernel.ld $(DOBJS) -o build/debug/boot/Dkernel.elf
	grub-mkrescue build/debug/ -o build/isos/debug.iso

build/debugBin/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -g -O0 -c $< -o $@

build/debugBin/%.o: src/%.s
	mkdir -p $(dir $@)
	$(AS) $< -o $@
#End debug

run:
	qemu-system-i386 -cdrom $(PLACE) $(QEMUFLAGS)

clean:
	rm -rf build/bin/* build/debug/boot/Dkernel.elf build/debugBin/* build/iso/boot/kernel.elf build/isos/* build/obj/*
