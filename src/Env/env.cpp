// 
//  env.cpp
//  Core
//  
//  Created by Cope, Wesley - Wesley on 2010-11-23.
//  Copyright 2010 FrenzyLabs, llc. All rights reserved.
// 


#include "env.h"

using namespace std;
using namespace v8;

namespace Core
{

   /**
    * C++ Method to get environment variables
    *
    * @param {const char *}name - environment variable to reference
    */
   string Env::Get(string key)
   {
      char *res = getenv(key.c_str());
      return (res == NULL)? string() : string(res);
   }
   
   /**
    * C++ Method to set an environment variable
    *
    * @param {const char *}name  - Name of environment variable
    * @param {void *}value       - Value to be stored.
    */
   bool Env::Set(string key, string val)
   {
      string build = key + "=" + val;
      return (putenv((char *)build.c_str()) == -1)? false : true;
   }

   /**
    * Javascript method to retrieve environment variables
    *
    * @param {Local<String>}property   - Variable name to retreive a value for
    * @param {const AccessorInfo}info  - Reference to information about access.
    */
   Handle<Value> Env::EnvGetter(Local<String> property, const AccessorInfo &info)
   {
      HandleScope scope;
      string key(*String::Utf8Value(property));

      return scope.Close(String::New(getenv(key.c_str())));
   }
   
   /**
    * Javascript method to set enviroment variable
    *
    * @param {Local<String>}property - Name of variable to set.
    * @param {Local<Value>}value     - Value to set variable to.
    * @param {const AccessorInfo}info  - Reference to information about access.
    */
   Handle<Value> Env::EnvSetter(Local<String> property, Local<Value> value, const AccessorInfo &info)
   {
       string key(*String::Utf8Value(property) );
       string val(*String::Utf8Value(value->ToString()));
       
       string build = key + "=" + val;
       
       int res = putenv((char *)build.c_str());
       
       if(res == -1)
           return THROW_ERROR("Could not set environment variable");
       
       return Undefined();
   }

   /**
    * Initialize Environment object and expose it to the global
    * object context.
    *
    * @param {Handle<ObjectTemplate}global Reference to global object
    */
   void Env::Init(Handle<ObjectTemplate> &global)
   {
       HandleScope scope;
       
       Handle<ObjectTemplate> envTemplate = ObjectTemplate::New();
       envTemplate->SetNamedPropertyHandler(EnvGetter, EnvSetter);
       
       global->Set(String::New("Env"), envTemplate);
   }
   
}