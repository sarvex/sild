#include <stdio.h>
#include <stdlib.h>

/* ----------*/
/* utilities */
/* ----------*/

int scmp(char *str1, char *str2) {
    int i;
    for (i = 0; str1[i] != '\0'; i++) {
        if (str1[i] != str2[i] || str2[i] == '\0') {
            return 0;
        }
    }
    if (str2[i] == '\0') {
        return 1;
    } else {
        return 0;
    }
}

/* ----------------------------------- */
/* cell structures and con/destructors */
/* ----------------------------------- */

enum CellType { NIL, LABEL, LIST, BUILTIN };

struct funcval {
    char *name;
    struct C *(*addr)(struct C*);
};

typedef union V {
    char * label;
    struct C * list;
    struct funcval func;
} V;

typedef struct C {
    enum CellType type;
    union V val;
    struct C * next;
} C;

C *makecell(int type, V val, C *next) {
    C *out = malloc(sizeof(C));
    if (!out) { exit(1); }
    out->type = type;
    out->val = val;
    out->next = next;
    return out;
};

void free_cell(C *c) {
    switch (c->type) {
        case LABEL:
            free(c->val.label);
            free_cell(c->next);
            free(c);
            break;
        case LIST:
            free_cell(c->val.list);
            free_cell(c->next);
            free(c);
            break;
        case BUILTIN:
            free(c->val.func.name);
            free_cell(c->next);
            free(c);
            break;
        case NIL:
            break;
    }
}

void free_one_cell(C *c) {
    switch (c->type) {
        case LABEL:
            free(c->val.label);
            free(c);
            break;
        case LIST:
            free_cell(c->val.list);
            free(c);
            break;
        case BUILTIN:
            free(c->val.func.name);
            free(c);
            break;
        case NIL:
            break;
    }
}

static C nil = { NIL, (V){ .list = NULL }, NULL };

C *empty_list() {
    return makecell(LIST, (V){.list = &nil}, &nil);
}

C *truth() {
    char *tru = malloc(sizeof(char) * 3);
    tru[0] = '#'; tru[1] = 't'; tru[2] = '\0';
    return makecell(LABEL, (V){ tru }, &nil);
}


/* ---------- */
/* eval/apply */
/* ---------- */

C *eval(C* c);

C *apply(C* c) {
    switch (c->type) {
        case BUILTIN:
            return c->val.func.addr(c->next);
        case LIST:
            if (c->val.list->type == NIL) {
                fprintf(stderr, "Error: attempted to apply non-procedure ()");
                exit(1);
            }
            return apply(eval(c));
        case LABEL:
            fprintf(stderr, "Error: attempted to apply non-procedure %s\n", c->val.label);
            exit(1);
        case NIL:
            return empty_list();
    }
}

C *eval(C* c) {
    switch (c->type) {
        case BUILTIN:
        case LABEL:
            c->next = eval(c->next);
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

/* ----------------- */
/* builtin functions */
/* ----------------- */

void print_to_err(C *l);

void arity_check(char *caller_name, int args, C *c) {
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
    if (operand->next->type != LIST) {
        exit(1);
    }
    C *operand2 = operand->next;
    operand->next = operand2->val.list;
    operand2->val.list = operand;
    return operand2;
}

C *atom(C *operand) {
    arity_check("atom", 1, operand);
    operand = eval(operand);

    C *out;
    if (operand->type == LIST && operand->val.list->type != NIL) {
        out = truth();
    } else {
        out = empty_list();
    }
    free_cell(operand);
    return out;
}

C *eq(C *operand) {
    arity_check("eq", 2, operand);
    operand = eval(operand);
    C *operand2 = operand->next;

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

/* ------------------------- */
/* debug and print functions */
/* ------------------------- */

void printtabs(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("|   ");
    }
}

void debug_list_inner(C *l, int depth) {
    printtabs(depth);
    switch (l->type) {
        case LABEL:
            printf("LABEL- Address: %p, Value: %s Next: %p\n", l, l->val.label, l->next);
            debug_list_inner(l->next, depth );
            break;
        case LIST:
            printf("LIST- Address: %p, List_Value: %p Next: %p\n", l, l->val.list, l->next);
            debug_list_inner(l->val.list, depth + 1);
            debug_list_inner(l->next, depth);
            break;
        case BUILTIN:
            printf("BUILTIN- Address: %p, Func: %s Next: %p\n", l, l->val.func.name, l->next);
            debug_list_inner(l->next, depth);
            break;
        case NIL:
            printf("NIL- Address: %p\n", &nil);
            printtabs(depth - 1);
            printf("-------------------------------------------------------\n");
            break;
    }
}

void debug_list(C *l) {
    printf("\n");
    debug_list_inner(l, 0);

}

void print_inner(C *l, int depth, FILE *output_stream) {
    switch (l->type) {
        case LABEL:
            fprintf(output_stream, "%s", l->val.label);

            if (l->next->type != NIL)
                fprintf(output_stream, " ");

            print_inner(l->next, depth, output_stream);
            break;
        case BUILTIN:
            fprintf(output_stream, "%s", l->val.func.name);

            if (l->next->type != NIL)
                fprintf(output_stream, " ");

            print_inner(l->next, depth, output_stream);
            break;
        case LIST:
            fprintf(output_stream, "(");
            print_inner(l->val.list, depth + 1, output_stream);

            if (l->next->type != NIL)
                fprintf(output_stream, " ");

            print_inner(l->next, depth, output_stream);
            break;
        case NIL:
            if (depth > 0) {
                fprintf(output_stream, ")");
            }
            break;
    }
}

void print(C *l) {
    print_inner(l, 0, stdout);
};
void print_to_err(C *l) {
    print_inner(l, 0, stderr);
};

/* ------ */
/* reader */
/* ------ */

int is_not_delimiter(char c) {
    return (c != ' ' && c != '\0' && c != '(' && c != ')');
};

char *read_substring(char **s) {
    int l = 0;
    while (is_not_delimiter((*s)[l])) { l++; }
    char *out = malloc(l);
    if (!out) { exit(1); }
    for (int i = 0; i < l; i++) {
        out[i] = *((*s)++);
    }
    out[l] = '\0';
    return out;
};


int list_depth = 0;
void verify(char c) {
    if (
            list_depth < 0
            ||
            (c == ')' && list_depth == 0)
            ||
            (c == '\0' && list_depth != 0)
       )
    {
        exit(1);
    }
}

C * read(char **s);

C* categorize(char **s) {
    char *token = read_substring(s);
    if (scmp(token, "quote")) {
        return makecell(BUILTIN, (V){ .func = {token, quote} }, read(s));
    } else if (scmp(token, "car")) {
        return makecell(BUILTIN, (V){ .func = {token, car} }, read(s));
    } else if (scmp(token, "cdr")) {
        return makecell(BUILTIN, (V){ .func = {token, cdr} }, read(s));
    } else if (scmp(token, "cons")) {
        return makecell(BUILTIN, (V){ .func = {token, cons} }, read(s));
    } else if (scmp(token, "atom")) {
        return makecell(BUILTIN, (V){ .func = {token, atom} }, read(s));
    } else if (scmp(token, "eq")) {
        return makecell(BUILTIN, (V){ .func = {token, eq} }, read(s));
    } else if (scmp(token, "cond")) {
        return makecell(BUILTIN, (V){ .func = {token, cond} }, read(s));
    } else {
        return makecell(LABEL, (V){ token }, read(s));
    }
}

C * read(char **s) {
    char current_char = **s;

    verify(current_char);

    switch(current_char) {
        case ')': case '\0':
            list_depth--;
            (*s)++;
            return &nil;
        case ' ': case '\n':
            (*s)++;
            return read(s);
        case '(':
            list_depth++;
            (*s)++;
            return makecell(LIST, (V){.list = read(s)}, read(s));
        default: {
            return categorize(s);
        }
    }
}


int main() {

    /* char *a_string = "((cond () cdr) (quote (1 2)))"; */
    char *a_string = "(() (quote (1 2)))";

    C *a_list          = read(&a_string);
    C *an_evalled_list = eval(a_list);
                         print(an_evalled_list);
                         /* debug_list(an_evalled_list); */
    return 0;
}
