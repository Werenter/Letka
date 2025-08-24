include config.mk

.PHONY : all docs clean

%:
	$(CC) -MM *.c -o objects.mk

all: square_equation tests

docs:
	doxygen doxygen_config

clean: 
	rm -rf *.o square_equation tests
	rm -f objects.mk

square_equation: main.o float.o square_equation.o sq_io.o
	$(CC) -o $@ $^ $(CFLAGS) -lm

tests: tests_main.o tests.o float.o square_equation.o sq_io.o
	$(CC) -o $@ $^ $(CFLAGS) -lm

include objects.mk

#main.o: main.c *.h
#float.o: float.c *.h
#square_equation.o: square_equation.c *.h
#sq_io.o: sq_io.c *.h
#tests.o: tests.c *.h
#tests_main.o: tests_main.c *.h
