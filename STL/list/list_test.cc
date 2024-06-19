#include <list>
#include <iostream>
#include <string>
#include <memory>

class Base {
public:
  ~Base() { std::cout << "Base destrust" << std::endl; }
  Base() { std::cout << "Base no parameters construst" << std::endl; }
Base(Base& b) = delete;
  Base(const Base& b) { std::cout << "Base copy construst" << std::endl; }

  Base(Base&& b) { std::cout << "Base move construst" << std::endl; }

private:
  std::string name = "";
};

int main() {
  std::list<Base> l = {};
  l.emplace_back(Base());
  return 0;
}