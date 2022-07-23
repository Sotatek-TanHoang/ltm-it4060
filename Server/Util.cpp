#define _CRT_SECURE_NO_WARNINGS
#include "Util.h"

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
string handleLogin(string username, Request* req) {
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
string handleLogout(Request* req) {

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
	auto it = std::find_if(rooms.begin(), rooms.end(),
		[name](const RoomInfor& a)
		{
			return a.roomName == name;
		});
	if (it == rooms.end())
		return -1; // or throw, depending on requirements
	else
		return it - rooms.begin();
}

int count1 = 0;

void printListQuestion() {
	for (int i = 0;i < count1;i++) {
		cout << listQuestion[i].question << " " << listQuestion[i].answer << " " << listQuestion[i].options << endl;
	}
}

/* The readQuestion function is to read question from file
* @param no param
* @return no return value
*/
void readQuestion() {
	ifstream inputFile("question.txt");
	string line;
	if (inputFile)
	{
		while (getline(inputFile, line))
		{
			QuizzInfor q;
			istringstream ss(line);

			ss >> q.question;

			ss >> q.answer;
			ss >> q.options[0];
			ss >> q.options[1];
			ss >> q.options[2];
			ss >> q.options[3];

			if (ss)
			{
				listQuestion[count1++] = q;
			}
		}
	}
	else
	{
		cout << "File cannot be opened" << std::endl;
	}
}


string adminCreateRoom(string payload, Request* req) {
	if (req->isLoggedIn == false) return "not logged in ";
	//if (req->user.role != USER_ADMIN) return "unauthorization";
	vector<string> v = split(payload, "-");
	if (findIndex(v[0]) >= 0) return "room exists";
	RoomInfor newRoom;
	newRoom.roomName = v[0];
	newRoom.maxRound = stoi(v[1]);
	newRoom.maxPaticipants = stoi(v[2]);
	rooms.push_back(newRoom);
	return "create room ok";
}

string adminStartRoom(string roomName, Request* req) {
	if (req->isLoggedIn == false) return "not logged in ";
	//if (req->user.role != USER_ADMIN) return "unauthorization";
	int index = findIndex(roomName);
	if (index ==-1) return "room does not exist";
	rooms[index].status = ROOM_ACTIVE;
	readQuestion();
	return "start room ok";
}

string adminEndGame(string roomName, Request* req) {
	if (req->isLoggedIn == false) return "not logged in ";
	//if (req->user.role != USER_ADMIN) return "unauthorization";
	int index = findIndex(roomName);
	if (index == -1) return "room does not exist";
	rooms[index].status = ROOM_DEACTIVE;
	rooms[index].currentRoundCount = 0;
	rooms[index].currentPaticipants = 0;
	return "end game ok";
}

string adminDeleteRoom(string roomName, Request* req) {
	if (req->isLoggedIn == false) return "not logged in ";
	//if (req->user.role != USER_ADMIN) return "unauthorization";
	int index = findIndex(roomName);
	if (index == -1) return "room does not exist";
	if (rooms[index].status == ROOM_ACTIVE) return "room is active";
	rooms.erase(rooms.begin() + index);
	return "delete ok";
}

// phu

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