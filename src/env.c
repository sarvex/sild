#include <stdlib.h>
#include <string.h>

#include "env.h"
#include "cell.h"
#include "util.h"

typedef struct Entry {
    char *key;
    C *value;
    struct Entry *next;
} Entry;

static Entry *new_entry(char *key, C *value) {
    char *keyval = malloc(sizeof(key));
    if (!keyval) { exit(1); };
    strcpy(keyval, key);

    Entry *out = malloc(sizeof(Entry));
    if (!out) { exit(1); };

    out->key = keyval;
    out->value = value;
    out->next = NULL;
    return out;
}

Env *new_env() {
    Env *out = malloc(sizeof(Env));
    if (!out) { exit(1); };
    out->head = NULL;
    out->next = NULL;
    return out;
}

C *get(Env* env, C *key) {
    Entry *cur = env->head;

    while (cur) {
        if (scmp(key->val.label, cur->key)) {
            return copy_one_cell(cur->value);
        }
        cur = cur->next;
    }

    if (env->next) {
        return get(env->next, key);
    }

    return NULL;
}

void set(Env* env, char *key, C *value) {
    struct Entry *new = new_entry(key, value);
    new->next = env->head;
    env->head = new;
}

Env *copy_env(Env* env) {
    Entry *cur = env->head;

    Env *out = new_env();
    while (cur) {
        set(out, scpy(cur->key), copy_cell(cur->value));
        cur = cur->next;
    }
    out->next = env->next;
    return out;
}

void free_env(Env* env) {
    Entry *cur = env->head;

    while (cur) {
        free(cur->key);
        free_cell(cur->value);
        cur = cur->next;
        if (!cur) { break; };
        free(cur->next);
    }
    free(env->head);
    free(env);
}
