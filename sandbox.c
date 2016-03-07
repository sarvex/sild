#include <stdio.h>
#include <stdlib.h>

struct Thing {
    int x;
    int y;
};


int thingy(int i, ...){
    return i;
}

int main() {
    struct Thing mything = { 1, 2 };
    printf("%i\n", mything.x);
    printf("%i\n", mything.y);

    struct Thing *mything2 = malloc(sizeof(struct Thing));
    (*mything2) = { 3, 4 };

    printf("%i\n", mything2->x);
    printf("%i\n", mything2->y);

    /* zomg pointers to functions */

    int (*myprintf)(const char *, ... ) = &printf;
    int (*mythingy)(int, ...) = &thingy;

    myprintf("%i", mythingy(78, 12, 219, 218));
    return 0;
}
