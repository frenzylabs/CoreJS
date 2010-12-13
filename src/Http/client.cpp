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
	
	string Http::getPostData(Session *te){
		
		char *host =  const_cast<char *>(te->myClient->host.c_str());
		char *path = const_cast<char *>(te->myClient->path.c_str());
		int port = te->myClient->port;
		std::string params =  te->myClient->params;
		
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

		//FILE *toFile;
		//toFile = fopen("testfile.txt", "w");
		
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

	//	fclose (toFile);

		close(sock);
		return tada;
		
	}
	
	string Http::getGetData(Session *te){
		/*

				char *host =  const_cast<char *>(te->myClient->host.c_str());
				char *path = const_cast<char *>(te->myClient->path.c_str());
				int port = te->myClient->port;
				std::string params =  te->myClient->params;
			*/
			
		return "";
		
	}
	
	JS_METHOD(Http::Get)
	{
		HandleScope scope;
		
		String::Utf8Value urlArg(args[0]);
		URL url;
		parse_url(*urlArg, &url);

		char *host;
		char *path;
		int port;

		host = url.host;
		path = url.path;
		port = url.port;

		int sock, bytes;
		struct sockaddr_in sockaddr;
		struct hostent *server;

		char buffer[MAX_BUFFER + 1];
		char header[MAX_BUFFER];

		if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		    return THROW_ERROR("Unable to create socket");

		if((server = gethostbyname(host)) == 0)
		    return THROW_ERROR("Could not find hostname");

		sockaddr.sin_addr.s_addr = ((struct in_addr *)(server->h_addr))->s_addr;
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_port = htons(port);


		if(connect(sock, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
		    return THROW_ERROR("Unable to connect to socket");
    
		bzero(&sockaddr, sizeof(sockaddr));

		sprintf(header, "GET %s HTTP/1.1\015\012Host: %s\015\012User-Agent:TypicalRequest\015\012\015\012", path, host);

		int headerLength = strlen(header);
		write(sock, header, headerLength);

		while((bytes = read(sock, buffer, MAX_BUFFER)) > 0)
		    write(1, buffer, bytes);
    
		close(sock);
		return scope.Close(String::New(buffer, strlen(buffer)));
   }
   
   JS_METHOD(Http::Post)
	{
		HandleScope scope;
		
		String::Utf8Value urlArg(args[0]);
      URL url;
      parse_url(*urlArg, &url);

		Session * nc = new Session();
		nc->myClient = new httpClient();
		
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
		
		nc->myClient->host = url.host;	
		nc->myClient->path = url.path;
		nc->myClient->port = url.port;
		nc->myClient->params = paramStr;
		
		
		if(args.Length()==3){
			if(args[2]->IsFunction()){
				nc->callback = Persistent<Function>::New(Local<Function>::Cast(args[2]));
				nc->funcptr = &getPostData;
				tpool->dispatch(nc);
				return scope.Close(Undefined());
			}
		}
		string data = Http::getPostData(nc);
		delete(nc);
		return scope.Close(String::New(data.c_str()));

   }
/*

	JS_METHOD(Http::New)
	{
		HandleScope scope;

		if(args.Length()!=2 && args.Length()!=0){
			ThrowException(String::New("Invalid Argument Length"));
		}

		// Create the new instance of the c++ class File
		Http *client = new httpClient();

		if(args.Length()==2){
			String::Utf8Value fpath(args[0]);
			String::Utf8Value fmode(args[1]);

			***
			*	Some weird stuff going on here.  Had to make the variables as strings.
			*   If I didnt then when I created new instances in JS code the other instances were getting
			*   overwritten.  
			***
			//fp->filepath = static_cast<string>(*fpath);		
			//fp->filemode = static_cast<string>(*fmode);
		}

	    **
		* Create the actual template object. 
		* Only need to do this once.  We'll store the object template in a persistent object.
		* This will allow us to create multiple instances of the object	
		* THE "handle_template" var is a Persistent<ObjectTemplate> and is declared in the ObjectWrap template;
		***	
		if (handle_template.IsEmpty()) {
			v8::Handle<v8::FunctionTemplate> fpt = FunctionTemplate::New(); 
			fpt->SetClassName(String::New("httpClient"));
			Handle<ObjectTemplate> raw_template = fpt->InstanceTemplate();
			raw_template->SetInternalFieldCount(1);

			handle_template = Persistent<ObjectTemplate>::New(raw_template);

			v8::Handle<v8::ObjectTemplate> protoTpl = fpt->PrototypeTemplate();

			protoTpl->Set(String::New("post"), FunctionTemplate::New(Post));
			protoTpl->Set(String::New("get"), FunctionTemplate::New(Get));

		}

		return scope.Close(client->WrapObject());
	}*/

	
	void Http::Init(Handle<ObjectTemplate> &global)
	{
		/**  This will create all the global functions and when a new File is created 
		*  will create the default template for the File class then. 
		**/
		HandleScope scope;     
		  
		v8::Handle<v8::ObjectTemplate> httpTempl = ObjectTemplate::New();

		httpTempl->Set(String::New("post"), FunctionTemplate::New(Post));
		httpTempl->Set(String::New("get"), FunctionTemplate::New(Get));
		
		global->Set(String::New("Http"), httpTempl);
	}
}