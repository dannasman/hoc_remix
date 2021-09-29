YFLAGS = -d
OBJS = numera.o lex.o init.o math.o symbol.o

numera:	$(OBJS)
		cc $(OBJS) -lm -ll -o numera

numera.o:	numera.h

lex.o init.o symbol.o: numera.h y.tab.h

pr:
	@pr numera.y numera.h init.c math.c symbol.c makefile

clean:
	-rm -f $(OBJS) y.tab.[ch]
