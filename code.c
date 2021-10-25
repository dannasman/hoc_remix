#include "numera.h"
#include "y.tab.h"

#define NSTACK 256
static Datum stack[NSTACK];
static Datum *stackp;

#define NPROG 2000
Inst prog[NPROG];
Inst *progp;
Inst *pc;

initcode()
{
	stackp = stack;
	progp = prog;
}

push(d)
	Datum d;
{
	if(stackp >= &stack[NSTACK])
		execerror("stack overflow", (char *) 0);
	*stackp++ = d;
}

Datum pop()
{
	if(stackp <= stack)
		execerror("stack underflow", (char *) 0);
	return *--stackp;
}

Inst *code(f)
	Inst f;
{
	Inst *oprogp = progp;
	if(progp >= &prog[NPROG])
		execerror("program too big", (char*) 0);
	*progp++ = f;
	return oprogp;
}

execute(p)
	Inst *p;
{
	for(pc = p; *pc != STOP;)
		(*(*pc++))();
}

constpush()
{
	Datum d;
	d.val = ((Symbol *)*pc++)->u.val;
	push(d);
}

varpush()
{
    Datum d;
    d.sym = (Symbol *)(*pc++);
    push(d);
}

add()
{
    Datum d1, d2;
    d2 = pop();
    d1 = pop();
    d1.val += d2.val;
    push(d1);
}

sub()
{
    Datum d1, d2;
    d2 = pop();
    d1 = pop();
    d1.val -= d2.val;
    push(d1);
}

mul()
{
    Datum d1, d2;
    d2 = pop();
    d1 = pop();
    d1.val *= d2.val;
    push(d1);
}

division()
{
    Datum d1, d2;
    d2 = pop();
    if(d2.val == 0.0)
        execerror("division by zero", (char *)0);
    d1 = pop();
    d1.val /= d2.val;
    push(d1);
}

negate()
{
    Datum d;
    d = pop();
    d.val = -d.val;
    push(d);
}

fmodulus()
{
    Datum d1, d2;
    extern double Fmod();
    d2 = pop();
    if(d2.val == 0.0)
        execerror("undefined behavior", (char *)0);
    d1 = pop();
    d1.val = Fmod(d1.val, d2.val);
    push(d1);
}

power()
{
    Datum d1, d2;
    extern double Pow();
    d2 = pop();
    d1 = pop();
    d1.val = Pow(d1.val, d2.val);
    push(d1);
}

eval()
{
    Datum d;
    d = pop();
    if(d.sym->type == UNDEF)
        execerror("undefined variable", d.sym->name);
    d.val = d.sym->u.val;
    push(d);
}

assign()
{
    Datum d1, d2;
    d1 = pop();
    d2 = pop();
    if(d1.sym->type != VAR && d1.sym->type != UNDEF)
        execerror("assignment to non-variable", d1.sym->name);
    d1.sym->u.val = d2.val;
    d1.sym->type = VAR;
    push(d2);
}

print()
{
    Datum d;
    d = pop();
    printf("\t%.8g\n", d.val);
}

bltin()
{
    Datum d1, d2;
    int narg = *pc++;
    switch(narg)  {
        case 0:
            d1.val = (*(double (*)())(*pc++))();
            break;
        case 1:
            d1 = pop();
            d1.val = (*(double (*)())(*pc++))(d1.val);
            break;
        case 2:
            d2 = pop();
            d1 = pop();
            d1.val = (*(double (*)())(*pc++))(d1.val, d2.val);
            break;
    }
    push(d1);
}
