#pragma once
#include "Header.h"
#include "Interface.h"

void DieWithError(char *errorMessage);

class Broadcaster
{
public:
	sockaddr_in broadcastAddr;
	SOCKET broadcastSock;
	Broadcaster(string address, int port)
	{
		memset(&broadcastAddr, 0, sizeof(broadcastAddr));		/* Zero out structure */
		broadcastAddr.sin_family = AF_INET;						/* Internet address family */
		broadcastAddr.sin_addr.s_addr = inet_addr(&address[0]); /* Broadcast IP address */
		broadcastAddr.sin_port = htons(port);					/* Broadcast port */

		SOCKET sock;				  /* Socket */
		BOOL broadcastPermission; /* Socket opt to set permission to broadcast */

		if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
			DieWithError("socket() failed");

		/* Set socket to allow broadcast */
		broadcastPermission = TRUE;
		if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&broadcastPermission, sizeof(broadcastPermission)) < 0)
			DieWithError("setsockopt() failed");
		broadcastSock = sock;
		return;
	}
	void bulkSend(string message)
	{
		const char *buff = &message[0];
		if (sendto(broadcastSock, buff, strlen(buff), 0, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr)) != strlen(buff))
			DieWithError("sendto() sent a different number of bytes than expected");
		printf("sented %s", &message[0]);
	}
	void cleanup()
	{
		closesocket(broadcastSock);
	}
};