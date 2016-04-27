SHELL = /bin/sh
CC = cc

sild: cell.o util.o sild.c
	$(CC) cell.o util.o sild.c -o sild

util.o: util.c
	$(CC) util.c -c

cell.o: cell.c
	$(CC) cell.c -c

clean:
	rm sild

run: sild
	./sild
