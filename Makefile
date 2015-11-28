
SRC_DIR = $(shell pwd)/src
BUILD_DIR = $(shell pwd)/build
ISO_DIR = $(BUILD_DIR)/isodir
GRUB_DIR = /usr/lib/grub/i386-pc/

all: kernel

kernel:
	make -C $(SRC_DIR)

iso: kernel
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(BUILD_DIR)/kernel $(ISO_DIR)/boot/
	cp grub.cfg $(ISO_DIR)/boot/grub
	grub-mkrescue -d $(GRUB_DIR) -o $(BUILD_DIR)/YAUK.iso $(ISO_DIR)

run: iso
	qemu-system-i386 -cdrom $(BUILD_DIR)/YAUK.iso

clean:
	make -C $(SRC_DIR) clean
