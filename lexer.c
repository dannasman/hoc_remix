#include <stdio.h>
#include "regex.h"

//Hifistelyidea: tee oma regex-toteutus

enum token	{
	INT_KEY, INT_LIT, OBRAC, CBRAC, OPARENT, CPARENT, SEMICOL, RET_KEY, ID
};

int * lex(char *filename)
{
	FILE *source;
	source = fopen(filename, "r");
	char word[256];

	static int tokens[256];
	int i = 0;

	while(fscanf(source, "%s", word)==1)	{
		if(match("{", word))	{
			tokens[i] = OBRAC;
			i++;
		}

		if(match("}", word))	{
			tokens[i] = CBRAC;
			i++;
		}

		if(match("\(", word))	{
			tokens[i] = OPARENT;
			i++;
		}

		if(match("\)", word))	{
			tokens[i] = CPARENT;
			i++;
		}

		if(match(";", word))	{
			tokens[i] = SEMICOL;
			i++;
		}

		if(match("int", word))	{
			tokens[i] = INT_KEY;
			i++;
		}
		
		if(match("return", word))	{
			tokens[i] = RET_KEY;
			i++;
		}

		if(match("[:word:]", word))	{
			tokens[i] = ID;
			i++;
		}

		if(match("[:number:]", word))	{
			tokens[i] = INT_LIT;
			i++;
		}
	}
	fclose(source);
	return tokens;
}

main()
{
	int *tokens = lex("test_source.c");
	for(int i=0;i<15;i++)
		printf("%d ", tokens[i]);
}
