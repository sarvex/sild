#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "eval.h"
#include "print.h"

/* ----------------- */
/* builtin functions */
/* ----------------- */

static void arity_check(char *caller_name, int args, C *c) {
    C *cur = c;
    int passed_in = 0;
    while (cur->type != NIL) {
        passed_in++;
        cur = cur->next;
    }
    if (passed_in != args) {
        fprintf(stderr,
        "\nArityError: %s expected %d, got %d: ",
        caller_name, args, passed_in
        );
        print_to_err(c);
        exit(1);
    }
}

C *quote(C *operand) {
    arity_check("quote", 1, operand);
    return operand;
}

C *car(C *operand) {
    arity_check("car", 1, operand);

    operand = eval(operand);
    if (operand->type != LIST) {
        exit(1);
    }
    C* outcell = operand->val.list;
    free_cell(operand->val.list->next);
    outcell->next = &nil;
    free(operand);
    return outcell;
}

C *cdr(C *operand) {
    arity_check("cdr", 1, operand);

    operand = eval(operand);
    if (operand->type != LIST || operand->val.list->type == NIL) {
        exit(1);
    }
    C *garbage = operand->val.list;
    operand->val.list = operand->val.list->next;
    free_one_cell(garbage);
    return operand;
}

C *cons(C *operand) {
    arity_check("cons", 2, operand);

    operand = eval(operand);
    C *operand2 = eval(operand->next);
    if (operand2->type != LIST) {
        exit(1);
    }
    operand->next = operand2->val.list;
    operand2->val.list = operand;
    return operand2;
}

C *atom(C *operand) {
    arity_check("atom", 1, operand);
    operand = eval(operand);

    C *out;
    if (operand->type == LIST && operand->val.list->type != NIL) {
        out = empty_list();
    } else {
        out = truth();
    }
    free_cell(operand);
    return out;
}

C *eq(C *operand) {
    arity_check("eq", 2, operand);
    operand = eval(operand);
    C *operand2 = eval(operand->next);

    C *out;
    if (
            (
             operand->type == BUILTIN && operand2->type == BUILTIN
             &&
             (operand->val.func.addr == operand2->val.func.addr)
            )
            ||
            (
             operand->type == LABEL && operand2->type == LABEL
             &&
             scmp(operand->val.label, operand2->val.label)
            )
            ||
            (
             operand->type == LIST && operand2->type == LIST
             &&
             (operand->val.list == &nil && operand2->val.list == &nil)
            )
       )
    {
        out = truth();
    } else {
        out = empty_list();
    }
    free_cell(operand);
    return out;
}


C *cond(C *operand) {
    if (operand->type == NIL) {
        fprintf(stderr, "\nArityError: cond expected at least 1 argument, got none.");
        exit(1);
    } else if (operand->next->type == NIL) {
        return eval(operand);
    }
    // assigning 3 operands to their own vars.
    C *op1 = operand;
    C *op2 = operand->next;
    C *op3 = operand->next->next;

    // isolating the first two arguments
    op1->next = &nil;
    op2->next = &nil;
    op1 = eval(op1);
    if (!(op1->type == LIST && op1->val.list->type == NIL)) {
        // free the boolean expression statement
        free_cell(op1);
        // free anything else that was passed in
        free_cell(op3);
        // return the evalled second arg.
        return eval(op2);
    } else {
        // won't need these anymore!
        free_cell(op1);
        free_cell(op2);

        if (op3->type != NIL) {
            return cond(op3);
        } else {
            // if the op3 type is NIL, you've reached the end of the cond form
            // without encountering any true predicates, and should return the
            // empty list.
            return makecell(LIST, (V){.list = &nil}, &nil);
        }
    }
}

C *load(C *operand) {
    arity_check("load", 1, operand);
    eval_file(operand->val.label);
    return truth();
}
