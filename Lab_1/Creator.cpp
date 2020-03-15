#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include "..\Project2\Header.h"
using namespace std;


int main(int argc, char* argv[])
{
	if (argc != 2) 
		return 1;
	LPSTR fileName = argv[0];
	int numberRecords = atoi(argv[1]);
	HANDLE hFile = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, NULL, 0, CREATE_NEW, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return 1;
	}
	for (int i = 0; i < numberRecords; i++) 
	{
		employee emp;
		cout << "Enter employee num:\n";
		cin >> emp.num;
		cout << "Enter employee name:\n";
		cin >> emp.name;
		cout << "Enter employee hours:\n";
		cin >> emp.hours;
		DWORD dwWrite = 0;
		if (!WriteFile(hFile, &emp, sizeof(employee), &dwWrite, NULL))
		{
			CloseHandle(hFile);
			return 1;
		}
	}
	CloseHandle(hFile);
	return 0;
}