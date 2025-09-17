include ../../config.mk

.PHONY : all clean

INCLUDES = -I../../include
CFLAGS += $(INCLUDES)

SOURCES := main.c
OBJECTS := $(SOURCES:.c=.o)

%:
	$(CC) -MM $(SOURCES) $(INCLUDES) > objects.mk

all: microgrep

clean: 
	rm -f microgrep
	rm -f objects.mk
	rm -f *.o

microgrep: $(OBJECTS) ../../src/libsquare_equation.a
	$(CC) -lm $(CFLAGS) -o $@ $^
	cp microgrep ../../

include objects.mk
