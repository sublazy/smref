sources = main.c fsm.c fsm_tlc.c fsm_tlc_actions.c

all: $(sources) Makefile
	gcc --std=c99 -Wall $(sources) -o main

code:
	gsl -q -script:fsmgen.gsl fsm_tlc.xml

clean:
	@rm -rf main

PHONY: clean code
