#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "Header.h"

using namespace std;


int main() {
    char fileName[20];
    char numberRecords[20];
    cout << "Enter file name:\n";
    cin >> fileName;
    cout << "Enter number of records:\n";
    cin >> numberRecords;
    sprintf(fileName, "%s.bin", fileName);


    pid_t pid1 = vfork();
    int status;
    if (pid1 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        if (execlp("/home/nastassia/CLionProjects/OS_lab3/Creator/a.out", "creator", fileName, numberRecords, NULL) == -1) {
            perror("execl");
            exit(EXIT_FAILURE);
        }
        exit(0);
    } else {
        if (waitpid(-1, &status, 0) == -1) {
            perror("waitpid");
        } else {
            if (WIFEXITED(status)) {
                printf("Child exited normally with code %d\n", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("Child was terminated by a signal %d\n", WTERMSIG(status));
            }
        }
    }


    ifstream fin1(fileName, ios::binary);
    if (!fin1.is_open() || fin1.peek() == EOF)
        exit(1);
    cout << "File: " << fileName << "\n";
    while (!fin1.eof()) {
        string str;
        getline(fin1, str);
        cout << str << "\n";
    }
    fin1.close();


    char reportName[20];
    char payment[20];
    cout << "Enter report name:\n";
    cin >> reportName;
    cout << "Enter payment:\n";
    cin >> payment;
    sprintf(reportName, "%s.txt", reportName);


    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid2 == 0) {
        if (execlp("/home/nastassia/CLionProjects/OS_lab3/Reporter/a.out", "reporter", fileName, reportName, payment, NULL) == -1) {
            perror("execl");
            exit(EXIT_FAILURE);
        }
    } else {
        if (waitpid(-1, &status, 0) == -1) {
            perror("waitpid");
        } else {
            if (WIFEXITED(status)) {
                printf("Child exited normally with code %d\n", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("Child was terminated by a signal %d\n", WTERMSIG(status));
            }
        }
    }


    ifstream fin2(reportName);
    if (!fin2.is_open() || fin2.peek() == EOF)
        return 1;
    cout << "File: " << reportName << "\n";
    while (!fin2.eof()) {
        string str;
        getline(fin2, str);
        cout << str << "\n";
    }
    return 0;
}