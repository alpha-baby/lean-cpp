#include <iostream>
#include <string>
#include <cstdio>

class Animal {
   public:
    Animal() {
        std::cout << "call empty Animal constructor" << std::endl;
        name = "empty name";
    }
    Animal(std::string n) : name(n) {
        std::cout << "call Animal constructor" << std::endl;
    }
    Animal(const Animal& a) {
        name = a.name;
        std::cout << "call Animal copy constructor" << std::endl;
    }
    Animal(Animal&& a) {
        a.name = "";
        name = a.name;
        std::cout << "call Animal move constructor" << std::endl;
    }
    Animal& operator=(const Animal& a) {
        if (this == &a) {  // 避免自赋值
            return *this;
        }
        this->name = a.name;
        std::cout << "call Animal operator= copy" << std::endl;
        return *this;
    }
    Animal& operator=(Animal&& a) {
        if (this == &a) {  // 避免自赋值
            return *this;
        }
        this->name = std::move(a.name);
        std::cout << "call Animal operator= move" << std::endl;
        return *this;
    }
    void print() { std::cout << "Animal name: " << name << std::endl; }

   private:
    std::string name;
};
// refer https://blog.csdn.net/xiaozhidian/article/details/114377907
class Human {
   public:
    Human(std::string name, Animal&& pet) : name_(name), pet_(std::move(pet)) {
        std::cout << "call Human constructor" << std::endl;
    }

    Animal& getPet() {
        return pet_;
    }
    int& getNumber() { return a; }

    std::string name_;
    Animal pet_;
    int a;
};

int main() {
    Animal a("mouse");
    a.print();
    Human peopel("zhangsan", std::move(a));
    Animal b("pig");
    b.print();
    b = peopel.getPet();
    b.print();
    char a1,b2;

    printf("a=%X\n", &a);
    printf("a1=%X\n", &a1);
    printf("b2=%X\n", &b2);
    return 0;
}
