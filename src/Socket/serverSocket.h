#ifndef __server_socket_h__
#define __server_socket_h__

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
	class ServerSocket : public Core::Socket
	{	
	protected:
			sockaddr_in m_addr;
		
	public:	
		ServerSocket();
		ServerSocket(int so);
		~ServerSocket();

		std::map<int, Socket * > sockMap;
		std::map<int, event * > socketEvents;
		
		evconnlistener *listener;
		
		// C/C++ Methods
		static void Init   (Handle<ObjectTemplate> &obj);
		void setSockID(int soId){m_sock = soId;};
		static void onServerRead(evutil_socket_t fd, short what, void * s);
		static void readStdin(evutil_socket_t fd, short what, void *s);
		static void sighup_function(evutil_socket_t fd, short what, void * s);
		static void accept_conn_cb(struct evconnlistener *lev, evutil_socket_t fd, struct sockaddr *address, int socklen,  void *server);
		v8::Persistent<v8::ObjectTemplate> getObjectTemplate(){return handle_template;}
		// Javascript Methods
		JS_METHOD_DECLARE(New);
	};
}


#endif