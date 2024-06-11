#include <iostream>

class MySharedPtr {
public:
    virtual ~MySharedPtr() {
        std::cout << "MySharedPrt destructor" << std::endl;
    }
    MySharedPtr() {
        std::cout << "MySharedPrt empty constructor" << std::endl;
    }
    MySharedPtr(const MySharedPtr& p) {
        std::cout << "MySharedPrt copy constructor" << std::endl;
    }
    MySharedPtr(MySharedPtr&& p) {
        std::cout << "MySharedPrt move constructor" << std::endl;
    }
    void setName(const std::string& s) {
        name_ = s;
    }
    void print() const noexcept {
        std::cout << "ptr: " << name_ << std::endl;
    }
private:
    std::string name_;
};

MySharedPtr NewPtr() {
    return MySharedPtr();
}

void test(const MySharedPtr& p) noexcept {
    // p.setName("jack");
    p.print();
    return;
}

int main() {
    auto printVar = [](auto v){
        std::cout << v << std::endl;
        return;
    };
    printVar(1);
    // auto p = NewPtr();
    test(NewPtr());
    std::cout << "hello world" << std::endl;
    return 0;
}