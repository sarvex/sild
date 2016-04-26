enum CellType { NIL, LABEL, LIST, BUILTIN };

struct funcval {
    char *name;
    struct C *(*addr)(struct C*);
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

C *makecell(int type, V val, C *next);
void free_cell(C *c);
void free_one_cell(C *c);
C nil;

C *empty_list();
C *truth();
