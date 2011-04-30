import os, platform

lib_target 	= "build/core"
src = os.path.abspath('src')
src_dir = src + "/%s"

headers = [
	"/usr/local/v8",
	"/usr/local/v8/include",
	'/usr/local/Cellar/libevent/2.0.10/include',
#	"/usr/local/lib"
]
headers.extend(["src/%s/" % name for name in os.listdir(src) if os.path.isdir(os.path.join(src, name))])
lib_sources = ["%s/%s" % (root, name) for root, dir, files in os.walk(src) for name in files if not name.startswith('main') and not name.endswith("os") and not name.endswith(".h")]

lib_paths = [
	'/usr/local/Cellar/libevent/2.0.10/lib',
#	'/usr/local/lib', 
	'/usr/local/v8', 
	'/usr/local/v8/include'
]

libs = [
	"libv8_g",
	"libevent",
	"libevent_pthreads"
]

flags = [
	"-Wall",
	"-pthread", 
	"-mmacosx-version-min=10.4", 
	"-pedantic",
]

env = Environment(ENV=os.environ)
env.Append(LIBPATH=lib_paths, LIBS=libs)
env.Append(CPPPATH=headers)
env.Append(CPPFLAGS=flags)

env.Depends(lib_target, 'libv8')
env.Depends(lib_target, 'libevent')


conf = Configure(env)
if not conf.CheckCXX():
	print('Compiler and/or environment is not configured correctly')
	Exit(0)

for lib in libs:
	 if not conf.CheckLib(lib):
		print "%s is required to build CoreJS" % lib
		Exit(1)

if not conf.CheckCXXHeader('v8.h'):
	print "Could not location lib v8.h"
	Exit(1)

env = conf.Finish()
env.SharedLibrary(target=lib_target, source=lib_sources)