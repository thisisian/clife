#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int main(void)
{
    char s[MAXSTR];
    FILE *map = fopen("./examplemap", "r");
    struct map *main_map_ptr = NULL;
    struct map *temp_map_ptr = NULL;
    initmap(map, 14, 11, &main_map_ptr);
    initmap(NULL, 14, 11, &temp_map_ptr);

    /* test findneighbor */


    for (;;) {
        printmap(*main_map_ptr);
        step_map(*main_map_ptr, temp_map_ptr);
        main_map_ptr = temp_map_ptr;
        printf("<ENTER> to continue\n");
        getinput(s, MAXSTR);
    }
    return 0;
}

/*
 * Load a map into mpaoutptr. Returns 1 on success. 
 */
int initmap(FILE *input, int w, int h, struct map **mapoutptr)
{
    int i;
    char c;

    *mapoutptr = malloc(sizeof (struct map));
    if (mapoutptr == NULL) {
        fprintf(stderr, "intimap: Failed to malloc map");
        exit(1);
    }
    if (w < 1 || h < 1) {
        fprintf(stderr, "initmap: Negative width or height");
        exit(1);
    }
    struct cell *cells = malloc(sizeof (struct cell) * w * h);
    if (cells == NULL) {
        fprintf(stderr, "initmap: Failed to malloc cells");
        exit(1);
    }
    for (i = 0; i < (w * h); ++i) {
        if (input != NULL) {
            while ((c = fgetc(input)) == '\n')
                    ;
            cells[i].data = c - '0';
        }
        else
            cells[i].data = 0;
        cells[i].n_steps = 0;
    }
    (*mapoutptr)->width = w;
    (*mapoutptr)->height = h;
    (*mapoutptr)->cell_array = cells;
    return 1;
}

/* Print map */
void printmap(struct map input)
{
    int i;
    int w = input.width;

    for (i = 0; i < input.width * input.height; ++i) {
        int data = input.cell_array[i].data;
        printf("%d", data);
        if ((i + 1) % w == 0 && i != 1)
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
    int north = n + w;
    int south = n - w;
    int xpos = getcol(n, w, h);
    int ypos = getrow(n, w, h);

    switch(i) {
        case 1:
            if (ypos < h)
                return north;
            return 0;
        case 2:
            if (ypos < h && xpos < w)
                return north + 1;
            return 0;
        case 3:
            if (xpos < w)
                return n + 1;
            return 0;
        case 4:
            if (ypos > 1 && xpos < w)
                return south + 1; 
            return 0;
        case 5: 
            if (ypos > 1) 
                return south; 
            return 0;
        case 6:
            if (ypos > 1 && xpos > 1)
                return south - 1;
                return 0;
        case 7:
            if (xpos > 1)
                return n - 1;
            return 0;
        case 8:
            if (ypos < h && xpos > 1)
                return north - 1;
            return 0;
        default:
            return -1;
    }
}

/* Evaluates a cell and returns a value (for next step) */
int evaluate(struct map m, int i)
{
    struct cell c = m.cell_array[i];
    int sum = sum_neighbors(m, i);

    if ((c.data == 1) && (sum < 2 || sum > 3)) {
        return 0;
    } else if (sum == 3) {
        return 1; 
    } else {
        return 0;
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

/* Takes 2D array index, writes x and y coords to xptr and yptr */
void array_to_coords(int i, int *xptr, int *yptr)
{
}

/* Saves map_in one step forward into map_out */
int step_map(struct map map_in, struct map *map_outptr)
{
    int i;
    int data_in;

    for (i = 0; i < map_in.width * map_in.height; ++i) {
        data_in = evaluate(map_in, i);
        map_outptr->cell_array[i].data = data_in;
    }
    return 0;
}


/* Gets maximum of lim from input into s, dumps stdin */
void getinput(char s[], int lim) 
{
    fgets(s, lim, stdin);
    int len = strlen(s);
    if (len == 0)
        s[0] = '\0';
    else if (len > 0 && s[len - 1] == '\n')
        s[len - 1] = '\0';
    fflush(stdin);
}

