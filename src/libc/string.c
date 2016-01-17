#include <libc/string.h>

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

int __itoa_recursive(char *buff, unsigned int n, unsigned int base)
{
    char chars[] = "0123456789abcdef";
    unsigned int deep = 0;

    if(base > 16)
    {
        base = 16;
    }

    if(n < base)
    {
        buff[0] = chars[n];
    } else {
        deep = __itoa_recursive(buff, n/base, base) + 1;
        buff[deep] = chars[n%base];
    }

    return deep;
}

void itoa(char *buff, unsigned int n, unsigned int base)
{
    unsigned int deep;
    deep = __itoa_recursive(buff, n, base);
    buff[deep+1] = '\0';
}