#include <iostream>
#include <vector>
#include <windows.h>
#include <tabulate/table.hpp>
#include <xlnt/xlnt.hpp>
#include "User.hpp"

using namespace std;
using namespace tabulate;

/* ================= COLOR HELPER ================= */
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void setColor(int color)
{
    SetConsoleTextAttribute(hConsole, color);
}

void resetColor()
{
    SetConsoleTextAttribute(hConsole, 7);
}

/* ================= INPUT VALIDATOR ================= */
// Returns true if a valid integer was read, false if user typed letters/symbols
bool readInt(int &out)
{
    string input;
    cin >> input;
    for (char c : input)
    {
        if (!isdigit(c) && !(c == '-' && &c == &input[0]))
        {
            setColor(12);
            cout << "\n  ⚠️  Invalid input! Please enter a NUMBER, not a letter.\n";
            resetColor();
            cin.clear();
            return false;
        }
    }
    try
    {
        out = stoi(input);
        return true;
    }
    catch (...)
    {
        setColor(12);
        cout << "\n  ⚠️  Invalid input! Number out of range.\n";
        resetColor();
        return false;
    }
}

/* ================= WORKER CLASS ================= */
class Worker
{
private:
    int id;
    string name;
    int age;
    float salary;

public:
    Worker() : id(0), name(""), age(0), salary(0.0f) {}

    /* ================= Vichhet - Add worker ================= */
    void input()
    {
        setColor(11);
        cout << "\n+---------------------------+\n";
        cout << "|      ADD WORKER          |\n";
        cout << "+---------------------------+\n";
        resetColor();

        setColor(14);
        cout << "ID: ";
        resetColor();
        while (!readInt(id))
        {
            setColor(14);
            cout << "ID: ";
            resetColor();
        }

        setColor(14);
        cout << "Name: ";
        resetColor();
        cin >> name;

        setColor(14);
        cout << "Age: ";
        resetColor();
        while (!readInt(age))
        {
            setColor(14);
            cout << "Age: ";
            resetColor();
        }

        setColor(14);
        cout << "Salary: ";
        resetColor();
        {
            string s;
            while (true)
            {
                cin >> s;
                bool valid = true, hasDot = false;
                for (char c : s)
                {
                    if (c == '.' && !hasDot)
                    {
                        hasDot = true;
                        continue;
                    }
                    if (!isdigit(c))
                    {
                        valid = false;
                        break;
                    }
                }
                if (valid && !s.empty())
                {
                    salary = stof(s);
                    break;
                }
                setColor(12);
                cout << "\n  ⚠️  Invalid input! Please enter a NUMBER for salary.\n";
                resetColor();
                setColor(14);
                cout << "Salary: ";
                resetColor();
            }
        }
    }

    /* ================= Vichhet - Update worker ================= */
    void update()
    {
        setColor(13);
        cout << "\n+---------------------------+\n";
        cout << "|     UPDATE WORKER        |\n";
        cout << "+---------------------------+\n";
        resetColor();

        setColor(14);
        cout << "New Name: ";
        resetColor();
        cin >> name;

        setColor(14);
        cout << "New Age: ";
        resetColor();
        while (!readInt(age))
        {
            setColor(14);
            cout << "New Age: ";
            resetColor();
        }

        setColor(14);
        cout << "New Salary: ";
        resetColor();
        {
            string s;
            while (true)
            {
                cin >> s;
                bool valid = true, hasDot = false;
                for (char c : s)
                {
                    if (c == '.' && !hasDot)
                    {
                        hasDot = true;
                        continue;
                    }
                    if (!isdigit(c))
                    {
                        valid = false;
                        break;
                    }
                }
                if (valid && !s.empty())
                {
                    salary = stof(s);
                    break;
                }
                setColor(12);
                cout << "\n  ⚠️  Invalid input! Please enter a NUMBER for salary.\n";
                resetColor();
                setColor(14);
                cout << "New Salary: ";
                resetColor();
            }
        }
    }

    // Setter methods
    void setId(int i) { id = i; }
    void setName(string n) { name = n; }
    void setAge(int a) { age = a; }
    void setSalary(float s) { salary = s; }

    // Getter methods
    int getId() { return id; }
    string getName() { return name; }
    int getAge() { return age; }
    float getSalary() { return salary; }
};

/* ================= EXCEL EXPORT ================= */
void exportExcel(vector<Worker> &workers)
{
    xlnt::workbook wb;
    auto ws = wb.active_sheet();
    ws.title("Workers");

    ws.cell("A1").value("ID");
    ws.cell("B1").value("Name");
    ws.cell("C1").value("Age");
    ws.cell("D1").value("Salary");

    int r = 2;
    for (int i = 0; i < workers.size(); i++)
    {
        ws.cell("A" + to_string(r)).value(workers[i].getId());
        ws.cell("B" + to_string(r)).value(workers[i].getName());
        ws.cell("C" + to_string(r)).value(workers[i].getAge());
        ws.cell("D" + to_string(r)).value(to_string((int)workers[i].getSalary()) + "$");
        r++;
    }
    wb.save("workersdata.xlsx");
}

/* ================= SORT ================= */
void sortByIdAsc(vector<Worker> &w)
{
    for (int i = 0; i < w.size(); i++)
        for (int j = i + 1; j < w.size(); j++)
            if (w[i].getId() > w[j].getId())
                swap(w[i], w[j]);
}

/* ================= Dy sorint - Show data as table (salary-low-high) ================= */
void sortSalaryLowHigh(vector<Worker> &w)
{
    for (int i = 0; i < w.size(); i++)
        for (int j = i + 1; j < w.size(); j++)
            if (w[i].getSalary() > w[j].getSalary())
                swap(w[i], w[j]);
}

/* ================= Dy sorint - Show data as table (salary-high-low) ================= */
void sortSalaryHighLow(vector<Worker> &w)
{
    for (int i = 0; i < w.size(); i++)
        for (int j = i + 1; j < w.size(); j++)
            if (w[i].getSalary() < w[j].getSalary())
                swap(w[i], w[j]);
}

/* ================= TABLE ================= */
/* ================= Kun sokea - Show data as table (default) ================= */
void showTable(vector<Worker> &list)
{
    if (list.size() == 0)
    {
        setColor(12);
        cout << "\nNo worker data found!\n";
        resetColor();
        return;
    }

    Table t;
    t.add_row({"ID", "Name", "Age", "Salary"});
    for (int i = 0; i < list.size(); i++)
    {
        t.add_row({to_string(list[i].getId()),
                   list[i].getName(),
                   to_string(list[i].getAge()),
                   to_string((int)list[i].getSalary()) + "$"});
    }

    setColor(11);
    cout << t << endl;
    resetColor();
}

/* ================= SEARCH ================= */
void showSearchTable(vector<Worker> result)
{
    if (result.size() == 0)
    {
        setColor(12);
        cout << "\n No data found!\n";
        resetColor();
        return;
    }

    Table t;
    t.add_row({"ID", "Name", "Age", "Salary"});
    for (int i = 0; i < result.size(); i++)
    {
        t.add_row({to_string(result[i].getId()),
                   result[i].getName(),
                   to_string(result[i].getAge()),
                   to_string((int)result[i].getSalary()) + "$"});
    }

    setColor(10);
    cout << t << endl;
    resetColor();
}

/* ================= REGISTRATION ================= */
/* ================= Sovannara - Register ================= */
void registerNewUser()
{
    string username, password, roleInput, role;

    setColor(13);
    cout << "\n+---------------------------+\n";
    cout << "|      REGISTRATION        |\n";
    cout << "+---------------------------+\n";
    resetColor();

    setColor(14);
    cout << "Username: ";
    resetColor();
    cin >> username;
    setColor(14);
    cout << "Password: ";
    resetColor();
    cin >> password;

    setColor(14);
    cout << "Role: \n1. Admin  \n2. Worker ";
    resetColor();
    cin >> roleInput;

    if (roleInput == "1" || roleInput == "admin")
        role = "admin";
    else if (roleInput == "2" || roleInput == "worker")
        role = "worker";
    else
    {
        setColor(12);
        cout << "\n  ⚠️  Invalid role selection! Please enter 1 or 2.\n";
        resetColor();
        return;
    }

    UserManager::registerUser(username, password, role);
}

/* ================= LOGIN(UNIFIED LOGIN)  ================= */
/* ================= Phanet - Login ================= */
pair<bool, string> login()
{
    string u, p;

    setColor(9);
    cout << "\n LOGIN\n";
    resetColor();

    setColor(14);
    cout << "Username: ";
    resetColor();
    cin >> u;
    setColor(14);
    cout << "Password: ";
    resetColor();
    cin >> p;

    string role = UserManager::loginUser(u, p);

    if (!role.empty())
    {
        setColor(10);
        if (role == "admin")
            cout << "\n Login successfully (Admin)\n";
        else if (role == "worker")
            cout << "\n Login successfully (Worker)\n";
        resetColor();
        return {true, role};
    }

    setColor(12);
    cout << "\n Error ! Wrong username or Password ! Try login again later\n";
    resetColor();
    return {false, ""};
}

/* ================= WORKER PAGE ================= */
void workerPage()
{
    setColor(6);
    cout << "\n====================================\n";
    cout << " WORKER MODE (VIEW ONLY)\n";
    cout << "====================================\n";
    resetColor();
}

/* ================= LOAD WORKERS FROM EXCEL ================= */
#include <xlnt/xlnt.hpp>

void loadWorkersFromExcel(vector<Worker> &workers)
{
    workers.clear();

    xlnt::workbook wb;
    try
    {
        wb.load("workersdata.xlsx");
    }
    catch (const std::exception &e)
    {
        //std::cerr << "Failed to load Excel file: " << e.what() << std::endl;
        return;
    }

    auto ws = wb.active_sheet();
    bool firstRow = true;
    for (auto row : ws.rows(false))
    {
        if (firstRow)
        {
            firstRow = false;
            continue;
        }
        try
        {
            int id = row[0].value<int>();
            std::string name = row[1].to_string();
            int age = row[2].value<int>();
            std::string salaryStr = row[3].to_string();
            size_t dollarPos = salaryStr.find('$');
            if (dollarPos != std::string::npos)
                salaryStr = salaryStr.substr(0, dollarPos);
            float salary = std::stof(salaryStr);

            Worker w;
            w.setId(id);
            w.setName(name);
            w.setAge(age);
            w.setSalary(salary);
            workers.push_back(w);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Row parsing error: " << e.what() << std::endl;
            continue;
        }
    }
}

/* ================= MENU ================= */
void menu()
{
    setColor(9);
    cout << "\n====================================\n";
    setColor(15);
    cout << "         WORKER DASHBOARD          \n";
    setColor(9);
    cout << "====================================\n";
    resetColor();

    setColor(10);
    cout << " 1. ➕ Add Worker\n";
    setColor(13);
    cout << " 2. 📝 Update Worker\n";
    setColor(11);
    cout << " 3. 📃 Show All Workers\n";
    setColor(12);
    cout << " 4. 🗑️  Delete Worker\n";
    setColor(14);
    cout << " 5. 🔎 Search Worker\n";
    setColor(1);
    cout << " 6. 🚪 Logout\n";
    setColor(4);
    cout << " 7. ❌ Exit\n";
    setColor(9);
    cout << "====================================\n";
    resetColor();

    setColor(15);
    cout << " Choose: ";
    resetColor();
}

/* ================= MAIN ================= */
int main()
{
    system("cls");
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    vector<Worker> workers;
    bool runProgram = true;

    while (runProgram)
    {
        bool isAdmin = false;
        int option;
        bool loginSuccess = false;

        /* ================= LOGIN SYSTEM ================= */
        while (!loginSuccess)
        {
            setColor(9);
            cout << "\n====================================\n";
            setColor(15);
            cout << "       USER AUTHENTICATION          \n";
            setColor(9);
            cout << "====================================\n";
            resetColor();

            setColor(10);
            cout << " 1. Register 📝\n";
            setColor(11);
            cout << " 2. Login 🔐\n";
            setColor(12);
            cout << " 3. Exit ❌\n";
            setColor(9);
            cout << "====================================\n";
            resetColor();

            setColor(15);
            cout << " Choose: ";
            resetColor();

            if (!readInt(option))
                continue; // ← catches letters here

            if (option == 1)
            {
                registerNewUser();
            }
            else if (option == 2)
            {
                auto [success, role] = login();
                if (success)
                {
                    if (role == "admin")
                        isAdmin = true;
                    else
                    {
                        isAdmin = false;
                        workerPage();
                    }
                    loadWorkersFromExcel(workers);
                    loginSuccess = true;
                }
            }
            else if (option == 3)
            {
                setColor(6);
                cout << "\n 👋 Goodbye!\n";
                resetColor();
                runProgram = false;
                loginSuccess = true;
            }
            else
            {
                setColor(12);
                cout << "\n  ⚠️  Invalid option! Please choose 1, 2, or 3.\n";
                resetColor();
            }
        }

        int op;

        /* ================= MENU LOOP ================= */
        do
        {
            menu();

            if (!readInt(op))
            {
                op = -1;
                continue;
            } // ← catches letters here

            if (!isAdmin && (op == 1 || op == 2 || op == 4))
            {
                setColor(12);
                cout << "\n  🚫 Admin only feature!\n";
                resetColor();
                continue;
            }

            switch (op)
            {
            case 1:
            {
                Worker w;
                w.input();
                bool duplicate = false;
                for (int i = 0; i < workers.size(); i++)
                {
                    if (workers[i].getId() == w.getId() || workers[i].getName() == w.getName())
                    {
                        duplicate = true;
                        break;
                    }
                }
                if (duplicate)
                {
                    setColor(12);
                    cout << "\n  ❌ Failed to add: Worker with the same ID or Name already exists!\n";
                    resetColor();
                }
                else
                {
                    workers.push_back(w);
                    exportExcel(workers);
                    setColor(10);
                    cout << "\n  ✅ Added successfully!\n";
                    cout << "  ID: " << w.getId() << " | Name: " << w.getName()
                         << " | Age: " << w.getAge() << " | Salary: " << (int)w.getSalary() << "$\n";
                    resetColor();
                }
                break;
            }

            case 2:
            {
                int id, found = 0;
                setColor(14);
                cout << "Enter ID: ";
                resetColor();
                if (!readInt(id))
                    break; // ← catches letters here

                for (int i = 0; i < workers.size(); i++)
                {
                    if (workers[i].getId() == id)
                    {
                        workers[i].update();
                        exportExcel(workers);
                        setColor(10);
                        cout << "\n  ✅ Updated successfully!\n";
                        resetColor();
                        found = 1;
                    }
                }
                if (!found)
                {
                    setColor(12);
                    cout << "\n  ❌ No data found!\n";
                    resetColor();
                }
                break;
            }

            case 3:
            {
                int choice;

                setColor(11);
                cout << "\n====================================\n";
                cout << " SHOW ALL WORKERS\n";
                cout << "====================================\n";
                resetColor();

                setColor(7);
                cout << " 1. Default (By ID)\n";
                setColor(10);
                cout << " 2. Salary Low to High\n";
                setColor(12);
                cout << " 3. Salary High to Low\n";
                setColor(11);
                cout << "====================================\n";
                resetColor();

                setColor(15);
                cout << " Choose: ";
                resetColor();

                if (!readInt(choice))
                    break; // ← catches letters here

                if (choice == 1)
                    sortByIdAsc(workers);
                else if (choice == 2)
                    sortSalaryLowHigh(workers);
                else if (choice == 3)
                    sortSalaryHighLow(workers);
                else
                {
                    setColor(12);
                    cout << "\n  ⚠️  Invalid option, showing default.\n";
                    resetColor();
                    sortByIdAsc(workers);
                }

                showTable(workers);
                break;
            }

            case 4:
            {
                /* ================= Dararithy - Delete Worker ================= */
                int id, found = 0;
                setColor(12);
                cout << "Delete ID: ";
                resetColor();
                if (!readInt(id))
                    break; // ← catches letters here

                for (int i = 0; i < workers.size(); i++)
                {
                    if (workers[i].getId() == id)
                    {
                        workers.erase(workers.begin() + i);
                        exportExcel(workers);
                        setColor(10);
                        cout << "\n  ✅ Deleted successfully!\n";
                        resetColor();
                        found = 1;
                        break;
                    }
                }
                if (!found)
                {
                    setColor(12);
                    cout << "\n  ❌ No data found!\n";
                    resetColor();
                }
                break;
            }

            case 5:
            {
                /* ================= Sethapiseth - Search workers ================= */
                vector<Worker> result;
                int c;

                setColor(14);
                cout << "Search \n1.By ID \n2.By Name \n3.By Age: ";
                resetColor();
                if (!readInt(c))
                    break; // ← catches letters here

                if (c == 1)
                {
                    int id;
                    setColor(14);
                    cout << "Enter ID: ";
                    resetColor();
                    if (!readInt(id))
                        break; // ← catches letters here
                    for (auto &w : workers)
                        if (w.getId() == id)
                            result.push_back(w);
                }
                else if (c == 2)
                {
                    string n;
                    setColor(14);
                    cout << "Enter Name: ";
                    resetColor();
                    cin >> n;
                    for (auto &w : workers)
                        if (w.getName() == n)
                            result.push_back(w);
                }
                else if (c == 3)
                {
                    int age;
                    setColor(14);
                    cout << "Enter Age: ";
                    resetColor();
                    if (!readInt(age))
                        break; // ← catches letters here
                    for (auto &w : workers)
                        if (w.getAge() == age)
                            result.push_back(w);
                }
                else
                {
                    setColor(12);
                    cout << "\n  ⚠️  Invalid search option!\n";
                    resetColor();
                    break;
                }

                showSearchTable(result);
                break;
            }

            case 6:
            {
                /* ================= Phanet - Logout ================= */
                setColor(10);
                cout << "\n  ✅ Logout successful! Returning to login page...\n";
                resetColor();
                break;
            }

            case 7:
            {
                setColor(6);
                cout << "\n 👋 Goodbye!\n";
                resetColor();
                runProgram = false;
                break;
            }

            default:
                setColor(12);
                cout << "\n  ⚠️  Invalid option! Please enter a number from the menu.\n";
                resetColor();
            }

        } while (op != 6 && op != 7);
    }

    return 0;
}