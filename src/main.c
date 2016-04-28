#include <stdio.h>
#include <stdlib.h>

#include "cell.h"
#include "read.h"
#include "eval.h"
#include "print.h"

void eval_file(const char *filename) {
    FILE *f = fopen(filename, "r");

    C * c;
    while((c = read(f)) != &nil) {
        c = eval(c);
        print(c);
        free_cell(c);
    }

    fclose(f);
}

int main() {
    eval_file("./test.sld");
    return 0;
}
