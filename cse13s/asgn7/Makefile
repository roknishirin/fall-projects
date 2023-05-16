CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic -gdwarf-4 -g $(shell pkg-config --cflags gmp)
SOURCES = $(wildcard *.c)
# OBJECTS = $(SOURCES:%.c=%.o)
OBJECTS = huffman.o node.o pq.o code.o stack.o io.o

all: encode decode

encode: $(OBJECTS) defines.h header.h encode.o encode.c
	$(CC) -o encode  encode.o $(OBJECTS) 

decode: $(OBJECTS) defines.h header.h decode.o decode.c
	$(CC) -o decode  decode.o $(OBJECTS) 

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJECTS)

spotless:
	rm -f encode decode $(OBJECTS)
	rm -f encode decode

format:
	clang-format -i -style=file *.[ch]

