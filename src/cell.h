#ifndef CELL_GUARD
#define CELL_GUARD

enum CellType { NIL, LABEL, LIST, BUILTIN, PROC, TRUTH };

struct Env;

struct procval {
    struct C *args;
    struct C *body;
    struct Env *env;
};

struct funcval {
    char *name;
    struct C *(*addr)(struct C*, struct Env*);
};

typedef union V {
    char * label;
    struct C * list;
    struct funcval func;
    struct procval proc;
} V;

typedef struct C {
    enum CellType type;
    union V val;
    struct C * next;
} C;

C *makecell(enum CellType, V, C*);
void free_cell(C*);
void free_one_cell(C*);
C *copy_cell(C*);
C *copy_one_cell(C*);

extern C nil;

C *empty_list();
C *truth();

#endif

