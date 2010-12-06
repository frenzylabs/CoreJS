// 
//  module.h
//  Core
//  
//  Created by Wess Cope on 2010-11-23.
//  Copyright 2010 FrenzyLabs, llc. All rights reserved.
// 

#ifndef __module_h__
#define __module_h__

#include <vector>
#include <dlfcn.h>

#include "common.h"
#include "system.h"
#include "env.h"
#include "path.h"

using namespace std;
using namespace v8;

namespace Core
{
   class Module
   {
   protected:
      
   public:
      Module();
      ~Module();

      static Handle<Value> loadJsModule(string filename);
      static Handle<Value> loadDsoModule(string filename);

      static void getPaths(vector<string> &usePaths);

      static Handle<Value> resolveModule(string name);
      //static void resolveModule(string name);
      static void resolveExtension(string path);

      JS_METHOD_DECLARE(Require);

      static void Init(Handle<ObjectTemplate> &global);
   };
}

#endif