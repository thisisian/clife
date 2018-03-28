#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAXSTR 1000

#define _POSIX_SOURCE 1

/* Function Prototypes */
/* main.c */
void getInput(char *, int);
void printMap();
int initMap(FILE *, int, int);
void printError(char *);

/* evalmap.c */
int stepMap();
int evaluate(int, int, char [2][9]);
int sumMoore(int, int);
int findNeighbor(int, int);
