#include <stdlib.h>

#include "env.h"
#include "cell.h"
#include "util.h"

typedef struct Entry {
    char *key;
    C *value;
} Entry;

struct Env {
    struct Entry *head;
};

C *get(Env* env, C *key) {
    if (scmp(key->val.label, env->head->key)) {
        return truth();
    } else {
        return empty_list();
    }
}

static Entry *new_entry() {
    Entry *out = malloc(sizeof(Entry));
    if (!out) { exit(1); };
    out->key = "derp";
    out->value = NULL;
    return out;
}

Env *new_env() {
    Env *out = malloc(sizeof(Env));
    if (!out) { exit(1); };
    out->head = new_entry();
    return out;
}
