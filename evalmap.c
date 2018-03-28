#include "header.h"
#include "assert.h"

/* External variables */
extern char ruleset[2][9];
extern int maprows;
extern int mapcols;
extern int **buffmap;
extern int **mainmap;

/* Function Prototypes */
int getCell(int r, int c);

/* Function implementations */
/* stepMap sets mainmap to its next iteration */
int stepMap() 
{ 
    int i, j; 
    int **temp;

    for (i = 0; i < maprows; ++i) {
        for (j = 0; j < mapcols; ++j) {
            buffmap[i][j] = evaluate(i, j, ruleset);
        }
    }
    temp = buffmap;
    buffmap = mainmap;
    mainmap = temp;
    return 0;
}

/* evaluate returns the new value of a cell as determined by ruleset  */
int evaluate(int r, int c, char ruleset[2][9])
{
    if (r >= maprows || r < 0 || c >= mapcols || c < 0) {
        printf("(%d, %d)", r, c);
        printError("evaluate: cell out of range of map\n");
    }
    int data = mainmap[r][c];
    int sum = sumMoore(r, c);
    return ((data == 0 && ruleset[0][sum] == 1) || 
            (data == 1 && ruleset[1][sum] == 1));
}

/* sumMoore finds the sum of neighbors at row r column c */
int sumMoore(int r, int c)
{
    int i;
    int sum = 0;
    
    sum += getCell(r-1, c) + getCell(r+1, c);
    for (i = 0; i < 3; ++i) {
        sum += getCell(r-1+i, c+1);
        sum += getCell(r-1+i, c-1);
    }
    assert(sum <= 8);
    return sum;
}

/* 
 * getCell returns the value at cell at row r, column c with respect to
 * toroid plane.
 */
int getCell(int r, int c) {
    int w = mapcols;
    int h = maprows;
    return mainmap[(r % h + h) % h][(c % w + w) % w];
}
