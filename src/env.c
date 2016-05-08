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

    out->key = keyval;
    out->value = value;
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

void set(Env* env, char *key, C *value) {
    struct Entry *new = new_entry(key, value);
    new->next = env->head;
    env->head = new;
}

static void free_entry(struct Entry *entry) {
    free(entry->key);
    free_cell(entry->value);
}

#include <stdio.h> 
void delete_entry(Env* env, char *key) {
    struct Entry *cur = env->head;
    if (scmp(cur->key, key)) {
        env->head = cur->next;
        free_entry(cur);
        return;
    }
    while (cur->next != NIL) {
        if (scmp(cur->next->key, key)) {
            free_entry(cur->next);
            break;
        }
        cur->next = cur->next->next;
    };
}
