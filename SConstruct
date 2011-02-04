env = Environment()

#src_dir = "/Users/wescope/Projects/wess/CoreJS/src/%s"
src_dir = "src/%s"

lib_target 	= "build/core"
lib_sources	= [
	src_dir % "/Directory/directory.cpp",
	src_dir % "/Env/env.cpp",
	src_dir % "/File/file.cpp",
	src_dir % "/Module/module.cpp",
	src_dir % "/Path/path.cpp",
	src_dir % "/Path/pathHistory.cpp",
	src_dir % "/System/system.cpp",
	src_dir % "/Events/BaseEvent.cpp",
	src_dir % "/Http/client.cpp",
	src_dir % "/Http/url.cpp",
	src_dir % "/Socket/clientSocket.cpp",
	src_dir % "/Socket/serverSocket.cpp",
	src_dir % "/Socket/Socket.cpp",
	src_dir % "/Threading/threadPool.cpp",
]

lib_headers = [
	"/usr/local/include/",
	"/usr/local/v8/include/",
	src_dir % "/",
	src_dir % "/Headers/",
	src_dir % "/Directory/",
	src_dir % "/Env/",
	src_dir % "/File/",
	src_dir % "/Module/",
	src_dir % "/Path/",
	src_dir % "/Path/",
	src_dir % "/System/",
	src_dir % "/Events/",
	src_dir % "/Http/",
	src_dir % "/Socket/",
	src_dir % "/Threading/",
]

libs = [
	"libv8_g",
	"libevent",
]

flags = [
	"-Wall",
	"-lpthread", 
	"-levent",
]

env.Append(CPPPATH=lib_headers)
env.Append(LIBS=libs)
env.Append(CPPFLAGS=flags)
env.Append(LIBPATH=['/usr/local/lib', '/usr/local/v8/'])


env.SharedLibrary(target=lib_target, source=lib_sources)