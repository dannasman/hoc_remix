typedef struct Symbol	{
	char *name;
	short type;
	union	{
		double val;
		double (*ptr)();
	} u;
	struct Symbol *next;
} Symbol;
Symbol *install(), *lookup();

typedef union Datum	{
	double val;
	Symbol *sym;
} Datum;
extern Datum pop();

typedef int (*Inst)();
#define STOP (Inst) 0

//extern Inst prog[];
extern struct Prog *prog, *sprog;
extern eval(), add(), sub(), mul(), division(), negate(), fmodulus(), power();
extern assign(), bltin(), varpush(), constpush(), print();
extern gt(), lt(), eq(), ge(), le(), ne(), and(), or(), not();
extern whilecode();
