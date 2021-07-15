#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, TCHAR *argv[]) {
	STARTUPINFO producerProcess;
	PROCESS_INFORMATION producerInfo;

	STARTUPINFO consumerProcess;
	PROCESS_INFORMATION consumerInfo;

	ZeroMemory(&producerProcess, sizeof(producerProcess));
	producerProcess.cb = sizeof(producerProcess);
	ZeroMemory(&producerInfo, sizeof(producerInfo));

	ZeroMemory(&consumerProcess, sizeof(consumerProcess));
	consumerProcess.cb = sizeof(consumerProcess);
	ZeroMemory(&consumerInfo, sizeof(consumerInfo));

	if (CreateProcess(L"C:\\Users\\John Foster\\Documents\\School Work\\WSU Comp Sci\\Summer 2021\\CS 4350\\CS4350-Project-John-Foster\\Debug\\producer.exe", 
		argv[1], NULL, NULL, FALSE, 0, NULL, NULL, &producerProcess, &producerInfo)) {
		cout << "producer created" << endl;
	}

	if (CreateProcess(L"C:\\Users\\John Foster\\Documents\\School Work\\WSU Comp Sci\\Summer 2021\\CS 4350\\CS4350-Project-John-Foster\\Debug\\consumer.exe",
		argv[1], NULL, NULL, FALSE, 0, NULL, NULL, &consumerProcess, &consumerInfo)) {
		cout << "consumer created" << endl;
	}

	cout << "Process ID: " << producerInfo.dwProcessId << endl;
	cout << "Thread ID: " << producerInfo.dwThreadId << endl;

	cout << "Process ID: " << consumerInfo.dwProcessId << endl;
	cout << "Thread ID: " << consumerInfo.dwThreadId << endl;

	WaitForSingleObject(producerInfo.hProcess, INFINITE);
	WaitForSingleObject(consumerInfo.hProcess, INFINITE);

	CloseHandle(producerInfo.hThread);
	CloseHandle(producerInfo.hProcess);

	CloseHandle(consumerInfo.hThread);
	CloseHandle(consumerInfo.hProcess);

	system("PAUSE");
}