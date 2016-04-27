SHELL = /bin/sh
CC = cc

sild: util.o sild.c
	$(CC) util.o sild.c -o sild

util.o: util.c
	$(CC) util.c -c

clean:
	rm sild

run: sild
	./sild
