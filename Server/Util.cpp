#include "Util.h"

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
bool processData(char * buff, char * queue) {

	string message = takeMesage(queue);
	if (message.compare("")==0) return false;

	// TODO: handle message here
	strcpy(buff, &message[0]);
	
	
	return true;
}
void appendQueue(char * buff,char * queue) {
	string temp = queue;
	temp += buff;
	strcpy(queue, &temp[0]);
	
}