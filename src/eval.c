#include <stdio.h>
#include <stdlib.h>

#include "cell.h"
#include "env.h"
#include "eval.h"
#include "print.h"

/* ---------- */
/* eval/apply */
/* ---------- */

static int count_list(C *c){
    int i= 0;
    while(c->type != NIL) {
        i++;
        c = c->next;
    }
    return i;
};

static C *apply_proc(C* proc, C* supplied_args, Env *env) {

    C *cur = proc->val.proc.args->val.list;
    C *curarg = supplied_args;

    int arity = count_list(cur);
    int numpassed = count_list(curarg);

    if (arity != numpassed) {
        exit(3);
    }

    struct Env *head = copy_env(env);
    for(int i = 0; i < arity; i++) {
        set(env, cur->val.label, eval(curarg, head));
        cur = cur->next;
        curarg = curarg->next;
    }

    C *out = eval(proc->val.proc.body, env);

    cur = proc->val.proc.args->val.list;
    curarg = supplied_args;

    for(int i = 0; i < arity; i++) {
        remove_entry(env, cur->val.label);
        cur = cur->next;
    }
    free_one_cell(proc->val.proc.args);
    free_cell(supplied_args);
    free(proc);
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

