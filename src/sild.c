#include "sild.h"

int main() {
    char *a_string = "(+ 1 1(- 0 2))";
    C *a_list = read(&a_string);
    debug_list(a_list);
    return 0;
}
