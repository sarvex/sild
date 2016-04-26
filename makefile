sild: read.o print.o builtins.o eval.o cell.o utils.o sild.c
	cc read.o print.o builtins.o eval.o cell.o utils.o sild.c -o sild

read.o:
	cc -c read.c

print.o:
	cc -c print.c

builtins.o:
	cc -c builtins.c

eval.o:
	cc -c eval.c

cell.o:
	cc -c cell.c

utils.o:
	cc -c utils.c

clean:
	rm *.o sild

run: sild
	./sild
	make clean
