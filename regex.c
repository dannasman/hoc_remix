#include <stdio.h>

int match(char *regexp, char *text)
{
	if(regexp[0] == '^')
		return matchhere(regexp+1, text);
	do	{
		if(matchhere(regexp, text))
			return 1;
	} while (*text++ != '\0');
	return 0;
}

//elegantimmaksi jos aikaa
int matchhere(char *regexp, char *text)
{
	if(regexp[0] == '\0')
		return 1;
	if(regexp[0] == '[')
		return matchsquarebr(regexp + 1, text);
	if(regexp[1] == '*')
		return matchstar(regexp[0], regexp + 2, text);
	if(regexp[1] == '+')
		return matchplus(regexp[0], regexp + 2, text);
	if(regexp[1] == '?')
		return matchqmark(regexp[0], regexp + 2, text);
	if(regexp[0] == '$' && regexp[1] == '\0')
		return *text == '\0';
	if(*text != '\0' && (regexp[0] == '.' || regexp[0] == *text))
		return matchhere(regexp+1 ,text+1);
	return 0;
}

int matchstar(int c, char *regexp, char *text)
{
	char *t;

	for(t = text; *t != '\0' && (*t == c || c == '.'); t++)
		;

	do	{
		if(matchhere(regexp, t))
			return 1;
	} while (t-- > text);
	return 0;
}

int matchplus(int c, char *regexp, char *text)
{
	char *t;
	int matches = 0;
	for(t = text; *t != '\0' && *t == c; t++, matches++)
		;
	if(matches > 0) return matchhere(regexp, t);
	return 0;
}

int matchqmark(int c, char *regexp, char *text)
{
	char *t;
	int matches = 0;
	for(t = text; *t != '\0' && *t == c; t++, matches++)
		;
	if(matches < 2) return matchhere(regexp, t);
	return 0;
}

int matchsquarebr(char *regexp, char *text)
{
	char *word = ":word:";
	char *number = ":number:";

	int match_word = 1;
	int match_number = 1;

	char *r;
	int i;

	for(r = regexp, i=0; r[0] != ']'; r++, i++)	{
		if(word[i] != r[0]) match_word = 0;
		if(number[i] != r[0]) match_number = 0;
	}

	int matches = 0;
	char *t;
	if(match_word)	{
		for(t = text; *t != '\0' && iswordchar(*t); t++, matches++)
			;
		if(matches > 0) return matchhere(r + 1, t);
	}
	if(match_number)	{
		for(t = text; *t != '\0' && isnumber(*t); t++, matches++)
			;
		if(matches > 0) return matchhere(r + 1, t);
	}

	return 0;
	
}

int iswordchar(int c)
{
	return (islower(c) || isupper(c) || isnumber(c) || c == '_');
}

int islower(int c)
{
	return (c >= 97 && c <= 122);
}

int isupper(int c)
{
	return (c >= 65 && c <=90);
}

int isnumber(int c)
{
	return (c >= 48 && c <= 57);
}
