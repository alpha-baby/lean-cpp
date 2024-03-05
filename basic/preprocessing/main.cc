#include <iostream>

#if !defined(__APPLE__)
#error "Linux platform file is part of non-Linux build."
#endif

#if defined(__APPLE__)
void hello_world() {
    std::cout << "hello world apple" << std::endl;
}
#elif defined(__linux__)
void hello_world() {
    std::cout << "hello world linux" << std::endl;
}
#elif defined(_WIN32) 
void hello_world() {
    std::cout << "hello world win32" << std::endl;
}
#elif defined(_WIN64) 
void hello_world() {
    std::cout << "hello world win64" << std::endl;
}
#endif



int main() {
    hello_world();
    return 0;
}