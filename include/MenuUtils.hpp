#pragma once
#include <vector>
#include <string>
#include "Student.hpp"

using std::vector; 
using std::string;

// ===== MENU UTILITY FUNCTIONS =====
// Helper functions for displaying menus and tables

// Display all students in a formatted table
void displayTable(const vector<Student> &students);

// Print a menu with custom items
void printMenu(const vector<string> &items);
