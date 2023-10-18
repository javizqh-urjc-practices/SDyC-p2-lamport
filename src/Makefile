CC=gcc
CFLAGS=-I.
DEPS =

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: stub.o P1 P2 P3

debug: stub.o P1_debug P2_debug P3_debug

P1: P1.c 
	gcc -g -Wall -o P1 P1.c -lpthread stub.o

P1_debug: P1.c 
	gcc -g -DDEBUG -Wall -o P1 P1.c -lpthread stub.o

P2: P2.c 
	gcc -g -Wall -o P2 P2.c -lpthread stub.o

P2_debug: P2.c 
	gcc -g -DDEBUG -Wall -o P2 P2.c -lpthread stub.o

P3: P3.c 
	gcc -g -Wall -o P3 P3.c -lpthread stub.o

P3_debug: P3.c 
	gcc -g -DDEBUG -Wall -o P3 P3.c -lpthread stub.o

clean:
	rm -f *.o P1 P2 P3
