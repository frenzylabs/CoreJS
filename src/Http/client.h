#ifndef __client_h__
#define __client_h__

#include "common.h"

#include <errno.h>
#include <signal.h>
#ifndef WIN32
#include <netinet/in.h>
#endif

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctype.h>

#include "url.h"
#include <iostream>
#include <ObjectWrapT.h>

using namespace v8;
namespace Core
{
	class Http : Core::ObjectWrapT<Http>
	{
		public:	
			Http(){};
			~Http(){};
		
			static void Init   (Handle<ObjectTemplate> &obj);
		
			JS_METHOD_DECLARE(Get);
			JS_METHOD_DECLARE(Post);
		
			//JS_METHOD_DECLARE(New);
	};
}

#endif