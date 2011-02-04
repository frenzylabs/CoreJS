#ifndef __common_h__
#define __common_h__

#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <v8.h>

#include "ObjectWrap.h"
#include "pathHistory.h"

// Require Path History container
using namespace std;
using namespace v8;



#define __DIR__(stringName)             char buffer[PATH_MAX + 1]; \
                                        char *dir = realpath(__FILE__, buffer); \
                                        stringName = string(dir); \
                                        stringName.substr(0, stringName.rfind('/'));

#define EXPLODE(vect, str, sep)        int found = str.find_first_of(sep); \
                                       while(found != (int)string::npos) { \
                                          if(found > 0) vect.push_back(str.substr(0, found)); \
                                          str = str.substr(found + 1); \
                                          found = str.find_first_of(sep); } \
                                       if(str.length() > 0) vect.push_back(str);  

// JS (V8) Macros
#define THROW_EXCEPTION(type, reason)   v8::ThrowException(v8::Exception::type(String::New(reason)))
#define THROW_ERROR(reason)             THROW_EXCEPTION(Error, reason)
#define THROW_TYPE(reason)              THROW_EXCEPTION(TypeError, reason)
#define THROW_SYNTAX(reason)            THROW_EXCEPTION(syntaxError, reason)

#define ARG_LENGTH(count)               if(args.Length() != count) \
                                            return ThrowException(String::New("Invalid Argument Length"));

#define ARG_MIN_LENGTH(count)           if(args.Length() > count) \
                                            return ThrowException(String::New("Minimum arguments required is count"));

#define ARG_MAX_LENGTH(count)           if(args.Length() < count) \
                                            return ThrowException(String::New("Maximum arguments required is count"));

#define JS_METHOD_DECLARE(name)         static Handle<Value>name          (const Arguments &args)
#define JS_METHOD(name)                 Handle<Value>name                 (const Arguments &args)

#endif