#pragma once
#include <string>
#include <vector>
#include "Worker.hpp"

using std::string;

// export workers 
void exportExcel(std::vector<Worker> &workers);

// load workers 
void loadWorkersFromExcel(std::vector<Worker> &workers);
