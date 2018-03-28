#include "header.h"
#include <unistd.h>
#include <regex.h>

/* Function prototypes */
int getMapDims(FILE *, int *, int *);
int processRuleStr(char *);
int randBin(float);
int **make2DArr(unsigned int, unsigned int);

/* ruleset defines number of neighbors for a cell to give birth or survive */
         /* # Neighbors: 0  1  2  3  4  5  6  7  8  */ 
char ruleset[2][9] = { { 0, 0, 0, 1, 0, 0, 0, 0, 0, },     /* Birth */
                       { 0, 0, 1, 1, 0, 0, 0, 0, 0, }, };  /* Surive */

/* Global variables */
float density = 50;

/* Map-related variables */
int maprows, mapcols = 0;
int **mainmap = NULL;
int **buffmap = NULL;

/* Command-line argument flags */
int mflag = 0;
int sflag = 0;
int dflag = 0;
int rflag = 0;

int main(int argc, char *argv[])
{
    char c;     // temp variable
    FILE *mapfile = NULL;

    srand(time(NULL));

    while ((c = getopt(argc, argv, "m:s:d:r:")) != -1)
        switch (c) {
            case 'm':
                mflag = 1;
                if ((mapfile = fopen(optarg, "r")) == NULL)
                    printError("Failed to open map file\n");
                break;
            case 's':         /* soup */
                density = atof(optarg);
                sflag = 1;
                break;
            case 'd':
                if (sscanf(optarg, "%d:%d", &mapcols, &maprows) == -1)
                    printError("Invalid dimentions\n");
                dflag = 1;
                break;
            case 'r':
                if (processRuleStr(optarg) == -1)
                    printError("Invalid rulestring\n");
                rflag = 1;
                break;
            case '?':
                printf("Invalid option!\n");
                exit(1);
        }

    if (mflag == 1 && sflag == 1) 
        printError("A random soup cannot be generated"
                   " in a loaded mapfile.\n");
    if (mflag == 0 && dflag == 0)
        printError("No dimentions were provided\n");

    initMap(mapfile, mapcols, maprows);

    for (;;) {
        printMap();
        printf("<ENTER> to continue\n");
        fgetc(stdin);
        fflush(stdin);
        stepMap();
    }
    return 0;
}

/* processRuleStr modifies ruleset based on rulestring s */
int processRuleStr(char s[])
{
    int i;
    regex_t regexp;
    regmatch_t match[3];

    regcomp(&regexp, "^b([0-8]+)/s([0-8]+)$", 
            REG_EXTENDED | REG_ICASE | REG_NEWLINE);
    if (regexec(&regexp, s, 3, match, 0) != 0)
            return -1;
    for (i = match[1].rm_so; i < match[1].rm_eo; ++i)
        ruleset[0][s[i] - '0'] = 1;
    for (i = match[2].rm_so; i < match[2].rm_eo; ++i)
        ruleset[1][s[i] - '0'] = 1;
    regfree(&regexp);
    return 0;
}


/*
 * initMap initalizes  a map into mainmap and loads in cells
 * If input is NULL, map dimensions will be w and h, otherwise w and h 
 * are ignored
 */
int initMap(FILE *input, int w, int h)
{
    int i, j;
    char c;

    if (input != NULL) { 
        getMapDims(input, &mapcols, &maprows);
        rewind(input);
    } 
    
    /* Create map */
    buffmap = make2DArr(maprows,mapcols);
    mainmap = make2DArr(maprows,mapcols);

    /* Fill cells */
    for (i = 0; i < maprows; ++i) {
        for (j = 0; j < mapcols; ++j) {
            if (sflag == 1) {
                mainmap[i][j] = randBin(density);
            } else if (input == NULL) {
                mainmap[i][j] =  0;
            } else {
                while ((c = fgetc(input)) == '\n')
                    ;
                mainmap[i][j] = c - '0';
            } 
        }
    }

    return 1;
}

/* getMapDims writes input's  dimentions into w and h */
int getMapDims(FILE *input, int *pw, int *ph)
{
    int mapcols, maprows;
    char c;

    mapcols = maprows = 0;
    while ((c = fgetc(input)) != EOF) {
        if (c == '\n')
            ++maprows;
        if (maprows == 0)
            ++mapcols;
    }
    *pw = mapcols;
    *ph = maprows;
    return 0;
}

/* printMap prints a map to stdout */
void printMap()
{
    int i, j;
    for (i = 0; i < maprows; ++i) {
        for (j = 0; j < mapcols; ++j) {
            if (mainmap[i][j] == 1)
                printf("%c", 'o');
            else if (mainmap[i][j] == 0) {
                printf("%c", '.');
            }
            else
                printf("#");
        }
        printf("\n");
    }
}

/* make2DArr returns a dynamically allocated 2D int array */
int **make2DArr(unsigned int nrows, unsigned int ncols) 
{
    int **array = malloc(nrows * sizeof (int*) + sizeof (int) * nrows * ncols);
    if (array == NULL)
        printf("made2DArr: failed to malloc array\n");
    for(size_t i = 0; i < nrows; i++)
      array[i] = (int *) (array + nrows) + i * ncols;
    return array;
}

/* printError prints s and exits program */
void printError(char s[])
{
    fprintf(stderr, "%s", s);
    exit(1);
}

/* randBin has a p probabilty of returning 1 and (1-p) of returning 0  */
int randBin(float p)
{
    return (float)rand()/RAND_MAX > p ? 0 : 1;
}
