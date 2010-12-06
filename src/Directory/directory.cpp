// 
//  directory.cpp
//  Core
//  
//  Created by Cope, Wesley - Wesley on 2010-11-23.
//  Copyright 2010 FrenzyLabs, llc. All rights reserved.
// 

#include "directory.h"

using namespace std;
using namespace v8;

namespace Core
{
   /**
    * Javascript method to create a directory
    *
    * @param {const Arguments}args - Reference to arguments passed to javascript method.
    */
   JS_METHOD(Directory::Create)
   {
      ARG_LENGTH(1);
      HandleScope scope;
      
      String::Utf8Value dirname(args[0]);
      
      if(mkdir(*dirname, 0777) < 0)
         return scope.Close(THROW_ERROR("Could not create directory"));
         
      return scope.Close(True());
   }

   /**
    * Javascript method to remove a directory
    *
    * @param {const Arguments}args - Reference to arguments passed to javascript method.
    */
   JS_METHOD(Directory::Remove)
   {
      ARG_LENGTH(1);
      HandleScope scope;
      
      String::Utf8Value dirname(args[0]);
      
      if(rmdir(*dirname) < 0)
         return scope.Close(THROW_ERROR("Could not remove directory"));
         
      return scope.Close(Undefined());
   }

   /**
    * Javascript method to list contents of a directory
    *
    * @param {const Arguments}args - Reference to arguments passed to javascript method.
    */
   JS_METHOD(Directory::List)
   {
      ARG_LENGTH(1);
      HandleScope scope;
      
      Local<Array> listings = Array::New();
      String::Utf8Value dirname(args[0]);
      
      DIR *dir;
      
      int cnt = 0;
      struct dirent *drt = NULL;
      
      if(!(dir = opendir(*dirname)))
         return scope.Close(THROW_ERROR("Failed to open directory"));
         
      while((drt = readdir(dir)) != NULL)
      {
         if(strcmp(".", drt->d_name) && strcmp("..", drt->d_name))
            listings->Set(Integer::New(cnt++), String::New(drt->d_name));
      }
      
      closedir(dir);
      
      return scope.Close(listings);
   }

   /**
    * Javascript method to test is passed param is a directory or not
    *
    * @param {const Arguments}args - Reference to arguments passed to javascript method.
    */
   JS_METHOD(Directory::isDirectory)
   {
      ARG_LENGTH(1);
      HandleScope scope;
      
      struct stat sInfo;
      String::Utf8Value dirname(args[0]);
      int res = stat(*dirname, &sInfo);
      
      return scope.Close(Boolean::New(res != -1 && S_ISDIR(sInfo.st_mode)));
   }

   /**
    * Setup Directory methods and Directory javascript object, expose methods
    * to give Javascript access to them
    *
    * @param {Handle<ObjectTemplate>}global - Reference to the javascript global object
    */
   void Directory::Init(Handle<ObjectTemplate> &global)
   {
      HandleScope scope;

      Local<ObjectTemplate> dirObject = ObjectTemplate::New();

      dirObject->Set(String::New("create"),      FunctionTemplate::New(Create));
      dirObject->Set(String::New("remove"),      FunctionTemplate::New(Remove));
      dirObject->Set(String::New("list"),        FunctionTemplate::New(List));
      dirObject->Set(String::New("isDirectory"), FunctionTemplate::New(isDirectory));

      global->Set(String::New("Directory"), dirObject);
   }
}