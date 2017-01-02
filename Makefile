sources = main.c sm.c sm_tlc.c sm_tlc_actions.c sm_car.c sm_tlc2.c

all: $(sources) Makefile
	gcc --std=c99 -Wall $(sources) -o main

clean:
	@rm -rf main

PHONY: clean
