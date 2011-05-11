CC=gcc
CFLAGS=-Wall -Iinc -std=c99 -I/usr/X11/include
LFLAGS=-L/usr/X11/lib

TDIR=
TODIR=
ODIR=obj

_TEST = 
TEST = $(patsubst %,$(TODIR)/%.test,$(_TEST))

_OBJ = adventuretris.o piece.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

adventuretris: $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)

$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o adventuretris
