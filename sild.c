#include <stdio.h>
#include <stdlib.h>

int inner_reads = 0;

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
    if (l->type == LABEL) {
            printtabs(depth);
            printf("LABEL- Address: %p, Value: %s Next: %p\n",
            l,
            l->val.label,
            l->next);
            debug_list_inner(l->next, depth );
    } else if (l->type == LIST) {
            printtabs(depth);
            printf("LIST- Address: %p, List_Value: %p Next: %p\n",
            l,
            l->val.list,
            l->next);
            debug_list_inner(l->val.list, depth + 1);
            debug_list_inner(l->next, depth);
    } else if (l->type == NIL) {
            printtabs(depth );
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

int count_list_length(char *s) {
    int depth = 1;
    int i = 1;
    while (depth > 0) {

        if (s[i] == 'i')
            inner_reads++;

        if (s[i] == '(') {
            depth += 1;
        } else if (s[i] == ')'){
            depth -= 1;
        }
        i++;
    }
    return i;
}

int count_substring_length(char *s) {
    int i = 0;
    while (s[i] != ' ' && s[i] != '\0' && s[i]!= ')') {

        if (s[i] == 'i')
            inner_reads++;

        i++;
    }
    return i;
}

char *read_substring(char *s) {
    int len = count_substring_length(s);
    char *out = malloc(len);
    for (int i = 0; i < len; i++) {
        if (s[i] == 'i')
            inner_reads++;

        out[i] = s[i];
    }
    out[len] = '\0';
    return out;
};


C * read(char *s) {
    switch(*s) {
        case '\0': case ')':
            return &nil;
        case ' ': case '\n':
            return read(s + 1);
        case '(':
            return makecell(LIST, (union V){.list = read(s + 1)}, read(s + count_list_length(s)));
        default:
            return makecell(LABEL, (union V){.label = read_substring(s)}, read(s + count_substring_length(s)));
    }
}

int main() {
    C *a_list = read("i");
    debug_list(a_list);
    printf("%i", inner_reads);
    return 0;
}
