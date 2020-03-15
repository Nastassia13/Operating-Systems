#include <iostream>
#include <fstream>
#include "../Main/Header.h"

using namespace std;


int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "Wrong arguments!\n";
        return 1;
    }
    string fileName = argv[1];
    int numberRecords = atoi(argv[2]);
    ofstream fout(fileName, ios::binary);
    for (int i = 0; i < numberRecords; i++) {
        employee emp;
        cout << "Enter employee num:\n";
        cin >> emp.num;
        cout << "Enter employee name:\n";
        cin >> emp.name;
        cout << "Enter employee hours:\n";
        cin >> emp.hours;
        fout << emp.num << " " << emp.name << " " << emp.hours << "\n";
    }
    fout.close();
    return 0;
}
