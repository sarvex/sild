#include <stdlib.h>
#include "cell.h"

/* ----------------------------------- */
/* cell structures and con/destructors */
/* ----------------------------------- */

C *makecell(enum CellType type, V val, C *next) {
    C *out = malloc(sizeof(C));
    if (!out) { exit(1); }
    out->type = type;
    out->val = val;
    out->next = next;
    return out;
};

void free_cell(C *c) {
    switch (c->type) {
        case LABEL:
            free(c->val.label);
            free_cell(c->next);
            free(c);
            break;
        case LIST:
            free_cell(c->val.list);
            free_cell(c->next);
            free(c);
            break;
        case BUILTIN:
            free(c->val.func.name);
            free_cell(c->next);
            free(c);
            break;
        case NIL:
            break;
    }
}

void free_one_cell(C *c) {
    switch (c->type) {
        case LABEL:
            free(c->val.label);
            free(c);
            break;
        case LIST:
            free_cell(c->val.list);
            free(c);
            break;
        case BUILTIN:
            free(c->val.func.name);
            free(c);
            break;
        case NIL:
            break;
    }
}

static C nil = { NIL, (V){ .list = NULL }, NULL };

C *empty_list() {
    return makecell(LIST, (V){.list = &nil}, &nil);
}

C *truth() {
    char *tru = malloc(sizeof(char) * 3);
    tru[0] = '#'; tru[1] = 't'; tru[2] = '\0';
    return makecell(LABEL, (V){ tru }, &nil);
}

