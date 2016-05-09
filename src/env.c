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

struct Env {
    struct Entry *head;
};

static Entry *new_entry(char *key, C *value) {
    char *keyval = malloc(sizeof(key));
    if (!keyval) { exit(1); };
    strcpy(keyval, key);

    Entry *out = malloc(sizeof(Entry));
    if (!out) { exit(1); };

    out->key = scpy(keyval);
    out->value = copy_one_cell(value);
    out->next = NULL;
    return out;
}

Env *new_env() {
    Env *out = malloc(sizeof(Env));
    if (!out) { exit(1); };
    out->head = NULL;
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
    return NULL;
}

void free_one_entry(struct Entry *entry) {
    free(entry->key);
    free_one_cell(entry->value);
}

void remove_entry(Env* env, char *key) {
    struct Entry *cur = env->head;

    if (scmp(key, cur->key)) {
        env->head = cur->next;
        free_one_entry(cur);
        return;
    }

    struct Entry *last;
    while (cur) {
        last = cur;
        cur = cur->next;
        if (scmp(key, cur->key)) {
            last->next = cur->next;
            free_one_entry(cur);
            return;
        }
    }
}

void set(Env* env, char *key, C *value) {
    struct Entry *new = new_entry(key, value);
    new->next = env->head;
    env->head = new;
}
