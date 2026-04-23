#include <iostream>
#include <vector>
#include <tabulate/table.hpp>
#include <xlnt/xlnt.hpp>

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
    Worker() {}

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
        ws.cell("D" + to_string(r)).value(workers[i].getSalary());
        r++;
    }

    wb.save("workers.xlsx");
}

/* ================= SORT ================= */
void sortByIdAsc(vector<Worker> &w)
{
    for (int i = 0; i < w.size(); i++)
        for (int j = i + 1; j < w.size(); j++)
            if (w[i].getId() > w[j].getId())
                swap(w[i], w[j]);
}

void sortSalaryLowHigh(vector<Worker> &w)
{
    for (int i = 0; i < w.size(); i++)
        for (int j = i + 1; j < w.size(); j++)
            if (w[i].getSalary() > w[j].getSalary())
                swap(w[i], w[j]);
}

void sortSalaryHighLow(vector<Worker> &w)
{
    for (int i = 0; i < w.size(); i++)
        for (int j = i + 1; j < w.size(); j++)
            if (w[i].getSalary() < w[j].getSalary())
                swap(w[i], w[j]);
}

/* ================= TABLE ================= */
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
        t.add_row({
            to_string(list[i].getId()),
            list[i].getName(),
            to_string(list[i].getAge()),
            to_string((int)list[i].getSalary()) + "$"
        });
    }

    cout << t << endl;
}

/* ================= SEARCH ================= */
void showSearchTable(vector<Worker> result)
{
    if (result.size() == 0)
    {
        cout << "\n🔎 No data found!\n";
        return;
    }

    Table t;
    t.add_row({"ID", "Name", "Age", "Salary"});

    for (int i = 0; i < result.size(); i++)
    {
        t.add_row({
            to_string(result[i].getId()),
            result[i].getName(),
            to_string(result[i].getAge()),
            to_string((int)result[i].getSalary()) + "$"
        });
    }

    cout << t << endl;
}

/* ================= LOGIN ================= */
bool loginAdmin()
{
    string u, p;

    cout << "\n🛡️ ADMIN LOGIN\n";
    cout << "Username: ";
    cin >> u;
    cout << "Password: ";
    cin >> p;

    if (u == "admin" && p == "123")
    {
        cout << "\n✅ Login successfully (Admin)\n";
        return true;
    }

    cout << "\n❌ Wrong username or password\n";
    return false;
}

bool loginWorker()
{
    string u, p;

    cout << "\n👷 WORKER LOGIN\n";
    cout << "Username: ";
    cin >> u;
    cout << "Password: ";
    cin >> p;

    if (u == "worker" && p == "321")
    {
        cout << "\n✅ Login successfully (Worker)\n";
        return true;
    }

    cout << "\n❌ Wrong username or password\n";
    return false;
}

/* ================= WORKER PAGE ================= */
void workerPage()
{
    cout << "\n====================================\n";
    cout << "👷 WORKER MODE (VIEW ONLY)\n";
    cout << "====================================\n";
}

/* ================= MENU ================= */
void menu()
{
    cout << "\n====================================\n";
    cout << "         WORKER DASHBOARD          \n";
    cout << "====================================\n";
    cout << " 1. Add Worker\n";
    cout << " 2. Update Worker\n";
    cout << " 3. Show All Workers\n";
    cout << " 4. Delete Worker\n";
    cout << " 5. Search Worker\n";
    cout << " 6. Exit\n";
    cout << "====================================\n";
    cout << " Choose: ";
}

/* ================= MAIN ================= */
int main()
{
    system("cls");
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    vector<Worker> workers;
    bool isAdmin = false;

    int role;
    bool loginSuccess = false;

    /* ================= LOGIN SYSTEM ================= */
    while (!loginSuccess)
    {
        cout << "\n====================================\n";
        cout << " 1. Admin Login 🛡️\n";
        cout << " 2. Worker Login 👷\n";
        cout << "====================================\n";
        cout << " Choose: ";
        cin >> role;

        if (role == 1)
        {
            if (loginAdmin())
            {
                isAdmin = true;
                loginSuccess = true;
            }
        }
        else if (role == 2)
        {
            if (loginWorker())
            {
                isAdmin = false;
                workerPage();
                loginSuccess = true;
            }
        }
        else
        {
            cout << "\n❌ Invalid option (choose 1 or 2)\n";
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
            cout << "\n➕ Added successfully!\n";
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
                    cout << "\n✏️ Updated successfully!\n";
                    found = 1;
                }
            }

            if (!found)
                cout << "\n❌ No data found!\n";

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
                sortByIdAsc(workers);
            else if (choice == 2)
                sortSalaryLowHigh(workers);
            else if (choice == 3)
                sortSalaryHighLow(workers);
            else
                cout << "\n❌ Invalid option, showing default\n";

            showTable(workers);
            break;
        }

        case 4:
        {
            int id, found = 0;
            cout << "Delete ID: ";
            cin >> id;

            for (int i = 0; i < workers.size(); i++)
            {
                if (workers[i].getId() == id)
                {
                    workers.erase(workers.begin() + i);
                    exportExcel(workers);
                    cout << "\n🗑️ Deleted successfully!\n";
                    found = 1;
                    break;
                }
            }

            if (!found)
                cout << "\n❌ No data found!\n";

            break;
        }

        case 5:
        {
            vector<Worker> result;
            int c;

            cout << "Search 1.By ID 2.By Name: ";
            cin >> c;

            if (c == 1)
            {
                int id;
                cin >> id;

                for (auto &w : workers)
                    if (w.getId() == id) result.push_back(w);
            }
            else
            {
                string n;
                cin >> n;

                for (auto &w : workers)
                    if (w.getName() == n) result.push_back(w);
            }

            showSearchTable(result);
            break;
        }

        case 6:
            cout << "\n❌ Exit\n";
            break;

        default:
            cout << "\n❌ Invalid option\n";
        }

    } while (op != 6);

    return 0;
}