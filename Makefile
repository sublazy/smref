sources = main.c fsm.c fsm_tlc.c fsm_tlc_actions.c
headers = $(wildcard *.h)
diagram_files = fsm_tlc.png

warning_flags = \
-Wall \
-Wextra \
-Wshadow \
-Wpointer-arith \
-Wcast-qual \
-Wcast-align \
-Wstrict-prototypes \
-Wmissing-prototypes \
-Wdouble-promotion \
-Wswitch-default \
-Wold-style-definition \
-Wmisleading-indentation

defines = -DDENSE

main: $(sources) $(headers)
	@gcc --std=c99 $(warning_flags) $(defines) $(sources) -o $@

fsm_%.c fsm_%.h fsm_%_actions_skel.c: fsm_%.xml fsmgen.gsl
	@gsl -q -script:fsmgen.gsl $<

fsm_%.txt: fsm_%.xml fsm_diagram.gsl
	@gsl -q -script:fsm_diagram.gsl $< > $@

fsm_%.png: fsm_%.txt
	@plantuml $<

# Phony targets
# ------------------------------------------------------------------------------
.PHONY: all clean code diagrams

all: main diagrams

code:
	@gsl -q -script:fsmgen.gsl fsm_tlc.xml

diagrams: $(diagram_files)

test:
	@cd tests; $(MAKE) test

runtests:
	@cd tests; $(MAKE) runtests

lint:
	@cppcheck --std=c99 --quiet \
		--enable=warning,style,performance,portability,information \
		--suppress=missingIncludeSystem \
		$(sources) $(headers)

clean:
	@rm -rf main $(diagram_files)

