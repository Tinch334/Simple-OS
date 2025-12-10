#pragma once
#include "common.h"

//Represents the program state saved in kernel_entry, the order of the registers is the same as the one in the function.
struct trap_frame {
    uint32_t ra;
    uint32_t gp;
    uint32_t tp;
    uint32_t t0;
    uint32_t t1;
    uint32_t t2;
    uint32_t t3;
    uint32_t t4;
    uint32_t t5;
    uint32_t t6;
    uint32_t a0;
    uint32_t a1;
    uint32_t a2;
    uint32_t a3;
    uint32_t a4;
    uint32_t a5;
    uint32_t a6;
    uint32_t a7;
    uint32_t s0;
    uint32_t s1;
    uint32_t s2;
    uint32_t s3;
    uint32_t s4;
    uint32_t s5;
    uint32_t s6;
    uint32_t s7;
    uint32_t s8;
    uint32_t s9;
    uint32_t s10;
    uint32_t s11;
    uint32_t sp;
} __attribute__((packed)); // Compress all fields, don't use padding.

// Read CSR(Control status register).
#define READ_CSR(reg)                                           \
    ({                                                          \
        unsigned long __tmp;                                    \
        __asm__ __volatile__("csrr %0, " #reg : "=r"(__tmp));   \
        __tmp;                                                  \
    })

// Write the specified value to the CSR(Control status register).
#define WRITE_CSR(reg, value)                                   \
    do {                                                        \
        uint32_t __tmp = (value);                               \
        __asm__ __volatile__("csrw " #reg ", %0" ::"r"(__tmp)); \
    } while(0)

//Defined as a macro so __FILE__ and __LINE__ are correct.
//Note that __VA_ARGS__ takes and supplies an arbitrary amount of arguments, the "##" is to remove the comma in case of no additional arguments.
#define PANIC(fmt, ...)                                                         \
    do {                                                                        \
        printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);   \
        while (1) {}                                                            \
    } while (0) //A simple way to enclose multiline macros, a loop that will only run once.

struct sbiret {
    long error;
    long value;
};

// Performs ECALL to OpenSBI.
struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4,
                       long arg5, long fid, long eid);

// Writes a single character to the console.
void putchar(const char ch);

// Maximum number of concurrent processes.
#define PROCS_MAX 8

#define PROC_UNUSED 0 // Unused process control structure.
#define PROC_RUNNABLE 1 // Process is runnable process.

struct process {
    int pid;                // Process ID.
    int state;              // Process state: PROC_UNUSED or PROC_RUNNABLE.
    vaddr_t sp;             // Stack pointer.
    uint8_t stack[8192];    // Kernel stack, for the process.
};