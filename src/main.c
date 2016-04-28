#include <stdio.h>
#include <stdlib.h>

#include "cell.h"
#include "read.h"
#include "eval.h"
#include "print.h"

int main() {

    FILE *input = fopen("./test.sld", "r");

    C *a_list;
    C *an_evalled_list;

    a_list          = read(input);
    an_evalled_list = eval(a_list);
                      print(an_evalled_list);

    a_list          = read(input);
    an_evalled_list = eval(a_list);
                      print(an_evalled_list);

    return 0;
}
