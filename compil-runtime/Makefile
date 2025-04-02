LDFLAGS = -nostdlib
CFLAGS = -Wall -g

all: main

main: main.o fun.o

%.o: %.asm
	nasm -g -felf64 -F dwarf $< -o $@

fun.asm:
	cp -n fun.def.asm fun.asm || true

dist: Makefile main.c fun.def.asm readme
	mkdir -p compil-runtime
	cp -t compil-runtime $^
	tar czvf compil-runtime.tgz compil-runtime
	rm -rf compil-runtime