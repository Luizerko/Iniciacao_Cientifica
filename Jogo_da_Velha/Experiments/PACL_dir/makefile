CFLAGS = -g -Wall

all: PACL

PACL: PACL.o
	gcc PACL.o -o PACL

PACL.o: PACL.c
	gcc $(CFLAGS) PACL.c -c

clean:
	rm -rf *.o PACL
