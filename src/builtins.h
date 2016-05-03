#ifndef BUILTINS_GUARD
#define BUILTINS_GUARD

#include "cell.h"
#include "print.h"

C *quote(C*, C*);
C *car(C*, C*);
C *cdr(C*, C*);
C *cons(C*, C*);
C *atom(C*, C*);
C *eq(C*, C*);
C *cond(C*, C*);

C *display(C*, C*);

#endif
