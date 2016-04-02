#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum CellType { NIL, LABEL, LIST };

typedef union V {
    char * label;
    struct C * list;
} V;

typedef struct C {
    enum CellType type;
    union V val;
    struct C * next;
} C;

static C nil = { NIL, (V){ .list = NULL }, NULL };

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
            return makecell(LABEL, (V){read_substring(s)}, read(s));
        }
    }
}

void print_list_inner(C *l, int depth) {
    switch (l->type) {
        case LABEL:
            printf("%s", l->val.label);

            if (l->next->type != NIL)
                printf(" ");

            print_list_inner(l->next, depth);
            break;
        case LIST:
            printf("(");
            print_list_inner(l->val.list, depth + 1);

            if (l->next->type != NIL)
                printf(" ");

            print_list_inner(l->next, depth);
            break;
        case NIL:
            if (depth > 0) {
                printf(")");
            }
            break;
    }
}

void print_list(C *l) {
    print_list_inner(l, 0);
};

C *eval(C*);

C *apply(C* c) {
    switch (c->type) {
        case LABEL:
            if (!strcmp(c->val.label, "/dev/null")) {
                free_cell(c);
                return &nil;
            }
        case LIST:
        case NIL:
            return eval(c);
    }
}

C *eval(C* c) {
    switch (c->type) {
        case LABEL:
            c->next = eval(c->next);
            return c;
        case LIST:
            c->val.list = apply(c->val.list);
            c->next = eval(c->next);
            return c;
        case NIL:
            return c;
    }
}

int main() {
    char *a_string = "(this is outside (/dev/null and (anything (can go here) (it (doesn't matter)))))";
    C *a_list = read(&a_string);
    print_list(eval(a_list));
    return 0;
}
