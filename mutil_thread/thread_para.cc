#include <iostream>
#include <thread>

class Para {
   public:
    Para() { std::cout << "construct para" << std::endl; }
    Para(const Para& p) {  // 拷贝构造，深拷贝
        std::cout << "copy construct para" << std::endl;
        a = p.a;
    }
    ~Para() { std::cout << "destruct para" << std::endl; }
    std::string a;
};

void SubMain(int i, double d, std::string s, Para p) {
    std::cout << "sub thread, threadid:" << std::this_thread::get_id()
              << std::endl;
    for (int i = 0; i < 3; i++) {
        std::cout << "sub thread working: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "sub thread, over" << std::endl;
}

void SubThreadPtrPara(Para* p) {
    std::cout << "sub thread ptr para, threadid:" << std::this_thread::get_id()
              << std::endl;
    for (int i = 0; i < 3; i++) {
        std::cout << "sub thread ptr para, working: " << i << "para: " << p->a
                  << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "sub thread ptr para,, over" << std::endl;
}

int main() {
    std::cout << "main thread, threadid:" << std::this_thread::get_id()
              << std::endl;
    // {
    //     Para p;
    //     p.a = "aa";
    //     std::thread th(SubMain, 1, 1.1, "test sub thread parameter",
    //                    p);  // 参数传递的时候会被深拷贝
    //     th.join();
    // }

    {
        std::thread h1;
        Para p;
        p.a = "aa";
        h1 = std::thread(SubThreadPtrPara, &p);  // 通过指针传递参数
        // 当退出此作用域的时候 p 的内存就被回收了，不能继续访问了，不然就会野指针了
        // 在传递参数的时候我们应该保证 p 的生命周期和当前的子生命周期是一致的
        // 可以把 p 放到堆内存中
    }

    std::this_thread::sleep_for(std::chrono::seconds(4));

    return 0;
}