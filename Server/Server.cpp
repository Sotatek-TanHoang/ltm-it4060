#include "Header.h"
#include "Util.h"
#include "Methods.h"
#include "Interface.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define MAX_QUESTION 100

using namespace std;



QuizzInfor listQuestion[MAX_QUESTION];

unsigned __stdcall thread(void *param) {
	ThreadInformation *data= (ThreadInformation *)param;

	SOCKET connectedSocket = data->s;
	char * clientIP = data->clientIP;
	int clientPort = data->clientPort;
	
	char buff[BUFF_SIZE];
	char messageQueue[BUFF_SIZE *2];
	messageQueue[0] = 0;
	buff[0] = 0;
	int ret;
	bool isStop = false;
	while (!isStop) {
		ret = recv(connectedSocket, buff, BUFF_SIZE, 0);
		
		if (ret == SOCKET_ERROR)
		{
			printf("Error %d: Cannot receive data.\n", WSAGetLastError());
			isStop = true;
			break;
		}
		else if (ret == 0)
		{
			printf("Client disconnects.\n");
			isStop = true;
			break;
		}
		else if (strlen(buff) > 0) {
			buff[ret] = 0;
			printf("Receive from client[%s:%d]\n",clientIP,clientPort);
		
			appendQueue(buff, messageQueue);
		
			while (processData(buff, messageQueue)) {
				
				ret = send(connectedSocket, buff, strlen(buff), 0);
				if (ret == SOCKET_ERROR) {
					printf("Error %d: Cannot send data.\n", WSAGetLastError());
					isStop = true;
					break;
				}
			}
		}
	}

	closesocket(connectedSocket);
	return 0;
}

int count1 = 0;

void printListQuestion() {
	for (int i = 0;i < count1;i++) {
		cout << listQuestion[i].question << " " << listQuestion[i].answer << " " << listQuestion[i].options << endl;
	}
}

/* The readQuestion function is to read question from file
* @param no param
* @return no return value
*/
void readQuestion() {
	ifstream inputFile("question.txt");
	string line;
	if (inputFile)
	{
		while (getline(inputFile, line))
		{
			QuizzInfor q;
			istringstream ss(line);

			ss >> q.question;

			ss >> q.answer;
			ss >> q.options[0];
			ss >> q.options[1];
			ss >> q.options[2];
			ss >> q.options[3];

			if (ss)
			{
				listQuestion[count1++] = q;
			}
		}
	}
	else
	{
		cout << "File cannot be opened" << std::endl;
	}
}

int main(int argc, char* argv[])
{
	//Step 1: Initiate WinSock
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData)) {
		printf("Winsock 2.2 is not supported\n");
		return 0;
	}

	//Step 2: Construct socket	
	SOCKET listenSock;
	listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//Step 3: Bind address to socket
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_ADDR, &serverAddr.sin_addr);
	if (bind(listenSock, (sockaddr *)&serverAddr, sizeof(serverAddr)))
	{
		printf("Error %d: Cannot associate a local address with server socket.", WSAGetLastError());
		return 0;
	}

	//Step 4: Listen request from client
	if (listen(listenSock, 10)) {
		printf("Error %d: Cannot place server socket in state LISTEN.", WSAGetLastError());
		return 0;
	}

	printf("Server started!\n");

	//Step 5: Communicate with client
	SOCKET connSocket;
	sockaddr_in clientAddr;
	char clientIP[INET_ADDRSTRLEN];
	int clientAddrLen = sizeof(clientAddr), clientPort;
	while (1) {
		connSocket = accept(listenSock, (sockaddr *)& clientAddr, &clientAddrLen);
		if (connSocket == SOCKET_ERROR)
			printf("Error %d: Cannot permit incoming connection.\n", WSAGetLastError());
		else {
			inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
			clientPort = ntohs(clientAddr.sin_port);
			printf("Accept incoming connection from %s:%d\n", clientIP, clientPort);
			ThreadInformation data = {};
			data.s = connSocket;
			data.clientIP = clientIP;
			data.clientPort = clientPort;
			_beginthreadex(0, 0, thread, (void *)&data, 0, 0); //start thread
		}
	}

	closesocket(listenSock);

	WSACleanup();

	return 0;
}