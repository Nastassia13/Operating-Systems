#include <stdio.h>
#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include "../Main/Header.h"

using namespace std;


int main(int argc, char *argv[]) {
    if (argc != 4)
        return 1;
    string fileName = argv[1];
    string reportName = argv[2];
    double payment = atof(argv[3]);
    ofstream fReport(reportName);
    ifstream fFile(fileName, ios::binary);
    if (!fFile.is_open() || fFile.peek() == EOF)
        exit(1);
    fReport << "Report on the file: " << fileName << "\n";
    while (fFile.peek() != EOF) {
        employee emp;
        fFile >> emp.num >> emp.name >> emp.hours;
        double salary = emp.hours * payment;
        fReport << emp.num << " " << emp.name << " " << emp.hours << " " << salary << "\n";
        fFile.ignore();
    }
    fReport.close();
    fFile.close();
    return 0;
}