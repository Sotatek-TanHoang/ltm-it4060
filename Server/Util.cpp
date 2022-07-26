#define _CRT_SECURE_NO_WARNINGS
#include "Util.h"

bool isExist(string username, vector<Participant> * members) {
	for (unsigned i = 0; i < members->size(); i++) {
		if (username.compare(members->at(i).username) == 0) {
			return true;
		}
	}
	return false;
}
string formatResponse(string code,string payload="") {
	return code+" "+ payload  + DEMILITER;
}
string handleRequest(string message, Request* req) {
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
			payload = formatResponse(ACTION_FAILED);
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
		// E.g name-maxRound-maxParticipan
		else if (method.compare(ADMIN_CREATE_ROOM) == 0) {
			return adminCreateRoom(payload, req);
		}
		// admin start a room
		else if (method.compare(ADMIN_START_ROOM) == 0) {
			return adminStartRoom(payload, req);
		}
		// admin end a game
		else if (method.compare(ADMIN_END_GAME) == 0) {
			return adminEndGame(payload, req);
		}
		else if (method.compare(ADMIN_DELETE_ROOM) == 0) {
			return adminDeleteRoom(payload, req);
		}
		else if (method.compare(PLAYER_GET_ROOMS) == 0) {
			return userGetRooms(payload,req);
		}
		else if (method.compare(PLAYER_JOIN_ROOM) == 0) {
			return playerJoinRoom(payload,req);
		}
		else if (method.compare(PLAYER_ANSWER_QUIZZ) == 0) {
			return playerAnswerQuizz(payload,req);
		}
		else if (method.compare(PLAYER_EXIT_ROOM) == 0) {
			return playerQuitRoom(payload,req);
		}
		else if (method.compare(PLAYER_UPDATE_ROOM_INFORMATION) == 0) {
			return "update ok";
		}
	}
	catch (const std::exception&)
	{
		return "internal server error";
	}
	cout << "here" << endl;
	return "unknown";

}
// tan
string handleLogin(string username, Request* req) {
	cout << req->user.username << '.' << endl;
	if (req->isLoggedIn == true) return formatResponse(LOGIN_FAILED);
	EnterCriticalSection(&USER_LOCK);
	if (USERS_STATUS[username] == 0) {
		USERS_STATUS[username] = 1;
		req->isLoggedIn = true;
		LeaveCriticalSection(&USER_LOCK);
		req->user = {};
		req->user.username = username;
		int role = ADMINS_LIST[username];
		req->user.role = role;
		return formatResponse(LOGIN_OK,username+PAYLOAD_SEPERATER+to_string(role));
	}
	LeaveCriticalSection(&USER_LOCK);
	return formatResponse(LOGIN_FAILED);
}
string handleLogout(Request* req) {

	if (req->isLoggedIn == false) return formatResponse(LOGOUT_FAILED);

	EnterCriticalSection(&USER_LOCK);
	if (USERS_STATUS[req->user.username] == 1) {

		USERS_STATUS[req->user.username] = 0;
		req->isLoggedIn = false;
		LeaveCriticalSection(&USER_LOCK);
		req->user = {};

		return formatResponse(LOGOUT_OK);
	}
	LeaveCriticalSection(&USER_LOCK);

	return formatResponse(LOGOUT_FAILED);
}
// duc

vector<string> split(string s, string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	string token;
	vector<string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

int findIndex(string name) {
	int index = -1;
	for (unsigned i = 0; i < rooms.size(); i++) {
		if (rooms[i].roomName.compare(name) == 0) {
			index = i;
			break;
		}
	}
	return index;
}

string adminCreateRoom(string payload, Request* req) {
	return formatResponse(ACTION_FAILED);
	if (req->isLoggedIn == false) return formatResponse(NOT_LOGGED_IN);
	if (req->user.role != USER_ADMIN) return formatResponse(UNAUTHORIZED);
	vector<string> v = split(payload, PAYLOAD_SEPERATER);
	if (findIndex(v[0]) >= 0) return formatResponse(ADMIN_EXISTED_ROOM);
	RoomInfor newRoom;
	newRoom.roomName = v[0];
	newRoom.maxRound = stoi(v[1]);
	newRoom.maxPaticipants = stoi(v[2]);
	rooms.push_back(newRoom);
	return formatResponse(ADMIN_CREATE_ROOM_OK,newRoom.roomName);
}

string adminStartRoom(string roomName, Request* req) {
	if (req->isLoggedIn == false) formatResponse(NOT_LOGGED_IN);
	if (req->user.role != USER_ADMIN) return formatResponse(UNAUTHORIZED);
	int index = findIndex(roomName);
	if (index ==-1) return formatResponse(ADMIN_NON_EXISTED_ROOM);
	RoomInfor * params = &rooms[index];
	params->status = ROOM_ACTIVE;
	params->broadcastPort = ++PORT_COUNTER;
	_beginthreadex(0, 0, startGame, (void*)params, 0, 0); //start game in a new thread
	return formatResponse(GAME_STARTED);
}

string adminEndGame(string roomName, Request* req) {
	return formatResponse(ACTION_FAILED);
	/*if (req->isLoggedIn == false) return "not logged in ";
	if (req->user.role != USER_ADMIN) return "unauthorization";
	int index = findIndex(roomName);
	if (index == -1) return "room does not exist";
	rooms[index].isEnded = true;
	rooms[index].currentRoundCount = 0;

	return "end game ok";*/
}

string adminDeleteRoom(string roomName, Request* req) {
	return formatResponse(ACTION_FAILED);
	/*if (req->isLoggedIn == false) return "not logged in ";
	if (req->user.role != USER_ADMIN) return "unauthorization";
	int index = findIndex(roomName);
	if (index == -1) return "room does not exist";
	if (rooms[index].status == ROOM_ACTIVE) return "room is active";
	rooms.erase(rooms.begin() + index);
	return "delete ok";*/
}

// phu
string userGetRooms(string payload, Request * req) {
	if (req->isLoggedIn == false) return formatResponse(NOT_LOGGED_IN);
	string roomList = "";
	for (unsigned i = 0; i < rooms.size(); ++i) {
		if (rooms[i].status == ROOM_DEACTIVE) {
			roomList += rooms[i].roomName;
			if (i < rooms.size() - 1) {
				roomList += PAYLOAD_SEPERATER;
			}
		}
	}
	return formatResponse(GET_ROOM_OK, roomList);
};
string playerJoinRoom(string payload, Request* req) {
	if (req->isLoggedIn == false) return formatResponse(NOT_LOGGED_IN);
	if (req->user.role != USER_GUESS) return formatResponse(UNAUTHORIZED);
	const string roomName = payload;
	RoomInfor * targetRoom=NULL;
	EnterCriticalSection(&ROOM_LOCK);
	// find room
	for (unsigned i = 0; i < rooms.size(); ++i) {
		if (rooms[i].roomName.compare(roomName) == 0 && rooms[i].status==ROOM_DEACTIVE) {
			targetRoom = &rooms[i];
			break;
		}
	}
	if (targetRoom == NULL) {
		LeaveCriticalSection(&ROOM_LOCK);
		return formatResponse(ROOM_NOT_EXIST);
	}

	Participant newParticipant = {};
	newParticipant.username = req->user.username;
	newParticipant.isMainPlayer = false;
	newParticipant.isActive = true;
	newParticipant.totalPoint = 10;
	newParticipant.skipCount = 0;
	newParticipant.currentAnswer = -1;
	newParticipant.userSocket = req->socket;
	if (!isExist(req->user.username, &targetRoom->participants)) {
		cout << "user " << req->user.username << "join room " << targetRoom->roomName << endl;
		targetRoom->participants.push_back(newParticipant);
	}

	LeaveCriticalSection(&ROOM_LOCK);
	return formatResponse(JOIN_ROOM_OK, roomName+PAYLOAD_SEPERATER+to_string(targetRoom->broadcastPort));

};
string playerQuitRoom(string payload, Request* req) {
	if (req->isLoggedIn == false) return formatResponse(NOT_LOGGED_IN);
	if (req->user.role != USER_GUESS) return formatResponse(UNAUTHORIZED);
	const string roomName = payload;
	RoomInfor * targetRoom = NULL;
	EnterCriticalSection(&ROOM_LOCK);
	for (unsigned i = 0; i < rooms.size(); ++i) {
		if (rooms[i].roomName.compare(roomName) == 0) {
			targetRoom = &rooms[i];
			break;
		}
	}
	if (targetRoom == NULL) {
		LeaveCriticalSection(&ROOM_LOCK);
		return formatResponse(ROOM_NOT_EXIST);
	}
	
	for (unsigned i = 0; i < targetRoom->participants.size(); ++i) {
		if (targetRoom->participants[i].username.compare(req->user.username)==0) {
			// remove from vector.
			targetRoom->participants.erase(targetRoom->participants.begin()+i);
			break;
		}
	}
	LeaveCriticalSection(&ROOM_LOCK);
	
	return formatResponse(ACTION_SUCCESS);
}
string playerAnswerQuizz(string payload, Request* req) {
	
	

	if (req->isLoggedIn == false) return formatResponse(NOT_LOGGED_IN);
	if (req->user.role != USER_GUESS) return formatResponse(UNAUTHORIZED);


	vector<string> data = split(payload, PAYLOAD_SEPERATER);
	const string roomName = data[0];
	const string quizzID = data[1];
	const string anwser = data[2];

	RoomInfor * targetRoom = NULL;
	EnterCriticalSection(&ROOM_LOCK);
	for (unsigned i = 0; i < rooms.size(); ++i) {
		if (rooms[i].roomName.compare(roomName) == 0) {
			targetRoom = &rooms[i];
			break;
		}
	}
	if (targetRoom == NULL) {
		LeaveCriticalSection(&ROOM_LOCK);
		return formatResponse(ROOM_NOT_EXIST);
	}
	bool accepted = false;
	time_t now;
	// current timestamp in seconds.
	now = time(NULL);

	for (unsigned i = 0; i < targetRoom->participants.size(); ++i) {
		if (targetRoom->participants[i].username.compare(req->user.username) == 0) {
			// remove from vector.
			if (targetRoom->currentRoundCount > atoi(&quizzID[0])) {
				accepted = false;
				break;
			}
			if (req->user.username.compare(targetRoom->currentMainPlayer) == 0) {
				if (now > targetRoom->mainPlayerTimestamp) {
					accepted = false;
					break;
				}
			}
			else {
				if (now > targetRoom->subPlayerTimestamp) {
					accepted = false;
					break;
				}
			}
			targetRoom->participants[i].currentAnswer = atoi(&anwser[0]);
			targetRoom->participants[i].submitTime = now;
			accepted = true;
			break;
		}
	}
	LeaveCriticalSection(&ROOM_LOCK);
	if (!accepted) {
		return formatResponse(ACTION_FAILED);
	}
	return formatResponse(ACTION_SUCCESS);
}
// stable.
string takeMesage(char* buff) {
	string src(buff);
	int index = src.find(DEMILITER);
	if (index < 0) return "";
	string result = src.substr(0, index);
	// 2 is demiliter lenth.
	string end = src.substr(index + 2);
	strcpy(buff, &end[0]);
	return result;
}
bool processData(char* buff, char* queue, Request* req) {

	string message = takeMesage(queue);
	if (message.compare("") == 0) return false;

	// TODO: handle message here

	string response = handleRequest(message, req);

	// end.
	strcpy(buff, &response[0]);


	return true;
}
void appendQueue(char* buff, char* queue) {
	string temp = queue;
	temp += buff;
	strcpy(queue, &temp[0]);

}
