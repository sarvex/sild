SHELL = /bin/sh
CC = cc
CFLAGS = -Wall -Werror

sild: src/obj/read.o src/obj/print.o src/obj/builtins.o src/obj/eval.o src/obj/cell.o src/obj/util.o src/main.c
	$(CC) $(CFLAGS) src/obj/read.o src/obj/print.o src/obj/builtins.o src/obj/eval.o src/obj/cell.o src/obj/util.o src/main.c -o sild

src/obj/util.o: src/util.c src/obj
	$(CC) $(CFLAGS) src/util.c -c -o src/obj/util.o

src/obj/cell.o: src/cell.c src/obj
	$(CC) $(CFLAGS) src/cell.c -c -o src/obj/cell.o

src/obj/eval.o: src/eval.c src/obj
	$(CC) $(CFLAGS) src/eval.c -c -o src/obj/eval.o

src/obj/builtins.o: src/builtins.c src/obj
	$(CC) $(CFLAGS) src/builtins.c -c -o src/obj/builtins.o

src/obj/print.o: src/print.c src/obj
	$(CC) $(CFLAGS) src/print.c -c -o src/obj/print.o

src/obj/read.o: src/read.c src/obj
	$(CC) $(CFLAGS) src/read.c -c -o src/obj/read.o

src/obj:
	mkdir src/obj

.PHONY: clean run
clean:
	rm sild
	rm -r src/obj

run: sild
	./sild
