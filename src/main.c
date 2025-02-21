extern char __bss[], __bss_end[], __stack_top[];

void kernel_main(void) {
    for (;;);
}

__attribute((section(".text.boot")))
__attribute((naked))
void boot(void) {
    __asm__ __volatile__(
        "mv sp, %[stack_top]\n"
        "j kernel_main\n"
        :
        : [stack_top] "r" (__stack_top)
    );
}