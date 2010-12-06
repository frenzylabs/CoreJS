// 
//  directory.h
//  Core
//  
//  Created by Cope, Wesley - Wesley on 2010-11-23.
//  Copyright 2010 FrenzyLabs, llc. All rights reserved.
// 

#ifndef __directory_h__
#define __directory_h__

#include "common.h"
#include <dirent.h>

using namespace v8;

namespace Core
{
   /**
    * @class Directory
    * Class wrapper for exposing directory functions/methods
    * to javascript land.
    */
    
    class Directory
    {
    public:
       JS_METHOD_DECLARE(Create);
       JS_METHOD_DECLARE(Remove);
       JS_METHOD_DECLARE(List);
       JS_METHOD_DECLARE(isDirectory);

       static void Init(Handle<ObjectTemplate> &global);
    };
}

#endif