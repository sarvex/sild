#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "cell.h"
#include "eval.h"
#include "builtins.h"
#include "print.h"

/* ------ */
/* reader */
/* ------ */

int is_not_delimiter(char c) {
    return (c != ' ' && c != '\0' && c != '(' && c != ')');
};

char *read_substring(char **s) {
    int l = 0;
    while (is_not_delimiter((*s)[l])) { l++; }
    char *out = malloc(l);
    if (!out) { exit(1); }
    for (int i = 0; i < l; i++) {
        out[i] = *((*s)++);
    }
    out[l] = '\0';
    return out;
};


int list_depth = 0;
void verify(char c) {
    if (
            list_depth < 0
            ||
            (c == ')' && list_depth == 0)
            ||
            (c == '\0' && list_depth != 0)
       )
    {
        exit(1);
    }
}

C * read(char **s);

C* categorize(char **s) {
    char *token = read_substring(s);
    if (scmp(token, "quote")) {
        return makecell(BUILTIN, (V){ .func = {token, quote} }, read(s));
    } else if (scmp(token, "car")) {
        return makecell(BUILTIN, (V){ .func = {token, car} }, read(s));
    } else if (scmp(token, "cdr")) {
        return makecell(BUILTIN, (V){ .func = {token, cdr} }, read(s));
    } else if (scmp(token, "cons")) {
        return makecell(BUILTIN, (V){ .func = {token, cons} }, read(s));
    } else if (scmp(token, "atom")) {
        return makecell(BUILTIN, (V){ .func = {token, atom} }, read(s));
    } else if (scmp(token, "eq")) {
        return makecell(BUILTIN, (V){ .func = {token, eq} }, read(s));
    } else if (scmp(token, "cond")) {
        return makecell(BUILTIN, (V){ .func = {token, cond} }, read(s));
    } else {
        return makecell(LABEL, (V){ token }, read(s));
    }
}

C * read(char **s) {
    char current_char = **s;

    verify(current_char);

    switch(current_char) {
        case ')': case '\0':
            list_depth--;
            (*s)++;
            return &nil;
        case ' ': case '\n':
            (*s)++;
            return read(s);
        case '(':
            list_depth++;
            (*s)++;
            return makecell(LIST, (V){.list = read(s)}, read(s));
        default: {
            return categorize(s);
        }
    }
}


int main() {

    char *a_string = "(cons ((cond car) (quote (1))) (cdr (quote (2 3 4 5))))";

    C *a_list          = read(&a_string);
    C *an_evalled_list = eval(a_list);
                         print(an_evalled_list);
                         /* debug_list(an_evalled_list); */
    return 0;
}
