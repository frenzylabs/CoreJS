#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "v8.h"

using namespace std;
using namespace v8;

class TestMod
{
public:
   
   static Handle<Value> Test(const Arguments &args)
   {
      HandleScope scope;
      printf("\nHello From TestMod, test method!\n");
      
      return scope.Close(Undefined());
   }
   
   static void Init(Handle<Object> exports)
   {
      printf("\nROLLING\n");
      HandleScope scope;

      printf("\nSUCK IT\n");

      exports->Set(String::New("test"), FunctionTemplate::New(Test)->GetFunction());
      
      printf("\nNO REALLY...\n");
   }
};
extern "C" void CoreInit(Handle<Object> exports)
{
   TestMod::Init(exports);
}