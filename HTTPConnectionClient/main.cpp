#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 7777

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

	/* SetSockopt()를 통한 Keep Alive 속성 설정
	int keepAlive = 1;
	int keepAliveTime = 60000; // 유지 시간 (60초)
	int keepAliveInterval = 1000; // 검사 간격 (1초)

	if (setsockopt(clientSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&keepAlive, sizeof(int)) == SOCKET_ERROR ||
		setsockopt(clientSocket, IPPROTO_TCP, TCP_KEEPIDLE, (char*)&keepAliveTime, sizeof(int)) == SOCKET_ERROR ||
		setsockopt(clientSocket, IPPROTO_TCP, TCP_KEEPINTVL, (char*)&keepAliveInterval, sizeof(int)) == SOCKET_ERROR) {
		cerr << "Keep-Alive 속성 설정 실패" << endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}
	*/
	// 서버에 연결
	if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
		cerr << "연결 안되는데" << endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}

	// GET 요청 전송
	string getRequest = "GET / HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Connection: close\r\n"
		// "Connection: keep-alive\r\n"
		"\r\n";
	int bytesSent = send(clientSocket, getRequest.c_str(), getRequest.length(), 0);
	if (bytesSent == SOCKET_ERROR) {
		cerr << "요청 보내기 실패" << endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}

	// 서버로부터 데이터 받기
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
		// Content-Length 필드 확인, 현재 상황에서 크게 의미 없는 것 같아 삭제.
		size_t contentLengthPos = response.find("Content-Length: ");
		if (contentLengthPos != string::npos) {
			size_t endOfLinePos = response.find("\r\n", contentLengthPos);
			if (endOfLinePos != string::npos) {
				string contentLengthStr = response.substr(contentLengthPos + 16, endOfLinePos - contentLengthPos - 16);
				size_t contentLength = strtoul(contentLengthStr.c_str(), nullptr, 10);

				// 수신한 데이터 크기가 Content-Length와 같으면 루프를 종료
				if (response.length() >= endOfLinePos + 4 + contentLength) {
					break;
				}
			}
		}
		*/
	}

	/* Select문을 활용한 Keep Alive 루프
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(clientSocket, &readfds);

	// recv() 루프로 돌려 상시 응답 받게끔 수정
	while (true) {
		timeval timeout;
		timeout.tv_sec = 5; // 5초의 타임아웃 시간 설정
		timeout.tv_usec = 0;

		int activity = select(clientSocket + 1, &readfds, NULL, NULL, &timeout);

		if (activity < 0) {
			cerr << "select() 호출 에러" << endl;
			break;
		}

		if (activity == 0) {
			cout << "타임아웃, 데이터 수신 대기 중..." << endl;
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
	// 응답 출력
	cout << "Server response:" << endl;
	cout << response << endl;

	// 소켓 닫기
	closesocket(clientSocket);
	// Winsock 정리
	WSACleanup();

	return 0;
}