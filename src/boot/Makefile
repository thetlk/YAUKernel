
OBJ = boot.bin

all: $(OBJ)

$(OBJ): boot.asm
	nasm -f elf32 -o $@ $^

clean:
	rm -rf $(OBJ)
