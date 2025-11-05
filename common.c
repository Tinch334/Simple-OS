#include "common.h"

void putchar(char ch);

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
            case 'd': {//Print integer in decimal.
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
            case 'x': {
                unsigned value = va_arg(vargs, unsigned);
                for (int i = 7; i >= 0 ; i--) {
                    //Get groups of 4 bits, convert to hexadecimal and then print appropriate character.
                    unsigned nibble = (value >> (i * 4)) & 0xf;
                    putchar("0123456789abcdef"[nibble]);
                }
            }
            }
        } else {
            putchar(*fmt);
        }

        fmt++;
    }

end:
    va_end(vargs);
}