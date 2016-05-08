#ifndef ENV_GUARD
#define ENV_GUARD

#include "cell.h"

typedef struct Env Env;

void set(Env*, char *key, C *value);
C *get(Env*, C *key);
void delete_entry(Env*, char *key);

Env *new_env();

#endif
