#include "numera.h"
#include "y.tab.h"

static Symbol *symlist = 0;

Symbol *lookup(s)
	char *s;
{
	Symbol *sp;

	for(sp = symlist; sp != (Symbol *) 0; sp = sp->next)
		if(strcmp(sp->name, s) == 0)
			return sp;
	return 0;
}
Symbol *install(s, t, d)
	char* s;
	int t;
	double d;
{
	Symbol *sp;
	char *emalloc();

	sp = (Symbol *) emalloc(sizeof(Symbol));
	sp->name = emalloc(strlen(s)+1);
	strcpy(sp->name, s);
	sp->type = t;
	sp->u.val = d;
	sp->next = symlist;
	symlist = sp;
	return sp;
}

char *emalloc(n)
	unsigned n;
{
	char *p, *malloc();

	p = malloc(n);
	if(p == 0)
		execerror("out of memory", (char *) 0);
	return p;
}
