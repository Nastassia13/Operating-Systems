#include <Windows.h>
#include <iostream>
using namespace std;
int main() {
	STARTUPINFO sti;
	ZeroMemory(&sti, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	CreateProcess("E:\\2 ����\\4 �������\\Project1\\Debug\\Project2.exe", NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &sti, &pi);
	system("pause");
	return 0;
}