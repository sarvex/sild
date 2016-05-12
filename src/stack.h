#ifndef STACK_GUARD
#define STACK_GUARD

struct StackEntry {
    const char *name;
};

extern int stackindex;
extern struct StackEntry stack[100];

#endif
