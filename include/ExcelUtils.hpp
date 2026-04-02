#pragma once
#include <string>
#include <vector>
#include "Student.hpp"

using std::string; 

void writeExcel(const string &filename, std::vector<Student> &students);
std::vector<Student> readExcelToVector(const string &filename);
void readExcel(const string &filename);
