#include "kernel.h"
#include "common.h"

//Get linker symbols, this gets us the address of those symbols.
extern char __bss[], __bss_end[], __stack_top[];

//Perform ECALL to OpenSBI. Transfers control between the SEE and the supervisor.
struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4,
                       long arg5, long fid, long eid) {
    register long a0 __asm__("a0") = arg0;
    register long a1 __asm__("a1") = arg1;
    register long a2 __asm__("a2") = arg2;
    register long a3 __asm__("a3") = arg3;
    register long a4 __asm__("a4") = arg4;
    register long a5 __asm__("a5") = arg5;
    register long a6 __asm__("a6") = fid;
    register long a7 __asm__("a7") = eid;

    __asm__ __volatile__("ecall"
                         : "=r"(a0), "=r"(a1)
                         : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5),
                           "r"(a6), "r"(a7)
                         : "memory");
    return (struct sbiret){.error = a0, .value = a1};
}

//A putchar function that prints to the debug console.
void putchar(const char ch) {
    sbi_call(ch, 0, 0, 0, 0, 0, 0, 1);
}

void kernel_main(void) {
    printf("\n\nHello %s\n", "World!");
    printf("1 + 2 = %d, %x\n", 1 + 2, 0x1234abcd);

    for (;;) {
        //Halt, by waiting for an interruption that will never come.
        __asm__ __volatile__("wfi");
    }
}

__attribute__ ((section(".text.boot"))) //Place instruction at ".text.boot", address used by OpenSBI.
__attribute__ ((naked)) //Don't generate unnecessary code before and after function.
void boot(void) {
    __asm__ __volatile__ (
        "mv sp, %[stack_top]\n" //Set stack pointer to stack start.
        "j kernel_main"         //Jump to kernel main function.
        :
        : [stack_top] "r" (__stack_top) // Pass the stack top address as %[stack_top].
    );
}