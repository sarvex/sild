#include <stdio.h>
#include <stdlib.h>

enum { LABEL, LIST };

union V {
    char * label;
    struct C * list;
};

typedef struct C {
    int type;
    union V;
    struct C * next;
} C;

void debug_list(C *car) {
    if (car->type == LABEL) {
            printf("LABEL- Address: %p, Value: %s Next: %p\n",
            car,
            car->val,
            car->next);
    } else if (car->type == LIST) {
            printf("LIST- Address: %p, List_Value: %p Next: %p\n",
            car,
            car->list_val,
            car->next);
    }

    if (car->list_val) {
        debug_list(car->list_val);
    } else if (car->next) {
        debug_list(car->next);
    }
}

C *makecell(int type, char *val, C *list_val, C *next) {
    C *out = malloc(sizeof(C));
    out->type = type;
    out->val = val;
    out->list_val = list_val;
    out->next = next;
    return out;
};

int count_list_length(char *s) {
    int i = 0;
    while (s[i] != ')' && s[i] != '\0')
        i++;
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
            return NULL;
        case ' ': case '\n':
            return read(s + 1);
        case '(':
            return makecell(LIST, NULL, read(s + 1), read(s + count_list_length(s) + 1));
        default:
            return makecell(LABEL, read_substring(s), NULL, read(s + count_substring_length(s) + 1));
    }
}

int main() {
    C *a_list = read("(let us consider words not chars)");
    debug_list(a_list);
    return 0;
}
