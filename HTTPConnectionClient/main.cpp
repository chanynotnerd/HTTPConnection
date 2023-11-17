#include <iostream>
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
    }

    // ���� ���
    cout << "Server response:" << endl;
    cout << response << endl;

    
    // ���� �ݱ�
    closesocket(clientSocket);

    // Winsock ����
    WSACleanup();
    
    return 0;
}