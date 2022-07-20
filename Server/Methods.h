#pragma once
#include "Header.h"

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


string handleRequest(string message,User * user);
// handlers for each method.