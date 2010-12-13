#ifndef __client_socket_h__
#define __client_socket_h__

#include "Socket.h"

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