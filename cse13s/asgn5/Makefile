CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic -gdwarf-4 -g -O3 -Ofast $(shell pkg-config --cflags gmp) -pg
LFLAGS = $(shell pkg-config --libs gmp) -pg

all: keygen encrypt decrypt

keygen: keygen.o rsa.o randstate.o numtheory.o
	$(CC) -o $@ $^ $(LFLAGS)

encrypt: encrypt.o rsa.o randstate.o numtheory.o
	$(CC) -o $@ $^ $(LFLAGS)

decrypt: decrypt.o rsa.o randstate.o numtheory.o
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f keygen encrypt decrypt *.o

cleankeys:
	rm -f *.{pub,priv}

format:
	clang-format -i -style=file *.[ch]
