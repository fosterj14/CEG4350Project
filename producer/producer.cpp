#include <iostream>
#include <fstream>
#include <Windows.h>
#include <istream>

using namespace std;

int pipeProcess();
void messageProcess();
void socketProcess();

int main() {
	cout << "Producer" << endl;

	//pipeProcess();
	//messageProcess();
	socketProcess();
	cout << "End of producer" << endl;
}

//works
int pipeProcess() {
	cout << "Starting pipe process" << endl;

	ofstream pipeInput;
	pipeInput.open("pipeInput.txt");

	int* randArr = NULL;
	randArr = new int[100];
	srand(time(0));

	for (int i = 0; i < 100; i++)
	{
		int temp = rand() % 1000;
		randArr[i] = temp;
		pipeInput << temp << endl;
	}

	HANDLE pipe = CreateNamedPipe(L"\\\\.\\pipe\\pipeTest", PIPE_ACCESS_OUTBOUND, PIPE_TYPE_BYTE, 1, 0, 0, 0, NULL);
	if (pipe == NULL || pipe == INVALID_HANDLE_VALUE) {
		cout << "Failed to create outbound pipe instance";
		system("pause");
		return 1;
	}

	cout << "Waiting for client to conenct to pipe" << endl;

	BOOL result = ConnectNamedPipe(pipe, NULL);
	if (!result) {
		cout << "Failed to make connection on names pipe" << endl;
		CloseHandle(pipe);
		system("pause");
		return 1;
	}
	cout << "Sending data to pipe..." << endl;
	DWORD numBytesWritten = 0;

	cout << "SIZEOF ARR: " << sizeof(randArr) << endl;
	result = WriteFile(pipe, randArr, 400, &numBytesWritten, NULL);
	//result = WriteFile(pipe, data, wcslen(data) * sizeof(wchar_t), &numBytesWritten, NULL);

	if (!result) {
		cout << "Failed to send data" << endl;
	}

	CloseHandle(pipe);
	cout << "Producer side of pipe finished" << endl;
	return 0;
}

void messageProcess() {
	cout << "Starting message process" << endl;
	ofstream messageInput;
	messageInput.open("messageInput.txt");

	int* randArr = NULL;
	randArr = new int[100];
	srand(time(0));

	for (int i = 0; i < 100; i++)
	{
		int temp = rand() % 1000;
		randArr[i] = temp;
		messageInput << temp << endl;
	}

}

void socketProcess() {
	cout << "Starting socket process" << endl;

	ofstream socketInput;
	socketInput.open("socketInput.txt");

	int* randArr = NULL;
	randArr = new int[100];
	srand(time(0));

	for (int i = 0; i < 100; i++)
	{
		int temp = rand() % 1000;
		randArr[i] = temp;
		socketInput << temp << endl;
	}

	cout << "MESSAGE SIZE: " << socketInput.tellp() << endl;

	const int num = 512;

	struct sockaddr_in si_other;
	int s, slen = sizeof(si_other);
	char buffer[num];
	char message[num];
	WSADATA wsa;

	cout << "Initializing producer winsock" << endl;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		cout << "failed winsock startup" << endl;
	}
	cout << "PRODUCER INITIALIZED" << endl;

	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) {
		cout << "Poducer socket failed" << endl;
	}
	cout << "PRODUCER SOCKET MADE" << endl;

	memset((char*)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(8888);
	si_other.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	char convertedArr[num];

	cin >> message;

	while (true) {
		if (sendto(s, message, num, 0, (struct sockaddr*)&si_other, slen) == SOCKET_ERROR) {
			cout << "sendto failed" << endl;
		}

		memset(buffer, NULL, num);

		if (recvfrom(s, buffer, num, 0, (struct sockaddr*)&si_other, &slen) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		puts(buffer);
	}

	closesocket(s);
	WSACleanup();
}