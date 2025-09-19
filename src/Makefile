include ../config.mk

.PHONY : all clean

INCLUDES = -I../include
CFLAGS += $(INCLUDES)

SOURCES := float.c square_equation.c sq_io.c arg_parse.c logger.c velosiped.c arrays.c strings.c
OBJECTS := $(SOURCES:.c=.o)

%:
	$(CC) -MM $(SOURCES) $(INCLUDES) > objects.mk

all: libsquare_equation.a

clean: 
	rm -rf libsquare_equation.a
	rm -f objects.mk
	rm -f *.o

libsquare_equation.a: $(OBJECTS)
	$(AR) r $@ $^

include objects.mk
