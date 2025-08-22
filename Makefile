include config.mk

.PHONY : all

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)

all: square_equation

clean: 
	rm -rf *.o square_equation

square_equation: $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -c -o $@

#main.o: main.c *.h
#float.o: float.c *.h
#square_equation.o: square_equation.c *.h
#sq_io.o: sq_io.c *.h
