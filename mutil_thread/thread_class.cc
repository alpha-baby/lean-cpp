#include <iostream>
#include <thread>

class MyThread {
   public:
    void Main() {  // 入口函数
        std::cout << "MyThread main " << name << ": " << age << std::endl;
    }
    std::string name;
    int age;
};

class XThread {
   public:
    virtual void Strat() {
        th_ = std::thread(&XThread::Main, this);

    }
    virtual void Wait() {
        if (!th_.joinable()) {
            return;
        }
        th_.join();
    }
    virtual void Stop() {
        is_exit_ = true;
        this->Wait();
    }
    bool is_exit() {
        return this->is_exit_;
    }
    std::string name;
   private:
   virtual void Main() = 0;
   std::thread th_; 
   bool is_exit_ = false;
};

class TestXThread: public XThread {
    public:
    void Main() override {
        std::cout << "TestThread Main()"  << std::endl;
        while (!is_exit())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << "." << std::flush;
        }
        
    }
};

int main() {
    // MyThread myth;
    // myth.age = 10;
    // myth.name = "test";
    // std::thread th(&MyThread::Main, &myth);
    // th.join();

    TestXThread testTh;
    testTh.name = "test thread";
    testTh.Strat();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    testTh.Stop();
    testTh.Wait();
    return 0;
}