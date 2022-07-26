// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define BUFF_SIZE 2048 
#include <vector>
#define ENDING "\r\n"
#pragma comment(lib, "Ws2_32.lib")
char SERVER_ADDR[INET_ADDRSTRLEN] = "127.0.0.1";
short SERVER_PORT = 5500l;

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // used for goto
COORD CursorPosition; // used for goto
string answer;
string res;

struct User {
	string username;
	int mainPlayer = 1;
	int round = 0;
};

struct Response {
	string user10 = "Login success";
	string user11 = "Duplicate account";
	string user12 = "You are not login";
	string user21 = "you are not user admin";
	string quit95 = "Logout Success";
	string crea20 = "";
	string cquz40 = "";
	string strt50 = "Start room success";
	string strt51 = "Room has been started";
	string endq60 = "End room success";
	string endq61 = "Room not start";
	string loga70 = "Admin logout success";
	string getr80 = "";
	string join90 = "Join room success";
	string answ15 = "Correct answer";
	string answ16 = "Wrong answer";
	string answ17 = "Fastest - You are main player";
	string answ18 = "You are win";
	string exit25 = "Quit game success";
	string geti35 = "";
	string skip45 = "Skip success";
	string skip46 = "You are not main player";
};

User user;
Response response;
/*
* @function handleByte: process information received from the server
* @param {SOCKET} client: information of client that receive
* @return {string} message detail
*/

string handleByte(SOCKET *client) {
	string post;
	int ret;
	char buff[BUFF_SIZE];
	while (1) {
		memset(&buff, 0, sizeof(buff));
		// Recive echo message
		ret = recv(*client, buff, BUFF_SIZE, 0);
		if (ret == SOCKET_ERROR) {
			cout << "Error " << WSAGetLastError() << ": Cannot receive message";
			break;
		}
		else {
			buff[ret] = 0;
			string m = buff;
			post = post + m;
			int k = post.find(ENDING);
			if (k != -1) {
				string data = post.substr(0, k);
				return data;
			}
		}
	}
}


/*
* @function streamProcessing: send byte-stream data
* @param {SOCKET} client: information of client
* @param {string} mess: client's message sent to server
* @return {string}
*/

void streamProcessing(SOCKET *client, string mess) {
	mess = mess + ENDING;
	char buff[BUFF_SIZE];
	int messLen = mess.length();
	int bytes = messLen;
	int k = 0, ret;

	while (bytes != 0) {
		memset(&buff, 0, sizeof(buff));
		// Split post to spread when size of mess is larger than buff size
		if (messLen <= BUFF_SIZE) {
			// if mess.length <= BUFF_SIZE send all
			string m = mess.substr(k, bytes);
			strcpy_s(buff, m.length() + 1, &m[0]);
			bytes = 0;
			ret = send(*client, buff, strlen(buff), 0);
			if (ret == SOCKET_ERROR) {
				cout << "Error " << WSAGetLastError();
			}
		}
		else {
			// if mess.lenth >= BUFF_SIZE send each part
			string m = mess.substr(k, BUFF_SIZE);
			strcpy_s(buff, m.length() + 1, &m[0]);
			ret = send(*client, buff, strlen(buff), 0);
			if (ret == SOCKET_ERROR) {
				cout << "Error " << WSAGetLastError();
			}
			else {
				k = k + ret;
				bytes = bytes - ret;
			}
		}
	}
}


string split(string str, string delimiter) {
	size_t pos = 0;
	string token;
	while ((pos = str.find(delimiter)) != string::npos) {
		token = str.substr(0, pos);
		cout << token << endl;
		str.erase(0, pos + delimiter.length());

	}
	return str;
}

vector<string> split1(string s, string delimiter) {
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


void exitRoom(SOCKET *client) {
	string mess = "25 ";
	streamProcessing(client, mess);
	string data = handleByte(client);
	data = data.substr(0, 2);
	if (data == "25") {
		cout << response.exit25 << endl;
	}
	else if (data == "12") {
		cout << response.user12 << endl;
	}
}


string windowLogin(SOCKET *client, string *username) {
	cout << "Please choose a option" << endl;
	cout << "1. Login" << endl;
	string n;
	string mess = "";
	getline(cin, n);
	if (n == "1") {
		*username = "";
		cout << "Enter your username: ";
		getline(cin, *username);
		mess = "USER " + *username;
		streamProcessing(client, mess);
		string data = handleByte(client);
		return data;
	}
	else {
		return "0   ";
	}
}

void waitQuestion(SOCKET *client, bool *k) {
	string data = handleByte(client);
	cout << data;
	string head = data.substr(0, 2);
	string body = data.substr(3);
	vector<string> question = split1(body, "<br/>");
	if (head == "51") {
		for (int i = 2;i < question.size(); i++) {
			cout << question[i]<<endl;
		}
	}
	else if (head == "12") {
		cout << response.user12;
		*k = false;
	}
}

unsigned __stdcall answerTheQuestion(void* param) {
	SOCKET *client = (SOCKET *)param;
	cout << "1. Answer the question." << endl;
	cout << "2. Exit this game" << endl;
	if (user.mainPlayer == 1)  cout << "3. Skip." << endl;
	cout << "Please choose your option: ";
	int n;
	cin >>n ;
	if (n == 1) {
		cout << "Please choose the answer: " << endl;
		answer = "";
		getline(cin, answer);
	}
	else if (n == 2) {
		exitRoom(client);
		return 0;
	}
	else if (n == 3) {
		cout << "You are skip. Please wait a few second." << endl;
		string answer = "99";
	}

	string mess = "ANSW " + answer;
	streamProcessing(client, mess);
	cout << "Please wait a few second. We are checking your answers" << endl;
	return 0;
}

unsigned __stdcall waitTheAnswer(void* param) {
	SOCKET *client = (SOCKET *)param;
	res = handleByte(client);
	return 0;
}

unsigned __stdcall watch(void* param) {
	if (user.mainPlayer == 1) {
		Sleep(10000);
		cout << "Het thoi gian" << endl;
	}
	else {
		Sleep(8000);
		cout << "Het thoi gian" << endl;
	}
	return 0;
}

void answerQuestion(SOCKET *client) {
	bool k = true;
	while (k) {
		waitQuestion(client, &k);
		if (k == true) {
			HANDLE userAnswer, serverAnswer, time;
			unsigned userAnswerID, serverAnswerID, timeID;
			userAnswer = (HANDLE)_beginthreadex(NULL, 0, &answerTheQuestion, (void*)client, 0, &userAnswerID);
			serverAnswer = (HANDLE)_beginthreadex(NULL, 0, &waitTheAnswer, (void*)client, 0, &serverAnswerID);
			time = (HANDLE)_beginthreadex(NULL, 0, &watch, NULL, 0, &timeID);

			//User tra loi trong thoi gian dem nguoc
			WaitForSingleObject(time, INFINITE);
			CloseHandle(time);
			TerminateThread(userAnswer, 0);
			WaitForSingleObject(serverAnswer, INFINITE);
			CloseHandle(serverAnswer);
			string head = res.substr(0, 1);
			string body = res.substr(3);
			vector<string> dataString = split1(body, "<br/>");
			if (answer == dataString[2]) {
				if (dataString[3] == user.username) {
					user.mainPlayer = 1;
					cout << "Correct answer fastest - You are main player" << endl;
				}
				else {
					cout << "Correct answer" << endl;
				}
			}
			else if (answer == "SKIP") {
				continue;
			}
			else {
				cout << "You are wrong" << endl;
				k = false;
			}

			if (dataString[4] == "1") {
				if (user.username == dataString[3]) {
					cout << "Ban da chien thang" << endl;
					k = false;
				}
				else {
					cout << "Tro choi ket thuc" << endl;
					k = false;
				}
			}
		}
	}
}

void selectRoom(SOCKET *client, string body, bool *k) {
	cout << body <<endl;
	split(body, "<br/>");
	cout << "1. Choose your room." << endl;
	cout << "2. Logout." << endl;
	string n = "";
	string mess = "";
	getline(cin, n);
	if (n == "1") {
		cout << "Please choose your room: ";
		getline(cin, mess);
		mess = "JOIN " + mess;
		streamProcessing(client, mess);
		string data = handleByte(client);
		Sleep(1000);
		cout << "from join " << data << endl;
		data = data.substr(0, 2);
		if (data == "30") {
			cout << response.join90 << endl;
			answerQuestion(client);
		}
		else if (data == "12") {
			cout << response.user12 << endl;
			*k = false;
		}
	}
	else if (n == "2") {
		mess = "95 " + mess;
		streamProcessing(client, mess);
		string data = handleByte(client);
		data = data.substr(0, 2);
		if (data == "95") {
			cout << response.quit95 << endl;
			*k = false;
		}
		else if (data == "12") {
			cout << response.user12 << endl;
			*k = false;
		}
	}
}

void showRoom(SOCKET *client) {
	bool k = true;
	while (k) {
		//đưa ra danh sách phòng rồi chọn phòng
		string mess = "GETQ ";
		streamProcessing(client, mess);
		string data = handleByte(client);
		string head = data.substr(0, 2);
		string body = data.substr(3);
		if (head == "20") {
			selectRoom(client, body, &k);
		}
		else if (head == "12") {
			cout << response.user12 << endl;
			break;
		}
	}
}

void startRoomAdmin(SOCKET *client, string body){
	cout << body<<endl;
	cout << "Please choose room start: ";
	string room;
	getline(cin, room);
	if (room == "nhom-15") {
		string mess = "STRT " + body;
		cout << mess;
		streamProcessing(client, mess);
		string data = handleByte(client);
		string head = data.substr(0, 2);
		if (head == "20") {
			cout << "Start room success";
		}
	}
	else {
		cout << "Room does not exits"<<endl;
	}
}

void startRoom(SOCKET *client) {
	string mess = "GETQ ";
	streamProcessing(client, mess);
	string data = handleByte(client);
	string head = data.substr(0, 2);
	string body = data.substr(3);
	if (head == "20") {
		startRoomAdmin(client, body);
	}
}


//void endRoom(SOCKET *client) {
//	string mess = "60 ";
//	streamProcessing(client, mess);
//	string data = handleByte(client);
//	string head = data.substr(0, 2);
//	if (head == "60") {
//		cout << response.endq60 << endl;
//	}
//	else if (head == "61") {
//		cout << response.endq61 << endl;
//	}
//	else if (head == "21") {
//		cout << response.user21 << endl;
//	}
//}

void adminLogout(SOCKET *client) {
	string mess = "QUIT ";
	streamProcessing(client, mess);
	string data = handleByte(client);
	string head = data.substr(0, 2);
	if (head == "12") {
		cout << response.loga70 << endl;
	}
	else if (head == "13") {
		cout << response.user12 << endl;
	}
}

void windowAdmin(SOCKET *client) {
	while (1) {
		cout << "Page cua admin" << endl;
		cout << "1. Start room" << endl;
		cout << "2. Logout" << endl;
		cout << "Please choose the option: ";
		string n;
		getline(cin, n);
		if (n == "1") {
			startRoom(client);
		}
		else if (n == "2") {
			adminLogout(client);
			break;
		}
		else {
			cout << "That option is not available, please choose again" << endl;
		}
	}
}


int main(int argc, char* argv[]) {

	char port[10];
	if (argc == 3)
	{
		strcpy_s(SERVER_ADDR, argv[1]);
		strcpy_s(port, argv[2]);
		SERVER_PORT = atoi(port);
	}
	else
	{
		cout << "Port change failed! Use default port: 5500\n" << endl;
	}

	//Step 1: Inittiate WinSock 
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData))
	{
		printf("Winsock 2.2 is not supported\n");
		system("pause");
		return 0;
	}

	//Step 2: Construct socket 
	SOCKET client;
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client == INVALID_SOCKET)
	{
		printf("Error %d: Cannot create server socket.", WSAGetLastError());
		system("pause");
		return 0;
	}

	//Step 3: Specify server address 
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_ADDR, &serverAddr.sin_addr);


	//Step 4: Request to connect server 
	if (connect(client, (sockaddr *)&serverAddr, sizeof(serverAddr)))
	{
		printf("Error %d: Cannot connect server.", WSAGetLastError());
		system("pause");
		return 0;
	}
	printf("Connected server!\n");

	//Step 5: Communicate with server 
	char buff[BUFF_SIZE];
	int ret, messageLen;
	int login = 1;
	while (1) {
		string response = windowLogin(&client, &user.username);
		string res = response.substr(0, 2);
		string payload = response.substr(3);
		vector<string> roleAdmin = split1(payload, "<br/>");
		if (res == "10") {
			//neu role tra ve la nguoi choi - selectRoom()
			if (roleAdmin[1] == "0") {
				showRoom(&client);
			}
			//neu role la admin --windowAdmin()
			else if (roleAdmin[1] == "1") {
				windowAdmin(&client);
			}
		}
		else if (response == "11") {
			user.username = "";
			cout << "Duplicate account" << endl;
			continue;
		}
		else {
			cout << "Please choose again" << endl;
			continue;
		}
	}

	//Step 6: Close socket 
	closesocket(client);
	//Step 7: Terminate Winsock 
	WSACleanup();
	system("pause");
	return 0;


}



