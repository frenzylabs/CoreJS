#ifndef __core_h__
#define __core_h__

#include "common.h"

// Core Modules
#include "system.h"
#include "env.h"
#include "directory.h"
#include "module.h"
#include "file.h"
// #include "client.h"
#include "path.h"
#include "ObjectWrapT.h"


#define CORE_JS_INIT(context)       Core::System::Init(context);      \
                                    Core::Env::Init(context);         \
                                    Core::Directory::Init(context);   \
                                    Core::File::Init(context);        \
                                    Core::Module::Init(context);

#endif