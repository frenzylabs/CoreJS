#include "serverSocket.h"
#include "string.h"
#include <string.h>
#include <errno.h>

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;

using namespace std;
using namespace v8;
namespace Core
{
	
	ThreadPool *pool = ThreadPool::instance();

    // C/C++ Methods
	ServerSocket::ServerSocket()
	{
		memset ( &m_addr,	0,	sizeof ( m_addr ) );
	}


	ServerSocket::ServerSocket(int so) 
	{
		m_sock = so;
		memset ( &m_addr,	0, sizeof ( m_addr ) );
	}
	
	ServerSocket::~ServerSocket() {}


	void ServerSocket::sighup_function(evutil_socket_t fd, short what, void * s){
		Socket * so = static_cast<Socket *>(s);
		cout << "\n Connection was lost \n";

		/** Notifies all connected sockets that the server has gone offline  ***/
		std::map<int, Socket *>::iterator it;
		for ( it=so->sockMap.begin() ; it != so->sockMap.end(); it++ )
		{
			so->sockMap[it->first]->send("Server has gone offline");
		}

		struct timeval delay = { 0, 0 };
		event_base_loopexit(pool->base, &delay);
	}
	
	void ServerSocket::onServerRead(evutil_socket_t fd, short what, void * s){
		ServerSocket * so = static_cast<ServerSocket *>(s);
		string data;
		
		if(!so->sockMap[fd]->recv(data)){
			free(so->sockMap[fd]);
			event_del(so->socketEvents[fd]);
			so->sockMap.erase(fd);
			if(so->FuncIsSet.count("onDisconnect")){
				if((so->FuncIsSet["onDisconnect"])->IsFunction()){
					Handle<Value> res = (so->FuncIsSet["onDisconnect"])->Call(v8::Context::GetCurrent()->Global(), 0, NULL);
				}
			}
		}else{
		
			if(so->FuncIsSet.count("onData")){
				if((so->FuncIsSet["onData"])->IsFunction()){
					Handle<Value> val = {String::New(data.c_str())};
					Handle<Value> res = (so->FuncIsSet["onData"])->Call(v8::Context::GetCurrent()->Global(), 1, &val);
				}
			}
			string reply = "";
			if(data=="close") reply = "close";
			else reply = "Server replied with \""+data+"\"";
		
			if(data.substr(0, 4) == "all:"){
				std::map<int, Socket *>::iterator it;
			  for ( it=so->sockMap.begin() ; it != so->sockMap.end(); it++ )
				{
					if(it->first == fd) so->sockMap[fd]->send("back at ya\n");
					else{
						so->sockMap[it->first]->send(reply);
					}
				}
			}else{
				so->sockMap[fd]->send(reply);
			}
		}
		
	}
	
	void ServerSocket::readStdin(evutil_socket_t fd, short what, void *s){
		string input;
		ServerSocket * so = static_cast<ServerSocket *>(s);
		getline(std::cin, input);
		so->send(input);
	}
	
	void ServerSocket::accept_conn_cb(struct evconnlistener *lev, evutil_socket_t fd, struct sockaddr *address, int socklen,  void *server)
	{
		
		ServerSocket * serve = static_cast<ServerSocket *>(server);
		Socket * so = new Socket(fd);
		serve->sockMap[fd] = so;
		
		if(serve->FuncIsSet.count("onConnect")){
			if((serve->FuncIsSet["onConnect"])->IsFunction()){
				//Handle<Value> val = {""};
				Handle<Value> res = (serve->FuncIsSet["onConnect"])->Call(v8::Context::GetCurrent()->Global(), 0, NULL);
			}
		}

		struct event * ev = event_new(pool->base, so->m_sock, EV_READ|EV_PERSIST, ServerSocket::onServerRead, (void *)(serve));
		event_add(ev, NULL);
		serve->socketEvents[fd] = ev;
	}

	
    // Javascript Methods
	
	JS_METHOD(ServerSocket::New)
	{
		ARG_LENGTH(1);
		HandleScope scope;
		// Create the new instance of the c++ class File
		ServerSocket *so = new ServerSocket();
		int port = args[0]->Int32Value();
		
		if ( !so->create() )
		{
			return scope.Close(THROW_ERROR("Could not create server socket \n "));
		}

		
		/* Clear the sockaddr before using it, in case there are extra
		         * platform-specific fields that can mess us up. */
		memset(&so->m_addr, 0, sizeof(so->m_addr));
		/* This is an INET address */
		so->m_addr.sin_family = AF_INET;
		/* Listen on 0.0.0.0 */
		so->m_addr.sin_addr.s_addr = htonl(0);
		/* Listen on the given port. */
		so->m_addr.sin_port = htons(port);

		
		struct event * hup_event = evsignal_new(pool->base, SIGINT, ServerSocket::sighup_function, (void *)so);
		event_add(hup_event, NULL);
		
		/** This binds the socket to the port and begins listening for connections.  
		  * When a connection has been established will call the callback function
		 **/
		so->listener = evconnlistener_new_bind(pool->base, ServerSocket::accept_conn_cb, (void *)so,
			LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_EXEC, -1, (struct sockaddr*)&so->m_addr, sizeof(so->m_addr));

		if (!so->listener) {
			return scope.Close(THROW_ERROR("Could not create listener \n "));
		}

		
		if (so->getObjectTemplate().IsEmpty()) {
			v8::Handle<v8::FunctionTemplate> fpt = FunctionTemplate::New(); 
			fpt->SetClassName(String::New("Socket"));
			Handle<ObjectTemplate> raw_template = fpt->InstanceTemplate();
			raw_template->SetInternalFieldCount(1);

			handle_template = Persistent<ObjectTemplate>::New(raw_template);

			v8::Handle<v8::ObjectTemplate> protoTpl = fpt->PrototypeTemplate();

			protoTpl->Set(String::New("onData"), FunctionTemplate::New(onData));
			protoTpl->Set(String::New("onConnect"), FunctionTemplate::New(onConnect));
			protoTpl->Set(String::New("onDisconnect"), FunctionTemplate::New(onDisconnect));
			protoTpl->Set(String::New("close"), FunctionTemplate::New(close));

		}

		return scope.Close(so->WrapObject());		

	}
    
	
	void ServerSocket::Init(Handle<ObjectTemplate> &global)
	{
	/**  This will create all the global functions and when a new File is created 
	*  will create the default template for the File class then. 
	**/
		HandleScope scope;       
		global->Set(String::New("serverSocket"), FunctionTemplate::New(New));
	}
}
