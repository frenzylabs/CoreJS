#include "core.h"
#include "BaseEvent.h"
#include <vector>
#include <algorithm>

using namespace std;
using namespace v8;
using namespace Core;


static int SetNonBlocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1)
    flags = 0;
  return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}


ThreadPool *myThreadPool = ThreadPool::instance();
BaseEvent *myEvents = BaseEvent::instance();


char *MainReadFile(const char *name)
{
    FILE *readFile = fopen(name, "rb");
    if(readFile == NULL)
        return NULL;
        
    fseek(readFile, 0, SEEK_END);
    int size = ftell(readFile);
    rewind(readFile);
    
    char *chars = new char[size + 1];
    chars[size] = '\0';
    
    for(int i = 0; i < size;)
    {
        int read = fread(&chars[i], 1, size - i, readFile);
        i += read;
    }
    
    fclose(readFile);
    
    return chars;
}

void AtExit()
{
   fflush(stdout);
   fflush(stderr);
}


int RunCore (int argc, char const *argv[])
{
	
	string filename;
	filename = (argc > 1)? string(argv[1]) : "main.js";

	char currentPath[FILENAME_MAX];
	getcwd(currentPath, sizeof(currentPath));

	char fullpath[100];

	strcpy(fullpath, currentPath);
	strcat(fullpath, "/");

	const char *Cfilename = filename.c_str();
	strcat(fullpath, Cfilename);

	struct stat fileStat;
	int statRes;

	statRes = stat(fullpath, &fileStat);
	if(statRes == -1)
	{
	    printf("\nCould not find file: %s\n", fullpath);
	    return 0;
	} 

	char pathBuffer[PATH_MAX + 1];

	string mainpath = string(realpath(__FILE__, pathBuffer));
	mainpath = mainpath.substr(0, mainpath.rfind('/'));



	V8::Initialize();
	HandleScope scope;

	Handle<ObjectTemplate> global = ObjectTemplate::New();
	global->Set(String::New("JS_SYS_PATH"), String::New(mainpath.c_str()));

	Core::System::Init(global);
	Core::Env::Init(global);
	Core::Directory::Init(global);
	Core::File::Init(global);
	Core::Module::Init(global);
	Core::Http::Init(global);
	Core::Socket::Init(global);

	Persistent<Context>baseContext_ = Context::New(NULL, global);
	Context::Scope contextScope(baseContext_);
	
	Core::System::ExecuteString (String::New(MainReadFile(fullpath)), String::New(filename.c_str()), true, true);
	
	pthread_mutex_lock(&myThreadPool->threadLock);
	if(myThreadPool->eventCnt>0){
		
		int fds[2];
		if (pipe(fds)) {
			cerr << "pipe() failed, errno: " << errno;
			return false;
		}
	
		if (SetNonBlocking(fds[0])) {
		  cerr << "SetNonBlocking for pipe fd[0] failed, errno: " << errno;
		  return false;
		}

		myThreadPool->wakeup_pipe_out = fds[0];
		myThreadPool->wakeup_pipe_in = fds[1];
		
		myThreadPool->wakeup_event = event_new(myEvents->base, myThreadPool->wakeup_pipe_out, EV_READ|EV_PERSIST, ThreadPool::onNotify, (void *)"");
		event_add(myThreadPool->wakeup_event, NULL);
		
	}
	pthread_mutex_unlock(&myThreadPool->threadLock);
	
	evthread_use_pthreads();

	event_base_dispatch(myEvents->base);

	
	baseContext_.Dispose();
	V8::Dispose();

	close(myThreadPool->wakeup_pipe_in);
	close(myThreadPool->wakeup_pipe_out);
	delete myThreadPool;
	free(myEvents);
	atexit(AtExit);

	return 0;
}