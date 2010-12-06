#include <v8.h>

/**
 *
 usage:
 ** Placing class object into JS
 MyClass *myclass = new MyClass();
 Handle<Object> myClassFoJS;
 myClassFoJS = Core::ExposeClass<My
 
 ** Regaining class object from JS
 namespace::classname *obj = Core::UnwrapClass<namespace::classname>(JS Object to Unwrap)
 */

using namespace v8;

namespace Core
{
   template<class T> Handle<Object> WrapClass(T *y)
   {
      HandleScope scope;
      
      Handle<ObjectTemplate> objTpl = ObjectTemplate::New();
      objTpl->SetInternalFieldCount(1);
      
      Persistent<Object> classTemplate;
      classTemplate = Persistent<ObjectTemplate>::New(objTpl);

      Handle<Object> result = classTemplate->NewInstance();
      
      Handle<External> classPointer;
      classPointer = External::New(static_cast<T*>(y));
      
      result->SetInternalField(0, classPointer);
      
      return scope.Close(result);
   }
   
   template<class T> Handle<Object>ExposeClass(Persistent<Context> context, T *y, Handle<Value> exposedName, PropertyAttribute property)
   {
      HandleScope scope;
      Handle<Object> obj = Core::WrapClass<T>(y);
      context->Global()->Set(exposedName, obj, property);
      
      return scope.Close(obj);
   }
   
   template<class T>T* UnwrapClass(Handle<Object> obj)
   {
      Handle<External> field;
      field = Handle<External>::Cast(obj->GetInternalField(0));
      void *ptr = field->Value();
      
      return static_cast<T*>(ptr);
   }
   
   void Expose(Handle<Object> intoObject, Handle<Value> nameV8String, InvocationCallback funcId)
   {
      HandleScope scope;
      Handle<FunctionTemplate> func;
      func = FunctionTemplate::New(funcId);
      intoObject->Set(nameV8String, func->GetFunction());
   }
}