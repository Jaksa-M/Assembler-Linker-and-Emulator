# Assembler, Linker and Emulator

A small toolchain for a made-up 32-bit processor: an assembler, a linker and an emulator that runs the result. I wrote it for the System Software course at the School of Electrical Engineering in Belgrade.

You write assembly, the assembler turns each file into an object file, the linker joins them into a memory image, and the emulator executes that image and prints the register state when the program halts.

## Parts

**Assembler** (`src/assembler.cpp`, `misc/lexer.l`, `misc/parser.y`)
Parsing is done with Flex and Bison. It works in one pass; forward references are collected in a table and patched at the end. The output is a relocatable object file with a symbol table and relocation records, laid out like an ELF file.

Supported directives: `.global`, `.extern`, `.section`, `.word`, `.skip`, `.ascii`, `.end`.

**Linker** (`src/linker.cpp`)
Merges sections from all input files, resolves symbols and applies relocations. It has two modes:

- `-hex` produces a loadable memory image. Sections can be pinned to an address with `-place=section@address`.
- `-relocatable` produces another object file that can be linked again later.

**Emulator** (`src/emulator.cpp`)
Emulates the CPU: 16 general purpose registers (`r14` is `sp`, `r15` is `pc`) and three control registers (`status`, `handler`, `cause`). Execution starts at `0x40000000`. There is a memory-mapped terminal (output at `0xFFFFFF00`, input at `0xFFFFFF04`), and a key press raises an interrupt.

## Build

Linux only, because the emulator uses `termios` and the assembler uses `elf.h`. You need `g++`, `flex` and `bison`.

```
make test_prepare
```

This builds `assembler`, `linker` and `emulator` in the repo root.

## Run

```
./assembler -o main.o main.s
./linker -hex -place=my_code@0x40000000 -o program.hex main.o
./emulator program.hex
```

There is a ready-made example in `tests/nivo-a`, with several source files, a software interrupt and the terminal:

```
cd tests/nivo-a
chmod +x start.sh
./start.sh
```
