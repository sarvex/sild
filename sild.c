#include <stdio.h>
#include <stdlib.h>

enum { NIL, LABEL, LIST };

union V {
    char * label;
    struct C * list;
};

typedef struct C {
    int type;
    union V val;
    struct C * next;
} C;

static C nil = { NIL, (union V){ .list = NULL }, NULL };

void printtabs(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("|   ");
    }
}

void debug_list_inner(C *l, int depth) {
    printtabs(depth);
    if (l->type == LABEL) {
            printf("LABEL- Address: %p, Value: %s Next: %p\n", l, l->val.label, l->next);
            debug_list_inner(l->next, depth );
    } else if (l->type == LIST) {
            printf("LIST- Address: %p, List_Value: %p Next: %p\n", l, l->val.list, l->next);
            debug_list_inner(l->val.list, depth + 1);
            debug_list_inner(l->next, depth);
    } else if (l->type == NIL) {
            printf("NIL- Address: %p\n", &nil);
            printtabs(depth - 1);
            printf("-------------------------------------------------------\n");
    }
}

void debug_list(C *l) {
    printf("\n");
    debug_list_inner(l, 0);

}

C *makecell(int type, union V val, C *next) {
    C *out = malloc(sizeof(C));
    out->type = type;
    out->val = val;
    out->next = next;
    return out;
};

int count_substring_length(char *s) {
    int i = 0;
    while (s[i] != ' ' && s[i] != '\0' && s[i]!= ')') { i++; }
    return i;
}

int current_substring_length = 0;
char *read_substring(char *s) {
    current_substring_length = count_substring_length(s);
    char *out = malloc(current_substring_length);
    for (int i = 0; i < current_substring_length; i++) {
        out[i] = s[i];
    }
    out[current_substring_length] = '\0';
    return out;
};


C * read(char **s) {
    switch(**s) {
        case '\0': case ')':
            (*s)++;
            return &nil;
        case ' ': case '\n':
            (*s)++;
            return read(s);
        case '(':
            (*s)++;
            return makecell(LIST, (union V){.list = read(s)}, read(s));
        default: {
            char *label_val = read_substring(*s);
            (*s) += current_substring_length;
            return makecell(LABEL, (union V){.label = label_val}, read(s));
        }
    }
}

int main() {
    char *a_string = "(maybe (a (more complex) list) will work?)";
    C *a_list = read(&a_string);
    debug_list(a_list);
    return 0;
}
