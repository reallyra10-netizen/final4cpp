#pragma once
#include <string>
using std::string; 
class Student {
private:
    string name;
    int age;

public:
    Student(string name, int age);
    string getName() const;
    int getAge() const;
    void setName(string name);
    void setAge(int age);
};
