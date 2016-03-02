#include <stdio.h>
#include <stdlib.h>

typedef struct C {
    int val;
    struct C * next;
} C;

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
    C *output;
    char current = *s;

    if (current == '\0') {
        output = NULL;
    } else {
        output = makecell(current, read(s + 1));
    }
    return output;
}

int main() {
    print_list(read("1"));
    printf("\n");
    print_list(read("1 2 3 4"));
    printf("\n");
    print_list(read("jsdfkl;ajsdfkl;asdjkl;"));
    return 0;
}
