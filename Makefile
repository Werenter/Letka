include config.mk

.PHONY : all

all: square_equation

clean: 
	rm -rf *.o square_equation

square_equation: main.o
	$(CC) -o $@ $^ $(CFLAGS)

main.o: main.c
