#include <stdio.h>
#include <stdlib.h>

#include "cell.h"
#include "read.h"
#include "eval.h"
#include "print.h"

int main(int argc, char *argv[]) {
    /* if (argc == 1) { */
    /*     fprintf(stderr, "Error, no file names given and repl not yet implemented.\n"); */
    /*     exit(1); */
    /* } */

    /* for (int i = 1; argv[i] != NULL; i++) { */
    /*     eval_file(argv[i]); */
    /* } */

    eval_file("test.sld");

    return 0;
}
