import os

lib_target 	= "build/core"
src = os.path.abspath('src')
src_dir = src + "/%s"

headers = ["src/%s/" % name for name in os.listdir(src) if os.path.isdir(os.path.join(src, name))]
headers.insert(0, "/usr/local/v8")
headers.insert(0, "/usr/local/v8/include")
headers.insert(0, "/usr/local/lib")

lib_sources	= Glob("%s/*.cpp" % src_dir)

lib_paths = [
	'/usr/local/lib', 
	'/usr/local/v8', 
	'/usr/local/v8/include'
]

libs = [
	"libv8",
	"libevent",
]

flags = [
	"-Wall",
	"-pthread", 
	"-mmacosx-version-min=10.4", 
	"-pedantic",
]

env = Environment()

env.Append(LIBPATH=lib_paths, LIBS=libs)
env.Append(CPPPATH=headers)
env.Append(CPPFLAGS=flags)


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