#include <stdlib.h>

#include "env.h"
#include "cell.h"
#include "util.h"

typedef struct Entry {
    char *key;
    C *value;
    struct Entry *next;
} Entry;

struct Env {
    struct Entry *head;
};

C *get(Env* env, C *key) {
    Entry *cur = env->head;

    while (cur) {
        if (scmp(key->val.label, cur->key)) {
            return truth();
        }
        cur = cur->next;
    }
    return NULL;
}

static Entry *new_entry() {
    Entry *out = malloc(sizeof(Entry));
    if (!out) { exit(1); };
    out->key = "derp";
    out->value = NULL;
    out->next = NULL;
    return out;
}

static Entry *new_entry2() {
    Entry *out = malloc(sizeof(Entry));
    if (!out) { exit(1); };
    out->key = "another";
    out->value = NULL;
    out->next = new_entry();
    return out;
}

Env *new_env() {
    Env *out = malloc(sizeof(Env));
    if (!out) { exit(1); };
    out->head = new_entry2();
    return out;
}
