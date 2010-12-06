// 
//  mysql.cpp
//  Core
//  
//  Created by Cope, Wesley - Wesley on 2010-12-01.
//  Copyright 2010 FrenzyLabs, llc. All rights reserved.
// 

#include "mysql.h"

MYSQL_RES *Mysql::getResults()
{
   return result;
}

MYSQL_FIELD *Mysql::getFields()
{
   return fields;
}

Mysql::Mysql(){}
   
Mysql::~Mysql()
{
   this->disconnect();
}

bool Mysql::connect(const char *host, const char *user, const char *password, const char *db, uint32_t port, const char *sock)
{
   this->connection = mysql_init(NULL);
   return mysql_real_connect(this->connection, host, user, password, db, port, NULL, 0);
}
   
void Mysql::disconnect()
{
   if(this->connection)
   {
      mysql_close(this->connection);
      this->connection = NULL;
   }
}

bool Mysql::query(const char *q)
{
   int res = mysql_query(this->connection, q);
   
   if(res != 0)
      return false;
      
   if(!mysql_field_count(this->connection))
      return true;
      
   this->result = mysql_store_result(connection);
   this->fields = mysql_fetch_fields(result);
   
   return result;
}

void Mysql::DeclareRowObject(Handle<Value> &jsField, MYSQL_FIELD field, char *fieldValue)
{
   switch (field.type) 
   {
      case MYSQL_TYPE_NULL:  // NULL-type field
         jsField = Undefined();
      break;
      
      case MYSQL_TYPE_TINY:  // TINYINT field
      case MYSQL_TYPE_BIT:  // BIT field (MySQL 5.0.3 and up)
      case MYSQL_TYPE_SHORT:  // SMALLINT field
      case MYSQL_TYPE_LONG:  // INTEGER field
      case MYSQL_TYPE_INT24:  // MEDIUMINT field
      case MYSQL_TYPE_LONGLONG:  // BIGINT field
      case MYSQL_TYPE_YEAR:  // YEAR field
         jsField = String::New(fieldValue)->ToInteger();
      break;
      
      case MYSQL_TYPE_DECIMAL:  // DECIMAL or NUMERIC field
      case MYSQL_TYPE_NEWDECIMAL:  // Precision math DECIMAL or NUMERIC field
      case MYSQL_TYPE_FLOAT:  // FLOAT field
      case MYSQL_TYPE_DOUBLE:  // DOUBLE or REAL field
         jsField = String::New(fieldValue)->ToNumber();
      break;
      
      case MYSQL_TYPE_TIME:  // TIME field
         jsField = String::New(fieldValue);
      break;
      
      case MYSQL_TYPE_TIMESTAMP:  // TIMESTAMP field
      case MYSQL_TYPE_DATETIME:  // DATETIME field
         jsField = String::New(fieldValue);
      break;
      
      case MYSQL_TYPE_DATE:  // DATE field
      case MYSQL_TYPE_NEWDATE:  // Newer const used > 5.0
         jsField = String::New(fieldValue);
      break;
      
      case MYSQL_TYPE_TINY_BLOB:
      case MYSQL_TYPE_MEDIUM_BLOB:
      case MYSQL_TYPE_LONG_BLOB:
      case MYSQL_TYPE_BLOB:
      case MYSQL_TYPE_VAR_STRING:
      case MYSQL_TYPE_VARCHAR:
      case MYSQL_TYPE_SET:  // SET field
      case MYSQL_TYPE_ENUM:  // ENUM field
      case MYSQL_TYPE_GEOMETRY:  // Spatial fielda
      default:
         if (fieldValue) 
            jsField = String::New(fieldValue);
   }
}

JS_METHOD(Mysql::New)
{
   HandleScope scope;
   Mysql *mysql = new Mysql();

   if(args.Length() > 0)
   {
      ARG_MIN_LENGTH(5);

      String::Utf8Value host     (args[0]->ToString());
      String::Utf8Value user     (args[1]->ToString());
      String::Utf8Value password (args[2]->ToString());
      String::Utf8Value db       (args[3]->ToString());
      uint32_t port = args[4]->Uint32Value();
      String::Utf8Value sock       (args[5]->ToString());
   
      bool connected = mysql->connect(*host, *user, *password, *db, port, *sock);
      
      if(!connected)
      {
         char error[100];
         sprintf(error, "Unable to connected to database %s at %s with username %s", *db, *host, *user);
         return scope.Close(THROW_ERROR(error));
      }
   }

   return scope.Close(mysql->WrapObject());
}

JS_METHOD(Mysql::Connect)
{
   ARG_MIN_LENGTH(5);
   HandleScope scope;
   
   String::Utf8Value host     (args[0]->ToString());
   String::Utf8Value user     (args[1]->ToString());
   String::Utf8Value password (args[2]->ToString());
   String::Utf8Value db       (args[3]->ToString());
   uint32_t port = args[4]->Uint32Value();
   String::Utf8Value sock       (args[5]->ToString());
   
   Mysql *mysql = UnwrapObject(args.This());
   bool connected = mysql->connect(*host, *user, *password, *db, port, *sock);
   
   return scope.Close(connected? True() : False());
}

JS_METHOD(Mysql::Disconnect)
{
   HandleScope scope;
   
   Mysql *mysql = UnwrapObject(args.This());
   mysql->disconnect();
   
   return scope.Close(Undefined());
}

JS_METHOD(Mysql::Query)
{
   ARG_LENGTH(1);
   HandleScope scope;
   
   String::Utf8Value qry(args[0]->ToString());
   Mysql *mysql = UnwrapObject(args.This());
   
   return scope.Close(mysql->query(*qry)? True() : False());
}

JS_METHOD(Mysql::LastInsertId)
{
   HandleScope scope;
   Mysql *mysql = UnwrapObject(args.This());
   MYSQL_RES *result = mysql->getResults();
   my_ulonglong insertId = 0;
   
   if(result == 0 && mysql_field_count(mysql->connection) == 0 && mysql_insert_id(mysql->connection) != 0)
      insertId = mysql_insert_id(mysql->connection);
   
   return scope.Close(Integer::New(insertId));
}

JS_METHOD(Mysql::NumRows)
{
   HandleScope scope;
   
   Mysql *mysql = UnwrapObject(args.This());
   int numFields = mysql_num_rows(mysql->getResults());
   
   return scope.Close(Integer::New(numFields));
}

JS_METHOD(Mysql::FetchArray)
{
   HandleScope scope;

   Mysql *mysql = UnwrapObject(args.This());

   MYSQL_FIELD *fields = mysql->getFields();
   MYSQL_RES *result = mysql->getResults();
   MYSQL_ROW row;

   if(mysql_num_rows(result) == 0)
      return scope.Close(Integer::New(0));

   int numFields = mysql_num_fields(result);

   Local<Array> resultArray = Array::New();
   Local<Array> tmpResAry;
   Local<Value> jsField;

   int cnt = 0;

   while( (row = mysql_fetch_row(result)) )
   {
      tmpResAry = Array::New();
      for(int i=0; i < numFields; i++)
      {
         mysql->DeclareRowObject(jsField, fields[i], row[i]);
         tmpResAry->Set(Integer::New(i), jsField);
      }

      resultArray->Set(Integer::New(cnt++), tmpResAry);
   }

   return scope.Close(resultArray);
}

JS_METHOD(Mysql::FetchObject)
{
   HandleScope scope;

   Mysql *mysql = UnwrapObject(args.This());

   MYSQL_FIELD *fields = mysql->getFields();
   MYSQL_RES *result = mysql->getResults();
   MYSQL_ROW row;

   int numFields = mysql_num_fields(result);

   Local<Array> resultArray = Array::New();
   Local<Object> tmpRowObj;
   Local<Value> jsField;

   int cnt = 0;
   while( (row = mysql_fetch_row(result)) )
   {
      tmpRowObj = Object::New();
      for(int i=0; i < numFields; i++)
      {
         mysql->DeclareRowObject(jsField, fields[i], row[i]);
         tmpRowObj->Set(String::New(fields[i].name), jsField);
      }
      
      resultArray->Set(Integer::New(cnt++), tmpRowObj);
   }

   return scope.Close(resultArray);
}

JS_METHOD(Mysql::Error)
{
   HandleScope scope;
   Mysql *mysql = UnwrapObject(args.This());
   
   const char *error = mysql_error(mysql->connection);
   unsigned int errno = mysql_errno(mysql->connection);
   
   Local<Object> errorObj = Object::New();
   errorObj->Set(String::New("errno"), Integer::New(errno));
   errorObj->Set(String::New("error"), String::New(error));
   
   return scope.Close(errorObj);
}

JS_METHOD(Mysql::EscapeString)
{
   ARG_LENGTH(1);
   
   HandleScope scope;
   Mysql *mysql = UnwrapObject(args.This());
   
   String::Utf8Value str(args[0]);
   int len = args[0]->ToString()->Utf8Length();
   char *res = new char[2*len + 1];
   
   int length = mysql_real_escape_string(mysql->connection, res, *str, len);
   Local<String> escapedStr = String::New(res, length);
   
   delete[] res;
   
   return scope.Close(escapedStr);
}

void Mysql::Init(Handle<Object>exports)
{
   HandleScope scope;

   Handle<FunctionTemplate> tpl = FunctionTemplate::New(New);
   tpl->InstanceTemplate()->SetInternalFieldCount(1);

   Handle<ObjectTemplate> prototype = tpl->PrototypeTemplate();
   prototype->SetInternalFieldCount(1);
   
   handle_template = Persistent<ObjectTemplate>::New(prototype);

   prototype->Set(String::New("connect"), FunctionTemplate::New(Connect));
   prototype->Set(String::New("disconnect"), FunctionTemplate::New(Disconnect));
   prototype->Set(String::New("query"), FunctionTemplate::New(Query));
   prototype->Set(String::New("lastInsertId"), FunctionTemplate::New(LastInsertId));
   prototype->Set(String::New("numRows"), FunctionTemplate::New(NumRows));
   prototype->Set(String::New("fetchArray"), FunctionTemplate::New(FetchArray));
   prototype->Set(String::New("fetchObject"), FunctionTemplate::New(FetchObject));
   prototype->Set(String::New("error"), FunctionTemplate::New(Error));
   prototype->Set(String::New("escapeString"), FunctionTemplate::New(EscapeString));

   exports->Set(String::New("Mysql"), tpl->GetFunction());
}

extern "C" void CoreInit(Handle<Object> exports)
{
   Mysql::Init(exports);
}