#include <stdio.h>
#include <string.h>

struct out {
    struct in inside;
    char message[];
}

struct in {
    char message[];
}

struct out createstruct(void)
{
    struct out structure;
    strcpy(structure.message, "Hello one");
    strcpy(structure.message.in.message, "Hello two");
    return structure;
}

void main(void)
{
    struct out structure = createstruct(void)
        printf("%s\n", structure.message);
        printf("%s\n", structure.message.in.message);
}
