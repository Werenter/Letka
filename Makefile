include config.mk

.PHONY : all docs clean

all:
	$(MAKE) -C src
	$(MAKE) -C apps/tests
	$(MAKE) -C apps/square_equation
	$(MAKE) -C apps/microgrep
	$(MAKE) -C apps/onegin
	$(MAKE) -C apps/stack_test
	$(MAKE) -C apps/microcpu

docs:
	doxygen doxygen_config

clean: 
	$(MAKE) -C src clean
	$(MAKE) -C apps/tests clean
	$(MAKE) -C apps/square_equation clean
	$(MAKE) -C apps/microgrep clean
	$(MAKE) -C apps/onegin clean
	$(MAKE) -C apps/stack_test clean
	$(MAKE) -C apps/microcpu clean
	rm -f square_equation tests microgrep onegin stack_test microcpu microasm microdisasm
