#include <stdio.h>
#include <stdlib.h>

#include "header.h"
struct map {
    int width;
    int height;
    struct cell *cell_array;
};

struct cell {
    int data;
    int n_steps; /* number of times cell has been turned on */
};


/*
 * Returns a map loaded from input. 
 * If input is null, returns zeroed map.
 */
struct map initmap(FILE *input, int w, int h)
{
    int i;

    if (w < 1 || h < 1) {
        fprintf(stderr, "initmap: Negative width or height");
        exit(1);
    }
    struct cell *cells = malloc(sizeof (struct cell) * w * h);
    if (cells == NULL) {
        fprintf(stderr, "initmap: Failed to malloc map");
        exit(1);
    }
    for (i = 0; i < (w * h); ++i) {
        if (input != NULL)
            cells[i].data = fgetc(input);
        else
            cells[i].data = 0;
        cells[i].data = cells[i].n_steps = 0;
    }
    struct map newmap = { w, h, cells };
    return newmap;
}

/* Print map */
void printmap(struct map input)
{
    int i;

    for (i = 0; i < input.width * input.height; ++i) {
        int data = input.cell_array[i].data;
        printf("%d", data);
        if (i % input.width == 0 && i != 1)
            printf("\n");
    }
}

void cell_on(struct map *m, int i)
{
    m->cell_array[i].data = 1;
}

void cell_off(struct map *m, int i)
{
    m->cell_array[i].data = 0;
}

void cell_toggle(struct map *m, int i)
{
    int *data = &m->cell_array[i].data;
    *data = (*data == 1) ? 0 : 1;
}

/* Takes map structure and array index, gives the sum of all Moore neighbors */
int sum_neighbors(struct map m, int n)
{
    int i;
    int temp;
    int sum = 0;

    for (i = 1; i <= 8; ++i) {
        temp = find_neighbor(n, i, m.width, m.height);
        sum += m.cell_array[temp].data;
    }
    return sum;
}

int getcol(int n, int w, int h) 
{
    return n % w + 1;
}

int getrow(int n, int w, int h) 
{
    return n / h + 1;
}

/* Find neighbor of n (Numbers increment CW starting beginning at 12) */
int find_neighbor(int n, int i, int w, int h)
{
    int north = n + (w + 1);
    int south = n + (w - 1);
    int x = getcol(n, w, h);
    int y = getrow(n, w, h);

    switch(i) {
        case 1:
            if (y < h)
                return north;
        case 2:
            if (y < h && x < w)
                return north + 1;
        case 3:
            if (x < w)
                return n + 1;
        case 4:
            if (y > 1 && x < w)
                return south + 1;
        case 5:
            if (y > 1)
                return south;
        case 6:
            if (y > 1 && x > 1)
                return south - 1;
        case 7:
            if (x > 1)
                return n - 1;
        case 8:
            if (y < h && x > 1)
                return north - 1;
        default:
            return -1;
    }
}

/* Evaluates a cell and returns a value (for next step) */
int evaluate(struct map m, int i)
{
    struct cell c = m.cell_array[i];
    int sum = sum_neighbors(m, i);
    if (c.data == 1 && (sum < 2 || sum > 3)) {
        return 0;
    } else if (sum == 3) {
        return 1;
    } else {
        fprintf(stderr, "evaluate: failed to find valid evaluation");
        exit(1);
    }
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

/* Returns a map one step forward from input map */
struct map step_map(struct map in)
{
    int i;
    struct map out;
    for (i = 0; i < input.width * input.height; ++i) {
        struct cell cell_in = intput.cell_array[i]
        if (out.cell_array[i] = evaluate(cell_in))
            struct cell cell_out;
    }
    return out;
}

int main(void)
{
    return 0;
}
