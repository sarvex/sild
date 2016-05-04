#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "cell.h"
#include "builtins.h"

C *set(C *env, C *key, C *value) {
    C *entry = key;
    entry->next = value;
    value->next = &nil;
    return makecell(LIST, (V){.list = entry}, env);
};

C *get(C *env, C *key) {
    if (env->val.list->type == NIL){
        return NULL;
    }

    if (scmp(key->val.label, env->val.list->val.label)) {
        return env->val.list->next;
    } else {
        return get(env->next, key);
    }
};

C* new_env() {
    return empty_list();
};
