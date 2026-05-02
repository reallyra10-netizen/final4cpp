#include "User.hpp"
#include <iostream>
#include <xlnt/xlnt.hpp>
#include <filesystem>

using namespace std;

// ===== CONSTANT DEFINITION =====
// The filename where all user accounts are stored
const string UserManager::USERS_FILE = "users.xlsx";

// ===== USER CLASS IMPLEMENTATION =====

// Constructor: Initialize a user with username, password, and role
User::User(string username, string password, string role)
    : username(username), password(password), role(role) {}

// Return the user's username
string User::getUsername() const { return username; }

// Return the user's password
string User::getPassword() const { return password; }

// Return the user's role ("admin" or "worker")
string User::getRole() const { return role; }

// Update the user's username
void User::setUsername(string username) { this->username = username; }

// Update the user's password
void User::setPassword(string password) { this->password = password; }

// Update the user's role
void User::setRole(string role) { this->role = role; }

// ===== USER MANAGER IMPLEMENTATION =====

// Load all users from the Excel file
// Returns: vector of User objects, empty if file doesn't exist
vector<User> UserManager::loadUsers() {
    vector<User> users;  // Create empty list to store users
    
    // Check if the users file exists first
    if (!std::filesystem::exists(USERS_FILE)) {
        return users;  // Return empty list if file doesn't exist
    }
    
    xlnt::workbook wb;  // Create Excel workbook object
    
    try {
        wb.load(USERS_FILE);  // Try to load the file
    } catch (...) {
        cout << "⚠️ Error loading users file.\n";
        return users;  // Return empty list if error occurs
    }
    
    auto ws = wb.active_sheet();  // Get the active worksheet
    bool isHeader = true;         // Flag to skip header row
    
    // Read each row in the worksheet
    for (auto row : ws.rows(false)) {
        if (isHeader) {
            isHeader = false;  // Skip the header row
            continue;
        }
        
        // Extract data from each column
        string username = row[0].to_string();  // Column A: username
        string password = row[1].to_string();  // Column B: password
        string role = row[2].to_string();      // Column C: role
        
        // Only add if username is not empty
        if (!username.empty()) {
            users.emplace_back(username, password, role);  // Add user to list
        }
    }
    
    return users;  // Return the list of loaded users
}

// Save all users to the Excel file
// Overwrites the file with the current list of users
void UserManager::saveUsers(const vector<User> &users) {
    xlnt::workbook wb;       // Create new Excel workbook
    auto ws = wb.active_sheet();  // Get the active worksheet
    ws.title("Users");       // Name the worksheet "Users"
    
    // Create header row
    ws.cell("A1").value("Username");
    ws.cell("B1").value("Password");
    ws.cell("C1").value("Role");
    
    int row = 2;  // Start data at row 2 (row 1 is header)
    
    // Write each user to the spreadsheet
    for (const auto &user : users) {
        ws.cell("A" + to_string(row)).value(user.getUsername());
        ws.cell("B" + to_string(row)).value(user.getPassword());
        ws.cell("C" + to_string(row)).value(user.getRole());
        row++;  // Move to next row
    }
    
    wb.save(USERS_FILE);  // Save the file to disk
}

// Check if a username already exists
// Returns: true if user exists, false otherwise
bool UserManager::userExists(const string &username) {
    vector<User> users = loadUsers();  // Load all users
    
    // Check each user
    for (const auto &user : users) {
        if (user.getUsername() == username) {
            return true;  // Found the user
        }
    }
    
    return false;  // User not found
}

// Register a new user (create an account)
// Returns: true if successful, false if username exists or validation fails
bool UserManager::registerUser(const string &username, const string &password, const string &role) {
    // Step 1: Load existing users
    vector<User> users = loadUsers();
    
    // Step 2: Check if username already exists
    for (const auto &user : users) {
        if (user.getUsername() == username) {
            cout << "\n❌ Username already exists! Please choose a different username.\n";
            return false;  // Registration failed
        }
    }
    
    // Step 3: Validate password (must be at least 3 characters)
    if (password.length() < 3) {
        cout << "\n❌ Password must be at least 3 characters long.\n";
        return false;  // Registration failed
    }
    
    // Step 4: Validate role (must be "admin" or "worker")
    if (role != "admin" && role != "worker") {
        cout << "\n❌ Invalid role! Choose 'admin' or 'worker'.\n";
        return false;  // Registration failed
    }
    
    // Step 5: Add new user to the list
    users.emplace_back(username, password, role);
    
    // Step 6: Save all users (including new user) to file
    saveUsers(users);
    
    cout << "\n✅ Registration successful!\n";
    return true;  // Registration succeeded
}

// Login user (verify credentials)
// Returns: user's role ("admin" or "worker") if successful, empty string if failed
string UserManager::loginUser(const string &username, const string &password) {
    vector<User> users = loadUsers();  // Load all users
    
    // Check each user to find matching username and password
    for (const auto &user : users) {
        if (user.getUsername() == username && user.getPassword() == password) {
            return user.getRole();  // Return role on successful login
        }
    }
    
    return "";  // Return empty string if login fails
}
