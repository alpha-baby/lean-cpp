#include <stdio.h>

int global_var = 1;
void foobar(int i) {
    printf("Printing form Lib.so %d\n", i);
    printf("Printing form Lib.so global_var: %d\n", global_var);
}
