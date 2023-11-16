#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 7777
#define FILE_PATH "C:\\Works\\HTTPConnection\\index.html"

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

    // ������ ����
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        cerr << "���� �ȵǴµ�" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // �����κ��� ������ �ޱ�
    char buffer[4096];
    std::string response;

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0) {
            break;
        }
        response += buffer;
    }

    // ���� ���
    std::cout << "Server response:" << std::endl;
    std::cout << response << std::endl;

    // ���� �ݱ�
    closesocket(clientSocket);

    // Winsock ����
    WSACleanup();

    return 0;
}