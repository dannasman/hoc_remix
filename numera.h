typedef struct Symbol	{
	char *name;
	short type;
	union	{
		double val;
		double (*ptr)();
	} u;
	struct Symbol *next;
} Symbol;
typedef struct Arg	{
	double val;
	int pos;
	struct Arg *next;
} Arg;
Symbol *install(), *lookup();
