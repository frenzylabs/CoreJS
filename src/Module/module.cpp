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
   /**
    * Class construct method
    */
   Module::Module(){}
   
   /**
    * Class deconstruct method
    */
   Module::~Module(){}

   /**
    * Setup paths, insuring to include the CoreJS system/default path
    *
    * @param {vector<string>} reference to a vector that will contain the various paths
    */
   void Module::getPaths(vector<string> &usePaths)
   {
      
      // If our env var for default path location is not there, add it.
      if((Env::Get(string("FAE_MODULE_PATH"))).empty())
         Env::Set(string("FAE_MODULE_PATH"), string("/Users/weslcope/Desktop/Fae/modules/Core/test"));
        
      // Setup a vector to hold our paths (tmp)
      vector<string> paths;
      // Macro to split up our path env var and place each one in 
      // our tmp 'paths' vector.
      EXPLODE(paths, Env::Get("FAE_MODULE_PATH"), ":");
      
      // Add our paths to the referenced path vector passed to us.
      for(unsigned int i = 0; i < paths.size(); i++)
         usePaths.push_back(paths[i]);
      
      // See if we have a home directory set
      // If so, be sure to add our hidden folder to our paths.
      if((Env::Get(string("HOME"))).empty())
      {
         string userpath = Env::Get(string("HOME"));
         userpath += "/.fae-packages";
         usePaths.push_back(userpath);
      } 
   }
   
   /**
    * Look for a module on our paths and resolve the extension we need to use
    *
    * @param {string} name of the module we are looking for
    */
   Handle<Value> Module::resolveModule(string name)
   {
      HandleScope scope;

      // Setup our current path.
      char currentPath[FILENAME_MAX];
      getcwd(currentPath, sizeof(currentPath));
      
      // Define our containers for our module.
      string resultPath;
      string extension;
      string usingPath;
      string originalPath = string(currentPath);
      
      // Test if the path is absolte, if so use it, else resolve the path name.
      usingPath = (Path::IsAbsolute(string(name.c_str())))? name : Path::Normalize(string(currentPath) + "/" + name);
      // get the location of our last /
      size_t lastSlash = usingPath.find_last_of('/');

      // Hand our strings to the Path method to resolve our module's extension.
      if(!Path::FileExtension(usingPath, resultPath, extension))
      {
         // If we can't find it, throw an error.
         string errStr = "\nUnable to locate: ";
         errStr += name + extension;

         return scope.Close(THROW_ERROR(errStr.c_str()));
      }         

      // Setup path, rebuild it so we can use it.
      usingPath = usingPath.substr(0, lastSlash + 1);

      // Change to the directory that contains our require
      // (hack to simplify relational path requires (recursively)?)
      chdir(usingPath.c_str());

      // Setup our result and test for extensions. 
      // Load our module based on extension type.
      Handle<Value> result;
      if(extension == "js")
         result = Module::loadJsModule(resultPath);
      else if(extension == "fm")
         result = Module::loadDsoModule(resultPath);
      
      // Once module is loaded, change back to our initial directory
      chdir(originalPath.c_str());
       
       // Return with our new found module all loaded up.
      return scope.Close(result);
   }

   // Container for Handling a DSO Modules Init method.
   typedef void (*CoreModule)(Handle<Object> exports);
   
   /**
    * Load a shared library module
    *
    * @param {string} name, with path, to our module.
    */
   Handle<Value> Module::loadDsoModule(string filename)
   {
      HandleScope scope;
      
      // Create our exports object to use with the module.
      Local<Object> exports = Object::New();

      // Open our shared module.
      void *handle = dlopen(filename.c_str(), RTLD_LAZY);
      
      // If we can't open it, throw an error
      if(handle == NULL)
      {
         string loadError = "Failed to load module";
         loadError += ": " + filename;
         
         scope.Close(THROW_ERROR(loadError.c_str()));
      }
         

      // Setup our init method (init method is standard for all modules)
      void *initMethod = dlsym(handle, "CoreInit");
      
      // Fail if we have no init method.
      if(initMethod == NULL)
      {
         dlclose(handle);
         printf("\nError loading module, could not locate CoreInit method.\n");
         exit(0);
      }

      // Grab our Init method, formally and execute it passing it our exports object.
      void (*moduleFunction)(Handle<Object> exports) = (CoreModule)(initMethod);
      moduleFunction(exports);

      // Return our exports object with loaded module methods/classes.
      return scope.Close(exports);
   }
   
   /**
    * Handle loading up a Javascript based module.
    *
    * @param {string} file name (with path) to Javascript module file
    */
   Handle<Value> Module::loadJsModule(string filename)
   {
      HandleScope scope;
      
      // Setup our try catch object
      TryCatch tryCatch;
      
      // Setup our exports object and grab the source of our js file.
      Handle<Object> exports = Object::New();
      char *src = System::CReadFile(filename.c_str());
      
      // Grab the directory of our module and file name.
      string dirname = filename.substr(0, filename.find_last_of("/") + 1);
      string filenameOnly = filename.substr(filename.find_last_of("/") + 1, filename.length());
      
      // Set our source, from the module file, to a string.
      string source = string(src);

      // Wrap our js method to give it some 'const' goodness and to hand it our exports object
      source = "(function(exports, __FILE__, __DIR__) { " + source + "    return exports; })";
      
      // Build our script and run it.
      Handle<Script> script = Script::Compile(String::New(source.c_str()));
      Handle<Value> result = script->Run();
      
      // Cast our source result to a formal JS callable function.
      Handle<Function> func = Handle<Function>::Cast(result);
      
      // Setup our params, to get ready to past to the modules' scope.
      Handle<Value> params[3] = {exports, String::New(filenameOnly.c_str()), String::New(dirname.c_str())};

      // Call the function passing it our params.
      Handle<Value> funcResult = func->Call(exports, 3, params);
      
      // Return the function's executed results.
      return scope.Close(funcResult);
   }
   
   /**
    * Javascript method for requiring/include/using a module.
    *
    * @param {js string} the relative or absolute path to the module.
    */
   JS_METHOD(Module::Require)
   {
      // Test argument length.
      ARG_LENGTH(1);
      HandleScope scope;
      // Grab our path
      String::Utf8Value relativePath(args[0]);
      
      // Pass the path/module to resolve method
      Handle<Value> result = Module::resolveModule(*relativePath);
      
      // return the exports built from resolving.
      return scope.Close(result);
   }

   /**
    * Initialization method to add methods to the global object
    *
    * @param {Handle<ObjectTemplate>} reference to the global object.
    */
   void Module::Init(Handle<ObjectTemplate> &global)
   {
      HandleScope scope;       
      global->Set(String::New("require"), FunctionTemplate::New(Require));

   }
}