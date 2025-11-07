#pragma once

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef uint32_t size_t;
typedef uint32_t paddr_t; //Represents physical memory addresses.
typedef uint32_t vaddr_t; //Represents virtual memory addresses, equivalent to uintptr_t.

#define true 1
#define false 0
#define NULL ((void *) 0)
#define align_up(value, align) __builtin_align_up(value, align) //Rounds value to the nearest multiple of align, align must be a power of 2.
#define is_aligned(value, align) __builtin_is_aligned(value, align) //Checks if is a multiple of align, align must be a power of 2.
#define offsetoff(type, member)  __builtin_offsetoff(type, member) //Returns offset for a member within a structure.

//Builtins from the compiler for arbitrary argument handling, the aliases are for easier usage.
#define va_list  __builtin_va_list
#define va_start __builtin_va_start
#define va_end   __builtin_va_end
#define va_arg   __builtin_va_arg

#define PAGE_SIZE 4096

void *memset(void *buf, char c, size_t n);
void *memcpy(void *dst, const void *src, size_t n);
char *strcpy(char *dst, const char *src);
char *strcpy_s(char *dst, size_t dst_s, const char *src);
int strcmp(const char *s1, const char *s2);
void printf(const char *fmt, ...);