#include <sys/tss.h>

static struct tss kernel_tss = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void *tss_get_address()
{
	return (void*) &kernel_tss;
}

unsigned int tss_get_ss0()
{
	return kernel_tss.ss0;
}

unsigned int tss_get_esp0()
{
	return kernel_tss.esp0;
}

void tss_update(unsigned int ss0, unsigned int esp0)
{
	kernel_tss.ss0 = ss0;
	kernel_tss.esp0 = esp0;
}
