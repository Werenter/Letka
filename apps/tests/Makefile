include ../../config.mk

.PHONY : all clean

INCLUDES = -I../../include
CFLAGS += $(INCLUDES)

SOURCES := tests.c tests_main.c
OBJECTS := $(SOURCES:.c=.o)

%:
	$(CC) -MM $(SOURCES) $(INCLUDES) > objects.mk

all: tests

clean: 
	rm -rf tests
	rm -f objects.mk
	rm -f *.o

tests: $(OBJECTS) ../../src/libsquare_equation.a
	$(CC) -lm $(CFLAGS) -o $@ $^
	cp tests ../../

include objects.mk
