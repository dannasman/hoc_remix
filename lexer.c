#include <stdio.h>
#include <regex.h>

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
	
	regex_t regex;
	int return_value;

	while(fscanf(source, "%s", word)==1)	{
		return_value = regcomp(&regex, "{", 0);
		if(regexec(&regex, word, 0, NULL, 0)==0)	{
			tokens[i] = OBRAC;
			i++;
		}

		return_value = regcomp(&regex, "}", 0);
		if(regexec(&regex, word, 0, NULL, 0)==0)	{
			tokens[i] = CBRAC;
			i++;
		}

		return_value = regcomp(&regex, "\(", 0);
		if(regexec(&regex, word, 0, NULL, 0)==0)	{
			tokens[i] = OPARENT;
			i++;
		}

		return_value = regcomp(&regex, "\)", 0);
		if(regexec(&regex, word, 0, NULL, 0)==0)	{
			tokens[i] = CPARENT;
			i++;
		}

		return_value = regcomp(&regex, ";", 0);
		if(regexec(&regex, word, 0, NULL, 0)==0)	{
			tokens[i] = SEMICOL;
			i++;
		}

		return_value = regcomp(&regex, "int", 0);
		if(regexec(&regex, word, 0, NULL, 0)==0)	{
			tokens[i] = INT_KEY;
			i++;
		}
		
		return_value = regcomp(&regex, "return", 0);
		if(regexec(&regex, word, 0, NULL, 0)==0)	{
			tokens[i] = RET_KEY;
			i++;
		}

		return_value = regcomp(&regex, "[a-zA-Z]\\w", 0);
		if(regexec(&regex, word, 0, NULL, 0)==0)	{
			tokens[i] = ID;
			i++;
		}
	}
	fclose(source);
	return tokens;
}

main()
{
	int *tokens = lex("test_source.c");
	for(int i=0;i<10;i++)
		printf("%d ", tokens[i]);
}
