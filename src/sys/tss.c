#include <sys/tss.h>

static struct tss kernel_tss = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

struct tss *tss_get()
{
    return &kernel_tss;
}

void tss_update(unsigned int ss0, unsigned int esp0)
{
    kernel_tss.ss0 = ss0;
    kernel_tss.esp0 = esp0;
}
