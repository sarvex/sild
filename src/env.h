#ifndef ENV_GUARD
#define ENV_GUARD

#include "cell.h"

C *get(C *env, C *key);
C* new_env();

#endif
