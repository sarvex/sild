#include <stdio.h>

struct Cell {
    int value;
    struct Cell * next;
};

int main() {
    struct Cell a_cell;
    a_cell.value = 1;
    a_cell.next = 0x0;

    printf("a_cell's value is: %d\n", a_cell.value);
    printf("the next cell after a_cell is: %p", a_cell.next);
}
