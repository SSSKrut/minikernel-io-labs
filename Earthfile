VERSION 0.6

deps:
    FROM gcc:latest
    WORKDIR /workspace
    RUN apt-get update && apt-get install -y \
        clang \
        llvm \
        lld \
        curl \
        qemu-system-riscv32
    RUN curl -LO https://github.com/qemu/qemu/raw/v8.0.4/pc-bios/opensbi-riscv32-generic-fw_dynamic.bin

build:
    FROM +deps
    COPY . .
    RUN make earth-run
    SAVE ARTIFACT build/program AS LOCAL build/program

lint:
    FROM +deps
    COPY . .
    RUN make lint

format:
    FROM +deps
    COPY . .
    RUN make format
    SAVE ARTIFACT src AS LOCAL src
    SAVE ARTIFACT tests AS LOCAL tests

run:
    FROM +deps
    COPY . .
    RUN make run
