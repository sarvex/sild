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

    print_list(read("\
                1"
                ));

    printf("\n");

    print_list(read(
                "1                             2 3 \
                4     "
                ));

    printf("\n");

    print_list(read(
                "\
                jsd\
                fkl;   ajsd     fkl;a\
                \
                \
                sdjkl;"));

    return 0;
}
