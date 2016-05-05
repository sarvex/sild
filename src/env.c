#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "cell.h"
#include "builtins.h"

void set(C **env, C *key, C *value) {
    C *entry = key;
    entry->next = value;
    value->next = &nil;
    *env = makecell(LIST, (V){.list = entry}, *env);
};

C *get(C **env, C *key) {
    C *cur = *env;
    if (cur->val.list->type == NIL){
        return NULL;
    }

    if (scmp(key->val.label, cur->val.list->val.label)) {
        return cur->val.list->next;
    } else {
        return get(&cur->next, key);
    }
};

C* new_env() {
    return empty_list();
};
