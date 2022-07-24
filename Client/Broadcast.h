#pragma once

#include "Header.h"
#define MAXRECVSTRING 255
void DieWithError(char *errorMessage);
class BroadcastReceiver
{
private:
	struct sockaddr_in broadcastAddr;
	SOCKET broadcastSock;
	char recvString[MAXRECVSTRING + 1];

public:
	BroadcastReceiver(string address, int port)
	{
		int sock;						  /* Socket */
		struct sockaddr_in broadcastAddr; /* Broadcast Address */
										  /* Create a best-effort datagram socket using UDP */
		if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
			DieWithError("socket() failed");

		/* Construct bind structure */
		memset(&broadcastAddr, 0, sizeof(broadcastAddr));
		broadcastAddr.sin_family = AF_INET;
		broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		broadcastAddr.sin_port = htons(port);

		/* Bind to the broadcast port */
		if (bind(sock, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr)) < 0)
			DieWithError("bind() failed");

		broadcastSock = sock;

		return;
	}
	string receiveFromBroadcast()
	{
		int recvLen = 0;
		if ((recvLen = recvfrom(broadcastSock, recvString, MAXRECVSTRING, 0, NULL, 0)) < 0)
			DieWithError("recvfrom() failed");

		recvString[recvLen] = '\0';

		return string(recvString);
	}
	void cleanup()
	{
		closesocket(broadcastSock);
		return;
	}
};
