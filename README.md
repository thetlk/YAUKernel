
# Yet Another Unfinished|Useless Kernel

Just to learn some stuff

## Compile

gcc, nasm and cmake must be installed
```
mkdir build && cd build
cmake ..
make
```

directly run the kernel
```
qemu-system-i386 -kernel bin/kernel
```

Create iso image (grub must be installed)
```
make iso
```

run iso
```
qemu-system-i386 -cdrom bin/YAUK.iso
```

