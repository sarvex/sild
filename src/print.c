#include <stdio.h>

#include "cell.h"
#include "env.h"
#include "print.h"

/* ------------------------- */
/* debug and print functions */
/* ------------------------- */

static void printtabs(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("|   ");
    }
}

static void debug_list_inner(C *l, int depth) {
    printtabs(depth);
    switch (l->type) {
        case LABEL:
            printf("LABEL- Address: %p, Value: %s Next: %p\n", l, l->val.label, l->next);
            debug_list_inner(l->next, depth );
            break;
        case LIST:
            printf("LIST- Address: %p, List_Value: %p Next: %p\n", l, l->val.list, l->next);
            debug_list_inner(l->val.list, depth + 1);
            debug_list_inner(l->next, depth);
            break;
        case BUILTIN:
            printf("BUILTIN- Address: %p, Func: %s Next: %p\n", l, l->val.func.name, l->next);
            debug_list_inner(l->next, depth);
            break;
        case PROC:
            printf("PROC- Address: %p, \n| Args: ", l);
            print(l->val.proc.args);
            printf("| Body: ");
            print(l->val.proc.body);
            debug_list_inner(l->next, depth);
            break;
        case NIL:
            printf("NIL- Address: %p\n", &nil);
            printtabs(depth - 1);
            printf("-------------------------------------------------------\n");
            break;
    }
}

void debug_list(C *l) {
    printf("\n");
    debug_list_inner(l, 0);

}

static void print_inner(C *l, int depth, FILE *output_stream) {
    switch (l->type) {
        case LABEL:
            fprintf(output_stream, "%s", l->val.label);

            if (l->next->type != NIL)
                fprintf(output_stream, " ");

            print_inner(l->next, depth, output_stream);
            break;
        case BUILTIN:
            fprintf(output_stream, "%s", l->val.func.name);

            if (l->next->type != NIL)
                fprintf(output_stream, " ");

            print_inner(l->next, depth, output_stream);
            break;
        case LIST:
            fprintf(output_stream, "(");
            print_inner(l->val.list, depth + 1, output_stream);

            if (l->next->type != NIL)
                fprintf(output_stream, " ");

            print_inner(l->next, depth, output_stream);
            break;
        case PROC:
            fprintf(output_stream, "(PROC ");
            print_inner(l->val.proc.args, depth, output_stream);
            fprintf(output_stream, " ");
            print_inner(l->val.proc.body, depth, output_stream);
            fprintf(output_stream, ")");
            break;
        case NIL:
            if (depth > 0) {
                fprintf(output_stream, ")");
            }
            break;
    }
}

void print(C *l) {
    print_inner(l, 0, stdout);
    printf("\n");
};
void print_to_err(C *l) {
    print_inner(l, 0, stderr);
    printf("\n");
};

