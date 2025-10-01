include ../../config.mk

.PHONY : all clean

INCLUDES = -I../../include
CFLAGS += $(INCLUDES)

SOURCES_MICROCPU := microcpu.c
OBJECTS_MICROCPU := $(SOURCES_MICROCPU:.c=.o)

SOURCES_MICROASM := microasm.c
OBJECTS_MICROASM := $(SOURCES_MICROASM:.c=.o)

SOURCES_MICRODISASM := microdisasm.c
OBJECTS_MICRODISASM := $(SOURCES_MICRODISASM:.c=.o)

%:
	$(CC) -MM $(SOURCES_MICROCPU) $(SOURCES_MICRODISASM) $(SOURCES_MICROASM) $(INCLUDES) > objects.mk

all: microcpu microasm microdisasm

clean: 
	rm -f microcpu microasm microdisasm
	rm -f objects.mk
	rm -f *.o

microcpu: $(OBJECTS_MICROCPU) ../../src/libsquare_equation.a
	$(CC) -lm $(CFLAGS) -o $@ $^
	cp $@ ../../

microasm: $(OBJECTS_MICROASM) ../../src/libsquare_equation.a
	$(CC) -lm $(CFLAGS) -o $@ $^
	cp $@ ../../

microdisasm: $(OBJECTS_MICRODISASM) ../../src/libsquare_equation.a
	$(CC) -lm $(CFLAGS) -o $@ $^
	cp $@ ../../

include objects.mk
