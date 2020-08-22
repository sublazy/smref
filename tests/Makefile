testrunner: wvtest/c/wvtest.c wvtest/c/wvtestmain.c fsm.t.c ../fsm.c
	gcc -D WVTEST_CONFIGURED -DDENSE -o $@ -I. -I wvtest/c $^

runtests: testrunner
	@./testrunner

test: testrunner
	@./wvtest/wvtestrun $(MAKE) runtests

clean::
	rm -f *.o testrunner

