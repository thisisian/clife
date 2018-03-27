#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAXSTR 1000

#define _POSIX_SOURCE 1

/* Typedefs */
struct map {
    int width;
    int height;
    struct cell *cell_array;
    struct cell *buffer;
};

struct cell {
    int data;
};

/* Function Prototypes */
/* main.c */
void getinput(char *s, int lim);
void printmap();
int initmap(FILE *input, int w, int h);

/* evalmap.c */
int step_map();
int evaluate(int i, int j, char rulearr[2][9]);
int summoore(int i, int j);
int find_neighbor(int n, int i);
