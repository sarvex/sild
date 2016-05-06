#ifndef ENV_GUARD
#define ENV_GUARD

#include "cell.h"

typedef struct Env Env;

C *set(Env*, C *key, C *value);
C *get(Env*, C *key);
C *delete(Env*, C *key);

Env *new_env();

#endif
