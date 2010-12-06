// 
//  memcache.h
//  Core
//  
//  Created by Cope, Wesley - Wesley on 2010-12-02.
//  Copyright 2010 FrenzyLabs, llc. All rights reserved.
// 


#ifndef __memcache_h__
#define __memcache_h__

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <v8.h>

#include <libmemcached/memcached.hpp>
#include "core.h"

using namespace v8;

namespace Core
{
   memcached_st *client;

   class Memcached : Core::ObjectWrapT<Memcached>
   {
   public:
      Memcached();
      ~Memcached();
   
      JS_METHOD_DECLARE(New);
      JS_METHOD_DECLARE(Reset);
      JS_METHOD_DECLARE(SetDomainKey);
      JS_METHOD_DECLARE(GetDomainKey);
      JS_METHOD_DECLARE(ClearDomainKey);
      JS_METHOD_DECLARE(AddServer);
      JS_METHOD_DECLARE(Flush);
      JS_METHOD_DECLARE(Set);
      JS_METHOD_DECLARE(Replace);
      JS_METHOD_DECLARE(Add);
      JS_METHOD_DECLARE(Del);
      JS_METHOD_DECLARE(Get);
   
      static void Init(Handle<Object>exports);
   };
}
#endif