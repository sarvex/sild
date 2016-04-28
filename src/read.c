#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "cell.h"
#include "read.h"
#include "builtins.h"

/* ------ */
/* reader */
/* ------ */

static int is_not_delimiter(char c) {
    return (c != ' ' && c != '\n' &&c != '\0' && c != '(' && c != ')' && c != EOF);
};

static char *read_substring(FILE *s) {
    int l = 0;
    while (is_not_delimiter(getc(s))) { l++; }
    char *out = malloc(l);
    if (!out) { exit(1); }

    fseek(s, -l - 1, SEEK_CUR);

    for (int i = 0; i < l; i++) {
        out[i] = getc(s);
    }
    out[l] = '\0';
    return out;
};


static int list_depth = 0;
static void verify(char c) {
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

static C* categorize(FILE *s) {
    char *token = read_substring(s);
    C *out;

    if (scmp(token, "quote")) {
        out = makecell(BUILTIN, (V){ .func = {token, quote} }, &nil);
    } else if (scmp(token, "car")) {
        out = makecell(BUILTIN, (V){ .func = {token, car} }, &nil);
    } else if (scmp(token, "cdr")) {
        out = makecell(BUILTIN, (V){ .func = {token, cdr} }, &nil);
    } else if (scmp(token, "cons")) {
        out = makecell(BUILTIN, (V){ .func = {token, cons} }, &nil);
    } else if (scmp(token, "atom")) {
        out = makecell(BUILTIN, (V){ .func = {token, atom} }, &nil);
    } else if (scmp(token, "eq")) {
        out = makecell(BUILTIN, (V){ .func = {token, eq} }, &nil);
    } else if (scmp(token, "cond")) {
        out = makecell(BUILTIN, (V){ .func = {token, cond} }, &nil);
    } else {
        out = makecell(LABEL, (V){ token }, &nil);
    }

    if (list_depth > 0) {
        out->next = read(s);
    }

    return out;
}

C * read(FILE *s) {
    char current_char = getc(s);

    verify(current_char);

    switch(current_char) {
        case ')': case '\0': case EOF:
            list_depth--;
            return &nil;
        case ' ': case '\n':
            return read(s);
        case '(':
            list_depth++;
            return makecell(
                    LIST,
                    (V){.list = read(s)},
                    (list_depth > 0 ? read(s) : &nil)
                    );
        default:
            fseek(s, -1, SEEK_CUR);
            return categorize(s);
    }
}

