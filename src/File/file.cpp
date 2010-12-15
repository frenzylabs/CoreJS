#include "file.h"
#include "Session.h"
#include "threadPool.h"

using namespace std;
using namespace v8;

namespace Core
{
	
	ThreadPool *filePool = ThreadPool::instance();
	
    // C/C++ Methods
	File::File():file(NULL),filepath(),filemode(){}
	File::~File() {}
	
	string File::readFileCb()
	{
		
		File *fp = this;
		if(fp->filepath == "" || fp->filemode=="")
			return "Cannot open file";
			
		if(fp->file == NULL){
			fp->file = fopen(fp->filepath.c_str(), fp->filemode.c_str());
		}
		fseek(fp->file, 0, SEEK_END);
        int size = ftell(fp->file);
        rewind(fp->file);

        char *chars = new char[size + 1];
        chars[size] = '\0';
        
        for(int i = 0; i < size;)
        {
            int read = fread(&chars[i], 1, size - i, fp->file);
            i += read;
        }
		return chars;
	}
	char * File::ReadFile(const char *name)
    {
        FILE *readFile = fopen(name, "rb");
        if(readFile == NULL)
            return NULL;
                    
        fseek(readFile, 0, SEEK_END);
        int size = ftell(readFile);
        rewind(readFile);
                
        char *chars = new char[size + 1];
        chars[size] = '\0';
                
        for(int i = 0; i < size;)
        {
            int read = fread(&chars[i], 1, size - i, readFile);
            i += read;
        }
                
        fclose(readFile);
               	

       return chars;
    }

    // Javascript Methods
    JS_METHOD(File::readFile)
	{
		HandleScope scope;
		ARG_LENGTH(1);
        String::Utf8Value filename(args[0]);    
        char *contents = (File::ReadFile(*filename));
        return scope.Close(String::New(contents, strlen(contents)));
    }

	JS_METHOD(File::writeFile)
	{
		HandleScope scope;
		ARG_LENGTH(2);
        String::Utf8Value filename(args[0]);
        String::Utf8Value contents(args[1]);
    
	    char *mode;
    
	    if(args[2]->IsUndefined())
	    {
	        mode = (char *)"w+";
	    }
	    else
	    {
	        String::Utf8Value utfMode(args[2]);
	        mode = *utfMode;
	    }
    
	    FILE *toFile;
	    toFile = fopen(*filename, mode);
    
	    if(toFile != NULL)
	    {
	        fwrite(*contents, sizeof(char), args[1]->ToString()->Utf8Length(), toFile);
	        fclose(toFile);
        
	        return scope.Close(v8::Boolean::New(true));
	    }
	    else
	    {
	        return scope.Close(THROW_ERROR("Could not open file"));
	    }
	}
	
	JS_METHOD(File::removeFile)
    {
		ARG_LENGTH(1);
        HandleScope scope;
		String::Utf8Value path(args[0]);
    
   	 	if(unlink(*path) < 0)
	        return scope.Close(THROW_ERROR("Cannot remove file"));
        
	    return scope.Close(v8::Boolean::New(true));
	}
		
	JS_METHOD(File::isFile)
	{
		/*
		HandleScope scope;
		ARG_LENGTH(1);
        String::Utf8Value filename(args[0]);
        struct stat sInfo;
        int res = stat(*filename, &sInfo);

        return scope.Close(v8::Boolean::New(res != -1 && S_ISREG(sInfo.st_mode)));
		*/
		return Undefined();
	}
		
    JS_METHOD(File::open)
    {
	//	ARG_LENGTH(2);
        HandleScope scope;

		/*** Unwrapping c++ pointer ***/
		File *fp = UnwrapObject(args.This());
		if(args.Length()< 2 && fp->filepath=="" && fp->filemode==""){			
			return scope.Close(THROW_EXCEPTION(Error, "Cannot open file"));
		}
		if(args.Length()==2){
			String::Utf8Value fpath(args[0]);
			String::Utf8Value fmode(args[1]);
			fp->filepath = static_cast<string>(*fpath);		
			fp->filemode = static_cast<string>(*fmode);
		}
		fp->file = fopen(fp->filepath.c_str(), fp->filemode.c_str());
		
		if(fp->file == NULL)
			return scope.Close(THROW_EXCEPTION(Error, "Cannot open file"));

        return scope.Close(v8::Boolean::New(1));
		
    }

	JS_METHOD(File::close)
	{
		
		HandleScope scope;
		/*** Unwrapping c++ pointer ***/
		File *fp = UnwrapObject(args.This());
		
		fclose(fp->file);
		
		return scope.Close(Undefined());
		
	}
	
	JS_METHOD(File::read)
	{
		HandleScope scope;
		
			
		/*** Unwrapping c++ pointer ***/
		File *fp = UnwrapObject(args.This());
	
		if(fp->filepath == "" || fp->filemode=="")
			return scope.Close(THROW_EXCEPTION(Error, "Cannot open file"));
			
		if(fp->file == NULL){
			fp->file = fopen(fp->filepath.c_str(), fp->filemode.c_str());
		}
		fseek(fp->file, 0, SEEK_END);
        int size = ftell(fp->file);
        rewind(fp->file);

        char *chars = new char[size + 1];
        chars[size] = '\0';
        
        for(int i = 0; i < size;)
        {
            int read = fread(&chars[i], 1, size - i, fp->file);
            i += read;
        }
        
        Handle<String> result = String::New(chars, size);
        
        delete[] chars;

		return scope.Close(result);
	}
	
	JS_METHOD(File::write)
	{
		HandleScope scope;
		
		ARG_LENGTH(1);
        String::Utf8Value data(args[0]);
		
		/*** Unwrapping c++ pointer ***/
		File *fp = UnwrapObject(args.This());
		
		if(fp->file == NULL){
			return scope.Close(THROW_EXCEPTION(Error, "No file specified"));
		}else{
        	fwrite(*data, sizeof(char), args[0]->ToString()->Utf8Length(), fp->file);
    	}

		return scope.Close(Undefined());
	}
	
	JS_METHOD(File::Remove)
	{
		HandleScope scope;
		
		/*** Unwrapping c++ pointer ***/
		File *fp = UnwrapObject(args.This());
		
		if(fp->filepath == ""){
			return scope.Close(THROW_EXCEPTION(Error, "No file specified to remove"));
		}
		if(unlink(fp->filepath.c_str())<0)
			return scope.Close(THROW_EXCEPTION(Error, "Cannot remove file"));
		return scope.Close(v8::Boolean::New(true));
	}
	
	JS_METHOD(File::Rewind)
	{
		HandleScope scope;
		
		/*** Unwrapping c++ pointer ***/
		File *fp = UnwrapObject(args.This());
		
		if(fp->file == NULL)
			return scope.Close(THROW_EXCEPTION(Error, "Cannot rewind file: No file specified."));
			
		rewind(fp->file);
		return scope.Close(v8::Boolean::New(true));
	}
	
    
	JS_METHOD(File::New)
	{
		HandleScope scope;
		
		if(args.Length()!=2 && args.Length()!=0){
			ThrowException(String::New("Invalid Argument Length"));
		}
		
		// Create the new instance of the c++ class File
		File *fp = new File();
		
		if(args.Length()==2){
			String::Utf8Value fpath(args[0]);
			String::Utf8Value fmode(args[1]);
			
			/***
			*	 Had to make the variables as strings.
			*   If I didnt then when I created new instances in JS code the other instances were getting
			*   overwritten.  
			***/
			fp->filepath = static_cast<string>(*fpath);		
			fp->filemode = static_cast<string>(*fmode);
			fp->file = fopen(fp->filepath.c_str(), fp->filemode.c_str());
		}
		
	    /**
		* Create the actual template object. 
		* Only need to do this once.  We'll store the object template in a persistent object.
		* This will allow us to create multiple instances of the object	
		* THE "handle_template" var is a Persistent<ObjectTemplate> and is declared in the ObjectWrap template;
		***/	
		if (handle_template.IsEmpty()) {
			v8::Handle<v8::FunctionTemplate> fpt = FunctionTemplate::New(); 
			fpt->SetClassName(String::New("File"));
			Handle<ObjectTemplate> raw_template = fpt->InstanceTemplate();
			raw_template->SetInternalFieldCount(1);
			
			handle_template = Persistent<ObjectTemplate>::New(raw_template);
			
			v8::Handle<v8::ObjectTemplate> protoTpl = fpt->PrototypeTemplate();


			protoTpl->Set(String::New("onReadCb"), FunctionTemplate::New(onReadCb));
			protoTpl->Set(String::New("open"), FunctionTemplate::New(open));
			protoTpl->Set(String::New("close"), FunctionTemplate::New(close));
			protoTpl->Set(String::New("read"), FunctionTemplate::New(read));
			protoTpl->Set(String::New("write"), FunctionTemplate::New(write));
			protoTpl->Set(String::New("remove"), FunctionTemplate::New(Remove));
			protoTpl->Set(String::New("rewind"), FunctionTemplate::New(Rewind));
			protoTpl->Set(String::New("getFilePath"), FunctionTemplate::New(getFilePath));
			protoTpl->Set(String::New("getFileMode"), FunctionTemplate::New(getFileMode));

		}
		
		return scope.Close(fp->WrapObject());
	}
	
	JS_METHOD(File::getFilePath)
	{
		HandleScope scope;
		/*** Unwrapping c++ pointer ***/
		File *fp = UnwrapObject(args.This());
			
		return scope.Close(String::New(fp->filepath.c_str(), fp->filepath.length()));
	}
	
	JS_METHOD(File::getFileMode)
	{
		HandleScope scope;
		/*** Unwrapping c++ pointer ***/
		File *fp = UnwrapObject(args.This());
		
		return scope.Close(String::New(fp->filemode.c_str(), fp->filemode.length()));
	}
	
	JS_METHOD(File::onReadCb)
	{
		HandleScope scope;
		/*** Unwrapping c++ pointer ***/
		File *fp = UnwrapObject(args.This());
	
		if(args.Length()==1){
			if(args[0]->IsFunction()){
				fp->callback = Persistent<Function>::New(Local<Function>::Cast(args[0]));
				fp->setCallback(fp, &File::readFileCb);
				filePool->dispatch(fp);
				return scope.Close(Undefined());
			}
		}
				//string data = File::readFileCb();
				//delete(nc);		

//		return scope.Close(String::New(data.c_str()));
		
		return scope.Close(Undefined());
		
		
	}
	
	void File::Init(Handle<ObjectTemplate> &global)
    {
		/**  This will create all the global functions and when a new File is created 
		  *  will create the default template for the File class then. 
		**/
		HandleScope scope;       
		global->Set(String::New("File"), FunctionTemplate::New(New));
		global->Set(String::New("readFile"), FunctionTemplate::New(readFile));
		global->Set(String::New("writeFile"), FunctionTemplate::New(writeFile));
		global->Set(String::New("removeFile"), FunctionTemplate::New(removeFile));
		global->Set(String::New("isFile"), FunctionTemplate::New(isFile));
    }


}
