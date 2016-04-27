SHELL = /bin/sh
CC = cc

sild: eval.o cell.o util.o sild.c
	$(CC) eval.o cell.o util.o sild.c -o sild

util.o: util.c
	$(CC) util.c -c

cell.o: cell.c
	$(CC) cell.c -c

eval.o: eval.c
	$(CC) eval.c -c

clean:
	rm sild

run: sild
	./sild
