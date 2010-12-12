#include "Socket.h"
#include "clientSocket.h"
#include "serverSocket.h"
#include "string.h"
#include <string.h>
#include <errno.h>
//#include "SocketException.h"

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;

using namespace std;
using namespace v8;
namespace Core
{
	
//	ThreadPool *pool = ThreadPool::instance();

    // C/C++ Methods
	Socket::Socket() :m_sock (-1)
	{
		memset ( &m_addr,	0,	sizeof ( m_addr ) );
	}


	Socket::Socket(int so) :m_sock ( so )
	{
		memset ( &m_addr,	0, sizeof ( m_addr ) );
	}
	
	Socket::~Socket() {}


	
	bool Socket::create()
	{
		int newsock = socket ( AF_INET, SOCK_STREAM, 0);

		//cout << "socket created and = " << newsock << "\n";
		m_sock = newsock;
		if ( ! is_valid() )
			return false;

		// TIME_WAIT - argh
		int on = 1;
		if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
			return false;

		return true;

	}
	
	
	bool Socket::bind ( const int port )
	{

		if ( ! is_valid() )
		{
			return false;
		}

		m_addr.sin_family = AF_INET;
		m_addr.sin_addr.s_addr = INADDR_ANY;
		m_addr.sin_port = htons ( port );

		int bind_return = ::bind ( m_sock, ( struct sockaddr * ) &m_addr, sizeof ( m_addr ) );


		if ( bind_return == -1 )
	    {
			return false;
	    }

		return true;
	}
	
	bool Socket::listen() const
	{
		if ( ! is_valid() )
	    {
			return false;
	    }
		int maxConnections = 10;
		int listen_return = ::listen ( m_sock, maxConnections );


		if ( listen_return == -1 )
	    {
			return false;
	    }

		return true;
	}
	
	
	bool Socket::connect ( const std::string host, const int port )
	{
		if ( ! is_valid() ) return false;

		m_addr.sin_family = AF_INET;
		m_addr.sin_port = htons ( port );

		int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

		if ( errno == EAFNOSUPPORT ) return false;

		status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

	//	cout << "connected with status of " << status << " and sock of " << m_sock << "\n";

		if ( status == 0 )
			return true;
		else
			return false;
	}
	
	
	bool Socket::send ( const std::string s ) const
	{
		int status = ::send ( m_sock, s.c_str(), s.size(), SO_NOSIGPIPE); // this doesnt exist on mac MSG_NOSIGNAL );

	//	std::cout << "sent data and status = " << status << " and sock= " << m_sock << "\n";
		if ( status == -1 )
	    {

			return false;
	    }
		else
	    {
			return true;
	    }
	}


	int Socket::recv ( std::string& s ) const
	{
		char buf [ MAXRECV + 1 ];

		s = "";

		memset ( buf, 0, MAXRECV + 1 );

		int status = ::recv ( m_sock, buf, MAXRECV, 0 );

		if ( status == -1 )
	    {
			std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
			return 0;
	    }
		else if ( status == 0 )
	    {
			return 0;
	    }
		else
	    {
			s = buf;
			return status;
	    }
	}

	
    // Javascript Methods
	JS_METHOD(Socket::onDisconnect)
	{
		HandleScope scope;
		Socket *so = UnwrapObject(args.This());
		if(args[0]->IsFunction()){
			so->FuncIsSet["onDisconnect"] = Persistent<Function>::New(Handle<Function>::Cast(args[0]));
		}
		return scope.Close(Undefined());
	}
	
	JS_METHOD(Socket::onConnect)
	{
		HandleScope scope;
		Socket *so = UnwrapObject(args.This());
		if(args[0]->IsFunction()){
			so->FuncIsSet["onConnect"] = Persistent<Function>::New(Handle<Function>::Cast(args[0]));
		}
		return scope.Close(Undefined());
	}
	
	JS_METHOD(Socket::onData)
	{
		HandleScope scope;
		Socket *so = UnwrapObject(args.This());
		if(args[0]->IsFunction()){
			so->FuncIsSet["onData"] = Persistent<Function>::New(Handle<Function>::Cast(args[0]));
		}
		return scope.Close(Undefined());
	}
	
	
	JS_METHOD(Socket::close)
	{
		HandleScope scope;
		Socket *so = UnwrapObject(args.This());
		std::map<std::string, event * >::iterator it;
		for ( it=so->whichEvents.begin() ; it != so->whichEvents.end(); it++ )
		{
			event_del(it->second);
			so->whichEvents.erase(it->first);
		}
		::close(so->m_sock);
		free(so);
		return scope.Close(Undefined());
	}
	
	JS_METHOD(Socket::create)
	{
		HandleScope scope;
		Socket *so = UnwrapObject(args.This());
		if(so->create()){
			return scope.Close(Boolean::New(true));
		}else
			return scope.Close(THROW_ERROR("Cannot Create Socket"));
	}
	
	
	void Socket::Init(Handle<ObjectTemplate> &global)
	{
	/**  This will create all the global functions and when a new File is created 
	*  will create the default template for the File class then. 
	**/
		HandleScope scope;     
		Core::ServerSocket::Init(global);
		Core::ClientSocket::Init(global);		
	}
}
