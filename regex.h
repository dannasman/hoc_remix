#ifndef REGEX_H_
#define REGEX_H_

int match(char *regexp, char *text);
int matchhere(char *regexp, char *text);
int matchstar(int c, char *regexp, char *text);
int matchplus(int c, char *regexp, char *text);
int matchqmark(int c, char *regexp, char *text);
int matchsquarebr(char *regexp, char *text);

int iswordchar(int c);
int islower(int c);
int isupper(int c);
int isnumber(int c);

#endif
