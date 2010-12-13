#include "clientSocket.h"
//#include "SocketException.h"

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;



using namespace std;
using namespace v8;
namespace Core
{
//	BaseEvent *myEvents = BaseEvent::instance();
//	ThreadPool *pool = ThreadPool::instance();

	BaseEvent *myCEvents = BaseEvent::instance();

    // C/C++ Methods
	ClientSocket::ClientSocket()
	{
		memset ( &m_addr,	0,	sizeof ( m_addr ) );
	}


	ClientSocket::ClientSocket(int so)
	{
		m_sock = so;
		memset ( &m_addr,	0, sizeof ( m_addr ) );
	}
	
	ClientSocket::~ClientSocket() {}


	void ClientSocket::onRead(evutil_socket_t fd, short what, void * s){
		
		ClientSocket * so = static_cast<ClientSocket *>(s);
		string data = "";
		so->recv(data);
		if(data == "Server has gone offline"){
			event_del(so->whichEvents["stdin"]);
			event_del(so->whichEvents["readEvent"]);
		}
		if(so->FuncIsSet.count("onData")){
			if((so->FuncIsSet["onData"])->IsFunction()){
				Handle<Value> val = {String::New(data.c_str())};
				Handle<Value> res = (so->FuncIsSet["onData"])->Call(v8::Context::GetCurrent()->Global(), 1, &val);
			}
		}
	}

	void ClientSocket::readStdin(evutil_socket_t fd, short what, void *s){
		string input;
		ClientSocket * so = static_cast<ClientSocket *>(s);
		getline(std::cin, input);
		so->send(input);
	}
	
	bool ClientSocket::connect ( const std::string host, const int port )
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
	
	
    // Javascript Methods
	
	JS_METHOD(ClientSocket::New)
	{
		ARG_LENGTH(2);
		HandleScope scope;
		ClientSocket *so = new ClientSocket();
		
		String::Utf8Value temp(args[0]);
		string host = static_cast<string>(*temp);
		int port = args[1]->Int32Value();

		if ( !so->create() )
		{
			return scope.Close(THROW_ERROR("Could not create client socket \n "));
		}
		if ( ! so->connect ( host, port ) )
		{
			return scope.Close(THROW_ERROR("Could not bind to port\n"));
		}

		struct event * stdev = event_new(myCEvents->base, fileno(stdin), EV_READ|EV_PERSIST, ClientSocket::readStdin, (void *)(so));
		event_add(stdev, NULL);
		so->whichEvents["stdin"] = stdev;
		
		struct event * readEvent = event_new(myCEvents->base, so->m_sock, EV_READ|EV_PERSIST, ClientSocket::onRead, (void *)(so));
		event_add(readEvent, NULL);
		so->whichEvents["readEvent"] = readEvent;
		
		/**
		* Create the actual template object. 
		* Only need to do this once.  We'll store the object template in a persistent object.
		* This will allow us to create multiple instances of the object	
		* THE "handle_template" var is a Persistent<ObjectTemplate> and is declared in the ObjectWrap template;
		***/	
		if (handle_template.IsEmpty()) {
			v8::Handle<v8::FunctionTemplate> fpt = FunctionTemplate::New(); 
			fpt->SetClassName(String::New("ClientSocket"));
			Handle<ObjectTemplate> raw_template = fpt->InstanceTemplate();
			raw_template->SetInternalFieldCount(1);
			
			handle_template = Persistent<ObjectTemplate>::New(raw_template);
			
			v8::Handle<v8::ObjectTemplate> protoTpl = fpt->PrototypeTemplate();


			protoTpl->Set(String::New("onData"), FunctionTemplate::New(onData));
			protoTpl->Set(String::New("onConnect"), FunctionTemplate::New(onConnect));
			protoTpl->Set(String::New("close"), FunctionTemplate::New(close));
		}
		
		return scope.Close(so->WrapObject());
	}
	
	
	void ClientSocket::Init(Handle<ObjectTemplate> &global)
	{
	/**  This will create all the global functions and when a new File is created 
	*  will create the default template for the File class then. 
	**/
		HandleScope scope;       
		global->Set(String::New("clientSocket"), FunctionTemplate::New(New));
	}
}
