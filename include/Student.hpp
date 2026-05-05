#pragma once
#include <string>

using std::string;

// ===== STUDENT CLASS =====
// Represents a student with a name and age
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
