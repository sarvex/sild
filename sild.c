#include <stdio.h>
#include <stdlib.h>

typedef struct C {
    int val;
    struct C * next;
} C;

void debug_list(C *car) {
    printf("Address: %p, Value: %c, Next: %p\n", car, car->val, car->next);
    if (car->next) {
        debug_list(car->next);
    }
}

void print_list(C *car) {
    printf("%c ", car->val);
    if (car->next) {
        print_list(car->next);
    }
}

C *makecell(char val, C *next) {
    C *out = malloc(sizeof(C));
    out->val = val; out->next = next;
    return out;
};

C * read(char *s) {
    switch(*s) {
        case '\0':
            return NULL;
        case ' ': case '\n':
            return read(s + 1);
        default:
            return makecell(*s, read(s + 1));
    }
}

int main() {
    C *a_list = read("1 2 3");
    debug_list(a_list);
    return 0;
}
