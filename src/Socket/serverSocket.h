#ifndef __server_socket_h__
#define __server_socket_h__

#include "Socket.h"


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
		static void onServerRead(evutil_socket_t fd, short what, void * s);
		static void readStdin(evutil_socket_t fd, short what, void *s);
		static void sighup_function(evutil_socket_t fd, short what, void * s);
		static void accept_conn_cb(struct evconnlistener *lev, evutil_socket_t fd, struct sockaddr *address, int socklen,  void *server);
		
		
		// Javascript Methods
		JS_METHOD_DECLARE(New);
	};
}


#endif