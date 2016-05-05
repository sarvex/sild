#include <stdio.h>
#include <stdlib.h>

#include "evalfile.h"
#include "env.h"

#include "cell.h"
#include "string.h"
#include "builtins.h"

static C* cdr_builtin() {
    char *valfunclabel = malloc(4);
    strcpy(valfunclabel, "cdr");
    return makecell(BUILTIN, (V){ .func = { valfunclabel, cdr} }, &nil);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "Error, no file names given and repl not yet implemented.\n");
        exit(1);
    }

    C *top_level_env = new_env();
    top_level_env = set(&top_level_env,
            makecell(LABEL, (V){"whatever"}, &nil),
            cdr_builtin());

    for (int i = 1; argv[i] != NULL; i++) {
        eval_file(argv[i], &top_level_env);
    }

    return 0;
}
