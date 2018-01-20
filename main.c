#include "header.h"

struct map *pbuffmap = NULL;
struct map *pmainmap = NULL;

int main(void)
{
    int width = 40;
    int height = 20;

    char s[MAXSTR];
    FILE *mapfile = fopen("./examplemap", "r");
    initmap(mapfile, width, height, *pmainmap);
    initmap(NULL, width, height, *pbuffmap);

    for (;;) {
        printmap(pmainmap);
        printf("<ENTER> to continue\n");
        getinput(s, MAXSTR);
        step_map(pmainmap);
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

/* 
 * Accepts 2D array index, direction index (CCW starting north), 
 * array index and height and outputs index of neighbor 
 *
 * Returns -1 if result is outside array
 */
int find_neighbor(int xpos, int ypos, int i, int w, int h)
{
    int north = n + w;
    int south = n - w;
    int xpos = getcol(n, w, h);
    int ypos = getrow(n, w, h);
    int result = -1;

    switch(i) {
        case 1:
            if (ypos < h - 1)
                result = north;
            break;
        case 2:
            if (ypos < h - 1 && xpos < w - 1)
                result = north + 1;
            break;
        case 3:
            if (xpos < w - 1)
                result = n + 1;
            break;
        case 4:
            if (ypos > 0 && xpos < w - 1)
                result = south + 1; 
            break;
        case 5: 
            if (ypos > 0) 
                result = south; 
            break;
        case 6:
            if (ypos > 0 && xpos > 0)
                result = south - 1;
            break;
        case 7:
            if (xpos > 0)
                result = n - 1;
            break;
        case 8:
            if (ypos < h - 1 && xpos > 0)
                result = north - 1;
            break;
        default:
            fprintf(stderr, "find_neighbor: case fell through");
            exit(1);
    }
    return result;
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
