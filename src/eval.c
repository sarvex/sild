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
    C *cur = proc->val.proc.args->val.list;

    C *first = supplied_args;
    C *second = supplied_args->next;
    first->next = &nil;
    second->next = &nil;
    first = copy_one_cell(first);
    first = eval(first, env);
    second = copy_one_cell(second);
    second = eval(second, env);

    set(env, cur->val.label, first);
    set(env, cur->next->val.label, second);

    C *out = eval(copy_one_cell(proc->val.proc.body), env);
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

