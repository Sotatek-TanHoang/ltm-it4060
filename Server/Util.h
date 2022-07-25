#pragma once
#include "Header.h"
#include "Interface.h"
#include "Global.h"
#include "Broadcast.h"



string handleRequest(string message, Request* req);
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
string userGetRoom(Request *req);
string userJoinRoom(string roomName, Request *req);
bool processData(char* buff, char* queue, Request* req);

void appendQueue(char* buff, char* queue);