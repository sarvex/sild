#include <stdio.h>
#include <stdlib.h>

#include "cell.h"
#include "read.h"
#include "eval.h"
#include "print.h"
#include "env.h"

void eval_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit (1);
    }

    C * c;

    Env *env = new_env();
    set(env, "hi", truth());
    set(env, "hi", empty_list());

    while((c = read(fp)) != &nil) {
        c = eval(c, env);
        print(c);
        free_cell(c);
    }

    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "Error, no file names given and repl not yet implemented.\n");
        exit(1);
    }

    for (int i = 1; argv[i] != NULL; i++) {
        eval_file(argv[i]);
    }

    return 0;
}
