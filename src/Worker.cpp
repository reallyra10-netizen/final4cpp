#include "Worker.hpp"
#include "ConsoleUtils.hpp"
#include <iostream>
#include <vector>

using namespace std;

Worker::Worker()
    : id(0), name(""), age(0), salary(0.0f),
      gender(""), position(""), department(""), dateHired("") {}

//  salary 
static float askSalary(const string &label)
{
    while (true)
    {
        setColor(14);
        cout << label;
        resetColor();
        string s;
        cin >> s;
        bool ok = true, dot = false;
        for (char c : s)
        {
            if (c == '.' && !dot)
            {
                dot = true;
                continue;
            }
            if (!isdigit(c))
            {
                ok = false;
                break;
            }
        }
        if (ok && !s.empty())
        {
            float val = stof(s);
            // salary range 
            if (val < 100.0f)
            {
                animError("salary must be at least $100.");
                continue;
            }
            if (val > 100000.0f)
            {
                animError("salary cannot exceed $100,000.");
                continue;
            }
            return val;
        }
        animError("invalid salary. enter a positive number.");
    }
}

//  gender 
static string askGender(const string &label)
{
    while (true)
    {
        setColor(14);
        cout << label;
        resetColor();
        string g;
        cin >> g;
        
        for (char &c : g)
            c = tolower(c);
        if (g == "m" || g == "male")
            return "Male";
        if (g == "f" || g == "female")
            return "Female";
        animError("invalid gender. enter Male/M or Female/F.");
    }
}

//  date
static string askDate(const string &label)
{
    while (true)
    {
        setColor(14);
        cout << label;
        resetColor();
        string d;
        cin >> d;
        // expect
        bool ok = (d.size() == 10 &&
                   isdigit(d[0]) && isdigit(d[1]) && d[2] == '/' &&
                   isdigit(d[3]) && isdigit(d[4]) && d[5] == '/' &&
                   isdigit(d[6]) && isdigit(d[7]) && isdigit(d[8]) && isdigit(d[9]));
        if (ok)
            return d;
        animError("invalid date. use format DD/MM/YYYY.");
    }
}

static string askName(const string &label)
{
    while (true)
    {
        setColor(14);
        cout << label;
        resetColor();
        string n;
        cin >> ws;
        getline(cin, n);

        // trim leading/trailing whitespace
        size_t start = n.find_first_not_of(' ');
        size_t end = n.find_last_not_of(' ');
        if (start == string::npos)
        {
            animError("name cannot be empty.");
            continue;
        }
        n = n.substr(start, end - start + 1);

        bool ok = true;
        for (char c : n)
        {
            if (isdigit(static_cast<unsigned char>(c)))
            {
                ok = false;
                break;
            }
            if (!isalpha(static_cast<unsigned char>(c)) && c != ' ' && c != '-' && c != '\'')
            {
                ok = false;
                break;
            }
        }

        if (ok && !n.empty())
            return n;

        animError("invalid name. use letters only, no numbers.");
    }
}

static string askOption(const string &label, const vector<string> &options)
{
    while (true)
    {
        setColor(14);
        cout << "\n  " << label << ":\n";
        resetColor();

        for (size_t i = 0; i < options.size(); ++i)
            cout << "    [" << (i + 1) << "] " << options[i] << "\n";

        cout << "    [4] Custom\n";
        setColor(14);
        cout << "  Choose an option: ";
        resetColor();

        int choice;
        if (!readInt(choice))
            continue;

        if (choice == 0)
        {
            setColor(14);
            cout << "  Enter " << label << ": ";
            resetColor();
            string value;
            cin >> value;
            return value;
        }

        if (choice >= 1 && choice <= static_cast<int>(options.size()))
            return options[choice - 1];

        animError("invalid selection. choose a valid option.");
    }
}

static string askPosition()
{
    static const vector<string> positions = {"Manager", "Engineer", "Clerk"};
    return askOption("Position", positions);
}

static string askDepartment()
{
    static const vector<string> departments = {"HR", "Sales", "IT"};
    return askOption("Department", departments);
}

// input (add)
void Worker::input()
{
    // id
    setColor(14);
    cout << "\n  ID         : ";
    resetColor();
    while (!readInt(id))
    {
        setColor(14);
        cout << "\n  ID         : ";
        resetColor();
    }

    // name
    name = askName("\n  Name       : ");

    // range age
    while (true)
    {
        setColor(14);
        cout << "\n  Age        : ";
        resetColor();
        if (readInt(age))
        {
            if (age < 18)
            {
                animError("age must be at least 18.");
                continue;
            }
            if (age > 65)
            {
                animError("age cannot exceed 65.");
                continue;
            }
            break;
        }
    }

    salary = askSalary("\n  Salary     : ");
    gender = askGender("\n  Gender (Male/Female) : ");
    position = askPosition();
    department = askDepartment();
    dateHired = askDate("\n  Date Hired (DD/MM/YYYY) : ");
}

// update worker data
void Worker::update()
{
    name = askName("\n  Name       : ");
    department = askDepartment();
    dateHired = askDate("\n  Date Hired (DD/MM/YYYY) : ");
}

// setters
void Worker::setId(int i) { id = i; }
void Worker::setName(string n) { name = n; }
void Worker::setAge(int a) { age = a; }
void Worker::setSalary(float s) { salary = s; }
void Worker::setGender(string g) { gender = g; }
void Worker::setPosition(string p) { position = p; }
void Worker::setDepartment(string d) { department = d; }
void Worker::setDateHired(string h) { dateHired = h; }

// getters
int Worker::getId() const { return id; }
string Worker::getName() const { return name; }
int Worker::getAge() const { return age; }
float Worker::getSalary() const { return salary; }
string Worker::getGender() const { return gender; }
string Worker::getPosition() const { return position; }
string Worker::getDepartment() const { return department; }
string Worker::getDateHired() const { return dateHired; }
