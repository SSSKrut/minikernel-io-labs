extern char __bss[], __bss_end[], __stack_top[];

struct sbiret {
    long error;
    long value;
};

static inline struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4,
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
                         : "+r"(a0), "+r"(a1)
                         : "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a6), "r"(a7)
                         : "memory");
    return (struct sbiret){a0, a1};
}

void putchar(char ch) { sbi_call(ch, 0, 0, 0, 0, 0, 0, 0x1); }

int getchar(void) {
     return sbi_call(0, 0, 0, 0, 0, 0, 0, 0x02).error; 
}

void puts(const char *s) {
    while (*s) putchar(*s++);
    putchar('\n');
}

void print_num(unsigned long num) {
    if (num >= 10) print_num(num / 10);
    putchar('0' + (num % 10));
}

void kernel_main(void) {
    puts("\n========== OpenSBI Menu ==========");
    puts("Available commands:");
    puts("1. Get SBI specification version");
    puts("2. Get number of counters");
    puts("3. Get details of a counter");
    puts("4. System Shutdown");

    while (1) {
        int choice = -1;
        while ((choice = getchar()) == -1); 
        puts("Chosen option: ");
        putchar(choice);
        puts("\n");

        switch (choice) {
            case '1': {
                struct sbiret ret = sbi_call(0, 0, 0, 0, 0, 0, 0, 0x10);
                puts("SBI specification version: ");
                print_num(ret.value >> 16);
                putchar('.');
                print_num(ret.value & 0xFFFF);
                putchar('\n');
                break;
            }
            case '2': {
                struct sbiret ret = sbi_call(0, 0, 0, 0, 0, 0, 0, 0x30);
                puts("Number of available counters: ");
                print_num(ret.value);
                putchar('\n');
                break;
            }
            case '3': {
                puts("Enter counter ID:");
                int counter_id = -1;
                while ((counter_id = getchar()) == -1);
                putchar(counter_id);
                puts("\n");

                struct sbiret ret = sbi_call(counter_id - '0', 0, 0, 0, 0, 0, 0, 0x31);
                puts("Counter details: ");
                print_num(ret.value);
                putchar('\n');
                break;
            }
            case '4': {
                puts("Shutting down the system...");
                sbi_call(0, 0, 0, 0, 0, 0, 0, 0x08);
                break;
            }
            default:
                puts("\nInvalid choice! Please enter 1-4\n");
        }
    }
}

__attribute__((section(".text.boot"))) __attribute__((naked)) void boot(void) {
    __asm__ __volatile__(
        "mv sp, %[stack_top]\n"
        "j kernel_main\n"
        :
        : [stack_top] "r"(__stack_top));
}