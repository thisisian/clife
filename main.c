#include "header.h"
#include <unistd.h>
#include <regex.h>

int getmapdims(FILE *mapfile, int *pw, int *ph);
void printerror(char *s);

int process_rule_str(char *);
struct map *pmap = NULL;
int randd(int d);

                     /*  0  1  2  3  4  5  6  7  8   */ 
char rulearr[2][9] = { { 0, 0, 0, 1, 0, 0, 0, 0, 0, },     /* Birth */
                       { 0, 0, 1, 1, 0, 0, 0, 0, 0, }, };  /* Surive */
/* Argument flags */
int mflag = 0;
int sflag = 0;
int dflag = 0;
int rflag = 0;
int density = 0;

int main(int argc, char *argv[])
{
    char c;
    FILE *mapfile = NULL;
    int mapwidth, mapheight;
    mapwidth = mapheight = 0;

    srand(time(NULL));

    while ((c = getopt(argc, argv, "m:s:d:r:")) != -1)
        switch (c) {
            case 'm':
                mflag = 1;
                if ((mapfile = fopen(optarg, "r")) == NULL)
                    printerror("Failed to open map file\n");
                break;
            case 's':         /* soup */
                density = atoi(optarg);
                sflag = 1;
                break;
            case 'd':
                if (sscanf(optarg, "%d:%d", &mapwidth, &mapheight) == -1)
                    printerror("Invalid dimentions\n");
                dflag = 1;
                break;
            case 'r':
                if (process_rule_str(optarg) == -1)
                    printerror("Invalid rulestring\n");
                rflag = 1;
                break;
            case '?':
                printf("Invalid option!\n");
        }

    if (mflag == 1 && sflag == 1) 
        printerror("A random soup cannot be generated when provided a map\n");
    if (mflag == 0 && dflag == 0)
        printerror("No dimentions were provided\n");

    initmap(mapfile, mapwidth, mapheight);

    for (;;) {
        printmap();
        printf("<ENTER> to continue\n");
        fgetc(stdin);
        fflush(stdin);
        step_map();
    }
    return 0;
}

/* Modify rulearr based on rulestring s */
int process_rule_str(char s[])
{
    int i;
    regex_t regexp;
    regmatch_t match[3];

    regcomp(&regexp, "^b([0-8]+)/s([0-8]+)$", 
            REG_EXTENDED | REG_ICASE | REG_NEWLINE);
    if (regexec(&regexp, s, 3, match, 0) != 0)
            return -1;
    for (i = match[1].rm_so; i < match[1].rm_eo; ++i)
        rulearr[0][s[i] - '0'] = 1;
    for (i = match[2].rm_so; i < match[2].rm_eo; ++i)
        rulearr[1][s[i] - '0'] = 1;
    regfree(&regexp);
    return 0;
}

/*
 * Load a map into mapoutptr. Returns 1 on success. 
 * If input is NULL, map dimensions will be w and h, otherwise w and h 
 * are ignored
 */
int initmap(FILE *input, int w, int h)
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
    
    pmap = malloc(sizeof (struct map));
    if (pmap == NULL) {
        fprintf(stderr, "intimap: Failed to malloc map");
        exit(1);
    }
    pmap->cell_array = malloc(sizeof (struct cell) * width * height);
    if (pmap->cell_array == NULL) {
        fprintf(stderr, "initmap: Failed to malloc cells");
        exit(1);
    }
    pmap->buffer = malloc(sizeof (struct cell) * width * height);
    if (pmap->buffer == NULL) {
        fprintf(stderr, "initmap: Failed to malloc cells");
        exit(1);
    }
    pmap->width = width;
    pmap->height = height;

    for (i = 0; i < (width * height); ++i) {
        if (sflag == 1) {
            pmap->cell_array[i].data = randd(density);
        } else if (input == NULL) {
            pmap->cell_array[i].data = 0;
        } else {
            while ((c = fgetc(input)) == '\n')
                ;
            pmap->cell_array[i].data = c - '0';
        } 
    }
    printf("%d %d\n", width, height);

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
void printmap()
{
    int i;
    int w = pmap->width;
for (i = 0; i < pmap->width * pmap->height; ++i) {
        if (pmap->cell_array[i].data)
            printf("*");
        else
            printf(" ");
        if ((i + 1) % w == 0 && i != 1)
            printf("\n");
    }
}

int getcol(int arrindex) 
{
    int w = pmap->width;
    return arrindex % w;
}

int getrow(int arrindex) 
{
    int w = pmap->width;
    return arrindex / w;
}

/* Takes 2d coordinates and dimentions and outputs appropriate array index */
int coord_to_array(int x, int y)
{
    int w = pmap->width;
    int h = pmap->height;
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

/* Outputs 1 or 0 using given density */
int randd(int d)
{
    return rand() % 100 > d ? 0 : 1;
}
