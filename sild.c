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
    printf("-------------------------------------------------------\n");
    debug_list_inner(l, 1);

}

C *makecell(int type, V val, C *next) {
    C *out = malloc(sizeof(C));
    if (!out) {
        fprintf(stderr, "System Error: makecell failed to allocate memory.");
        exit(1);
    }
    out->type = type;
    out->val = val;
    out->next = next;
    return out;
};

int is_not_delimiter(char c) {
    return (c != ' ' && c != '\0' && c != '(' && c != ')');
};

char *read_substring(char **s) {
    int l = 0;
    while (is_not_delimiter((*s)[l])) { l++; }
    char *out = malloc(l);
    if (!out) {
        fprintf(stderr, "System Error: read_substring failed to allocate memory.");
        exit(1);
    }
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
        fprintf(stderr, "Syntax Error: mismatched parens");
        exit(1);
    }
}

C * read(char **s);

C* categorize(char **s) {
    char *input = read_substring(s);
    if (!strcmp(input, "debug")) {
        return makecell(LABEL, (V){"debug"}, read(s));
    } else {
        return makecell(LABEL, (V){input}, read(s));
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

C *eval(C* c);

C *apply(C *operator) {
    switch(operator->type) {
        case NIL:
        case LABEL:
            return operator;
        case LIST:
            return apply(operator->val.list);
    }
}

C *eval(C* c) {
    switch (c->type) {
        case NIL:
            return c;
        case LABEL:
            c->next = eval(c->next);
            return c;
        case LIST:
            return apply(c->val.list);
    }
}

int main() {
    char *a_string = "(a (b) c)";
    C *a_list = read(&a_string);
    debug_list(eval(a_list));
    return 0;
}
