#include <thread>
#include <iostream>

void SubMain() {
    std::cout << "sub thread, threadid:" << std::this_thread::get_id() << std::endl;
    for (int i=0;i<10;i++) {
        std::cout << "sub thread working: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "sub thread, over" << std::endl;
}

int main() {
    std::cout << "main thread, threadid:" << std::this_thread::get_id() << std::endl;
    {
        // std::thread th(SubMain);
        // 执行结果如下：
        // main thread, threadid:0x1ddd98140
        // libc++abi: terminating
        // sub thread, threadid:0x16b8eb000
        // sub thread working: 0
        // [1]    76925 abort      ./a.out
        // 当主线程退出后，程序会执行出错，th 对象会被释放
    }

    {
        std::thread t(SubMain); // 创建一个子线程并执行
        t.detach(); // 子线程和主线程分离，相当与把子线程变成守护线程
        // t.join();分离之后，主线程不能再调用 join 去等待子线程退出
    }
    
    return 0;
}