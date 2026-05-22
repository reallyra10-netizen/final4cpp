#pragma once
#include <vector>
#include <string>
#include "Worker.hpp"

using std::vector;
using std::string;

// sort
void sortId  (vector<Worker> &w); 
void sortLow (vector<Worker> &w); 
void sortHigh(vector<Worker> &w); 

// show as table
void showTable      (vector<Worker> &list,  int borderColor);
void showSearchTable(vector<Worker>  result);


void welcomeScreen();
void goodbyeScreen();

// auth 
void printAuthMenu();
void registerNewUser();
std::pair<bool, string> login();
void workerPage();

// main 
void printMainMenu(bool isAdmin);
void showSortMenu();
void showSearchMenu(); 

void runApp();
