#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

int main() {
    // std::chrono::time_point 第一个表示时钟，这里我们使用系统时钟
    // 第二个类型为时间的单位，这里我们使用纳秒
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>
        t1 = std::chrono::system_clock::now();
    // 这里还有一种简单的写法就是
    std::chrono::system_clock::time_point t2 = std::chrono::system_clock::now();

    // 时间转换 -- 把纳秒转换成秒
    std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>
        t3 = std::chrono::time_point_cast<std::chrono::seconds>(t1);
    std::cout << t3.time_since_epoch().count()
              << std::endl;  // 输出的是 unix 时间戳，表示的是0时区
    // convert time type
    time_t tmTomorrow = std::chrono::system_clock::to_time_t(t3);

    // time string
    std::cout << "tomorrow will be: " << ctime(&tmTomorrow);
    // system_clock
    // 是系统时间，如果操作系统的时间发生了变更，那么这个时间也会随着发生变更

    std::cout << "------steady clock-----" << std::endl << std::endl;
    std::chrono::steady_clock::time_point t4 = std::chrono::steady_clock::now();
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::seconds>
        t5 = std::chrono::time_point_cast<std::chrono::seconds>(t4);
    std::cout
        << t4.time_since_epoch().count() << "ns"
        << std::endl;  // 返回的是 当前时间点距离开机时间的时间长度,单位是纳秒
    std::cout
        << t5.time_since_epoch().count() << "s"
        << std::endl;  // 返回的是 当前时间点距离开机时间的时间长度，单位是秒
    std::chrono::steady_clock::time_point t6 = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::micro> dTimeSpan =
        std::chrono::duration<double, std::micro>(t6 - t4);
    std::cout << dTimeSpan.count() << std::endl;

    // 时间长度，第一个表示数据类型，可以是整形，可以是浮点型
    // 第二个是和秒的比值 std::ratio<60, 1> 表示为 1分钟，std::ratio<1, 1> 一秒， std::ratio<1, 1000> 一毫秒
    // std::ratio<60, 1> 这种通常的定义我们可以直接使用  std::nano
    //  std::ratio<1, 1000>  可以直接使用  std::milli 
    // std::chrono::duration<long long, std::ratio<60, 1>> 可以直接使用 std::chrono::milliseconds
    std::chrono::duration<long long, std::ratio<60, 1>> d1(10);
    std::chrono::minutes d2(10);
    std::chrono::seconds s1 = std::chrono::duration_cast<std::chrono::seconds>(d2);
    std::cout <<"d1:"<< d1.count() << std::endl;

    std::cout <<"d2:"<< d2.count() << std::endl;

    std::cout <<"d2 cast to s1:"<< s1.count() << std::endl;
    using namespace ::std::literals::chrono_literals;
    std::chrono::minutes d3 = 1min; // 字面量
    std::this_thread::sleep_for(10s);
    return 0;
}