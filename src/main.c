#include <stdio.h>
#include <stdlib.h>

#include "evalfile.h"
#include "env.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "Error, no file names given and repl not yet implemented.\n");
        exit(1);
    }

    C *top_level_env = new_env();

    for (int i = 1; argv[i] != NULL; i++) {
        eval_file(argv[i], top_level_env);
    }

    return 0;
}
