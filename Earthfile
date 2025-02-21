VERSION 0.6

deps:
    FROM gcc:latest
    WORKDIR /workspace
    RUN apt-get update && apt-get install -y \
        clang \
        clang-format \
        clang-tidy \
        make

build:
    FROM +deps
    COPY . .
    RUN make
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
