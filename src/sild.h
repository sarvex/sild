#ifndef SILD_H
#define SILD_H

#include <stdio.h>
#include <stdlib.h>

enum CellType { NIL, LABEL, LIST };

typedef union V {
    char * label;
    struct C * list;
} V;

typedef struct C {
    enum CellType type;
    union V val;
    struct C * next;
} C;

static C nil = { NIL, (V){ .list = NULL }, NULL };

C *makecell(int type, V val, C *next);
C * read(char **s);
void debug_list(C *l);

#endif /* SILD_H */
