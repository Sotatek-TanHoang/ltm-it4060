#include "Header.h"
#include "Interface.h"
#include "Util.h"
#include "Global.h"

map<string, int> USERS_STATUS;
map<string, int> ADMINS_LIST;

CRITICAL_SECTION USER_LOCK;
CRITICAL_SECTION ROUND_LOCK;
CRITICAL_SECTION ROOM_LOCK;
vector<QuizzInfor> listQuestion;
vector<RoomInfor> rooms;
int PORT_COUNTER;



unsigned __stdcall thread(void* param) {
	ThreadInformation* data = (ThreadInformation*)param;

	SOCKET connectedSocket = data->s;
	char* clientIP = data->clientIP;
	int clientPort = data->clientPort;

	char buff[BUFF_SIZE];
	char messageQueue[BUFF_SIZE * 2];
	messageQueue[0] = 0;
	buff[0] = 0;
	int ret;
	bool isStop = false;

	Request thisReq = {};
	thisReq.ip = string(clientIP);
	thisReq.port = clientPort;
	thisReq.threadId = 1;
	thisReq.isLoggedIn = false;
	thisReq.user = {};
	thisReq.socket = connectedSocket;

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
			printf("Receive from client[%s:%d]\n", clientIP, clientPort);

			appendQueue(buff, messageQueue);

			while (processData(buff, messageQueue, &thisReq)) {

				ret = send(connectedSocket, buff, strlen(buff), 0);
				if (ret == SOCKET_ERROR) {
					printf("Error %d:client exited.\n", WSAGetLastError());
					isStop = true;
					break;
				}
				
			}
		}
	}
	// logout user if exit unexpectedly.
	try
	{
		cout << thisReq.isLoggedIn << endl;
		if (thisReq.isLoggedIn) {
			EnterCriticalSection(&USER_LOCK);
			USERS_STATUS[thisReq.user.username] = 0;
			LeaveCriticalSection(&USER_LOCK);
		}
	}
	catch (const std::exception&)
	{
		cout << "log out error" << endl;
	}
	closesocket(connectedSocket);
	return 0;
}

int main(int argc, char* argv[])
{
	ADMINS_LIST["tan"] = USER_ADMIN;
	ADMINS_LIST["duc"] = USER_ADMIN;
	ADMINS_LIST["phu"] = USER_ADMIN;
	PORT_COUNTER = SERVER_PORT + 1;
	RoomInfor newRoom = {};
	newRoom.broadcastPort = PORT_COUNTER + 1;
	newRoom.roomName = "nhom-15";
	rooms.push_back(newRoom);

	// add some quizz.

	QuizzInfor one;
	one.answer = 1;
	one.options = {
		"ANSWER","A","B"
	};
	one.question = "Con nguoi co may chan";
	listQuestion.push_back(one);


	//init critical section.
	InitializeCriticalSection(&USER_LOCK);
	InitializeCriticalSection(&ROOM_LOCK);
	InitializeCriticalSection(&ROUND_LOCK);
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
	if (bind(listenSock, (sockaddr*)&serverAddr, sizeof(serverAddr)))
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
		connSocket = accept(listenSock, (sockaddr*)&clientAddr, &clientAddrLen);
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
			_beginthreadex(0, 0, thread, (void*)&data, 0, 0); //start thread
		}
	}
	
	closesocket(listenSock);

	WSACleanup();

	return 0;
}