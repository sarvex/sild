#include <stdio.h>
#include <stdlib.h>

typedef struct C {
    int val;
    struct C * next;
} C;


void print_list(C *car) {
    printf("%d ", car->val);
    if (car->next) {
        print_list(car->next);
    }
}

C *makecell(int val, C *next) {
    C *out = malloc(sizeof(C));
    out->val = val; out->next = next;
    return out;
};


int main() {
    C *a_cell = makecell(1, makecell(2, NULL));
    print_list(a_cell);
    return 0;
}
