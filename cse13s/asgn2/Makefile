CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: mathlib-test 

mathlib-test: mathlib.o mathlib-test.o
	$(CC) -o mathlib-test mathlib-test.o mathlib.o -lm

mathlib-test.o: mathlib-test.c
	$(CC) $(CFLAGS) -c mathlib-test.c 
	
mathlib.o: mathlib.c
	$(CC) $(CFLAGS) -c mathlib.c 
clean:
	rm -f mathlib-test mathlib-test.o mathlib.o

format:
	clang-format -i -style=file *.{c,h}

