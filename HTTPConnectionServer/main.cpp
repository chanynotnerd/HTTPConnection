#include <iostream>
#include <fstream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 7777
#define FILE_PATH "C:\\Works\\HTTPConnection\\welcome.html"


string getResponse(const std::string& filePath)
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        return "HTTP/1.1 404 Not Found\r\n\r\n"; // ������ ã�� �� ���� ��� 404 ���� ��ȯ
    }
    file.close();

    return "HTTP/1.1 200 OK\r\n\r\nHello, Client!"; // ������ ã�� ��� 200 ����� ���� ���� ��ȯ
    // return "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\n\r\n" + response; // ������ ã�� ��� 200 ����� ���� ���� ��ȯ (Connection: keep-alive �߰�)
}

int main()
{
    // Winsock �ʱ�ȭ
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "�ʱ�ȭ �ȵƴµ�." << endl;
        return 1;
    }

    // ���� ����
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        cerr << "���� �ȸ�������µ�" << endl;
        return 1;
    }

    // ���� ���� ����
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(serverAddress.sin_addr)) <= 0)
    {
        cerr << "�ּ� Ʋ�Ȱų� ���µ�" << endl;
        return 1;
    }

    // ���� ���ε�
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
    {
        cerr << "���ε� �����ε�" << endl;
        return 1;
    }

    // ���� ������
    if (listen(serverSocket, SOMAXCONN) < 0)
    {
        cerr << "���� �ȵǴµ�" << endl;
        return 1;
    }

    cout << "Server Running " << SERVER_IP << ":" << SERVER_PORT << endl;

    while (true) {
        // Ŭ���̾�Ʈ ���� ����
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0)
        {
            cerr << "���� ���� �ȵǴµ�" << endl;
            continue;
        }

        // Ŭ���̾�Ʈ�κ��� ��û �ޱ�
        char buffer[4096];
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0)
        {
            closesocket(clientSocket);
            continue;
        }

        // ��û �޽��� �Ľ�
        string request(buffer);

        // URL ��� ����
        string url;
        size_t start = request.find("GET ") + 4;
        size_t end = request.find(" HTTP/1.1");
        if (start != string::npos && end != string::npos)
        {
            url = request.substr(start, end - start);
        }

        // ���� ����
        string response = getResponse(FILE_PATH);

        // Ŭ���̾�Ʈ�� ���� ����
        send(clientSocket, response.c_str(), response.length(), 0);

        // Ŭ���̾�Ʈ ���� �ݱ�
        closesocket(clientSocket);
    }

    // ���� �ݱ�
    closesocket(serverSocket);

    return 0;
    /*  Keep Alive ������ ���� ��
    while (true) {
        // Ŭ���̾�Ʈ ���� ����
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0)
        {
            cerr << "���� ���� �ȵǴµ�" << endl;
            continue;
        }

        while (true) {
            // Ŭ���̾�Ʈ�κ��� ��û �ޱ�
            char buffer[4096];
            memset(buffer, 0, sizeof(buffer));
            int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesRead <= 0)
            {
                break;
            }

            // ��û �޽��� �Ľ�
            string request(buffer);

            // URL ��� ����
            string url;
            size_t start = request.find("GET ") + 4;
            size_t end = request.find(" HTTP/1.1");
            if (start != string::npos && end != string::npos)
            {
                url = request.substr(start, end - start);
            }

            // ���� ����
            string response = getResponse(FILE_PATH);

            // Ŭ���̾�Ʈ�� ���� ����
            send(clientSocket, response.c_str(), response.length(), 0);
        }

        // Ŭ���̾�Ʈ ���� �ݱ�
        closesocket(clientSocket);
    }
    */
}