#pragma once
#include <string>
#include <vector>
#include "Student.hpp"

using std::string;

// ===== EXCEL UTILITY FUNCTIONS =====
// Helper functions for working with Excel files

// Write all students to an Excel file
void writeExcel(const string &filename, std::vector<Student> &students);

// Read students from an Excel file and return as a vector
std::vector<Student> readExcelToVector(const string &filename);

// Read an Excel file and display its contents on console
void readExcel(const string &filename);
