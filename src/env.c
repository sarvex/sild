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

    struct Entry *entry1 = new_entry("one", truth());
    struct Entry *entry2 = new_entry("two", truth());
    entry1->next = entry2;

    Env *out = malloc(sizeof(Env));
    if (!out) { exit(1); };
    out->head = entry1;
    return out;
}
