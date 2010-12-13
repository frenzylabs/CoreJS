#ifndef __client_h__
#define __client_h__

#include "common.h"

#include <errno.h>
#include <signal.h>
#ifndef WIN32
#include <netinet/in.h>
#endif


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <fcntl.h>

#include "threadPool.h"
#include "url.h"
#include <iostream>
#include "session.h"
#include <ObjectWrapT.h>

using namespace v8;

namespace Core
{	
	class Http : Core::ObjectWrapT<Http>
	{
		public:	
			Http(){};
			~Http(){};
		
			v8::Persistent<v8::Function> callback;
			
			static void Init   (Handle<ObjectTemplate> &obj);
			static std::string getPostData(Session *te);
			static std::string getGetData(Session *te);
			
			JS_METHOD_DECLARE(Get);
			JS_METHOD_DECLARE(Post);
	};
}

#endif