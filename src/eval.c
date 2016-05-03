#include <stdio.h>
#include <stdlib.h>

#include "cell.h"
#include "eval.h"
#include "print.h"
#include "env.h"

/* ---------- */
/* eval/apply */
/* ---------- */

static C *apply(C* c, C *env) {
    switch (c->type) {
        case BUILTIN:
            return c->val.func.addr(c->next, env);
        case LIST:
            c->next = &nil;
            fprintf(stderr, "\nError: attempted to apply non-procedure: ");
            print_to_err(c);
            exit(1);
        case LABEL:
            fprintf(stderr, "\nError: attempted to apply non-procedure %s\n", c->val.label);
            exit(1);
        case NIL:
            fprintf(stderr, "\nError: attempted to evaluate an empty list: ()\n");
            exit(1);
    }
}

C *eval(C* c, C* env) {
    switch (c->type) {
        case LIST:
        {
            C *out = apply(eval(c->val.list, env), env);
            out->next = c->next;
            free(c);
            return out;
        }
        case LABEL:
        {
            C *out = get(env, c);
            if (out) {
                out->next = c->next;
                free(c);
                return out;
            } else {
                fprintf(stderr, "\nError: unbound label: %s\n", c->val.label);
                exit(1);
            };
        }
        case BUILTIN:
        case NIL:
            return c;
    }
}

