#ifndef __client_socket_h__
#define __client_socket_h__

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <iostream>
#include <fcntl.h>

#include <map>

#include "common.h"
#include "Socket.h"
#include <ObjectWrapT.h>



using namespace v8;
namespace Core
{
	class ClientSocket : public Core::Socket
	{	
	public:	
		ClientSocket();
		ClientSocket(int so);
		~ClientSocket();
		
		// C/C++ Methods
		static void Init   (Handle<ObjectTemplate> &obj);
		static void readStdin(evutil_socket_t fd, short what, void *s);
		static void onRead(evutil_socket_t fd, short what, void * s);
		
		bool connect ( const std::string host, const int port );

		// Javascript Methods

		JS_METHOD_DECLARE(New);
		
		
	};
}


#endif