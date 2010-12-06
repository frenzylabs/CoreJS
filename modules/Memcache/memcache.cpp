// 
//  memcache.cpp
//  Core
//  
//  Created by Cope, Wesley - Wesley on 2010-12-02.
//  Copyright 2010 FrenzyLabs, llc. All rights reserved.
// 

#include "memcache.h"

using namespace std;
using namespace v8;
using namespace memcache;

namespace Core 
{

   Memcached::Memcached(){}
   Memcached::~Memcached(){}

   JS_METHOD(Memcached::New)
   {
      HandleScope scope;
      client = memcached_create(NULL);
      return scope.Close(args.This());
   }

   JS_METHOD(Memcached::Reset)
   {
      ARG_LENGTH(1);
      HandleScope scope;
   
      memcached_free(client);
   
      return scope.Close(args.This());
   }

   JS_METHOD(Memcached::SetDomainKey)
   {
      ARG_LENGTH(1);
      HandleScope scope;
   
      String::Utf8Value domainKey(args[0]->ToString());
   
      memcached_return rc = memcached_callback_set(client, MEMCACHED_CALLBACK_PREFIX_KEY, *domainKey);

      if(rc != MEMCACHED_SUCCESS)
         return scope.Close(THROW_ERROR(memcached_strerror(client, rc)));
   
      return args.This();
   }

   JS_METHOD(Memcached::GetDomainKey)
   {
      HandleScope scope;
   
      memcached_return rc;
      char *value = (char *)memcached_callback_get(client, MEMCACHED_CALLBACK_PREFIX_KEY, &rc);
   
      if(rc != MEMCACHED_SUCCESS)
         return scope.Close(THROW_ERROR(memcached_strerror(client, rc)));
   
      return scope.Close(String::New(value, strlen(value)));
   }

   JS_METHOD(Memcached::ClearDomainKey)
   {
      HandleScope scope;
      memcached_return rc = memcached_callback_set(client, MEMCACHED_CALLBACK_PREFIX_KEY, NULL);
   
      if(rc != MEMCACHED_SUCCESS)
         return scope.Close(THROW_ERROR(memcached_strerror(client, rc)));
      
      return scope.Close(args.This());
   }

   JS_METHOD(Memcached::AddServer)
   {
      ARG_MIN_LENGTH(3);
   
      HandleScope scope;
      memcached_return rc;
   
      if(args.Length() == 2 && args[0]->IsString() && args[1]->IsInt32())
      {
         String::Utf8Value host(args[0]);
         int port = args[1]->Int32Value();
      
         rc = memcached_server_add(client, *host, port);
      }
      else if(args.Length() == 3 && args[0]->IsString() && args[1]->IsInt32(), args[2]->IsInt32())
      {
         String::Utf8Value host(args[0]);
         int port = args[1]->Int32Value();
         uint32_t weight = args[2]->Uint32Value();
      
         rc = memcached_server_add_with_weight(client, *host, port, weight);
      }
   
      if(rc != MEMCACHED_SUCCESS)
         return scope.Close(THROW_ERROR(memcached_strerror(client, rc)));
      
      return args.This();
   }

   JS_METHOD(Memcached::Flush)
   {
      ARG_LENGTH(1);
      HandleScope scope;
   
      if(!args[0]->IsInt32())
         return scope.Close(THROW_ERROR("Argument requires integer"));
      
      uint32_t expires = args[0]->Uint32Value();
      memcached_return rc = memcached_flush(client, expires);
   
      if(rc != MEMCACHED_SUCCESS)
         return scope.Close(THROW_ERROR(memcached_strerror(client, rc)));
      
      return args.This();
   }

   JS_METHOD(Memcached::Set)
   {
      HandleScope scope;
      ARG_MIN_LENGTH(2);
   
      uint32_t expires = 0;
      uint32_t flags   = 0;

      String::Utf8Value key(args[0]);
      String::Utf8Value val(args[1]);

      memcached_return rc;

      switch(args.Length())
      {
         case 2: 
         {}
         break;

         case 3:
         {
            expires = args[2]->Uint32Value();
         }
         break;

         case 4:
         {
            expires = args[2]->Uint32Value();
            flags   = args[3]->Uint32Value();
         }
         break;

         default:
            return scope.Close(THROW_ERROR("Set requires at least 2 arguments (4 max), key and value."));
      }

      rc = memcached_set(client, *key, key.length(), *val, val.length(), expires, flags);

      if(rc != MEMCACHED_SUCCESS)
         return scope.Close(THROW_ERROR(memcached_strerror(client, rc)));

      return scope.Close(args.This());
   }

   JS_METHOD(Memcached::Replace)
   {
      ARG_MIN_LENGTH(2);
      HandleScope scope;
   
      uint32_t expires = 0;
      uint32_t flags   = 0;

      String::Utf8Value key(args[0]);
      String::Utf8Value val(args[1]);

      memcached_return rc;

      switch(args.Length())
      {
         case 2: {}
         break;

         case 3:
         {
            expires = args[2]->Uint32Value();
         }
         break;

         case 4:
         {
            expires = args[2]->Uint32Value();
            flags   = args[3]->Uint32Value();
         }
         break;

         default:
         return scope.Close(THROW_ERROR("Set requires at least 2 arguments (4 max), key and value."));
      }

      rc = memcached_replace(client, *key, key.length(), *val, val.length(), expires, flags);

      if(rc != MEMCACHED_SUCCESS)
         return scope.Close(THROW_ERROR(memcached_strerror(client, rc)));

      return args.This();
   }

   JS_METHOD(Memcached::Add)
   {
      ARG_MIN_LENGTH(2);
      HandleScope scope;
   
      uint32_t expires = 0;
      uint32_t flags   = 0;

      String::Utf8Value key(args[0]);
      String::Utf8Value val(args[1]);

      memcached_return rc;

      switch(args.Length())
      {
         case 2: {}
         break;

         case 3:
         {
            expires = args[2]->Uint32Value();
         }
         break;

         case 4:
         {
            expires = args[2]->Uint32Value();
            flags   = args[3]->Uint32Value();
         }
         break;

         default:
            return scope.Close(THROW_ERROR("Set requires at least 2 arguments (4 max), key and value."));
      }

      rc = memcached_add(client, *key, key.length(), *val, val.length(), expires, flags);

      if(rc != MEMCACHED_SUCCESS)
         return scope.Close(THROW_ERROR(memcached_strerror(client, rc)));

      return args.This();   
   }

   JS_METHOD(Memcached::Del)
   {
      ARG_MIN_LENGTH(1);

      HandleScope scope;
      memcached_return rc;
      uint32_t expires = 0;

      if(args.Length() == 2)
         expires = args[1]->Uint32Value();

      String::Utf8Value key(args[0]->ToString());

      rc = memcached_delete(client, *key, key.length(), expires);
   
      if(rc != MEMCACHED_SUCCESS)
         return scope.Close(THROW_ERROR(memcached_strerror(client, rc)));

      return args.This();
   }

   JS_METHOD(Memcached::Get)
   {
      ARG_LENGTH(1);

      HandleScope scope;
      String::Utf8Value key(args[0]);
      size_t length;
      uint32_t flags;
      memcached_return rc;
      char *value = memcached_get(client, *key, key.length(), &length, &flags, &rc);

      if(rc == MEMCACHED_NOTFOUND)
         return scope.Close(Undefined());
      else if(rc != MEMCACHED_SUCCESS)
         return scope.Close(THROW_ERROR(memcached_strerror(client, rc)));

      Handle<Object> resObj = Object::New();
      resObj->Set(String::New("value"), String::New(value, length));
      resObj->Set(String::New("flags"), Integer::New(flags));

      free(value);
      return scope.Close(resObj);
   }

   void Memcached::Init(Handle<Object>exports)
   {
      HandleScope scope;

      Handle<FunctionTemplate> tpl = FunctionTemplate::New(New);
      tpl->InstanceTemplate()->SetInternalFieldCount(1);

      Handle<ObjectTemplate> prototype = tpl->PrototypeTemplate();
      prototype->SetInternalFieldCount(1);
   
      handle_template = Persistent<ObjectTemplate>::New(prototype);

      prototype->Set(String::New("reset"),          FunctionTemplate::New(Reset));
      prototype->Set(String::New("setDomainKey"),   FunctionTemplate::New(SetDomainKey));
      prototype->Set(String::New("getDomainkey"),   FunctionTemplate::New(GetDomainKey));
      prototype->Set(String::New("clearDomainKey"), FunctionTemplate::New(ClearDomainKey));
      prototype->Set(String::New("addServer"),      FunctionTemplate::New(AddServer));
      prototype->Set(String::New("flush"),          FunctionTemplate::New(Flush));
      prototype->Set(String::New("set"),            FunctionTemplate::New(Set));
      prototype->Set(String::New("replace"),        FunctionTemplate::New(Replace));
      prototype->Set(String::New("add"),            FunctionTemplate::New(Add));
      prototype->Set(String::New("del"),            FunctionTemplate::New(Del));
      prototype->Set(String::New("get"),            FunctionTemplate::New(Get));

      exports->Set(String::New("Memcache"), tpl->GetFunction());
   }
}
extern "C" void CoreInit(Handle<Object> exports)
{
   Core::Memcached::Init(exports);
}