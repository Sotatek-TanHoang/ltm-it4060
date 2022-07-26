#pragma once
#include "Header.h"
#include "Interface.h"
#include "Global.h"
#include "Game.h"



string handleRequest(string message, Request* req);
string formatResponse(string code, string payload);
// handlers for each method.
// tan
string handleLogin(string username, Request* req);
string handleLogout(Request* req);
//duc
string adminCreateRoom(string payload, Request* req);
string adminStartRoom(string roomName, Request* req);
string adminEndGame(string roomName, Request* req);
string adminDeleteRoom(string roomName, Request* req);
//phu
string userGetRooms(string payload, Request * req);
string playerJoinRoom(string payload, Request* req);
string playerQuitRoom(string payload, Request* req);
string playerAnswerQuizz(string payload, Request* req);

bool processData(char* buff, char* queue, Request* req);

void appendQueue(char* buff, char* queue);