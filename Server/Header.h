#pragma once
#include "stdio.h"
#include <iostream>
#include "conio.h"
#include "string.h"
#include "ws2tcpip.h"
#include "winsock2.h"
#include "process.h"
#include <string>
#include "Interface.h"

#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"
#define BUFF_SIZE 2048
#define DEMILITER "\r\n"

#pragma comment (lib, "Ws2_32.lib")
using namespace std;
