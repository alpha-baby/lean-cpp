#include <iostream>
#include <thread>
#include <mutex>


int main() {
    auto func = [](){
        std::cout << "test lambda func" << std::endl;
    };
    std::thread th(func);
    th.join();

    {
        static std::mutex mu;
        {
            std::unique_lock<std::mutex> lock(mu);
            // lock area
            lock.unlock();
            // unlock area

            lock.lock();
            // lock area
        }

        {
            std::lock_guard<std::mutex> lock(mu);
        }


        mu.lock();
        {
            // 如果已经拥有锁，那么不锁住,退出的时候会解锁
            std::unique_lock<std::mutex> lock(mu, std::adopt_lock); 
        }

        {
            mu.lock();
            // 如果已经拥有锁，那么不锁住,退出的时候会解锁
            std::unique_lock<std::mutex> lock(mu, std::adopt_lock); 
        }
        mu.lock();
        using namespace std::chrono_literals;
        auto d = 1h;
        std::this_thread::sleep_for(1s);
        mu.unlock();

        {
            std::scoped_lock<std::mutex> lock(mu);
            // scoped_lock 用来解决多锁的场景
        }

    }
    return 0;
}