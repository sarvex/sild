#include <stdlib.h>
#include "sild.h"

C *makecell(int type, V val, C *next) {
    C *out = malloc(sizeof(C));
    out->type = type;
    out->val = val;
    out->next = next;
    return out;
};
