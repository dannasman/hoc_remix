YFLAGS = -d
OBJS = numera.o code.o lex.o init.o math.o symbol.o

numera:	$(OBJS)
		cc $(OBJS) -lm -ll -o numera

numera.o code.o init.o symbol.o lex.o: numera.h

code.o lex.o init.o symbol.o: x.tab.h

x.tab.h: y.tab.h
	-cmp -s x.tab.h y.tab.h || cp y.tab.h x.tab.h
pr: numera.y numera.h code.c init.c math.c symbol.c
	@pr $?
	@touch pr

clean:
	-rm -f $(OBJS) [xy].tab.[ch]
