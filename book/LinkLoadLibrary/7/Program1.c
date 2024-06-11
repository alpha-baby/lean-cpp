#include "Lib.h"
#include <unistd.h>

int main() {
    foobar(1);
    foobar(global_var);
    sleep(-1);
    return 0;
}