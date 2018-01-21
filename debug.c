#include "header.h"

/* Prints info on cell at coordinate x, y */
void probecell(struct map * mapptr, int x, int y)
{

    int width = mapptr->width;
    int height = mapptr->height;
    int index = coord_to_array(x, y, mapptr->width, mapptr->height);
    int col = getcol(index, width, height);
    int row = getrow(index, width, height);
    int val = mapptr->cell_array[index].data;
    int sum = sum_neighbors(*mapptr, index);
    
    fprintf(stderr, "x = %d, y = %d, col = %d, row = %d, index = %d, val = %d"
           " sum = %d, mapw = %d, maph = %d\n",
            x, y, col, row, index, val, sum, width, height);
}

/* Prints map with neighbor sums displayed at each cell */
void printsums(struct map * mapptr)
{
    int i, j, n;
    int sum;
    int w = mapptr->width;
    int h = mapptr->height;
    for (i = 0; i < mapptr->height; ++i){
        for (j = 0; j < mapptr->width; ++j) {
            n = coord_to_array(j, i, w, h);
            sum = sum_neighbors(*mapptr, n);
            printf("%d-", sum);
        }
        printf("\n");
    }
    printf("\n");
}
