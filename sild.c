#include <stdio.h>

struct Cell {
    int value;
    struct Cell * next;
};

int main() {
    struct Cell another_cell;
    another_cell.value = 2;
    another_cell.next = 0x0;

    struct Cell a_cell;
    a_cell.value = 1;
    a_cell.next = &another_cell;

    printf("a_cell's value is: %d\n", a_cell.value);
    printf("the next cell after a_cell is: %p, which is called another_cell\n", a_cell.next);
    printf("another_cell's value is: %d\n", another_cell.value);
    printf("the next cell after another_cell is: %p", another_cell.next);
}
