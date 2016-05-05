#include <stdlib.h>
#include <string.h>
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

C* copy_cell(C *c) {
    switch (c->type) {
        case LIST:
            return makecell(LIST, (V){ .list = copy_cell(c->val.list) }, copy_cell(c->next));
        case LABEL:
            return makecell(LABEL, (V){c->val.label}, copy_cell(c->next));
        case BUILTIN:
            return makecell(BUILTIN, (V){ .func = {c->val.func.name, c->val.func.addr}}, c->next);
        case NIL:
            return &nil;
    }
}

C* copy_one_cell(C *c) {
    switch (c->type) {
        case LIST:
            return makecell(LIST, (V){ .list = copy_cell(c->val.list) }, &nil);
        case LABEL:
            return makecell(LABEL, (V){c->val.label}, &nil);
        case BUILTIN:
            return makecell(BUILTIN, (V){ .func = {c->val.func.name, c->val.func.addr}}, &nil);
        case NIL:
            return &nil;
    }
}

C nil = { NIL, (V){ .list = NULL }, NULL };

C *empty_list() {
    return makecell(LIST, (V){.list = &nil}, &nil);
}

C *truth() {
    char *tru = malloc(sizeof(char) * 3);
    strcpy(tru, "#t");
    return makecell(LABEL, (V){ tru }, &nil);
}

C *get_void() {
    char *voyd = malloc(sizeof(char) * 6);
    strcpy(voyd, "#void");
    return makecell(LABEL, (V){ voyd }, &nil);
}

