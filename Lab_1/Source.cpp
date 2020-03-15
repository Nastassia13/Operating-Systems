#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Header.h"
using namespace std;


int main() {
	char fileName[20];
	int numberRecords;
	cout << "Enter file name:\n";
	cin >> fileName;
	cout << "Enter number of records:\n";
	cin >> numberRecords;
	STARTUPINFO sti;
	ZeroMemory(&sti, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	char args1[100];
	sprintf(fileName, "%s.bin", fileName);
	sprintf(args1, "%s %d", fileName, numberRecords);
	if (!CreateProcess("..\\Debug\\Creator.exe", args1, NULL, NULL, FALSE, NULL, NULL, NULL, &sti, &pi))
		return 1;
	WaitForSingleObject(pi.hProcess, INFINITE);
	char buf[1000];
	DWORD dwRead = 0;
	HANDLE hFile = CreateFile(fileName, GENERIC_READ, NULL, 0, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return 1;
	}
	employee emp;
	DWORD dwBytes;
	cout << "File: " << fileName << "\n";
	ReadFile(hFile, &emp, sizeof(employee), &dwBytes, NULL);
	do
	{
		cout << emp.num << " ";
		cout << emp.name << " ";
		cout << emp.hours << " \n";
		ReadFile(hFile, &emp, sizeof(employee), &dwBytes, NULL);
	} while (dwBytes == sizeof(employee));
	CloseHandle(hFile);

	char reportName[20];
	int payment;
	cout << "Enter report name:\n";
	cin >> reportName;
	cout << "Enter payment:\n";
	cin >> payment;
	char args2[100];
	sprintf(reportName, "%s.txt", reportName);
	sprintf(args2, "%s %s %d", fileName, reportName, payment);
	CreateProcess("..\\Debug\\Reporter.exe", args2, NULL, NULL, FALSE, NULL, NULL, NULL, &sti, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	ifstream fin(reportName);
	if (!fin.is_open() || fin.peek()==EOF)
		return 1;
	cout << "File: " << reportName << "\n";
	while (!fin.eof())
	{
		string str;
		getline(fin, str);
		cout << str << "\n";
	}
	system("pause");
	return 0;
}