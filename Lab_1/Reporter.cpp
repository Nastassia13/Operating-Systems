#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include "..\Project2\Header.h"
using namespace std;


int main(int argc, char* argv[])
{
	if (argc != 3)
		return 1;
	LPSTR fileName = argv[0];
	LPSTR reportName = argv[1];
	double payment = atof(argv[2]);
	HANDLE hReport = CreateFile(reportName, GENERIC_WRITE, NULL, 0, CREATE_NEW, NULL, NULL);
	if (hReport == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hReport);
		return 1;
	}
	HANDLE hFile = CreateFile(fileName, GENERIC_READ, NULL, 0, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hReport);
		CloseHandle(hFile);
		return 1;
	}
	WriteFile(hReport, "Report on the file \"", 20, 0, NULL);
	WriteFile(hReport, fileName, sizeof(fileName), 0, NULL);
	WriteFile(hReport, "\"", 1, 0, NULL);
	WriteFile(hReport, "\n", 1, 0, NULL);
	employee emp;
	char str[100];
	DWORD dwBytes;
	ReadFile(hFile, &emp, sizeof(employee), &dwBytes, NULL);
	do
	{
		double salary = emp.hours*payment;
		sprintf(str, "%d %s %f %f\n", emp.num, emp.name, emp.hours, salary);
		dwBytes = (DWORD)strlen(str);
		if (!WriteFile(hReport, str,dwBytes , 0, NULL))
		{
			CloseHandle(hFile);
			CloseHandle(hReport);
			return 1;
		}
		ReadFile(hFile, &emp, sizeof(employee), &dwBytes, NULL);
	} while (dwBytes == sizeof(employee));
	CloseHandle(hFile);
	CloseHandle(hReport);
	return 0;
}