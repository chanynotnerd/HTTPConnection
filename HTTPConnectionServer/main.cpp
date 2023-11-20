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
        return "HTTP/1.1 404 Not Found\r\n\r\n"; // 파일을 찾을 수 없는 경우 404 응답 반환
    }
    file.close();

    return "HTTP/1.1 200 OK\r\n\r\nHello, Client!"; // 파일을 찾은 경우 200 응답과 파일 내용 반환
    // return "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\n\r\n" + response; // 파일을 찾은 경우 200 응답과 파일 내용 반환 (Connection: keep-alive 추가)
}

int main()
{
    // Winsock 초기화
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "초기화 안됐는데." << endl;
        return 1;
    }

    // 소켓 생성
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        cerr << "소켓 안만들어졌는데" << endl;
        return 1;
    }

    // 서버 정보 설정
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(serverAddress.sin_addr)) <= 0)
    {
        cerr << "주소 틀렸거나 없는데" << endl;
        return 1;
    }

    // 소켓 바인딩
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
    {
        cerr << "바인드 에러인데" << endl;
        return 1;
    }

    // 소켓 리스닝
    if (listen(serverSocket, SOMAXCONN) < 0)
    {
        cerr << "리슨 안되는데" << endl;
        return 1;
    }

    cout << "Server Running " << SERVER_IP << ":" << SERVER_PORT << endl;

    while (true) {
        // 클라이언트 연결 수락
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0)
        {
            cerr << "연결 수락 안되는데" << endl;
            continue;
        }

        // 클라이언트로부터 요청 받기
        char buffer[4096];
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0)
        {
            closesocket(clientSocket);
            continue;
        }

        // 요청 메시지 파싱
        string request(buffer);

        // URL 경로 추출
        string url;
        size_t start = request.find("GET ") + 4;
        size_t end = request.find(" HTTP/1.1");
        if (start != string::npos && end != string::npos)
        {
            url = request.substr(start, end - start);
        }

        // 응답 생성
        string response = getResponse(FILE_PATH);

        // 클라이언트에 응답 전송
        send(clientSocket, response.c_str(), response.length(), 0);

        // 클라이언트 소켓 닫기
        closesocket(clientSocket);
    }

    // 소켓 닫기
    closesocket(serverSocket);

    return 0;
    /*  Keep Alive 루프문 설정 시
    while (true) {
        // 클라이언트 연결 수락
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0)
        {
            cerr << "연결 수락 안되는데" << endl;
            continue;
        }

        while (true) {
            // 클라이언트로부터 요청 받기
            char buffer[4096];
            memset(buffer, 0, sizeof(buffer));
            int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesRead <= 0)
            {
                break;
            }

            // 요청 메시지 파싱
            string request(buffer);

            // URL 경로 추출
            string url;
            size_t start = request.find("GET ") + 4;
            size_t end = request.find(" HTTP/1.1");
            if (start != string::npos && end != string::npos)
            {
                url = request.substr(start, end - start);
            }

            // 응답 생성
            string response = getResponse(FILE_PATH);

            // 클라이언트에 응답 전송
            send(clientSocket, response.c_str(), response.length(), 0);
        }

        // 클라이언트 소켓 닫기
        closesocket(clientSocket);
    }
    */
}