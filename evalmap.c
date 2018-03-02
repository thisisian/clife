#include "header.h"

extern struct map *pmap;
extern char rulearr[2][9];

/* Moves mapinputptr to next step of the map */
int step_map() 
{ 
    int i; 
    struct cell *temp;

    for (i = 0; i < pmap->width * pmap->height; ++i) {
        pmap->buffer[i].data = evaluate(i, rulearr);
    }
    temp = pmap->cell_array;
    pmap->cell_array = pmap->buffer;
    pmap->buffer = temp;
    return 0;
}

/* Evaluates a cell and returns a value or -1 if failure */
int evaluate(int arrindex, char rulearr[2][9])
{
    int data = pmap->cell_array[arrindex].data;
    int sum = sum_neighbors(arrindex);

    if (arrindex >= pmap->width * pmap->height) {
        fprintf(stderr, "evaluate: arrindex out of bounds\n");
        return -1;
    }
    if ((data == 0 && rulearr[0][sum] == 1) || 
        (data == 1 && rulearr[1][sum] == 1))
        return 1;
    return 0;
}
/* Sum neighbors at arrindex, return -1 if failure */
int sum_neighbors(int arrindex)
{
    int i;
    int temp;
    int sum = 0;
    
    if (arrindex >= pmap->width * pmap->height) {
        fprintf(stderr, "sum_neighbors: array index out of bounds\n");
        return -1;
    }

    for (i = 1; i <= 8; ++i) {
        temp = find_neighbor(arrindex, i);
        if (temp != -1)
            sum += pmap->cell_array[temp].data;
    }
    return sum;
}

/* 
 * Accepts 2D array index, direction index (CCW starting north), 
 * array dimentions and  outputs index of neighbor 
 */
int find_neighbor(int arrindex, int dir)
{

    int w = pmap->width;
    int h = pmap->height;
    int north = arrindex + w;
    int south = arrindex - w;
    int xpos = getcol(arrindex);
    int ypos = getrow(arrindex);
    int result = -1;

    if (arrindex >= w * h)
        fprintf(stderr, "find_neighbor: Array outside bounds\n");
    if (dir < 1 || dir > 8)
        fprintf(stderr, "find_neighbor: Invalid direction int\n");

    switch(dir) {
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
                result = arrindex + 1;
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
                result = arrindex - 1;
            break;
        case 8:
            if (ypos < h - 1 && xpos > 0)
                result = north - 1;
            break;
        default:
            fprintf(stderr, "find_neighbor: case fell through");
    }
    return result;
}
