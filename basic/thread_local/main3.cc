#include <iostream>
#include <thread>

class A {
   public:
    A() { std::cout << "-------- construct -----------" << std::endl; }
    ~A() {
        std::cout << "-------- destruct: " + thread_name_ + " ------------"
                  << std::endl;
    }
    void set_thread_name(const std::string &name) {
        thread_name_ = name;
        return;
    }

    void test() {
        ++count;
        std::cout << thread_name_ << ": " << count << std::endl;
    }
    std::string thread_name_;
    int count;
};

class ThreadLocalClass {
   public:
     static thread_local A a_;
};

thread_local A ThreadLocalClass::a_;

void func(const std::string &name) {
    std::cout << "start run " + name + " thread" << std::endl;
    ThreadLocalClass::a_.set_thread_name(name);
    ThreadLocalClass::a_.test();
    ThreadLocalClass::a_.test();
    std::cout << "end " + name + " thread" << std::endl;
    // a_local will be destruct
}

// build:
// g++ -std=c++11
int main(int argc, char *argv[]) {
    std::cout << "start run main thread" << std::endl;
    func("main");
    std::thread t1(func, "t1");  // 创建线程 1
    std::thread t2(func, "t2");  // 创建线程 2
    t1.join();
    t2.join();
    func("main");
    std::cout << "end main thread" << std::endl;
    return 0;
}