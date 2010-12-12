#ifndef __socket_h__
#define __socket_h__

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
#include "threadPool.h"
#include <ObjectWrapT.h>



using namespace v8;
namespace Core
{
	class Socket : protected Core::ObjectWrapT<Socket>
	{	
	protected:
			sockaddr_in m_addr;
		
	public:	
		Socket();
		Socket(int so);
		~Socket();

		std::map<std::string, v8::Persistent<v8::Function> > FuncIsSet;
		std::map<int, Socket * > sockMap;
		std::map<std::string, event * > whichEvents;
		std::map<int, event * > socketEvents;
		
		evconnlistener *listener;
		
		// C/C++ Methods
		static void Init   (Handle<ObjectTemplate> &obj);
		bool create();
		bool bind(const int port);
		bool listen() const;
		
		bool connect ( const std::string host, const int port );
		Socket * accept() const;
		bool is_valid() const { return m_sock != -1; }
		bool send ( const std::string ) const;
		int recv ( std::string& ) const;

		// Javascript Methods
		JS_METHOD_DECLARE(create);
		JS_METHOD_DECLARE(bind);
		JS_METHOD_DECLARE(listen);
		JS_METHOD_DECLARE(accept);

		JS_METHOD_DECLARE(connect);

		JS_METHOD_DECLARE(send);
		JS_METHOD_DECLARE(receive);
		
		JS_METHOD_DECLARE(serverSocket);
		
		JS_METHOD_DECLARE(close);
		JS_METHOD_DECLARE(New);
		
		
		JS_METHOD_DECLARE(onConnect);
		JS_METHOD_DECLARE(onDisconnect);
		JS_METHOD_DECLARE(onData);
		
		int m_sock;
	};
}


#endif