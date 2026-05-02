#pragma once
#include <string>
#include <vector>

using std::string;
using std::vector;

// ===== USER CLASS =====
// Represents a single user account with username, password, and role
class User {
private:
    string username;  // The username for login
    string password;  // The password for login
    string role;      // "admin" or "worker"

public:
    // Default constructor - creates empty user
    User() {}
    
    // Constructor that sets username, password, and role
    User(string username, string password, string role);
    
    // Getter methods - retrieve user information
    string getUsername() const;
    string getPassword() const;
    string getRole() const;
    
    // Setter methods - modify user information
    void setUsername(string username);
    void setPassword(string password);
    void setRole(string role);
};

// ===== USER MANAGER CLASS =====
// Manages user operations: registration, login, and file storage
class UserManager {
public:
    static const string USERS_FILE;  // Filename to store users ("users.xlsx")
    
    // Register a new user (create an account)
    static bool registerUser(const string &username, const string &password, const string &role);
    
    // Login user (verify username and password)
    static string loginUser(const string &username, const string &password);
    
    // Check if a username already exists
    static bool userExists(const string &username);
    
    // Load all users from Excel file into memory
    static vector<User> loadUsers();
    
    // Save all users to Excel file
    static void saveUsers(const vector<User> &users);
};
