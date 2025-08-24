include ../../config.mk

.PHONY : all clean

INCLUDES = -I../../include
CFLAGS += $(INCLUDES)

%:
	$(CC) -MM *.c -o objects.mk $(INCLUDES)

all: tests

clean: 
	rm -rf tests
	rm -f objects.mk
	rm -f *.o

tests: tests.o tests_main.o ../../src/libsquare_equation.a
	$(CC) -lm $(CFLAGS) -o $@ $^
	cp tests ../../

include objects.mk
