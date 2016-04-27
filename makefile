SHELL = /bin/sh
CC = cc
CFLAGS = -Wall -Werror
OBJDIR=src/obj
vpath %.c src
vpath %.h src

objects = $(addprefix $(OBJDIR)/, util.o cell.o eval.o builtins.o print.o read.o main.o)

sild: $(objects)
	$(CC) $(CFLAGS) -o sild $(objects)

$(OBJDIR)/%.o: %.c $(OBJDIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR):
	mkdir src/obj

.PHONY: clean run
clean:
	rm sild
	rm -r src/obj

run: sild
	./sild
