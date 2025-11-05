typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

//Get linker symbols, this gets us the address of those symbols.
extern char __bss[], __bss_end[], __stack_top[];

void *memset(void *buf, char c, size_t n) {
    //Cast to "uint8_t", so incrementing p advances trough buffer on byte at a time.
    uint8_t *p = (uint8_t *) buf;

    while(n--) {
        *p++ = c;
    }
    return buf;
}

void kernel_main(void) {
    //Manually clear .bss section, just in case.
    memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);

    for (;;);
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