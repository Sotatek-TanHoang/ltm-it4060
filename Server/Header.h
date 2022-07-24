#pragma once
#include "stdio.h"
#include <iostream>
#include "conio.h"
#include "string.h"
#include "ws2tcpip.h"
#include "winsock2.h"
#include "process.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

#pragma comment(lib, "Ws2_32.lib")
using namespace std;

#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"
#define BUFF_SIZE 2048
#define DEMILITER "\r\n"

#define METHOD_LEN 4

// define method.
#define USER_LOGIN "USER"
#define USER_LOGOUT "QUIT"

#define ADMIN_CREATE_ROOM "CREA"
#define ADMIN_START_ROOM "STRT"
#define ADMIN_END_GAME "ENDQ"
#define ADMIN_DELETE_ROOM "DELE"

#define PLAYER_GET_ROOMS "GETQ"
#define PLAYER_JOIN_ROOM "JOIN"
#define PLAYER_ANSWER_QUIZZ "ANSW"
#define PLAYER_EXIT_ROOM "EXIT"
#define PLAYER_UPDATE_ROOM_INFORMATION "GETI"

// define responses.
