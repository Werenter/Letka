include config.mk

.PHONY : all

all: square_equation

clean: 
	rm -rf *.o square_equation

square_equation: main.o float.o square_equation.o sq_io.o
	$(CC) -o $@ $^ $(CFLAGS)

main.o: main.c *.h
float.o: float.c *.h
square_equation.o: square_equation.c *.h
sq_io.o: sq_io.c *.h
