#ifndef ENV_GUARD
#define ENV_GUARD

#include "cell.h"

typedef struct Env Env;

void set(Env*, char *key, C *value);
void remove_entry(Env*, char *key);
C *get(Env*, C *key);

Env *new_env();

#endif
