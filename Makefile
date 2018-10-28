sources = main.c fsm.c fsm_tlc.c fsm_tlc_actions.c

main: $(sources)
	gcc --std=c99 -Wall $(sources) -o $@

code:
	gsl -q -script:fsmgen.gsl fsm_tlc.xml

clean:
	@rm -rf main

PHONY: clean code
