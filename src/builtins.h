#ifndef BUILTINS_GUARD
#define BUILTINS_GUARD

#include "cell.h"
#include "print.h"

C *quote(C*);
C *car(C*);
C *cdr(C*);
C *cons(C*);
C *atom(C*);
C *eq(C*);
C *cond(C*);

C *display(C*);

#endif
