include config.mk

.PHONY : all docs clean

all:
	$(MAKE) -C src
	$(MAKE) -C apps/tests
	$(MAKE) -C apps/square_equation

docs:
	doxygen doxygen_config

clean: 
	$(MAKE) -C src clean
	$(MAKE) -C apps/tests clean
	$(MAKE) -C apps/square_equation clean
