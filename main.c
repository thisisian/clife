#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int main(void)
{
    int width = 40;
    int height = 20;


    char s[MAXSTR];
    FILE *mapfile = fopen("./examplemap", "r");
    struct map *main_map_ptr = NULL;
    struct map *temp_map_ptr = NULL;
    initmap(mapfile, width, height, &main_map_ptr);
    initmap(NULL, width, height, &temp_map_ptr);

    test_coordsys(10, 10, 40, 40);
    test_coordsys(20, 30, 40, 40);
    test_coordsys(15, 15, 40, 40);
    test_coordsys(15, 15, 40, 40);
        getinput(s, MAXSTR);

    for (;;) {
        printsums(main_map_ptr);
        printmap(main_map_ptr);
        printf("<ENTER> to continue\n");
        getinput(s, MAXSTR);
        step_map(&main_map_ptr, &temp_map_ptr);
        
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
void printmap(struct map *mapptr)
{
    int i;
    int w = mapptr->width;

    for (i = 0; i < mapptr->width * mapptr->height; ++i) {
        if (mapptr->cell_array[i].data)
            printf("*");
        else
            printf(" ");
        if ((i + 1) % w == 0 && i != 1)
            printf("\n");
    }
}

int getcol(int n, int w, int h) 
{
    return n % w;
}

int getrow(int n, int w, int h) 
{
    return n / h;
}

/* Takes 2d coordinates and dimentions and outputs appropriate array index */
int coord_to_array(int x, int y, int w, int h)
{
    if (x > w || y > h) {
        fprintf(stderr, "coord_to_array: Given coordinate outside range");
        exit(1);
    }
    return x + y * w;
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
