#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 7777
#define FILE_PATH "C:\\Works\\HTTPConnection\\index.html"

int main() {
    // Winsock 초기화
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "초기화 안됐는데." << endl;
        return 1;
    }

    // 소켓 생성
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "소켓 안만들어졌는데" << endl;
        WSACleanup();
        return 1;
    }

    // 서버 정보 설정
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(serverAddress.sin_addr)) <= 0) {
        cerr << "주소 틀렸거나 없는데" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // 서버에 연결
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        cerr << "연결 안되는데" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // 서버로부터 데이터 받기
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

    // 응답 출력
    std::cout << "Server response:" << std::endl;
    std::cout << response << std::endl;

    // 소켓 닫기
    closesocket(clientSocket);

    // Winsock 정리
    WSACleanup();

    return 0;
}