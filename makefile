SHELL = /bin/sh
CC = cc

sild: src/obj/read.o src/obj/print.o src/obj/builtins.o src/obj/eval.o src/obj/cell.o src/obj/util.o src/sild.c
	$(CC) src/obj/read.o src/obj/print.o src/obj/builtins.o src/obj/eval.o src/obj/cell.o src/obj/util.o src/sild.c -o sild

src/obj/util.o: src/util.c src/obj
	$(CC) src/util.c -c -o src/obj/util.o

src/obj/cell.o: src/cell.c src/obj
	$(CC) src/cell.c -c -o src/obj/cell.o

src/obj/eval.o: src/eval.c src/obj
	$(CC) src/eval.c -c -o src/obj/eval.o

src/obj/builtins.o: src/builtins.c src/obj
	$(CC) src/builtins.c -c -o src/obj/builtins.o

src/obj/print.o: src/print.c src/obj
	$(CC) src/print.c -c -o src/obj/print.o

src/obj/read.o: src/read.c src/obj
	$(CC) src/read.c -c -o src/obj/read.o

src/obj:
	mkdir src/obj

.PHONY: clean run
clean:
	rm sild
	rm -r src/obj

run: sild
	./sild
