#pragma once
#include <string>

using std::string;

// ===== STUDENT CLASS =====
// Represents a student with a name and age
class Student {
private:
    string name;  // Student's full name
    int age;      // Student's age in years

public:
    // Constructor: create a student with name and age
    Student(string name, int age);
    
    // Getter methods - retrieve student information
    string getName() const;   // Get student's name
    int getAge() const;       // Get student's age
    
    // Setter methods - modify student information
    void setName(string name);  // Change student's name
    void setAge(int age);       // Change student's age
};
