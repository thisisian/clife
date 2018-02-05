#include "header.h"
#include <unistd.h>

int getmapdims(FILE *mapfile, int *pw, int *ph);
void printerror(char *s);

struct map *pbuffmap = NULL;
struct map *pmainmap = NULL;

                     /*  0  1  2  3  4  5  6  7  8   */ 
char rulearr[2][9] = { { 0, 0, 0, 1, 0, 0, 0, 0, 0, },     /* Birth */
                       { 0, 0, 1, 1, 0, 0, 0, 0, 0, }, };  /* Surive */
/* Argument flags */
int mflag = 0;
int sflag = 0;
int dflag = 0;
int rflag = 0;

int main(int argc, char *argv[])
{
    char c;
    FILE *mapfile = NULL;
    int mapwidth, mapheight;
    mapwidth = mapheight = 0;

    srand(time(NULL));

    while ((c = getopt(argc, argv, "m:s::d:r:")) != -1)
        switch (c) {
            case 'm':
                mflag = 1;
                if ((mapfile = fopen(optarg, "r")) == NULL)
                    printerror("Failed to open map file\n");
                break;
            case 's':         /* soup */
                printf("Argument 's' with argument %s\n", optarg);
                sflag = 1;
                break;
            case 'd':
                if (sscanf(optarg, "%d:%d", &mapwidth, &mapheight) == -1)
                    printerror("Invalid dimentions");
                dflag = 1;
                break;
            case 'r':
                printf("Argument 'r' with argument %s\n", optarg);
                rflag = 1;
                break;
            case '?':
                printf("Invalid option!\n");
        }

    if (mflag == 1 && sflag == 1)
        printerror("A random soup cannot be generated when provided a map\n");
    if (mflag == 0 && dflag == 0)
        printerror("No dimentions were provided\n");

    initmap(mapfile, mapwidth, mapheight, &pmainmap);
    initmap(NULL, pmainmap->width, pmainmap->height, &pbuffmap);

    for (;;) {
        printmap(pmainmap);
        printf("<ENTER> to continue\n");
        fgetc(stdin);
        fflush(stdin);
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
    int width, height;
    char c;

    struct cell *cells = NULL;

    if (input != NULL) { 
        getmapdims(input, &width, &height);
        rewind(input);
    } else {
        width = w;
        height = h;
    }
    
    *mapoutptr = malloc(sizeof (struct map));
    if (mapoutptr == NULL) {
        fprintf(stderr, "intimap: Failed to malloc map");
        exit(1);
    }
    cells = malloc(sizeof (struct cell) * width * height);
    if (cells == NULL) {
        fprintf(stderr, "initmap: Failed to malloc cells");
        exit(1);
    }

    for (i = 0; i < (width * height); ++i) {
        if (sflag == 1) {
            cells[i].data = rand() % 2;
        } else if (input == NULL) {
            cells[i].data = 0;
        } else {
            while ((c = fgetc(input)) == '\n')
                ;
            cells[i].data = c - '0';
        } 
    }
    printf("%d %d\n", width, height);

    (*mapoutptr)->width = width;
    (*mapoutptr)->height = height;
    (*mapoutptr)->cell_array = cells;
    return 1;
}

/* writes map dimentions into w and h */
int getmapdims(FILE *input, int *pw, int *ph)
{
    int ncols, nrows;
    char c;

    ncols = nrows = 0;
    while ((c = fgetc(input)) != EOF) {
        if (c == '\n')
            ++nrows;
        if (nrows == 0)
            ++ncols;
    }
    *pw = ncols;
    *ph = nrows;
    printf("w: %d, h: %d\n", *pw, *ph);
    return 0;
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

int getcol(int arrindex, int w, int h) 
{
    return arrindex % w;
}

int getrow(int arrindex, int w, int h) 
{
    return arrindex / w;
}

/* Takes 2d coordinates and dimentions and outputs appropriate array index */
int coord_to_array(int x, int y, int w, int h)
{
    if (x >= w || y >= h) {
        fprintf(stderr, "coord_to_array: Given coordinate outside range\n");
    }
    return x + y * w;
}

/* Print error and exit program */
void printerror(char s[])
{
    fprintf(stderr, "%s", s);
    exit(1);
}
