#pragma once
#include "Header.h"
#define USER_ADMIN 1
#define USER_GUESS 0
#define MAX_USER_PER_ROOM 100
#define MAX_ROUND_PER_ROOM 30
#define MAX_SKIP 2
#define PARTICIPANT_INIT_POINT 10
struct ThreadInformation {
	SOCKET s;
	char * clientIP="";
	int clientPort=0;
};

struct User
{
	int8_t role=USER_GUESS; // enum USER_XXX;
	string username="";
};

struct Participant{
	string username="";
	float totalPoint=PARTICIPANT_INIT_POINT;
	bool isActive=false;
	bool isMainPlayer=false;
	int8_t skipCount=MAX_SKIP;
};
struct RoomInfor {
	string roomName;
	int currentPaticipants=0;
	int maxPaticipants=MAX_USER_PER_ROOM;
	int maxRound=MAX_ROUND_PER_ROOM;
	int currentRoundCount=0;

};
struct QuizzInfor{
	string question;
	string options[100];
	int answer;
};
struct RoundInfor{
    QuizzInfor quizz;
	int roundNumber;
	
};

