#include <stdio.h>
#include <stdlib.h>

#include "cell.h"
#include "env.h"
#include "eval.h"
#include "lambda.h"

/* ---------- */
/* eval/apply */
/* ---------- */

static int is_lambda(C *test) {
    return (
            test->type == LIST
            &&
            test->val.list->type == BUILTIN
            &&
            test->val.list->val.func.addr == lambda
            );
};

static C *apply(C* c, Env *env) {
    switch (c->type) {
        case BUILTIN:
            return c->val.func.addr(c->next, env);
        case LIST:
            if (is_lambda(c)) {
                return lambda(c, env);
            } else {
                return apply(eval(c, env), env);
            }
        case LABEL:
            fprintf(stderr, "\nError: attempted to apply non-procedure %s\n", c->val.label);
            exit(1);
        case NIL:
            fprintf(stderr, "\nError: attempted to evaluate an empty list: ()\n");
            exit(1);
    }
}

C *eval(C* c, Env *env) {
    switch (c->type) {
        case LIST:
            if (is_lambda(c)) {
                return c;
            } else {
                C *out = apply(eval(c->val.list, env), env);
                out->next = c->next;
                free(c);
                return out;
            }
        case LABEL:
        {
            C *out = get(env, c);
            if (out) {
                free_one_cell(c);
                return out;
            } else {
                fprintf(stderr, "\nError: unbound label: %s\n", c->val.label);
                exit(1);
            }
        }
        case BUILTIN:
        case NIL:
            return c;
    }
}

