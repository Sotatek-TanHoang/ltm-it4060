#pragma once
#include "Header.h"
#define USER_ADMIN 1
#define USER_GUESS 0
#define MAX_USER_PER_ROOM 100
#define MAX_ROUND_PER_ROOM 30
#define MAX_SKIP 2
#define PARTICIPANT_INIT_POINT 10
#define ROOM_ACTIVE 1
#define ROOM_DEACTIVE 0

struct ThreadInformation
{
	SOCKET s;
	char *clientIP;
	int clientPort = 0;
};

struct User
{
	int8_t role = USER_GUESS; // enum USER_XXX;
	std::string username = "";
};
struct Request
{
	User user;
	std::string ip;
	int port;
	int threadId;
	bool isLoggedIn = false;
	SOCKET socket;
};

struct Participant
{
	std::string username = "";
	float totalPoint = PARTICIPANT_INIT_POINT;
	bool isActive = false;
	bool isMainPlayer = false;
	int8_t skipCount = MAX_SKIP;
	int currentAnswer = 0;
	int submitTime;
	SOCKET userSocket;
};
struct RoomInfor
{
	std::string roomName;
	int maxPaticipants = MAX_USER_PER_ROOM;
	int maxRound = MAX_ROUND_PER_ROOM;
	string currentMainPlayer = "";
	int currentRoundCount = 0;
	int status = ROOM_DEACTIVE;
	int isEnded = false;
	vector<Participant> participants;
	int broadcastPort;
	long int mainPlayerTimestamp;
	long int subPlayerTimestamp;
};
struct QuizzInfor
{
	std::string question;
	vector<string> options;
	int answer;
	
};
struct RoundInfor
{
	QuizzInfor quizz;
	int roundNumber;
};