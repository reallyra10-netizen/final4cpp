#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <windows.h>
#include <tabulate/table.hpp>
#include <xlnt/xlnt.hpp>
#include "User.hpp"

using namespace std;
using namespace tabulate;

//CONSOLE SETUP & COLOR
  
HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
void setColor(int c) { SetConsoleTextAttribute(hCon, c); }
void resetColor()    { SetConsoleTextAttribute(hCon, 7); }
void ms(int t)       { Sleep(t); }
void cls()           { system("cls"); }

void hideCursor() { CONSOLE_CURSOR_INFO ci={1,FALSE}; SetConsoleCursorInfo(hCon,&ci); }
void showCursor() { CONSOLE_CURSOR_INFO ci={10,TRUE};  SetConsoleCursorInfo(hCon,&ci); }

void gotoxy(int x, int y)
{
    COORD c; c.X = x; c.Y = y;
    SetConsoleCursorPosition(hCon, c);
}

void setupConsole()
{
    system("mode con: cols=72 lines=38");
    SetConsoleTitleA("  Worker Management System  v2.0");
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
}


 
const int IW = 68;

const string TL="╔", TR="╗", BL="╚", BR="╝";
const string H ="═", V ="║";
const string ML="╠", MR="╣";
const string SL="╟", SR="╢", SH="─";

string rep(const string &s, int w)
{
    string r; r.reserve(w * s.size());
    for (int i = 0; i < w; i++) r += s;
    return r;
}

string center(const string &s, int w)
{
    int len = (int)s.size();
    if (len >= w) return s.substr(0, w);
    int l = (w - len) / 2, r = w - len - l;
    return string(l, ' ') + s + string(r, ' ');
}

string left_pad(const string &s, int w)
{
    int len = (int)s.size();
    if (len >= w) return s.substr(0, w);
    return s + string(w - len, ' ');
}


   //ANIMATION PRIMITIVES
void typeText(const string &t, int color, int delay = 8)
{
    setColor(color);
    for (char c : t) { cout << c; cout.flush(); ms(delay); }
    resetColor();
}


void animStr(const string &s, int color, int delay = 2)
{
    setColor(color);
    for (char c : s) { cout << c; cout.flush(); ms(delay); }
    resetColor();
}

// Instant colored print
void put(const string &s, int color)
{
    setColor(color); cout << s; resetColor();
}


void progressBar(int color, int steps = 28, int delay = 12)
{
    put("  [", 8);
    for (int i = 0; i < steps; i++)
    {
        cout << "\r";
        put("  [", 8);
        put(rep("#", i+1), color);
        put(rep(" ", steps-i-1), 8);
        put("]", 8);
        put("  " + to_string((i+1)*100/steps) + "%  ", 15);
        cout.flush();
        ms(delay);
    }
    cout << "\n";
}


void spinner(const string &label, int color, int dur = 400)
{
    const char fr[] = {'|','/','-','\\'};
    int steps = dur / 60;
    for (int i = 0; i < steps; i++)
    {
        setColor(color);
        cout << "\r  " << fr[i%4] << "  " << label << "   ";
        resetColor(); cout.flush(); ms(60);
    }
    cout << "\r" << string(50,' ') << "\r";
}


void pulse(const string &msg, int color, int times = 2)
{
    for (int i = 0; i < times; i++)
    {
        setColor(color);   cout << "\r  " << msg; cout.flush(); ms(80);
        setColor(color-2 < 0 ? color : color-2);
        cout << "\r  " << msg; cout.flush(); ms(80);
    }
    setColor(color); cout << "\r  " << msg << "\n"; resetColor();
}

   //BOX 
void drawTop(int borderColor, int delay = 0)
{
    setColor(borderColor);
    cout << TL << rep(H, IW+2) << TR << "\n";
    resetColor();
}

void drawBot(int borderColor, int delay = 0)
{
    setColor(borderColor);
    cout << BL << rep(H, IW+2) << BR << "\n";
    resetColor();
}

void drawMid(int borderColor, int delay = 0)
{
    setColor(borderColor);
    cout << ML << rep(H, IW+2) << MR << "\n";
    resetColor();
}

void drawSep(int borderColor)
{
    setColor(borderColor);
    cout << SL << rep(SH, IW+2) << SR << "\n";
    resetColor();
}

void row(const string &content, int borderColor, int textColor, bool centered = false)
{
    string padded = centered ? center(content, IW) : left_pad(" "+content, IW);
    setColor(borderColor); cout << V;
    setColor(textColor);   cout << " " << padded << " ";
    setColor(borderColor); cout << V << "\n";
    resetColor();
}

void rowEmpty(int borderColor)
{
    setColor(borderColor);
    cout << V << string(IW+2, ' ') << V << "\n";
    resetColor();
}

// Page header 
void pageHeader(const string &title, int borderColor, int titleColor)
{
    cout << "\n";
    drawTop(borderColor);
    row(title, borderColor, titleColor, true);
    drawMid(borderColor);
    cout << "\n";
}

void pageFooter(int borderColor)
{
    cout << "\n";
    drawBot(borderColor);
}

/*  MENU ROW  */
void menuRow(const string &text, int borderColor, int textColor, int delay = 0)
{
    row(text, borderColor, textColor);
}


//SUCCESS 
  
void animSuccess(const string &msg) { cout << "\n"; pulse("  OK  " + msg, 10); }
void animError(const string &msg)   { cout << "\n"; pulse("  !!  " + msg, 12); }

//INPUT VALIDATOR
bool readInt(int &out)
{
    string inp; cin >> inp;
    for (size_t i = 0; i < inp.size(); i++)
    {
        char c = inp[i];
        if (!isdigit(c) && !(c=='-' && i==0))
        { animError("Invalid input! Enter a NUMBER."); cin.clear(); return false; }
    }
    try { out = stoi(inp); return true; }
    catch (...) { animError("Number out of range."); return false; }
}

//   WORKER CLASS
class Worker
{
    int id; string name; int age; float salary;
public:
    Worker() : id(0), name(""), age(0), salary(0.0f) {}

    /* ===== Vichhet — Add ===== */
    void input()
    {
        auto askSalary = [&]()
        {
            string s;
            while (true)
            {
                cin >> s;
                bool ok=true, dot=false;
                for (char c:s){ if(c=='.'&&!dot){dot=true;continue;} if(!isdigit(c)){ok=false;break;} }
                if (ok && !s.empty()) { salary=stof(s); return; }
                animError("Invalid salary.");
                setColor(14); cout << "\n  Salary  : "; resetColor();
            }
        };
        setColor(14); cout << "\n  ID      : "; resetColor();
        while (!readInt(id)) { setColor(14); cout << "\n  ID      : "; resetColor(); }
        setColor(14); cout << "\n  Name    : "; resetColor(); cin >> name;
        setColor(14); cout << "\n  Age     : "; resetColor();
        while (!readInt(age)) { setColor(14); cout << "\n  Age     : "; resetColor(); }
        setColor(14); cout << "\n  Salary  : "; resetColor();
        askSalary();
    }

    /* ===== Vichhet — Update ===== */
    void update()
    {
        auto askSalary = [&]()
        {
            string s;
            while (true)
            {
                cin >> s;
                bool ok=true, dot=false;
                for (char c:s){ if(c=='.'&&!dot){dot=true;continue;} if(!isdigit(c)){ok=false;break;} }
                if (ok && !s.empty()) { salary=stof(s); return; }
                animError("Invalid salary.");
                setColor(14); cout << "\n  New Salary : "; resetColor();
            }
        };
        setColor(14); cout << "\n  New Name   : "; resetColor(); cin >> name;
        setColor(14); cout << "\n  New Age    : "; resetColor();
        while (!readInt(age)) { setColor(14); cout << "\n  New Age    : "; resetColor(); }
        setColor(14); cout << "\n  New Salary : "; resetColor();
        askSalary();
    }

    void setId(int i)       { id=i; }
    void setName(string n)  { name=n; }
    void setAge(int a)      { age=a; }
    void setSalary(float s) { salary=s; }
    int    getId()     { return id; }
    string getName()   { return name; }
    int    getAge()    { return age; }
    float  getSalary() { return salary; }
};

// Ecxel Stylee
void exportExcel(vector<Worker> &w)
{
    spinner("Saving to Excel ...", 11, 350);
    xlnt::workbook wb; auto ws=wb.active_sheet(); ws.title("Workers");
    ws.cell("A1").value("ID"); ws.cell("B1").value("Name");
    ws.cell("C1").value("Age"); ws.cell("D1").value("Salary");
    int r=2;
    for (auto &x:w)
    {
        ws.cell("A"+to_string(r)).value(x.getId());
        ws.cell("B"+to_string(r)).value(x.getName());
        ws.cell("C"+to_string(r)).value(x.getAge());
        ws.cell("D"+to_string(r)).value(to_string((int)x.getSalary())+"$");
        r++;
    }
    wb.save("workersdata.xlsx");
}

void loadWorkersFromExcel(vector<Worker> &workers)
{
    workers.clear();
    spinner("Loading data ...", 11, 350);
    xlnt::workbook wb;
    try { wb.load("workersdata.xlsx"); } catch (...) { return; }
    auto ws=wb.active_sheet(); bool first=true;
    for (auto row:ws.rows(false))
    {
        if (first){first=false;continue;}
        try
        {
            int id=row[0].value<int>();
            string name=row[1].to_string();
            int age=row[2].value<int>();
            string sal=row[3].to_string();
            auto p=sal.find('$'); if(p!=string::npos) sal=sal.substr(0,p);
            Worker x; x.setId(id); x.setName(name); x.setAge(age); x.setSalary(stof(sal));
            workers.push_back(x);
        } catch(...){continue;}
    }
}

//Sort
void sortId(vector<Worker> &w)
{ for(int i=0;i<(int)w.size();i++) for(int j=i+1;j<(int)w.size();j++) if(w[i].getId()>w[j].getId()) swap(w[i],w[j]); }

/* ===== Dy sorint — salary low-high ===== */
void sortLow(vector<Worker> &w)
{ for(int i=0;i<(int)w.size();i++) for(int j=i+1;j<(int)w.size();j++) if(w[i].getSalary()>w[j].getSalary()) swap(w[i],w[j]); }

/* ===== Dy sorint — salary high-low ===== */
void sortHigh(vector<Worker> &w)
{ for(int i=0;i<(int)w.size();i++) for(int j=i+1;j<(int)w.size();j++) if(w[i].getSalary()<w[j].getSalary()) swap(w[i],w[j]); }

//Show as table
void showTable(vector<Worker> &list, int borderColor)
{
    if (list.empty()) { animError("No worker data found!"); return; }
    Table t;
    t.add_row({"ID","Name","Age","Salary"});
    t[0].format().font_color(Color::cyan).font_style({FontStyle::bold});
    for (auto &w:list)
        t.add_row({to_string(w.getId()), w.getName(),
                   to_string(w.getAge()), to_string((int)w.getSalary())+"$"});

    cout << "\n";
    ostringstream oss; oss << t;
    istringstream ss(oss.str()); string line; int ln=0;
    while (getline(ss,line))
    {
        setColor(ln==1 ? 14 : (ln%2==0 ? 7 : 15));
        cout << "  " << line << "\n";
        resetColor();
        ln++;
    }
    cout << "\n";
}

void showSearchTable(vector<Worker> result)
{
    if (result.empty()) { animError("No matching worker found!"); return; }
    Table t;
    t.add_row({"ID","Name","Age","Salary"});
    t[0].format().font_color(Color::green).font_style({FontStyle::bold});
    for (auto &w:result)
        t.add_row({to_string(w.getId()), w.getName(),
                   to_string(w.getAge()), to_string((int)w.getSalary())+"$"});

    cout << "\n";
    ostringstream oss; oss << t;
    istringstream ss(oss.str()); string line; int ln=0;
    while (getline(ss,line))
    {
        setColor(ln==1 ? 10 : 7);
        cout << "  " << line << "\n";
        resetColor();
        ln++;
    }
    cout << "\n";
}


void waitEnter()
{
    cout << "\n";
    setColor(8); cout << "  Press ENTER to continue ..."; resetColor();
    cin.ignore(1000,'\n'); cin.get();
}


void welcomeScreen()
{
    cls(); hideCursor();

   
    setColor(9);
    for (int i=0;i<72;i++){ cout<<"═"; cout.flush(); ms(2); }
    cout<<"\n"; resetColor();


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
    int lc[] = {0,11,11,14,14,9,0,15,0};
    for (int i=0;i<9;i++) typeText(logo[i]+"\n", lc[i], 4);

    setColor(9);
    for (int i=0;i<72;i++){ cout<<"═"; cout.flush(); ms(2); }
    cout<<"\n\n"; resetColor();

    
    setColor(8); cout<<"  "; resetColor();
    typeText(" Institute of Science and Technology Advanced Development | Group 1\n", 7, 10);

    cout<<"\n";
    setColor(11); cout<<"  Loading "; resetColor();
    for (int i=0;i<18;i++)
    {
        setColor(i%2==0?10:14); cout<<"▓"; cout.flush(); ms(30);
    }
    cout<<"\n\n";

    progressBar(10, 28, 10);

    cout<<"\n";
    setColor(15); typeText("  >> Press ENTER to start ...", 15, 8);
    resetColor(); cout<<"\n";
    showCursor(); cin.get(); cls();
}

/* ════════════════════════════════════════════════════
   AUTH MENU  (Sovannara / Phanet)
   ════════════════════════════════════════════════════ */
void printAuthMenu()
{
    cls();
    pageHeader("USER AUTHENTICATION", 9, 15);
    menuRow("",            9, 7);
    menuRow("[1]  Register", 9, 10);
    menuRow("[2]  Login",    9, 11);
    menuRow("[3]  Exit",     9, 12);
    menuRow("",            9, 7);
    drawBot(9);
    cout<<"\n"; setColor(15); cout<<"  >> Choose: "; resetColor();
}

/* ════════════════════════════════════════════════════
   REGISTER  (Sovannara)
   ════════════════════════════════════════════════════ */
void registerNewUser()
{
    cls();
    pageHeader("USER REGISTRATION", 13, 15);

    string u,p,ri,role;
    setColor(14); cout<<"  Username : "; resetColor(); cin>>u;
    setColor(14); cout<<"\n  Password : "; resetColor(); cin>>p;

    cout<<"\n";
    menuRow("[1]  Admin",  13, 10);
    menuRow("[2]  Worker", 13, 11);
    cout<<"\n"; setColor(15); cout<<"  >> Role: "; resetColor();
    cin>>ri;

    if      (ri=="1"||ri=="admin")  role="admin";
    else if (ri=="2"||ri=="worker") role="worker";
    else
    {
        animError("Invalid role! Enter 1 or 2.");
        waitEnter();
        return;
    }

    spinner("Creating account ...", 13, 400);
    bool ok = UserManager::registerUser(u, p, role);
    if (ok)
    {
        animSuccess("Account created!");
    }
    else
    {
        animError("Account creation failed! Username already exists.");
    }
    ms(400);
    waitEnter();
}

/* ════════════════════════════════════════════════════
   LOGIN  (Phanet)
   ════════════════════════════════════════════════════ */
pair<bool,string> login()
{
    cls();
    pageHeader("LOGIN", 9, 15);

    string u,p;
    setColor(14); cout<<"  Username : "; resetColor(); cin>>u;
    setColor(14); cout<<"\n  Password : "; resetColor(); cin>>p;

    spinner("Authenticating ...", 9, 400);
    string role = UserManager::loginUser(u, p);

    if (!role.empty())
    {
        animSuccess("Welcome back! Logged in as " + string(role=="admin"?"ADMIN":"WORKER"));
        ms(400);
        return {true, role};
    }
    animError("Wrong username or password.");
    ms(400);
    waitEnter();
    return {false,""};
}

/* ════════════════════════════════════════════════════
   WORKER PAGE
   ════════════════════════════════════════════════════ */
void workerPage()
{
    cls();
    pageHeader("WORKER MODE — VIEW ONLY", 6, 15);
    menuRow("",                                          6, 7);
    menuRow("You are logged in as:  WORKER",            6, 6);
    menuRow("View and Search are available to you.",    6, 7);
    menuRow("Add / Update / Delete require Admin.",     6, 8);
    menuRow("",                                          6, 7);
    drawBot(6);
    waitEnter();
}

/* ════════════════════════════════════════════════════
   MAIN MENU
   ════════════════════════════════════════════════════ */
void printMainMenu(bool isAdmin)
{
    cls();
    pageHeader("WORKER DASHBOARD", 9, 15);

    if (isAdmin)
    {
        menuRow("[1]  Add Worker",    9, 10);
        menuRow("[2]  Update Worker", 9, 13);
        menuRow("[3]  Show All",      9, 11);
        menuRow("[4]  Delete Worker", 9, 12);
        menuRow("[5]  Search Worker", 9, 14);
        menuRow("[6]  Logout",        9,  1);
        menuRow("[7]  Exit",          9,  4);
    }
    else
    {
        menuRow("[1]  Add Worker     (Admin only)", 9, 8);
        menuRow("[2]  Update Worker  (Admin only)", 9, 8);
        menuRow("[3]  Show All",                   9,11);
        menuRow("[4]  Delete Worker  (Admin only)", 9, 8);
        menuRow("[5]  Search Worker",              9,14);
        menuRow("[6]  Logout",                     9, 1);
        menuRow("[7]  Exit",                       9, 4);
    }

    drawBot(9);
    cout<<"\n"; setColor(15); cout<<"  >> Choose: "; resetColor();
}

/* ════════════════════════════════════════════════════
   SORT SUB-MENU
   ════════════════════════════════════════════════════ */
void showSortMenu()
{
    cls();
    pageHeader("SHOW ALL WORKERS — SORT BY", 11, 15);
    menuRow("[1]  Default (by ID)",     11,  7);
    menuRow("[2]  Salary: Low to High", 11, 10);
    menuRow("[3]  Salary: High to Low", 11, 12);
    drawBot(11);
    cout<<"\n"; setColor(15); cout<<"  >> Choose: "; resetColor();
}

/* ════════════════════════════════════════════════════
   SEARCH SUB-MENU
   ════════════════════════════════════════════════════ */
void showSearchMenu()
{
    cls();
    pageHeader("SEARCH WORKERS", 14, 15);
    menuRow("[1]  By ID",   14, 7);
    menuRow("[2]  By Name", 14, 7);
    menuRow("[3]  By Age",  14, 7);
    drawBot(14);
    cout<<"\n"; setColor(15); cout<<"  >> Choose: "; resetColor();
}

/* ════════════════════════════════════════════════════
   GOODBYE SCREEN  — ORIGINAL (unchanged)
   ════════════════════════════════════════════════════ */
void goodbyeScreen()
{
    cls();
    pageHeader("GOODBYE", 6, 15);
    rowEmpty(6);
    row("Thank you for using Worker Console!", 6, 14, true);
    row("Have a great day!",                  6,  7, true);
    rowEmpty(6);
    drawBot(6);
    cout<<"\n";
    ms(700);
}

/* ════════════════════════════════════════════════════
   MAIN
   ════════════════════════════════════════════════════ */
int main()
{
    setupConsole();
    welcomeScreen();

    vector<Worker> workers;
    bool run = true;

    while (run)
    {
        bool isAdmin=false, loggedIn=false;
        int option;

        /* ══ AUTH LOOP ══ */
        while (!loggedIn)
        {
            printAuthMenu();
            if (!readInt(option)) { waitEnter(); continue; }

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
                    if (!isAdmin) workerPage();
                    loadWorkersFromExcel(workers);
                    loggedIn = true;
                }
            }
            else if (option == 3)
            {
                goodbyeScreen();
                run = false; loggedIn = true;
            }
            else
            {
                animError("Invalid option! Choose 1, 2, or 3.");
                waitEnter();
            }
        }

        if (!run) break;

        int op;

        /* ══ MENU LOOP ══ */
        do
        {
            printMainMenu(isAdmin);
            if (!readInt(op)) { waitEnter(); op=-1; continue; }

            // Access control
            if (!isAdmin && (op==1||op==2||op==4))
            {
                cls();
                pageHeader("ACCESS DENIED", 12, 15);
                rowEmpty(12);
                row("This feature requires ADMIN access.", 12, 12, true);
                rowEmpty(12);
                drawBot(12);
                animError("Admin only feature!");
                ms(500);
                waitEnter();
                continue;
            }

            switch (op)
            {
            /* ADD WORKER */
            case 1:
            {
                cls();
                pageHeader("ADD NEW WORKER", 11, 15);
                Worker w; w.input();
                bool dup=false;
                for (auto &e:workers) if(e.getId()==w.getId()){dup=true;break;}
                if (dup)
                {
                    animError("Worker with ID "+to_string(w.getId())+" already exists!");
                }
                else
                {
                    workers.push_back(w); exportExcel(workers);
                    animSuccess("Worker added successfully!");
                    cout<<"\n";
                    setColor(14);
                    cout<<"  ID:"<<w.getId()<<"  Name:"<<w.getName()
                        <<"  Age:"<<w.getAge()<<"  Salary:"<<(int)w.getSalary()<<"$\n";
                    resetColor();
                }
                waitEnter(); break;
            }

            /* UPDATE WORKER  (Vichhet) */
            case 2:
            {
                cls();
                pageHeader("UPDATE WORKER", 13, 15);
                int id, found=0;
                setColor(14); cout<<"  Worker ID to update: "; resetColor();
                if (!readInt(id)) { waitEnter(); break; }
                for (auto &w:workers)
                {
                    if (w.getId()==id)
                    {
                        cls(); pageHeader("UPDATE WORKER", 13, 15);
                        w.update(); exportExcel(workers);
                        animSuccess("Worker updated successfully!");
                        found=1; break;
                    }
                }
                if (!found) animError("Worker ID "+to_string(id)+" not found.");
                waitEnter(); break;
            }

            /* SHOW ALL  (Kun sokea) */
            case 3:
            {
                int ch; showSortMenu();
                if (!readInt(ch)) { waitEnter(); break; }
                cls();
                if      (ch==1) sortId(workers);
                else if (ch==2) sortLow(workers);
                else if (ch==3) sortHigh(workers);
                else
                {
                    animError("Invalid sort option, using default.");
                    sortId(workers);
                }
                pageHeader("ALL WORKERS", 11, 15);
                showTable(workers, 11);
                waitEnter(); break;
            }

            /* DELETE WORKER  (Dararithy) */
            case 4:
            {
                cls();
                pageHeader("DELETE WORKER", 12, 15);
                int id, found=0;
                setColor(12); cout<<"  Worker ID to delete: "; resetColor();
                if (!readInt(id)) { waitEnter(); break; }
                for (int i=0;i<(int)workers.size();i++)
                {
                    if (workers[i].getId()==id)
                    {
                        string nm=workers[i].getName();
                        spinner("Deleting "+nm+" ...", 12, 350);
                        workers.erase(workers.begin()+i);
                        exportExcel(workers);
                        animSuccess("Worker \""+nm+"\" deleted successfully!");
                        found=1; break;
                    }
                }
                if (!found) animError("Worker ID "+to_string(id)+" not found.");
                waitEnter(); break;
            }

            /* SEARCH  (Sethapiseth) */
            case 5:
            {
                showSearchMenu();
                int c; if (!readInt(c)) { waitEnter(); break; }
                vector<Worker> result;
                cls();

                if (c==1)
                {
                    int id;
                    pageHeader("SEARCH BY ID", 14, 15);
                    setColor(14); cout<<"  Enter ID: "; resetColor();
                    if (!readInt(id)) { waitEnter(); break; }
                    spinner("Searching ...", 14, 300);
                    for (auto &w:workers) if(w.getId()==id) result.push_back(w);
                }
                else if (c==2)
                {
                    string n;
                    pageHeader("SEARCH BY NAME", 14, 15);
                    setColor(14); cout<<"  Enter Name: "; resetColor(); cin>>n;
                    spinner("Searching ...", 14, 300);
                    for (auto &w:workers) if(w.getName()==n) result.push_back(w);
                }
                else if (c==3)
                {
                    int age;
                    pageHeader("SEARCH BY AGE", 14, 15);
                    setColor(14); cout<<"  Enter Age: "; resetColor();
                    if (!readInt(age)) { waitEnter(); break; }
                    spinner("Searching ...", 14, 300);
                    for (auto &w:workers) if(w.getAge()==age) result.push_back(w);
                }
                else
                {
                    animError("Invalid search option.");
                    waitEnter();
                    break;
                }

                showSearchTable(result);
                waitEnter(); break;
            }

            /* LOGOUT  (Phanet) */
            case 6:
            {
                cls();
                pageHeader("LOGGING OUT", 6, 15);
                spinner("Logging out ...", 6, 400);
                animSuccess("Logged out! Returning to login page ...");
                ms(500);
                waitEnter();
                break;
            }

            
            case 7:
                goodbyeScreen();
                run = false;
                break;

            default:
                animError("Invalid option! Choose from the menu.");
                ms(300);
                waitEnter();
            }

        } while (op != 6 && op != 7);
    }

    showCursor();
    return 0;
}