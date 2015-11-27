
SRC_DIR = $(shell pwd)/src

.PHONY: kernel

all: kernel

kernel:
	make -C $(SRC_DIR)

clean:
	make -C $(SRC_DIR) clean
