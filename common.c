#include "common.h"

void putchar(char ch);

void *memset(void *buf, char c, size_t n) {
    //Cast to "uint8_t", so incrementing p advances trough buffer on byte at a time.
    uint8_t *p = (uint8_t *) buf;

    while(n--)
        *p++ = c;

    return buf;
}

void *memcpy(void *dst, const void *src, size_t n) {
    //Cast to "uint8_t", so incrementing pointers advances trough buffer on byte at a time.
    const uint8_t *srcP = (uint8_t *) src;
    uint8_t *dstP = (uint8_t *) dst;

    while (n--)
        *dstP++ = +srcP++;

    return dst;
}

char *strcpy(char *dst, const char *src) {
    char *d = dst;

    while(*src)
        *d++ = *src++;

    *d = '\0';
    return dst;
}

char *strcpy_s(char *dst, size_t dst_s, const char *src) {
    char *d = dst;

    while(*src && dst_s > 1) {
        *d++ = *src++;
        dst_s--;
    }

    *d = '\0';
    return dst;
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && *s1++ == *s2++);

    //Casting to (unsigned char *) done to conform to POSIX specification.
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

void printf(const char *fmt, ...) {
    //Allows for reading an unknown number of arguments.
    va_list vargs;
    va_start(vargs, fmt);

    while(*fmt) {
        if (*fmt == '%') {
            //Skip '%' character.
            fmt++;

            switch(*fmt) {
            case '\0': //There's a '%' at the end of the format string.
                putchar('%');
                goto end;
            case 's': { //Print null terminated string.
                const char *s = va_arg(vargs, const char *);
                while (*s) {
                    putchar(*s);
                    s++;
                }
                break;
            }
            case 'd': { //Print integer in decimal.
                int value = va_arg(vargs, int);
                unsigned magnitude = value;

                //Determine sign of number and turn negative numbers positive.
                if (value < 0) {
                    putchar('-');
                    magnitude = -magnitude;
                }

                unsigned divisor = 1;
                while (magnitude / divisor > 9)
                    divisor *= 10;

                //Get digits starting from most significant and print them one by one.
                while (divisor > 0) {
                    putchar('0' + magnitude / divisor);
                    magnitude %= divisor;
                    divisor /= 10;
                }

                break;
            }
            case 'x': { //Print integer in hexadecimal.
                unsigned value = va_arg(vargs, unsigned);
                for (int i = 7; i >= 0 ; i--) {
                    //Get groups of 4 bits, convert to hexadecimal, then print appropriate character.
                    unsigned nibble = (value >> (i * 4)) & 0xf;
                    putchar("0123456789abcdef"[nibble]);
                }
            }
            }
        } else { //Just a regular character, print it.
            putchar(*fmt);
        }

        fmt++;
    }

end:
    va_end(vargs);
}