#pragma once
#include "Header.h"
#include "Interface.h"

extern map<string, int> USERS_STATUS;
extern map<string, int> ADMINS_LIST;

extern vector<QuizzInfor> listQuestion;
extern vector<RoomInfor> rooms;
extern CRITICAL_SECTION ROOM_LOCK;
extern CRITICAL_SECTION USER_LOCK;
extern CRITICAL_SECTION ROUND_LOCK;

extern int PORT_COUNTER;