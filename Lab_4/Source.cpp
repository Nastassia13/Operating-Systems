#include <iostream>
#include <Windows.h>
#include <stdlib.h>
using namespace std;

int* arr;
int n;
HANDLE hStartEvent;
HANDLE* hStopEvent;
HANDLE* hTerm;
HANDLE hCont;

CRITICAL_SECTION section;

int marker(int t);

int main() {
	cout << "Enter number of elements (n): ";
	cin >> n;
	arr = new int[n]();
	int t;
	cout << "Enter number of threads (t): ";
	cin >> t;

	hStopEvent = new HANDLE[t];
	hTerm = new HANDLE[t];
	HANDLE* hThread = new HANDLE[t];
	DWORD*	dwThread = new DWORD[t];
	bool* terminate = new bool[t];

	InitializeCriticalSection(&section);

	hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (hStartEvent == NULL)
		return GetLastError();
	hCont = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (hCont == NULL)
		return GetLastError();

	for (int i = 0; i < t; i++) {
		hThread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)marker, (LPVOID)(i + 1), 0, &dwThread[i]);
		hStopEvent[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (hStopEvent[i] == NULL)
			return GetLastError();
		hTerm[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (hTerm[i] == NULL)
			return GetLastError();
		terminate[i] = false;
	}

	int tTemp = t;
	while (tTemp) {
		SetEvent(hStartEvent);
		WaitForMultipleObjects(t, hStopEvent, TRUE, INFINITE);

		cout << "Array: ";
		for (int i = 0; i < n; i++) {
			cout << arr[i] << " ";
		}

		cout << "\n";
		int threadEnd;
		while (true) {
			cout << "\nEnter the number of the thread which you want to terminate: ";
			cin >> threadEnd;
			if (terminate[threadEnd - 1])
				cout << "Error! Try again..\n";
			else
				break;
		}

		terminate[threadEnd - 1] = true;
		SetEvent(hTerm[threadEnd - 1]);
		SetEvent(hCont);
		WaitForSingleObject(hThread[threadEnd - 1], INFINITE);

		cout << "\nArray: ";
		for (int i = 0; i < n; i++) {
			cout << arr[i] << " ";
		}
		cout << "\n";

		ResetEvent(hCont);
		for (int i = 0; i < t; i++) {
			if (!terminate[i]) {
				ResetEvent(hStopEvent[i]);
			}
		}
		tTemp--;
	}
	WaitForMultipleObjects(n, hThread, TRUE, INFINITE);
	DeleteCriticalSection(&section);

	delete[] hStopEvent;
	delete[] hTerm;
	delete[] hThread;
	delete[] dwThread;
	CloseHandle(hStartEvent);
	CloseHandle(hStopEvent);
	CloseHandle(hTerm);
	CloseHandle(hCont);
	CloseHandle(hThread);
	system("pause");
	return 0;
}

int marker(int t) {
	int count = 0;
	srand(t);
	while (true)
	{
		WaitForSingleObject(hStartEvent, INFINITE);
		EnterCriticalSection(&section);
		int a = rand();
		a %= n;
		if (arr[a] == 0) {
			Sleep(5);
			arr[a] = t;
			LeaveCriticalSection(&section);
			count++;
			Sleep(5);
		}
		else {
			cout << "\nMy serial number: " << t << "\n";
			cout << "Number of marked items: " << count << "\n";
			cout << "Index of an array element that cannot be marked: " << a << "\n\n";
			LeaveCriticalSection(&section);

			SetEvent(hStopEvent[t - 1]);
			WaitForSingleObject(hCont, INFINITE);
			ResetEvent(hStartEvent);

			if (WaitForSingleObject(hTerm[t - 1], 0) == WAIT_OBJECT_0) {
				EnterCriticalSection(&section);
				for (int i = 0; i < n; i++) {
					if (arr[i] == t)
						arr[i] = 0;
				}
				cout << "#" << t << " terminated...\n";
				LeaveCriticalSection(&section);
				break;
			}
		}
	}
	return 0;
}