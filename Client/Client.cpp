// UDP_SERVER.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<stdio.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#pragma comment(lib,"Ws2_32.lib")


#define DELIMITER "\r\n"
#define BUFF_SIZE 2048
u_short SERVER_PORT;
char * SERVER_ADDRESS;

int main(int argc, char **argv)
{
	if (argc < 3) {
		printf("provide both address and port!");
		return -1;
	}
	SERVER_ADDRESS = argv[1];
	SERVER_PORT = atoi(argv[2]);

	// init
	WSADATA wsaData;

	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	SOCKET client;
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (client == INVALID_SOCKET) {
		printf("init socket failed,%d", WSAGetLastError());
		return 500;
	}

	int tv = 10000; // 10s.
	setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char*)(&tv), sizeof(int));

	// server addr
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_ADDRESS, &serverAddr.sin_addr);
	// connect with server
	if (connect(client, (sockaddr*)&serverAddr, sizeof(serverAddr))) {
		printf("cannot connect to server");
		return 500;
	}
	printf("server connected\n");

	// interact with server.



	char buff[BUFF_SIZE];
	int ret, messageLen;
	while (1) {
		printf("send to server: ");
		gets_s(buff, BUFF_SIZE);
		sprintf(buff,"%s\r\n", buff);
		messageLen = strlen(buff);
		if (messageLen == 0) {
			printf("empty input, exiting!");
			break;
		}
		ret = send(client, buff, messageLen, 0);
		if (ret == SOCKET_ERROR) {
			printf("cannot send &d \n", WSAGetLastError());

		}
		ret = recv(client, buff, BUFF_SIZE, 0);
		if (ret == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAETIMEDOUT) {
				printf("TIME OUT! \n");
			}
			else printf("cannot send &d \n", WSAGetLastError());

		}
		else if (strlen(buff) > 0) {
			buff[ret] = 0;
			printf("Recv from server: %s. \n", buff);
		}
	}
	// clean up
	closesocket(client);
	WSACleanup();
	return 0;
}

