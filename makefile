SHELL = /bin/sh
CC = cc

sild: builtins.o eval.o cell.o util.o sild.c
	$(CC) builtins.o eval.o cell.o util.o sild.c -o sild

util.o: util.c
	$(CC) util.c -c

cell.o: cell.c
	$(CC) cell.c -c

eval.o: eval.c
	$(CC) eval.c -c

builtins.o: builtins.c
	$(CC) builtins.c -c

clean:
	rm sild

run: sild
	./sild
