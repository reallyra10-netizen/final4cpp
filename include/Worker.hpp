#pragma once
#include <string>

using std::string;

// worker 
class Worker {
private:
    int    id;
    string name;
    int    age;
    float  salary;
    string gender;
    string position;
    string department;
    string dateHired;

public:
    Worker();

    // input / update
    void input();
    void update();

    // setters
    void setId(int i);
    void setName(string n);
    void setAge(int a);
    void setSalary(float s);
    void setGender(string g);
    void setPosition(string p);
    void setDepartment(string d);
    void setDateHired(string h);

    // getters
    int    getId()         const;
    string getName()       const;
    int    getAge()        const;
    float  getSalary()     const;
    string getGender()     const;
    string getPosition()   const;
    string getDepartment() const;
    string getDateHired()  const;
};
