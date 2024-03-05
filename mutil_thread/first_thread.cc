#include <thread>
#include <iostream>

void ThreadMain() {
    std::cout << "sub thread, threadid:" << std::this_thread::get_id() << std::endl;
    for (int i=0;i<10;i++) {
        std::cout << "sub thread working: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "sub thread, over" << std::endl;
}

int main() {
    std::cout << "main thread, thread id: " << std::this_thread::get_id() << std::endl;
    std::thread th(ThreadMain);
    // 阻塞等待子线程结束
    th.join();
    return 0;
}