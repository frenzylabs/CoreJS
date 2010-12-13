#ifndef event_session_H
#define event_session_H
#include "common.h"
#include "file.h"
#include <map>

using namespace v8;


typedef struct{
	std::string host;
	std::string path;
	int port;
	std::string params;
} httpClient;


class Session{
	public:
		int ready;
		std::string data;
		Persistent<Function> callback;
	
		/** THE ARGUMENT THAT WILL BE USED WHEN THE FUNCTION POINTER (see below) IS CALLED  **/
		union
		{
			httpClient* myClient;
			Core::File* fp;
		};
		
		std::string (*funcptr)(Session *sp);
	
		Session():ready(0){};
		~Session(){};

};
#endif