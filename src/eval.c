#include <stdio.h>
#include <stdlib.h>

#include "cell.h"
#include "eval.h"
#include "read.h"
#include "print.h"

/* ---------- */
/* eval/apply */
/* ---------- */

static C *apply(C* c) {
    switch (c->type) {
        case BUILTIN:
            return c->val.func.addr(c->next);
        case LIST:
            return apply(eval(c));
        case LABEL:
            fprintf(stderr, "\nError: attempted to apply non-procedure %s\n", c->val.label);
            exit(1);
        case NIL:
            fprintf(stderr, "\nError: attempted to evaluate an empty list: ()\n");
            exit(1);
    }
}

C *eval(C* c) {
    switch (c->type) {
        case LIST:
        {
            C *out = apply(eval(c->val.list));
            out->next = c->next;
            free(c);
            return out;
        }
        case BUILTIN:
        case LABEL:
        case NIL:
            return c;
    }
}

void eval_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit (1);
    }

    C * c;
    while((c = read(fp)) != &nil) {
        c = eval(c);
        print(c);
        free_cell(c);
    }

    fclose(fp);
}
