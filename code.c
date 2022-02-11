#include "numera.h"
#include "y.tab.h"

#define NSTACK 256
//static Datum stack[NSTACK];
//static Datum *stackp;

struct Stack    {
    Datum d;
    struct Stack *next;
};
struct Stack *stack;

struct Prog {
    Inst f;
    struct Prog *next;
    struct Prog *prev;
};
struct Prog *prog;
struct Prog *pc;

initcode()
{
	prog = 0;
    stack = 0;
}

push(d)
	Datum d;
{
	struct Stack *nptr = malloc(sizeof(struct Stack));
    nptr->d = d;
    nptr->next = stack;
    stack = nptr;
}

Datum pop()
{
	if(stack == 0)
		execerror("empty stack", (char *) 0);
    struct Stack *temp;
    temp = stack;
    Datum d = temp->d;
    stack = stack->next;
    free(temp);
	return d;
}

Inst *code(f)
	Inst f;
{
    struct Prog *nptr = malloc(sizeof(struct Prog));
    nptr->f = f;
    nptr->prev = prog;
    nptr->next = 0;
    Inst *oprogp = 0;
    if(prog != 0)   {
        prog->next = nptr;
        oprogp = prog->f;
    }
    prog = nptr;
	return oprogp;
}

execute(p)
	Inst *p;
{
    for(pc = prog; pc->prev != 0; pc = pc->prev)
        ;
    while(pc->next != 0)    {
        (*(pc->f))();
        pc = pc->next;
    }
	/*for(pc = prog; pc->next != 0;)
		(*(*pc++))();*/
}

constpush()
{
	Datum d;
    pc = pc->next;
	d.val = ((Symbol *)pc->f)->u.val;
	push(d);
}

varpush()
{
    Datum d;
    pc = pc->next;
    d.sym = (Symbol *)(pc->f);
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
    pc = pc->next;
    int narg = pc->f;
    switch(narg)  {
        case 0:
            pc = pc->next;
            d1.val = (*(double (*)())(pc->f))();
            break;
        case 1:
            d1 = pop();
            pc = pc->next;
            d1.val = (*(double (*)())(pc->f))(d1.val);
            break;
        case 2:
            d2 = pop();
            d1 = pop();
            pc = pc->next;
            d1.val = (*(double (*)())(pc->f))(d1.val, d2.val);
            break;
    }
    push(d1);
}
