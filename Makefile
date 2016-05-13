sources = main.c sm.c

all: $(sources) Makefile
	gcc --std=c99 -Wall $(sources) -o main

clean:
	@rm -rf main

PHONY: clean
