#pragma once
#include <string>
#include <vector>

using std::string;
using std::vector;

// user class
class User {
private:
    string username;
    string password;
    string role;

public:
    User() {}
    User(string username, string password, string role);

    string getUsername() const;
    string getPassword() const;
    string getRole()     const;

    void setUsername(string username);
    void setPassword(string password);
    void setRole(string role);
};

// user manager
class UserManager {
public:
    static const string USERS_FILE;

    static bool   registerUser(const string &username, const string &password, const string &role);
    static string loginUser   (const string &username, const string &password);
    static bool   userExists  (const string &username);

    static vector<User> loadUsers();
    static void         saveUsers(const vector<User> &users);
};
