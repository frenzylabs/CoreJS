## NOTE: TO BUILD v8::
## do: sudo su && cd /usr/local
## Checkout from: svn checkout http://v8.googlecode.com/svn/trunk/ v8
## then (for debug) run: scons mode=debug library=shared
## once that is done, run: install_name_tool -id /usr/local/v8/libv8_g.dylib libv8_g.dylib

SRC_DIR = src
BUILD_TARGET = build/core
V8_DIR       = /usr/local/v8/libv8_g.dylib
V8_INC       = /usr/local/v8/include

SOURCES =   $(SRC_DIR)/main.cpp \
            $(SRC_DIR)/System/system.cpp \
            $(SRC_DIR)/Env/env.cpp \
            $(SRC_DIR)/Directory/directory.cpp \
				$(SRC_DIR)/File/file.cpp \
				$(SRC_DIR)/Http/url.cpp \
            $(SRC_DIR)/Path/path.cpp \
            $(SRC_DIR)/Path/pathHistory.cpp \
            $(SRC_DIR)/Http/client.cpp \
            $(SRC_DIR)/Module/module.cpp \
            $(SRC_DIR)/Events/BaseEvent.cpp \
            $(SRC_DIR)/Socket/Socket.cpp \
            $(SRC_DIR)/Socket/serverSocket.cpp \
            $(SRC_DIR)/Socket/clientSocket.cpp \
				$(SRC_DIR)/Threading/threadPool.cpp
         
HEADERS =   -I/usr/local/include \
				-I$(V8_INC) \
            -I$(SRC_DIR) \
            -I$(SRC_DIR)/Headers \
            -I$(SRC_DIR)/System \
            -I$(SRC_DIR)/Env \
            -I$(SRC_DIR)/Directory \
				-I$(SRC_DIR)/File \
            -I$(SRC_DIR)/Path \
            -I$(SRC_DIR)/Http \
            -I$(SRC_DIR)/Module \
            -I$(SRC_DIR)/Events \
            -I$(SRC_DIR)/Socket \
				-I$(SRC_DIR)/Threading 

LIBS = $(V8_DIR) \
		/usr/local/lib/libevent_pthreads.dylib \
		-L/usr/local/lib
         
TARGET = $(BUILD_TARGET)

OBJS = $(SOURCES:.cpp=.o)
CC=g++
CXXFLAGS= -arch i386 -Wall -lpthread -levent

all: $(OBSJ) 
	$(CC) $(CXXFLAGS) $(HEADERS) $(SOURCES) -o $(TARGET) $(LIBS)
clean:
	rm -rf build/*
