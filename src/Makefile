
CC = gcc
CFLAGS = -Wall -m32 -O2 -nostdlib -nodefaultlibs

BOOT_DIR = $(shell pwd)/boot
BOOT_OBJECT = $(BOOT_DIR)/boot.bin

OBJ = kmain.o

all: $(BOOT_OBJECT) kernel

$(BOOT_OBJECT):
	make -C $(BOOT_DIR)

kernel: $(OBJ)
	$(LD) -m elf_i386 -Ttext=100000 --entry=_start -o $@ $(BOOT_OBJECT) $^

clean:
	make -C $(BOOT_DIR) clean
	rm -rf $(OBJ)
