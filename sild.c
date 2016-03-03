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

void debug_list(C *car) {
    if (car->type == LABEL) {
            printf("LABEL- Address: %p, Value: %s Next: %p\n",
            car,
            car->val.label,
            car->next);
            debug_list(car->next);
    } else if (car->type == LIST) {
            printf("LIST- Address: %p, List_Value: %p Next: %p\n",
            car,
            car->val.list,
            car->next);
            debug_list(car->val.list);
            debug_list(car->next);
    } else if (car->type == NIL) {
            printf("NIL- Address: %p\n", &nil);
    }
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
    while (s[i] != ' ' && s[i] != '\0' && s[i]!= ')')
        i++;
    return i;
}

char *read_substring(char *s) {
    int len = count_substring_length(s);
    char *out = malloc(len);
    for (int i = 0; i < len; i++) {
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
    C *a_list = read("(let us (consider) words not chars)");
    debug_list(a_list);
    return 0;
}
