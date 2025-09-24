include ../../config.mk

.PHONY : all clean

INCLUDES = -I../../include
CFLAGS += $(INCLUDES)

SOURCES := main.c
OBJECTS := $(SOURCES:.c=.o)

%:
	$(CC) -MM $(SOURCES) $(INCLUDES) > objects.mk

all: stack_test

clean: 
	rm -f stack_test
	rm -f objects.mk
	rm -f *.o

stack_test: $(OBJECTS) ../../src/libsquare_equation.a
	$(CC) -lm $(CFLAGS) -o $@ $^
	cp $@ ../../

include objects.mk
