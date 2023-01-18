#include "hoc_remix.h"
#include "y.tab.h"
#include <math.h>

extern double Log(), Log10(), Exp(), Sqrt(), integer(), Rand(), Fmod();
static struct	{
	char *name;
	double cval;
} consts[] = {
	"PI", 3.14159265358979323846,
	"E", 2.71828182845904523536,
	"GAMMA", 0.57721566490153286060,
	"DEG", 57.29577951308232087680,
	"PHI", 1.61803398874989484820,
	0, 0
};
static struct	{
	char *name;
	double (*func)();
} builtins[] = {
	"sin", sin,
	"cos", cos,
	"atan", atan,
	"atan2", atan2,
	"log", Log,
	"log10", Log10,
	"exp", Exp,
	"sqrt", Sqrt,
	"int", integer,
	"abs", fabs,
	"rand", Rand,
    "fmod", Fmod,
	0, 0
};
static struct   {
    char *name;
    int kval;
} keywords[] = {
    "while", WHILE,
    "if", IF,
    "else", ELSE,
     0, 0,
};
init()
{
	int i;
	Symbol *s;

	for(i = 0; consts[i].name; i++)
		install(consts[i].name, VAR, consts[i].cval);
	for(i = 0; builtins[i].name; i++)	{
		s = install(builtins[i].name, BLTIN, 0.0);
		s->u.ptr = builtins[i].func;
	}
    for(i = 0; keywords[i].name; i++)
        install(keywords[i].name, keywords[i].kval, 0.0);
}
