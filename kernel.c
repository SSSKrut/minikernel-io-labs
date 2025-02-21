extern char __bss[], __bss_end[], __stack_top[];

struct sbiret {
    long error;
    union {
        long value;
        unsigned long uvalue;
    };
    
};

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

void putchar(char ch) {
    sbi_call(ch, 0, 0, 0, 0, 0, 0, 1);
}

char getchar() {
    struct sbiret result = sbi_call(0, 0, 0, 0, 0, 0, 0, 2); 
    while (result.error == -1) {
        result = sbi_call(0, 0, 0, 0, 0, 0, 0, 2);
    }
    return result.value;
}


void print_console(char str) {
    
}

char read_line() {
    char read_str[1024] = {0};
    int last_index = 0;

    while (1) {
        char read_char = getchar();
        
        if (last_index > 1022){
            read_str[last_index] = 0;
            return read_str;
        }
        if (read_char == 0x03) {
            read_str[last_index] = 0;
            return read_str;
        } else {
            read_str[last_index] = read_char;
            last_index++;
            putchar(read_char);
        }
    }
}

void kernel_main(void) {
    return;
    while (1) {
        putchar(getchar());
    }
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
