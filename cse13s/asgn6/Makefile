CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic -gdwarf-4 -g $(shell pkg-config --cflags gmp)
OBJECTS = banhammer.o bf.o bv.o ll.o node.o ht.o parser.o city.o

all: banhammer

banhammer: $(OBJECTS) messages.h
	$(CC) -o banhammer  $(OBJECTS) 

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJECTS)

spotless:
	rm -f banhammer $(OBJECTS)
	rm -f banhammer

format:
	clang-format -i -style=file *.[ch]
