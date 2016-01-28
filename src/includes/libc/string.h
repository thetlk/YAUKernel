#ifndef STRING_H
#define STRING_H

void *memcpy(void *dest, const void *src, unsigned int n);
void *memset(void *s, int c, unsigned int n);
unsigned int strlen(char *s);
void itoa(char *buff, unsigned int n, unsigned int base);
int __itoa_recursive(char *buff, unsigned int n, unsigned int base);

#endif