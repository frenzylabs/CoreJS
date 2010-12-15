#ifndef __file_h__
#define __file_h__

#include "common.h"
#include <ObjectWrapT.h>
#include "ThreadBase.h"

using namespace v8;
namespace Core
{
	class File : Core::ObjectWrapT<File>, CallbackTemplate<File>
	{	
		protected:
			FILE *file;
			std::string filepath;
			std::string filemode;
		
		public:	
			File();
			~File();

			// C/C++ Methods
			std::string readFileCb();
			static char * ReadFile(const char *name);
			static void Init   (Handle<ObjectTemplate> &obj);


			JS_METHOD_DECLARE(onReadCb);

			// Javascript Methods
			JS_METHOD_DECLARE(open);
			JS_METHOD_DECLARE(close);
			JS_METHOD_DECLARE(read);
			JS_METHOD_DECLARE(write);
			JS_METHOD_DECLARE(Remove);
			JS_METHOD_DECLARE(Rewind);
			JS_METHOD_DECLARE(getFilePath);
			JS_METHOD_DECLARE(getFileMode);

			JS_METHOD_DECLARE(readFile);
			JS_METHOD_DECLARE(writeFile);
			JS_METHOD_DECLARE(removeFile);
			JS_METHOD_DECLARE(isFile);
			JS_METHOD_DECLARE(New);
	};
}


#endif