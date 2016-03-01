#include <stdio.h>

struct Cell {
    int value;
    struct Cell * next;
};

void print_list(struct Cell car) {
    printf("%d ", car.value);
    if (car.next) {
        print_list(*car.next);
    }
}

struct Cell makecell(int value, struct Cell *next) {
    struct Cell outcell = { value, next };
    return outcell;
};


int main() {
    struct Cell another_cell = makecell(2, 0x0);
    struct Cell a_cell = makecell(1, &another_cell);

    print_list(a_cell);
}
