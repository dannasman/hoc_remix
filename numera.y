%{
#include<math.h>
#include "numera.h"
#define code2(c1, c2)		code(c1); code(c2)
#define code3(c1, c2, c3)	code(c1); code(c2); code(c3)
extern double Pow();
%}
%union	{
	Symbol *sym;
	Inst *inst;
    int narg;
}
%token <sym> NUMBER VAR BLTIN UNDEF
%type <narg> arglist
%right '='
%left '+' '-'
%left '*' '/' '%'
%left UNARYMINUS
%right '^'
%%
list:
		| list '\n'
		| list asgn '\n'	{	code2(pop, STOP); return 1;	}
		| list expr '\n'	{	code2(print, STOP); return 1;	}
		| list error '\n'	{	yyerrok;	}
		;
asgn:	VAR '=' expr	{	code3(varpush, (Inst)$1, assign);	}
		;
expr:		NUMBER	{	code2(constpush, (Inst)$1);	}
		|	VAR	{	code3(varpush, (Inst)$1, eval);	}
		|	asgn
		|	BLTIN '(' arglist ')'	{	code3(bltin, (Inst)$3, (Inst)$1->u.ptr);	}
		|	'(' expr ')'
		|	expr '+' expr	{	code(add);	}
		|	expr '-' expr	{	code(sub);	}
		|	expr '*' expr	{	code(mul);	}
		|	expr '/' expr	{	code(division);	}
		|	expr '%' expr	{	code(fmod);	}
		|	expr '^' expr	{	code(power);	}
		|	'-' expr %prec UNARYMINUS	{	code(negate);	}
		;
arglist:    {   $$ = 0;    }
        |   expr    {   $$ = 1; }
        |   arglist ',' expr    {   $$ = $1 + 1; }
        ;
%%

#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>
jmp_buf begin;

char *progname;
int lineno = 1;

main(argc, argv)
	char *argv[];
{
	int fpecatch();

	progname = argv[0];
	init();
	setjmp(begin);
	signal(SIGFPE, fpecatch);
	for(initcode(); yyparse(); initcode())
		execute();
	return 0;
}

execerror(s, t)
	char *s, *t;
{
	warning(s, t);
	longjmp(begin, 0);
}

fpecatch()
{
	execerror("floating point exception", (char *) 0);
}

/*yylex()
{
	int c;

	while((c=getchar()) == ' ' || c == '\t')
		;
	if(c == EOF)
		return 0;
	if(c == '.' || isdigit(c))	{
		ungetc(c, stdin);
		scanf("%lf", &yylval.val);
		return NUMBER;
	}
	if(c == '\n')
		lineno++;
	if(isalpha(c))	{
		Symbol *s;
		char sbuf[100], *p = sbuf;
		do	{
			*p++ = c;
		}	while((c=getchar()) != EOF && isalnum(c));
		ungetc(c, stdin);
		*p = '\0';
		if((s=lookup(sbuf)) == 0)
			s = install(sbuf, UNDEF, 0.0);
		yylval.sym = s;
		return s->type == UNDEF ? VAR : s->type;
	}
	return c;
}*/

yyerror(s)
	char *s;
{
	warning(s, (char *) 0);
}

warning(s, t)
	char *s, *t;
{
	fprintf(stderr, "%s: %s", progname, s);
	if(t)
		fprintf(stderr, " %s", t);
	fprintf(stderr, " near line %d\n", lineno);
}


