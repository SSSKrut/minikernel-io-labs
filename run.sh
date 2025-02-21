set -xue

QEMU=qemu-system-riscv32

CC=/usr/bin/clang
CFLAGS="-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32 -ffreestanding -nostdlib"

$CC $CFLAGS -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o kernel.elf kernel.c

$QEMU -machine virt -bios default -nographic -serial mon:stdio --no-reboot -kernel kernel.elf