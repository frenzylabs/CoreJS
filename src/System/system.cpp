// 
//  system.cpp
//  Core
//  
//  Created by Cope, Wesley - Wesley on 2010-11-23.
//  Copyright 2010 FrenzyLabs, llc. All rights reserved.
// 

#include "system.h"

using namespace std;
using namespace v8;

namespace Core
{
    /**
     * C++ method that takes a Javascript UTF8 string and converts it to a const char*
     *
     * @param {const String::Utf8Value}value reference to the JS UTF8 String.
     */
    const char *System::CString (const String::Utf8Value &value)
    {
        return *value? *value : "Invalid C String conversion";
    }
    
    /**
     * C++ Land method for reading a file
     *
     * @param {const char}name - Path to file to read
     */
    char *System::CReadFile(const char *name)
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
    
    /**
     * C++ Method to execute a javascript string.  If error it's reported with detail otherwise
     * returns true or false if execute successfully.
     *
     * @param {Handle<String>}source - Source string of javascript code to execute.
     * @param {Handle<Value>}name    - Name to reference source string
     * @param {bool}printResult      - Flag for printing the result from javascript execution
     * @param {bool}reportExceptions - Flag for reporting any errors/exceptions from executed javascript
     */
    bool System::ExecuteString (Handle<String> source, Handle<Value> name, bool printResult, bool reportExceptions)
    {
        HandleScope scope;
        TryCatch tryCatch;
        
        Handle<Script> script = Script::Compile(source, name);
        if(script.IsEmpty())
        {
            if(reportExceptions)
                System::ReportException(&tryCatch);
            
            return false;
        }
        else
        {
            Handle<Value> result = script->Run();

            if(result.IsEmpty())
            {
                if(reportExceptions)
                    System::ReportException(&tryCatch);
                
                return false;
            }
            else
            {
                if(printResult && !result->IsUndefined())
                {
                    String::Utf8Value str(result);
                    const char *cstr = CString(str);
                    
                    printf("%s\n", cstr);
                }
                
                return true;
            }
        }
    }
    
    /**
     * C++ Method For executing a js file
     *
     * @param {const char}path - System path to file to execute
     */
     void System::ExecuteFile(const char *path)
     {
        char *fname;
        fname = strrchr(path, '/') + 1;

        char *src = CReadFile(path);

        //bool System::ExecuteString (Handle<String> source, Handle<Value> name, bool printResult, bool reportExceptions)
        ExecuteString(String::New(src), String::New(fname), false, true);
        
        free(src);
     }
    
    /**
     * C++ Method for handling the reporting of exceptions returned from executed
     * javascript.
     *
     * @param {TryCatch}handler - Object containing TryCatch params/details.
     */
      void System::ReportException (TryCatch *tryCatch)
      {
         HandleScope handle_scope;

         String::Utf8Value exception(tryCatch->Exception());
         const char* exceptionString = CString(exception);

         Handle<v8::Message> message = tryCatch->Message();

         if (message.IsEmpty()) 
         {
            printf("%s\n", exceptionString);
         } 
         else 
         {
            String::Utf8Value filename(message->GetScriptResourceName());
            const char* filenameString = CString(filename);

            int linenum = message->GetLineNumber();

            printf("%s:%i: %s\n", filenameString, linenum, exceptionString);

            String::Utf8Value sourceline(message->GetSourceLine());
            const char* sourcelineString = CString(sourceline);

            printf("%s\n", sourcelineString);

            int start = message->GetStartColumn();

            for (int i = 0; i < start; i++)
               printf(" ");

            int end = message->GetEndColumn();

            for (int i = start; i < end; i++)
               printf("^");

            printf("\n");
         }    
      }

    /**
     * Javascript method for printing to the console (or stdout)
     *
     * @param {const Arguments}args - Javascript arguments passed to the method.
     */
    JS_METHOD(System::Print)
    {
        ARG_LENGTH(1);
        HandleScope scope;

        printf("%s", *String::Utf8Value(args[0]));
        
        return scope.Close(Undefined());
    }    

    /**
     * Javascript method for printing (with new line) to the console (or stdout)
     *
     * @param {const Arguments}args - Javascript arguments passed to the method.
     */
    JS_METHOD(System::Println)
    {
       ARG_LENGTH(1);
       HandleScope scope;

       printf("%s\n", *String::Utf8Value(args[0]));
       
       return scope.Close(Undefined());
    }

    /**
     * Javascript method for reading a file and returning the contents as
     * a javascript string.
     *
     * @param {const Arguments}args - Reference to arguments passed to javascript method.
     */
    JS_METHOD(System::ReadFile)
    {
        ARG_LENGTH(1);
        
        HandleScope scope;
        
        String::Utf8Value name(args[0]);
        FILE *file = fopen(*name, "rb");
        
        if(file == NULL)
            return Handle<String>();
            
        fseek(file, 0, SEEK_END);
        int size = ftell(file);
        rewind(file);
        
        char *chars = new char[size + 1];
        chars[size] = '\0';
        
        for(int i = 0; i < size;)
        {
            int read = fread(&chars[i], 1, size - i, file);
            i += read;
        }
        
        fclose(file);
        
        Handle<String> result = String::New(chars, size);
        delete[] chars;
        
        return scope.Close(result);
    }
    
    /**
     * Javascript method to get the current working directory
     *
     * @params n/a
     */
    JS_METHOD(System::Cwd)
    {
        HandleScope scope;
        
        char current[FILENAME_MAX];
        
        if(!getcwd(current, sizeof(current)))
            return THROW_ERROR("Unable to get current working directory");
        
        return scope.Close(String::New(current));        
    }
    
    /**
     * Javascript method to change the permissions of a file or directory
     *
     * @param {const Arguments}args - Reference to arguments passed to javascript method.
     */     
    JS_METHOD(System::Chmod)
    {
        ARG_LENGTH(2);
        
        HandleScope scope;
        
        String::Utf8Value path(args[0]->ToString());
        int mode = (int)(args[1]->Int32Value());
        
        if(chmod(*path, mode) < 0)
            return THROW_ERROR("Unable to change permissions");
            
        return scope.Close(Boolean::New(true));
    }
    
    /**
     * Javascript method to move or rename files (*nix mv style)
     *
     * @param {const Arguments}args - Reference to arguments passed to javascript method.
     */
    JS_METHOD(System::Mv)
    {
        ARG_LENGTH(2);
        
        HandleScope scope;
        
        String::Utf8Value from  (args[0]->ToString());
        String::Utf8Value to    (args[1]->ToString());
        
        if(rename(*from, *to) < 0)
            return scope.Close(THROW_ERROR("System:Mv - Unable to complete request"));
        
        return scope.Close(Boolean::New(true));
    }
    
    /**
     * Javascript method to see if a file or directory exists
     *
     * @param {const Arguments}args - Reference to arguments passed to javascript method.
     */
    JS_METHOD(System::Exists)
    {
        ARG_LENGTH(1);
        
        HandleScope scope;
        
        String::Utf8Value path(args[0]);
        struct stat sInfo;
        int res = stat(*path, &sInfo);
        
        return scope.Close(Boolean::New(res != - 1));
    }
    
    /**
     * Javascript method to see if a file/directory is writable
     *
     * @param {const Arguments}args - Reference to arguments passed to javascript method.
     */
    JS_METHOD(System::isWritable)
    {
        ARG_LENGTH(1);
        
        HandleScope scope;
        
        String::Utf8Value item(args[0]->ToString());
        
        return scope.Close(Boolean::New(access(*item, W_OK) == 0));
    }
    
    /**
     * Javascript method to see if a file/directory is able to be read
     * @param {const Arguments}args - Reference to arguments passed to javascript method.
     */
    JS_METHOD(System::isReadable)
    {
        ARG_LENGTH(1);
        
        HandleScope scope;
        
        String::Utf8Value item(args[0]->ToString());
        
        return scope.Close(Boolean::New(access(*item, R_OK) == 0));        
    }
    
    /**
     * Javascript method to exit the current context/process
     * @param N/A
     */
    JS_METHOD(System::Exit)
    {
        int exitCode = args[0]->Int32Value();
        exit(exitCode);
        
        return Undefined();
    }

    
    /**
     * C/C++ method to create a javascript object with the system methods,
     * that are converted to javascript functions/methods.
     *
     * @param {Handle<ObjectTemplate>}global - Reference to the global javascript object.
     */
    void System::Init(Handle<ObjectTemplate> &global)
    {
        HandleScope scope;

        Local<ObjectTemplate> systemObject = ObjectTemplate::New();
        systemObject->Set(String::New("print"),      FunctionTemplate::New(Print));
        systemObject->Set(String::New("println"),    FunctionTemplate::New(Println));
        systemObject->Set(String::New("readfile"),   FunctionTemplate::New(ReadFile));
        systemObject->Set(String::New("cwd"),        FunctionTemplate::New(Cwd));
        systemObject->Set(String::New("chmod"),      FunctionTemplate::New(Chmod));
        systemObject->Set(String::New("mv"),         FunctionTemplate::New(Mv));
        systemObject->Set(String::New("exists"),     FunctionTemplate::New(Exists));
        systemObject->Set(String::New("isWritable"), FunctionTemplate::New(isWritable));
        systemObject->Set(String::New("isReadable"), FunctionTemplate::New(isReadable));
        systemObject->Set(String::New("Exit"),       FunctionTemplate::New(Exit));

        global->Set(String::New("print"), FunctionTemplate::New(Print));
        global->Set(String::New("System"), systemObject);
    }
}