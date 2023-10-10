CC=gcc
CFLAGS=-I.
DEPS =

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: stub.o P1 P2 P3

P1: P1.c 
	gcc -g -Wall -o P1 P1.c -lpthread stub.o

P2: P2.c 
	gcc -g -Wall -o P2 P2.c -lpthread stub.o

P3: P3.c 
	gcc -g -Wall -o P3 P3.c -lpthread stub.o

clean:
	rm -f *.o P1 P2 P3
