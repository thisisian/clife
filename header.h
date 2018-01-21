#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSTR 1000

/* Typedefs */
struct map {
    int width;
    int height;
    struct cell *cell_array;
};

struct cell {
    int data;
};

/* Function Prototypes */
/* main.c */
void getinput(char *s, int lim);
void printmap(struct map *mapptr);
int initmap(FILE *input, int w, int h, struct map **mapoutptr);
int coord_to_array(int x, int y, int w, int h);
int find_neighbor(int n, int i, int w, int h);
int getcol(int n, int w, int h);
int getrow(int n, int w, int h);

/* debug.c */
void test_coordsys(int x, int y,  int width, int height);
void printsums(struct map * mapptr);
void probecell(struct map * mapptr, int x, int y);

/* evalmap.c */
int step_map(struct map **pmap);
int evaluate(struct map *mapptr, int i);
int sum_neighbors(struct map m, int n);
