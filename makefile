YFLAGS = -d
OBJS = hoc_remix.o code.o init.o math.o symbol.o

hoc_remix:	$(OBJS)
		cc $(OBJS) -lm -o hoc_remix

hoc_remix.o code.o init.o symbol.o: hoc_remix.h

code.o init.o symbol.o: x.tab.h

x.tab.h: y.tab.h
	-cmp -s x.tab.h y.tab.h || cp y.tab.h x.tab.h
pr: hoc_remix.y hoc_remix.h code.c init.c math.c symbol.c
	@pr $?
	@touch pr

clean:
	-rm -f $(OBJS) [xy].tab.[ch]
