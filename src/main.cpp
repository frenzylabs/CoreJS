#include "main.h"

using namespace std;
using namespace v8;
using namespace Core;

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

int RunMain (int argc, char const *argv[])
{
   string filename;
   filename = (argc > 1)? string(argv[1]) : "main.js";

   string usingPath;
   
   if((Path::IsAbsolute(string(filename.c_str()))))
   {
      usingPath = filename;
   }
   else 
   {
      char currentPath[FILENAME_MAX];
      getcwd(currentPath, sizeof(currentPath));

      char fullpath[100];

      strcpy(fullpath, currentPath);
      strcat(fullpath, "/");


      const char *Cfilename = filename.c_str();
      strcat(fullpath, Cfilename);
      
      usingPath = string(fullpath);
   }
   


   string runningPath = usingPath;
   runningPath = runningPath.substr(0, runningPath.find_last_of('/'));
   PathHistory::Instance()->addPath(string(runningPath));


   struct stat fileStat;
   int statRes;
   
   statRes = stat(usingPath.c_str(), &fileStat);
   if(statRes == -1)
   {
       printf("\nCould not find file: %s\n", usingPath.c_str());
       return 0;
   } 
   
   
   V8::Initialize();
   HandleScope scope;
   
   Handle<ObjectTemplate> global = ObjectTemplate::New();
   
   Core::System::Init(global);
   Core::Env::Init(global);
   Core::Directory::Init(global);
   Core::File::Init(global);
   Core::Module::Init(global);

   Persistent<Context>baseContext_ = Context::New(NULL, global);
   Context::Scope contextScope(baseContext_);

   string dirname = filename.substr(0, filename.find_last_of("/"));
   string filenameOnly = filename.substr(filename.find_last_of("/") + 1, filename.length());
   
   string source = string(MainReadFile(usingPath.c_str()));
   source = "(function(__FILE__, __DIR__) { " + source + " })";

   Path::Chdir(dirname);
   
   Handle<Script> script = Script::Compile(String::New(source.c_str()));
   Handle<Value> result = script->Run();
   
   Handle<Function> func = Handle<Function>::Cast(result);
   Persistent<Function> pFunc = Persistent<Function>::New(func);
   
   Handle<Value> params[2] = { String::New(filenameOnly.c_str()), String::New(runningPath.c_str()) };
   Handle<Value> funcResult = pFunc->Call(Context::GetCurrent()->Global(), 2, params);


   baseContext_.Dispose();
   V8::Dispose();
   
   atexit(AtExit);
   
   return 0;
}

int main (int argc, char const *argv[])
{
    return RunMain(argc, argv);
}