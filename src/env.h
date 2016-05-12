#ifndef ENV_GUARD
#define ENV_GUARD

#include "cell.h"

typedef struct Env Env;

void set(Env*, char *key, C *value);
C *get(Env*, C *key);
C *delete(Env*, C *key);

Env *new_env();
void free_env(Env*);

#endif
