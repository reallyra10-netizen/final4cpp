#include "User.hpp"
#include <iostream>
#include <xlnt/xlnt.hpp>
#include <filesystem>

using namespace std;


const string UserManager::USERS_FILE = "users.xlsx";

// user
User::User(string username, string password, string role)
    : username(username), password(password), role(role) {}

string User::getUsername() const { return username; }
string User::getPassword() const { return password; }
string User::getRole()     const { return role;     }

void User::setUsername(string u) { username = u; }
void User::setPassword(string p) { password = p; }
void User::setRole    (string r) { role     = r; }

// show users from excel file
vector<User> UserManager::loadUsers()
{
    vector<User> users;

    if (!std::filesystem::exists(USERS_FILE))
        return users;

    xlnt::workbook wb;
    try { wb.load(USERS_FILE); }
    catch (...) { cout << "warning: could not load users file.\n"; return users; }

    auto ws = wb.active_sheet();
    bool isHeader = true;

    for (auto row : ws.rows(false))
    {
        if (isHeader) { isHeader = false; continue; }

        string u = row[0].to_string();
        string p = row[1].to_string();
        string r = row[2].to_string();

        if (!u.empty())
            users.emplace_back(u, p, r);
    }
    return users;
}

// save  to excel 
void UserManager::saveUsers(const vector<User> &users)
{
    xlnt::workbook wb;
    auto ws = wb.active_sheet();
    ws.title("Users");

    ws.cell("A1").value("Username");
    ws.cell("B1").value("Password");
    ws.cell("C1").value("Role");

    int row = 2;
    for (const auto &user : users)
    {
        ws.cell("A" + to_string(row)).value(user.getUsername());
        ws.cell("B" + to_string(row)).value(user.getPassword());
        ws.cell("C" + to_string(row)).value(user.getRole());
        row++;
    }
    wb.save(USERS_FILE);
}

// check Username 
bool UserManager::userExists(const string &username)
{
    for (const auto &u : loadUsers())
        if (u.getUsername() == username)
            return true;
    return false;
}

// REegister
bool UserManager::registerUser(const string &username,
                                const string &password,
                                const string &role)
{
    vector<User> users = loadUsers();

    for (const auto &u : users)
        if (u.getUsername() == username)
            return false; // duplicate

    if (password.length() < 3)
        return false;

    if (role != "admin" && role != "worker")
        return false;

    users.emplace_back(username, password, role);
    saveUsers(users);
    return true;
}

// login
string UserManager::loginUser(const string &username, const string &password)
{
    for (const auto &u : loadUsers())
        if (u.getUsername() == username && u.getPassword() == password)
            return u.getRole();
    return "";
}
