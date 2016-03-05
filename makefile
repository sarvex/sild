sild: cell.o debugging.o parser.o
	cc src/sild.c cell.o debugging.o parser.o -o sild

cell.o:
	cc -c src/cell.c -o cell.o

debugging.o:
	cc -c src/debugging.c -o debugging.o

parser.o:
	cc -c src/parser.c -o parser.o

clean:
	rm sild

.INTERMEDIATE: cell.o debugging.o parser.o
