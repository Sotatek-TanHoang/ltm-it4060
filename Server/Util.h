#pragma once
#include "Header.h"
#include "Interface.h"
#include "Global.h"
#include "Game.h"


//@function:  handleRequest
//	@param: message-user message
//		    req- request object from user
//  @purpose: handle all incomming request to handlers
string handleRequest(string message, Request* req);
//@function:  formatReponse
//	@param: code-method sent to server
//		    payload- payload seperated in demiliter of <br/>
//  @purpose: handle all incomming request to handlers
string formatResponse(string code, string payload);
// handlers for each method.
// tan

//@function:  handleLogin
//	@param: message-user message
//		    req- request object from user
//  @purpose: handle admin and player login
string handleLogin(string username, Request* req);
//@function:  handleLogout
//	@param: message-user message
//		    req- request object from user
//  @purpose: handle admin and user logout
string handleLogout(Request* req);
//duc
//@function:  adminCreateRoom
//	@param: message-user message
//		    req- request object from user
//  @purpose: handle admin create a room with roomName
string adminCreateRoom(string payload, Request* req);
//@function:  adminStartRoom
//	@param: message-user message
//		    req- request object from user
//  @purpose: handle admin start a game.
string adminStartRoom(string roomName, Request* req);
//@function:  adminEndGame
//	@param: message-user message
//		    req- request object from user
//  @purpose: handle admin end a game.
string adminEndGame(string roomName, Request* req);
//@function:  adminDeleteRoom
//	@param: message-user message
//		    req- request object from user
//  @purpose: handle admin delete an inactive room.
string adminDeleteRoom(string roomName, Request* req);
//phu
//@function:  userGetRooms
//	@param: message-user message
//		    req- request object from user
//  @purpose: handle admin and user get a room list.
string userGetRooms(string payload, Request * req);
//@function:  playerJoinRoom
//	@param: message-user message
//		    req- request object from user
//  @purpose: handle user join an inactive room.
string playerJoinRoom(string payload, Request* req);
//@function:  playerQuitRoom
//	@param: message-user message
//		    req- request object from user
//  @purpose: handle user quit an inactive room
string playerQuitRoom(string payload, Request* req);
//@function:  playerAnswerQuizz
//	@param: message-user message
//		    req- request object from user
//  @purpose: handle player answer a quizz in a round.
string playerAnswerQuizz(string payload, Request* req);

//@function:  processData
//	@param: message-user message
//		    req- request object from user
//  @purpose: process buffer from recv, pass to handleRequest
bool processData(char* buff, char* queue, Request* req);

//@function:  appendQueue
//	@param: message-user message
//		    req- request object from user
//  @purpose: helper to deal with streaming problems, works like a message queue to wait for all payload to be received.
void appendQueue(char* buff, char* queue);