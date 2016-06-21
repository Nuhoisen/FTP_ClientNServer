CFLAGS = -g -Wall -Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes -Wdeclaration-after-statement -lpthread
PROG = owlzillas owlzillac


all:$(PROG)

owlzillas: owlzillas.o
	gcc $(CFLAGS) -o owlzillas owlzillas.o

owlzillas.o: owlzillas.c
	gcc $(CFLAGS) -c owlzillas.c

owlzillac: owlzillac.o
	gcc $(CFLAGS) -o owlzillac owlzillac.o

owlzillac.o: owlzillac.c
	gcc $(CFLAGS) -c owlzillac.c

clean:
	rm -f $(PROG) *.o *~ \#*

