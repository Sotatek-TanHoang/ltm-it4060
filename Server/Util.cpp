#include "Util.h"

string handleRequest(string message, Request * req) {
	try
	{
		const string method = message.substr(0, METHOD_LEN);
		string payload;
		try
		{
			payload = message.substr(METHOD_LEN + 1);
		}
		catch (const std::exception&)
		{
			payload = "";
		}
		// login

		if (method.compare(USER_LOGIN) == 0) {
			return handleLogin(payload, req);
		}
		// logout
		else if (method.compare(USER_LOGOUT) == 0) {
			return handleLogout(req);
		}
		// admin create a room
		else if (method.compare(ADMIN_CREATE_ROOM) == 0) {
			return "logout ok";
		}
		// admin start a room
		else if (method.compare(ADMIN_START_ROOM) == 0) {
			return "logout ok";
		}
		// admin end a game
		else if (method.compare(ADMIN_END_GAME) == 0) {
			return "end game ok";
		}
		else if (method.compare(ADMIN_DELETE_ROOM) == 0) {
			return "delete room ok";
		}
		else if (method.compare(PLAYER_GET_ROOMS) == 0) {
			return "get rooms ok";
		}
		else if (method.compare(PLAYER_JOIN_ROOM) == 0) {
			return "join ok";
		}
		else if (method.compare(PLAYER_ANSWER_QUIZZ) == 0) {
			return "anwser ok";
		}
		else if (method.compare(PLAYER_EXIT_ROOM) == 0) {
			return "exit room ok";
		}
		else if (method.compare(PLAYER_UPDATE_ROOM_INFORMATION) == 0) {
			return "update ok";
		}
	}
	catch (const std::exception&)
	{
		return "internal server error";
	}

	return "unknown";

}
// tan
string handleLogin(string username, Request * req) {
	cout << req->user.username << '.' << endl;
	if (req->isLoggedIn == true) return "logged in";
	EnterCriticalSection(&USER_LOCK);
	if (USERS_STATUS[username] == 0) {
		USERS_STATUS[username] = 1;
		req->isLoggedIn = true;
		LeaveCriticalSection(&USER_LOCK);
		req->user = {};
		req->user.username = username;
		req->user.role = USER_GUESS;
		return "logged";
	}
	LeaveCriticalSection(&USER_LOCK);
	return "username duplicated";
}
string handleLogout(Request * req) {

	if (req->isLoggedIn == false) return "not logged in ";
	
	EnterCriticalSection(&USER_LOCK);
	if (USERS_STATUS[req->user.username] == 1) {

		USERS_STATUS[req->user.username] = 0;
		req->isLoggedIn = false;
		LeaveCriticalSection(&USER_LOCK);
		req->user = {};
		
		return "logged out";
	}
	LeaveCriticalSection(&USER_LOCK);

	return "log out failed";
}
// duc


// phu

// stable.
string takeMesage(char * buff) {
	string src(buff);
	int index = src.find(DEMILITER);
	if (index < 0) return "";
	string result = src.substr(0, index);
	// 2 is demiliter lenth.
	string end = src.substr(index + 2);
	strcpy(buff,&end[0]);
	return result;
}
bool processData(char * buff, char * queue,Request * req) {

	string message = takeMesage(queue);
	if (message.compare("")==0) return false;

	// TODO: handle message here

	string response = handleRequest(message, req);

	// end.
	strcpy(buff, &response[0]);
	
	
	return true;
}
void appendQueue(char * buff,char * queue) {
	string temp = queue;
	temp += buff;
	strcpy(queue, &temp[0]);
	
}