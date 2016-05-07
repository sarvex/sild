#ifndef BUILTINS_GUARD
#define BUILTINS_GUARD

#include "cell.h"
#include "print.h"
#include "env.h"

C *quote(C*, Env*);
C *car(C*, Env*);
C *cdr(C*, Env*);
C *cons(C*, Env*);
C *atom(C*, Env*);
C *eq(C*, Env*);
C *cond(C*, Env*);

C *define(C*, Env*);

#endif
