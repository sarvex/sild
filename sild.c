#include <stdio.h>
#include <stdlib.h>

struct Cell {
    int value;
    struct Cell * next;
};

void print_list(struct Cell *car) {
    printf("%d ", car->value);
    if (car->next) {
        print_list(car->next);
    }
}

struct Cell *makecell(int value, struct Cell *next) {
    struct Cell *outcell = malloc(sizeof(struct Cell));
    outcell->value = value;
    outcell->next = next;
    return outcell;
};


int main() {
    struct Cell *a_cell = makecell(1, makecell(2, 0x0));
    print_list(a_cell);
}
