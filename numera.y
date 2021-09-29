%{
#include<math.h>
#include "numera.h"
extern double Pow();
%}
%union	{
	double val;
	char *args;
	Arg *arg;
	Symbol *sym;
}
%token <val> NUMBER
%token <sym> VAR BLTIN UNDEF
%type <val> expr asgn
%type <arg> arg_list
%right '='
%left '+' '-'
%left '*' '/' '%'
%left UNARYMINUS
%left UNARYPLUS
%right '^'
%%
list:
		| list '\n'
		| list ';'
		| list asgn '\n'
		| list asgn ';'
		| list expr '\n'	{	printf("\t%.8g\n", $2);	}
		| list expr ';'
		| list error '\n'	{	yyerrok;	}
		| list error ';'	{ 	yyerrok;	}
		;
asgn:	VAR '=' expr	{	if(strcmp($1->name, "PI") == 0 || strcmp($1->name, "E") == 0 || strcmp($1->name, "GAMMA") == 0 || strcmp($1->name, "DEG") == 0 || strcmp($1->name, "PHI") == 0)
								execerror("assignment to constant", "");
							$$ = $1->u.val=$3; $1->type = VAR;	}
		;
expr:		NUMBER
		|	VAR	{	if($1->type == UNDEF)
						execerror("undefined variable", $1->name);
					$$ = $1->u.val;	}
		|	asgn
		|	BLTIN '(' arg_list ')'	{	switch($3->pos)	{
											case 1:
												$$ = (*($1->u.ptr))($3->val);
												break;
											case 2:
												$$ = (*($1->u.ptr))($3->val, $3->next->val);
												break;
											default:
												$$ = (*($1->u.ptr))(); 
										}	}
		|	'-' expr %prec UNARYMINUS	{	$$ = -$2;	}
		|	'+' expr %prec UNARYPLUS	{	$$ = $2;	}
		|	expr '+' expr	{	$$ = $1 + $3;	}
		|	expr '-' expr	{	$$ = $1 - $3;	}
		|	expr '*' expr	{	$$ = $1 * $3;	}
		|	expr '/' expr	{	
								if($3 == 0.0)
									execerror("division by zero", "");
								$$ = $1 / $3;	}
		|	expr '%' expr	{	$$ = fmod($1, $3);	}
		|	expr '^' expr	{	$$ = Pow($1, $3);	}
		|	'(' expr ')'	{	$$ = $2;	}
		;
arg_list:
			{	$$ = malloc(sizeof(struct Arg));
				$$->next = 0;
				$$->pos = 0;	}
		|	expr	{	$$ = malloc(sizeof(struct Arg));
						$$->next = 0;
						$$->pos = 1;
						$$->val = $1;	}
		|	expr ',' arg_list	{	$$ = malloc(sizeof(struct Arg));
								 	$$->next = $3;
									$$->pos = $3->pos+1;
									$$->val = $1;	}
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
	yyparse();
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


