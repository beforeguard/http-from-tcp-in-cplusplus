#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
		return 1;
	}

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(42069);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listenSocket, (sockaddr*)&hint, sizeof(hint));
	listen(listenSocket, SOMAXCONN);

	SOCKET clientSocket = accept(listenSocket, NULL, NULL);

	closesocket(listenSocket);

	WSACleanup();
	return 0;
}