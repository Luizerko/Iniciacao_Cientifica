CFLAGS = -g -Wall

all: APACL

APACL: APACL.o
	gcc APACL.o -o APACL

APACL.o: APACL.c
	gcc $(CFLAGS) APACL.c -c

clean:
	rm -rf *.o APACL
