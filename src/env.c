#include <stdlib.h>

#include "env.h"
#include "cell.h"
#include "util.h"

struct Env {
    char *key;
    C *value;
};

C *get(Env* env, C *key) {
    if (scmp(key->val.label, env->key)) {
        return truth();
    } else {
        return empty_list();
    }
}

Env * new_env() {
    Env *out = malloc(sizeof(Env));
    out->key = "derp";
    out->value = NULL;
    return out;
}
