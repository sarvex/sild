sild: cell.o utils.o sild.c
	cc cell.o utils.o sild.c -o sild

cell.o:
	cc -c cell.c

utils.o:
	cc -c utils.c

clean:
	rm *.o sild
