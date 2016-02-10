#ifndef IDE_H
#define IDE_H

#include <sys/types.h>

void ide_select(uint32_t drive, uint32_t numblock, uint32_t count);
uint32_t ide_read(uint32_t drive, uint32_t numblock, uint32_t count, char *buf);
uint32_t ide_write(uint32_t drive, uint32_t numblock, uint32_t count, char *buf);

#endif