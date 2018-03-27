#include "header.h"
#include "assert.h"

/* External variables */
extern char rulearr[2][9];
extern int maprows;
extern int mapcols;
extern int **buff;
extern int **mainmap;

/* Function Prototypes */
int getcell(int r, int c);

/* Function implementations */
/* step_map sets mainmap to next iteration */
int step_map() 
{ 
    printf("%d , %d\n", maprows, mapcols);
    int i, j; 
    int **temp;

    for (i = 0; i < maprows; ++i) {
        for (j = 0; j < mapcols; ++j) {
            buff[i][j] = evaluate(i, j, rulearr);
        }
    }
    temp = buff;
    buff = mainmap;
    mainmap = temp;
    return 0;
}

/* Evaluates a cell and returns a value or -1 if failure */
int evaluate(int r, int c, char rulearr[2][9])
{
    int data = mainmap[r][c];
    int sum = summoore(r, c);
    return ((data == 0 && rulearr[0][sum] == 1) || 
            (data == 1 && rulearr[1][sum] == 1));
}
/* summoore finds the sum of neighbors at row r column c */
int summoore(int r, int c)
{
    int i;
    int sum = 0;
    
    sum += getcell(r-1, c) + getcell(r+1, c);
    for (i = 0; i < 3; ++i) {
        sum += getcell(r-1+i, c+1);
        sum += getcell(r-1+i, c-1);
    }
    assert(sum <= 8);
    return sum;
}

/* getc returns the value at cell at row r, column c. Respects toroid plane */
int getcell(int r, int c) {
    int w = mapcols;
    int h = maprows;
    return mainmap[(r % h + h) % h][(c % w + w) % w];
}
