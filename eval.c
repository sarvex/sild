#include <stdio.h>
#include <stdlib.h>
#include "cell.h"
#include "eval.h"

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
        case BUILTIN:
        case LABEL:
            c->next = c->next;
            return c;
        case LIST:
        {
            C *out = apply(c->val.list);
            out->next = c->next;
            free(c);
            return out;
        }
        case NIL:
            return c;
    }
}

