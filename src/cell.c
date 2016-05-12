#include <stdlib.h>
#include "cell.h"
#include "util.h"
#include "env.h"

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
        case PROC:
            free_cell(c->val.proc.args);
            free_cell(c->val.proc.body);
            free_env(c->val.proc.env);
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
        case PROC:
            free_cell(c->val.proc.args);
            free_cell(c->val.proc.body);
            free_env(c->val.proc.env);
            break;
        case NIL:
            break;
    }
}

C *copy_cell(C *c) {
    switch (c->type) {
        case LABEL:
            return makecell(LABEL, (V){ scpy(c->val.label) }, copy_cell(c->next));
        case LIST:
            return makecell(LIST, (V){ .list = copy_cell(c->val.list) }, copy_cell(c->next));
        case BUILTIN:
            return makecell(BUILTIN, (V){ .func = { scpy(c->val.func.name), c->val.func.addr} }, copy_cell(c->next));
        case PROC:
            return makecell(PROC, (V){ .proc = { copy_one_cell(c->val.proc.args), copy_one_cell(c->val.proc.body), c->val.proc.env } }, copy_cell(c->next));
        case NIL:
            return &nil;
    }
}

C *copy_one_cell(C *c) {
    switch (c->type) {
        case LABEL:
            return makecell(LABEL, (V){ scpy(c->val.label) }, &nil);
        case LIST:
            return makecell(LIST, (V){ .list = copy_cell(c->val.list) }, &nil);
        case BUILTIN:
            return makecell(BUILTIN, (V){ .func = { scpy(c->val.func.name), c->val.func.addr} }, &nil);
        case PROC:
            return makecell(PROC, (V){ .proc = { copy_one_cell(c->val.proc.args), copy_one_cell(c->val.proc.body), c->val.proc.env } }, &nil);
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
    tru[0] = '#'; tru[1] = 't'; tru[2] = '\0';
    return makecell(LABEL, (V){ tru }, &nil);
}

