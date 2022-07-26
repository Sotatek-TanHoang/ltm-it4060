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
#include <time.h>

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
#define PAYLOAD_SEPERATER "<br/>"

#define LOGIN_OK "10"
#define LOGIN_FAILED "11"
#define LOGOUT_OK "12"
#define LOGOUT_FAILED "13"

#define NOT_LOGGED_IN "14"
#define UNAUTHORIZED "15"
 
#define GAME_STARTED "30"
#define GAME_ENDED "34"

#define GET_ROOM_OK "20"
#define JOIN_ROOM_OK "21"
#define ROOM_NOT_EXIST "22"

#define ANWSER_OK "40"
#define ANWSER_TIMEOUT "41"


#define BROADCAST_GAME_RESULT "50"
#define BROADCAST_NEW_ROUND_QUIZZ "51"

#define ADMIN_CREATE_ROOM_OK "60"
#define ADMIN_DELETE_ROOM_OK "61"

#define ADMIN_EXISTED_ROOM "62"
#define ADMIN_NON_EXISTED_ROOM "63"

#define ACTION_FAILED "99"
#define ACTION_SUCCESS "90"