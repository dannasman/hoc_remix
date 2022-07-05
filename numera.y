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
%token <sym> NUMBER VAR BLTIN UNDEF WHILE IF ELSE
%type <inst> stmt asgn expr stmtlist cond while if end
%type <narg> arglist
%right '='
%left OR
%left AND
%left GT GE LT LE EQ NE
%left '+' '-'
%left '*' '/' '%'
%left UNARYMINUS NOT
%right '^'
%%
list:
		| list '\n'
        | list stmt '\n'    {   code(STOP); return 1;   }
		| list asgn '\n'	{	code2(pop, STOP); return 1;	}
		| list expr '\n'	{	code2(print, STOP); return 1;	}
		| list error '\n'	{	yyerrok;	}
		;
asgn:	VAR '=' expr	{	code3(varpush, (Inst)$1, assign);	}
		;
stmt:       expr    {   code(pop);  }
        |   while cond stmt end
        |   if cond stmt end    {   code(STOP);   }
        |   if cond stmt end ELSE stmt end
        |   '{' stmtlist '}'    {   $$ = $2;    }
        ;
cond:   '(' expr ')'    {   code(STOP); $$ = $2;    }
        ;
while:  WHILE   {   $$ = code(whilecode);    }
        ;
if:     IF      {   $$ = code(ifcode);  }
        ;
end:        {   code(STOP); }
        ;
stmtlist:   {   $$ = prog;   }
        |   stmtlist '\n'
        |   stmtlist stmt
        ;
expr:		NUMBER	{	code2(constpush, (Inst)$1);	}
		|	VAR	{	code3(varpush, (Inst)$1, eval);	}
		|	asgn
		|	BLTIN '(' arglist ')'	{	code3(bltin, (Inst)$3, (Inst)$1->u.ptr);	}
		|	'(' expr ')'    {   $$ = $2;    }
		|	expr '+' expr	{	code(add);	}
		|	expr '-' expr	{	code(sub);	}
		|	expr '*' expr	{	code(mul);	}
		|	expr '/' expr	{	code(division);	}
		|	expr '%' expr	{	code(fmod);	}
		|	expr '^' expr	{	code(power);	}
		|	'-' expr %prec UNARYMINUS	{	code(negate);	}
        |   expr GT expr    {   code(gt);   }
        |   expr GE expr    {   code(ge);   }
        |   expr LT expr    {   code(lt);   }
        |   expr LE expr    {   code(le);   }
        |   expr EQ expr    {   code(eq);   }
        |   expr NE expr    {   code(ne);   }
        |   expr AND expr   {   code(and);  }
        |   expr OR expr    {   code(or);   }
        |   NOT expr        {   $$ = $2; code(not); }
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
		execute(sprog);
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

yylex()
{
	int c;

	while((c=getchar()) == ' ' || c == '\t')
		;
	if(c == EOF)
		return 0;
	if(c == '.' || isdigit(c))	{
        double d;
        ungetc(c, stdin);
        scanf("%lf", &d);
        yylval.sym = install("", NUMBER, d);
		return NUMBER;
	}
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
    switch(c)   {
    case '>':   return follow('=', GE, GT);
    case '<':   return follow('=', LE, LT);
    case '=':   return follow('=', EQ, '=');
    case '!':   return follow('=', NE, NOT);
    case '|':   return follow('|', OR, '|');
    case '&':   return follow('&', AND, '&');
    case '\n':  lineno++; return '\n';
    default:    return c;
    }
	return c;
}

follow(expect, ifyes, ifno)
{
    int c = getchar();

    if(c == expect)
        return  ifyes;
    ungetc(c, stdin);
    return ifno;
}

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


