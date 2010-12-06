// 
//  system.h
//  Core
//  
//  Created by Cope, Wesley - Wesley on 2010-11-23.
//  Copyright 2010 FrenzyLabs, llc. All rights reserved.
// 

#ifndef __system_h__
#define __system_h__

#include "common.h"

using namespace v8;

namespace Core
{
   /**
    * @class System
    * Class wrapper for handling basic system level functions and
    * exposes them to the javascript environment
    */
   
    class System
    {
    public:
        /* Server/Computer level */
        static const char *CString  (const String::Utf8Value &value);
        static char *CReadFile      (const char *name);
        static bool ExecuteString   (Handle<String> source, Handle<Value> name, bool printResult, bool reportExceptions);
        static void ExecuteFile     (const char *path);
        static void ReportException (TryCatch *tryCatch);
        static void Init            (Handle<ObjectTemplate> &global);
        
        
        /* Javascript common */
        JS_METHOD_DECLARE(Print);
        JS_METHOD_DECLARE(Println);
        JS_METHOD_DECLARE(ReadFile);
        JS_METHOD_DECLARE(Cwd);
        JS_METHOD_DECLARE(Chmod);
        JS_METHOD_DECLARE(Mv);
        JS_METHOD_DECLARE(Exists);
        JS_METHOD_DECLARE(isWritable);
        JS_METHOD_DECLARE(isReadable);
        JS_METHOD_DECLARE(Exit);
    
        JS_METHOD_DECLARE(New);
    };
    
}

#endif 

