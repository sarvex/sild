sild: sild.c
	cc sild.c -o sild

clean:
	rm sild

run: sild
	./sild
