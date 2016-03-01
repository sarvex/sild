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

int main() {
    struct Cell another_cell = { 2, 0x0 };
    struct Cell a_cell = { 1, &another_cell };

    print_list(a_cell);
}
