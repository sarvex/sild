SHELL = /bin/sh
CC = cc
CFLAGS = -Wall -Werror
OBJDIR=obj
vpath %.c src

objects = $(addprefix $(OBJDIR)/, util.o cell.o eval.o builtins.o print.o read.o env.o lambda.o main.o)

sild: $(objects)
	$(CC) $(CFLAGS) -o sild $(objects)

$(OBJDIR)/%.o: %.c $(OBJDIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

.PHONY: clean run
clean:
	rm sild
	rm -r $(OBJDIR)

run: sild
	./sild test.sld
