#include "client.h"
#include <errno.h>
#include <time.h>

/*
static int waitnum = 1;
void wait ( int seconds )
{
//	waitnum++;
  clock_t endwait;
  endwait = clock () + seconds * CLOCKS_PER_SEC ;
  while (clock() < endwait) {}
}
*/

using namespace std;
using namespace v8;
using namespace URLLib;

#define USER_AGENT "TypicalRequest/1"
#define MAX_BUFFER 4096
#define N 1000
#define MEGEXTRA 1000000
 
static const int PORT = 9995;
static const char MESSAGE[] = "Hello, World!\n";


namespace Core
{
	
	ThreadPool *tpool = ThreadPool::instance();
	
	string Http::getPostData(){
		char *host =  const_cast<char *>(this->host.c_str());
		char *path = const_cast<char *>(this->path.c_str());
		int port = this->port;
		std::string params =  this->params;
		
		int sock, bytes;
		struct sockaddr_in sockaddr;
		struct hostent *server;

		char buffer[MAX_BUFFER + 1];
		char header[MAX_BUFFER];
		
		bzero(&sockaddr, sizeof(sockaddr));
		
		if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			return "Unable to create socket";
		
		if((server = gethostbyname(host)) == 0)
			return "Could not find hostname";
		
		sockaddr.sin_addr.s_addr = ((struct in_addr *)(server->h_addr))->s_addr;
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_port = htons(port);

		if(connect(sock, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
			return "Unable to connect to socket";

		sprintf(header, "POST %s HTTP/1.1\015\012Host: %s\015\012User-Agent:TypicalRequest\015\012Content-Type: application/x-www-form-urlencoded\015\012Content-Length: %d\015\012\015\012%s", path, host, (int)(params.length()), params.c_str());

		int headerLength = strlen(header);

		write(sock, header, headerLength);

		int contPos = 0;
		int contentLength = 0;
		int eoh = 1;
		int lastEoh = 0;
		int msg = 0;
		string tada;
		string str;
		
		while(1){

			if(((bytes = recv(sock, buffer, 1, 0))) <= 0 ) { cout << "break here \n"; }
			
			tada.append(buffer, 1);

			if(!contentLength && (signed int)(tada.find("Content-Length")) != -1){
				contPos = tada.find("Content-Length:");
				if((signed int)tada.find("\n", contPos) != -1){

					str = tada.substr((tada.find("Content-Length:")+15));

					size_t startpos = str.find_first_not_of(" \t\f\v\n\r");
					str = str.substr(startpos);

					size_t endpos = str.find_first_of(" \t\f\v\n\r");

					str = str.substr(0, endpos);
					contentLength = atoi(str.c_str());
				}
			}
			if(contentLength>0){
				eoh = 0;
				lastEoh = 1;
				str = tada;

				while((signed int)eoh !=-1 && eoh != lastEoh && !msg){
					lastEoh = eoh;

					eoh = tada.find_first_of("\n", eoh+1);

					size_t charpos = tada.find_first_not_of(" \t\v\n\r", eoh);
					size_t secondPos = tada.find_first_of("\n", eoh+1);

					if((signed int)charpos != -1 && (signed int)secondPos != -1){
						if(secondPos < charpos){
							msg = charpos;
							break;
						}
					}else{
						eoh = -1;
					}
				}

				if(msg){
					if((signed int)tada.substr(msg).length() == (signed int)contentLength){
						break;

					}
				}
			}

			if(buffer[bytes-1] == '\0'){
				break;
			}
		}

		close(sock);
		return tada;
		
	}
	
	string Http::getGetData(){
		
		this->path = this->path+"?"+this->params;
		char *host =  const_cast<char *>(this->host.c_str());
		char *path = const_cast<char *>(this->path.c_str());
		int port = this->port;
		std::string params =  this->params;

		
		int sock, bytes;
		struct sockaddr_in sockaddr;
		struct hostent *server;

		char buffer[MAX_BUFFER + 1];
		char header[MAX_BUFFER];

		if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		    return "Unable to create socket";

		if((server = gethostbyname(host)) == 0)
		    return "Could not find hostname";

		sockaddr.sin_addr.s_addr = ((struct in_addr *)(server->h_addr))->s_addr;
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_port = htons(port);


		if(connect(sock, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
		    return "Unable to connect to socket";
    
		bzero(&sockaddr, sizeof(sockaddr));

		sprintf(header, "GET %s HTTP/1.1\015\012Host: %s\015\012User-Agent:TypicalRequest\015\012\015\012", path, host);

		int headerLength = strlen(header);
		write(sock, header, headerLength);

		int contPos = 0;


		int contentLength = 0;
		int eoh = 1;
		int lastEoh = 0;
		int msg = 0;
		string tada;
		string str;
		
		while(1){

			if(((bytes = recv(sock, buffer, 1, 0))) <= 0 ) { cout << "break here \n"; }
			
			tada.append(buffer, 1);

			if(!contentLength && (signed int)(tada.find("Content-Length")) != -1){
				contPos = tada.find("Content-Length:");
				if((signed int)tada.find("\n", contPos) != -1){

					str = tada.substr((tada.find("Content-Length:")+15));

					size_t startpos = str.find_first_not_of(" \t\f\v\n\r");
					str = str.substr(startpos);

					size_t endpos = str.find_first_of(" \t\f\v\n\r");

					str = str.substr(0, endpos);
					contentLength = atoi(str.c_str());
				}
			}
			if(contentLength>0){
				eoh = 0;
				lastEoh = 1;
				str = tada;

				while((signed int)eoh !=-1 && eoh != lastEoh && !msg){
					lastEoh = eoh;

					eoh = tada.find_first_of("\n", eoh+1);

					size_t charpos = tada.find_first_not_of(" \t\v\n\r", eoh);
					size_t secondPos = tada.find_first_of("\n", eoh+1);

					if((signed int)charpos != -1 && (signed int)secondPos != -1){
						if(secondPos < charpos){
							msg = charpos;
							break;
						}
					}else{
						eoh = -1;
					}
				}

				if(msg){
					if((signed int)tada.substr(msg).length() == (signed int)contentLength){
						//fwrite (buffer , 1 , bytes , toFile );
						break;

					}
				}
			}

			if(buffer[bytes-1] == '\0'){
				//fwrite("\0", 1, 1, toFile);
				break;
			}
			if(bytes>0){
				//fwrite (buffer , 1 , bytes , toFile );
			}
		}
		
		close(sock);
		return tada;
			
		
	}
	
	JS_METHOD(Http::Get)
	{
		HandleScope scope;
		Http *client = new Http();
		
		String::Utf8Value urlArg(args[0]);
      URL url;
      parse_url(*urlArg, &url);
		
		string paramStr = "";
		
		if(args[1]->IsObject()){
			Local<v8::Object> params = args[1]->ToObject();
			Local<v8::Array> props = params->GetPropertyNames();
			int paramLength = static_cast<unsigned int>(props->Length());
			for(int i=0; i< paramLength; i++){
				String::Utf8Value propval(params->Get(props->Get(i)));
				String::Utf8Value propKey(props->Get(i));
				if(i!=0) paramStr.append("&");
				paramStr.append(static_cast<string>(*propKey)+"="+static_cast<string>(*propval));
			}
		}
		
		client->host = url.host;	
		client->path = url.path;
		client->port = url.port;
		client->params = paramStr;
		
		if(args.Length()==3){
			if(args[2]->IsFunction()){
				Http * c = new Http();
				c->host = url.host;	
				c->path = url.path;
				c->port = url.port;
				c->params = paramStr;
				c->callback = Persistent<Function>::New(Local<Function>::Cast(args[2]));
				c->setCallback(c, &Http::getGetData);
				tpool->dispatch(c);
				return scope.Close(Undefined());
			}
		}
		
		string data = client->getGetData();
		return scope.Close(String::New(data.c_str()));
   }
   
   JS_METHOD(Http::Post)
	{
		HandleScope scope;
		
		String::Utf8Value urlArg(args[0]);
      URL url;
      parse_url(*urlArg, &url);

		Http *client = new Http();
		string paramStr = "";
		
		
		if(args[1]->IsObject()){
			Local<v8::Object> params = args[1]->ToObject();
			Local<v8::Array> props = params->GetPropertyNames();
			int paramLength = static_cast<unsigned int>(props->Length());
			for(int i=0; i< paramLength; i++){
				String::Utf8Value propval(params->Get(props->Get(i)));
				String::Utf8Value propKey(props->Get(i));
				if(i!=0) paramStr.append("&");
				paramStr.append(static_cast<string>(*propKey)+"="+static_cast<string>(*propval));
			}
		}
		
		client->host = url.host;	
		client->path = url.path;
		client->port = url.port;
		client->params = paramStr;
		
		string data = "";

		if(args.Length()==3){
			if(args[2]->IsFunction()){
				client->callback = Persistent<Function>::New(Local<Function>::Cast(args[2]));
				client->setCallback(client, &Http::getPostData);
				tpool->dispatch(client);
				return scope.Close(Undefined());
			}
		}else{
		
		 	data = client->getPostData();
			//delete(client);	
		}
		return scope.Close(String::New(data.c_str()));

   }

	JS_METHOD(Http::GetObj)
	{
		HandleScope scope;
		Http *client = UnwrapObject(args.This());
		
		String::Utf8Value urlArg(args[0]);
      URL url;
      parse_url(*urlArg, &url);
		
		string paramStr = "";
		
		if(args[1]->IsObject()){
			Local<v8::Object> params = args[1]->ToObject();
			Local<v8::Array> props = params->GetPropertyNames();
			int paramLength = static_cast<unsigned int>(props->Length());
			for(int i=0; i< paramLength; i++){
				String::Utf8Value propval(params->Get(props->Get(i)));
				String::Utf8Value propKey(props->Get(i));
				if(i!=0) paramStr.append("&");
				paramStr.append(static_cast<string>(*propKey)+"="+static_cast<string>(*propval));
			}
		}
		
		client->host = url.host;	
		client->path = url.path;
		client->port = url.port;
		client->params = paramStr;
		
		if(args.Length()==3){
			if(args[2]->IsFunction()){
				Http * c = new Http();
				c->host = url.host;	
				c->path = url.path;
				c->port = url.port;
				c->params = paramStr;
				c->callback = Persistent<Function>::New(Local<Function>::Cast(args[2]));
				c->setCallback(c, &Http::getGetData);
				tpool->dispatch(c);
				return scope.Close(Undefined());
			}
		}
		
		string data = client->getGetData();
		return scope.Close(String::New(data.c_str()));
	}

	JS_METHOD(Http::PostObj)
	{
		HandleScope scope;
		Http *client = UnwrapObject(args.This());
		
		String::Utf8Value urlArg(args[0]);
      URL url;
      parse_url(*urlArg, &url);
		
		string paramStr = "";
		
		if(args[1]->IsObject()){
			Local<v8::Object> params = args[1]->ToObject();
			Local<v8::Array> props = params->GetPropertyNames();
			int paramLength = static_cast<unsigned int>(props->Length());
			for(int i=0; i< paramLength; i++){
				String::Utf8Value propval(params->Get(props->Get(i)));
				String::Utf8Value propKey(props->Get(i));
				if(i!=0) paramStr.append("&");
				paramStr.append(static_cast<string>(*propKey)+"="+static_cast<string>(*propval));
			}
		}
		
		client->host = url.host;	
		client->path = url.path;
		client->port = url.port;
		client->params = paramStr;
		
		if(args.Length()==3){
			if(args[2]->IsFunction()){
				Http * c = new Http();
				c->host = url.host;	
				c->path = url.path;
				c->port = url.port;
				c->params = paramStr;
				c->callback = Persistent<Function>::New(Local<Function>::Cast(args[2]));
				c->setCallback(c, &Http::getPostData);
				tpool->dispatch(c);
				return scope.Close(Undefined());
			}
		}
		
		string data = client->getPostData();
		return scope.Close(String::New(data.c_str()));
	}

	JS_METHOD(Http::New)
	{
		HandleScope scope;

		if(args.Length()!=2 && args.Length()!=0){
			ThrowException(String::New("Invalid Argument Length"));
		}

		// Create the new instance of the c++ class File
		Http *client = new Http();

	   /**
		* Create the actual template object. 
		* Only need to do this once.  We'll store the object template in a persistent object.
		* This will allow us to create multiple instances of the object	
		* THE "handle_template" var is a Persistent<ObjectTemplate> and is declared in the ObjectWrap template;
		***/	
		if (handle_template.IsEmpty()) {
			v8::Handle<v8::FunctionTemplate> fpt = FunctionTemplate::New(); 
			fpt->SetClassName(String::New("Http"));
			Handle<ObjectTemplate> raw_template = fpt->InstanceTemplate();
			raw_template->SetInternalFieldCount(1);

			handle_template = Persistent<ObjectTemplate>::New(raw_template);

			v8::Handle<v8::ObjectTemplate> protoTpl = fpt->PrototypeTemplate();

			protoTpl->Set(String::New("post"), FunctionTemplate::New(PostObj));
			protoTpl->Set(String::New("get"), FunctionTemplate::New(GetObj));

		}

		return scope.Close(client->WrapObject());
	}

	
	void Http::Init(Handle<ObjectTemplate> &global)
	{
		/**  This will create all the global functions and when a new File is created 
		*  will create the default template for the File class then. 
		**/
	
		v8::Handle<v8::ObjectTemplate> httpTempl = ObjectTemplate::New();

		httpTempl->Set(String::New("post"), FunctionTemplate::New(Post));
		httpTempl->Set(String::New("get"), FunctionTemplate::New(Get));
		
		global->Set(String::New("HttpClient"), FunctionTemplate::New(New));
		global->Set(String::New("Http"), httpTempl);
		
	}
}