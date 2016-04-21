#include <stdio.h>
#include <stdlib.h>

/* ----------*/
/* utilities */
/* ----------*/

int scmp(char *str1, char *str2);

/* ----------------------------------- */
/* cell structures and con/destructors */
/* ----------------------------------- */

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
static C nil = { NIL, (V){ .list = NULL }, NULL };

C *empty_list();
C *truth();

/* ---------- */
/* eval/apply */
/* ---------- */

C *apply(C* c);
C *eval(C* c);

/* ----------------- */
/* builtin functions */
/* ----------------- */

void arity_check(char *caller_name, int args, C *c);

C *quote(C *operand);
C *car(C *operand);
C *cdr(C *operand);
C *cons(C *operand);
C *atom(C *operand);
C *eq(C *operand);
C *cond(C *operand);

/* ------------------------- */
/* debug and print functions */
/* ------------------------- */

void printtabs(int depth);
void debug_list_inner(C *l, int depth);
void debug_list(C *l);
void print_inner(C *l, int depth, FILE *output_stream);
void print(C *l);;
void print_to_err(C *l);;

/* ------ */
/* reader */
/* ------ */

int is_not_delimiter(char c);
char *read_substring(char **s);
void verify(char c);
C* categorize(char **s);
C * read(char **s);
