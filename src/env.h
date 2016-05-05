#ifndef ENV_GUARD
#define ENV_GUARD

#include "cell.h"

C *get(C **env, C *key);
C *set(C **env, C *key, C *value);
C* new_env();

#endif
