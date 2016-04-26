SHELL = /bin/sh
CC = cc

sild: sild.c
	$(CC) sild.c -o sild

clean:
	rm sild

run: sild
	./sild
