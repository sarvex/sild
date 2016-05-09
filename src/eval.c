#include <stdio.h>
#include <stdlib.h>

#include "cell.h"
#include "env.h"
#include "eval.h"
#include "print.h"

/* ---------- */
/* eval/apply */
/* ---------- */

static C *apply_proc(C* proc, C* supplied_args, Env *env) {
    proc->next = &nil;

    // need to set all passed in args here
    set(env, proc->val.proc.args->val.list->val.label, eval(supplied_args, env));

    C *out = eval(proc->val.proc.body, env);

    // need to reset env with delete here
    // need to free all things that need to be freed here

    return out;
}

static C *apply(C* c, Env *env) {
    switch (c->type) {
        case BUILTIN:
            return c->val.func.addr(c->next, env);
        case LIST:
            return apply(eval(c, env), env);
        case PROC:
            return apply_proc(c, c->next, env);
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
                free_one_cell(c);
                return out;
            } else {
                fprintf(stderr, "\nError: unbound label: %s\n", c->val.label);
                exit(1);
            }
        }
        case BUILTIN:
        case PROC:
        case NIL:
            return c;
    }
}

