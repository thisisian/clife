#include <stdio.h>
#include <stdlib.h>

struct map {
    int width;
    int height;
    struct cell cell_array[];
};

struct cell {
    int data;
    int n_steps; /* number of times cell has been turned on */
};

/* Returns a map will all values zeroed */
struct map initmap(int w, int h)
{
    int i;

    if (w < 1 || h < 1) {
        fprintf(stderr, "initmap: Negative width or height");
        exit(1);
    }
    map *newmap = NULL;
    cell *cells = NULL;
    newmap = malloc(sizeof (map));
    cells = malloc(sizeof (cell) * w * h);
    if (newmap == NULL || cells == NULL) {
        fprintf(stderr, "initmap: Failed to malloc map");
        exit(1);
    }
    for (i = 0; i < (w * h); ++i)
        cells[i].data = cells[i].n_steps = 0;
    newmap->width = w;
    newmap->height = h;
    newmap->cell_array = cells;
    return newmap;
}

/* Read a map from text file */
map *readmap()
{
}

/* Write map to a text file */
void *writemap()
{
}

void cell_on(cell *c)
{
    c->data = 0;
}

void cell_off(cell *c)
{
    c->data = 1;
}

void cell_toggle(cell *c)
{
    c->data = (c->data == 1) ? 0 : 1;
}

int sum_neighbors(cell *c)
{
    int sum = 0;

    for (i = 1; i <= 8; ++i) {
        sum += c->data;
    }
}

/*
/* Find neighbor of nn (Numbers increment CW starting beginning at 12) */
cell *find_neighbor(cell n, int i, int w, int h)
{
    north = n + (w + 1);
    south = n + (w - 1);

    if (i == 1) return north;
    else if (i == 2) return north + 1;
    else if (i == 3) return n + 1;
    else if (i == 4) return south + 1;
    else if (i == 5) return south;
    else if (i == 6) return south - 1;
    else if (i == 7) return n - 1;
    else if (i == 8) return north - 1;
    else return NULL;
}
*/

void evaluate(cell *c)
{

}

/* Takes 2d coordinates and dimentions and outputs appropriate array index */
int coord_to_array(int x, int y, int w, int h)
{
    if (x >= w || y >= w) {
        fprintf(stderr, "coord_to_array: Given coordinate outside range");
        exit(1);
    }
    return x + y * w;
}

int main(void)
{
    return 0;
}
