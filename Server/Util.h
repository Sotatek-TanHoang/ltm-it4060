#pragma once
#include "Header.h"
#include "Interface.h"
#include "Global.h"




string handleRequest(string message, Request * req);
// handlers for each method.
// tan
string handleLogin(string username, Request * req);
string handleLogout(Request * req);
//duc


//phu

bool processData(char * buff,char *queue,Request * req);

void appendQueue(char * buff, char * queue);