#include <libc/mem.h>

void *memcpy(void *dest, const void *src, unsigned int n)
{
	unsigned char *tmp_dest = dest;
	const unsigned char *tmp_src = src;

	while(n--)
	{
		*tmp_dest++ = *tmp_src++;
	}
	
	return dest;
}

void *memset(void *s, int c, unsigned int n)
{
	unsigned char *tmp = s;

	while(n--)
	{
		*tmp++ = c;
	}

	return s;
}
