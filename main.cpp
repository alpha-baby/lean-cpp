#include "main.h"
#include "ctime"
#include "unistd.h"
#include <iostream>

using namespace std;

Student::Student(const string name, int age, int no) : m_c(time(nullptr)) {
  m_name = name;
  m_age = age;
  m_no = no;
}

Student::Student(const Student &s) {
  this->m_name = s.m_name;
  this->m_age = s.m_age;
  this->m_no = s.m_no;
}

void Student::who() {
  cout << "名字：" << m_name << "，年龄：" << m_age << "，学号:" << m_no
       << endl;
}

Clock::Clock(time_t t) {
  tm *l_t = localtime(&t);
  m_hour = l_t->tm_hour;
  m_min = l_t->tm_min;
  m_sec = l_t->tm_sec;
}

Clock::Clock(void) {
  m_hour = 0;
  m_min = 0;
  m_sec = 0;
}

void Clock::run() {
  while (1) {
    printf("\r%02d:%02d:%02d", m_hour, m_min, m_sec);
    fflush(stdout);
    if (++m_sec == 60) {
      m_sec = 0;
      if (++m_min == 60) {
        m_min = 0;
        if (++m_hour == 24) {
          m_hour = 0;
        }
      }
    }
    sleep(1);
  }
}

class Complex {
public:
  Complex(int r, int i) {
    m_r = r;
    m_i = i;
  }
  Complex(const Complex &c) {
    m_r = c.m_r;
    m_i = c.m_i;
    cout << "copy construct" << endl;
  }
  ~Complex() {
    this->print();
    cout << "destory object" << endl;
  }
  void print() { cout << m_r << "+" << m_i << "i" << endl; }
  int m_r;
  int m_i;
};

int main() {

  Complex *c1 = new Complex(1, 2);
  c1->print();
  Complex c2 = *c1;
  c2.print();
  delete c1;
  return 0;
}
