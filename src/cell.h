#ifndef CELL_GUARD
#define CELL_GUARD

enum CellType { NIL, LABEL, LIST, BUILTIN };

struct funcval {
    char *name;
    struct C *(*addr)(struct C*, struct C*);
};

typedef union V {
    char * label;
    struct C * list;
    struct funcval func;
} V;

typedef struct C {
    enum CellType type;
    union V val;
    struct C * next;
} C;

C *makecell(enum CellType, V, C*);
void free_cell(C*);
void free_one_cell(C*);

extern C nil;

C *empty_list();
C *truth();
C *get_void();

#endif

