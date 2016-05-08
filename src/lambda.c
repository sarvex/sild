#include <stdio.h>
#include <stdlib.h>

#include "print.h"
#include "cell.h"
#include "env.h"

static void lambda_form_check(C *c) {
    if (c->val.list->next->type != LIST || c->val.list->next->next->type == NIL) {
        c->next = &nil;
        fprintf(stderr, "\nSyntaxError: malformed procedure ");
        print(c);
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

C *lambda(C* c, Env *env) {
    lambda_form_check(c);
    lambda_arity_check(c);

    // ready to rock
    return &nil;
};
