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
#include "ThreadBase.h"
#include "url.h"
#include <iostream>
#include <ObjectWrapT.h>

using namespace v8;

namespace Core
{	
	class Http : Core::ObjectWrapT<Http>, CallbackTemplate<Http>
	{
		public:	
			Http(){};
			~Http(){};
			
			std::string host;
			std::string path;
			int port;
			std::string params;
			
			static void Init   (Handle<ObjectTemplate> &obj);
			std::string getGetData();		 
			std::string getPostData();	

			JS_METHOD_DECLARE(Get);
			JS_METHOD_DECLARE(Post);
			
			JS_METHOD_DECLARE(GetObj);
			JS_METHOD_DECLARE(PostObj);
			
			JS_METHOD_DECLARE(New);
	};
}

#endif