CFLAGS=-Wall -Wextra -O3

.PHONY: all clean

all: lib/libstd.a lib/libstd.so

lib/libstd.o: libstd.c
	${CC} ${CFLAGS} -nostdlib --static -c -o $@ $^

lib/libstd.a: lib/libstd.o
	ar rcs $@ $^

lib/libstd.so: libstd.c
	${CC} ${CFLAGS} -nostdlib -c -o $@ $^

clean:
	rm -r lib/*
 

