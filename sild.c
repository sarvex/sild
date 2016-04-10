#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef union V {
    char * label;
    struct C * list;
    struct C *(*func)(struct C*);
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
        case NIL:
            break;
    }
}

static C nil = { NIL, (V){ .list = NULL }, NULL };


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

void print_inner(C *l, int depth) {
    switch (l->type) {
        case LABEL:
            printf("%s", l->val.label);

            if (l->next->type != NIL)
                printf(" ");

            print_inner(l->next, depth);
            break;
        case LIST:
            printf("(");
            print_inner(l->val.list, depth + 1);

            if (l->next->type != NIL)
                printf(" ");

            print_inner(l->next, depth);
            break;
        case NIL:
            if (depth > 0) {
                printf(")");
            }
            break;
    }
}

void print(C *l) {
    print_inner(l, 0);
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
    return makecell(LABEL, (V){ token }, read(s));
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

/* ----------------- */
/* builtin functions */
/* ----------------- */
C *eval(C*);

C *quote(C *operand) {
    if (operand->type == NIL || operand->next->type != NIL) {
        exit(1);
    }
    return operand;
}

C *car(C *operand) {
    if (operand->type == NIL || operand->next->type != NIL) {
        exit(1);
    }
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
    if (operand->type == NIL || operand->next->type != NIL) {
        exit(1);
    }
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
    if (operand->type == NIL ||
        operand->next->type == NIL ||
        operand->next->next->type != NIL) {
        exit(1);
    }
    operand = eval(operand);
    if (operand->next->type != LIST) {
        exit(1);
    }
    C *operand2 = operand->next;
    operand->next = operand2->val.list;
    operand2->val.list = operand;
    return operand2;
}

/* ---------- */
/* eval/apply */
/* ---------- */

C *apply(C* c) {
    switch (c->type) {
        case LABEL: {
            C *outcell;
            if (scmp(c->val.label, "quote")) {
                outcell = quote(c->next);
            } else if (scmp(c->val.label, "car")) {
                outcell = car(c->next);
            } else if (scmp(c->val.label, "cdr")) {
                outcell = cdr(c->next);
            } else if (scmp(c->val.label, "cons")) {
                outcell = cons(c->next);
            } else {
                exit(1);
            }
            free(c);
            return outcell;
        }
        case LIST:
            return apply(eval(c));
        case NIL:
            exit(1);
    }
}

C *eval(C* c) {
    switch (c->type) {
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

int main() {

    char *a_string = "(cons (quote theng) (quote (thing thang)))";

    C *a_list          = read(&a_string);
    C *an_evalled_list = eval(a_list);
                         print(an_evalled_list);
                         /* debug_list(an_evalled_list); */
    return 0;
}
