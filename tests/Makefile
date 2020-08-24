sources = wvtest/c/wvtest.c wvtest/c/wvtestmain.c fsm.t.c

testrunner: wvtest/c/wvtest.c wvtest/c/wvtestmain.c fsm.t.c ../fsm.c
	gcc -g -ggdb -D WVTEST_CONFIGURED -DDENSE -DNDEBUG -o $@ -I. -I wvtest/c $(sources)

runtests: testrunner
	@./testrunner

test: testrunner
	@./wvtest/wvtestrun $(MAKE) runtests

clean::
	rm -f *.o testrunner

