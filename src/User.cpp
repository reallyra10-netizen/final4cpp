#include "User.hpp"
#include <iostream>
#include <xlnt/xlnt.hpp>

using namespace std;

const string UserManager::USERS_FILE = "users.xlsx";

// User class implementation
User::User(string username, string password, string role)
    : username(username), password(password), role(role) {}

string User::getUsername() const { return username; }
string User::getPassword() const { return password; }
string User::getRole() const { return role; }

void User::setUsername(string username) { this->username = username; }
void User::setPassword(string password) { this->password = password; }
void User::setRole(string role) { this->role = role; }

// UserManager implementation
vector<User> UserManager::loadUsers() {
    vector<User> users;
    xlnt::workbook wb;
    
    try {
        wb.load(USERS_FILE);
    } catch (...) {
        cout << "📋 No users file found. Starting fresh.\n";
        return users;
    }
    
    auto ws = wb.active_sheet();
    bool isHeader = true;
    
    for (auto row : ws.rows(false)) {
        if (isHeader) {
            isHeader = false;
            continue;
        }
        
        string username = row[0].to_string();
        string password = row[1].to_string();
        string role = row[2].to_string();
        
        if (!username.empty()) {
            users.emplace_back(username, password, role);
        }
    }
    
    return users;
}

void UserManager::saveUsers(const vector<User> &users) {
    xlnt::workbook wb;
    auto ws = wb.active_sheet();
    ws.title("Users");
    
    ws.cell("A1").value("Username");
    ws.cell("B1").value("Password");
    ws.cell("C1").value("Role");
    
    int row = 2;
    for (const auto &user : users) {
        ws.cell("A" + to_string(row)).value(user.getUsername());
        ws.cell("B" + to_string(row)).value(user.getPassword());
        ws.cell("C" + to_string(row)).value(user.getRole());
        row++;
    }
    
    wb.save(USERS_FILE);
}

bool UserManager::userExists(const string &username) {
    vector<User> users = loadUsers();
    for (const auto &user : users) {
        if (user.getUsername() == username) {
            return true;
        }
    }
    return false;
}

bool UserManager::registerUser(const string &username, const string &password, const string &role) {
    // Check if username already exists
    if (userExists(username)) {
        cout << "\n❌ Username already exists! Please choose a different username.\n";
        return false;
    }
    
    // Validate password
    if (password.length() < 3) {
        cout << "\n❌ Password must be at least 3 characters long.\n";
        return false;
    }
    
    // Validate role
    if (role != "admin" && role != "worker") {
        cout << "\n❌ Invalid role! Choose 'admin' or 'worker'.\n";
        return false;
    }
    
    // Load existing users
    vector<User> users = loadUsers();
    
    // Add new user
    users.emplace_back(username, password, role);
    
    // Save all users
    saveUsers(users);
    
    cout << "\n✅ Registration successful!\n";
    return true;
}

string UserManager::loginUser(const string &username, const string &password) {
    vector<User> users = loadUsers();
    
    for (const auto &user : users) {
        if (user.getUsername() == username && user.getPassword() == password) {
            return user.getRole(); // Return role on successful login
        }
    }
    
    return ""; // Return empty string if login fails
}
