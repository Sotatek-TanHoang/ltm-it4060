#pragma once
#include "Header.h"
#include "Interface.h"
#include "Broadcast.h"
#include "Global.h"

unsigned __stdcall startGame(void* param);
QuizzInfor randomQuizz();
QuizzInfor setupSelectMainPlayer(RoomInfor * room);
QuizzInfor setupNormalRound(RoomInfor * room);

string selectMainPlayer(RoomInfor * room,QuizzInfor quizz);
string calculateNormalRound(RoomInfor * room,QuizzInfor quizz);


void resetPlayerState(RoomInfor * room);
bool checkIsMainPlayerExist(string name);