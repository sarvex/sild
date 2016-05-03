#include <stdio.h>
#include <stdlib.h>

#include "cell.h"
#include "read.h"
#include "eval.h"
#include "print.h"

void eval_file(const char *filename, C *env) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit (1);
    }

    C * c;
    while((c = read(fp)) != &nil) {
        c = eval(c, env);
        free_cell(c);
    }

    fclose(fp);
}

