#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "cell.h"
#include "builtins.h"

C *get(C *env, C *key) {
    if (scmp(key->val.label, "thinger"))
        return env->val.list;
    else {
        return NULL;
    }
};

static C* car_builtin() {
    char *valfunclabel = malloc(4);
    strcpy(valfunclabel, "cdr");
    return makecell(BUILTIN, (V){ .func = { valfunclabel, cdr} }, &nil);
}

C* new_env() {
    return makecell(LIST, (V){.list = car_builtin()}, &nil);
};
