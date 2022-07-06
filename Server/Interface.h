#pragma once
#include "Header.h"

struct ThreadInformation {
	SOCKET s;
	char * clientIP="";
	int clientPort=0;
};
