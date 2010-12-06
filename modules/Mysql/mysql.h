// 
//  mysql.h
//  Core
//  
//  Created by Cope, Wesley - Wesley on 2010-12-01.
//  Copyright 2010 FrenzyLabs, llc. All rights reserved.
// 
#ifndef __mysql_h__
#define __mysql_h__

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <v8.h>
#include "core.h"

#include <mysql.h>

using namespace v8;

class Mysql : Core::ObjectWrapT<Mysql>
{
protected:
   MYSQL *connection;
   MYSQL_RES *result;
   MYSQL_FIELD *fields;
   
   int queryState;
   const char *error;
   unsigned int errno;
   
public:
   MYSQL_RES *getResults();
   MYSQL_FIELD *getFields();
   
   Mysql();
   ~Mysql();
   
   bool connect(
      const char *host,
      const char *user,
      const char *password,
      const char *db,
      uint32_t port,
      const char *sock
      );
      
   void disconnect();
   bool query(const char *q);
   void DeclareRowObject(Handle<Value> &jsField, MYSQL_FIELD field, char *fieldValue);
   
   JS_METHOD_DECLARE(New);
   JS_METHOD_DECLARE(Connect);
   JS_METHOD_DECLARE(Disconnect);
   JS_METHOD_DECLARE(Query);
   JS_METHOD_DECLARE(LastInsertId);
   JS_METHOD_DECLARE(NumRows);
   JS_METHOD_DECLARE(FetchArray);
   JS_METHOD_DECLARE(FetchObject);
   JS_METHOD_DECLARE(Error);
   JS_METHOD_DECLARE(EscapeString);
   
   static void Init(Handle<Object>exports);
};

#endif