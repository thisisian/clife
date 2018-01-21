#include "header.h"

struct map *pbuffmap = NULL;
struct map *pmainmap = NULL;

int main(void)
{
    int width = 80;
    int height = 50;

    char s[MAXSTR];
    FILE *mapfile = fopen("./examplemap", "r");
    initmap(mapfile, width, height, &pmainmap);
    initmap(NULL, pmainmap->width, pmainmap->height, &pbuffmap);

    probecell(pmainmap, 0, 0);
    probecell(pmainmap, 1, 1);
    probecell(pmainmap, 78, 48);
    probecell(pmainmap, 79, 49);
    probecell(pmainmap, 80, 50);
    probecell(pmainmap, 81, 51);

    for (;;) {
        //printsums(pmainmap);
        printmap(pmainmap);
        printf("<ENTER> to continue\n");
        getinput(s, MAXSTR);
        step_map(&pmainmap);
    }
    return 0;
}

/*
 * Load a map into mapoutptr. Returns 1 on success. 
 * If input is NULL, map dimensions will be w and h, otherwise w and h 
 * are ignored
 */
int initmap(FILE *input, int w, int h, struct map **mapoutptr)
{
    int i;
    char c;

    struct cell *cells = NULL;
    
    *mapoutptr = malloc(sizeof (struct map));
    if (mapoutptr == NULL) {
        fprintf(stderr, "intimap: Failed to malloc map");
        exit(1);
    }
    if (w < 1 || h < 1) {
        fprintf(stderr, "initmap: Invalid width or height");
        exit(1);
    }
    cells = malloc(sizeof (struct cell) * w * h);
    if (cells == NULL) {
        fprintf(stderr, "initmap: Failed to malloc cells");
        exit(1);
    }

    for (i = 0; i < (w * h); ++i) {
        if (input == NULL) {
            cells[i].data = 0;
        } else {
            while ((c = fgetc(input)) == '\n')
                ;
            cells[i].data = c - '0';
        } 
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
    if (x >= w || y >= h) {
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
