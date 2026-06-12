#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

void ReadLinesFromSocket(SOCKET clientSocket)
{
	std::string buffer;

	while (true)
	{
		char temp[1024];

		int bytesReceived = recv(clientSocket, temp, sizeof(temp) - 1, 0);

		if (bytesReceived > 0)
		{
			buffer.append(temp, bytesReceived);

			size_t pos;
			while ((pos = buffer.find('\n')) != std::string::npos)
			{
				std::string line = buffer.substr(0, pos);
				buffer.erase(0, pos + 1);

				std::cout << "Line: " << line << "\n";
			}
		}
		else if (bytesReceived == 0)
		{
			// Client closed connection
			std::cout << "Client disconnected\n";
			break;
		}
		else
		{
			// Error
			std::cerr << "recv failed: " << WSAGetLastError() << "\n";
			break;
		}
	}
}

int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
		return 1;
	}

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET) {
		std::cerr << "Socket creation failed: " << WSAGetLastError() << "\n";
		WSACleanup();
		return 1;
	}

	sockaddr_in serverService{};
	serverService.sin_family = AF_INET;
	serverService.sin_addr.s_addr = INADDR_ANY;
	serverService.sin_port = htons(42069);

	if (bind(listenSocket, (SOCKADDR*)&serverService, sizeof(serverService)) == SOCKET_ERROR) {
		std::cerr << "Bind failed: " << WSAGetLastError() << "\n";
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cerr << "Listen failed: " << WSAGetLastError() << "\n";
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	std::cout << "Waiting for client connections on port 42069...\n";

	sockaddr_in clientAddr{};
	int clientAddrLen = sizeof(clientAddr);

	SOCKET clientSocket = accept(listenSocket, (SOCKADDR*)&clientAddr, &clientAddrLen);

	if (clientSocket == INVALID_SOCKET) {
		std::cerr << "Accept failed with error: " << WSAGetLastError() << "\n";
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	ReadLinesFromSocket(clientSocket);

	closesocket(clientSocket);
	closesocket(listenSocket);

	WSACleanup();
	return 0;
}