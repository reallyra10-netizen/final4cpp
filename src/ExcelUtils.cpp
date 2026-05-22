#include "ExcelUtils.hpp"
#include "ConsoleUtils.hpp"
#include <iostream>
#include <xlnt/xlnt.hpp>

using namespace std;

// export workers to excel
void exportExcel(vector<Worker> &workers)
{
    spinner("Saving to Excel ...", 11, 350);

    xlnt::workbook wb;
    auto ws = wb.active_sheet();
    ws.title("Workers");

    // header row
    ws.cell("A1").value("ID");
    ws.cell("B1").value("Name");
    ws.cell("C1").value("Age");
    ws.cell("D1").value("Salary");
    ws.cell("E1").value("Gender");
    ws.cell("F1").value("Position");
    ws.cell("G1").value("Department");
    ws.cell("H1").value("Date Hired");

    int r = 2;
    for (auto &w : workers)
    {
        ws.cell("A" + to_string(r)).value(w.getId());
        ws.cell("B" + to_string(r)).value(w.getName());
        ws.cell("C" + to_string(r)).value(w.getAge());
        ws.cell("D" + to_string(r)).value(to_string((int)w.getSalary()) + "$");
        ws.cell("E" + to_string(r)).value(w.getGender());
        ws.cell("F" + to_string(r)).value(w.getPosition());
        ws.cell("G" + to_string(r)).value(w.getDepartment());
        ws.cell("H" + to_string(r)).value(w.getDateHired());
        r++;
    }
    wb.save("workersdata.xlsx");
}

// load workers from excel
void loadWorkersFromExcel(vector<Worker> &workers)
{
    workers.clear();
    spinner("Loading data ...", 11, 350);

    xlnt::workbook wb;
    try { wb.load("workersdata.xlsx"); }
    catch (...) { return; }

    auto ws = wb.active_sheet();
    bool first = true;

    for (auto row : ws.rows(false))
    {
        if (first) { first = false; continue; }
        try
        {
            int    id         = row[0].value<int>();
            string name       = row[1].to_string();
            int    age        = row[2].value<int>();
            string sal        = row[3].to_string();
            string gender     = row.length() > 4 ? row[4].to_string() : "";
            string position   = row.length() > 5 ? row[5].to_string() : "";
            string department = row.length() > 6 ? row[6].to_string() : "";
            string dateHired  = row.length() > 7 ? row[7].to_string() : "";

          
            auto p = sal.find('$');
            if (p != string::npos) sal = sal.substr(0, p);

            Worker w;
            w.setId(id);
            w.setName(name);
            w.setAge(age);
            w.setSalary(stof(sal));
            w.setGender(gender);
            w.setPosition(position);
            w.setDepartment(department);
            w.setDateHired(dateHired);
            workers.push_back(w);
        }
        catch (...) { continue; }
    }
}
