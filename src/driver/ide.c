#include <driver/ide.h>
#include <sys/asm.h>

void ide_select(uint32_t drive, uint32_t numblock, uint32_t count)
{
    outb(0x1F1, 0x00);
    outb(0x1F2, count);
    outb(0x1F3, numblock & 0xFF);
    outb(0x1F4, (numblock >> 8) & 0xFF);
    outb(0x1F5, (numblock >> 16) & 0xFF);
    outb(0x1F6, 0xE0 | (drive << 4) | ((numblock >> 24) & 0x0F));
}

uint32_t ide_read(uint32_t drive, uint32_t numblock, uint32_t count, char *buf)
{
    uint16_t tmp;
    uint32_t idx;

    ide_select(drive, numblock, count);
    outb(0x1F7, 0x20);

    while((inb(0x1F7) & 0x08) == 0);

    for(idx = 0; idx < 256 * count; idx++)
    {
        tmp = inw(0x1F0);
        buf[idx*2] = (char) (tmp & 0xFF);
        buf[idx*2 + 1] = (char) ((tmp >> 8) & 0xFF);
    }

    return count;
}

uint32_t ide_write(uint32_t drive, uint32_t numblock, uint32_t count, char *buf)
{
    uint16_t tmp;
    uint32_t idx;

    ide_select(drive, numblock, count);
    outb(0x1F7, 0x30);

    while((inb(0x1F7) & 0x08) == 0);

    for(idx = 0; idx < 256 * count; idx++)
    {
        tmp = (uint16_t) ((buf[idx * 2 + 1] << 8) | buf[idx * 2]);
        outw(0x1F0, tmp);
    }

    return count;
}

