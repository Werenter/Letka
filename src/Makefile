include ../config.mk

.PHONY : all clean

INCLUDES = -I../include
CFLAGS += $(INCLUDES)

%:
	$(CC) -MM *.c -o objects.mk $(INCLUDES)

all: libsquare_equation.a

clean: 
	rm -rf libsquare_equation.a
	rm -f objects.mk
	rm -f *.o

libsquare_equation.a: float.o square_equation.o sq_io.o
	$(AR) r $@ $^

include objects.mk
