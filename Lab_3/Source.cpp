#include <iostream>
#include <Windows.h>
using namespace std;

typedef struct parametrs {
	int *array;
	int n;
	int min_i;
	int max_i;
	int mean = 0;
}parametrs;

CRITICAL_SECTION section;

void min_max(parametrs &p);
void average(parametrs &p);

int main() {
	parametrs p;
	cout << "Entaer n: ";
	cin >> p.n;
	p.array = new int[p.n];
	for (int i = 0; i < p.n; i++) {
		cout << "Enter element #" << i + 1 << ": ";
		cin >> p.array[i];
	}
	HANDLE hMinMax, hAverage;
	DWORD dwMinMax, dwAverage;
	int o;
	InitializeCriticalSection(&section);
	hMinMax = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)min_max, &p, 0, &dwMinMax);
	hAverage = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)average, &p, 0, &dwAverage);
	WaitForSingleObject(hMinMax, INFINITE);
	WaitForSingleObject(hAverage, INFINITE);
	DeleteCriticalSection(&section);
	p.array[p.min_i] = p.mean;
	p.array[p.max_i] = p.mean;
	for (int i = 0; i < p.n; i++)
		cout << p.array[i] << " ";
	cout << "\n";
	CloseHandle(hMinMax);
	CloseHandle(hAverage);
	system("pause");
	return 0;
}

void min_max(parametrs &p) {
	int min = INT_MAX, max = INT_MIN;
	for (int i = 0; i < p.n; i++) {
		if (p.array[i] < min) {
			min = p.array[i];
			p.min_i = i;
		}
		Sleep(7);
		if (p.array[i] > max) {
			max = p.array[i];
			p.max_i = i;
		}
		Sleep(7);
	}
	EnterCriticalSection(&section);
	cout << "Min: " << min << "\n";
	cout << "Max: " << max << "\n";
	LeaveCriticalSection(&section);
}

void average(parametrs &p) {
	for (int i = 0; i < p.n; i++) {
		p.mean += p.array[i];
		Sleep(12);
	}
	p.mean /= p.n;
	EnterCriticalSection(&section);
	cout << "Mean: " << p.mean << "\n";
	LeaveCriticalSection(&section);
}