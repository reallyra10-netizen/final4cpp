#pragma once
#include <string>
#include <vector>

using std::string;
using std::vector;

class User {
private:
    string username;
    string password;
    string role; // "admin" or "worker"

public:
    User() {}
    User(string username, string password, string role);
    
    string getUsername() const;
    string getPassword() const;
    string getRole() const;
    
    void setUsername(string username);
    void setPassword(string password);
    void setRole(string role);
};

class UserManager {
public:
    static const string USERS_FILE;
    
    // Register a new user
    static bool registerUser(const string &username, const string &password, const string &role);
    
    // Login user (verify credentials)
    static string loginUser(const string &username, const string &password);
    
    // Check if username already exists
    static bool userExists(const string &username);
    
    // Load all users from Excel
    static vector<User> loadUsers();
    
    // Save users to Excel
    static void saveUsers(const vector<User> &users);
};
