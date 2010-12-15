#ifndef event_test_H
#define event_test_H
#include "common.h"
#include <map>

using namespace v8;

class ThreadBase{
	public:
		
			int ready;
			std::string data;
			Persistent<Function> callback;
	      // two possible functions to call member function. virtual cause derived
	      // classes will use a pointer to an object and a pointer to a member function
	      // to make the function call
	      virtual std::string operator()()=0;  // call using operator
	      virtual std::string Call()=0;        // call using function
		};

template <typename T>
	class CallbackTemplate : public ThreadBase{
		public:
	
			/** THE ARGUMENT THAT WILL BE USED WHEN THE FUNCTION POINTER (see below) IS CALLED  **/

			T* obj; // pointer to object
			std::string (T::*fpt)();   // pointer to member function

			void setCallback(T* _obj, std::string(T::*_fpt)()){
				obj = _obj; 
				fpt = _fpt;
			}
			
			// override operator "()"
			virtual std::string operator()() { return (*obj.*fpt)();};              // execute member function

			// override function "Call"
			virtual std::string Call()
			{ 
				return (*obj.*fpt)();
			};

			CallbackTemplate(){ ready = 0;};
			~CallbackTemplate(){};

	};
	
#endif