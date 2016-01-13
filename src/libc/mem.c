#include <libc/mem.h>

void *memcpy(unsigned char *dst, unsigned char *src, unsigned int l)
{
	unsigned char *ret = dst;

	while(l--)
	{
		*dst++ = *src++;
	}
	
	return ret;
}

void *memset(unsigned char *dst, int c, unsigned int size)
{
	unsigned char *ret = dst;

	while(size--)
	{
		*dst++ = c;
	}

	return ret;
}
