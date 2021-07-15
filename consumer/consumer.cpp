#define _WINSOCKAPI_ 
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <WinSock2.h>

using namespace std;

int pipeProcess();
void messageProcess();
void socketProcess();

int main() {
	cout << "Consumer" << endl;

	//pipeProcess();
	//messageProcess();
	socketProcess();

}

//works
int pipeProcess() {
	ofstream pipeOutput;
	pipeOutput.open("pipeOutput.txt");

	HANDLE pipe = CreateFile(L"\\\\.\\pipe\\pipeTest", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (pipe == INVALID_HANDLE_VALUE) {
		cout << "Failed to connect to pipe." << endl;
		system("pause");
		return 1;
	}
	cout << "Reading data from pipe..." << endl;

	int buffer[128];
	DWORD numBytesRead = 0;
	BOOL result = ReadFile(pipe, buffer, 127 * sizeof(int), &numBytesRead, NULL);

	if (result) {
		for (int i = 0; i < 100; i++)
		{
			pipeOutput << buffer[i] << endl;
		}
	}
	else {
		cout << "Failed to read data from the pipe" << endl;
	}

	CloseHandle(pipe);
	cout << "Consumer pipe side done" << endl;
	system("pause");
	return 0;
}

void messageProcess() {
	cout << "Consumer message process" << endl;
	ofstream messageOutput;
	messageOutput.open("messageOutput.txt");
}

//https://www.binarytides.com/udp-socket-programming-in-winsock/
void socketProcess() {
	cout << "Consumer socket process" << endl;

	ofstream socketOutput;
	socketOutput.open("socketOutput.txt");

	const int num = 512;

	SOCKET s;
	struct sockaddr_in consumer, si_other;
	int slen, recv_len;
	char buffer[num];
	WSADATA wsa;

	slen = sizeof(si_other);

	cout << "initializing consumer Winsock..." << endl;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		cout << "Failed to initialize Winsock" << endl;
	}
	cout << "CONSUMER INITIALIZED" << endl;

	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
		cout << "Could not create socket" << endl;
	}
	cout << "SOCKET CREATED" << endl;
	consumer.sin_family = AF_INET;
	consumer.sin_addr.s_addr = INADDR_ANY;
	consumer.sin_port = htons(8888);

	if (bind(s, (struct sockaddr*)&consumer, sizeof(consumer)) == SOCKET_ERROR) {
		cout << "bind failed" << endl;
	}
	cout << "BIND DONE" << endl;

	while (true) {
		cout << "waiting for data" << endl;
		fflush(stdout);

		memset(buffer, NULL, num);

		if ((recv_len = recvfrom(s, buffer, num, 0, (struct sockaddr*)&si_other, &slen)) == SOCKET_ERROR) {
			cout << "recvfrom failed" << endl;
		}
		cout << "SOCKET TRANSFER RECIEVED" << endl;

		cout << buffer << endl;
		
		for (int i = 0; i < num; i++)
		{
			socketOutput << buffer[i]-'0' << endl;
		}

		if (sendto(s, buffer, recv_len, 0, (struct sockaddr*)&si_other, slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
	}

	closesocket(s);
	WSACleanup();
}