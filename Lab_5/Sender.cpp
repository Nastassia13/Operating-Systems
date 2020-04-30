#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
using namespace std;

int main(int argc, char* argv[]) {
	char startEvent[100];
	sprintf(startEvent, "startEvent%s", argv[1]);
	HANDLE hStartEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, startEvent);
	if (hStartEvent == NULL)
		return GetLastError();
	HANDLE hMutex = OpenMutex(SYNCHRONIZE, FALSE, "Mutex");
	SetEvent(hStartEvent);
	while (true) {
		WaitForSingleObject(hMutex, INFINITE);
		cout << "1 - send message\n";
		cout << "2 - end\n";
		int choice;
		cin >> choice;
		if (choice == 1) {
			int count = 0;
			ifstream fileFirst(argv[0], ios::binary);
			string str;
			while (fileFirst.peek() != EOF) {
				getline(fileFirst, str);
				if (str == "")
					break;
				count++;
			}
			fileFirst.close();
			cout << count << endl;
			if (count >= atoi(argv[2])) {
				cout << "You can't send!\n";
				ReleaseMutex(hMutex);
				continue;
			}
			ofstream fileSecond(argv[0], ios::app | ios::binary);
			string mes;
			while (true)
			{
				cout << "Enter message: ";
				cin >> mes;
				if (mes.length() < 20) {
					fileSecond << mes << endl;
					break;
				}
				else
					cout << "Length must be < 20!\n";
			}
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
	CloseHandle(hMutex);
	return 0;
}