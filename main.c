#include "sild.h"

int main() {

    char *a_string = "(quote 1)";

    C *a_list          = read(&a_string);
    C *an_evalled_list = eval(a_list);
                         print(an_evalled_list);
                         /* debug_list(an_evalled_list); */
    return 0;
}
