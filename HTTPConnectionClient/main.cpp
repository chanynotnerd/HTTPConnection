#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 7777

int main() {
	// Winsock �ʱ�ȭ
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cerr << "�ʱ�ȭ �ȵƴµ�." << endl;
		return 1;
	}

	// ���� ����
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		cerr << "���� �ȸ�������µ�" << endl;
		WSACleanup();
		return 1;
	}

	// ���� ���� ����
	sockaddr_in serverAddress{};
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(SERVER_PORT);
	if (inet_pton(AF_INET, SERVER_IP, &(serverAddress.sin_addr)) <= 0) {
		cerr << "�ּ� Ʋ�Ȱų� ���µ�" << endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}

	/* SetSockopt()�� ���� Keep Alive �Ӽ� ����
	int keepAlive = 1;
	int keepAliveTime = 60000; // ���� �ð� (60��)
	int keepAliveInterval = 1000; // �˻� ���� (1��)

	if (setsockopt(clientSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&keepAlive, sizeof(int)) == SOCKET_ERROR ||
		setsockopt(clientSocket, IPPROTO_TCP, TCP_KEEPIDLE, (char*)&keepAliveTime, sizeof(int)) == SOCKET_ERROR ||
		setsockopt(clientSocket, IPPROTO_TCP, TCP_KEEPINTVL, (char*)&keepAliveInterval, sizeof(int)) == SOCKET_ERROR) {
		cerr << "Keep-Alive �Ӽ� ���� ����" << endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}
	*/
	// ������ ����
	if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
		cerr << "���� �ȵǴµ�" << endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}

	// GET ��û ����
	string getRequest = "GET / HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Connection: close\r\n"
		// "Connection: keep-alive\r\n"
		"\r\n";
	int bytesSent = send(clientSocket, getRequest.c_str(), getRequest.length(), 0);
	if (bytesSent == SOCKET_ERROR) {
		cerr << "��û ������ ����" << endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}

	// �����κ��� ������ �ޱ�
	char buffer[4096];
	string response;

	while (true) {
		memset(buffer, 0, sizeof(buffer));
		int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
		if (bytesRead <= 0) {
			break;
		}
		response += buffer;
		
		/*
		// Content-Length �ʵ� Ȯ��, ���� ��Ȳ���� ũ�� �ǹ� ���� �� ���� ����.
		size_t contentLengthPos = response.find("Content-Length: ");
		if (contentLengthPos != string::npos) {
			size_t endOfLinePos = response.find("\r\n", contentLengthPos);
			if (endOfLinePos != string::npos) {
				string contentLengthStr = response.substr(contentLengthPos + 16, endOfLinePos - contentLengthPos - 16);
				size_t contentLength = strtoul(contentLengthStr.c_str(), nullptr, 10);

				// ������ ������ ũ�Ⱑ Content-Length�� ������ ������ ����
				if (response.length() >= endOfLinePos + 4 + contentLength) {
					break;
				}
			}
		}
		*/
	}

	/* Select���� Ȱ���� Keep Alive ����
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(clientSocket, &readfds);

	// recv() ������ ���� ��� ���� �ްԲ� ����
	while (true) {
		timeval timeout;
		timeout.tv_sec = 5; // 5���� Ÿ�Ӿƿ� �ð� ����
		timeout.tv_usec = 0;

		int activity = select(clientSocket + 1, &readfds, NULL, NULL, &timeout);

		if (activity < 0) {
			cerr << "select() ȣ�� ����" << endl;
			break;
		}

		if (activity == 0) {
			cout << "Ÿ�Ӿƿ�, ������ ���� ��� ��..." << endl;
			continue;
		}

		if (FD_ISSET(clientSocket, &readfds)) {
			memset(buffer, 0, sizeof(buffer));
			int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
			if (bytesRead <= 0) {
				break;
			}
			response += buffer;
		}

	}
	*/
	// ���� ���
	cout << "Server response:" << endl;
	cout << response << endl;

	// ���� �ݱ�
	closesocket(clientSocket);
	// Winsock ����
	WSACleanup();

	return 0;
}