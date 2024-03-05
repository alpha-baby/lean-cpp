#ifndef __STUDENT_H
#define __STUDENT_H

#include "iostream"

using namespace std;

class Clock {
public:
  Clock(time_t t);

  Clock(void);

  void run();
  int m_hour;
  int m_min;
  int m_sec;
};

class Student {
public:
  Student(const string name, int age, int no);
  Student(const Student &s);

  void who();

  string m_name;
  int m_age;
  int m_no;
  Clock m_c;
};

#endif // __STUDENT_H