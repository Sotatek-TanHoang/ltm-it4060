#include "Methods.h"


string handleRequest(string message, User * user) {
	try
	{
		const string method = message.substr(0, METHOD_LEN);
		// login

		if (method.compare(USER_LOGIN) == 0) {
			return "login ok";
		}
		// logout
		else if (method.compare(USER_LOGOUT) == 0) {
			return "logout ok";
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