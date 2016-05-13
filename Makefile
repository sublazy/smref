all: *.c Makefile
	@gcc --std=c99 -Wall $< -o main

clean:
	@rm -rf main

PHONY: clean
