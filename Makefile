sources = main.c fsm.c fsm_tlc.c fsm_tlc_actions.c
diagram_files = fsm_tlc.png

main: $(sources)
	gcc --std=c99 -Wall $(sources) -o $@

fsm_%.c fsm_%.h fsm_%_actions_skel.c: fsm_%.xml fsmgen.gsl
	gsl -q -script:fsmgen.gsl $<

fsm_%.txt: fsm_%.xml fsm_diagram.gsl
	gsl -q -script:fsm_diagram.gsl $< > $@

fsm_%.png: fsm_%.txt
	plantuml $<

# Phony targets
# ------------------------------------------------------------------------------
PHONY: clean code diagrams

code:
	gsl -q -script:fsmgen.gsl fsm_tlc.xml

diagrams: $(diagram_files)

clean:
	@rm -rf main

