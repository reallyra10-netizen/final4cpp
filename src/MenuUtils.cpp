#include "MenuUtils.hpp"
#include "ConsoleUtils.hpp"
#include "User.hpp"
#include "ExcelUtils.hpp"
#include <tabulate/table.hpp>
#include <iostream>
#include <sstream>

using namespace std;
using namespace tabulate;

// sort id
void sortId(vector<Worker> &w)
{
    for (int i = 0; i < (int)w.size(); i++)
        for (int j = i + 1; j < (int)w.size(); j++)
            if (w[i].getId() > w[j].getId())
                swap(w[i], w[j]);
}

// salary low to high
void sortLow(vector<Worker> &w)
{
    for (int i = 0; i < (int)w.size(); i++)
        for (int j = i + 1; j < (int)w.size(); j++)
            if (w[i].getSalary() > w[j].getSalary())
                swap(w[i], w[j]);
}

//  high to low
void sortHigh(vector<Worker> &w)
{
    for (int i = 0; i < (int)w.size(); i++)
        for (int j = i + 1; j < (int)w.size(); j++)
            if (w[i].getSalary() < w[j].getSalary())
                swap(w[i], w[j]);
}

// Show table
static void printTable(vector<Worker> &list, int headerColor)
{
    if (list.empty())
    {
        animError("no worker data found!");
        return;
    }

    Table t;
    t.add_row({"ID", "Name", "Age", "Salary", "Gender", "Position", "Department", "Date Hired"});
    t[0].format().font_color(headerColor == 10 ? Color::green : Color::cyan).font_style({FontStyle::bold});

    for (auto &w : list)
        t.add_row({to_string(w.getId()),
                   w.getName(),
                   to_string(w.getAge()),
                   to_string((int)w.getSalary()) + "$",
                   w.getGender(),
                   w.getPosition(),
                   w.getDepartment(),
                   w.getDateHired()});

    cout << std::endl;
    ostringstream oss;
    oss << t;
    istringstream ss(oss.str());
    string line;
    int ln = 0;
    while (getline(ss, line))
    {
        setColor(ln == 1 ? 14 : (ln % 2 == 0 ? 7 : 15));
        cout << "  " << line << std::endl;
        resetColor();
        ln++;
    }
    cout << std::endl;
}

// show all workers
void showTable(vector<Worker> &list, int borderColor)
{
    printTable(list, borderColor);
}

// print data in search 
void showSearchTable(vector<Worker> result)
{
    printTable(result, 10);
}

void welcomeScreen()
{
    cls();
    hideCursor();

    setColor(9);
    for (int i = 0; i < 72; i++)
        cout << "═";
    cout << std::endl;
    resetColor();

    const string logo[] = {
        "",
        "     __        ______  ____  _  _______ ____  ",
        "     \\ \\      / / _ \\|  _ \\| |/ / ____|  _ \\ ",
        "      \\ \\ /\\ / / | | | |_) | ' /|  _| | |_) |",
        "       \\ V  V /| |_| |  _ <| . \\| |___|  _ < ",
        "        \\_/\\_/  \\___/|_| \\_\\_|\\_\\_____|_| \\_\\",
        "",
        "              M A N A G E M E N T   S Y S T E M",
        "",
    };
    int lc[] = {0, 11, 11, 14, 14, 9, 0, 15, 0};
    for (int i = 0; i < 9; i++)
        typeText(logo[i] + "\n", lc[i], 4);

    cout << std::endl;

    setColor(8);
    cout << "  ";
    resetColor();
    typeText(" Institute of Science and Technology Advanced Development | Group 1\n", 7, 10);

    cout << std::endl;

    setColor(9);
    for (int i = 0; i < 72; i++)
        cout << "═";
    cout << std::endl
         << std::endl;
    resetColor();

    setColor(11);
    cout << "  Loading ";
    resetColor();
    for (int i = 0; i < 18; i++)
    {
        setColor(i % 2 == 0 ? 10 : 14);
        cout << "▓";
        cout.flush();
        ms(30);
    }
    cout << std::endl
         << std::endl;

    progressBar(10, 28, 10);

    cout << std::endl;
    typeText("  >> Press ENTER to start ...", 15, 8);
    resetColor();
    cout << std::endl;
    showCursor();
    cin.get();
    cls();
}

//exit style
void goodbyeScreen()
{
    cls();
    pageHeader("GOODBYE", 6, 15);
    rowEmpty(6);
    row("Thank you for using Worker Console!", 6, 14, true);
    row("Have a great day!", 6, 7, true);
    rowEmpty(6);
    drawBot(6);
    cout << std::endl;
    ms(700);
}

// auth menu
void printAuthMenu()
{
    cls();
    pageHeader("USER AUTHENTICATION", 9, 15);
    menuRow("", 9, 7);
    menuRow("[1]  Register", 9, 10);
    menuRow("[2]  Login", 9, 11);
    menuRow("[3]  Exit", 9, 12);
    menuRow("", 9, 7);
    drawBot(9);
    cout << std::endl;
    setColor(15);
    cout << "  >> Choose: ";
    resetColor();
}

static bool confirmYes(const string &prompt)
{
    setColor(14);
    cout << prompt;
    resetColor();
    string answer;
    cin >> answer;
    for (char &c : answer) c = tolower(c);
    return (answer == "y" || answer == "yes");
}

// register new user
void registerNewUser()
{
    cls();
    pageHeader("USER REGISTRATION", 13, 15);

    string u, p, ri, role;
    setColor(14);
    cout << "  Username : ";
    resetColor();
    cin >> u;

    while (true)
    {
        setColor(14);
        cout << "\n  Password : ";
        resetColor();
        p = readPassword();

        setColor(14);
        cout << "\n  Confirm Password : ";
        resetColor();
        string confirm = readPassword();

        if (p == confirm)
            break;

        animError("passwords do not match. try again.");
        cout << std::endl;
    }

    cout << std::endl;
    menuRow("[1]  Admin", 13, 10);
    menuRow("[2]  Worker", 13, 11);
    cout << std::endl;
    setColor(15);
    cout << "  >> Role: ";
    resetColor();
    cin >> ri;

    if (ri == "1" || ri == "admin")
        role = "admin";
    else if (ri == "2" || ri == "worker")
        role = "worker";
    else
    {
        animError("invalid role! enter 1 or 2.");
        waitEnter();
        return;
    }

    spinner("Creating account ...", 13, 400);
    bool ok = UserManager::registerUser(u, p, role);
    if (ok)
        animSuccess("account created!");
    else
        animError("account creation failed! username already exists.");
    ms(400);
    waitEnter();
}

// login
pair<bool, string> login()
{
    cls();
    pageHeader("LOGIN", 9, 15);

    string u, p;
    setColor(14);
    cout << "  Username : ";
    resetColor();
    cin >> u;
    setColor(14);
    cout << "\n  Password : ";
    resetColor();
    p = readPassword();

    spinner("Authenticating ...", 9, 400);
    string role = UserManager::loginUser(u, p);

    if (!role.empty())
    {
        animSuccess("welcome back! logged in as " + string(role == "admin" ? "ADMIN" : "WORKER"));
        ms(400);
        return {true, role};
    }
    animError("wrong username or password.");
    ms(400);
    waitEnter();
    return {false, ""};
}

// worker page
void workerPage()
{
    cls();
    pageHeader("WORKER MODE — VIEW ONLY", 6, 15);
    menuRow("", 6, 7);
    menuRow("You are logged in as:  WORKER", 6, 6);
    menuRow("View and Search are available to you.", 6, 7);
    menuRow("Add / Update / Delete require Admin.", 6, 8);
    menuRow("", 6, 7);
    drawBot(6);
    waitEnter();
}

// main menu
void printMainMenu(bool isAdmin)
{
    cls();
    pageHeader("WORKER DASHBOARD", 9, 15);

    if (isAdmin)
    {
        menuRow("[1]  Add Worker", 9, 10);
        menuRow("[2]  Update Worker", 9, 13);
        menuRow("[3]  Show All", 9, 11);
        menuRow("[4]  Delete Worker", 9, 12);
        menuRow("[5]  Search Worker", 9, 14);
        menuRow("[6]  Logout", 9, 1);
        menuRow("[7]  Exit", 9, 4);
    }
    else
    {
        menuRow("[1]  Add Worker     (Admin only)", 9, 8);
        menuRow("[2]  Update Worker  (Admin only)", 9, 8);
        menuRow("[3]  Show Worker Data", 9, 11);
        menuRow("[4]  Delete Worker  (Admin only)", 9, 8);
        menuRow("[5]  Search Worker", 9, 14);
        menuRow("[6]  Logout", 9, 1);
        menuRow("[7]  Exit", 9, 4);
    }

    drawBot(9);
    cout << std::endl;
    setColor(15);
    cout << "  >> Choose: ";
    resetColor();
}

// sort page
void showSortMenu()
{
    cls();
    pageHeader("SHOW ALL WORKERS — SORT BY", 11, 15);
    menuRow("[1]  Default (by ID)", 11, 7);
    menuRow("[2]  Salary: Low to High", 11, 10);
    menuRow("[3]  Salary: High to Low", 11, 12);
    drawBot(11);
    cout << std::endl;
    setColor(15);
    cout << "  >> Choose: ";
    resetColor();
}

// search page
void showSearchMenu()
{
    cls();
    pageHeader("SEARCH WORKERS", 14, 15);
    menuRow("[1]  By ID", 14, 7);
    menuRow("[2]  By Name", 14, 7);
    menuRow("[3]  By Age", 14, 7);
    menuRow("[4]  By Gender", 14, 7);
    menuRow("[5]  By Position", 14, 7);
    menuRow("[6]  By Department", 14, 7);
    drawBot(14);
    cout << std::endl;
    setColor(15);
    cout << "  >> Choose: ";
    resetColor();
}

//  entry point
void runApp()
{
    setupConsole();
    welcomeScreen();

    vector<Worker> workers;
    bool run = true;

    while (run)
    {
        bool isAdmin = false, loggedIn = false;
        int option;

        // looop auth 
        while (!loggedIn)
        {
            printAuthMenu();
            if (!readInt(option))
            {
                waitEnter();
                continue;
            }

            if (option == 1)
            {
                registerNewUser();
            }
            else if (option == 2)
            {
                auto [ok, role] = login();
                if (ok)
                {
                    isAdmin = (role == "admin");
                    if (!isAdmin)
                        workerPage();
                    loadWorkersFromExcel(workers);
                    loggedIn = true;
                }
            }
            else if (option == 3)
            {
                goodbyeScreen();
                run = false;
                loggedIn = true;
            }
            else
            {
                animError("invalid option! choose 1, 2, or 3.");
                waitEnter();
            }
        }

        if (!run)
            break;

        int op;

        // menu 
        do
        {
            printMainMenu(isAdmin);
            if (!readInt(op))
            {
                waitEnter();
                op = -1;
                continue;
            }

            // access 
            if (!isAdmin && (op == 1 || op == 2 || op == 4))
            {
                cls();
                pageHeader("ACCESS DENIED", 12, 15);
                rowEmpty(12);
                row("This feature requires ADMIN access.", 12, 12, true);
                rowEmpty(12);
                drawBot(12);
                animError("admin only feature!");
                ms(500);
                waitEnter();
                continue;
            }

            switch (op)
            {
            
            case 1:
            {
                cls();
                pageHeader("ADD NEW WORKER", 11, 15);
                Worker w;
                w.input();

                bool dup = false;
                for (auto &e : workers)
                    if (e.getId() == w.getId())
                    {
                        dup = true;
                        break;
                    }

                if (dup)
                {
                    animError("worker with ID " + to_string(w.getId()) + " already exists!");
                }
                else
                {
                    workers.push_back(w);
                    exportExcel(workers);
                    animSuccess("worker added successfully!");
                    cout << std::endl;
                    setColor(14);
                    cout << "  ID:" << w.getId()
                         << "  Name:" << w.getName()
                         << "  Age:" << w.getAge()
                         << "  Salary:" << (int)w.getSalary() << "$"
                         << "  Gender:" << w.getGender()
                         << "  Position:" << w.getPosition()
                         << "  Dept:" << w.getDepartment()
                         << "  Hired:" << w.getDateHired() << std::endl;
                    resetColor();
                }
                waitEnter();
                break;
            }

            // update worker
            case 2:
            {
                if (workers.empty())
                {
                    cls();
                    pageHeader("UPDATE WORKER", 13, 15);
                    animError("no workers available to update.");
                    waitEnter();
                    break;
                }
                sortId(workers);
                cls();
                pageHeader("UPDATE WORKER", 13, 15);
                showTable(workers, 13);
                cout << std::endl;
                int id, found = 0;
                setColor(14);
                cout << "  Worker ID to update: ";
                resetColor();
                if (!readInt(id))
                {
                    waitEnter();
                    break;
                }
                for (auto &w : workers)
                {
                    if (w.getId() == id)
                    {
                        cls();
                        pageHeader("UPDATE WORKER", 13, 15);
                        w.update();
                        exportExcel(workers);
                        animSuccess("worker updated successfully!");
                        found = 1;
                        break;
                    }
                }
                if (!found)
                    animError("worker ID " + to_string(id) + " not found.");
                waitEnter();
                break;
            }

            // show all
            case 3:
            {
                int ch;
                showSortMenu();
                if (!readInt(ch))
                {
                    waitEnter();
                    break;
                }
                cls();
                if (ch == 1)
                    sortId(workers);
                else if (ch == 2)
                    sortLow(workers);
                else if (ch == 3)
                    sortHigh(workers);
                else
                {
                    animError("invalid sort option, using default.");
                    sortId(workers);
                }
                pageHeader("ALL WORKERS", 11, 15);
                showTable(workers, 11);
                waitEnter();
                break;
            }

            // delete worker
            case 4:
            {
                if (workers.empty())
                {
                    cls();
                    pageHeader("DELETE WORKER", 12, 15);
                    animError("no workers available to delete.");
                    waitEnter();
                    break;
                }
                sortId(workers);
                cls();
                pageHeader("DELETE WORKER", 12, 15);
                showTable(workers, 12);
                cout << std::endl;
                int id, found = 0;
                setColor(12);
                cout << "  Worker ID to delete: ";
                resetColor();
                if (!readInt(id))
                {
                    waitEnter();
                    break;
                }
                for (int i = 0; i < (int)workers.size(); i++)
                {
                    if (workers[i].getId() == id)
                    {
                        string nm = workers[i].getName();
                        cout << std::endl;
                        if (!confirmYes("  Confirm delete worker \"" + nm + "\"? (Y/N): "))
                        {
                            animError("delete cancelled.");
                            found = 1;
                            break;
                        }

                        spinner("Deleting " + nm + " ...", 12, 350);
                        workers.erase(workers.begin() + i);
                        exportExcel(workers);
                        animSuccess("worker \"" + nm + "\" deleted successfully!");
                        found = 1;
                        break;
                    }
                }
                if (!found)
                    animError("worker ID " + to_string(id) + " not found.");
                waitEnter();
                break;
            }

            // search
            case 5:
            {
                showSearchMenu();
                int c;
                if (!readInt(c))
                {
                    waitEnter();
                    break;
                }
                vector<Worker> result;
                cls();

                if (c == 1)
                {
                    int id;
                    pageHeader("SEARCH BY ID", 14, 15);
                    setColor(14);
                    cout << "  Enter ID: ";
                    resetColor();
                    if (!readInt(id))
                    {
                        waitEnter();
                        break;
                    }
                    spinner("Searching ...", 14, 300);
                    for (auto &w : workers)
                        if (w.getId() == id)
                            result.push_back(w);
                }
                else if (c == 2)
                {
                    string n;
                    pageHeader("SEARCH BY NAME", 14, 15);
                    setColor(14);
                    cout << "  Enter Name: ";
                    resetColor();
                    cin >> n;
                    spinner("Searching ...", 14, 300);
                    for (auto &w : workers)
                        if (w.getName() == n)
                            result.push_back(w);
                }
                else if (c == 3)
                {
                    int age;
                    pageHeader("SEARCH BY AGE", 14, 15);
                    setColor(14);
                    cout << "  Enter Age: ";
                    resetColor();
                    if (!readInt(age))
                    {
                        waitEnter();
                        break;
                    }
                    spinner("Searching ...", 14, 300);
                    for (auto &w : workers)
                        if (w.getAge() == age)
                            result.push_back(w);
                }
                else if (c == 4)
                {
                    string g;
                    pageHeader("SEARCH BY GENDER", 14, 15);
                    setColor(14);
                    cout << "  Enter Gender (Male/Female): ";
                    resetColor();
                    cin >> g;
                    // normalize
                    for (char &ch2 : g)
                        ch2 = tolower(ch2);
                    string gn = (g == "m" || g == "male") ? "Male" : "Female";
                    spinner("Searching ...", 14, 300);
                    for (auto &w : workers)
                        if (w.getGender() == gn)
                            result.push_back(w);
                }
                else if (c == 5)
                {
                    string pos;
                    pageHeader("SEARCH BY POSITION", 14, 15);
                    setColor(14);
                    cout << "  Enter Position: ";
                    resetColor();
                    cin >> pos;
                    spinner("Searching ...", 14, 300);
                    for (auto &w : workers)
                        if (w.getPosition() == pos)
                            result.push_back(w);
                }
                else if (c == 6)
                {
                    string dept;
                    pageHeader("SEARCH BY DEPARTMENT", 14, 15);
                    setColor(14);
                    cout << "  Enter Department: ";
                    resetColor();
                    cin >> dept;
                    spinner("Searching ...", 14, 300);
                    for (auto &w : workers)
                        if (w.getDepartment() == dept)
                            result.push_back(w);
                }
                else
                {
                    animError("invalid search option.");
                    waitEnter();
                    break;
                }

                showSearchTable(result);
                waitEnter();
                break;
            }

            // logout
            case 6:
            {
                cls();
                pageHeader("LOGGING OUT", 6, 15);
                spinner("Logging out ...", 6, 400);
                animSuccess("logged out! returning to login page ...");
                ms(500);
                waitEnter();
                break;
            }

            // exit
            case 7:
                goodbyeScreen();
                run = false;
                break;

            default:
                animError("invalid option! choose from the menu.");
                ms(300);
                waitEnter();
            }

        } while (op != 6 && op != 7);
    }

    showCursor();
}
