// 
//  module.cpp
//  Core
//  
//  Created by Wess Cope on 2010-11-23.
//  Copyright 2010 FrenzyLabs, llc. All rights reserved.
// 

#include <sys/stat.h>
#include "module.h"

using namespace std;
using namespace v8;

namespace Core
{
   Module::Module(){}
   
   Module::~Module(){}

   void Module::getPaths(vector<string> &usePaths)
   {
      
      if((Env::Get(string("FAE_MODULE_PATH"))).empty())
         Env::Set(string("FAE_MODULE_PATH"), string("/Users/weslcope/Desktop/Fae/modules/Core/test"));
      //   Env::Set(string("FAE_MODULE_PATH"), string("/usr/local/fae/packages"));
        
      vector<string> paths;
      EXPLODE(paths, Env::Get("FAE_MODULE_PATH"), ":");
      
      for(unsigned int i = 0; i < paths.size(); i++)
         usePaths.push_back(paths[i]);
      
      if((Env::Get(string("HOME"))).empty())
      {
         string userpath = Env::Get(string("HOME"));
         userpath += "/.fae-packages";
         usePaths.push_back(userpath);
      } 
   }
   
   Handle<Value> Module::resolveModule(string name)
   {
      HandleScope scope;

      char currentPath[FILENAME_MAX];
      getcwd(currentPath, sizeof(currentPath));
      
      string resultPath;
      string extension;
      string usingPath;
      string originalPath = string(currentPath);
      
      usingPath = (Path::IsAbsolute(string(name.c_str())))? name : Path::Normalize(string(currentPath) + "/" + name);
      size_t lastSlash = usingPath.find_last_of('/');

      if(!Path::FileExtension(usingPath, resultPath, extension))
      {
         string errStr = "\nUnable to locate: ";
         errStr += name + extension;

         return scope.Close(THROW_ERROR(errStr.c_str()));
      }         

      usingPath = usingPath.substr(0, lastSlash + 1);

      chdir(usingPath.c_str());

      Handle<Value> result;
      if(extension == "js")
         result = Module::loadJsModule(resultPath);
      else if(extension == "fm")
         result = Module::loadDsoModule(resultPath);
      
      chdir(originalPath.c_str());
       
      return scope.Close(result);
   }

   typedef void (*CoreModule)(Handle<Object> exports);
   Handle<Value> Module::loadDsoModule(string filename)
   {
      HandleScope scope;
      Local<Object> exports = Object::New();

      void *handle = dlopen(filename.c_str(), RTLD_LAZY);
      
      if(handle == NULL)
      {
         string loadError = "Failed to load module";
         loadError += ": " + filename;
         
         scope.Close(THROW_ERROR(loadError.c_str()));
      }
         

      //CoreModule moduleMethod = (CoreModule)dlsym(handle, "CoreInit");
      void *initMethod = dlsym(handle, "CoreInit");
      if(initMethod == NULL)
      {
         dlclose(handle);
         printf("\nError loading module\n");
         exit(0);
      }

      void (*moduleFunction)(Handle<Object> exports) = (CoreModule)(initMethod);
      moduleFunction(exports);
      
      
      
      
      return scope.Close(exports);
   }
   
   Handle<Value> Module::loadJsModule(string filename)
   {
      HandleScope scope;
      TryCatch tryCatch;
      
      Handle<Object> exports = Object::New();
      char *src = System::CReadFile(filename.c_str());
      
      string dirname = filename.substr(0, filename.find_last_of("/") + 1);
      string filenameOnly = filename.substr(filename.find_last_of("/") + 1, filename.length());
      
      string source = string(src);      
      source = "(function(exports, __FILE__, __DIR__) { " + source + "    return exports; })";
      
      Handle<Script> script = Script::Compile(String::New(source.c_str()));
      Handle<Value> result = script->Run();
      
      Handle<Function> func = Handle<Function>::Cast(result);
      
      Handle<Value> params[3] = {exports, String::New(filenameOnly.c_str()), String::New(dirname.c_str())};
      Handle<Value> funcResult = func->Call(exports, 3, params);
      
      return scope.Close(funcResult);
   }
   
   
   JS_METHOD(Module::Require)
   {
      ARG_LENGTH(1);
      HandleScope scope;
      String::Utf8Value relativePath(args[0]);
      Handle<Value> result = Module::resolveModule(*relativePath);
      
      return scope.Close(result);
   }

   void Module::Init(Handle<ObjectTemplate> &global)
   {
      HandleScope scope;       
      global->Set(String::New("require"), FunctionTemplate::New(Require));

   }
}