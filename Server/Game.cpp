#include "Game.h"
#define SKIP_THIS_ROUND 99
void bulkSend(vector<Participant> * p,string message) {
	

	for (unsigned i = 0; i < p->size(); i++) {
		try
		{
			const char * buff = &message[0];
			send(p->at(i).userSocket, buff, strlen(buff), 0);
		}
		catch (const std::exception&)
		{
			cout << "failed to send to client" << p->at(i).username << endl;
		}
	}
	
}
QuizzInfor randomQuizz() {
	return listQuestion[0];
}
string formatBroadcast(string code, string payload="") {
	return code + " " + payload+DEMILITER;
}
string formatQuizz(QuizzInfor quizz) {
	string result = "";
	result += quizz.question+PAYLOAD_SEPERATER;

	// add options here.
	for (unsigned i = 0; i < quizz.options.size(); ++i) {
		result += quizz.options.at(i);
		if (i < quizz.options.size() - 1) {
			result += PAYLOAD_SEPERATER;
		}
	}
	return result;
}
unsigned __stdcall startGame(void* param) {
	bool isStop = false;
	RoomInfor * infor=(RoomInfor *) param;

	
	
	
	// game data.
	string currentMainPlayer = "";
	int currentRound = 0;
	QuizzInfor currentQuizz;
	string broadcast_message, payload;
	// notify clients to start game.
	bulkSend(&infor->participants,string(GAME_STARTED )+PAYLOAD_SEPERATER+infor->roomName);
	Sleep(1000);
	// start game.

	while (!isStop) {
		currentRound++;
		QuizzInfor thisRoundQuizz;
		bool isNormalRound = checkIsMainPlayerExist(infor->currentMainPlayer);
		cout << "load quizz " << isNormalRound << endl;
		vector<Participant> temp = vector<Participant>(infor->participants);
		if (isNormalRound) {
				// normal round.
			thisRoundQuizz = setupNormalRound(infor);
		}
		else {
			// must select main player;
			thisRoundQuizz = setupSelectMainPlayer(infor);
		}
		// update current round in room infor.
		infor->currentRoundCount = currentRound;
		// send quizz to all players
		payload = infor->roomName+PAYLOAD_SEPERATER+to_string(currentRound)+PAYLOAD_SEPERATER+ formatQuizz(thisRoundQuizz);
		broadcast_message = formatBroadcast(BROADCAST_NEW_ROUND_QUIZZ, payload);
		bulkSend(&temp,broadcast_message);
		
		//wait until time limit.
		//60s
		cout << "wait result " << endl;
		Sleep(60 * 1000);

		// calculate result;
		cout << "calc result " << endl;
		if (isNormalRound) {
			payload=calculateNormalRound(infor,thisRoundQuizz);
		}
		else {
			payload=selectMainPlayer(infor,thisRoundQuizz);
		}

		cout << "send result " << endl;
		// send result to client: roomName, round, anwser, mainplayer,1.
		broadcast_message=formatBroadcast(BROADCAST_GAME_RESULT,payload);
		bulkSend(&infor->participants, broadcast_message);
		cout << "clean up result " << endl;
		//delay for clients to get ready.
		Sleep(5000);
		//cleanup
		
		if (infor->isEnded == true || infor->participants.size()<=1) {
			
			isStop = true;
			
			
		}
		else {
			// if game is not ended, reset players state.
			for (unsigned i = 0; i < infor->participants.size(); ++i) {
				if (infor->participants[i].isActive == true) {
					// only reset active players.
					infor->participants[i].currentAnswer = -1;
					infor->participants[i].submitTime = -1;
					infor->participants[i].isMainPlayer = infor->participants[i].username.compare(currentMainPlayer) == 0;
				}
			}
		}
		
		
		cout << "loop again" << endl;
		// this is end of one round.
		continue;
	}
	// game end.
	// send: winner, points, round.
	Sleep(2000);
	cout << "game end" << endl;
	bulkSend(&infor->participants,string(GAME_ENDED)+PAYLOAD_SEPERATER+infor->roomName);

	// cleanup.
	infor->participants = vector<Participant>();
	infor->status = ROOM_DEACTIVE;

	// game end here.
	return 0;
}

bool checkIsMainPlayerExist(string name) {
	return name.compare("") != 0;
};

QuizzInfor setupSelectMainPlayer(RoomInfor * room) {
	long int endTime;
	time_t seconds;
	// current timestamp in seconds.
	seconds = time(NULL);
	endTime = seconds + 60;
	QuizzInfor someQuizz = randomQuizz();
	room->mainPlayerTimestamp = endTime;
	room->subPlayerTimestamp = endTime;

	return someQuizz;
};
QuizzInfor setupNormalRound(RoomInfor * room) {
	long int mainEndTime,subEndTime;
	
	time_t seconds;
	// current timestamp in seconds.
	seconds = time(NULL);

	mainEndTime = seconds + 60;
	subEndTime = seconds + 30;

	QuizzInfor someQuizz = randomQuizz();
	room->mainPlayerTimestamp = mainEndTime;
	room->subPlayerTimestamp = subEndTime;

	return someQuizz;
};
string selectMainPlayer(RoomInfor * room, QuizzInfor quizz) {
	string fastest="";
	int earliest=INT32_MAX,index=-1;
	for (unsigned i = 0; i < room->participants.size(); ++i) {

		room->participants[i].isMainPlayer = false;
		
		if (earliest > room->participants[i].submitTime) {
			// dont count anwser wrong players.
			if (room->participants[i].currentAnswer != quizz.answer) {
				continue;
			}
			earliest = room->participants[i].submitTime;
			fastest = room->participants[i].username;
			index = i;
		}
	}
	if (index >= 0) {
		room->participants[index].isMainPlayer = true;
		room->currentMainPlayer = fastest;
	}
	string isGameEnd = room->participants.size() <= 1 ? "1" : "0";

	return room->roomName + PAYLOAD_SEPERATER + to_string(room->currentRoundCount) + PAYLOAD_SEPERATER + to_string(quizz.answer) + PAYLOAD_SEPERATER + fastest +PAYLOAD_SEPERATER +isGameEnd;
};
string calculateNormalRound(RoomInfor * room, QuizzInfor quizz) {
	vector<Participant> nextRoundParticipants;
	Participant  thisRoundMainPlayer;
	float totalSharePoint = 0;
	// check result of all sub players.
	for (unsigned i = 0; i < room->participants.size(); ++i) {
		if (room->participants[i].isMainPlayer == false) {
			// if subplayer answer right then add to next round else skip and add theirs points to totalshare.
			if (room->participants[i].currentAnswer == quizz.answer) {
				nextRoundParticipants.push_back(room->participants[i]);
			}
			else {
				totalSharePoint += room->participants[i].totalPoint;
			}
		}
		else {
			// this player is main players. save his data.
			thisRoundMainPlayer = room->participants[i];
		}
	}
	
	// if main player anwser ok.
	if (thisRoundMainPlayer.currentAnswer == quizz.answer) {
		thisRoundMainPlayer.totalPoint += totalSharePoint;
		nextRoundParticipants.push_back(thisRoundMainPlayer);
	}
	else if (thisRoundMainPlayer.currentAnswer == SKIP_THIS_ROUND) {
		float subPoint = (thisRoundMainPlayer.totalPoint + totalSharePoint) / 2;
		thisRoundMainPlayer.totalPoint = subPoint;
		
		for (unsigned i = 0; i < nextRoundParticipants.size(); ++i) {
			nextRoundParticipants[i].totalPoint += (subPoint / nextRoundParticipants.size());
		}
		nextRoundParticipants.push_back(thisRoundMainPlayer);
	}
	else {
		// main player answer wrong.
		string fastest = "";
		int earliest = INT32_MAX, index = -1;
		// if no sub player answser right, main player is win
		if (nextRoundParticipants.size() == 0) {
			nextRoundParticipants.push_back(thisRoundMainPlayer);
		}
		else {
			// else bonus every subplay with main player total points
		
				for (unsigned i = 0; i < nextRoundParticipants.size(); ++i) {
					nextRoundParticipants[i].isMainPlayer = false;

					if (earliest > nextRoundParticipants[i].submitTime) {
						// dont count anwser wrong players.
						if (room->participants[i].currentAnswer != quizz.answer) {
							continue;
						}
						earliest = room->participants[i].submitTime;
						fastest = room->participants[i].username;
						index = i;
					}
				}
		
			nextRoundParticipants[index].isMainPlayer = true;
			room->currentMainPlayer = fastest;
		}
		

	}
	
	room->participants = nextRoundParticipants;

	string isGameEnd = nextRoundParticipants.size() <= 1?"1":"0";

	return room->roomName + PAYLOAD_SEPERATER + to_string(room->currentRoundCount) + PAYLOAD_SEPERATER + to_string(quizz.answer) + PAYLOAD_SEPERATER + room->currentMainPlayer+PAYLOAD_SEPERATER + isGameEnd;
};
void resetPlayerState(RoomInfor * room) {};