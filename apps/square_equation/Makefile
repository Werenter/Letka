include ../../config.mk

.PHONY : all clean

INCLUDES = -I../../include
CFLAGS += $(INCLUDES)

%:
	$(CC) -MM *.c -o objects.mk $(INCLUDES)

all: square_equation

clean: 
	rm -f square_equation
	rm -f objects.mk
	rm -f *.o

square_equation: main.o ../../src/libsquare_equation.a
	$(CC) -lm $(CFLAGS) -o $@ $^
	cp square_equation ../../

include objects.mk
