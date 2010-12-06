// 
//  ObjectWrapT.h
//  Core
//  
//  Borrowed From Node.js (www.nodejs.org)
// 

#ifndef object_wrapT_h
#define object_wrapT_h

#include <v8.h>
#include <assert.h>

using namespace v8;

namespace Core
{
	template <typename T>
    class ObjectWrapT {
     public:
      ObjectWrapT ( ) {
        refs_ = 0;
      }


      virtual ~ObjectWrapT ( ) {
        if (!handle_.IsEmpty()) {
          assert(handle_.IsNearDeath());
          handle_.ClearWeak();
          handle_->SetInternalField(0, v8::Undefined());
          handle_.Dispose();
          handle_.Clear();
        }
      }


      static inline T* Unwrap (v8::Handle<v8::Object> handle) {
        assert(!handle.IsEmpty());
        assert(handle->InternalFieldCount() > 0);
        return static_cast<T*>(handle->GetPointerFromInternalField(0));
      }


      v8::Persistent<v8::Object> handle_; // ro

     static v8::Persistent<v8::ObjectTemplate> handle_template; // ro

     protected:
      	inline void Wrap (v8::Handle<v8::Object> handle) {
        	assert(handle_.IsEmpty());
			assert(handle->InternalFieldCount() > 0);
			handle_ = v8::Persistent<v8::Object>::New(handle);
			handle_->SetPointerInInternalField(0, this);
			MakeWeak();
		}

	  	inline v8::Handle<v8::Object>WrapObject (){
			assert(!handle_template.IsEmpty());
			Handle<ObjectTemplate> templ = handle_template;

	        //Create the new handle to return, and set its template type
	        v8::Handle<v8::Object> result = templ->NewInstance();
			v8::Handle<v8::External> file_ptr = v8::External::New(this);

		    //Point the 0 index Field to the c++ pointer for unwrapping later
			result->SetInternalField(0, file_ptr);
			return result;
	  	}
	
		inline static T* UnwrapObject(const Handle<v8::Object> obj){
			Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
		  	void* ptr = field->Value();
			return static_cast<T*>(ptr);
		}

      inline void MakeWeak (void) {
        handle_.MakeWeak(this, WeakCallback);
      }

      /* Ref() marks the object as being attached to an event loop.
       * Refed objects will not be garbage collected, even if
       * all references are lost.
       */
      virtual void Ref() {
        assert(!handle_.IsEmpty());
        refs_++;
        handle_.ClearWeak();
      }

      /* Unref() marks an object as detached from the event loop.  This is its
       * default state.  When an object with a "weak" reference changes from
       * attached to detached state it will be freed. Be careful not to access
       * the object after making this call as it might be gone!
       * (A "weak reference" means an object that only has a
       * persistant handle.)
       *
       * DO NOT CALL THIS FROM DESTRUCTOR
       */
      virtual void Unref() {
        assert(!handle_.IsEmpty());
        assert(!handle_.IsWeak());
        assert(refs_ > 0);
        if (--refs_ == 0) { MakeWeak(); }
      }


      int refs_; // ro


     private:
      static void WeakCallback (v8::Persistent<v8::Value> value, void *data) {
        ObjectWrapT *obj = static_cast<ObjectWrapT*>(data);
        assert(value == obj->handle_);
        assert(!obj->refs_);
        assert(value.IsNearDeath());
        delete obj;
      }
    };
	template <typename T> v8::Persistent<v8::ObjectTemplate> ObjectWrapT<T>::handle_template;
}
#endif // object_wrap_h