#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
using namespace std;

int main() {
	char fileName[20];
	int records;
	int process;
	cout << "Enter file name: ";
	cin >> fileName;
	cout << "Enter number of records: ";
	cin >> records;
	sprintf(fileName, "%s.bin", fileName);
	ofstream file(fileName, ios::binary);
	file.close();
	cout << "Enter number of processes: ";
	cin >> process;
	PROCESS_INFORMATION *pi = new PROCESS_INFORMATION[process];
	HANDLE *hStartEvent = new HANDLE[process];
	for (int i = 0; i < process; i++) {
		STARTUPINFO sti;
		ZeroMemory(&sti, sizeof(STARTUPINFO));
		char arg[100];
		sprintf(arg, "%s %d %d", fileName, i, records);
		if (!CreateProcess("..\\Debug\\Sender.exe", arg, NULL, NULL, FALSE, NULL, NULL, NULL, &sti, &pi[i])) {
			CloseHandle(pi->hProcess);
			return 1;
		}
		char ev[100];
		sprintf(ev, "startEvent%d", i);
		hStartEvent[i] = CreateEvent(NULL, TRUE, FALSE, ev);
		if (hStartEvent[i] == NULL) {
			CloseHandle(hStartEvent);
			CloseHandle(pi->hProcess);
			return 1;
		}
	}
	HANDLE hMutex = CreateMutex(NULL, FALSE, "Mutex");
	if (hMutex == INVALID_HANDLE_VALUE) {
		CloseHandle(hStartEvent);
		CloseHandle(pi->hProcess);
		CloseHandle(hMutex);
		return 1;
	}
	WaitForMultipleObjects(process, hStartEvent, TRUE, INFINITE);
	while (true) {
		WaitForSingleObject(hMutex, INFINITE);
		cout << "1 - read message\n";
		cout << "2 - end\n";
		int choice;
		cin >> choice;
		if (choice == 1) {
			ifstream fileFirst(fileName, ios::binary);
			string mes;
			int count = 0;
			while (fileFirst.peek() != EOF) {
				getline(fileFirst, mes);
				cout << mes << "\n";
				count++;
			}
			if (!count)
				cout << "Nothing to read!\n";
			fileFirst.close();
			ofstream fileSecond(fileName, ios::binary);
			fileSecond.close();
		}
		else if (choice == 2) {
			break;
		}
		else {
			cout << "Try again!\n";
		}
		ReleaseMutex(hMutex);
	}
	CloseHandle(hStartEvent);
	CloseHandle(pi->hProcess);
	CloseHandle(hMutex);
	delete[] pi;
	delete[] hStartEvent;
	system("pause");
	return 0;
}