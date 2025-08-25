include ../../config.mk

.PHONY : all clean

INCLUDES = -I../../include
CFLAGS += $(INCLUDES)

SOURCES := main.c
OBJECTS := $(SOURCES:.c=.o)

%:
	$(CC) -MM $(SOURCES) $(INCLUDES) > objects.mk

all: square_equation

clean: 
	rm -f square_equation
	rm -f objects.mk
	rm -f *.o

square_equation: $(OBJECTS) ../../src/libsquare_equation.a
	$(CC) -lm $(CFLAGS) -o $@ $^
	cp square_equation ../../

include objects.mk
