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
    int n_steps; /* number of times cell has been turned on */
};

/* Function Prototypes */
void getinput(char *s, int lim);
void printmap(struct map input);
void cell_on(struct map *m, int i);
void cell_off(struct map *m, int i);
void cell_toggle(struct map *m, int i);
int sum_neighbors(struct map m, int n);
int evaluate(struct map m, int i);
int step_map(struct map map_in, struct map *map_outptr);
int initmap(FILE *input, int w, int h, struct map **mapoutptr);
int coord_to_array(int x, int y, int w, int h);
int find_neighbor(int n, int i, int w, int h);

