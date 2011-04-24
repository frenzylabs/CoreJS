import os

lib_target 	= "build/core"
src = os.path.abspath('src')

headers = ["src/%s/" % name for name in os.listdir(src) if os.path.isdir(os.path.join(src, name))]

libs = [
	"libv8",
	"libevent",
]

flags = [
	"-Wall",
	"-pthread", 
]


env = Environment(ENV=os.environ)

env.Append(CPPPATH=headers)
env.Append(LIBS=libs)
env.Append(LIBPATH=['/usr/local/lib', '/usr/local/v8/'])
env.Append(CPPFLAGS=flags)


conf = Configure(env)

if not conf.CheckCXX():
	print('Compiler and/or environment is not configured correctly')
	Exit(0)

for lib in libs:
	 if not conf.CheckLib(lib):
		print "%s is required to build CoreJS" % lib
		Exit(1)

if not conf.CheckHeader('v8.h'):
	print "Cannot locate v8 (v8.h) header file"
	Exit(1)

env = conf.Finish()
env.SharedLibrary(target=lib_target, source=Glob('src/*.cpp'))