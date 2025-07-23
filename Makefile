CFLAGS ?= -O2 -g
CFLAGS += -std=gnu99
CFLAGS += -Wall -Werror -Wformat-security -Wpointer-arith -Wtype-limits
CFLAGS += -Wstrict-prototypes -Wmissing-prototypes -Wfloat-equal
CFLAGS += -fsanitize=undefined -fsanitize-undefined-trap-on-error

CC += -m32 -no-pie -fno-pie
LDLIBS = -lm

.PHONY: all test clean

all: integral

integral.o: integral.c
	$(CC) $(CFLAGS) -o integral.o -c integral.c

function.o: function.asm
	nasm -f elf32 function.asm -o function.o

integral: integral.o function.o
	$(CC) $(CFLAGS) -o integral integral.o function.o $(LDLIBS)

build-test: integral
	cp integral test

test: build-test
	./test --test-root 1:2:0.5:2:0.000001:0.8457911
	./test --test-root 1:3:0.5:2:0.000001:0.7076254
	./test --test-root 2:3:0.5:2:0.000001:1.4685771
	./test --test-integral 1:0.1:2.3:0.000001:47.826087
	./test --test-integral 2:-3:-1:0.000001:0.95427712
	./test --test-integral 3:-1.5:-0.7:0.000001:2.2038764

clean:
	rm -f *.o integral test
