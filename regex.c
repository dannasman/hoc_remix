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

int matchhere(char *regexp, char *text)
{
	char *t, *r;
	for(t=text, r=regexp; (r[0]=='.' || r[0]==*t || r[0]=='\0' || r[0]=='['); t++, r++)	{
		if(r[0] == '\0')
			return 1;
		if(r[0] == '[')
			return matchsquarebr(r, t);
		if(r[1] == '*')
			return matchstar(r[0], r + 2, t);
		if(r[1] == '+')
			return matchplus(r[0], r + 2, t);
		if(r[1] == '?')
			return matchqmark(r[0], r + 2, t);
		if(r[0] == '$' && r[1] == '\0')
			return *t == '\0';
		if(*t == '\0')
			return 0;
	}
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
	char *identifier = ":identifier:"; //custom for lexer.c

	int match_word = 1;
	int match_number = 1;
	int match_identifier = 1;

	char *r;
	int i;
	for(r = regexp+1, i=0; r[0] != ']' && r[0] != '\0'; r++, i++)	{
		if(word[i] != r[0]) match_word = 0;
		if(number[i] != r[0]) match_number = 0;
		if(identifier[i] != r[0]) match_identifier = 0;
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
	if(match_identifier)	{
		int lettercount = 0;
		for(t = text; *t != '\0' && iswordchar(*t); t++, matches++)
			if(islower(*t) || isupper(*t)) lettercount++;
		if(matches > 0 && lettercount > 0) return matchhere(r + 1, t);
	}
	if(regexp[0]==*text) return matchhere(regexp + 1, text + 1);

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
