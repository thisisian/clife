#include <stdio.h>
#include <string.h>

struct out {
    char *message;
    struct in {
        char *message;
    } innerstruct;
};


struct out createstruct(void)
{
    struct out structure = {"Hello, world!"};
    struct in innerstruct = {"Hello, moon!"};
    structure.innerstruct = innerstruct;
    return structure;
}

void main(void)
{
    int *i = &2;
    printf("%d\n", *i);
}
