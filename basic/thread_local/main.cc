#include <iostream>
#include <thread>

        
class A {
   public:
    A() {
        std::cout << "-------- construct -----------" << std::endl;
    }
    ~A() {
        std::cout << "-------- destruct: " + thread_name_ + " ------------" << std::endl;
    }
    void set_thread_name(const std::string& name) {
        thread_name_ = name;
        return;
    }

    void test() {
        ++count;
        std::cout << thread_name_ << ": " << count << std::endl;
    }
    std::string  thread_name_;
    int count;
};


thread_local A a_local;

void func(const std::string &name) {
    std::cout << "start run " + name + " thread" << std::endl;
    a_local.set_thread_name(name);
    a_local.test();
    a_local.test();
    std::cout << "end " + name + " thread" << std::endl;
    // a_local will be destruct
}

// build:
// g++ -std=c++11
int main(int argc, char *argv[]) {
    std::cout << "start run main thread" << std::endl;
    a_local.set_thread_name("main");
    a_local.test();
    
    
    std::thread t1(func, "t1");  // 创建线程 1
    // std::thread t2(func, "t2");  // 创建线程 2
    t1.join();
    // t2.join();
    std::cout << "end main thread" << std::endl;
    // run here, main thread's variable of `a_locala` will be destruct
    return 0;
}