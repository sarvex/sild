#include <stdio.h>
#include <stdlib.h>

#include "print.h"
#include "cell.h"
#include "env.h"
#include "eval.h"

static void lambda_form_check(C *c) {
    if (c->val.list->next->type != LIST
            ||
            c->val.list->next->next->type == NIL
            ||
            c->val.list->next->next->next->type != NIL
       ) {
        c->next = &nil;
        fprintf(stderr, "\nSyntaxError: malformed procedure ");
        exit(1);
    }
};

static void lambda_arity_check(C *c) {
    C *one = c->val.list->next->val.list;
    C *two = c->next;

    int one_count = 0;
    while(one->type != NIL) {
        one_count++;
        one = one->next;
    }

    int two_count = 0;
    while(two->type != NIL) {
        two_count++;
        two = two->next;
    }

    if (!(one_count == two_count)) {
        C *args = c->next;
        c->next = &nil;
        fprintf(stderr, "\nArityError: procedure\n");
        print(c);
        fprintf(stderr, "expected %i argument%c, got %i:\n", one_count, (one_count==1?'\1':'s') ,two_count);
        print(args);
        exit(1);
    }
};

static int args_form_check(C *cur) {
    while(cur->type != NIL) {
        if (cur->type != LABEL) { return 0; }
        cur = cur->next;
    }
    return 1;
}

static void assign_lambda_args(C* c, Env *env) {

    C *args = c->val.list->next->val.list;

    if (!args_form_check(args)) {
        fprintf(stderr, "ArgumentError: procedure argument lists must only be labels. Received: ");
        print(args);
        exit(1);
    };

    C *supplied_args = c->next;

    while(args->type != NIL) {
        set(env, args->val.label, eval(supplied_args, env));
        args = args->next;
        supplied_args = supplied_args->next;
    }
}

static void delete_lambda_args(C* c, Env *env) {
    C *args = c->val.list->next->val.list;
    while(args->type != NIL) {
        delete_entry(env, args->val.label);
        args = args->next;
    }
}

C *lambda(C* c, Env *env) {
    lambda_form_check(c);
    lambda_arity_check(c);
    assign_lambda_args(c, env);
    C *out = eval(c->val.list->next->next, env);
    delete_lambda_args(c, env);
    return out;
};
