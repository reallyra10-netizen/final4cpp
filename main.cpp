#include <iostream>
#include <vector>
#include <windows.h>
#include <tabulate/table.hpp>
#include <xlnt/xlnt.hpp>
#include "User.hpp"

using namespace std;
using namespace tabulate;

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

    // Vichhet
    void input()
    {
        cout << "\n+---------------------------+\n";
        cout << "|      ADD WORKER          |\n";
        cout << "+---------------------------+\n";

        cout << "ID: ";
        cin >> id;
        cout << "Name: ";
        cin >> name;
        cout << "Age: ";
        cin >> age;
        cout << "Salary: ";
        cin >> salary;
    }

    // Vichhet
    void update()
    {
        cout << "\n+---------------------------+\n";
        cout << "|     UPDATE WORKER        |\n";
        cout << "+---------------------------+\n";

        cout << "New Name: ";
        cin >> name;
        cout << "New Age: ";
        cin >> age;
        cout << "New Salary: ";
        cin >> salary;
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
    {
        for (int j = i + 1; j < w.size(); j++)
        {
            if (w[i].getId() > w[j].getId())
            {
                swap(w[i], w[j]);
            }
        }
    }
}

// Dy sorint
void sortSalaryLowHigh(vector<Worker> &w)
{
    for (int i = 0; i < w.size(); i++)
    {
        for (int j = i + 1; j < w.size(); j++)
        {
            if (w[i].getSalary() > w[j].getSalary())
            {
                swap(w[i], w[j]);
            }
        }
    }
}

// Dy sorint
void sortSalaryHighLow(vector<Worker> &w)
{
    for (int i = 0; i < w.size(); i++)
    {
        for (int j = i + 1; j < w.size(); j++)
        {
            if (w[i].getSalary() < w[j].getSalary())
            {
                swap(w[i], w[j]);
            }
        }
    }
}

/* ================= TABLE ================= */
// Kun sokea
void showTable(vector<Worker> &list)
{
    if (list.size() == 0)
    {
        cout << "\nℹ️ No worker data found!\n";
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

    cout << t << endl;
}

/* ================= SEARCH ================= */
void showSearchTable(vector<Worker> result)
{
    if (result.size() == 0)
    {
        cout << "\n❌ No data found!\n";
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

    cout << t << endl;
}

/* ================= REGISTRATION ================= */
// Sovannara
void registerNewUser()
{
    string username, password, roleInput, role;

    cout << "\n+---------------------------+\n";
    cout << "|      REGISTRATION        |\n";
    cout << "+---------------------------+\n";

    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    cout << "Role: \n1. 🔐 Admin  \n2. 👷Worker ";
    cin >> roleInput;

    if (roleInput == "1" || roleInput == "admin")
    {
        role = "admin";
    }
    else if (roleInput == "2" || roleInput == "worker")
    {
        role = "worker";
    }
    else
    {
        cout << "\n❌ Invalid role selection!\n";
        return;
    }

    UserManager::registerUser(username, password, role);
}

/* ================= LOGIN ================= */
/* ================= UNIFIED LOGIN ================= */
// Phanet
pair<bool, string> login()
{
    string u, p;

    cout << "\n🔐 LOGIN\n";
    cout << "Username: ";
    cin >> u;
    cout << "Password: ";
    cin >> p;

    string role = UserManager::loginUser(u, p);

    if (!role.empty())
    {
        if (role == "admin")
        {
            cout << "\n✅ Login successfully (Admin)\n";
        }
        else if (role == "worker")
        {
            cout << "\n✅ Login successfully (Worker)\n";
        }
        return {true, role};
    }

    cout << "\n❌ Error ! Wrong username or Password ! Try login again later\n";
    return {false, ""};
}

/* ================= WORKER PAGE ================= */
void workerPage()
{
    cout << "\n====================================\n";
    cout << "👷 WORKER MODE (VIEW ONLY)\n";
    cout << "====================================\n";
}

/* ================= LOAD WORKERS FROM EXCEL ================= */
void loadWorkersFromExcel(vector<Worker> &workers)
{
    xlnt::workbook wb;
    try
    {
        wb.load("workersdata.xlsx");
    }
    catch (...)
    {
        return;
    }

    auto ws = wb.active_sheet();
    int loadedCount = 0;

    for (auto row : ws.rows(false))
    {
        // Skip header row
        if (row[0].to_string() == "ID")
            continue;

        try
        {
            int id = stoi(row[0].to_string());
            string name = row[1].to_string();
            int age = stoi(row[2].to_string());

            // Remove "$" from salary if present
            string salaryStr = row[3].to_string();
            size_t dollarPos = salaryStr.find('$');
            if (dollarPos != string::npos)
            {
                salaryStr = salaryStr.substr(0, dollarPos);
            }
            float salary = stof(salaryStr);

            // Create worker and add to vector
            Worker w;
            w.setId(id);
            w.setName(name);
            w.setAge(age);
            w.setSalary(salary);
            workers.push_back(w);
            loadedCount++;
        }
        catch (...)
        {
            // Skip rows that can't be parsed
            continue;
        }
    }
}

/* ================= MENU ================= */
void menu()
{
    cout << "\n====================================\n";
    cout << "         WORKER DASHBOARD          \n";
    cout << "====================================\n";
    cout << " 1. ➕ Add Worker\n";
    cout << " 2. 📝 Update Worker\n";
    cout << " 3. 📃 Show All Workers\n";
    cout << " 4. 🗑️ Delete Worker\n";
    cout << " 5. 🔎 Search Worker\n";
    cout << " 6. 🚪 Logout\n";
    cout << " 7. ❌Exit\n";
    cout << "====================================\n";
    cout << " Choose: ";
}

/* ================= MAIN ================= */
int main()
{
    system("cls");
    // show emoji configuration for windows console
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
            cout << "\n====================================\n";
            cout << "       USER AUTHENTICATION          \n";
            cout << "====================================\n";
            cout << " 1. Register 📝\n";
            cout << " 2. Login 🔐\n";
            cout << " 3. Exit ❌\n";
            cout << "====================================\n";
            cout << " Choose: ";
            cin >> option;

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
                    {
                        isAdmin = true;
                    }
                    else
                    {
                        isAdmin = false;
                        workerPage();
                    }
                    // Load persisted workers from Excel file
                    loadWorkersFromExcel(workers);
                    loginSuccess = true;
                }
            }
            else if (option == 3)
            {
                cout << "\n👋 Goodbye!\n";
                runProgram = false;
                loginSuccess = true;
            }
            else
            {
                cout << "\n❌ Invalid option (choose 1, 2, or 3)\n";
            }
        }

        int op;

        /* ================= MENU LOOP ================= */
        do
        {
            menu();
            cin >> op;

            if (!isAdmin && (op == 1 || op == 2 || op == 4))
            {
                cout << "\n❌ Admin only feature!\n";
                continue;
            }

            switch (op)
            {
            case 1:
            {
                Worker w;
                w.input();
                workers.push_back(w);
                exportExcel(workers);
                cout << "\n✅ Added successfully!\n";
                cout << "ID: " << w.getId() << " | Name: " << w.getName()
                     << " | Age: " << w.getAge() << " | Salary: " << (int)w.getSalary() << "$\n";
                break;
            }

            case 2:
            {
                int id, found = 0;
                cout << "Enter ID: ";
                cin >> id;

                for (int i = 0; i < workers.size(); i++)
                {
                    if (workers[i].getId() == id)
                    {
                        workers[i].update();
                        exportExcel(workers);
                        cout << "\n✅ Updated successfully!\n";
                        found = 1;
                    }
                }

                if (!found)
                {
                    cout << "\n❌ No data found!\n";
                }

                break;
            }

            case 3:
            {
                int choice;

                cout << "\n====================================\n";
                cout << " SHOW ALL WORKERS\n";
                cout << "====================================\n";
                cout << " 1. Default (By ID)\n";
                cout << " 2. Salary Low → High\n";
                cout << " 3. Salary High → Low\n";
                cout << "====================================\n";
                cout << " Choose: ";
                cin >> choice;

                if (choice == 1)
                {
                    sortByIdAsc(workers);
                }
                else if (choice == 2)
                {
                    sortSalaryLowHigh(workers);
                }
                else if (choice == 3)
                {
                    sortSalaryHighLow(workers);
                }
                else
                {
                    cout << "\n❌ Invalid option, showing default\n";
                }

                showTable(workers);
                break;
            }

            case 4:
            {
                // Dararithy
                int id, found = 0;
                cout << "Delete ID: ";
                cin >> id;

                for (int i = 0; i < workers.size(); i++)
                {
                    if (workers[i].getId() == id)
                    {
                        workers.erase(workers.begin() + i);
                        exportExcel(workers);
                        cout << "\n✅ Deleted successfully!\n";
                        found = 1;
                        break;
                    }
                }

                if (!found)
                {
                    cout << "\n❌ No data found!\n";
                }

                break;
            }

            case 5:
            {
                // Sethapiseth
                vector<Worker> result;
                int c;

                cout << "Search \n1.By ID \n2.By Name \n3.By Age: ";
                cin >> c;

                if (c == 1)
                {
                    int id;
                    cin >> id;

                    for (auto &w : workers)
                    {
                        if (w.getId() == id)
                        {
                            result.push_back(w);
                        }
                    }
                }
                else if (c == 2)
                {
                    string n;
                    cin >> n;

                    for (auto &w : workers)
                    {
                        if (w.getName() == n)
                        {
                            result.push_back(w);
                        }
                    }
                }
                else if (c == 3)
                {
                    int age;
                    cin >> age;

                    for (auto &w : workers)
                    {
                        if (w.getAge() == age)
                        {
                            result.push_back(w);
                        }
                    }
                }

                showSearchTable(result);
                break;
            }

            case 6:
            {
                // Phanet
                cout << "\n✅ Logout successful! Returning to login page...\n";
                break;
            }

            case 7:
            {
                cout << "\n👋 Goodbye!\n";
                runProgram = false;
                break;
            }

            default:
                cout << "\n❌ Invalid option\n";
            }

        } while (op != 6 && op != 7);
    }

    return 0;
}