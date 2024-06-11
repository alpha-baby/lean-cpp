#include <iostream>
#include "lib/word_generator.h"

int main(int argc, char* argv[]) {
    auto&& generator = sandbox::create_generator();
    std::cout << generator->next() << std::endl;
    return 0;
}