#include <stdio.h>
#include <stdlib.h>

#include "cell.h"
#include "read.h"
#include "eval.h"
#include "print.h"

int main() {

    FILE *input = fopen("./test.sld", "r");

    C * c;
    while((c = read(input)) != &nil) {
        c = eval(c);
        print(c);
        free_cell(c);
    }

    return 0;
}
