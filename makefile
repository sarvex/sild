sild: utils.o sild.c
	cc utils.o sild.c -o sild

utils.o:
	cc -c utils.c

clean:
	rm *.o sild
