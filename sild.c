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
            free(c->val.func.addr);
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
            free(c->val.func.addr);
            free(c->val.func.name);
            free(c);
            break;
        case NIL:
            break;
    }
}

static C nil = { NIL, (V){ .list = NULL }, NULL };

/* ---------- */
/* eval/apply */
/* ---------- */

C *eval(C* c);

C *apply(C* c) {
    switch (c->type) {
        case BUILTIN:
            return c->val.func.addr(c->next);
        case LIST:
            return apply(eval(c));
        case LABEL:
        case NIL:
            exit(1);
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
            out->next = eval(c->next);
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

    if (operand->type == LIST && operand->val.list->type != NIL) {
        return makecell(LIST, (V){.list = &nil}, &nil);
    } else {
        return makecell(LABEL, (V){ "#t" }, &nil);
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

    char *a_string = "(atom whatever)";

    C *a_list          = read(&a_string);
    C *an_evalled_list = eval(a_list);
                         print(an_evalled_list);
                         /* debug_list(an_evalled_list); */
    return 0;
}
