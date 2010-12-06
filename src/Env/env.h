// 
//  env.h
//  Core
//  
//  Created by Cope, Wesley - Wesley on 2010-11-23.
//  Copyright 2010 FrenzyLabs, llc. All rights reserved.
// 

#ifndef __env_h__
#define __env_h__

#include "common.h"
#include "system.h"

using namespace std;
using namespace v8;

namespace Core
{
   /**
    * @class Env
    * Class wrapper for handling accessors (getters/setters) for
    * a JS Object that is connected to system environment variables
    */
    class Env : Core::ObjectWrap
    {       
    public:

        static string Get(string key);
        static bool  Set(string key, string val);

        static Handle<Value> EnvGetter(Local<String> property, const AccessorInfo &info);
        static Handle<Value> EnvSetter(Local<String> property, Local<Value> value, const AccessorInfo &info);

        static void Init(Handle<ObjectTemplate> &global);
    };
    
}

#endif 

